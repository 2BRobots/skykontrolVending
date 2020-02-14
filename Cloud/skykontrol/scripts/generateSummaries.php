<?php
include('../credentials.php');

if ($_SERVER["REMOTE_ADDR"] !== gethostbyname($hostname) AND !isset($_SERVER['SHELL'])) {
    echo "UNAUTHORIZED SCRIPT CALL";  
    exit();    
}

$conn = new mysqli($servername, $username, $password, $dbname);

if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

$items  = array();
$sql    = "SELECT `device_id` FROM `$dbname`.`controller_boards`;";
$result = $conn->query($sql);
if ($result->num_rows > 0) {
    while ($row = $result->fetch_assoc()) {
        $items[] = $row["device_id"];
    }
} else {
    echo "ERROR";
    $conn->close();
    exit();
}

$month  = date('m', strtotime('-1 month'));
$resume = array();

$itemsCount = count($items);
for ($i = 0; $i < $itemsCount; ++$i) {
    $device_id = $items[$i];
    $sql       = "SELECT sum(`price`) as `ammount`, count(*) as `sales` FROM `$dbname`.`sales` WHERE MONTH (`datetime`) = $month AND `device_id` = $device_id AND `method` = 'Cash';";
    $result    = $conn->query($sql);
    if ($result->num_rows > 0) {
        $row     = $result->fetch_assoc();
        $ammount = $row["ammount"];
        $sales   = $row["sales"];
        if ($sales != 0) {
            $resume[] = $device_id;
            $resume[] = $sales;
            $resume[] = $ammount;
        } else {
            $resume[] = $device_id;
            $resume[] = 0;
            $resume[] = 0;
        }
        $sql    = "SELECT sum(`kwh`) as `power`, count(*) as `logs` FROM `$dbname`.`power_meter` WHERE MONTH (`datetime`) = $month AND `device_id` = $device_id;";
        $result = $conn->query($sql);
        if ($result->num_rows > 0) {
            $row   = $result->fetch_assoc();
            $power = $row["power"];
            $logs  = $row["logs"];
            if ($logs != 0) {
                $resume[] = $power;
            } else {
                $resume[] = 0;
            }
        } else {
            echo "FAILED";
            $conn->close();
            exit();
        }
    } else {
        echo "FAILED";
        $conn->close();
        exit();
    }
}

$start_date = date('Y-m-01', strtotime('-1 month'));
$end_date   = date('Y-m-t', strtotime('-1 month'));

$resumeCount = count($resume);
for ($i = 0; $i < $resumeCount; $i += 4) {
    $device_id = $resume[$i];
    $sales     = $resume[$i + 1];
    $ammount   = $resume[$i + 2];
    $power     = $resume[$i + 3];
    if ($sales != 0 OR $ammount != 0 OR $power != 0) {
        $sql    = "INSERT INTO `$dbname`.`summaries` (`device_id`, `start_date`, `end_date`, `sales`, `ammount`, `power`) VALUES ('$device_id', '$start_date', '$end_date', '$sales', '$ammount', '$power');";
        $result = $conn->query($sql);
        if ($result !== TRUE) {
            echo "FAIL";
            $conn->close();
            exit();
        }
    }
}
echo "DONE";

$conn->close();
?>
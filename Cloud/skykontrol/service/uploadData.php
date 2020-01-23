<?php
include('../credentials.php');

function process_input($data)
{
    $data = trim($data);
    $data = stripslashes($data);
    $data = htmlspecialchars($data);
    return $data;
}

$conn = new mysqli($servername, $username, $password, $dbname);

if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $mac         = process_input($_POST["mac"]);
    $ssid        = process_input($_POST["ssid"]);
    $pass        = process_input($_POST["pass"]);
    $lat         = process_input($_POST["lat"]);
    $lng         = process_input($_POST["lng"]);
    $accuracy    = process_input($_POST["accuracy"]);
    $has_charLCD = process_input($_POST["has_charLCD"]);
    $rev         = process_input($_POST["rev"]);
    if ($lat == "NA" or $lng == "NA" or $accuracy == "NA") {
        echo "ERROR";
        $conn->close();
        exit();
    }
} else {
    echo "ERROR";
    $conn->close();
    exit();
}

if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

$sql    = "SELECT `device_id` FROM `$dbname`.`controller_boards` WHERE mac_addr='$mac' LIMIT 1;";
$result = $conn->query($sql);
if ($result->num_rows > 0) {
    $row       = $result->fetch_assoc();
    $device_id = $row["device_id"];
    $sql       = "UPDATE `$dbname`.`controller_boards` SET ssid = '$ssid', pass = '$pass', latitude = '$lat', longitude = '$lng', accuracy = '$accuracy', has_charLCD = $has_charLCD, firmware = '$rev' WHERE device_id='$device_id';";
    $result    = $conn->query($sql);
    if ($result === TRUE) {
        echo "PASS";
    } else {
        echo "FAIL";
    }
} else {
    echo "FAIL";
}

$conn->close();
?>
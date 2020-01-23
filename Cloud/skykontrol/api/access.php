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
    $id  = process_input($_POST["id"]);
    $psw = process_input($_POST["psw"]);
} else {
    echo "ERROR";
    exit();
}

if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

$sql    = "SELECT EXISTS (SELECT * FROM `$dbname`.`controller_boards` WHERE  user_id='$id' AND password='$psw');";
$result = $conn->query($sql);
if ($result->fetch_row()[0] == 1) {
    $sql    = "SELECT `enabled`, `mac_addr`,`has_charLCD`,`company`,`latitude`,`longitude`,`accuracy`, `firmware`, `manufacture_date` FROM `$dbname`.`controller_boards` WHERE  user_id='$id' LIMIT 1;";
    $result = $conn->query($sql);
    if ($result->num_rows > 0) {
        $row = $result->fetch_assoc();
        if ($row["enabled"] == 1) {
            session_start();
            $_SESSION["id"]               = $id;
            $mac                          = $row["mac_addr"];
            $_SESSION["mac"]              = $mac;
            $charLCD                      = $row["has_charLCD"];
            $_SESSION["charLCD"]          = $charLCD;
            $company                      = $row["company"];
            $_SESSION["company"]          = $company;
            $latitude                     = $row["latitude"];
            $_SESSION["latitude"]         = $latitude;
            $longitude                    = $row["longitude"];
            $_SESSION["longitude"]        = $longitude;
            $accuracy                     = $row["accuracy"];
            $_SESSION["accuracy"]         = $accuracy;
            $firmware                     = $row["firmware"];
            $_SESSION["firmware"]         = $firmware;
            $manufacture_date             = $row["manufacture_date"];
            $_SESSION["manufacture_date"] = date('d-m-Y', strtotime($manufacture_date));
            echo "PASS";
        } else {
            echo "DENI";
        }
    } else {
        echo "FAIL";
    }
} else {
    echo "FAIL";
}

$conn->close();
?>
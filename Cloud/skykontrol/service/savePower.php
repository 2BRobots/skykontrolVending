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
    $mac    = process_input($_POST["mac"]);
    $kwh    = process_input($_POST["kwh"]);
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
    $sql       = "INSERT INTO `$dbname`.`power_meter` (`device_id`, `kwh`) VALUES ('$device_id', '$kwh');";
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
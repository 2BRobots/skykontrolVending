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

$sql    = "DELETE FROM `$dbname`.`power_meter` WHERE `datetime` < NOW() - INTERVAL 35 DAY;";
$result = $conn->query($sql);
if ($result === TRUE) {
    echo "CLEANED";
} else {
    echo "FAILED";
}

$conn->close();
?>
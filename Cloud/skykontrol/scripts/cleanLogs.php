<?php
include('../credentials.php');

$conn = new mysqli($servername, $username, $password, $dbname);

if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

$sql    = "DELETE FROM `$dbname`.`controller_events` WHERE `datetime` < NOW() - INTERVAL 60 DAY;";
$result = $conn->query($sql);
if ($result === TRUE) {
    echo "CLEANED";
} else {
    echo "FAILED";
}

$conn->close();
?>
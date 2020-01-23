<?php
include('../credentials.php');

$conn = new mysqli($servername, $username, $password, $dbname);

if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

$sql    = "DELETE FROM `$dbname`.`resumes` WHERE `datetime` < NOW() - INTERVAL 3 YEAR;";
$result = $conn->query($sql);
if ($result === TRUE) {
    echo "CLEANED";
} else {
    echo "FAILED";
}

$conn->close();
?>
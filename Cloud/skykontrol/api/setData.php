<?php
include('../credentials.php');

session_start();

if (empty($_SESSION["id"])) {
    echo "ERROR";
    exit();
}

$id = $_SESSION["id"];

function process_input($data)
{
    $data = trim($data);
    $data = stripslashes($data);
    $data = htmlspecialchars($data);
    return $data;
}

$conn = new mysqli($servername, $username, $password, $dbname);

if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $cost    = process_input($_POST["cost"]);
    $time    = process_input($_POST["time"]);
    $state   = process_input($_POST["state"]);
    $message = process_input($_POST["message"]);
    $token   = process_input($_POST["token"]);
} else {
    echo "ERROR";
    exit();
}

if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

if ($state == "true") {
    $state = 1;
} else {
    $state = 0;
}

$sql    = "UPDATE `$dbname`.`controller_boards` SET `cost`='$cost', `time`='$time', `active`=b'$state', `message`='$message' WHERE  user_id='$id' LIMIT 1;";
$result = $conn->query($sql);
if ($result === TRUE) {
    echo "PASS";
} else {
    echo "FAIL";
}

$conn->close();
?>
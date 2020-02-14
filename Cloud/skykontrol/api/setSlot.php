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
    $slot       = process_input($_POST["slot"]);
    $product_id = process_input($_POST["product_id"]);
    $cost       = process_input($_POST["cost"]);
    $time       = process_input($_POST["time"]);
    $counter    = process_input($_POST["counter"]);
    $quantity   = process_input($_POST["quantity"]);
    $token      = process_input($_POST["token"]);
} else {
    echo "ERROR";
    exit();
}

if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

$sql    = "SELECT `device_id` FROM `$dbname`.`controller_boards` WHERE user_id='$id' LIMIT 1;";
$result = $conn->query($sql);
if ($result->num_rows > 0) {
    $row       = $result->fetch_assoc();
    $device_id = $row["device_id"];
    $sql       = "UPDATE `$dbname`.`controller_slots` SET `product_id`='$product_id', `cost`='$cost', `time`='$time', `counter`='$counter', `quantity`='$quantity' WHERE device_id='$device_id' AND slot='$slot' LIMIT 1;";
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
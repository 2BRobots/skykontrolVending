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
    $token = process_input($_POST["token"]);
    $slot  = process_input($_POST["slot"]);
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
    $sql       = "SELECT `product_id`,`cost`,`time`,`counter`,`quantity` FROM `$dbname`.`controller_slots` WHERE device_id='$device_id' AND slot='$slot' LIMIT 1;";
    $result    = $conn->query($sql);
    if ($result->num_rows > 0) {
        $row = $result->fetch_assoc();
        echo $row["product_id"] . "," . $row["cost"] . "," . $row["time"] . "," . $row["counter"] . "," . $row["quantity"];
    } else {
        echo "FAIL";
    }
} else {
    echo "FAIL";
}

$conn->close();
?>
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
} else {
    echo "ERROR";
    exit();
}

if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

$sql    = "SELECT `device_id` FROM `$dbname`.`controller_boards` WHERE  user_id='$id' LIMIT 1;";
$result = $conn->query($sql);
if ($result->num_rows > 0) {
    $row       = $result->fetch_assoc();
    $device_id = $row["device_id"];
    $sql       = "SELECT `start_date`, `end_date`, `power`, `sales`, `ammount` FROM `$dbname`.`summaries` WHERE  device_id='$device_id' ORDER BY `datetime` DESC;";
    $result    = $conn->query($sql);
    if ($result->num_rows > 0) {
        while ($row = $result->fetch_assoc()) {
            echo date('d-m-Y', strtotime($row["start_date"]))  . "," . date('d-m-Y', strtotime($row["end_date"])) . "," . $row["power"] . "," . $row["sales"] . ",$" . $row["ammount"] ."\n";
        }
    } else {
        echo "EMPTY";
    }
} else {
    echo "FAIL";
}

$conn->close();
?>
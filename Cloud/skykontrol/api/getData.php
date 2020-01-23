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

$sql    = "SELECT `active`,`cost`,`time`,`last_connection`,`message` FROM `$dbname`.`controller_boards` WHERE  user_id='$id' LIMIT 1;";
$result = $conn->query($sql);
if ($result->num_rows > 0) {
    $row = $result->fetch_assoc();
    echo date('d-m-Y', strtotime($row["last_connection"])) . " " . date('H:i:s', strtotime($row["last_connection"])) . "," . $row["message"] . "," . $row["cost"] . "," . $row["time"] . "," . $row["active"];
} else {
    echo "FAIL";
}

$conn->close();
?>
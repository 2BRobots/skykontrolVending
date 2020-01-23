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
    $mac = process_input($_POST["mac"]);
} else {
    echo "ERROR";
    exit();
}

if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

$sql    = "SELECT `enabled`,`active`,`cost`,`time`,`message` FROM `$dbname`.`controller_boards` WHERE mac_addr='$mac' LIMIT 1;";
$result = $conn->query($sql);
if ($result->num_rows > 0) {
    $row  = $result->fetch_assoc();
    $auth = ($row["active"] && $row["enabled"]);
    echo $row["cost"] . "," . $row["time"] . "," . (int) $auth . "," . $row["message"];
    $last   = date("Y-m-d H:i:s");
    $sql    = "UPDATE `$dbname`.`controller_boards` SET `last_connection`='$last' WHERE mac_addr='$mac' LIMIT 1;";
    $result = $conn->query($sql);
    if ($result === TRUE) {
        echo ",OK";
    } else {
        echo ",WR";
    }
} else {
    echo "FAIL";
}

$conn->close();
?>
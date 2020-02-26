<?php
include('../credentials.php');

$current = 6;

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
    $rev = process_input($_POST["rev"]);
} else {
    echo "ERROR";
    exit();
}

if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

$sql    = "SELECT EXISTS (SELECT * FROM `$dbname`.`controller_boards` WHERE mac_addr='$mac');";
$result = $conn->query($sql);
if ($result->fetch_row()[0] == 1) {
    if ($rev < $current) {
        echo "UPDATE";
    } else {
        echo "UPTODATE";
    }
} else {
    echo "FAIL";
}

$conn->close();
?>
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
    $mac  = process_input($_POST["mac"]);
    $slot = process_input($_POST["slot"]);
} else {
    echo "ERROR";
    exit();
}

if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

$sql    = "SELECT `device_id` FROM `$dbname`.`controller_boards` WHERE mac_addr='$mac' LIMIT 1;";
$result = $conn->query($sql);
if ($result->num_rows > 0) {
    $row       = $result->fetch_assoc();
    $device_id = $row["device_id"];
    $sql       = "SELECT `product_id`,`time`,`counter`,`cost`,`quantity`,`capacity` FROM `$dbname`.`controller_slots` WHERE device_id='$device_id' AND slot='$slot' LIMIT 1;";
    $result    = $conn->query($sql);
    if ($result->num_rows > 0) {
        $row        = $result->fetch_assoc();
        $product_id = $row["product_id"];
        $cost       = $row["cost"];
        $time       = $row["time"];
        $counter    = $row["counter"];
        $quantity   = $row["quantity"];
        $capacity   = $row["capacity"];
        $sql        = "SELECT `name`,`unit` FROM `$dbname`.`products` WHERE product_id='$product_id' LIMIT 1;";
        $result     = $conn->query($sql);
        if ($result->num_rows > 0) {
            $row  = $result->fetch_assoc();
            $name = $row["name"];
            $unit = $row["unit"];
            echo $product_id . "," . $name . "," . $unit . "," . $time . "," . $counter . "," . $cost . "," . $quantity . "," . $capacity;
        } else {
            echo "FAIL";
        }
    } else {
        echo "FAIL";
    }
} else {
    echo "FAIL";
}

$conn->close();
?>
<?php
session_start();

function generateRandomString($length = 10)
{
    $characters       = '0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ';
    $charactersLength = strlen($characters);
    $randomString     = '';
    for ($i = 0; $i < $length; $i++) {
        $randomString .= $characters[rand(0, $charactersLength - 1)];
    }
    return $randomString;
}

if (empty($_SESSION["id"])) {
    echo "<script type='text/javascript'>window.location.replace('login');</script>";
    exit();
}

readfile("uix/console.html");

$customer = $_SESSION["company"];
$mac = $_SESSION["mac"];

$token = generateRandomString();

echo "<html><script> var customer = '$customer'; var token = '$token'; customize(); var mac = '$mac';</script>";
echo '<script src="js/mqttws31.min.js" type="text/javascript"></script>';
echo '<script src="js/mqtt2br.js" type="text/javascript"></script>';
echo "<script>Connect();</script></html>";
?>

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

readfile("uix/main.html");

$powerMeter = $_SESSION["powerMeter"];
$datalogger = $_SESSION["datalogger"];
$slots      = $_SESSION["slots"];
$charLCD    = $_SESSION["charLCD"];
$customer   = $_SESSION["company"];

$token = generateRandomString();

echo "\n<html><script> var customer = '$customer'; var token = '$token'; var charLCD = '$charLCD'; var powerMeter = '$powerMeter'; var datalogger = '$datalogger'; var slots = '$slots'; obtain(); accessories(); customize(); </script></html>";
?>

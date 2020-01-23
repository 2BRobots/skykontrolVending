<?php
include('credentials.php');
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

readfile("uix/location.html");

$customer = $_SESSION["company"];
$lat      = $_SESSION["latitude"];
$lng      = $_SESSION["longitude"];
$rad      = $_SESSION["accuracy"];

if ($lat == "NA" or $lng == "NA" or $rad == "NA") {
    $lat = 0;
    $rad = 0;
    $lng = 0;
}

$token = generateRandomString();

echo "<html><script> var customer = '$customer'; var token = '$token'; customize(); lat = '$lat'; lng = '$lng'; rad = '$rad';</script></html>\n";
echo "<script async defer src='https://maps.googleapis.com/maps/api/js?key=$mapsAPIkey&callback=initMap'></script>";
?>
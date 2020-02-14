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

readfile("uix/summaries.html");

$mac      = $_SESSION["mac"];
$customer = $_SESSION["company"];
$firmware = $_SESSION["firmware"];
$madeDate = $_SESSION["manufacture_date"];

$token = generateRandomString();

echo "<html><script> var customer = '$customer'; var token = '$token'; customize(); var firmware = '$firmware'; var mac = '$mac'; var madeDate = '$madeDate'; printDetails(); getResumes(); </script></html>";
?>

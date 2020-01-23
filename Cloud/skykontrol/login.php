<?php
session_start();

if (!empty($_SESSION["id"])) {
    echo "<script type='text/javascript'>window.location.replace('main');</script>";
    exit();
}
readfile("uix/login.html");
?>
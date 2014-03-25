<?php 
$res['id'] = $_GET['id'];
$res['wsport'] = $_GET['wsport'];
$url = "http://localhost:";
$url.=$res['wsport'];
$url.="/";
$url.=$res['id'];
$url.="/pvaNode/get";
echo $url 
//$contents = file_get_contents($url); 
//echo $contents; 
?> 

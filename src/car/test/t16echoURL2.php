<?php
//$res['id'] = $_POST['id']; 
//$res['wsport'] = $_POST['wsport'];
$res['id'] =  $_GET['id'];
$res['wsport'] = $_GET['wsport'];
if($_GET['wsport']) echo "not NULL\n";
$url=":";
$url.=$res['id'];
$url.=$res['wsport'];
echo "in t16echoURL2.php\n";
echo "$url"; 
echo "why\n";
?> 

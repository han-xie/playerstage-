﻿<?php 
//$res['id'] = $_POST['id'];
header("Content-Type: text/html; charset=utf-8");
$res['wsport'] = $_POST['wsport'];
$url = "http://localhost:"; 
$url.=$res['wsport'];
$url.="/sim/pva/get?format=xml&tick=1";
$contents = file_get_contents($url); 
//如果出现中文乱码使用下面代码 
//$getcontent = iconv("gb2312", "utf-8",$contents); 
echo $contents; 
?>

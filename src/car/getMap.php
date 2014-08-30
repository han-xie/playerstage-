<?php 
header("Content-Type: text/html; charset=utf-8");
//$res['pre'] = $_POST['pre'];
//$res['userId'] = $_POST['userId'];
$res['wsport'] = $_POST['wsport'];

$url = "http://localhost:";
$url.=$res['wsport'];
/*
$url.="/";
$url.=$res['pre'];
$url.="/";
$url.=$res['userId'];
*/
$url.="/sim/attribute/get?format=xml"; 
$contents = file_get_contents($url); 
//如果出现中文乱码使用下面代码 
//$getcontent = iconv("gb2312", "utf-8",$contents); 
echo $contents;
//echo  $res['wsport'];
?> 

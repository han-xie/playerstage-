<?php 
$res['id'] = $_POST['id'];
$res['wsport'] = $_POST['wsport']; 
$url = "http://localhost:";
$url.=$res['wsport'];
$url.="/";
$url.=$res['id'];
$url.=$res['id'];
$url.="/pvaNodeTick/get"; 
$contents = file_get_contents($url); 
//如果出现中文乱码使用下面代码 
//$getcontent = iconv("gb2312", "utf-8",$contents); 
echo $contents; 
?> 

<?php 
$res['wsport'] = $_POST['wsport'];
$res['model']=$_POST['model'];
$url = "http://localhost:";
$url.=$res['wsport'];
$url.="/";
$url.=$res['model'];
$url.="/attribute/get?format=xml";
$contents .= file_get_contents($url); 
//如果出现中文乱码使用下面代码 
//$getcontent = iconv("gb2312", "utf-8",$contents); 
echo $contents; 
?> 

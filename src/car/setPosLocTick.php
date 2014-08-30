<?php 
//$res['id'] = $_POST['id'];
header("Content-Type: text/html; charset=utf-8");
$res['wsport'] = $_POST['wsport'];
$res['Name']=$_POST['Name'];
$res['px']=$_POST['px'];
$res['py']=$_POST['py'];
$url = "http://localhost:"; 
$url.=$res['wsport'];
$url.="/";
$url.=$res['Name'];
$url.="/pva/set?px=";
$url.=$res['px'];
$url.="&py=";
$url.=$res['py'];
$contents = file_get_contents($url); 
//如果出现中文乱码使用下面代码 
//$getcontent = iconv("gb2312", "utf-8",$contents); 
//echo $contents;
echo $url; 
?>

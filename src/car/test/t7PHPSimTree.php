<?php
//$xml = new SimpleXMLElement("datas.xml");
$url = "http://localhost:8000/sim/tree/get?format=xml";
$contents .= file_get_contents($url); 
//濡傛灉鍑虹幇涓枃涔辩爜浣跨敤涓嬮潰浠ｇ爜 
//$getcontent = iconv("gb2312", "utf-8",$contents); 
echo $contents; 


$doc   =   new   DOMDocument();
$doc-> load($contents);  
$xpath   =   new   DOMXPath($doc);

$nodes   =   $xpath-> evaluate('/ModelTree/Model');

echo 'test';
$node   =   $nodes-> item(0);
echo 'test2';
$new   =   $doc-> createElement('Model');
echo 'test3';
echo $nodes-> item(0)-> getAttribute('Name');
?>

<?php
//$xml = new SimpleXMLElement("datas.xml");

$doc   =   new   DOMDocument();
$doc-> load('t7.xml');  
$xpath   =   new   DOMXPath($doc);

$nodes   =   $xpath-> evaluate('/graph/set');
$node   =   $nodes-> item(0);
$new   =   $doc-> createElement('set');
if($nodes-> item(0)-> getAttribute('name')=='Jan'){
    
    //$nodes-> item(0)-> setAttribute('value', '192.168.0.181qqq');
    //$nodes-> item(0)-> setAttribute('color', '192.168.0.181qqq');
echo $nodes-> item(0)-> getAttribute('value');
}


$doc-> save( 'datas.xml ');


?>

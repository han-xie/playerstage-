<?php 
 $res['id'] = $_POST['id'];
$res['name'] = "elar";
$res['age'] = "21";
//$response = "hello this is response".$_POST['id'];
echo json_encode($res);
?>

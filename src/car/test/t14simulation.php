<?php
$res['fvalue'] = $_POST['fvalue'];
$res['svalue'] = $_POST['svalue'];

$userlable = "";
$playerport = "6665";
$httpport = "8000";
$envpath = "./";
$configfilename = "test.cfg";
$configfilepath .= $envpath+$userlable+$configfilename;
$command = "player";
$commandhead = "playerRun";
$otherPara = "-g";
$finalcommand .= $commandhead + " "+$command+ " "+$configfilepath+ " "+$otherPara +" -p "+$playerport+" -h "+$httpport;
echo $finalcommand;

/**************************************************
connect to server and send command using socket
**************************************************/
// 建立客户端的socet连接  
$socket = socket_create(AF_INET, SOCK_STREAM, SOL_TCP);  
$connection = socket_connect($socket, '127.0.0.1', 10013);    //连接服务器端socket  
if (!socket_write($socket, "$finalcommand")) {  
            echo "send command failed\n";  
}
$buf = 'This is my buffer.'; 
if (false !== ($bytes = socket_recv($socket, $buf, 2048, 0))) {
	//echo "$buf\n";
$token = strtok($buf," ");
if($token !== false) $playerport=$token;
$token = strtok(" "); 
if($token !== false) $httpport=$token;
} else {
    echo "socket_recv() failed; reason: " . socket_strerror(socket_last_error($socket)) . "\n";
}  
socket_close ($socket ) ;

/***********************************************
jump to simulation url + send some data
************************************************/
//header('Location: http://localhost/car/test/t16echoURL2.php?id=1&wsport=2');
?> 

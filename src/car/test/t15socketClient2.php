<?php
if($_POST['userid'])  $userlable=$_POST['userid'];
else {$userlable = "";}
if($_POST['playerport']) $playerport=$_POST['playerport'];
else {$playerport = "6665";}
if($_POST['httpport'])  $httpport=$_POST['httpport'];
else {$httpport = "8000";}
if($_POST['conffilename']) $configfilename=$_POST['conffilename'];
else {$configfilename = "multiLaser.test3.cfg";}

$envpath = "./";
$configfilepath = $envpath;
$configfilepath .=$userlable.$configfilename;
$command = "player";
$commandhead = "playerRun";
$otherPara = "-g";
$finalcommand = $commandhead;
$finalcommand .=" ".$command." ".$configfilepath." ".$otherPara." -p ".$playerport." -h ".$httpport."\n";
echo $finalcommand;

  
// 建立客户端的socet连接  
$socket = socket_create(AF_INET, SOCK_STREAM, SOL_TCP);  
$connection = socket_connect($socket, '127.0.0.1', 10013);    //连接服务器端socket  
  
/*while ($buffer = @socket_read($socket, 1024, PHP_NORMAL_READ)) {  
    //服务端告诉客户端，自己的状态  
    if (preg_match("/not connect/",$buffer)) {  
        echo "don`t connect\n";  
        break;  
    } else {  
        //服务器传来信息  
        echo "Buffer Data: " . $buffer . "\n";  
  
        echo "Writing to Socket\n";  
        // 将客户的信息写到通道中，传给服务器端  
        if (!socket_write($socket, "SOME DATA\n")) {  
            echo "Write failed\n";  
        }  
        //服务器端收到信息后，给于的回应信息  
        while ($buffer = socket_read($socket, 1024, PHP_NORMAL_READ)) {  
                echo "sent to server: SOME DATA\n response from server was:" . $buffer . "\n";  
        }         
  
    }  
}*/ 

if (!socket_write($socket, "playerRun player multiLaser.test3.cfg -g -p 6665 -h 8020\n")) {  
            echo "Write failed\n";  
}  
else{
	echo "send success\n";
} 

$buf = 'This is my buffer.';
 
if (false !== ($bytes = socket_recv($socket, $buf, 2048, 0))) {
    echo " $bytes \n";
	echo "$buf\n";
$token = strtok($buf," ");
if($token !== false) $playerport=$token;
$token = strtok(" ") ;
if($token !== false) $httpport=$token;
echo "$playerport\n";
echo "$httpport\n";
} else {
    echo "socket_recv() failed; reason: " . socket_strerror(socket_last_error($socket)) . "\n";
}  
  
echo "close\n";

socket_close ($socket ) ;
  
?> 

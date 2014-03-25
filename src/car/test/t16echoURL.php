<?php 
echo "hi\n";
//$url = "http://localhost/car/test/t16echoURL2.php?id=1&wsport=2";
//$contents = file_get_contents($url); 
//echo "$url?id=1&wsport=2";
$temp = "2";
header("Location: http://localhost/car/test/t16echoURL2.php?id=1&wsport=$temp");
echo "end\n"; 
?> 

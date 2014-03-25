<?php
//创建一个数组，用来判断上传来的图片是否合法
$uptypes    = array(
                    'image/jpg',
                    'image/jpeg',
                    'image/png',
                    'image/pjpeg',
                    'image/gif',
                    'image/bmp',
                    'image/x-png'
                );
$files     = $_FILES["uppic"];
if($files["size"] > 2097152){
    echo "上传图片不能大于2M";
    exit;
}
$ftype    = $files["type"];
if(!in_array($ftype,$uptypes)){
    echo "上传的图片文件格式不正确";
    exit;
}
$fname = $files["tmp_name"];
//$image_info    = getimagesize($fname);

$name    = $files["name"];
$str_name    = pathinfo($name);
$extname    = strtolower($str_name["extension"]);
$upload_dir    = "uploadpics/";
$file_name    = date("YmdHis").rand(1000,9999).".".$extname;
$str_file    = $upload_dir.$file_name;
if(!file_exists($upload_dir)){
    mkdir($upload_dir);
}
if(!move_uploaded_file($files["tmp_name"],$str_file)){
    echo "图片上传失败";
    exit;
}
else{
    echo "图片上传成功"."<br>";    
}

//调整上传图片的大小
$width=75;  
$height=90;  
$size=getimagesize($str_file);  
if($size[2]==1)
$im_in=imagecreatefromgif($str_file);   
if($size[2]==2) 
$im_in=imagecreatefromjpeg($str_file);   
if($size[2]==3)
$im_in=imagecreatefrompng($str_file);  
$im_out=imagecreatetruecolor($width,$height);  
imagecopyresampled($im_out,$im_in,0,0,0,0,$width,$height,$size[0],$size[1]);  
imagejpeg($im_out,$str_file);
chmod($str_file,0777);  
imagedestroy($im_in);  
imagedestroy($im_out);

echo "<script>window.parent.location.href=('index.php?imgpath=".$str_file."');</script>"; 
echo "<script>window.close();</script>";
?>
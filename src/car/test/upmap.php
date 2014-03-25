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
$upload_dir    = "map/";
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

echo "<script>window.parent.location.href=('result.html?imgpath=".$str_file."');</script>"; 
echo "<script>window.close();</script>";
?>
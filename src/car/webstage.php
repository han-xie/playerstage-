<!doctype html>
        <head>
        		<meta charset="utf-8">
                <title>webstage v1.0</title>
                <style type="text/css">

					.map {
						width:480px;
						height:480px;
						MARGIN-RIGHT: auto;
						MARGIN-LEFT: auto;
						
						
						-moz-background-size: 100% 100%;
						-o-background-size: 100% 100%;
						-webkit-background-size: 100% 100%;
						background-size: 100% 100%;
						-moz-border-image: url('pic/cave_compact.png') 0;
						background-repeat:no-repeat\9;
						background-image:none\9;
						filter:progid:DXImageTransform.Microsoft.AlphaImageLoader(src='pic/cave_compact.png', sizingMethod='scale')\9;
					}
					
                </style>
                <!--  link href="style.css" rel="stylesheet" type="text/css" /-->
                <!--link href="car.js" rel="stylesheet" type="text/javascript" /-->  

<?php
//$tfurl = $_GET['httpport'] ;
$tfurl = "8000";
//$imageUrl = $_GET['map'];
$pre="car";
$userId="worlds1";
?>

    <script type="text/javascript">
        var time=1.0;        
        var xmlHttp=null;
        
        var paused=true;					///< control the siumulation start or pause
        var scale=60;						///< the rate of the map scale
        
		var model=new Array();				///< model,which its name can be like "cave","sink" or "source"  
		var modelMap=new Array();			///< the bitmap of the model
		var modelWidth=new Array();			///< the width of the model
		var modelHeight=new Array();		///< the height of the model
		var modelX=new Array();				///< the initial location(x) model
		var modelY=new Array();				///< the initial location(y) model
		
		var modelColor_R=new Array();		///< the color(R) of the model will be used if its bitmap none
		var modelColor_G=new Array();		///< the color(G) of the model will be used if its bitmap none
		var modelColor_B=new Array();		///< the color(B) of the model will be used if its bitmap none
		
		var position=new Array();			///< the name of position(the small car)
		var pos_bitmap=new Array();			///< the map of the position
		var pos_width=new Array();			///< the width of the position
		var pos_height=new Array();			///< the height of the position
        var pos_location_x=new Array();		///< the initial location(x) of the position(the small car)
		var pos_location_y=new Array();		///< the initial location(y) of the position(the small car)
		var pos_location_z=new Array(); 	///< the rotation of the small car
		
		var pos_color_R=new Array();		///< the color(R) of the position(the small car) will be used if its bitmap none 
		var pos_color_G=new Array();		///< the color(G) of the position(the small car) will be used if its bitmap none 
		var pos_color_B=new Array();		///< the color(B) of the position(the small car) will be used if its bitmap none  
		var car_left=new Array();			///< recode the car distance(left)
		var car_top=new Array();			///< recode the car distance(top)
		///the width(mapX) and height(mapY) of the div(id:map)
		var mapX;
		var mapY;
		
		var mapLeft=0;
		var newDis=20;
		var pos_dis=new Array();			///< distance from the last car 
		
        var timer;
        var simtime=100;
		var counter_s=counter_l=0;
		var sound_flag=new Array();
		var sound_content=new Array();
		var sound_left=new Array();			///< recode the sound(div) distance(left)
		var sound_top=new Array();			///< recode the sound(div) distance(top)
		
		var lcd_flag=new Array();
		var lcd_content=new Array();
		var lcd_left=new Array();			///< recode the lcd(div) distance(left)
		var lcd_top=new Array();			///< recode the lcd(div) distance(top)
		
		var webserverport="<?php echo $tfurl ?>";   
		var pre="<?php echo $pre ?>";
		var userId="<?php echo $userId ?>";
		window.onload = function(){
    		//alert("0000");
			newxmlHttpRequest();
			getMap();
			for(var s=0;s<position.length;++s)
			{
				sound_flag[s]=false;
				lcd_flag[s]=false;
				sound_content[s]="null";
				lcd_content[s]="null";
			}
        	drawMap();
			drawCar();	
			simulate();	
			for(var i=0;i<position.length;++i)
				drag(i);
        }
		///make the text to xml
		function createXml(str){ 
　　			if(document.all){ 
				//if the browser is IE
				//alert("The browser is IE.");
　				var xmlDom=new ActiveXObject("Microsoft.XMLDOM") 
　　				xmlDom.loadXML(str) 
　　				return xmlDom 
　　			} 
　			else{
				//alert("The browser is not IE");
　				return new DOMParser().parseFromString(str, "text/xml");
			}
　　		}
		
		///function dfabs(x){return  (x < 0) ? -x : x;  }
		///First:create the xmlHttpRequest's object whateven your browser is.
		function newxmlHttpRequest(){
			if (window.XMLHttpRequest){
			   	// code for IE7, Firefox, Opera, etc.
  				xmlHttp=new XMLHttpRequest();
  			}
			else if (window.ActiveXObject){
				// code for IE6, IE5
 				xmlHttp=new ActiveXObject("Microsoft.XMLHTTP");
  			}
			
        }
        
		///Second:get the file path,the file which includes the infomation of all maps 
		function getMap(){
	        if(xmlHttp!=null){
				//var url="http://192.168.1.113:"+webserverport+"/sim/tree/get";
	        	xmlHttp.onreadystatechange = getMap_change;
	        	xmlHttp.open("POST", "getMap.php", false);
				xmlHttp.setRequestHeader("Content-Type","application/x-www-form-urlencoded");
				//xmlHttp.send("id="+model);
				xmlHttp.send("wsport="+webserverport);
	        }
        }

		///Second_bind:to get the map respectively through the relative path	
		function getMap_change(){
			if(xmlHttp.readyState==4){
				var getMap_xmlDoc = createXml(xmlHttp.responseText);
				var getMap_model=getMap_xmlDoc.getElementsByTagName("Model");
				var modelPose="";
				var pose="";
				var j=p=0;					
				for(var i=0;i<getMap_model.length;i++){
					if(getMap_model[i].getAttribute("modeltype")=="model"){
						model[j]=getMap_model[i].getAttribute("Name");
						//alert("cave?"+j+"="+model[j]);
						modelMap[j]=getMap_model[i].getAttribute("bitmap");
						modelWidth[j]="";
						modelHeight[j]="";
						var modelSize=getMap_model[i].getAttribute("geometry");
						//alert("modelsize:"+modelSize);
						var width_end=false;
						for(var l=0;l<modelSize.length;++l)
							if(modelSize[l]!=',')
								if(width_end==false)	
									modelWidth[j] +=modelSize[l];		
								else			
									modelHeight[j] +=modelSize[l];
							else{
								if(width_end==false){
									width_end=true;
									continue;
								}
								else
									break;
							}
								
						
						modelX[j]="";
						modelY[j]="";
						modelPose=getMap_model[i].getAttribute("pose");
						var x_end=false;
						for(var l=0;l<modelPose.length;++l)
							if(modelPose[l]!=',')
								if(x_end==false)	
									modelX[j] +=modelPose[l];								
								else
									modelY[j] +=modelPose[l];								
							else{
								if(x_end==false){
									x_end=true;
									continue;
								}
								else
									break;
							}
						var modelColor=getMap_model[i].getAttribute("color");
						modelColor_R[j]="";
						modelColor_G[j]="";
						modelColor_B[j]="";
						var R_end=false;
						var G_end=false;
						//var B_end=false;
						for(var l=0;l<modelColor.length;++l)
							if(modelColor[l]!=',')
								if(R_end==false)									
									modelColor_R[j] +=modelColor[l];									
								else if(G_end==false)									
									modelColor_G[j] +=modelColor[l];
								else									
									modelColor_B[j] +=modelColor[l];
							else
								if(R_end==false)
									R_end=true;
								else if(G_end==false)
									G_end=true;
								else 
									break;
						j++;
					}
					if(getMap_model[i].getAttribute("modeltype")=="position")
					{
						position[p]=getMap_model[i].getAttribute("Name");
						pos_bitmap[p]=getMap_model[i].getAttribute("bitmap");
						pos_width[p]="";
						pos_height[p]="";
						var pos_size=getMap_model[i].getAttribute("geometry");
						var width_end=false;
						for(var l=0;l<pos_size.length;++l)
							if(pos_size[l]!=',')
								if(width_end==false){									
									pos_width[p] +=pos_size[l];
								}
								else{								
									pos_height[p] +=pos_size[l];
								}
							else{
								if(width_end==false){
									width_end=true;
									continue;
								}
								else
									break;
							}
						
						
						var pos_location=getMap_model[i].getAttribute("pose");
						//alert("car pos:"+pos_location);
						pos_location_x[p]="";
						pos_location_y[p]="";
						pos_location_z[p]="";
						var x_end=false;
						var y_end=false;
						var init_z_start=0;
						for(var l=0;l<pos_location.length;++l){
							if(pos_location[l]!=','){
								if(x_end==false)									
									pos_location_x[p] +=pos_location[l];
								else if(y_end==false)								
									pos_location_y[p] +=pos_location[l];
								else if(init_z_start==3)									
									pos_location_z[p] +=pos_location[l];
							}
							else{
								if(x_end==false)
									x_end=true;
								else if(y_end==false)
									y_end=true;
								else
									init_z_start++;
							}
						}
						var pos_color=getMap_model[i].getAttribute("color");
						pos_color_R[p]="";
						pos_color_G[p]="";
						pos_color_B[p]="";
						var R_end=false;
						var G_end=false;
						//var B_end=false;
						for(var l=0;l<pos_color.length;++l)
							if(pos_color[l]!=',')
								if(R_end==false)									
									pos_color_R[p] +=pos_color[l];									
								else if(G_end==false)									
									pos_color_G[p] +=pos_color[l];
								else								
									pos_color_B[p] +=pos_color[l];
							else
								if(R_end==false)
									R_end=true;
								else if(G_end==false)
									G_end=true;
								else 
									break;
						p++;
					}
				}	
			}
		}
		
		///Third:draw the map
		 function drawMap(){			
		  	var maxModel=0;
			var maxModel_flag=false;
			var maxModel_index=new Array();		//store the map-size-order from big to small
			var width=new Array();
			for(var i=0;i<modelWidth.length;++i)
				width[i]=parseFloat(modelWidth[i]);
			for(var j=0;j<width.length;++j){
				for(var i=0;i<width.length;++i){
					for(var p=0;p<maxModel_index.length;++p){
						if(i==maxModel_index[p]){
							maxModel_flag=true;
							break;
						}
					}
					if(maxModel_flag==false){
						 if(width[i]>width[maxModel])
						 	maxModel=i;
					}
					else{
						maxModel_flag=false;
						continue;
					}
				}
				maxModel_index[j]=maxModel;
				maxModel=0;
				for(var i=0;i<modelWidth.length;++i)
					for(var p=0;p<maxModel_index.length;++p){
						if(maxModel==maxModel_index[p])
								maxModel++;
				}
			}	
          	var dmap = document.getElementById("map");
            dmap.style.backgroundImage = "url(../"+pre+"/"+userId+"/"+modelMap[maxModel_index[0]]+")";
	
			mapX=parseFloat(modelWidth[maxModel_index[0]])/2.0;
			mapY=parseFloat(modelHeight[maxModel_index[0]])/2.0;
			scale=250/mapX;					
			dmap.style.width=scale*2*mapX+"px";		
			dmap.style.height=scale*2*mapY+"px";
			
			
			for(var j=1;j<maxModel_index.length;++j){
				//alert("drawMap-for start:");
				
				var newNode=document.createElement("div");
				newNode.setAttribute("id","model"+maxModel_index[j]);
				
				var modX=parseFloat(modelX[maxModel_index[j]]);
				var modY=parseFloat(modelY[maxModel_index[j]]);			
				var widX=parseFloat(modelWidth[maxModel_index[j]]);
				var heiY=parseFloat(modelHeight[maxModel_index[j]]);
				
				var left=scale*(mapX+modX-(widX/2.0))+"px";
				var top=(scale*(mapY-modY-(heiY/2.0))-newDis)+"px";				
				newDis =scale*heiY+scale*(mapY-modY-(heiY/2.0));
				//alert("top:"+top+"top="+parseFloat(top));
				//newDis =scale*heiY+parseFloat(top);
				
				var bac_image="../"+pre+"/"+userId+"/"+modelMap[maxModel_index[j]];
					
				
				var style1="width:"+scale*widX+"px;height:"+scale*heiY+"px;margin-left:"+left+";margin-top:"+top+";background:url("+bac_image+") no-repeat";				
				var style2="width:"+scale*widX+"px;height:"+scale*heiY+"px;margin-left:"+left+";margin-top:"+top+";background-color:rgb("+parseFloat(modelColor_R[j])*255+","+parseFloat(modelColor_G[j])*255+","+parseFloat(modelColor_B[j])*255+")";
							
				if(modelMap[maxModel_index[j]]!="")
					newNode.setAttribute("style",style1);
				else
					newNode.setAttribute("style",style2);
				
				document.getElementById("map").appendChild(newNode);
				
				
			}
			
		}
		
		///Forth:draw the small car
		function drawCar(){
			pos_dis[0]=newDis;
			for(var j=0;j<position.length;++j){
				
				var newNode=document.createElement("div");				
				var posX=parseFloat(pos_location_x[j]);				
				var posY=parseFloat(pos_location_y[j]);
				var widX=parseFloat(pos_width[j]);				
				var heiY=parseFloat(pos_height[j]);
				car_left[j]=scale*(mapX+posX-(widX/2.0));
				car_top[j]=scale*(mapY-posY-(heiY/2.0))-pos_dis[j];
				var left=scale*(mapX+posX-(widX/2.0))+"px";
				//alert(pos_dis[j]+":"+mapY+":"+posY+":"+heiY);
				var top=(scale*(mapY-posY-(heiY/2.0))-pos_dis[j])+"px";
				pos_dis[j+1] =scale*heiY+scale*(mapY-posY-(heiY/2.0));
				//pos_dis[j+1] =scale*heiY+parseFloat(top);
				//alert(pos_dis[j+1]);
				//alert("top="+top);
				bac_image="../"+pre+"/"+userId+"/"+pos_bitmap[j];
				newNode.setAttribute("id","car"+j);
				if(pos_bitmap[j]!="")
					newNode.setAttribute("style","width:"+scale*widX+"px;height:"+scale*heiY+"px;margin-left:"+left+";margin-top:"+top+";background:url("+bac_image+");background-repeat:no-repeat");
				else
					newNode.setAttribute("style","width:"+scale*widX+"px;height:"+scale*heiY+"px;margin-left:"+left+";margin-top:"+top+";background-color:rgb("+parseFloat(pos_color_R[j])*255+","+parseFloat(pos_color_G[j])*255+","+parseFloat(pos_color_B[j])*255+")");
					//newNode.setAttribute("background-color",modelColor[maxModel_index[j]]);*/
				document.getElementById("map").appendChild(newNode);
				
				var lcdNode=document.createElement("div");
				var lcdLeft=parseFloat(left)+scale*widX;				
				lcdNode.setAttribute("id","lcd"+j);
				//lcdNode.setAttribute("style","display:none;position:absolute;left:0px;top:0px");
				lcdNode.setAttribute("style","display:none;width:100px;height:108px;margin-left:"+lcdLeft+"px;margin-top:-"+2*scale*heiY+"px");
				//lcdNode.innerHTML="lcd!!";
				document.getElementById("map").appendChild(lcdNode);
				
				
				
				var soundNode=document.createElement("div");
				var soundLeft=parseFloat(left)+scale*widX;				
				soundNode.setAttribute("id","sound"+j);
				soundNode.setAttribute("style","display:none;margin-left:"+soundLeft+"px;margin-top:-"+scale*heiY+"px");
				document.getElementById("map").appendChild(soundNode);
				
				
				var soundImg=document.createElement("img");
				soundImg.setAttribute("id","soundImg"+j);
				soundImg.src="../user/fcc/bitmaps/sound.png";		
				document.getElementById("sound"+j).appendChild(soundImg);
			}
		}
		
		///Fifth:start simulating,the small car(position) in the map will move or rotate!
		function simulate(){
        if(simtime == 0) simtime=100;
            timer = setInterval(function(){  
			for(var s=0;s<position.length;++s)
			{
				if(sound_flag[s]==true)
					counter_s++;
				if(lcd_flag[s]==true)
					counter_l++;
			}
				getAllPVANodeSim();
               	for(var i=0;i<position.length;i++){
                	move(i);
                	rotate(i);
                }
            }, simtime);
        }
		function endsimulate(){
        	clearInterval(timer);
        }       
		function getAllPVANodeSim(){
        	if(xmlHttp!=null){
        		xmlHttp.onreadystatechange = pvaAll_Change;
        		xmlHttp.open("POST", "getAllPVANodeTick.php", false);
        		xmlHttp.setRequestHeader("Content-Type","application/x-www-form-urlencoded");
        		//xmlHttp.send(null);
				xmlHttp.send("wsport="+webserverport);
				
        	}
        }
		function pvaAll_Change(){
        	if (xmlHttp.readyState == 4) {
				var str = xmlHttp.responseText;
				document.getElementById('show').innerHTML = str;
				//alert(str);
				var getPva_xmlDoc = createXml(xmlHttp.responseText);
				var getPva_model = getPva_xmlDoc.getElementsByTagName("Model");
				var p=0;
				for(var i=0;i<getPva_model.length;++i){
					if(getPva_model[i].getAttribute("modeltype")=="position"){
						var pva_child=getPva_model[i].childNodes;
						var child_pva=getPva_model[i].firstChild;
						//alert("pva:"+pva_child.length);
						for(var m=0;m<pva_child.length;++m)
						{
							if (child_pva.nodeType==1 && child_pva.getAttribute("modeltype")=="Sound")
  							{  										
								for(var s=0;s<position.length;++s)
								{
									if(position[s]==getPva_model[i].getAttribute("Name"))
									{
										if(child_pva.getAttribute("content")!=sound_content[s])
										{
											//alert("sss");
											
											sound_content[s]=child_pva.getAttribute("content");
											
											sound_flag[s]=true;
											
											break;
										}
									}
								}															
 							}
							if (child_pva.nodeType==1 && child_pva.getAttribute("modeltype")=="LCD")
  							{
  										
								for(var l=0;l<position.length;++l)
								{
									if(position[l]==getPva_model[i].getAttribute("Name"))
									{
										if(child_pva.getAttribute("content")!=lcd_content[l] && child_pva.getAttribute("content")!="0 ")
										{
											lcd_content[l]="";
											var str=child_pva.getAttribute("content");
											var s;
											for(var g=0;g<str.length;++g)
											{
												if(str[g]==' ')
												{
													s=g+1;
													break;
												}
											}
											for(var f=s;f<str.length;++f)
												lcd_content[l] +=str[f];
											lcd_content[l]=lcd_content[l].substr(0,lcd_content[l].length-2);
											lcd_flag[l]=true;
											break;
										}
									}							
								}
 							}
							child_pva=child_pva.nextSibling;
						}
						time=parseFloat(getPva_model[i].getAttribute("Time"));
						var pos_location=getPva_model[i].getAttribute("pose");
						var x_end=false;
						var y_end=false;
						var get_indexZ=0;
						pos_location_x[p]="";
						pos_location_y[p]="";
						pos_location_z[p]="";
						for(var l=0;l<pos_location.length;++l)
							if(pos_location[l]!=','){
								if(x_end==false){
									pos_location_x[p] +=pos_location[l];
								}
								else if(y_end==false){
									pos_location_y[p] +=pos_location[l];
								}
								else if(get_indexZ==3){
									pos_location_z[p] +=pos_location[l];
								}
							}
							else{
								if(x_end==false){
									x_end=true;
									continue;
								}
								else if(y_end==false)
									y_end=true;
								else{
									get_indexZ++;
								}
							}
						p++;
					}
				}
				for(var s=0;s<position.length;++s)
				{
					var sound=document.getElementById("sound"+s);
					var lcd=document.getElementById("lcd"+s);
					if(sound_flag[s]==true)
					{
						if(counter_s%5==0){
							sound.style.display =(sound.style.display == "none") ? "block" : "none";
						}
						if(counter_s>26)
						{
							sound_flag[s]=false;
							counter_s=0;
						}
					}
					if(lcd_flag[s]==true)
					{						
						if(counter_l<50){
							lcd.innerHTML="<textarea style='width:100px;height:50px'>"+lcd_content[s]+"</textarea>";
							lcd.style.wordBreak="break-all";
							lcd.style.display ="block";
						}
						else
						{
							lcd.style.display ="none";
							lcd_flag[s]=false;
							counter_l=0;
						}
					}
				}
			}
        }
		///Seventh_bind_second:car moving
		function move(i){
				var element = document.getElementById("car"+i);
				var sound_ele=document.getElementById("sound"+i);
				var lcd_ele=document.getElementById("lcd"+i);
				var posX=parseFloat(pos_location_x[i]);
				var posY=parseFloat(pos_location_y[i]);
				var widX=parseFloat(pos_width[i]);
				var heiY=parseFloat(pos_height[i]);
				car_left[i]=scale*(mapX+posX-(widX/2.0));
				car_top[i]=scale*(mapY-posY-(heiY/2.0))-pos_dis[i];
				var left=car_left[i]+"px";
				var top=car_top[i]+"px";
				sound_left[i]=(car_left[i]+scale*widX)+"px";
				//sound_top[i]=(-0.6*scale*heiY)+"px";
				lcd_left[i]=(car_left[i]+scale*widX)+"px";
				lcd_top[i]=(-2*scale*heiY)+"px";
				
				sound_top[i]=(-scale*heiY)+"px";
				/*
				if(element.style.position=="absolute"){
					//alert("absolute.");
					element.style.left=(parseFloat(left)+mapLeft)+"px";
					element.style.top=top;
				}
				else{*/
					
					element.style.marginLeft=left;
					element.style.marginTop=top;
					sound_ele.style.marginLeft=sound_left[i];
					sound_ele.style.marginTop=sound_top[i];	
					lcd_ele.style.marginLeft=lcd_left[i];
					lcd_ele.style.marginTop=lcd_top[i];	
				//}
        }		
		///Seventh_bind_third:car rotating
		function rotate(i){
        	var element = document.getElementById("car"+i);			
        	var tposea =-(pos_location_z[i])*57.4;
			
			element.style.MozTransform = "rotate(" + tposea + "deg)";
            element.style.webkitTransform ="rotate(" + tposea + "deg)";
            element.style.msTransform = "rotate(" + tposea + "deg)";
            element.style.OTransform = "rotate(" + tposea + "deg)";
            element.style.transform = "rotate(" + tposea + "deg)";
        }
		function drag(i){
 			
		/*	
			var margin_left=0;
			var margin_top=0;
			for(var m=1;m<model.length;++m){
				var drag_model=document.getElementById("model"+m);
				margin_left +=parseFloat(drag_model.style.marginLeft);
				//margin_left +=parseFloat(drag_model.style.marginLeft)+parseFloat(drag_model.style.width);
				margin_top +=parseFloat(drag_model.style.height);
			}
			for(var c=0;c<i+1;++c){
				var drag_pos=document.getElementById("car"+c);
				margin_left +=parseFloat(drag_pos.style.marginLeft);
				//margin_left +=parseFloat(drag_pos.style.marginLeft)+parseFloat(drag_pos.style.width);
				margin_top +=parseFloat(drag_pos.style.height);
			}*/
			//alert(margin_left);
			var px,py;
 			var obj = document.getElementById("car"+i.toString());
			var sound=document.getElementById("sound"+i.toString());
  			obj.orig_index=9999;//设置当前对象永远显示在最上层
 	
 			obj.onmousedown=function (evt){//鼠标按下
				//endsimulate();
  				this.style.cursor="move";//设置鼠标样式
  				this.style.zIndex=1000;
				obj.style.backgroundColor="#0000FF";
  				if(!evt) evt=event || window.event;//按下时创建一个事
           		var scrollX = document.documentElement.scrollLeft || document.body.scrollLeft;
            	var scrollY = document.documentElement.scrollTop || document.body.scrollTop;
				var cliX = document.documentElement.clientLeft || document.body.clientLeft;
            	var cliY = document.documentElement.clientTop || document.body.clientTop;
          		var x=evt.clientX-scrollX-obj.offsetLeft;//x=鼠标相对于网页的x坐标-网页被卷去的宽-待移动对象的左外边距
  				var y=evt.clientY-scrollY-obj.offsetTop;//y=鼠标相对于网页的y左边-网页被卷去的高-待移动对象的左上边距
				//alert("yyyyy");
				//alert("y:"+e.clientY);
				if(mapLeft==0){
					mapLeft=evt.clientX-parseFloat(obj.style.marginLeft);
				}
				//alert("mapl"+mapLeft);
  				document.onmousemove=function(a){//鼠标移动
					obj.style.position="absolute";
   					if(!a) a=event || window.event;//移动时创建一个事件
					obj.style.left=(a.clientX+document.body.scrollLeft-x-car_left[i])+"px";
   					obj.style.top=(a.clientY+document.body.scrollTop-y-car_top[i])+"px";
					
					sound.style.position="absolute";
					sound.style.left=(a.clientX+document.body.scrollLeft-x-car_left[i]+2*scale*pos_width[i])+"px";
   					sound.style.top=(a.clientY+document.body.scrollTop-y-car_top[i])+"px";
					
  				}
  				document.onmouseup=function (eu){//鼠标放开
  	 				document.onmousemove=null;
					if(!eu) eu=event || window.event;
   					
					//alert("mapx="+scale*mapX+"y="+e.clientY);
					//alert("up:"+(parseFloat(obj.style.left)+car_left[i]+x));
					//alert(e.clientX);
					
					//px=((parseFloat(obj.style.left)+car_left[i]+x-mapLeft-scale*mapX+x)/scale).toString();
					//py=((scale*mapY-parseFloat(obj.style.top)-car_top[i]-y+y)/scale).toString();
					//alert(e.clientX);
					px=((eu.clientX-mapLeft-scale*mapX+x)/scale).toString();
					py=((scale*mapY-eu.clientY+y)/scale).toString();
   					//alert(e.clientX);
					//alert("sss");
					document.onmouseup = null;
					obj.style.cursor="normal";//设置放开的样式
   					obj.style.zIndex=obj.orig_index;//alert(parseFloat(obj.style.left)+":"+parseFloat(document.getElementById("map").style.left)+":"+scale*mapX);
					/*
					if(index==1){
						px=((e.clientX-mapLeft-scale*mapX+x)/scale).toString();
						py=((scale*mapY-e.clientY+y)/scale).toString();
						index=2;
					}
					else{
						px=e.clientX+x
					}*/
					//var px=(parseFloat(obj.style.left)/scale).toString();
					//var py=(parseFloat(obj.style.top)/scale).toString();
					//alert("px="+px+" py="+py);
					
					if(xmlHttp!=null){
						
        				xmlHttp.onreadystatechange = setPos_change;
						
        				xmlHttp.open("POST", "setPosLocTick.php", false);
						
        				xmlHttp.setRequestHeader("Content-Type","application/x-www-form-urlencoded");
        				//xmlHttp.send(null);

						xmlHttp.send("wsport="+webserverport+"&Name="+position[i]+"&px="+px+"&py="+py);
				
        			}
  				}
 			}  
		}
		function setPos_change(){
			if(xmlHttp.readyState==4){
				
				location.reload();
			}
		}
        </script>

        </head>
        
        <body background="">
        <div class="map" id="map">&nbsp;</div>
		<hr />
		<input type="button" id="btn" value="开始/暂停" onClick="start_pause()">    </input>
       
		<!--input type="button" id="reset" value="重新开始" -->
		<input type="button" id="upSpeed" value="speedUp" onClick="upSpeed()">    </input>
		<input type="button" id="downSpeed" value="speedDown" onClick="downSpeed()"></input>
       
        <br />
		<hr />
		<div id="show"></div>

		<script>
		function start_pause(){
			if(paused){
			endsimulate();
			paused = false;
			}else{
        		simulate();
				paused=true;
			}
		};
		/*$("reset").onclick = function(){
				
		};*/
		
		function upSpeed(){
				if(simtime >=40){
					endsimulate();
					simtime -=30;		
        			simulate();
				}
		};
		function downSpeed(){
				endsimulate();
				simtime +=30;
        		simulate();
		};
		</script>

		
        </body>
</html>

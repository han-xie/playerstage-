<html>
        <head>
                <title>webstage v1.0</title>
                <style type="text/css">
					/*#map{
						margin-left: 0PX;
						height: 500px;
						width: 500PX;
						background-image: url('pic/cave_compact.png');
						background-repeat: no-repeat;
						background-size: cover;
					}*/
					.map {
						width:480px;
						height:480px;
						/*MARGIN-RIGHT: auto;
						MARGIN-LEFT: auto;*/
						background-image:url('pic/cave_compact.png');
						-moz-background-size: 100% 100%;
						-o-background-size: 100% 100%;
						-webkit-background-size: 100% 100%;
						background-size: 100% 100%;
						-moz-border-image: url('pic/cave_compact.png') 0;
						background-repeat:no-repeat\9;
						background-image:none\9;
						filter:progid:DXImageTransform.Microsoft.AlphaImageLoader(src='pic/cave_compact.png', sizingMethod='scale')\9;
					}
					#car {
					  /*width:20px;
					  height:15px;*/
					  display:block;
					}
                </style>
                <!--  link href="style.css" rel="stylesheet" type="text/css" /-->
                <!--link href="car.js" rel="stylesheet" type="text/javascript" /-->  

<?php
$tfurl = $_GET['httpport'] ;
$imageUrl = "pic/cave_compact.png";
?>

    <script type="text/javascript">
        function $(id){return document.getElementById(id);}
        
        
        var posex=[1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0];
        var posey=[1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0];
        var posea=[1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0];
        var time=1.0;
        
        var xmlHttp=null;
        
        var modelSizex=16.0;
        var modelSizey=16.0;
        var modelSizez=0.8;
        
        var picSizex=500;
        var picSizey=500;
        
        var paused=false;
        
        //about display the background picture
        //var leftDis = 518;
        var leftDis = -8;
        var topDis = -8;
        var scale=30;
        
        var model="cave";
        var position=["r0","r1","r2","r3","r4","r5","r6","r7","r8","r9","r10","r11","r12","r13","r14","r15","r16","r17","r18","r19"];
        var positionNum=0;
        var webserverport="<?php echo $tfurl ?>"

//$res['id'] = $_POST['id']; 
//$res['wsport'] = $_POST['wsport'];
//$res['id'] =  $_GET['id'];
//$res['wsport'] = $_GET['wsport'];

        var timer;
        var simtime=100;
        
		window.onload = function(){
			getBKPicSize(); // I am suspect this because the picture which havn't loaded can't get its size. 
        	newxmlHttpRequest();
        	getSimTreeNode();
        	getmodelSize();
        	drawMap();
        	drawDivs();
        	//drawCarDivSize();
        	simulate();
			paused=true;
        }
        
        function drawMap(){
          	var dmap = document.getElementById("map");
            dmap.style.backgroundImage = "url("+"<?php echo $imageUrl ?>"+")";
			dmap.style.width=scale*modelSizex;
			dmap.style.height=scale*modelSizey;
		}
		
		function drawCarDivSize(){
			var dCarDivSize = document.getElementById("car");
			dCarDivSize.style.width = 0.7*scale;
			dCarDivSize.style.height = 0.5*scale;
		}

        function getBKPicSize(){
        		var image=new Image();
				//image.src="pic/cave_compact.png";
                image.src="<?php echo $imageUrl ?>";
				picSizex = image.width;
				picSizey = image.height;
				console.log("picWidth="+picSizex);
				console.log(",");
				console.log("picHeight="+picSizey);
        }
        
        function drawDivs(){
        	for(var i=0;i<positionNum;i++){
        		var   newNode=document.createElement("div");
        		newNode.setAttribute("id","rotate"+i);
        		//newNode.setAttribute("style","cursor:pointer;position:absolute;top:20px;left:10px;");
        		//newNode.setAttribute("style","cursor:pointer;position:inherit;top:20px;left:10px;");
                //newNode.setAttribute("style","cursor:pointer;position:absolute;top:20px;left:10px;width:14px;height:10px;");
        		//newNode.setAttribute("style","cursor:pointer;position:inherit;top:20px;left:10px;width:20px;height:15px;");
        		newNode.setAttribute("style","cursor:pointer;position:relative;top:0px;left:0px;width:20px;height:15px;");
        		var   newNodeNode=document.createElement("img");
        		newNodeNode.setAttribute("id","car");
        		newNodeNode.setAttribute("src","pic/car.png");
        		newNode.appendChild(newNodeNode);
        		document.getElementById("map").appendChild(newNode);
        	}
        }
        
   
        
        function newxmlHttpRequest(){
		    try {
			// Firefox, Opera 8.0+, Safari
			 xmlHttp = new XMLHttpRequest();
		    }
		    catch (e) {
			// Internet Explorer
			 try {
			    xmlHttp = new ActiveXObject("Msxml2.XMLHTTP");
			}
			catch (e) {

			    try {
				xmlHttp = new ActiveXObject("Microsoft.XMLHTTP");
			    }
			    catch (e) {
				alert("your brower don not support AJAX！");
				return false;
			    }
			}
		    }
        }
        
        function SimTreeNode_Change(){
        if (xmlHttp.readyState == 4) {
			var obj = eval('('+ xmlHttp.responseText +')');
			console.log(obj);
			console.log(";");
			model =obj.model;
			positionNum=parseInt(obj.position0);
			for(var i=0;i<positionNum;i++){
				if(i==0)
					position[0]=obj.position1;
				else if(i==1)
					position[1]=obj.position2;
				else if(i==2)
					position[2]=obj.position3;
				else if(i==3)
					position[3]=obj.position4;
				else if(i==4)
					position[4]=obj.position5;
				else if(i==5)
					position[5]=obj.position6;
				else if(i==6)
					position[6]=obj.position7;
				else if(i==7)
					position[7]=obj.position8;
				else if(i==8)
					position[8]=obj.position9;
				else if(i==9)
					position[9]=obj.position10;
			}
			console.log(model);
			console.log(positionNum);
			console.log(position[0]);
			console.log(position[1]);	   
		}
        }
        
        function getSimTreeNode(){
        	if(xmlHttp!=null){
        			xmlHttp.onreadystatechange = SimTreeNode_Change;
        			xmlHttp.open("POST", "getTreeNode.php", false);
				    xmlHttp.setRequestHeader("Content-Type","application/x-www-form-urlencoded"); 
				    //xmlHttp.send();
				    xmlHttp.send("wsport="+webserverport);
        	}
        }
        
        function getSimTreeXMl(){
                if(xmlHttp!=null){
        			xmlHttp.onreadystatechange = SimTreeXMl_Change;
        			xmlHttp.open("POST", "getTreeXML.php", false);
				    xmlHttp.setRequestHeader("Content-Type","application/x-www-form-urlencoded"); 
				    //xmlHttp.send();
				    xmlHttp.send("wsport="+webserverport);
        		}
        }
        
        
        function rotate(i){
        	var element = document.getElementById("rotate"+i);
        	var tposea =(-posea[i])*57.4;
			element.style.MozTransform = "rotate(" + tposea + "deg)";
            element.style.webkitTransform ="rotate(" + tposea + "deg)";
            element.style.msTransform = "rotate(" + tposea + "deg)";
            element.style.OTransform = "rotate(" + tposea + "deg)";
            element.style.transform = "rotate(" + tposea + "deg)";
        }
        
        function move(i){
				var element = document.getElementById("rotate"+i);
				var tposex=posex[i] +(modelSizex/2);
				tposex *= scale;
				tposex += leftDis;
				var tposey = (modelSizey/2)- posey[i];
				tposey *= scale;
                tposey += topDis;
				element.style.left=tposex+"px";
				element.style.top=tposey+"px";
        }
                
                
        function simulate(){
        if(simtime == 0) simtime=100;
            timer = setInterval(function(){
            	getAllPVANodeSim();
                for(var i=0;i<positionNum;i++){
                	move(i);
                	rotate(i);
                }
            }, simtime);
        }
        
        function endsimulate(){
        	clearInterval(timer);
        }
        
        function pva_Change(i){
        			if (xmlHttp.readyState == 4) {
					    var str = xmlHttp.responseText;
					    document.getElementById('show').innerHTML +=i;
					    var obj = eval('('+ xmlHttp.responseText +')');
					    //var obj = eval(({"id":"123","name":"elar","age":"21"}));
					   // alert(obj.posex);
						posex[i]=parseFloat(obj.posex);
						posey[i]=parseFloat(obj.posey);
						posea[i]=parseFloat(obj.posea);
						time=parseFloat(obj.time);
					}
        }
        function modelsize_Change(){
                	if (xmlHttp.readyState == 4) {
					    var obj = eval('('+ xmlHttp.responseText +')');
						modelSizex=parseFloat(obj.sizex);
						modelSizey=parseFloat(obj.sizey);
						modleSizez=parseFloat(obj.sizez);
						console.log(obj);
						console.log("modelSizex="+modelSizex);
						console.log("modelSizey="+modelSizey);
					}
        }
        
        function simtree_Change(){
                	if (xmlHttp.readyState == 4) {
					    var obj = eval('('+ xmlHttp.responseText +')');
					    postion=obj.postion;
						model=obj.model;
					}
        }
        
        function getsimtree(){
	        if(xmlHttp!=null){
	        xmlHttp.onreadystatechange = simtree_Change;
	        			xmlHttp.open("POST", "getTreeNode.php", true);
					    xmlHttp.setRequestHeader("Content-Type","application/x-www-form-urlencoded"); 
					    //xmlHttp.send("id=3");
					    xmlHttp.send("id=3&wsport="+webserverport);
	        }
        }
        
        function getmodelSize(){
	        if(xmlHttp!=null){
	        	xmlHttp.onreadystatechange = modelsize_Change;
	        	xmlHttp.open("POST", "getmodelsize.php", false);
				xmlHttp.setRequestHeader("Content-Type","application/x-www-form-urlencoded");
				//xmlHttp.send("id="+model);
				xmlHttp.send("id="+model+"&wsport="+webserverport);
	        }
        }
        
        function getPVANode(i){
        if(xmlHttp!=null){				    
                	xmlHttp.onreadystatechange = pva_Change(i);
				    //var data = "id=123";
				    xmlHttp.open("POST", "getPVANode.php", true);
				    xmlHttp.setRequestHeader("Content-Type","application/x-www-form-urlencoded"); 
				    //xmlHttp.send("id="+position[i]);
				    //xmlhttp.send("arg1=data1&arg2=data2");
				    xmlHttp.send("id="+position[i]+"&wsport="+webserverport);
        }
        }        
        
        function getPVANodeSim(i){
        if(xmlHttp!=null){				    
                	xmlHttp.onreadystatechange = pva_Change(i);
				    //var data = "id=123";
				    xmlHttp.open("POST", "getPVANodeSim.php", true);
				    xmlHttp.setRequestHeader("Content-Type","application/x-www-form-urlencoded"); 
				    //xmlHttp.send("id="+position[i]);
				    xmlHttp.send("id="+position[i]+"&wsport="+webserverport);
        }
        }
        function pvaAll_Change(){
        			if (xmlHttp.readyState == 4) {
					    var str = xmlHttp.responseText;
					    document.getElementById('show').innerHTML = str;
					    var obj = eval('('+ xmlHttp.responseText +')');
					    time=parseFloat(obj.p0.time);
					    if(positionNum>0){
					    	posex[0]=parseFloat(obj.p0.posex);
							posey[0]=parseFloat(obj.p0.posey);
							posea[0]=parseFloat(obj.p0.posea);
					    }
					    if(positionNum>1){
					    	posex[1]=parseFloat(obj.p1.posex);
							posey[1]=parseFloat(obj.p1.posey);
							posea[1]=parseFloat(obj.p1.posea);
					    }
					    if(positionNum>2){
					    	posex[2]=parseFloat(obj.p2.posex);
							posey[2]=parseFloat(obj.p2.posey);
							posea[2]=parseFloat(obj.p2.posea);
					    }
					    if(positionNum>3){
					    	posex[3]=parseFloat(obj.p3.posex);
							posey[3]=parseFloat(obj.p3.posey);
							posea[3]=parseFloat(obj.p3.posea);
					    }
					    if(positionNum>4){
					    	posex[4]=parseFloat(obj.p4.posex);
							posey[4]=parseFloat(obj.p4.posey);
							posea[4]=parseFloat(obj.p4.posea);
					    }
					    if(positionNum>5){
					    	posex[5]=parseFloat(obj.p5.posex);
							posey[5]=parseFloat(obj.p5.posey);
							posea[5]=parseFloat(obj.p5.posea);
					    }
					    if(positionNum>6){
					    	posex[6]=parseFloat(obj.p6.posex);
							posey[6]=parseFloat(obj.p6.posey);
							posea[6]=parseFloat(obj.p6.posea);
					    }
					    if(positionNum>7){
					    	posex[7]=parseFloat(obj.p7.posex);
							posey[7]=parseFloat(obj.p7.posey);
							posea[7]=parseFloat(obj.p7.posea);
					    }
					    if(positionNum>8){
					    	posex[8]=parseFloat(obj.p8.posex);
							posey[8]=parseFloat(obj.p8.posey);
							posea[8]=parseFloat(obj.p8.posea);
					    }
					    if(positionNum>9){
					    	posex[9]=parseFloat(obj.p9.posex);
							posey[9]=parseFloat(obj.p9.posey);
							posea[9]=parseFloat(obj.p9.posea);
					    }
					}
        }
        
        function getAllPVANodeSim(){
        if(xmlHttp!=null){
        	xmlHttp.onreadystatechange = pvaAll_Change;
        	xmlHttp.open("POST", "getAllPVANodeTick.php", true);
        	xmlHttp.setRequestHeader("Content-Type","application/x-www-form-urlencoded");
        	//xmlHttp.send(null);
			xmlHttp.send("wsport="+webserverport);
        }
        }

        </script>

        </head>
        
        <body>
                <div class="map" id="map"> 	
                        <!--div id="rotate" style=" cursor:pointer; position:absolute; left:100px; top:100px"-->
						<!--  div id="rotate" style=" cursor:pointer;position:absolute;">
                                <img id="car" src="pic/car.PNG"/>
                        </div-->
                </div>
		<hr />
		<input type="button" id="btn" value="start/pause" >    </input>
		<!--  input type="button" id="reset" value="重新开始" -->
		<input type="button" id="upSpeed" value="speedUp" >    </input>
		<input type="button" id="downSpeed" value="speedDown" ></input><br />
		<hr />
		<div id="show"></div>

		<script>
		$('btn').onclick = function(){
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
		$("upSpeed").onclick = function(){

				if(simtime >=40){
					endsimulate();
					simtime -=30;
        			simulate();
				}
		};
		$("downSpeed").onclick = function(){
				endsimulate();
				simtime +=30;
        		simulate();
		};
		</script>

		
        </body>
</html>

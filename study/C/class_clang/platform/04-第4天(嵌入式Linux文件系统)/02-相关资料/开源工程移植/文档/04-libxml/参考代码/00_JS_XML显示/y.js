function createXHR(){//此JS函数即为创建XML对象，并兼容IE，firefox等浏览器
	var xmlhttp = null;
	if(window.XMLHttpRequest){
		xmlhttp = new XMLHttpRequest();
	}else if(window.ActiveXObject){
		xmlhttp = new ActiveXObject("Microsoft.XMLHTTP");
	}else{
		xmlhttp = new ActiveXObject('Msxml2.XMLHTTP');
	}		
	if(xmlhttp){
		;//alert('create xmlhttp success!');
	}else{
		alert('create xmlhttp error!');
	}
	return xmlhttp;
}

function display_xml(xmlDoc){
	document.write("<table>");
	for (var i=0;i<xmlDoc.length;i++)//总共有多少man标签
	{ 
		document.write("<tr><td>");
		document.write(xmlDoc[i].getElementsByTagName("name")[0].childNodes[0].nodeValue);
		document.write("</td><td>");
		document.write(xmlDoc[i].getElementsByTagName("time")[0].childNodes[0].nodeValue);
		document.write("</td></tr>");
	}
	document.write("</table>");
}

function jssubmit(){
	var xmlhttp_object = createXHR();
	var URL = "/cgi-bin/y.cgi?user=e&psw=e&sex=man&car=bike";
	if(xmlhttp_object){
		xmlhttp_object.onreadystatechange = function(){	//创建回调函数
			if(xmlhttp_object.readyState==4 && xmlhttp_object.status==200){
				var xmlDoc = xmlhttp_object.responseXML;
				if(xmlDoc){
					var xml = xmlDoc.getElementsByTagName("man");//得到所有名字为man的标签
					display_xml(xml);
				}else{
					alert("xmlDoc is null");
				}
			}
		}
		xmlhttp_object.open("GET",URL,true);//false:同步;true:异步
		xmlhttp_object.setRequestHeader("If-Modified-Since", "0");
		xmlhttp_object.send();	
	}
	else{
		alert('please check the Bowser!');
	}
}
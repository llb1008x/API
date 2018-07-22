function createXHR(){//��JS������Ϊ����XML���󣬲�����IE��firefox�������
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
	for (var i=0;i<xmlDoc.length;i++)//�ܹ��ж���man��ǩ
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
		xmlhttp_object.onreadystatechange = function(){	//�����ص�����
			if(xmlhttp_object.readyState==4 && xmlhttp_object.status==200){
				var xmlDoc = xmlhttp_object.responseXML;
				if(xmlDoc){
					var xml = xmlDoc.getElementsByTagName("man");//�õ���������Ϊman�ı�ǩ
					display_xml(xml);
				}else{
					alert("xmlDoc is null");
				}
			}
		}
		xmlhttp_object.open("GET",URL,true);//false:ͬ��;true:�첽
		xmlhttp_object.setRequestHeader("If-Modified-Since", "0");
		xmlhttp_object.send();	
	}
	else{
		alert('please check the Bowser!');
	}
}
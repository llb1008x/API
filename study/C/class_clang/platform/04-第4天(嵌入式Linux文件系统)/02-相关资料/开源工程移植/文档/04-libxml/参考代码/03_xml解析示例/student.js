function createXHR()//此JS函数即为创建XML对象，并兼容IE，firefox等浏览器
{
	//alert('in createXHR');
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


function jssubmit()
{
	var xmlhttp_object = createXHR();
	var URL = "/cgi-bin/student.cgi";
	
	if(xmlhttp_object)
	{
		xmlhttp_object.onreadystatechange = function()
		{	//创建回调函数
		//	alert(xmlhttp_object.status);
			if(xmlhttp_object.readyState==4 && xmlhttp_object.status==200)
			{		
				var xmlDoc = xmlhttp_object.responseXML;//获取xml文档的对象
				
				var stu = xmlDoc.getElementsByTagName("studentName"); //查找到所有的 studentName 节点
				 var i = 0;			
				 for(i=0;i<stu.length;i++)	//student 节点的个数为stu.length，顺序从上往下,这里个数应该为3
				 {					
					 alert(stu[i].childNodes[0].nodeValue);//对应标签下的值，i=1，则依次打印值为：张三，李四，王五
				 }

			/*=========以下解析xml，使用的是xpath，具体了解请查阅w3school==========*/	
			/***********111111**********
				var path1 = "/studentInfo/student/studentName";
				var nodes = xmlDoc.selectNodes(path1);
				alert("length==" + nodes.length);//length=3
				var i;
				for(i=0;i<nodes.length;i++)
				{
					alert(nodes[i].childNodes[0].nodeValue);//依次显示张三，李四，王五
				}
			*/
			
			/************222222*************
				var path2 = "/studentInfo/student[1]/studentName";//选取 studentInfo 元素下面的第一个 student节点中的studentName，下标从0开始
				var nodes = xmlDoc.selectNodes(path2);
				alert("length==" + nodes.length);//length=1
				var i;
				for(i=0;i<nodes.length;i++)
				{
					alert(nodes[i].childNodes[0].nodeValue);//显示：李四
				}
			*/
				
			/**************333333****************************	
				var path = "/studentInfo/student/studentName/text()";选取 studentName节点中的文本
				var nodes = xmlDoc.selectNodes(path);
				var i;
				for(i=0;i<nodes.length;i++)
				{
					alert(nodes[i].nodeValue);//依次显示张三，李四，王五
				}	
			*/
			
			
			/*************44444444****************
				var nodes = xmlDoc.selectNodes("//student[@student_id]");//选取所有拥有名为 student_id 的属性的 student 节点。
				
				//var nodes = xmlDoc.selectNodes("//student[@student_id='1']");//选去student 节点，并且它的属性student_id等于'1'。
				
				var i;
				alert("length==="+nodes.length);//length=3
				for(i=0;i<nodes.length;i++)
				{
					var name = nodes[i].getElementsByTagName("studentName");//从student节点下查找studentName节点
					var j;
					for(j=0;j<name.length;j++)//length=1
					{
						alert(name[j].childNodes[0].nodeValue);//依次显示张三，李四，王五
					}	
				}
			*/
			
			/**********555555555获取节点属性***********
				var path = "/studentInfo/student[1]";//刚好查找到李四
				var nodes = xmlDoc.selectNodes(path);
				var i;
				for(i=0;i<nodes.length;i++)
				{
					alert("id==" + nodes[i].getAttribute("student_id"));//获取对应节点的属性值，显示：id=2
				}
			*/
			}
		}
		xmlhttp_object.open("GET",URL,true);//false:同步;true:异步
		xmlhttp_object.setRequestHeader("If-Modified-Since", "0");
		xmlhttp_object.send();	
	}
}
function createXHR()//��JS������Ϊ����XML���󣬲�����IE��firefox�������
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
		{	//�����ص�����
		//	alert(xmlhttp_object.status);
			if(xmlhttp_object.readyState==4 && xmlhttp_object.status==200)
			{		
				var xmlDoc = xmlhttp_object.responseXML;//��ȡxml�ĵ��Ķ���
				
				var stu = xmlDoc.getElementsByTagName("studentName"); //���ҵ����е� studentName �ڵ�
				 var i = 0;			
				 for(i=0;i<stu.length;i++)	//student �ڵ�ĸ���Ϊstu.length��˳���������,�������Ӧ��Ϊ3
				 {					
					 alert(stu[i].childNodes[0].nodeValue);//��Ӧ��ǩ�µ�ֵ��i=1�������δ�ӡֵΪ�����������ģ�����
				 }

			/*=========���½���xml��ʹ�õ���xpath�������˽������w3school==========*/	
			/***********111111**********
				var path1 = "/studentInfo/student/studentName";
				var nodes = xmlDoc.selectNodes(path1);
				alert("length==" + nodes.length);//length=3
				var i;
				for(i=0;i<nodes.length;i++)
				{
					alert(nodes[i].childNodes[0].nodeValue);//������ʾ���������ģ�����
				}
			*/
			
			/************222222*************
				var path2 = "/studentInfo/student[1]/studentName";//ѡȡ studentInfo Ԫ������ĵ�һ�� student�ڵ��е�studentName���±��0��ʼ
				var nodes = xmlDoc.selectNodes(path2);
				alert("length==" + nodes.length);//length=1
				var i;
				for(i=0;i<nodes.length;i++)
				{
					alert(nodes[i].childNodes[0].nodeValue);//��ʾ������
				}
			*/
				
			/**************333333****************************	
				var path = "/studentInfo/student/studentName/text()";ѡȡ studentName�ڵ��е��ı�
				var nodes = xmlDoc.selectNodes(path);
				var i;
				for(i=0;i<nodes.length;i++)
				{
					alert(nodes[i].nodeValue);//������ʾ���������ģ�����
				}	
			*/
			
			
			/*************44444444****************
				var nodes = xmlDoc.selectNodes("//student[@student_id]");//ѡȡ����ӵ����Ϊ student_id �����Ե� student �ڵ㡣
				
				//var nodes = xmlDoc.selectNodes("//student[@student_id='1']");//ѡȥstudent �ڵ㣬������������student_id����'1'��
				
				var i;
				alert("length==="+nodes.length);//length=3
				for(i=0;i<nodes.length;i++)
				{
					var name = nodes[i].getElementsByTagName("studentName");//��student�ڵ��²���studentName�ڵ�
					var j;
					for(j=0;j<name.length;j++)//length=1
					{
						alert(name[j].childNodes[0].nodeValue);//������ʾ���������ģ�����
					}	
				}
			*/
			
			/**********555555555��ȡ�ڵ�����***********
				var path = "/studentInfo/student[1]";//�պò��ҵ�����
				var nodes = xmlDoc.selectNodes(path);
				var i;
				for(i=0;i<nodes.length;i++)
				{
					alert("id==" + nodes[i].getAttribute("student_id"));//��ȡ��Ӧ�ڵ������ֵ����ʾ��id=2
				}
			*/
			}
		}
		xmlhttp_object.open("GET",URL,true);//false:ͬ��;true:�첽
		xmlhttp_object.setRequestHeader("If-Modified-Since", "0");
		xmlhttp_object.send();	
	}
}
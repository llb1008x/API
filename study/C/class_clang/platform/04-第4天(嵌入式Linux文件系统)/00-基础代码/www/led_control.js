var g_led1_flag = 0;
var g_led2_flag = 0;
var g_led3_flag = 0;
var g_ledall_flag = 0;


function createXHR()//此JS函数即为创建XML对象，并兼容IE，firefox等浏览器
{
	//alert('in createXHR');
	var xmlhttp = null;
	
	if(window.XMLHttpRequest)
		xmlhttp = new XMLHttpRequest();
	else if(window.ActiveXObject)
		xmlhttp = new ActiveXObject("Microsoft.XMLHTTP");
	else
		xmlhttp = new ActiveXObject('Msxml2.XMLHTTP');
			
	if(xmlhttp)
	{
		;//alert('create xmlhttp success!');
	}
	else
		alert('create xmlhttp error!');
	return xmlhttp;
}

function get_led_status1(led_status)
{
	//alert(led_status);
	if(led_status.indexOf('$', 0) == 0)
	{
		if(led_status.substr(1, 1) == 0)
		{
			g_led1_flag = 0;
			g_led2_flag = 0;
			g_led3_flag = 0;
			g_ledall_flag = 0;
			document.getElementById('led1').src = './img/led_off.png';
			document.getElementById('led2').src = './img/led_off.png';
			document.getElementById('led3').src = './img/led_off.png';
			document.getElementById('ledall').src = './img/alloff.png';
		}
		else if(led_status.substr(1, 1) == 0x02)
		{
			g_led1_flag = 1;
			g_led2_flag = 0;
			g_led3_flag = 0;
			g_ledall_flag = 0;
			document.getElementById('led1').src = './img/led_on.png';
			document.getElementById('led2').src = './img/led_off.png';
			document.getElementById('led3').src = './img/led_off.png';
			document.getElementById('ledall').src = './img/alloff.png';
		}
		else if(led_status.substr(1, 1) == 0x04)
		{
			g_led1_flag = 0;
			g_led2_flag = 1;
			g_led3_flag = 0;
			g_ledall_flag = 0;
			document.getElementById('led1').src = './img/led_off.png';
			document.getElementById('led2').src = './img/led_on.png';
			document.getElementById('led3').src = './img/led_off.png';
			document.getElementById('ledall').src = './img/alloff.png';
		}
		else if(led_status.substr(1, 1) == 0x06)
		{
			g_led1_flag = 1;
			g_led2_flag = 1;
			g_led3_flag = 0;
			g_ledall_flag = 0;
			document.getElementById('led1').src = './img/led_on.png';
			document.getElementById('led2').src = './img/led_on.png';
			document.getElementById('led3').src = './img/led_off.png';
			document.getElementById('ledall').src = './img/alloff.png';
		}
		else if(led_status.substr(1, 1) == 0x08)
		{
			g_led1_flag = 0;
			g_led2_flag = 0;
			g_led3_flag = 1;
			g_ledall_flag = 0;
			document.getElementById('led1').src = './img/led_off.png';
			document.getElementById('led2').src = './img/led_off.png';
			document.getElementById('led3').src = './img/led_on.png';
			document.getElementById('ledall').src = './img/alloff.png';
		}
		else if(led_status.substr(1, 1) == 0x0a)
		{
			g_led1_flag = 1;
			g_led2_flag = 0;
			g_led3_flag = 1;
			g_ledall_flag = 0;
			document.getElementById('led1').src = './img/led_on.png';
			document.getElementById('led2').src = './img/led_off.png';
			document.getElementById('led3').src = './img/led_on.png';
			document.getElementById('ledall').src = './img/alloff.png';
		}
		else if(led_status.substr(1, 1) == 0x0b)
		{
			g_led1_flag = 0;
			g_led2_flag = 1;
			g_led3_flag = 1;
			g_ledall_flag = 0;
			document.getElementById('led1').src = './img/led_off.png';
			document.getElementById('led2').src = './img/led_on.png';
			document.getElementById('led3').src = './img/led_on.png';
			document.getElementById('ledall').src = './img/alloff.png';
		}
		else if(led_status.substr(1, 1) == 0x0e)
		{
			g_led1_flag = 1;
			g_led2_flag = 1;
			g_led3_flag = 1;
			g_ledall_flag = 1;
			document.getElementById('led1').src = './img/led_on.png';
			document.getElementById('led2').src = './img/led_on.png';
			document.getElementById('led3').src = './img/led_on.png';
			document.getElementById('ledall').src = './img/allon.png';
		}
	}
}

function get_led_status()
{
	var xmlhttp_object = createXHR();
	var URL = "/cgi-bin/get_led_status.cgi";
	//var xmlhttp_status;
	
	if(xmlhttp_object)
	{
		xmlhttp_object.open("GET",URL,false);//false:synchronous;true:asynchronous
		xmlhttp_object.setRequestHeader("If-Modified-Since", "0");
		xmlhttp_object.send();
		xmlhttp_status = xmlhttp_object.status;
		if(200 == xmlhttp_status)
		{
			//alert(xmlhttp_object.responseText);
			get_led_status1(xmlhttp_object.responseText);
		}
		xmlhttp_object = null;
	}
	else
	{
		alert('please check the Bowser!');
	}
}




function switch_led(led_number, action)
{
	var xmlhttp_object = createXHR();
	var URL = "/cgi-bin/switch_led.cgi";
	var xmlhttp_status;
	var cmd1 = '$';

	//alert('led_number'+led_number);
	switch(led_number)
	{
		case 1:
		{
			if(0 == action)
			{
				g_led1_flag = 1;
				document.getElementById('led1').src = './img/led_on.png';
				cmd1 += '11';
			}
			else if(1 == action)
			{
				g_led1_flag = 0;
				document.getElementById('led1').src = './img/led_off.png';
				cmd1 += '10';
			}
		}
		break;
		
		case 2:
		{
			if(0 == action)
			{
				g_led2_flag = 1;
				document.getElementById('led2').src = './img/led_on.png';
				cmd1 += '21';
			}
			else if(1 == action)
			{
				g_led2_flag = 0;
				document.getElementById('led2').src = './img/led_off.png';
				cmd1 += '20';
			}
		}
		break;
		
		case 3:
		{
			if(0 == action)
			{
				g_led3_flag = 1;
				document.getElementById('led3').src = './img/led_on.png';
				cmd1 += '31';
			}
			else if(1 == action)
			{
				g_led3_flag = 0;
				document.getElementById('led3').src = './img/led_off.png';
				cmd1 += '30';
			}
		}
		break;
		
		case 4:
		{
			if(0 == action)
			{
				g_led1_flag = 1;
				g_led2_flag = 1;
				g_led3_flag = 1;
				g_ledall_flag = 1;
				document.getElementById('led1').src = './img/led_on.png';
				document.getElementById('led2').src = './img/led_on.png';
				document.getElementById('led3').src = './img/led_on.png';
				document.getElementById('ledall').src = './img/allon.png';
				cmd1 += '4';
			}
			else if(1 == action)
			{
				g_led1_flag = 0;
				g_led2_flag = 0;
				g_led3_flag = 0;
				g_ledall_flag = 0;
				document.getElementById('led1').src = './img/led_off.png';
				document.getElementById('led2').src = './img/led_off.png';
				document.getElementById('led3').src = './img/led_off.png';
				document.getElementById('ledall').src = './img/alloff.png';
				cmd1 += '5';
			}			
		}
		break;
		
		default:
		{
			;
		}
		break;
	}
	
	if(xmlhttp_object)
	{
		cmd1=encodeURI(cmd1);
		cmd1=encodeURI(cmd1);
		xmlhttp_object.open("POST",URL,false);//false:synchronous;true:asynchronous
		xmlhttp_object.setRequestHeader("If-Modified-Since", "0");
		//alert('cmd1 = ' + cmd1);
		xmlhttp_object.send(cmd1);
		if(4 == xmlhttp_object.readyState)
		{
			if(200 == xmlhttp_object.status)
			{
				//alert(xmlhttp_object.responseText);
				get_led_status();
			}
		}
		xmlhttp_object = null;
	}
	else
	{
		alert('please check the Bowser!');
	}
}






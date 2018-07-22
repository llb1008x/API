//编写人：技术部徐衍龙、罗鸣、王丽茹
//==================================================有关时间的函数===========================
//reloads the window if Nav4 resized




function reloadPage(init) {  
  if (init==true) with (navigator) {if ((appName=="Netscape")&&(parseInt(appVersion)==4)) {
    document.pgW=innerWidth; document.pgH=innerHeight; onresize=reloadPage; }}
  else if (innerWidth!=document.pgW || innerHeight!=document.pgH) location.reload();
}
reloadPage(true);


//js for feedback pages
//http://www.i.enet.com.cn/user/
function feedback_check(){
	if(document.question.message.value==""){
		alert("请填写留言!");
		document.question.message.focus();
		return false;
	}
if((!document.question.kind[0].checked)&&(!document.question.kind[1].checked)&&(!document.question.kind[2].checked)&&(!document.question.kind[3].checked)&&(!document.question.kind[4].checked)){
alert("请选择意见性质!");
document.question.kind[0].focus();
return false;
}
return true;
}

//http://other.enet.com.cn/js/date.js
//显示日期的函数
function showdate(){
	var today=new Date();                     
	var day; var date;                      
	var time_start = new Date();                     
	var clock_start = time_start.getTime();                     
	if(today.getDay()==0)  day="星期日"                     
	if(today.getDay()==1)  day="星期一"                     
	if(today.getDay()==2)  day="星期二"                     
	if(today.getDay()==3)  day="星期三"                     
	if(today.getDay()==4)  day="星期四"                     
	if(today.getDay()==5)  day="星期五"                     
	if(today.getDay()==6)  day="星期六"                     
	date=(today.getYear())+"年"+(today.getMonth()+1)+"月"+today.getDate()+"日 "; 
	document.write(date); 
	document.write(day+"</span>");                     
}

//显示时间函数
function showtime(){
	var today = new Date();
	var hour = today.getHours();
	var minute = today.getMinutes();
	var second = today.getSeconds();
	document.write(hour+":"+minute+":"+second);
}

//===========================================校验表单提交数据的函数=======================================
http://other.enet.com.cn/js/check.js


//校验文本长度
function checklength(field,sval){
	var testlen = field.value;
    if(testlen.length<sval)
  	{
   	alert("您输入的字符长度不够" + sval + "字节，请重新输入!");
   	field.focus();
   	return false;
  	}
  	return true;
}

//校验文本是否为空
function checknull(field,sval){
	if (field.value =="")
  	{
    	alert("请填写" + sval + "！");
    	field.focus();
    	return false;
  	}
  	return true;
}

//校验email
function chkemail(field,sval){
	var testemail = field.value;
    var pattern = /^\w+([-+.]\w+)*@\w+([-.]\w+)*\.\w+([-.]\w+)*/;
    var flag = pattern.test(testemail);
    if(flag&&testemail!=="")
    {
      	return true;
    }
	alert("请仔细填写您的"+sval+"!");
	field.focus();
	return false;
}

//校验是否为数字
function isNumber(field,sval)
{	
    var testnumber = field.value;
    var pattern = /^-?([1-9]\d*\.\d*|0\.\d*[1-9]\d*|0?\.0+|0)$/;
    var flag = pattern.test(testnumber);
    if(flag&&testnumber!==""){
      return 1;}
	alert("请填写"+sval+"!");
	return 0;
}

//校验是否为整数
function isint(field,sval)
{
    var testnumber = field.value;
    var pattern = /^-?[1-9]\d*$/;
    var flag = pattern.test(testnumber);
    if(flag&&testnumber!==""){
      return 1;}
	alert("请填写"+sval+"!");
	return 0;
}

//校验是否为(0-10000)的整数
function isint1(field,sval)
{	
    var testnumber = field.value;
    var pattern = /^[0-9]$|^([1-9])([0-9]){0,3}$|^10000$/;
    var flag = pattern.test(testnumber);
    if(flag&&testnumber!==""){
    return 1;}      
	alert("请填写"+sval+"!");
	return 0;
}

//匹配国内电话号码(0511-4405222 或 021-87888822)
function istell(field,sval)
{	
    var tellnumber = field.value;
    var pattern = /\d{3}-\d{8}|\d{4}-\d{7}/;
    var flag = pattern.test(tellnumber);
    if(flag&&tellnumber!==""){
    return 1;}      
	alert("请填写"+sval+"!");
	return 0;
}

//匹配腾讯QQ号(从10000开始)

function isqq(field,sval)
{
    var qqnumber = field.value;
    var pattern = /[1-9][0-9]{4,}/;
    var flag = pattern.test(qqnumber);
    if(flag&&qqnumber!==""){
    return 1;}
	alert("请填写"+sval+"!");
	return 0;
}

//匹配中国邮政编码(6位)
function ispostcode(field,sval)
{
    var postnum = field.value;
    var pattern = /[1-9]\d{5}(?!\d)/;
    var flag = pattern.test(postnum);
    if(flag&&postnum!==""){
    return 1;}
	alert("请填写"+sval+"!");
	return 0;
}

//匹配身份证(15位或18位)
function isidcard(field,sval)
{
    var idnum = field.value;
    var pattern = /\d{15}|\d{18}/;
    var flag = pattern.test(idnum);
    if(flag&&idnum!==""){
    return 1;}
	alert("请填写"+sval+"!");
	return 0;
}


//==================================================用户登录所用到的函数===========================
function Itlogin1(){
	if (document.eLogin1.userid.value==""){
		alert("请输入用户名！");
		document.eLogin1.userid.focus();
		return false;
	}else if(document.eLogin1.password.value==""){
		alert("请输入密码！");
		document.eLogin1.password.focus();
		return false;
	}else{
		return true;
	}
}



//====================手机评测转换DIV=====================
function changediv(divcount,olddivall,newdiv,newdivclass,olddivclass,listdiv,listdivall){
		 for(var p=0;p<divcount;p++){
		 	//alert(olddivall+p);
			document.getElementById(olddivall+p).className=newdivclass;
			document.getElementById(listdivall+p).style.display='none';
		}
		document.getElementById(newdiv).className=olddivclass;
		document.getElementById(listdiv).style.display='block';
}

//手机新闻排行转换DIV
function changediv_ph(listdiv,disdiv){
		document.getElementById(disdiv).style.display='none';
		document.getElementById(listdiv).style.display='block';
}

//检测关键词
function clearkey(svalue){
	if (svalue.value=="请输入关键词"){

		eval(svalue.value="");	
	}
}
function getnowD(artdate,acolor){
var mydate;
var Month;
var Day;
mydate=new Date(); 
var formtartdate="<font color="+acolor+">"+artdate+"</font>"
if (String(mydate.getMonth()+1).length>1){
	Month=parseInt(mydate.getMonth()+1);
}else{
	Month="0"+parseInt(mydate.getMonth()+1)
}
if (String(mydate.getDate()).length>1){
	Day=mydate.getDate();
}else{
	Day="0"+mydate.getDate()
}
mydate=Month+"-"+Day;
if (String(mydate)==artdate){
	document.write(formtartdate);
}else{
	document.write(artdate);
}
}

function getzqliD(artdate,acolor){
var mydate;
var Day;
mydate=new Date(); 
var formtartdate="<font color="+acolor+">"+artdate+"</font>"
if (String(mydate.getDate()).length>1){
	Day=mydate.getDate();
}else{
	Day="0"+mydate.getDate()
}
mydate=Day+"日";
if (String(mydate)==artdate){
	document.write(formtartdate);
}else{
	document.write(artdate);
}
}


function zqlidiv(divcount,olddivall,newdiv,newdivclass,olddivclass,listdiv,listdivall){
			 for(var p=0;p<divcount;p++){
				document.getElementById(olddivall+p).className=newdivclass;
				document.getElementById(listdivall+p).style.display='none';
			}
			document.getElementById(newdiv).className=olddivclass;
			document.getElementById(listdiv).style.display='block';
			if (listdiv=="rmphn0"){
				document.getElementById("img0").src="nav_1.gif"
			}else{
				document.getElementById("img0").src="nav2_1.gif"
			}
			if (listdiv=="rmphn1"){
				document.getElementById("img1").src="nav2_2.gif"
			}else{
				document.getElementById("img1").src="nav_2.gif"
			}
		}
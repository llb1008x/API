//��д�ˣ���������������������������
//==================================================�й�ʱ��ĺ���===========================
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
		alert("����д����!");
		document.question.message.focus();
		return false;
	}
if((!document.question.kind[0].checked)&&(!document.question.kind[1].checked)&&(!document.question.kind[2].checked)&&(!document.question.kind[3].checked)&&(!document.question.kind[4].checked)){
alert("��ѡ���������!");
document.question.kind[0].focus();
return false;
}
return true;
}

//http://other.enet.com.cn/js/date.js
//��ʾ���ڵĺ���
function showdate(){
	var today=new Date();                     
	var day; var date;                      
	var time_start = new Date();                     
	var clock_start = time_start.getTime();                     
	if(today.getDay()==0)  day="������"                     
	if(today.getDay()==1)  day="����һ"                     
	if(today.getDay()==2)  day="���ڶ�"                     
	if(today.getDay()==3)  day="������"                     
	if(today.getDay()==4)  day="������"                     
	if(today.getDay()==5)  day="������"                     
	if(today.getDay()==6)  day="������"                     
	date=(today.getYear())+"��"+(today.getMonth()+1)+"��"+today.getDate()+"�� "; 
	document.write(date); 
	document.write(day+"</span>");                     
}

//��ʾʱ�亯��
function showtime(){
	var today = new Date();
	var hour = today.getHours();
	var minute = today.getMinutes();
	var second = today.getSeconds();
	document.write(hour+":"+minute+":"+second);
}

//===========================================У����ύ���ݵĺ���=======================================
http://other.enet.com.cn/js/check.js


//У���ı�����
function checklength(field,sval){
	var testlen = field.value;
    if(testlen.length<sval)
  	{
   	alert("��������ַ����Ȳ���" + sval + "�ֽڣ�����������!");
   	field.focus();
   	return false;
  	}
  	return true;
}

//У���ı��Ƿ�Ϊ��
function checknull(field,sval){
	if (field.value =="")
  	{
    	alert("����д" + sval + "��");
    	field.focus();
    	return false;
  	}
  	return true;
}

//У��email
function chkemail(field,sval){
	var testemail = field.value;
    var pattern = /^\w+([-+.]\w+)*@\w+([-.]\w+)*\.\w+([-.]\w+)*/;
    var flag = pattern.test(testemail);
    if(flag&&testemail!=="")
    {
      	return true;
    }
	alert("����ϸ��д����"+sval+"!");
	field.focus();
	return false;
}

//У���Ƿ�Ϊ����
function isNumber(field,sval)
{	
    var testnumber = field.value;
    var pattern = /^-?([1-9]\d*\.\d*|0\.\d*[1-9]\d*|0?\.0+|0)$/;
    var flag = pattern.test(testnumber);
    if(flag&&testnumber!==""){
      return 1;}
	alert("����д"+sval+"!");
	return 0;
}

//У���Ƿ�Ϊ����
function isint(field,sval)
{
    var testnumber = field.value;
    var pattern = /^-?[1-9]\d*$/;
    var flag = pattern.test(testnumber);
    if(flag&&testnumber!==""){
      return 1;}
	alert("����д"+sval+"!");
	return 0;
}

//У���Ƿ�Ϊ(0-10000)������
function isint1(field,sval)
{	
    var testnumber = field.value;
    var pattern = /^[0-9]$|^([1-9])([0-9]){0,3}$|^10000$/;
    var flag = pattern.test(testnumber);
    if(flag&&testnumber!==""){
    return 1;}      
	alert("����д"+sval+"!");
	return 0;
}

//ƥ����ڵ绰����(0511-4405222 �� 021-87888822)
function istell(field,sval)
{	
    var tellnumber = field.value;
    var pattern = /\d{3}-\d{8}|\d{4}-\d{7}/;
    var flag = pattern.test(tellnumber);
    if(flag&&tellnumber!==""){
    return 1;}      
	alert("����д"+sval+"!");
	return 0;
}

//ƥ����ѶQQ��(��10000��ʼ)

function isqq(field,sval)
{
    var qqnumber = field.value;
    var pattern = /[1-9][0-9]{4,}/;
    var flag = pattern.test(qqnumber);
    if(flag&&qqnumber!==""){
    return 1;}
	alert("����д"+sval+"!");
	return 0;
}

//ƥ���й���������(6λ)
function ispostcode(field,sval)
{
    var postnum = field.value;
    var pattern = /[1-9]\d{5}(?!\d)/;
    var flag = pattern.test(postnum);
    if(flag&&postnum!==""){
    return 1;}
	alert("����д"+sval+"!");
	return 0;
}

//ƥ�����֤(15λ��18λ)
function isidcard(field,sval)
{
    var idnum = field.value;
    var pattern = /\d{15}|\d{18}/;
    var flag = pattern.test(idnum);
    if(flag&&idnum!==""){
    return 1;}
	alert("����д"+sval+"!");
	return 0;
}


//==================================================�û���¼���õ��ĺ���===========================
function Itlogin1(){
	if (document.eLogin1.userid.value==""){
		alert("�������û�����");
		document.eLogin1.userid.focus();
		return false;
	}else if(document.eLogin1.password.value==""){
		alert("���������룡");
		document.eLogin1.password.focus();
		return false;
	}else{
		return true;
	}
}



//====================�ֻ�����ת��DIV=====================
function changediv(divcount,olddivall,newdiv,newdivclass,olddivclass,listdiv,listdivall){
		 for(var p=0;p<divcount;p++){
		 	//alert(olddivall+p);
			document.getElementById(olddivall+p).className=newdivclass;
			document.getElementById(listdivall+p).style.display='none';
		}
		document.getElementById(newdiv).className=olddivclass;
		document.getElementById(listdiv).style.display='block';
}

//�ֻ���������ת��DIV
function changediv_ph(listdiv,disdiv){
		document.getElementById(disdiv).style.display='none';
		document.getElementById(listdiv).style.display='block';
}

//���ؼ���
function clearkey(svalue){
	if (svalue.value=="������ؼ���"){

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
mydate=Day+"��";
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
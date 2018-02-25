
//***********默认设置定义.*********************
var tPopWait=50;		//停留tWait豪秒后显示提示。
var tPopShow=6000;		//显示tShow豪秒后关闭提示
var showPopStep=20;
var popOpacity=95;
var tfontcolor="#000000";
var tbgcolor="#000000";
var tbordercolor="#666666";

//***************内部变量定义*****************
var pltsPop=null;
var pltsoffsetX = 10; // 弹出窗口位于鼠标左侧或者右侧的距离；3-12 合适
var pltsoffsetY = 15; // 弹出窗口位于鼠标下方的距离；3-12 合适
var pltsPopbg="#FF0099"; //背景色
var pltsPopfg="#880000"; //前景色
var pltsTitle="";
document.write('<div id=pltsTipLayer style="display: none;position: absolute; z-index:10001"></div>');
function pltsinits()
{
document.onmouseover = plts;
document.onmousemove = moveToMouseLoc;
}
function plts()
{ var o=event.srcElement;
if(o.alt!=null && o.alt!=""){o.dypop=o.alt;o.alt=""};
if(o.title!=null && o.title!=""){o.dypop=o.title;o.title=""};
pltsPop=o.dypop;
if(pltsPop!=null&&pltsPop!=""&&typeof(pltsPop)!="undefined")
{
pltsTipLayer.style.left=-1000;
pltsTipLayer.style.display='';
var Msg=pltsPop.replace(/\n/g,"<br>");
Msg=Msg.replace(/\0x13/g,"<br>");
var re=/\{(.[^\{]*)\}/ig;
if(!re.test(Msg))pltsTitle="『Unix爱好者家园』 欢迎您！";
else{
re=/\{(.[^\{]*)\}(.*)/ig;
pltsTitle=Msg.replace(re,"$1")+" ";
re=/\{(.[^\{]*)\}/ig;
Msg=Msg.replace(re,"");
Msg=Msg.replace("<br>","");}
var attr=(document.location.toString().toLowerCase().indexOf("list.asp")>0?"nowrap":"");
var content =
'<table style="FILTER:alpha(opacity=90) shadow(color=#bbbbbb,direction=135);" id=toolTipTalbe border=0><tr><td width="100%"><table class=td cellspacing="0" cellpadding="0" style="width:100%">'+
'<tr id=pltsPoptop ><th height=23 valign=bottom><b><p id=topleft align=left>↖'+pltsTitle+'</p><p id=topright align=right style="display:none">'+pltsTitle+'↗</font></b></th></tr>'+
'<tr><td "+attr+" class=bg_td style="padding-left:14px;padding-right:14px;padding-top: 6px;padding-bottom:6px;line-height:135%">'+Msg+'</td></tr>'+
'<tr id=pltsPopbot style="display:none"><th height=23 valign=bottom><b><p id=botleft align=left>↙'+pltsTitle+'</p><p id=botright align=right style="display:none">'+pltsTitle+'↘</font></b></th></tr>'+
'</table></td></tr></table>';
pltsTipLayer.innerHTML=content;
toolTipTalbe.style.width=Math.min(pltsTipLayer.clientWidth,document.body.clientWidth/2.2);
moveToMouseLoc();
return true;
}
else
{
pltsTipLayer.innerHTML='';
pltsTipLayer.style.display='none';
return true;
}
}

function moveToMouseLoc()
{
if(pltsTipLayer.innerHTML=='')return true;
var MouseX=event.x;
var MouseY=event.y;
//window.status=event.y;
var popHeight=pltsTipLayer.clientHeight;
var popWidth=pltsTipLayer.clientWidth;
if(MouseY+pltsoffsetY+popHeight>document.body.clientHeight)
{
popTopAdjust=-popHeight-pltsoffsetY*1.5;
pltsPoptop.style.display="none";
pltsPopbot.style.display="";
}
else
{
popTopAdjust=0;
pltsPoptop.style.display="";
pltsPopbot.style.display="none";
}
if(MouseX+pltsoffsetX+popWidth>document.body.clientWidth)
{
popLeftAdjust=-popWidth-pltsoffsetX*2;
topleft.style.display="none";
botleft.style.display="none";
topright.style.display="";
botright.style.display="";
}
else
{
popLeftAdjust=0;
topleft.style.display="";
botleft.style.display="";
topright.style.display="none";
botright.style.display="none";
}
pltsTipLayer.style.left=MouseX+pltsoffsetX+document.body.scrollLeft+popLeftAdjust;
pltsTipLayer.style.top=MouseY+pltsoffsetY+document.body.scrollTop+popTopAdjust;
return true;
}
pltsinits();

window.onerror=function(a,b,c){return true;}

var openview=true;
var js_ver=false;//true;
//if (ScriptEngineMinorVersion()<5) js_ver=false;
//alert(ScriptEngineMajorVersion()+"."+ScriptEngineMinorVersion()+"."+ScriptEngineBuildVersion());

function vtrue(vvar)
{
  var tmp1=false;
  if (vvar=="True" || vvar=="true") { tmp1=true; }
  return tmp1;
}

function vnull(vvar)
{
  var tmp1=true;
  if (vvar=="" || vvar==null) { tmp1=false; }
  return tmp1;
}

function click_return(cvar,ct)
{
  var nvar='';
  switch (ct)
  {
    case 1:
      nvar=cvar;
      break;
    default :
      nvar='您确定要'+cvar+'吗？\n\n执行该操作后将不可恢复！';
      break;
  }
  var cf=window.confirm(nvar);
  if (cf) { return true; }
  return false;
}

function note_emoney(nvar,nemoney,npower)
{
  if (nemoney==0) { return true; }
  var t1=window.confirm(nvar+" 将扣除你 "+nemoney+" 货币！\n\n1、如已扣除过将不再二次扣除；\n2、未过期的计时用户将不扣除；\n\n扣除后将不能恢复！ 你确定吗？");
  if (t1) { return true; }
  return false;
}

function jk_insert_em(emvar,em_frm,em_word)
{
  var obj_tmp=eval("document."+em_frm+"."+em_word);
  obj_tmp.value+=emvar;
  obj_tmp.focus();
}

function login_true()
{
  if (document.login_frm.username.value=="") { alert("请输入您在本站注册时的 用户名称 ！"); document.login_frm.username.focus(); return false; }
  if (document.login_frm.password.value=="") { alert("请输入您在本站注册时的 登陆密码 ！"); document.login_frm.password.focus(); return false; }
}

function open_win(url,name,width,height,scroll)
{
  var Left_size = (screen.width) ? (screen.width-width)/2 : 0;
  var Top_size = (screen.height) ? (screen.height-height)/2 : 0;
  var open_win=window.open(url,name,'width=' + width + ',height=' + height + ',left=' + Left_size + ',top=' + Top_size + ',toolbar=no,location=no,directories=no,status=yes,menubar=no,scrollbars=' + scroll + ',resizable=no' );
}

function commend_friend2(t1,t2,t3) { open_win("commend.asp?nsort="+url_encode(t1)+"&topic="+url_encode(t2)+"&url="+url_encode(t3)+"","commend",500,380,"no"); }

function format_now_time()
{
  var d_time=new Date();
  var week_dim=new Array("星期日","星期一","星期二","星期三","星期四","星期五","星期六");
  //var week_dim=week_dim.concat("星期三","星期四","星期五");
  var temp1="今天是：";
  temp1+=d_time.getYear()+"年";
  temp1+=(d_time.getMonth() + 1) + "月";
  temp1+=d_time.getDate() + "日&nbsp;";
  temp1+=week_dim[d_time.getDay()]
  temp1+="&nbsp;&nbsp;";
  return temp1;
}

function frm_submitonce(theform)
{
  if (document.all||document.getElementById)
  {
    for (i=0;i<theform.length;i++)
    {
      var tempobj=theform.elements[i]
      if(tempobj.type.toLowerCase()=="submit"||tempobj.type.toLowerCase()=="reset") { tempobj.disabled=true; }
    }
  }
}

function frm_quicksubmit(eventobject)
{
  if (event.keyCode==13 && event.ctrlKey)
  {
    frm_submitonce(eval("document.write_frm"));
    write_frm.submit();
  }
}

function open_view(ourl,ot)
{
  if (openview==true)
  { window.open(ourl); }
  else
  { document.location.href=""+web_dir+ourl+""; }
}

//-------------------------------------字符处理--------------------------------------
function code_html(vars,chtype,cutenum)
{
  if (vars==null || vars=="") { return(""); }
  var strer=vars;
  //strer=health_var(strer,1)
  if (cutenum>0) { strer=cuted(strer,cutenum); }
  // / &#47;
  if (chtype>10)
  {
    strer=strer.replace(/\'/gi,"\'")		//单引号&#39;
    strer=strer.replace(/\"/gi,"\"")		//双引号&#34;
  }
  else
  {
    strer=strer.replace(/</gi,"&lt;")		//&#60;
    strer=strer.replace(/>/gi,"&gt;")		//&#62;
    strer=strer.replace(/\'/gi,"&#39;")		//单引号&#39;
    strer=strer.replace(/\"/gi,"&quot;")		//双引号&#34;
    strer=strer.replace(/ /gi,"&nbsp;")		//空格
  }
  switch (chtype)
  {
    case 1,11:
      strer=strer.replace(/\r/gi,"")		//table
      strer=strer.replace(/\n/gi,"")		//回车
    case 2,22:
      strer=strer.replace(/\r/gi,"　　")	//table
      strer=strer.replace(/\n/gi,"<br>")	//回车
  }
  return(strer);
}

//------------------------------------字符分割--------------------------------------
function cuted(vars,num)
{
  if (vars==null || vars=="") { return(""); }
  var ctypes=vars;
  var cnum=num;
  var cute_d="";
  var tc=0,cc=0,c_mod=2;
  for (ci=1;ci<=ctypes.length;ci++)
  {
    if (cnum<0) { cute_d=cute_d+"..."; break; }
    tt=ctypes.charCodeAt(ci-1);
    if (tt>696 || tt<0)
    {
      cnum-=1;
      if (cnum<0) { cute_d+="..."; break; }
      cute_d+=ctypes.substring(ci-1,ci);
    }
    else
    {
      cc+=1;
      if (cc>c_mod && c_mod>0)
      {
        tc+=1;
        cc=0;
      }
      tc+=1;
      if (tc>1)
      {
        tc=0;
        cnum-=1;
        if (cnum<0) { cute_d+="..."; break; }
      }
      cute_d+=ctypes.substring(ci-1,ci);
    }
  }
  return(cute_d);
}

function sel_mod_num(n1,n2)
{
  var ni=0;
  var nn1=n2/n1;
  var nn2=Math.ceil(nn1);
  if (nn1==nn2) { return true; }
  return false;
}

function time_diff(dt,fir_tim,last_tim)
{
  var a=fir_tim.replace(/-/gi,"/");
  var b=last_tim.replace(/-/gi,"/");
  var aa=Date.parse(a);
  var bb=Date.parse(b);
  var ta=bb-aa;
  switch (dt)
  {
    case "h":
      ta=ta/360000;
      break;
    case "m":
      ta=ta/60000;
      break;
    case "s":
      ta=ta/1000;
      break;
  }
  return ta;
}

function url_encode(vars)
{
  if (js_ver==true)
  { return(encodeURI(vars)); }
  else
  { return(vars); }
}
//------------------------------------用户查看--------------------------------------

function format_power(u_power,pt)
{
  var fi,pdim,pnum,pdim2,c;
  if (pt==2)
  { c=0; }
  else
  { c=""; }
  pdim=web_user_power.split("|");
  pnum=pdim.length;
  for ( fi=0; fi<pnum; fi++ )
  {
    if (pdim[fi]=="" || pdim[fi]==null) continue;
    pdim2=pdim[fi].split(":");
    if (u_power==pdim2[0])
    {
      switch (pt)
      {
        case 1:
          c=pdim2[1];
          fi=pnum;
          break;
        case 2:
          c=fi+1;
          fi=pnum;
          break;
        default:
          c=u_power;
          fi=pnum;
          break;
      }
    }
  }
  return c;
}

function article_view_return(v_emoney,v_power,v_url)
{
  if (note_emoney('浏览此文章',v_emoney,v_power))
  { open_view(v_url,1); }
}
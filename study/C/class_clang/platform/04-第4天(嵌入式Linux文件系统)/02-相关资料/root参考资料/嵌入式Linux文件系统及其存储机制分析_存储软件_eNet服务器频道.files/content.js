//全局 begin
//GetObj
function GetObj(a){if(document.getElementById){return eval('document.getElementById("'+a+'")')}else if(document.layers){return eval("document.layers['"+a+"']")}else{return eval('document.all.'+a)}}
//hiddenObj
function hiddenObj(a){GetObj(a).style.display="none"}
//showObj
function showObj(a){GetObj(a).style.display="block"}
//全局 end

//document.domain = "enet.com.cn";

//========================= 设置字体大中小 start =============
function doZoom(size){
	var artibody = document.getElementById("artibody");
	artibody.className = "artTxt artTxt_" + size;
}
//========================= 设置字体大中小 end =============

//========================= 打印正文部分 start =============
//print js begin
function LoadPrintJsCallBack(){
  if(typeof forSPrint == "object" && forSPrint.Print){
    forSPrint.Print();
  }
}
function LoadPrintJs(){
  var jsFile = "http://www.enet.com.cn/i/print.js";  //打印主js文件url
  jsFile += "?t="+ (new Date()).getTime();
  var js = document.createElement("script");
  js.setAttribute("src",jsFile); 
  js.setAttribute("type","text\/javascript");
  js.setAttribute("id", "enetPrintJsUrl");
  //for ie
  js.onreadystatechange = function(){
  if(js.readyState=="loaded"){
     LoadPrintJsCallBack();
  }
};
//for ff
js.onload = LoadPrintJsCallBack;
  document.body.insertBefore(js,null); // null for ff
}

//print js end
//========================= 打印正文部分 end =============

//===========================
//全文浏览
function LoadFullViewJsCallBack(){
  if(typeof forSFullView == "object" && forSFullView.FullView){
    forSFullView.FullView();
  }
}
function LoadFullViewJs(){
  var jsFile = "http://www.enet.com.cn/i/fullscreenview.js";  //全文浏览主js文件url
  jsFile += "?t="+ (new Date()).getTime();
  var js = document.createElement("script");
  js.setAttribute("src",jsFile); 
  js.setAttribute("type","text\/javascript");
  js.setAttribute( "id", "enetFullViewJsUrl");
  //for ie
  js.onreadystatechange = function(){
  if(js.readyState=="loaded"){
     LoadFullViewJsCallBack();
  }
};
//for ff
js.onload = LoadFullViewJsCallBack;
  document.body.insertBefore(js,null); // null for ff
}
//===========================


//===========================
//为了美观，不要报错
function killerr(){return true;}
window.onerror = killerr;
//===========================
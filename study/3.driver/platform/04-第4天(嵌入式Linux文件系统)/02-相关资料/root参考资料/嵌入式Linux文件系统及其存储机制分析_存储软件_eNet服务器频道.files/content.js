//ȫ�� begin
//GetObj
function GetObj(a){if(document.getElementById){return eval('document.getElementById("'+a+'")')}else if(document.layers){return eval("document.layers['"+a+"']")}else{return eval('document.all.'+a)}}
//hiddenObj
function hiddenObj(a){GetObj(a).style.display="none"}
//showObj
function showObj(a){GetObj(a).style.display="block"}
//ȫ�� end

//document.domain = "enet.com.cn";

//========================= �����������С start =============
function doZoom(size){
	var artibody = document.getElementById("artibody");
	artibody.className = "artTxt artTxt_" + size;
}
//========================= �����������С end =============

//========================= ��ӡ���Ĳ��� start =============
//print js begin
function LoadPrintJsCallBack(){
  if(typeof forSPrint == "object" && forSPrint.Print){
    forSPrint.Print();
  }
}
function LoadPrintJs(){
  var jsFile = "http://www.enet.com.cn/i/print.js";  //��ӡ��js�ļ�url
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
//========================= ��ӡ���Ĳ��� end =============

//===========================
//ȫ�����
function LoadFullViewJsCallBack(){
  if(typeof forSFullView == "object" && forSFullView.FullView){
    forSFullView.FullView();
  }
}
function LoadFullViewJs(){
  var jsFile = "http://www.enet.com.cn/i/fullscreenview.js";  //ȫ�������js�ļ�url
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
//Ϊ�����ۣ���Ҫ����
function killerr(){return true;}
window.onerror = killerr;
//===========================
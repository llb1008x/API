(function(){var f=null,i=false;function j(a){this.t={};this.tick=function(b,c,d){d=d?d:(new Date).getTime();this.t[b]=[d,c]};this.tick("start",f,a)}var k=new j;window.GA_jstiming={Timer:j,load:k};if(window.GA_jstiming){window.GA_jstiming.d={};window.GA_jstiming.j=1;var l=function(a,b,c){var d=a.t[b],e=a.t.start;if(d&&(e||c)){d=a.t[b][0];e=c!=undefined?c:e[0];return d-e}};window.GA_jstiming.report=function(a,b,c){var d="";if(a.h)d+="&"+a.h;var e=a.t,g=e.start,G=[],v=[];for(var h in e)if(h!="start")if(h.indexOf("_")!=0){var w=e[h][1];if(w)e[w]&&v.push(h+"."+l(a,h,e[w][0]));else g&&G.push(h+"."+l(a,h))}delete e.start;if(b)for(var H in b)d+="&"+H+"="+b[H];a=[c?c:"http://csi.gstatic.com/csi","?v=3",
"&s="+(window.GA_jstiming.sn||"gam")+"&action=",a.name,v.length?"&it="+v.join(","):"","",d,"&rt=",G.join(",")].join("");b=new Image;var I=window.GA_jstiming.j++;window.GA_jstiming.d[I]=b;b.onload=b.onerror=function(){delete window.GA_jstiming.d[I]};b.src=a;b=f;return a}};var m=this,n=function(a,b,c){a=a.split(".");c=c||m;!(a[0]in c)&&c.execScript&&c.execScript("var "+a[0]);for(var d;a.length&&(d=a.shift());)if(!a.length&&b!==undefined)c[d]=b;else c=c[d]?c[d]:c[d]={}},o=function(a,b){function c(){}c.prototype=b.prototype;a.l=b.prototype;a.prototype=new c};var p=/^([\w-]+\.)*([\w-]{2,})(\:[0-9]+)?$/,q=function(a,b){if(!a)return b;var c=a.match(p);return c?c[0]:b};var r,s=parseFloat("0.05"),t=r=isNaN(s)||s>1||s<0?0:s;var u;var x="false";u=x=="true"?true:i;var y=u;var z=function(){return q("","pubads.g.doubleclick.net")};var A,B,C,D,E,F,J,K,aa,L=function(){return m.navigator?m.navigator.userAgent:f},M=function(){return m.navigator};E=D=C=B=A=i;var N;if(N=L()){var ba=M();A=N.indexOf("Opera")==0;B=!A&&N.indexOf("MSIE")!=-1;D=(C=!A&&N.indexOf("WebKit")!=-1)&&N.indexOf("Mobile")!=-1;E=!A&&!C&&ba.product=="Gecko"}var ca=A,da=B,ea=E,fa=C,ga,O=M(),P=ga=O&&O.platform||"";F=P.indexOf("Mac")!=-1;J=P.indexOf("Win")!=-1;K=P.indexOf("Linux")!=-1;aa=!!M()&&(M().appVersion||"").indexOf("X11")!=-1;var Q="",R;
if(ca&&m.opera){var S=m.opera.version;Q=typeof S=="function"?S():S}else{if(ea)R=/rv\:([^\);]+)(\)|;)/;else if(da)R=/MSIE\s+([^\);]+)(\)|;)/;else if(fa)R=/WebKit\/(\S+)/;if(R){var T=R.exec(L());Q=T?T[1]:""}};var ha=document,ia=f,U=ha.getElementsByTagName("script");if(U&&U.length){var ja=U[U.length-1];ia=ja.parentNode};q("","pagead2.googlesyndication.com");var ka=function(a,b){for(var c in a)Object.prototype.hasOwnProperty.call(a,c)&&b.call(f,a[c],c,a)};var V=function(a){return!!a&&(a.indexOf("?google_debug")>0||a.indexOf("&google_debug")>0)};var W=function(){this.c=[];this.b={};for(var a=0,b=arguments.length;a<b;++a)this.b[arguments[a]]=""};W.prototype.i=function(a){if(this.b.hasOwnProperty(a))return this.b[a];return""};W.prototype.geil=W.prototype.i;var la=function(a){var b=[],c=function(d){d!=""&&b.push(d)};ka(a.b,c);if(a.c.length>0&&b.length>0)return a.c.join(",")+","+b.join(",");return a.c.join(",")+b.join(",")};function ma(a){var b;b="adsense";if((a=a)&&typeof a=="string"&&a.length>0)if(b==f)b=f;else{var c=window.GS_googleServiceIds_[b];if(c==f){c=b=="adsense"?new X:new Y;window.GS_googleServiceIds_[b]=c}b:{for(b=0;b<c.a.length;b++)if(a==c.a[b])break b;c.a[c.a.length]=a}b=c}else b=f;return b}n("GS_googleAddAdSenseService",ma,void 0);function na(){for(var a in window.GS_googleServiceIds_){var b=window.GS_googleServiceIds_[a];typeof b!="function"&&b.e()}}n("GS_googleEnableAllServices",na,void 0);
function oa(){window.GS_googleServiceIds_={}}n("GS_googleResetAllServices",oa,void 0);function pa(){var a="adsense";a=a==f?f:window.GS_googleServiceIds_[a];return a=a==f?"":a.a.join()}n("GS_googleGetIdsForAdSenseService",pa,void 0);function qa(a){return Z(a)}n("GS_googleFindService",qa,void 0);function ra(){var a=Z("adsense");return a?la(a.f):""}n("GS_googleGetExpIdsForAdSense",ra,void 0);function $(a){this.k=a;this.a=[];this.f=new W}
$.prototype.toString=function(){for(var a="["+this.k+" ids: ",b=0;b<this.a.length;b++){if(b>0)a+=",";a+=this.a[b]}a+="]";return a};var Z=function(a){return a=a==f?f:window.GS_googleServiceIds_[a]};function Y(){$.call(this,"unknown")}o(Y,$);Y.prototype.e=function(){};function X(){$.call(this,"adsense");this.g=i}o(X,$);
var sa=function(a,b){var c;c=a;var d=Z("adsense");var e=d.f;d=["32942001","32942002"];var g;g=true;if(g&&!(Math.random()<1.0E-4)&&Math.random()<b){g=Math.floor(Math.random()*d.length);e=e;d=d[g];g=void 0;if(d=="")d="";else if(g)if(e.b.hasOwnProperty(g))d=e.b[g]=d;else d="";else{e.c.push(d);d=d}d=d}else d="";c=d==="32942002"||c&&(c.indexOf("?use_google_ads2")>0||c.indexOf("&use_google_ads2")>0)?true:i;if(c){if(V(a))return"google_ads_dbg2.js";return"google_ads2.js"}else{if(V(a))return"google_ads_dbg.js";
return"google_ads.js"}};X.prototype.e=function(){if(!this.g){var a;a=sa(document.URL,t);var b="http://"+q("","partner.googleadservices.com");if(y)b="https://"+q("","securepubads.g.doubleclick.net");var c="",d;d=z();(d=d=="pubads.g.doubleclick.net")||(c="?prodhost="+z());a=b+"/gampad/"+a+c;b="script";document.write("<"+b+' src="'+a+'"><\/script>');this.g=true;if(window.GA_jstiming&&window.GA_jstiming.Timer){window.GA_jstiming.load.name="load";window.GA_jstiming.load.tick("start")}}};
window.GS_googleServiceIds_||(window.GS_googleServiceIds_={});})()

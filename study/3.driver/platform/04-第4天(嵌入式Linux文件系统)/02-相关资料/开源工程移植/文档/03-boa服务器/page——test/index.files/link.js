	var delta=0.8
	var collection;
	function floaters() {
		this.items	= [];
		this.addItem	= function(id,x,y,content)
				  {
					document.write('<DIV id='+id+' style="Z-INDEX: 10; POSITION: absolute;  width:80px; height:60px;left:'+(typeof(x)=='string'?eval(x):x)+';top:'+(typeof(y)=='string'?eval(y):y)+'">'+content+'</DIV>');
					
					var newItem				= {};
					newItem.object			= document.getElementById(id);
					newItem.x				= x;
					newItem.y				= y;

					this.items[this.items.length]		= newItem;
				  }
		this.play	= function()
				  {
					collection				= this.items
					setInterval('play()',10);
				  }
		}
		function play()
		{

			for(var i=0;i<collection.length;i++)
			{
				var followObj		= collection[i].object;
				var followObj_x		= (typeof(collection[i].x)=='string'?eval(collection[i].x):collection[i].x);
				var followObj_y		= (typeof(collection[i].y)=='string'?eval(collection[i].y):collection[i].y);

				if(followObj.offsetLeft!=(document.body.scrollLeft+followObj_x)) {
					var dx=(document.body.scrollLeft+followObj_x-followObj.offsetLeft)*delta;
					dx=(dx>0?1:-1)*Math.ceil(Math.abs(dx));
					followObj.style.left=followObj.offsetLeft+dx;
					}

				if(followObj.offsetTop!=(document.documentElement.scrollTop+followObj_y)) {
					var dy=(document.documentElement.scrollTop+followObj_y-followObj.offsetTop)*delta;
					dy=(dy>0?1:-1)*Math.ceil(Math.abs(dy));
					followObj.style.top=followObj.offsetTop+dy;
					}
				followObj.style.display	= '';
			}
		}	
		
	var theFloaters		= new floaters();
	//右面document.body.clientWidth-90
	theFloaters.addItem('followDiv1','parseInt(document.body.clientWidth/2)-470',80,'<EMBED src=/unspimg/ytrip0.swf quality=high  WIDTH=80 HEIGHT=300 TYPE=application/x-shockwave-flash id=ad wmode=opaque></EMBED>');
	//左面
	theFloaters.addItem('followDiv2','document.body.clientWidth-(parseInt(document.body.clientWidth/2)-470)-50',80,'<EMBED src=/unspimg/ytrip0.swf quality=high  WIDTH=80 HEIGHT=300 TYPE=application/x-shockwave-flash id=ad wmode=opaque></EMBED>');
	theFloaters.play();
	
//图片格式调用方法
//<a href=http://www.makewing.com/lanren/ target=_blank><img src=images/ad_100x300.jpg border=0></a>
	
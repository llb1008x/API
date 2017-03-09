H每日计划:
    1.每天有疑问的内容尽可能当天掌握，不把问题留给明天
    2.工作计划要高质量的完成，追求效率
    3.定期的检查，调整
    4.核心内容是“实践”，在实践中掌握知识，技能
    5.每周定期总结，处理遗留问题




2017.2.4
必须：
    1.整理好相关的文档（API/my_data） 														90%
    2.emacs，编辑器的了解，使用，vim编辑器相关的插件,emacs ,vim ,gedit三个先用好其中一个		50%(使用gedit)
    3.公积金																					0%
    4.在csdn和开源中国社区看到的内容深度操作系统的源码											0%
    5.git revert回滚操作，回撤（这个命令还有疑问，相关内容）									0%
     git commit --amend  																	50%
     	
     

待定：   
	1.移植17W05A电量计算部分(重点)，battery_common_fg_20.c ,battery_meter_fg_20.c ,switch_charging.c跟原始代码对比
    2.RT5081的文档，及使用（重点）
    3.搭建android开发环境，编译原生代码
    4.正则表达式这个是需要练习的
    
    
    
2017.2.5
必须：
	1.移植17W05A电量计算部分(重点，重中之重)，battery_common_fg_20.c ,battery_meter_fg_20.c ,switch_charging.c		90%
	跟原始代码对比，理清思路，代码工作的流程
	2.RT5081的文档，要仔细阅读(重点)																					0%
	3.完成昨天遗留的必须任务																							0%
	
	
	
待定：
	1.移植fuelgauge部分，这部分可能需要很大的工作量	
    2.研读RT5081文档和txt
    
    
    
2017.2.6
必须：
	1.验证移植的fuelgauge GM2.0是否正确，并且提交上去											100%
	2.研读RT5081的文档，代码(重点，重中之重)													30%
	
待定：
	1.完成遗留的任务	    
	
	
	
2017.2.7
必须：
	1.RT5081文档，代码通过log理清工作流程(重点)												20%
	2.读完俞闵宏那本书(全是讲道理，文采一般般)													0%

	
待定：
	1.解决一些遗留的任务
	

备注：
	17G05A开发板
	
	
2017.2.8
必须：
	1.调试17G05A开发板																		1%(因为不清楚要干什么)
	2.看代码，看文档	，看log																	20%(看完另外文档)
	3.串口线能用																				100%


待定：	
	1.解决一些遗留的任务
	2.用C语言和C++分别写my_string的库和类，感觉基本的东西都不记得了，好菜...
	
	
	读完俞闵宏那本书(全是讲道理，文采一般般)													60%(全篇的讲道理，语言重复冗杂)
	
	
	
	
2017.2.9
必须:
	1.调试17G05A开发板，弄清楚要干什么														10%(抓log追踪代码)
		对比rt5081_pmu_charger_gn20.c 与rt5081_pmu_charger.c
		看看lk，preloader ,kernel三个阶段的dws配置
	2.看代码，看文档，看log	
	3.读完那本书，计划下一本看什么书，还有大学幕课也要准备看些什么								90%(书看完了，但是后面的计划还没有)
	

待定:
	1.解决一些遗留的任务
	2.调整状态，调整心情	
	
	
2017.2.10
必须：
	1.调试17G05A开发板，抓log，追踪代码，改代码										10%(感觉写了一天的BUG，要努力)
	2.自动化脚本看看怎么写的															0%
	3.鸟哥的私房菜这本书，定好计划，大学幕课上(c++，数据结构)有点准备					0%
	
	
待定：
	1.下载win7镜像，做好启动盘
	2.双系统安装，grub操作(不太理解)，最主要的是系统安装，硬盘分区方面	
	

备注：制作好了win7启动盘，有win7，centos，ubuntu的镜像		
	

2017.2.11
必须：
	1.安装好双硬盘双系统，grub分区怎么回事											50%(系统是安装好了，但是不是双系统)
	2.搭建好android开发环境，能编译android											90%
	3.写一个c语言的string库，C++的string类											0%

待定：
	1.看鸟哥的私房菜和今天到的几本书
	2.K&R版的c语言，静下心好好研习这本书，提高自己的能力
	
	
2017.2.12
必须：
	1.移植完17W05A，功能正常															100%
	2.17G05A为什么充电电流这么小，USB又不充电，找原因，改好							10%
	
	
待定：
	1.搭建好android开发环境，能编译android，相关的软件也安装好							80%(软件没装好)
	source_insight,VMware_workstation
	2.旧iphone和新iphone																100%		
	
	
	
	
	
	
*******************************************************************************************************************************************	
	
	
	
	
2017.2.13
必须：
	1.找出17G05A充电电流小的原因														100%(原因是用的假电)
	2.修改代码，使充电电流正常														100%(直接换真电池就行了)
	3.抓log，追踪代码，哪些密切相关的要注意											10%
		a.关机充电
	4.试一试monkey那个脚本管不管用													100%(管用，但是手机里面应该有个monkey的二进制文件)
		
	
待定：																
	1.公开课准备，K&R的C语言
	2.梦的解析这本书，有时间看看	
    
    
2017.2.14
必须：
	1.找出USB不充电的原因															100%(USB是充电的，只是充电电流很小)
	2.对比Type-C修改为micro USB代码修改的部分											90%(注释了几个宏)
	3.慕课网C课程																	100%(C语言的基础部分)
	
待定：
	1.K&R的C语言 | KMP算法 | git rebase用法											100%
	2.梦的解析	    																0%(没看，估计是没有一个好的开始)
	
	
2017.2.15
必须：
	1.调整充电器的充电电流和电压														50%
	2.调整好心情，调整好节奏															0%
		效率不高，还很受罪，心态放宽，放松思维
	3.慕课网C课程(C基础篇)															100%
		
		
待定：
	1.K&R的C语言
	2.多实践，敢去做去实践
		
		
2017.2.16
必须：
	1.调整充电电压，电流																100%
	2.提高工作效率																	80%(比之前一天好点)
	3.慕课网，中国大学慕课的课程														100%(幕课网C课程，数组)
	
待定：
	1.实践，静下心不要浮躁		

	
2017.2.17
必须：
	1.rt5081的充电流程，电池参数，及怎么调整											20%
	2.K&RC语言，数组，幕课网C课程														0%
	3.多实践，敢于尝试，不懂多思考，多去问											20%
	(效率不高)
	
待定：
	1.静下心不要浮躁，每天要有收获	

	
2017.2.18
		放松了一天，看了一部电影(剧情有点狗血)，应该算是无聊的一天。其实无聊的时候应该给自己找点事情干
	不要整天无所事事的，每天要有进步，不能停滞不前，努力让自己变得更好，变得更优秀。发现问题要敢于面对，敢于立刻去解决处理，不能拖。

	
2017.2.19
必须：
	1.移植OTG开关到7337项目上														50%(一直反复重启....)
	2.慕课网C语言课程(函数)，中国大学慕课的课程										100%(有个递归调用，还要自己写写看)
	
待定：
	1.不要彷徨，	每天有目标，有动力，让自己变得优秀


************************************************************************************************************************************

	
2017.2.20
必须：
	1.17G05A电流是否能升到1.9A														90%(可能达不到1.9A)
	2.移植OTG开关到BBL7337项目														80%(还差调试)
	3.慕课网C语言课程，K&R的C语言函数部分												0%
	
待定：
	1.相信自己，对自己要有信心
	2.想开点，让自己变得优秀
	
	
2017.2.21
必须：
	1.移植完OTG开关到BBL7337上(其实更应该清楚OTG开关的工作流程)						80%(但是OTG开关的工作流程还是不清楚，应该是函数)
	2.提交17G05A提高充电代码到服务器上												100%
	3.慕课网C语言课程，中国大学慕课的课程												100%
	
待定：
	1.K&R的C语言函数部分				
	2.手机抓dump	
	
	
2017.2.22
必须：
	1.弄清楚17G05A，从init开始的工作流程(先里清楚pep20那几个)							50%
	2.慕课网C语言课程																0%
	3.梦的解析																		100%
	
待定：
	1.K&R的C语言函数部分		
	
2017.2.23
必须：
	1.解决17W05A的那个bug															100%
	2.17G05A用快充充电器的修改，不支持快充											100%
	3.慕课网C语言课程																0%
	
待定：
	1.梦的解析		
	
2017.2.24
必须：
	1.KMP算法，字符串匹配															50%
	2.堆栈的C语言实现																0%
	3.慕课网C语言课程																0%
		
待定：
	1.梦的解析
	2.搬家

		
2017.2.25
遛了一天，跑到了中关村，颐和园，有点累，新地址还没装修好，鼻炎又犯了QAQ


2017.2.26
必须：
	1.百年航母看完																	100%
	2.慕课网C语言.指针部分															0%
	3.中国大学慕课网，第二周课程														0%
	
待定：
	1.英语学习计划，制定，坚持
	2.梦的解析	
	
	
*********************************************************************************************************************

2017.2.27
必须：
	1.解决17W05A的那个bug，logo显示中文，应该是英文的									10%(找到了图片但是代码编译的有问题)
	2.17G05A的快充，宏还是那个变量生效												0%
	3.慕课网C语言课程，指针部分														0%
	
待定：
	1.背单词，听英语																	
	2.梦的解析					
	

2017.2.28
必须：
	1.解决昨天遗留的任务，一定要完成													0%
	2.慕课网C语言课程																0%
	
待定：
	1.被单词，听英语																	0%
	2.简单，量少但一定要完成															0%
	
这两天的执行力，效率太低，必须要改变，强化执行任务的能力



2017.3.1	
必须：
	1.修改完17W05A的关机充电logo														100%
	2.熟悉17G05A的代码流程															50%
	3.提高工作效率																	0%
	
待定：
	1.看书，梦的解析，一定要静下心，不要浮躁
	2.听听英语，听听历史，慢慢来	
	
要有一种危机感，时刻提升自己才能向前	


2017.3.2
必须：
	1.熟悉17G05A的代码流程															20%
	2.分析昨天因为mmi测试导致的充电失败的原因											80%
	3.提高工作效率
	
待定：
	1.中国大学慕课的课程，数据结构和算法，感觉很有收获，要坚持							100%
	2.看看书，听听故事或者英语	


2017.3.3
必须：
	1.分析mmi测试导致充电失败，两个log，代码调用的流程									0%
	2.能不能提高17G05A的充电电流，库仑计等相关参数是在哪调用读取的						0%
	3.中国大学慕课网，数据结构，K&R的C语言，指针数组部分								0%

待定：
	1.提高工作效率
	2.听听英语，听听历史
	
2017.3.4
好吧又是休息了一天，傍晚还被人叫去加班QAQ	



2017.3.5
必须：
	1.BBL7337 OTG开关为什么不管用，17W05A的两个patch怎么打进去						80%
	2.两份log，mmi测试导致充电失败的log，关机重启的log，这两个log都要重点分析			0%
	3.中国大学慕课网两个课程的编程作业												0%
	4.一定要打起精神，努力完成今天的计划												20%
	
待定：
	1.K&R的C语言，指针数组部分	
	
************************************************************************************************************************	

2017.3.6
必须：
	1.mmi测试导致充电失败的log以及修改的方案											100%
	2.关机重启bug的log，以及关机，重启的整个流程										30%
	3.两个patch合入17W05A项目验证													0%
	4.完成上周的作业																	0%

待定：
	1.要有危机意识，打起精神工作
	2.C语言画椭圆那个算法以及代码实现
	
	
2017.3.7
必须：
	1.分析启动，关机，重启的log流程，如何抓上层的sytem，main log						50%
	2.合入patch，前提知道是哪些是跟fuelgauge相关的									0%
	3.中国大学慕课网C语言课程，K&R的C语言指针数组部分									0%
	
待定：
	1.打起精神，努力工作，提高自己的能力是最重要的，只为追求卓越	
	

2017.3.8
必须：
	1.分析关机，重启的log，走的什么流程												100%
	2.熟悉G1605A的代码，想想那个特性应该怎么写										0%
	3.调整工作时间，不然有些任务一直无法完成，这样下去不行								50%
	
待定：
	1.提高工作效率，必须有危机意识，不能放松
	2.中国大学慕课网C语言，K&R的C语言指针数组部分，一定要着手看了，不要只是计划		
	
	
	
2017.3.9
必须：
	1.相关的文档，代码分类整理好
	2.熟悉G1605A的代码流程，想想特性应该怎么写
	3.中国大学慕课网数据结构，那几个代码怎么写
	
待定：
	1.提高工作效率，每天要有进步	
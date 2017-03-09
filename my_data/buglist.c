
1.thransplant移植

2.power待机功耗

3.charging充电

4.thermal温升

5.USB-OTG相关






老化测试中出现的问题(老化测试方案本来就有问题)
{
--->老化测试的一些状况：大量刚出厂的机器，大批的放到一起，不断的跑一些case，然后在卖给客户...

	老化测试的一些调整措施：
		1.空间环境温度问题，大量的机器放到一起，如果产生的热量没有散去，容易积累热量导致空间整体温度升高，调整测试机器之间的距离
		需要知道当时环境的温度，以及他们直接的温度，

		2.电量较低的时候充电电流较大，产生的热量也很大，如果是满电，充电电流很小，产生不了多少热量

		3.



--->meta跟工厂写号，校准modem信号相关的
	meta写号错误是fuelgauge电量计服务没有初始化

	上层要给底层传达命令，但是fuelgauge没有初始化，就使用默认的电池参数，电量为-1，导致写号错误。

	手机写号？

	+#Gionee GuoJianqiu 20151022 modify for CR01570292 begin
	+service fuelgauged /system/bin/fuelgauged
	+    class main
	+#Gionee GuoJianqiu 20151022 modify for CR01570292 end
}
	




















































charging充电相关的
/*{

 关机电压跟开机电压是不一样的，首先开机电压肯定要比关机电压高。

需要注意的地方：
	1.fg_coulom库仑计的检测，变化，这里底层对于库仑计的数据的读取，数据的转换等相关内容？
	2.电量显示异常问题？电量显示的准确性

	电量0%，1%的上报，低电保护lowpower_protect_init（）




相关的BUG
GNSPR#54689	
	现象：测机放桌面，电量从9%直接掉0%自动关机后，连充电器自动开机，重启恢复（未做大量开关机动作）

	高优先级SSS	iphone电量经常这样跳变经常变成0%，然后关机

	19：55--->20:00--->20:18--->20:28
	9% -->中间自动关机了,电量跳变?-->7%-->2%-->3%?

	电量的0%检测？什么情况下会检测0%？
	mt_battery_0Percent_tracking_check()


	这可能的原因：电量计的偏差有关
		1.RTC记录的是上一次关机时候的电量，而中间电量几还没有来的急更新，又烧了新版本，
		或者插拔充电接口，电量变化了，但是电量计还没来的急更新，导致偏差变大
	
		2.电量计通常在100%或者0%更新，
		#define CUST_SYSTEM_OFF_VOLTAGE 3385 关机电压，而在9%左右电压已经低于关机电压了，
		可能是电量计偏差较大，或者负载较大的原因，将电压拉低了。
	<3>[37450.398205]  (2)[205:bat_routine_thr][kernel]AvgVbat 3434,bat_vol 3360, AvgI 0, I 0, VChr 0, AvgT 43, T 44, ZCV 3362, CHR_Type 0, SOC  10: 10:  9


 

GNSPR#57824
	现象：手机低电量自动关机待机一晚上连接充电器》充电到7%以后长按power键开机》闪现开机画面后手机不开机》
	且从7%快速充到12%又快速掉到8%一直连着充电器【5分钟后长按power键恢复
	
	可能的原因：
		1.电量不准，电压低于开机电压
		之前电量计没有经过校准，自动关机后，关机充电到7%，启动系统，检测到电压未达到开机电压，然后系统启动不了。
		（系统的这些电量电压检测会在哪些时候调用）

		2.电量计未来的及初始化
		电量计未初始化（RTC相关）导致使用默认的值，电压为0，未达到开机电压，启动不了。

		pmic子系统在未初始化时采用的默认值
/*		
		signed int gFG_DOD0_init = 0;
	 	signed int gFG_DOD0 = 0;
		signed int gFG_DOD1 = 0;
		signed int gFG_DOD_B = 0;
		signed int gFG_coulomb = 0;
		signed int gFG_coulomb_act = 0;
		signed int gFG_voltage = 0;
		signed int gFG_current = 0;
		signed int gFG_current_init = 0;
		signed int gFG_capacity = 0;
		signed int gFG_capacity_by_c = -1;
		signed int gFG_capacity_by_c_init = 0;
		signed int gFG_capacity_by_v = 0;
		signed int gFG_capacity_by_v_init = 0;
		signed int gFG_temp = 100;
		signed int gFG_temp_avg = 100;
		signed int gFG_temp_avg_init = 100;
		signed int gFG_resistance_bat = 0;
		signed int gFG_compensate_value = 0;
		signed int gFG_ori_voltage = 0;
		signed int gFG_BATT_CAPACITY = 0;
		signed int gFG_voltage_init = 0;
		signed int gFG_current_auto_detect_R_fg_total = 0;
		signed int gFG_current_auto_detect_R_fg_count = 0;
		signed int gFG_current_auto_detect_R_fg_result = 0;
		signed int gFG_15_vlot = 3700;
		signed int gFG_BATT_CAPACITY_high_current = 1200;
		signed int gFG_BATT_CAPACITY_aging = 1200;
		signed int gFG_vbat = 0;
		signed int gFG_swocv = 0;
		signed int gFG_hwocv = 0;
		signed int gFG_vbat_soc = 0;
		signed int gFG_hw_soc = 0;
		signed int gFG_sw_soc = 0;
*/



GNSPR#59577
	现象：连接着充电器，手机电量89%，强制重启手机后电量显示为98%，再次操作电量还是98%	
	开机的时候，系统会根据电量跟RTC上的电量进行比较，如果超过5%，就重新检测电量，如果是扣电池，一定会重新计算电量

	可能的原因：
		1.跟系统RTC相关，因为RTC会在开机的时候重新给电量计赋值，RTC可能计算或储存错误的电量值


    最近几个电量显示异常的可能都跟RTC相关
	
               关于硬件7秒 强制断电关机，我们设计是power+home组合键 7秒以上 强制断电关机 ，由于电路原因 没有达到组合键功能，按power键7秒就会强制断电关机；这样会引出一些问题：

1，首先7秒时间较短，很容易误触发；如下是我确认的一些SPR：

59143	 GNSPR	【品质压力】关机状态连充电器，按power键至手机震动，出现开机画面后，手机黑屏并未开机，第三次操作恢复【附视频】2/30 郭日锋	 分配	 高	Q_秦 星达	 2：严重的	Z_张 醒醒	G1605A	 2016-12-02


58921	 GNSPR	【品质】手机关机状态连接充电器电量为19%》长按power键开机闪现一下开机画面后》没有开机且快速掉电到16%【再次长按power键恢复，附视频】随机1次	 裁决	 高	W_王 国君	 2：严重的	Y_尤 梦婷	G1605A	 2016-12-01


57814	 GNSPR	【品质】手机关机后切换卡1与卡2后按电源键开机》弹出开机画面后黑屏开机无作用【开机后恢复，附视频】随机1次	 分配	 高	Y_尤 梦婷	 2：严重的	Y_尤 梦婷	G1605A	 2016-11-27
  
58432	 GNSPR	【品质压力】连充电器关机状态，长按电源键3s有振动，闪一下开机画面又自动关机，第4次操作恢复【4次】A16	 分配	 高	L_刘 子灵	 2：严重的	L_刘 子灵	G1605A	 2016-11-29

56218	 GNSPR	【品质压力】连接充电器-手动关机》按电源键开机有振动闪开机画面但未开机,连续操作3次恢复》再次操作无异常 3/50 B23	 监控	 高	W_王 国君	 2：严重的	W_吴 能田	G1605A	 2016-11-11

59244	 GNSPR	【品质】手机连接充电器关机状态按power键》手机震动后闪一下开机画面》就自动黑屏不开机【多次开机恢复，附视频】随机2次	 裁决	 高	W_王 国君	 2：严重的	Y_尤 梦婷	G1605A	 2016-12-03

57833	 GNSPR	【品质】耗电至自动关机，连接标配充电器充电至13%后，长按Power键5-6秒无法开机，只闪过开机画面后继续充电，再次长按Power键17秒左右才开机成功【单机1次】附log与视频 11.27 8:22 王博文	 裁决	 高	L_李 路宝	 3：中等的	B_白 海燕	G1605A	 2016-11-27

57824	 GNSPR	【品质】手机低电量自动关机待机一晚上连接充电器》充电到7%以后长按power键开机》闪现开机画面后手机不开机》且从7%快速充到12%又快速掉到8%一直连着充电器【5分钟后长按power键恢复，附视频】随机1次	 裁决	 高	L_李 路宝	 2：严重的	Y_尤 梦婷	G1605A	 2016-11-27

2，7秒强制断电后，RTC丢失，导致时间丢失、电量丢等 引出的一些问题   恢复原始时间 和电量跳变 问题，这些比较严重：


58578	 GNSPR	【品质压力】开启飞行，长按电源键11s重启至待机界面，时间又回到8:00，实际时间未19:14，等2min不自动对时，关闭飞行恢复，路径必现 对比S9无【6台必现】A8	 分配	 普通	W_王 国君	 3：中等的	L_刘 子灵	G1605A	 2016-11-29

59361	 GNSPR	【品质压力】测机关机后，多次短按电源键开机后，时间被重置，时间点变成2010年01月01日8：00，4min后开数据对时恢复【5/50】A11	 分配	 普通	W_王 国君	 2：严重的	L_刘 子灵	G1605A	 2016-12-03


57209	 GNSPR	【品质压力】待机界面》时间为09:57分-长按电源键点击关机,2min后长按电源键开机-开机后时间显示为原始时间(1月1日08:00)(对比另一测试机无此现象)》手动对时后恢复 2/10 Q27	 监控	 高	W_王 国君	 2：严重的	W_吴 能田	G1605A	 2016-11-22

59577	 GNSPR	【品质压力】连接着充电器，手机电量89%，强制重启手机后电量显示为98%，再次操作电量还是98%【附视频】单机一次 余前志	 分配	 普通	L_李 路宝	 3：中等的	Z_张 醒醒	G1605A	 2016-12-04

3，另外 uart switch、 指纹rpmb标记 也存在这里，也有问题；

我试了 关机闹钟 7秒 强制断电关机后也不行； 不自动对时 开机闹钟也不行；
是否还有其他日历、等问题，没有太多去看；


由于硬件是7秒 强制断电，软件强制关机 时间是11秒，硬件在软件之前 所以会导致MTK原有的强制关机功能不能触发； 理论上应该是 软件强制关机不起作用 再需要硬件强制断电功能；
目前这方面问题不少，建议暂时先去掉这个硬件功能，如果不能去掉，建议缩短断电时间（目前是400mS），增大RTC旁边电容，争取不让rtc丢失；
USB检测那段时间大概也是6秒多，有些BUG可能是关机充电按power开机，USB检测6秒多后震动，显示log，但是按了power7s导致的刚开机有关机了。

这个理解可能准确点
这个问题是关机充电状态下（出了充电图标），按power7s（硬件的强制关机），这个过程是先调用power_off->reset->正常开机
reset有两三秒时间







--->shutdown_gauge
	电池电压还很高，手机就关机了....
	这里涉及到几个关机检测策略问题，还有0%检测上报问题

	手机电压达到关机电压；电量0%维持一定的时间倒计时关机；底层上报0%后，上层传达命令关机；

eservice：
	{
		Dear MTK，
		[kernel]AvgVbat 3552,bat_vol 3549, AvgI 0, I 0, VChr 0, AvgT 35, T 36, ZCV 3678
		从log看 电池电压还在3.55V 就关机了； 代码里已经注掉了 SHUTDOWN_GAUGE0，请帮忙分析解决；ThankS！
		目前代码里相关定义如下：
		#define CUST_SYSTEM_OFF_VOLTAGE 3385

		#define Q_MAX_BY_SYS
		#define Q_MAX_SYS_VOLTAGE        3385
		//#define SHUTDOWN_GAUGE0		 //
		#define SHUTDOWN_GAUGE1_XMINS
		#define SHUTDOWN_GAUGE1_MINS    240	//电量在1%维持240s，倒计时后就关机

		#define SHUTDOWN_SYSTEM_VOLTAGE    3385 //系统关机电压

		#define CUST_SYSTEM_OFF_VOLTAGE 3385

		因为gm2.0关机判断逻辑是

		1.bat_vol<shutdown_system_voltage

		2.shutdown_gauge0==1&&fg_capacity_by_c==0

		3.if(shutdown_gauge1_xmins==1&&ui_soc2==1)
		判断duration time*60是否超时

		0% 是在batteryservicey跑的关机流程，不是kernel判断shutdown_cond了
		（0%电量要上报到上层上层判断0%才能跑关机流程，然后传输命令，执行关机）
		/frameworks/base/services/core/java/com/android/server/BatteryService.java

		参考processValuesLocked

		会发request
		ACTION_REQUEST_SHUTDOWN

		/* mt_battery_shutdown_check(); move to user space */

		这正式是我们要解决的问题啊，//#define SHUTDOWN_GAUGE0 注掉此红开关，应该要等电压的啊?
		如果batteryservice是会关机，GM2.0就不能让uisoc2为0啊? 这到底是哪里的问题呢?

        Dear sir
		但是我不是很理解，这是fuel gauge积分到了0，本来就有到0的情况发生啊

		UI_SOC为0，直接赋值UI_SOC2为0了呀？

		这是正常的，关机条件batteryservice都可以改

		您现在预期要什么效果呢？？

		battery_update
		//充电器不在位，且未满足关机条件：
			}
			else
			{        /* Only Battery */		但是这个判断代码里没有
				if(shutdown_cond > 0) {
					FGLOG_DEBUG("[FGADC_SHUT] %s\n",
						(shutdown_cond == 1)? "0%SOC shutdown":((shutdown_cond==2)?"30min shutdown":"voltage low shutdown"));
					uisoc_0Percent_tracking();
				} else {


		void uisoc_0Percent_tracking(void)
		{
			static int timer_counter = 10;
			int percent=0;

			if (ui_soc <= 0) {
				ui_soc = 0;
			} else {
				percent=battery_duration_time/timer_counter;

				ui_soc=ui_soc-percent;

				if(ui_soc<=0)
					ui_soc=0;
			}


		void uisoc_update_uisoc2(void)
		{
			int pseudoUI;
			static int pre_pseudoUI;
			static int smooth_uisoc2_cnt = 0;
		FGLOG_NOTICE("[uisoc_update_uisoc2]pseudoUI=%d,pre_pseudoUI=%d,ui_soc=%d,batterypseudo1=%d\n",pseudoUI,pre_pseudoUI,ui_soc,batterypseudo1);
			pseudoUI=(((ui_soc-batterypseudo1))*100+(batterypseudo100-batterypseudo1)/2)/(batterypseudo100-batterypseudo1);
		FGLOG_NOTICE("[uisoc_update_uisoc2]ui_soc2=%d,ui_soc=%d,pseudoUI=%d\n",ui_soc2,ui_soc,pseudoUI);
			if(ui_soc2==-100) {
				if(pseudoUI<=0) {
					if(is_kpoc())
						ui_soc2=0;
					else
						ui_soc2=1;
				} else if(pseudoUI>=100) {
					ui_soc2=100;
				} else {
					ui_soc2=pseudoUI;
				}
			} else {
				if(ui_soc==0) {
					ui_soc2=0;


			}

			Any update?
			您可以在kernel自己改一下
			UI_SOC2为0的话改回1

			在
			bmd_crtl_cmd_from_user

			case FG_DAEMON_CMD_SET_UI_SOC2:
			{

			BMT_status.UI_SOC2=UI_SOC;
			之后加上判断
			若为0
			改为1即可

			这样就会限制在kernel只有shutdown cond！0的时候才会关机了

			thanks


GNSPR#65245
     现象：
     手机关机充电，连接充电宝（13000mAh或20000mAh）充电到96%时，充电宝LED灯熄灭，手机会自动亮屏，有振动，无法继续充电，手动开机后可正常充电

	 原因：
	 充电宝给手机充电时，充电宝和手机都有对充电电流的检测，当充电快充满时，充电电流很小，充电宝端检测到充电电流很小，就断开充电
	 
	 

	 
}*/














	













USB&&OTG相关的
/*{

需要注意的地方：
	1.USB眼图？
	2.OTG开关不能只开关中断，原因跟USB的热插拔相似，插入USB（插入模块，创建设备文件，节点），拔除USB（关闭中断，想上层上报信息，，关闭中断，卸载模块）。
	3.一些问题有待新版本测试，以前版本OTG和反向充电开关默认是关闭的



GNSPR#54386 	
	现象：测机连接USB至显示充电不弹选项，计算机查看无法识别USB，插拔数据线不恢复 ，重启恢复



GNSPR#57228
	现象：【自动化monkey 72h测试】手机adb连接异常，导致测试不通过。
		手机数据能连上，能充电，就是adb会断开，之前不进行monkey72h正常，但是进行了monkey之后部分机器会出现adb连击不上的
		情况
		（是否是应用软件冲突的原因-monkey72h，其他都能连接上，应该不是硬件的原因（硬件一般是什么原因会造成连接不上））	
		
		手机IMEI1和IMEI2默认设置为999999999999994（此操作目的：自动化测试服务器会把该IMEI号手机的USB调试默认为打开状态）
	
	手机adb链接出现问题的常见原因：
		adb：		
	首先adb的意思是手机打开USB调试并连接电脑后，在电脑上自动运行的一个进程，相当于驱动程序。
	出现上述情况的原因一般是驱动程序安装错误或者电脑及手机上有其他程序发生了冲突


GNSPR #53212 	 OTG：连接2A旅充头，简单操作测机10min，未充进电，充电前电量显示57%，充电后点电量显示57%（未做过大量重启、开关机操作）【随机一次】A29



GNSPR #52967	 OTG：G1605手机插上一拖一反充线给M6手机反充电，充电10分钟后，查看充电电流，其中有值低于800mA（附图）。



GNSPR #53732 	 OTG：设置中反向充电和ＭＭＩ反充充电，切换后，设置中开启反向充电和ＭＭＩ里面都不能反向充电。


这两个问题都是充电频繁断开，跟OTG底层接口有关
GNSPR #52977	 OTG：测试机G1605给另一个G1605和S8充电，两台被充电的手机一直很频繁充电/不充电（附视频）
GNSPR #52885	 OTG：使用G1605给S8反向充电，频繁充电过程出现中断，但很快又连接上的问题（附视频）


OTG失败，USB也不显示，USB五个引脚的作用？


GNSPR#53131
			现象：关机状态连充电器，长按电源键3s开机，10s后才有振动提示和开机画面 对比S9无【6台必现】A9

	这个问题跟平台有一定的关系
MTK提供的解决方法
	1.方案一：这种方法在于在握手之前先判断是否是长按了音量下（强制刷机）,个人觉得这种方案是可以的，刷机本来就是用组合键
	   Sure, you can add keypad check function before usb_handshake() as you point out.
	We already provided the keypad check API in preloader. For example, you can use: mtk_detect_key (MTK_PMIC_RST_KEY)
	We have found the cause as below. We will commit it soon, you can also take a try.
	bootable/bootloader/preloader / platform/mt6735/src/drivers/pmic.c
	Only when KCOL is enable(force download), it is possible to use the MTK download tool.
	We will modify that keypad check for mtk download tool is disable. (CFG_USB_DOWNLOAD :=0 / CFG_EMERGENCY_DL_SUPPORT :=0)
	Please modify that USB handshake function is possible when keypad is pressed,
	For example
		    #if CFG_USB_TOOL_HANDSHAKE
		    (Add keypad check function)
		    if (TRUE == usb_handshake(handler))
		           g_meta_com_type = META_USB_COM;
		           BOOTING_TIME_PROFILING_LOG("USB handshake");
		    #endif
		    #if CFG_
	disable “CFG_USB_TOOL_HANDSHAKE”, the above problems will not exist anymore.
	But you will not be able to use USB download and USB Meta tool via preloader.
	If you think that is ok, we don’t have other concerns.

	cust_bldr.mak

	alps\bootable\bootloader\preloader\custom\[project]\cust_bldr.mak
	CFG_UART_TOOL_HANDSHAKE :=0


		cust_bldr.mak:
		CFG_UART_TOOL_HANDSHAKE :=0

	default.mak
		MTK_PMIC_RST_KEY :=1

	main.c

	    #if CFG_USB_TOOL_HANDSHAKE
	    if( mtk_detect_key (MTK_PMIC_RST_KEY) ){
		if (TRUE == usb_handshake(handler))
                g_meta_com_type = META_USB_COM;
		    BOOTING_TIME_PROFILING_LOG("USB handshake");
	    }
	   /*GioneeDrv LiLuBao 20161121 modify for fixed GNSPR#53131 end
        #endif


	2.方案二：修改检测延迟的时间timeout，不见效
	修改usb tool hanshake timour的时间为1s，这个修改可能是影响tool的连接使用，所以建议修改不能比1s少
	这里设置为1s
	#if !defined(CFG_USB_HANDSHAKE_TIMEOUT)
	-#define CFG_USB_HANDSHAKE_TIMEOUT (2500) /* 2.5s */
	+#define CFG_USB_HANDSHAKE_TIMEOUT (1000) /* 1s */
	#endif
	b>去掉uart hanshake
	/vendor/mediatek/proprietary/bootable/bootloader/preloader/platform/mt6755/default.mak
	CFG_UART_TOOL_HANDSHAKE :=0
	c>把dlpt初始化的位置挪到后面（从preloader移到lk阶段，但是刷机延迟时间就长了）

	#define CFG_USB_ENUM_TIMEOUT            (8000)           /* 8s */
	USB枚举时间超时，改变之后影响座充充电时间

	#define CFG_USB_HANDSHAKE_TIMEOUT       (2500)    	 /* 2.5s */
	USB握手时间超时，改变之后影响了用USB充电的时间

	3.方案三：
	将dlpt_init_inLK()移动到如下，在6s左右:如上传log：session--usb-44444.log
	#ifdef LK_PROFILING
	dprintf(CRITICAL, "[PROFILE] ------- show logo takes %d ms --------
	", (int)get_timer(time_show_logo));
	#endif
	/*add bebug by mtk ALPS02765977 20160624*/
	dlpt_init_inLK();


FAQ18845
	[REPRODUCTIVE]
          1, 6735/6753/6735M 手机
          2， 手机USB线连接 AC Adapter 充电器
          3， 查看uart log
 
          抓取uart log 看到：
        force STANDARD_HOST

		[PLFM] USB cable in
		[14:49:50][pmic_IsUsbCableIn] 1
		[14:49:51][pmic_IsUsbCableIn] 1
		pl pmic powerkey Release
		[14:49:52][pmic_IsUsbCableIn] 1
		pl pmic powerkey Release
		[14:49:53][pmic_IsUsbCableIn] 1
		pl pmic powerkey Release
		[14:49:54][pmic_IsUsbCableIn] 1
		pl pmic powerkey Release
		[14:49:55][pmic_IsUsbCableIn] 1
		pl pmic powerkey Release
		[14:49:56][pmic_IsUsbCableIn] 1
		pl pmic powerkey Release
		[14:49:57][pmic_IsUsbCableIn] 1
		pl pmic powerkey Release
 
[ANALYSIS]          
          为减小preloader size 默认强制USB 类型赋值为 STANDARD_HOST ，此情况下会去执行USB 枚举。 但是实际上是AC Adapter，因此会不断的枚举直到超时。枚举时间正好是8S。
 
[SOLUTION]
          /vendor/mediatek/proprietary/bootable/bootloader/preloader/platform/mt6735/src/drivers/pmic.c
           

118#ifndef PMIC_TYPE_DETECTION
119
120int hw_charger_type_detection(void)
121{
122    print("force STANDARD_HOST\r\n");
123    return STANDARD_HOST;
124}

      在make file 中定义 PMIC_TYPE_DETECTION 宏即可。
 
      如果开启该宏后引起preloader size 过大导致build 失败或者下载失败，则请参考FAQ15079 FAQ11794  处理。






GNSPR#70216
	mmi测试导致的充电失败
	
	OTG开关的工作流程：
	(battery_common_fg_20.c)show_otg_charge_switch_State,底层驱动提供给上层的节点，上层写值后通过调用这里向下调用，有两个状态位
	gn_otg_charge_switch_State，otg_charge_state；判断接受到的otg_charge_state状态是否为1，与gn_otg_charge_switch_State状态是否一样
	->(usb20_host.c)打开还是关闭这里的open和close函数之重点,关于中断使能，控制逻辑还要理解->mt_usb_ext_iddig_int当有otg中断来的时候
	调用musb_id_pin_work,对应的工作队列 ->musb_is_host判断插入的usb是host还是devices->musb_start配置一些usb传输的信息，改变usb状态信息，
	将usb主设备转变成从设备->switch_int_to_device设置iddig的中断使能，完成转变成从设备->工作队列任务完成
	
	为什么要设置vbus
	
开OTG使能	
	[ 1778.094714] <1>.(1)[3202:Binder_10][name:battery_common_fg_20&]show_otg_charge_switch_State = 0
	[ 1778.202733] <1>.(1)[1647:Binder_1][name:usb20_host&]****before gn_Open_Otg_Irq!
	[ 1778.312694] <1>.(1)[1647:Binder_1][name:usb20_host&][MUSB]gn_Open_Otg_Irq 751: iddig_state = 1
	[ 1778.352742] <1>.(1)[1647:Binder_1][name:usb20_host&][MUSB]gn_Open_Otg_Irq 759: gn_Open_Otg_Irq is done
	[ 1778.353909] <1>.(1)[1647:Binder_1][name:battery_common_fg_20&]store_otg_charge_switch_State = 1


	[ 1782.082073] <0>-(0)[0:swapper/0][name:usb20_host&][MUSB]mt_usb_ext_iddig_int 500: id pin interrupt assert
	[ 1782.472966] <0>.(0)[8137:kworker/0:0][name:usb20_host&][MUSB]musb_id_pin_work 412: work start, is_host=0, boot mode(0)
	[ 1782.474333] <0>.(0)[8137:kworker/0:0][name:usb20_host&][MUSB]musb_is_host 271: will mask PMIC charger detection
	[ 1782.475602] <0>.(0)[8137:kworker/0:0][name:usb20&][MUSB]mt_usb_enable 292: <0,0>,<4,4,3,3>
	[ 1782.476687] <0>.(0)[8137:kworker/0:0][name:usb20&][MUSB]mt_usb_enable 313: enable UPLL before connect
	[ 1782.477830] <0>.(0)[8137:kworker/0:0][name:usb20&][MUSB]vcore_hold 154: before releasing
	[ 1782.478838] <0>.(0)[8137:kworker/0:0][name:usb20&][MUSB]vcore_hold 157: after releasing
	[ 1782.487633] <0>.(0)[8137:kworker/0:0][name:usb20&][MUSB]vcore_hold 163: hold VCORE ok
	[ 1782.501709] <0>.(0)[8137:kworker/0:0][name:usb20_phy&][MUSB]HQA_special 22: HQA, 0x18, before:86
	[ 1782.502961] <0>.(0)[8137:kworker/0:0][name:usb20_phy&][MUSB]HQA_special 26: HQA, 0x18, after:86
	[ 1782.504054] <0>.(0)[8137:kworker/0:0][name:usb20_phy&][MUSB]hs_slew_rate_cal 271: [USBPHY]slew calibration:FM_OUT =326,x=4221,value=4
	[ 1782.505550] <0>.(0)[8137:kworker/0:0][name:usb20_phy&][MUSB]usb_phy_recover 644: usb recovery success
	[ 1782.506695] <0>.(0)[8137:kworker/0:0][name:usb20&][MUSB]mt_usb_enable 328: <4,4,4,3>

	[ 1782.507664] <0>.(0)[8137:kworker/0:0][name:usb20_host&][MUSB]musb_is_host 289: iddig_state = 0
	[ 1782.508733] <0>.(0)[8137:kworker/0:0][name:usb20_host&][MUSB]musb_is_host 326: usb_is_host = 1
	[ 1782.509807] <0>.(0)[8137:kworker/0:0][name:usb20_host&][MUSB]musb_id_pin_work 425: musb is as host
	[ 1782.511989] <0>.(0)[8137:kworker/0:0][name:usb20_host&][MUSB]mt_usb_set_vbus 91: mt65xx_usb20_vbus++,is_on=1
	[ 1782.625000] <0>.(0)[8137:kworker/0:0][name:usb20_host&][MUSB]musb_id_pin_work 446: force PHY to idle, 0x6d=3f, 0x6c=11
	[ 1782.631378] <0>.(0)[8137:kworker/0:0][name:usb20_host&][MUSB]musb_id_pin_work 457: force PHY to host mode, 0x6d=3f, 0x6c=2d
	[ 1782.633531] <0>.(0)[8137:kworker/0:0][name:musb_hdrc&][MUSB]musb_start 1221: start, is_host=1 is_active=0
	[ 1782.634807] <0>.(0)[8137:kworker/0:0][name:usb20&][MUSB]mt_usb_enable 292: <1,1>,<5,4,4,3>
	[ 1782.635877] <0>.(0)[8137:kworker/0:0][name:musb_hdrc&][MUSB]musb_start 1286: set ignore babble MUSB_ULPI_REG_DATA=89
	[ 1782.637236] <0>.(0)[8137:kworker/0:0][name:usb20_host&][MUSB]switch_int_to_device 362: switch_int_to_device is done
	[ 1782.638566] <0>.(0)[8137:kworker/0:0][name:usb20_host&][MUSB]musb_id_pin_work 487: work end, is_host=1

	[ 1782.804798] <0>-(0)[8137:kworker/0:0][name:musb_hdrc&][MUSB]musb_stage0_irq 967: MUSB_INTR_CONNECT (b_idle)
	[ 1782.804816] <0>-(0)[8137:kworker/0:0][name:musb_qmu&]QMU_WARN,<musb_disable_q_all 55>, disable_q_all
	[ 1782.805098] <0>-(0)[8137:kworker/0:0][name:musb_hdrc&][MUSB]musb_stage0_irq 1042: CONNECT (a_host) devctl 3d
	
	
关闭OTG使能
	[ 1783.421102] <0>.(0)[211:mtk charger_hv_][name:battery_common_fg_20&][upmu_is_chr_det] Charger exist but USB is host
	[ 1784.426584] <0>.(0)[211:mtk charger_hv_][name:battery_common_fg_20&][upmu_is_chr_det] Charger exist but USB is host
	[ 1784.734018] <0>.(0)[933:batterywarning][name:battery_common_fg_20&][Battery] show_BatteryNotify : 0
	[ 1785.435856] <0>.(0)[211:mtk charger_hv_][name:battery_common_fg_20&][upmu_is_chr_det] Charger exist but USB is host
	[ 1785.912684] <2>.(2)[2884:Binder_A][name:usb20_host&]****before gn_Close_Otg_Irq!
	[ 1785.913612] <2>.(2)[2884:Binder_A][name:usb20_host&][MUSB]gn_Close_Otg_Irq 695: iddig_state = 0
	[ 1785.914715] <1>-(1)[0:swapper/1][name:usb20_host&][MUSB]mt_usb_ext_iddig_int 500: id pin interrupt assert
	[ 1786.312716] <1>.(1)[6996:kworker/1:4][name:usb20_host&][MUSB]musb_id_pin_work 412: work start, is_host=1, boot mode(0)
	[ 1786.314066] <1>.(1)[6996:kworker/1:4][name:usb20_host&][MUSB]musb_is_host 271: will mask PMIC charger detection
	[ 1786.315340] <1>.(1)[6996:kworker/1:4][name:usb20&][MUSB]mt_usb_enable 292: <1,1>,<6,4,4,3>
	[ 1786.316415] <1>.(1)[6996:kworker/1:4][name:usb20_host&][MUSB]musb_is_host 289: iddig_state = 0
	[ 1786.317484] <1>.(1)[6996:kworker/1:4][name:usb20_host&][MUSB]musb_is_host 317: will unmask PMIC charger detection
	[ 1786.318774] <1>.(1)[6996:kworker/1:4][name:usb20_host&][MUSB]musb_is_host 326: usb_is_host = 0
	[ 1786.319840] <1>.(1)[6996:kworker/1:4][name:usb20_host&][MUSB]musb_id_pin_work 425: musb is as device
	[ 1786.321262] <1>.(1)[6996:kworker/1:4][name:usb20_host&][MUSB]musb_id_pin_work 465: devctl is 3d
	[ 1786.322349] <1>.(1)[6996:kworker/1:4][name:usb20_host&][MUSB]mt_usb_set_vbus 91: mt65xx_usb20_vbus++,is_on=0
	[ 1786.327596] <1>.(1)[6996:kworker/1:4][name:usb20_host&][MUSB]musb_id_pin_work 478: force PHY to idle, 0x6d=3f, 0x6c=11
	[ 1786.328928] <1>.(1)[6996:kworker/1:4][name:usb20&][MUSB]mt_usb_disable 340: <1,1>,<6,5,4,3>
	[ 1786.330809] <1>.(1)[6996:kworker/1:4][name:usb20_phy&][MUSB]usb_phy_savecurrent 536: usb save current success
	[ 1786.332041] <1>.(1)[6996:kworker/1:4][name:usb20&][MUSB]mt_usb_disable 358: <6,5,4,4>
	[ 1786.333122] <1>-(1)[6996:kworker/1:4][name:usb20&][MUSB]vcore_release 195: musb lock get, release it, mtk_musb:ffffffc0b5efc308
	[ 1786.346376] <1>.(1)[6996:kworker/1:4][name:usb20&][MUSB]vcore_release 202: release VCORE ok
	[ 1786.348502] <1>.(1)[6996:kworker/1:4][name:musb_hdrc&][MUSB]musb_stop 1357: HDRC disabled
	[ 1786.349825] <1>.(1)[6996:kworker/1:4][name:usb20_host&][MUSB]switch_int_to_host 380: switch_int_to_host is done
	[ 1786.351234] <1>.(1)[6996:kworker/1:4][name:usb20_host&][MUSB]musb_id_pin_work 487: work end, is_host=0
	[ 1786.352618] <1>.(1)[0:swapper/1][name:usb20&][MUSB]musb_do_idle 248: otg_state b_idle

	
	
	gn_otg_charge_switch_State：上层写设备节点，传下来的状态
	gn_otg_charge_switch_Closing这个是为了关闭开关后让检测引脚看成是高电平实际上是低电平
	iddig_state：iddig引脚的状态
	otg_charge_state：otg充电状态
	
	
	
	
	void gn_Close_Otg_Irq(void)
	{
		int iddig_state = 1;
		pr_notice("****before gn_Close_Otg_Irq!\n");	
	#ifdef ID_PIN_USE_EX_EINT
		#ifdef CONFIG_OF
		#if defined(CONFIG_MTK_LEGACY)
		iddig_state = mt_get_gpio_in(iddig_pin);
		#else
		iddig_state = __gpio_get_value(iddig_pin);
		#endif
		DBG(0, "iddig_state = %d\n", iddig_state);

		if(iddig_state)
		{
			//Gionee GuoJianqiu 201601026 modify for GNSPR #70216 begin
			gn_otg_charge_switch_Closing = KAL_TRUE;
			//Gionee GuoJianqiu 201601026 modify for GNSPR #70216 end
			disable_irq(usb_iddig_number);
			irq_set_irq_type(usb_iddig_number, IRQF_TRIGGER_LOW);
			msleep(10);
			pinctrl_select_state(pinctrl, pinctrl_iddig_close);
			msleep(20);
		}
		else
		{
			gn_otg_charge_switch_Closing = KAL_TRUE;
			irq_set_irq_type(usb_iddig_number, IRQF_TRIGGER_LOW);
			msleep(500);
			pinctrl_select_state(pinctrl, pinctrl_iddig_close);
			msleep(100);
		}

		/*
		disable_irq(usb_iddig_number);
		irq_set_irq_type(usb_iddig_number, IRQF_TRIGGER_LOW);
		msleep(10);
		if(!iddig_state)
		{
		gn_musb_id_pin_work();
		msleep(300);
		}
		pinctrl_select_state(pinctrl, pinctrl_iddig_close);
		if(iddig_state)
			msleep(20);
		else
			msleep(200);
		*/
		#endif
	#endif
		DBG(0, "gn_Close_Otg_Irq is done\n");
	}



	void gn_Open_Otg_Irq(void)
	{
		int iddig_state = 1;
		pr_notice("****before gn_Open_Otg_Irq!\n");	
	#ifdef ID_PIN_USE_EX_EINT
		#ifdef CONFIG_OF		
		pinctrl_select_state(pinctrl, pinctrl_iddig);
		msleep(100);
		gn_otg_charge_switch_Closing = KAL_FALSE;
		irq_set_irq_type(usb_iddig_number, IRQF_TRIGGER_LOW);
		//Gionee GuoJianqiu 201601026 modify for GNSPR #70216 begin
		irq_set_irq_type(usb_iddig_number, IRQF_TRIGGER_LOW);
		//Gionee GuoJianqiu 201601026 modify for GNSPR #70216 end
		enable_irq(usb_iddig_number);	
		//Gionee GuoJianqiu 201601026 modify for GNSPR #70216 begin
		enable_irq(usb_iddig_number);	
		DBG(0, "gn_Set_Otg_Irq again!!\n");
		//Gionee GuoJianqiu 201601026 modify for GNSPR #70216 end
	
		#if defined(CONFIG_MTK_LEGACY)
		iddig_state = mt_get_gpio_in(iddig_pin);
		#else
		iddig_state = __gpio_get_value(iddig_pin);
		#endif
		DBG(0, "iddig_state = %d\n", iddig_state);
	
		if(iddig_state)
			msleep(30);
		else
			msleep(350);
		#endif
	#endif
		DBG(0, "gn_Open_Otg_Irq is done\n");
	}
	#endif
	//Gionee GuoJianqiu 201601026 modify for OTG SWITCH end
	
	
	
	
	
	
	
	
	
	


}*/



	













thermal温度控制相关的
/*{

	首先thermal.txt文件内的相关参数要知道什么意思，怎么改更合理，有效。

	相关的几个case(12时00分12秒)ALPS02710500 ，ALPS02796764这个是温升降低充电电流的 


--->温升CPU降核，降频问题
	在如下时间点系统发生cpu占用率高，导致系统弹出cpu占有率高的对话框，请驱动同事做进一步分析！

	11-24 17:13:38.570567 <7>[25342.267555] (2)[509:AALLightSensor][name:aal_control&][ALS/AAL]Get als dat :40
	11-24 17:13:38.718625 <3>[25342.415613] (0)[11158:kworker/0:2]<STEP_COUNTER> step_c data48
	11-24 17:13:38.728131 <4>[25342.425119] (0)[11236:kworker/0:3][Power/CPU_Thermal]set_adaptive_cpu_power_limit 1621 T=63200,1734 T=62600,1884 T=61500,2010 T=61900,0 T=58700
	11-24 17:13:38.728154 <4>[25342.425142] (0)[11236:kworker/0:3][Power/cpufreq] mt_cpufreq_thermal_protect(): limited_power = 1384
	11-24 17:13:38.728170 <7>[25342.425158] (0)[11236:kworker/0:3][name:mt_cpufreq&][Power/cpufreq] @mt_cpufreq_thermal_protect: found = 1, limited_power_idx = 1, limited_max_freq = 1235000, limited_max_ncpu = 4
	11-24 17:13:38.728319 <7>[25342.425307] (0)[11236:kworker/0:3][name:mt_cpufreq&][Power/cpufreq] @_mt_cpufreq_set_locked(): Vproc = 1112mv, freq = 1235000 KHz
	11-24 17:13:38.728345 <7>[25342.425333] (0)[11236:kworker/0:3][name:mtk_cooler_atm&][Power/CPU_Thermal]set_adaptive_gpu_power_limit 600
	11-24 17:13:38.770063 <7>[25342.467051] (0)[11236:kworker/0:3][name:mtk_ts_bts&][Power/BTS_Thermal] T_AP=51000
	11-24 17:13:43.660714 <7>[25347.357702] (0)[507:AALServiceMain][name:ddp_pwm&][PWM] (latest= 5): 145( 823, 575) 144( 823, 592) 143( 823, 609) 141( 823, 643) 140( 823, 660)
	11-24 17:13:43.728026 <4>[25347.425014] (0)[11158:kworker/0:2][Power/cpufreq] mt_cpufreq_thermal_protect(): limited_power = 1420
	11-24 17:13:43.728049 <7>[25347.425037] (0)[11158:kworker/0:2][name:mt_cpufreq&][Power/cpufreq] @mt_cpufreq_thermal_protect: found = 1, limited_power_idx = 1, limited_max_freq = 1235000, 


	温度升高导致系统启动温度预警，限制CPU频率，核的数量，最终导致CPU负载过大，系统卡顿

	set_adaptive_cpu_power_limit（）

	adaptive_limit：
	1621 T=63200,
	1734 T=62600,
	1884 T=61500,
	2010 T=61900,
	0    T=58700

	mt_cpufreq_thermal_protect()
	limited_power = 1384

	mt_cpufreq_thermal_protect（）
	found = 1,
	limited_power_idx = 1,
	limited_max_freq = 1235000,

	_mt_cpufreq_set_locked()
	Vproc = 1112mv, 
	freq = 1235000 KHz

	set_adaptive_gpu_power_limit（）
	600


	T_AP=51000

	[PWM] (latest= 5): 145( 823, 575) 144( 823, 592) 143( 823, 609) 141( 823, 643) 140( 823, 660)

	mt_cpufreq_thermal_protect()
	limited_power = 1420

	mt_cpufreq_thermal_protect（）
	found = 1, 
	limited_power_idx = 1,
	limited_max_freq = 1235000, 


	#if THERMAL_HEADROOM
	static int p_Tpcb_correlation;
	static int Tpcb_trip_point;
	static int thp_max_cpu_power;
	static int thp_p_tj_correlation;
	static int thp_threshold_tj;
	#endif


	static struct mt_cpu_dvfs cpu_dvfs[] = {
		[MT_CPU_DVFS_LITTLE] = {
			.name = __stringify(MT_CPU_DVFS_LITTLE),
			.cpu_id = MT_CPU_DVFS_LITTLE,	/* TODO: FIXME */
			.cpu_level = CPU_LEVEL_1,	/* FY segment */
			.ops = &dvfs_ops,
	#ifdef CONFIG_CPU_DVFS_TURBO_MODE
			.turbo_mode = 0,
	#endif
	#ifdef CONFIG_CPU_DVFS_POWER_THROTTLING
			.idx_opp_tbl_for_pwr_thro = -1,
			.idx_pwr_thro_max_opp = 0,
	#endif
		},
	};


	#elif defined(CONFIG_ARCH_MT6735M)
	/* CPU LEVEL 0, 1.2GHz segment */
	static struct mt_cpu_freq_info opp_tbl_e1_0[] = {
		OP(CPU_DVFS_FREQ0, 125000),
		OP(CPU_DVFS_FREQ1, 121875),
		OP(CPU_DVFS_FREQ4, 118750),
		OP(CPU_DVFS_FREQ6, 115000),
		OP(CPU_DVFS_FREQ7, 110000),
		OP(CPU_DVFS_FREQ8, 105000),
		OP(CPU_DVFS_FREQ9, 100000),
		OP(CPU_DVFS_FREQ10, 95000),
	};

	/* CPU LEVEL 1, 1GHz segment */
	static struct mt_cpu_freq_info opp_tbl_e1_1[] = {
		OP(CPU_DVFS_FREQ2, 125000),
		OP(CPU_DVFS_FREQ3, 121875),
		OP(CPU_DVFS_FREQ5, 118750),
		OP(CPU_DVFS_FREQ6, 115000),
		OP(CPU_DVFS_FREQ7, 110000),
		OP(CPU_DVFS_FREQ8, 105000),
		OP(CPU_DVFS_FREQ9, 100000),
		OP(CPU_DVFS_FREQ10, 95000),
	};

	/* CPU LEVEL 2, 1.25GHz segment */
	static struct mt_cpu_freq_info opp_tbl_e1_2[] = {
		OP(CPU_DVFS_FREQ0_1,125000),
		OP(CPU_DVFS_FREQ1,  121875),
		OP(CPU_DVFS_FREQ5,  118750),
		OP(CPU_DVFS_FREQ6,  115000),
		OP(CPU_DVFS_FREQ7,  110000),
		OP(CPU_DVFS_FREQ8,  105000),
		OP(CPU_DVFS_FREQ9,  100000),
		OP(CPU_DVFS_FREQ10, 95000),
	};

	/* CPU LEVEL 3, 1.1GHz segment */
	static struct mt_cpu_freq_info opp_tbl_e1_3[] = {
		OP(CPU_DVFS_FREQ1_1,125000),
		OP(CPU_DVFS_FREQ2,  121875),
		OP(CPU_DVFS_FREQ5,  118750),
		OP(CPU_DVFS_FREQ6,  115000),
		OP(CPU_DVFS_FREQ7,  110000),
		OP(CPU_DVFS_FREQ8,  105000),
		OP(CPU_DVFS_FREQ9,  100000),
		OP(CPU_DVFS_FREQ10, 95000),
	};

	static struct opp_tbl_info opp_tbls[] = {
		[CPU_LEVEL_0] = {opp_tbl_e1_0, ARRAY_SIZE(opp_tbl_e1_0)},
		[CPU_LEVEL_1] = {opp_tbl_e1_1, ARRAY_SIZE(opp_tbl_e1_1)},
		[CPU_LEVEL_2] = {opp_tbl_e1_2, ARRAY_SIZE(opp_tbl_e1_2)},
		[CPU_LEVEL_3] = {opp_tbl_e1_3, ARRAY_SIZE(opp_tbl_e1_3)},
	};




	mtktscpu  (IC junction temperature)

	ATM (adaptive thermal management)

	overshooting 过辐射

	opp ：operating performance point 操作性能点

	

	收敛

	/proc/driver/thermal/clatm_setting
	0  2010 15 30 1 720 2010 600 1050 

	0  	id
	2010    firststep
	15	theta(ja)fall
	30	theta(js)rise	 （fall/rise 比值越小变化越快）	
	1	minbudgetchange （每次最少要变化的大小）
	720	minCPUpower(mW)
	2010	maxCPUpower(mW)
	600	minGPUpower(mW)
	1050	maxGPUpower(mW)


	/proc/driver/thermal/clctm
	1 85000 66000 38999 41999 75000 60000 331980 6333 269995 5000   

	1 	ctm on
	85000	Target Tj0
	66000	Target Tj2
	38999	Tpcb1
	41999	Tpcb2
	75000	exit Tj0
	60000	exit Tj2
	331980	enter_a
	6333	enter_b
	269995	exit_a
	5000	exit_b


	thermal温度高想降频，但降频会导致系统卡顿，无响应，温升和降频是一对互斥的

	温度容易升高，同时系统容易卡顿......

	整理一下逻辑：温度升高，系统启动mt_cpufreq_thermal_protect，降压，降频，限制CPU核，导致系统无响应，降频过快还是跨度过大，还是CPU核减的太少


	Cooler Name    ctm    ENABLE
	Extra    ctm on    1
	Extra    Target Tj 0    85000
	Extra    Target Tj 2    66000
	Extra    Tpcb 1    38999
	Extra    Tpcb 2    41999
	Extra    Exit Tj 0    75000
	Extra    Exit Tj 2    60000
	Extra    Enter_a    331980
	Extra    Enter_b    6333
	Extra    Exit_a    269995
	Extra    Exit_b    5000

	算法：
	Tj = min(85, max(66, (Enter_a – Enter_b*bts_cur_temp/1000)/1000))

	Exit tj = min(75, max(60, (Exit_a – Exit_b*bts_cur_temp/1000)/1000))


	//通过这个函数获取CPU动态调节的策略
	p = id_to_cpu_dvfs(_get_cpu_dvfs_id(policy->cpu));

	[MT_CPU_DVFS_LITTLE/0]
	[0] = { .cpufreq_khz = 1300000,	.cpufreq_ncpu = 4,	.cpufreq_power = 1379, },
	[1] = { .cpufreq_khz = 1235000,	.cpufreq_ncpu = 4,	.cpufreq_power = 1271, },
	[2] = { .cpufreq_khz = 1170000,	.cpufreq_ncpu = 4,	.cpufreq_power = 1157, },
	[3] = { .cpufreq_khz = 1300000,	.cpufreq_ncpu = 3,	.cpufreq_power = 1034, },
	[4] = { .cpufreq_khz = 1235000,	.cpufreq_ncpu = 3,	.cpufreq_power = 953, },
	[5] = { .cpufreq_khz = 1040000,	.cpufreq_ncpu = 4,	.cpufreq_power = 935, },
	[6] = { .cpufreq_khz = 1170000,	.cpufreq_ncpu = 3,	.cpufreq_power = 867, },
	[7] = { .cpufreq_khz = 1040000,	.cpufreq_ncpu = 3,	.cpufreq_power = 701, },
	[8] = { .cpufreq_khz = 1300000,	.cpufreq_ncpu = 2,	.cpufreq_power = 689, },
	[9] = { .cpufreq_khz = 819000,	.cpufreq_ncpu = 4,	.cpufreq_power = 685, },
	[10] = { .cpufreq_khz = 1235000,	.cpufreq_ncpu = 2,	.cpufreq_power = 635, },
	[11] = { .cpufreq_khz = 1170000,	.cpufreq_ncpu = 2,	.cpufreq_power = 578, },
	[12] = { .cpufreq_khz = 819000,	.cpufreq_ncpu = 3,	.cpufreq_power = 513, },
	[13] = { .cpufreq_khz = 598000,	.cpufreq_ncpu = 4,	.cpufreq_power = 468, },
	[14] = { .cpufreq_khz = 1040000,	.cpufreq_ncpu = 2,	.cpufreq_power = 467, },
	[15] = { .cpufreq_khz = 598000,	.cpufreq_ncpu = 3,	.cpufreq_power = 351, },
	[16] = { .cpufreq_khz = 1300000,	.cpufreq_ncpu = 1,	.cpufreq_power = 344, },
	[17] = { .cpufreq_khz = 819000,	.cpufreq_ncpu = 2,	.cpufreq_power = 342, },
	[18] = { .cpufreq_khz = 442000,	.cpufreq_ncpu = 4,	.cpufreq_power = 324, },
	[19] = { .cpufreq_khz = 1235000,	.cpufreq_ncpu = 1,	.cpufreq_power = 317, },
	[20] = { .cpufreq_khz = 1170000,	.cpufreq_ncpu = 1,	.cpufreq_power = 289, },
	[21] = { .cpufreq_khz = 442000,	.cpufreq_ncpu = 3,	.cpufreq_power = 243, },
	[22] = { .cpufreq_khz = 598000,	.cpufreq_ncpu = 2,	.cpufreq_power = 234, },
	[23] = { .cpufreq_khz = 1040000,	.cpufreq_ncpu = 1,	.cpufreq_power = 233, },
	[24] = { .cpufreq_khz = 299000,	.cpufreq_ncpu = 4,	.cpufreq_power = 209, },
	[25] = { .cpufreq_khz = 819000,	.cpufreq_ncpu = 1,	.cpufreq_power = 171, },
	[26] = { .cpufreq_khz = 442000,	.cpufreq_ncpu = 2,	.cpufreq_power = 162, },
	[27] = { .cpufreq_khz = 299000,	.cpufreq_ncpu = 3,	.cpufreq_power = 156, },
	[28] = { .cpufreq_khz = 598000,	.cpufreq_ncpu = 1,	.cpufreq_power = 117, },
	[29] = { .cpufreq_khz = 299000,	.cpufreq_ncpu = 2,	.cpufreq_power = 104, },
	[30] = { .cpufreq_khz = 442000,	.cpufreq_ncpu = 1,	.cpufreq_power = 81, },
	[31] = { .cpufreq_khz = 299000,	.cpufreq_ncpu = 1,	.cpufreq_power = 52, },


	mt_gpufreqs_power[0].gpufreq_khz = 448500
	mt_gpufreqs_power[0].gpufreq_volt = 115000
	mt_gpufreqs_power[0].gpufreq_power = 753

	mt_gpufreqs_power[1].gpufreq_khz = 279500
	mt_gpufreqs_power[1].gpufreq_volt = 115000
	mt_gpufreqs_power[1].gpufreq_power = 487


	mt_gpufreqs_power[2].gpufreq_khz = 279500
	mt_gpufreqs_power[2].gpufreq_volt = 105000
	mt_gpufreqs_power[2].gpufreq_power = 396



--->温升导致的充电电流降低
	Dear MTK，
	之前在6753平台 ，mtk_cooler_bcct.c 会根据 thermal.conf 里 配置 调用 mtk_cl_bcct_set_bcct_limit来限制充电电流；现在6755平台这个功能 没有起作用，请问是不在支持了吗？？ 还是某个开关控制没有打开呢？
	我现在 thermal.conf 里设置如下：
	/proc/driver/thermal/clbcct
	0 1000 900 750
	/proc/driver/thermal/tzbts
	6 100000 0 mtktspa-sysrst 90000 0 mtk-cl-shutdown00 57000 0 mtk-cl-cam00 50000 0 mtk-cl-bcct02 45000 0 mtk-cl-bcct01 40000 0 mtk-cl-bcct00
	实际温度已经56度了，T_AP=56000，仍旧没有调用 mtk_cl_bcct_set_bcct_limit；
	请给予解答，ThankS！

	我现在 thermal.conf 里设置如下：
	/proc/driver/thermal/clbcct
	0 1000 900 750
	/proc/driver/thermal/tzbts
	6 100000 0 mtktspa-sysrst 90000 0 mtk-cl-shutdown00 57000 0 mtk-cl-cam00 50000 0 mtk-cl-bcct02 45000 0 mtk-cl-bcct01 40000 0 mtk-cl-bcct00


	这个应该可以生效，可以在mtk_cl_bcct_set_bcct_limit 加点log看看是否可以触发？
	内部机器配置基本是这样：
	/proc/driver/thermal/clbcct
	0 2000 1000 500
	/proc/driver/thermal/tzbts
	6 100000 0 mtktspa-sysrst 90000 0 mtk-cl-shutdown00 57000 0 mtk-cl-cam00 50000 0 mtk-cl-bcct02 49000 0 mtk-cl-bcct01 48000 0 mtk-cl-bcct00 0 0 no-cooler 0 0 no-cooler 0 0 no-cooler 0 0 no-cooler 1000

	mtk_cl_bcct_set_bcct_limit里默认就有log啊，没有出来；并且肯定没有调到set_bat_charging_current_limit，这个我里面是加了log的；我们关心的就是为何没有调到set_bat_charging_current_limit，调整电流；
	thermal.conf    4.18 KB     和 thermal.eng.conf    4.18 KB
	APLog_2015_0101_081542.rar    62.39 MB     抓取的mobile log，请参考分析；

	为了减小内核的log量，
	thermal management default将kernel部分的debug log都关闭了。
	只留下了一些，error和warning log。

	而有些问题的分析和thermal policy的tuning是需要完整的thermal log的。
	特别是各个thermal zone device的温度值。

	为此可以使用Elephant Stress工具来抓取完整的thermal log。
	(user load也可以使用)
	Elephant stress tool可以在敝司DDC网站上下载。

	或者修改一下mtk_cl_bcct_set_bcct_limit

	/kernel-3.10/drivers/misc/mediatek/thermal/mt6755/common/coolers/mtk_cooler_bcct.c
	下的log是否有开
	9#define mtk_cooler_bcct_dprintk(fmt, args...) \
	20do { \
	21    if (1 == cl_bcct_klog_on) \
	22        pr_debug("[thermal/cooler/bcct]" fmt, ##args); \
	23} while (0)

	初始化或者其它code流程 都没有bcct 部分的log


	dear customer
	echo 1 1000 900 750 > /proc/driver/thermal/clbcct

	也可以打开log


	Dear MTK，
	root@GiONEE_BFL7512B:/ # echo 1 1000 900 750 > /proc/driver/thermal/clbcct
	root@GiONEE_BFL7512B:/proc/driver/thermal # cat clbcct
	klog 1
	curr_limit 65535
	mtk-cl-bcct00 1000 mA, state 0
	mtk-cl-bcct01 900 mA, state 0
	mtk-cl-bcct02 750 mA, state 0
		
	mobilelog-7512-gaowenlog.rar    73.05 MB     新抓取的 mobile log，请参考分析；


	<7>[ 345.529336].(0)[3790:sh][name:mtk_cooler_bcct&][thermal/cooler/bcct]_cl_bcct_read invoked.
	<7>[ 362.634991].(0)[3790:sh][name:mtk_cooler_bcct&][thermal/cooler/bcct]_cl_bcct_read invoked.
	这两句log 可以看出，log是打开了的；
	并且6755平台其他项目 也有反馈mtk_cl_bcct_set_bcct_limit 没有其作用；


	dear customer
	有开启MTK_PUMP_EXPRESS_PLUS_SUPPORT 吗?

	在检查device\mediatek\<.your projec name>\ProjectConfig.mk

	在device\mediatek\mt6755\device.mk 有区分



	修改一下：
	device\mediatek\mt6755\device.mk

	ifeq ($(strip $(MTK_PUMP_EXPRESS_PLUS_SUPPORT)),yes)
	ifeq ($(TARGET_BUILD_VARIANT),eng)
	PRODUCT_COPY_FILES += $(LOCAL_PATH)/thermal.eng.conf:system/etc/.tp/thermal.conf
	else
	PRODUCT_COPY_FILES += $(LOCAL_PATH)/thermal.conf:system/etc/.tp/thermal.conf
	endif
	PRODUCT_COPY_FILES += $(LOCAL_PATH)/thermal.wfd.6755.conf:system/etc/.tp/.thermal_policy_00
	PRODUCT_COPY_FILES += $(LOCAL_PATH)/thermal.pip.6755.conf:system/etc/.tp/.thermal_policy_01
	PRODUCT_COPY_FILES += $(LOCAL_PATH)/thermal.fdvrgis.6755.conf:system/etc/.tp/.thermal_policy_02
	PRODUCT_COPY_FILES += $(LOCAL_PATH)/thermal.meta.6755.conf:system/etc/.tp/.thermal_meta.conf
	else
	ifeq ($(TARGET_BUILD_VARIANT),eng)
	PRODUCT_COPY_FILES += $(LOCAL_PATH)/thermal.eng_noBCCT.conf:system/etc/.tp/thermal.conf
	else
	PRODUCT_COPY_FILES += $(LOCAL_PATH)/thermal_noBCCT.conf:system/etc/.tp/thermal.conf
	endif
	PRODUCT_COPY_FILES += $(LOCAL_PATH)/thermal.wfd.6755_noBCCT.conf:system/etc/.tp/.thermal_policy_00
	PRODUCT_COPY_FILES += $(LOCAL_PATH)/thermal.pip.6755_noBCCT.conf:system/etc/.tp/.thermal_policy_01
	PRODUCT_COPY_FILES += $(LOCAL_PATH)/thermal.fdvrgis.6755_noBCCT.conf:system/etc/.tp/.thermal_policy_02
	PRODUCT_COPY_FILES += $(LOCAL_PATH)/thermal.meta.6755_noBCCT.conf:system/etc/.tp/.thermal_meta.conf
	endif

	或者定义MTK 的快充MTK_PUMP_EXPRESS_PLUS_SUPPORT
	建议是修改device.mk


	


}*/









 


其他问题
/*
{
   

   
待机电流

签名版










}*/

	











another其他问题
/*
{


GNSPR#52112 MMI充电测试失败
	1.现象：
	a.充电测试失败
	无论是交流充电还是usb，充电电压是在增加，但是电流是负，且电量越来越少。
	b.充电芯片是正常工作的，起码充进去了电，但是充的比耗的多
	

	2.可能的原因：
	a.软件：pmic未工作，或者未挂上i2c总线
	b.硬件：电源管理芯片出现问题，或内部硬件短路，造成较大的功耗。
	c.我理解的可能原因，电池对外放电（电流为负）

	原因：充电芯片没有焊好


GNSPR#55817 MMI充电测试失败

	1.现象：
		机器插上充电器或者USB充电电流为负


	2.可能的原因：
		a.跟上次类似充电芯片未正常工作
		b.根据log,fg_coulomb的数值，i2c传输数据有问题 
			c.充电芯片未正常工作，或者充电接口出现问题（未充电时电流等其他都是正常的）

	原因：又是充电芯片没焊好导致的充电芯片没有正常工作





待解决的问题
   /*a.明确打印出来的那几个物理量的意思，充放电过程什么变化情况。*/
log:
	bat_routine_thr][kernel]AvgVbat 3806,bat_vol 3749, AvgI 0, I 0, VChr 0, AvgT 29, T 30, ZCV 3864, CHR_Type 0, SOC  54: 54: 54

	bat_routine_thr][kernel]AvgVbat 3804,bat_vol 3725, AvgI 505, I -775, VChr 5137, AvgT 29, T 29, ZCV 3864, CHR_Type 1, SOC  54: 54: 54

	bat_routine_thr][kernel]AvgVbat 3800,bat_vol 3694, AvgI 465, I -655, VChr 5137, AvgT 29, T 30, ZCV 3864, CHR_Type 1, SOC  54: 54: 54

	bat_routine_thr][kernel]AvgVbat 3796,bat_vol 3718, AvgI 427, I -586, VChr 5137, AvgT 29, T 29, ZCV 3864, CHR_Type 1, SOC  54: 54: 54

	bat_routine_thr][kernel]AvgVbat 3793,bat_vol 3707, AvgI 385, I -706, VChr 5128, AvgT 29, T 29, ZCV 3864, CHR_Type 1, SOC  54: 54: 54

	bat_routine_thr][kernel]AvgVbat 3788,bat_vol 3675, AvgI 344, I -706, VChr 5128, AvgT 29, T 30, ZCV 3864, CHR_Type 1, SOC  54: 54: 54

	bat_routine_thr][kernel]AvgVbat 3784,bat_vol 3691, AvgI 306, I -586, VChr 5128, AvgT 29, T 31, ZCV 3864, CHR_Type 1, SOC  54: 54: 54

	bat_routine_thr][kernel]AvgVbat 3781,bat_vol 3726, AvgI 264, I -706, VChr 5137, AvgT 29, T 32, ZCV 3864, CHR_Type 1, SOC  54: 54: 54

	bat_routine_thr][kernel]AvgVbat 3778,bat_vol 3717, AvgI 225, I -620, VChr 5516, AvgT 29, T 32, ZCV 3864, CHR_Type 1, SOC  54: 54: 54

	bat_routine_thr][kernel]AvgVbat 3775,bat_vol 3716, AvgI 184, I -689, VChr 5128, AvgT 29, T 32, ZCV 3864, CHR_Type 1, SOC  54: 54: 54

	bat_routine_thr][kernel]AvgVbat 3772,bat_vol 3719, AvgI 142, I -689, VChr 5128, AvgT 29, T 32, ZCV 3864, CHR_Type 1, SOC  54: 54: 54

	bat_routine_thr][kernel]AvgVbat 3768,bat_vol 3702, AvgI 102, I -655, VChr 5128, AvgT 29, T 32, ZCV 3864, CHR_Type 1, SOC  54: 54: 54

	bat_routine_thr][kernel]AvgVbat 3765,bat_vol 3730, AvgI 65, I -568, VChr 5610, AvgT 29, T 32, ZCV 3864, CHR_Type 1, SOC  54: 54: 54


battery_log(BAT_LOG_CRTI,
	    "[kernel]AvgVbat %d,bat_vol %d, AvgI %d, I %d, VChr %d, AvgT %d, T %d, ZCV %d, CHR_Type %d, SOC %3d:%3d:%3d\n",
	    BMT_status.bat_vol, bat_vol, BMT_status.ICharging, ICharging,
	    BMT_status.charger_vol, BMT_status.temperature, temperature, BMT_status.ZCV,
	    BMT_status.charger_type, BMT_status.SOC, BMT_status.UI_SOC, BMT_status.UI_SOC2);


AvgVbat ：电池的平均电压 
	ret = battery_meter_ctrl(BATTERY_METER_CMD_GET_ADC_V_BAT_SENSE, &val)--->BMT_status.bat_vol =mt_battery_average_method(BATTERY_AVG_VOLT, 
		&batteryVoltageBuffer[0], ZCV, &bat_sum, batteryIndex);

bat_vol：电池的电压 ret = battery_meter_ctrl(BATTERY_METER_CMD_GET_ADC_V_BAT_SENSE, &val)

AvgI:平均充电电流（跟上面相似）

I   ：充电电流，以前是通过检测充电引脚正负极参数，做适当的补偿，算出充电的电流，现在是通过充电芯片，IC算出电流，然后读取寄存器的值

电压，电流的变化跟之前的充电过程一样，涓流->恒流->恒压->充满--->检测是否小于回充电压，是否打电话等条件，在判断是否要充电，充电还是放电主要看充电器电压，然后电池电压，
电流的变化，温度是否正常

VChr	：充电器的电压

AvgT	：电池的平均温度

T	：电池的温度

ZCV	：电池当前的开路电压，这个开路电压跟当前的温度有关，导入的温度曲线，通过线性插值法找到具体的温度对应的电压

CHR_Type：充电接口的类型

SOC	:底层的电量

UI_SOC	:上层的电量

   force_get_tbat()---》BattVoltToTemp（）上面的那几个运算，什么do_div。。。64位的除法    	


   /*b.pmic工作的过程，如何确定i2c是否正常工作*/
   安装了i2c_tools，还有什么dump register?上面有一个打印pmic寄存器的log，正常打印，i2c设备应该正常。



   /*c.充电升降压调节的控制具体怎么控制的，双充电芯片？*/
定义的输入输出电流开关的宏
CONFIG_MTK_SWITCH_INPUT_OUTPUT_CURRENT_SUPPORT

定义的开关标志位g_bcct_flag    g_bcct_input_flag，好像跟温度有关的

快充部分有相应的升压调节，快充部分的升压通过充电适配器升高电压，而快充的通信好像是循环升压，看是否变化，变化了，证明连接上了，多了次数的判断，满足条件，可以快充。
mtk_ta_retry_increase()->mtk_ta_increase()->control的那个宏，set_ta_current将电压逐渐升到12

而普通的升压跟充电模式相关，应该在各种模式中，调度算法里 select_charging_current()设置充电电流，涓流->恒流->恒压->充满--->检测是否小于回充电压，
是否打电话等条件，在判断是否要充电，充电还是放电主要看充电器电压，然后电池电压，电流的变化，温度是否正常

基于输入电压的动态电源管理 (VIN-DPM)：电源和充电器之间的电阻会阻碍充电器从电源获得最大功率，导致电源电压陡降，造成充电器欠压闭锁。
已经有了用来计算充电器所需最小电源电压的方程式，其可计算既定电源适配器的最大充电电流。此外， VIN-DPM 特性还能动态地降低充电器的输入电流限值，避免适配器电压陡降，
因而允许使用多种类型的适配器和/或电源连接

双充电芯片：
两个充电芯片同时为电池工作，可以降低充电的温度（热量分散到两个芯片上）
定义的这个宏：CONFIG_MTK_DUAL_INPUT_CHARGER_SUPPORT



   /*d.反向充电，升压，USB，otg*/

当设备检测到USB_ID信号为低时，表该设备应作为Host（主机，也称A设备）用。
当设备检测到USB_ID信号为高时，表示该设备作为Slave(外设，也称B设备）用。

switch_int_to_device   switch_int_to_host  ，根据变量的值判断进入device还是host，而这个值是是读取寄存器的值，iddig
pinctrl_select_state控制gpio引脚的状态，
反向充电应该跟USB_OTG有关，当检测到USB 的id引脚为0，otg模式，触发中断。当然逻辑好像改了，如果是OTG充电模式，上层要点击确定，使OTG中断使能，才可以反向充电。

is_enter_mmi_test 反向充电测试开关


开机充电，关机充电 ，低电量保护？

正常开机充电，一直再看。

关机充电：

       关机时充电器检测代码在lk部分，当接入充电器时，PMIC会通过ADC采样，检测Vchrg（charger/usb电源）电压，
判断Vchrg的值如果在这个区间：4.4v~6.5v，系统就会开始进入充电过程，这个过程包括show low battery logo，接着
显示充电电量logo。
相关代码：
mt6572/mediatek/platform/mt6572/lk/platform.c

查看函数void platform_init(void)：

充电器检测：
充电过程中，upmu_is_chr_det() 检测Vchrg状态，返回KAL_FALSE则power off。
#ifdef MTK_KERNEL_POWER_OFF_CHARGING
    if((g_boot_arg->boot_reason == BR_USB) && (upmu_is_chr_det() == KAL_FALSE))
    {
        printf("[%s] Unplugged Charger/Usb between Pre-loader and Uboot in Kernel Charging Mode, Power Off \n", __func__);
        mt6575_power_off();
    }
#endif

显示LCM背光：
#ifndef DISABLE_FOR_BRING_UP
    mt65xx_backlight_on(); //[TODO] workaround
#endif

boot模式选择，用于区分开机过程。
boot_mode_select();

显示low battery logo，点亮充电指示灯：

        if(kernel_charging_boot() == 1) //关机充电状态
        {
                mt_disp_power(TRUE);
                mt_disp_show_low_battery();
                mt_disp_wait_idle();
#ifndef DISABLE_FOR_BRING_UP
                mt65xx_leds_brightness_set(6, 110);
#endif
        }

kernel_charging_boot定义：
#if defined (MTK_KERNEL_POWER_OFF_CHARGING)
int kernel_charging_boot(void)
{
        if((g_boot_mode == KERNEL_POWER_OFF_CHARGING_BOOT || g_boot_mode == LOW_POWER_OFF_CHARGING_BOOT) && upmu_is_chr_det() == KAL_TRUE)
        {
                printf("[%s] Kernel Power Off Charging with Charger/Usb \n", __func__);
                return  1;
        }
        else if((g_boot_mode == KERNEL_POWER_OFF_CHARGING_BOOT || g_boot_mode == LOW_POWER_OFF_CHARGING_BOOT) && upmu_is_chr_det() == KAL_FALSE)
        {
                printf("[%s] Kernel Power Off Charging without Charger/Usb \n", __func__);
                return -1;
        }
        else
                return 0;
}
#endif


   /*e.分析那个电池曲线，出现的什么问题，自己想（虽然不是我解决的）*/
充电时间有点长

	

   /*f.ZCV电池曲线，导入，操作*/
	battery_profile_tX 几个数组

   /*g.充电类型的判断*/
	
   pmic和USB都要ready,PMU检测充电端是否正常工作


SS：高优先级--->温度的调控，处理算法，  h.thermal相关的内容还是不太清楚


待机状态下有些会定期唤醒
	1.modem不太了解，经常唤醒，耗电很大
	2.1800s定期唤醒（30min），防睡死，1%检测中断

充电状态下BAT_thread几个线程10s唤醒一次















关机和重启的两种方式
--->按powerkey弹框选择重启还是关机，搜AmigoGlobalActions
这个是从上层发送指令reboot还是shutdown广播-->ShutdownThread: reboot,这个线程负责关闭很多service
这个是上层发的一个全局性的广播,AmigoGlobalActions



AmigoGlobalActions: onReceive:reason  homekey mViewState REBOOT

[  477.443918] <0>.(0)[54:pmic_thread][name:kpd&]kpd: Power Key generate, pressed=1 



--->长按powerkey达到一定的时间然后有软件上的关机和硬件上的断电关机












}*/


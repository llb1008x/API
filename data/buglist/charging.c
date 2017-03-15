/**********************************************************************************
							充电相关的
**********************************************************************************/





需要注意的地方
{

	1.fg_coulom库仑计的检测，变化，这里底层对于库仑计的数据的读取，数据的转换等相关内容？
	
	2.电量显示异常问题？电量显示的准确性

	3.电量0%，1%的上报，低电保护lowpower_protect_init（）
	
	4.关机电压跟开机电压是不一样的，首先开机电压肯定要比关机电压高。
}










相关的BUG
{
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
	 
	 

GNSPR#72403
	现象：
	插入SIM卡，电量50%后台挂QQ微信，（运行10个常用应用）运行一个大型游戏在游戏界面连接标配充电器，（8:29--15:58）7小时29分才充满电，对比1605A用时5小时25分充满


    原因：
	测试机和对比机电量显示都有误差，电量计之前可能没校准。测试机在开始插入充电器的时候，电池电压偏高，所以充到92%的时候电流快截止了，后面的充电电流很小，时间很长
而对比机，开始插入充电器的时候电池电压偏低，所以充到电量显示100%，但实际上未到100%。

	通过vchr看到的电池电压是随着系统负载动态变化的，不准确，可以看zcv电池曲线查表

}


	 

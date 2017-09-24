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
	gFG_voltage



********************************************************************************************************************************************

GNSPR#75833
	现象：手机剩余电量14%手动重启后显示电量为62%

	原因：
	从log看 测试员一开始 进行反向充电大负载测试，把电压拉得很低，电量计算 也会 下降， 后面又进行充电 把电压拉得很高，导致电压很高；在重启开机计算电量的时候，
根据高电压计算出来的电量就会很高，超出了 跳变门限的话就会发生跳变；这个是要是 先大幅在测试、又进行充电，电压跳变太大；MTK在开机采集的电压 产生偏差
导致最终的电量跳变；


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



--->电量计不准
	现象：
	a.充电到78%之后，电量显示上不去，怎么充也不见电量上升，插拔电池后，电量上升至96%
	b.正常放电至18%~20%后，基本上每秒下降1%，放电至关机		



GNSPR#87038
	G1605A底层电量跟上层电量偏差大，导致电量跳变问题，原因
    {
      电量偏差累计
        1,测试电量相关的问题，要在Download软件版本后，或者 更换电池后，先充满一次电，同步一下电量，再进行测试；
        2,禁止频繁开关机重启、频繁插拔充电器操作，操作间隔时间要5分钟以上，且操作次数不要超过5次；
        3,未按照以上1、2操作，引出来的电量显示问题，开发可以直接驳回；

        eService号：ALPS03218062
        GNSPR#86321
     
      /*debug*/  
        mt_battery_meter.h  
        修改底层电量跟rtc电量偏差的阈值，这种方法在一定程度上可以降低上层显示电量跳变的概率，但是无法从根本上避免
        这个跟电池特性有关，最好的办法是直接返回rtc记录的值，但是GM2.0算法封装了，无法获取到代码
        
        //Gionee <gn_by_charging> <lilubao> <20170620> add for fixed bug begin
        #define DIFFERENCE_HWOCV_RTC		60		// 32-> 60
        #define DIFFERENCE_HWOCV_SWOCV		12		// 10-> 12
        #define DIFFERENCE_SWOCV_RTC		20		// 18-> 20
        //Gionee <gn_by_charging> <lilubao> <20170620> add for fixed bug end

    }	

}


	 

	pass G1605A电量跳变问题
	{	
		G1605A手上的BUG
		90667		待机功耗
		89288		电量跳变问题
		89243		这个也是电量跳变问题引起的，电量虚高
		90309		电量跳变问题
	


		现象：低电量关机充电条件下，多次插拔充电器，手机电量从8%跳变到45%，开机之后电量仍然是45%
		但是那只是暂时的，之后手机电量迅速降低
	
		相关的BUG号：87038，86321，90309，89288，89243
		eService ID：ALPS03373196
		合入patch前后的版本		P80
		G1605A-T0114-170310AB
		G1605A-T0115-170314AB
		G1605A-T0116-170324AB
	
		原因：
			1.fuelgauge2.0的电量是根据电池电压来算的，也就是ZCV table上的电压算电量，多次插拔充电器之后，
			电池电压升的很高(插入充电器之后，电池电压升高是什么原因，为什么每插一次充电器之后，电池电压都会上升一次)
		
			2.开机之后显示的电量是45%，但是电池电压仍然只有3.54V左右，之后十分钟以内电量下降的很快，因为开机之前记录
			的是RTC的电量，但是之后fuelgauge2.0是会去检测电池电压，计算电量，但是差别很大，为了使电量变化平缓，所以
			是放慢下降的速率。
		
			3.底层电量跟上层显示差别较大，然后触发电量跳变问题	，
			mt_battery_meter.h
		

		分析
		{
			初步分析原因是P80引入的问题，但是P80就是为了解决这个问题的 
			gitk  --author=liteng   
			2017.3.15   73194 P80,	2017.5.22 	84712 P96


			/* fg 2.0 */
			#define DIFFERENCE_HWOCV_RTC		30		//硬件跟RTC的最大偏差
			#define DIFFERENCE_HWOCV_SWOCV		10		//硬件跟软件的最大偏差
			#define DIFFERENCE_SWOCV_RTC		10		//软件跟RTC的最大偏差
			#define MAX_SWOCV					3		//这是个%，就是检测与RTC的电量超过%，就重新检测电池电量
	
	
			关机电量跳变过程 FGADC_D0   embedded 0 
			[ 3.440954] <0>.(1)[220:fuelgauged]MTK_FG: [FGADC_D0](HW OCV 3765,25 SW OCV 3852,49 RTC 9, VBAT 3775,28, T_avg 34, I 5895, is_charging 0, is_charger 1, gap1 32,10,18,5,10,30 D0 91 ui_soc:9 7 embedded 0 plugout 0 )
			[ 6.506033] <0>.(0)[220:fuelgauged]MTK_FG: [FGADC_D0](HW OCV 3765,25 SW OCV 3852,49 RTC 9, VBAT 3775,28, T_avg 34, I 5895, is_charging 0, is_charger 1, gap1 32,10,18,5,10,30 D0 91 ui_soc:9 7 embedded 0 plugout 0 )
			[ 3.409184] <0>.(1)[221:fuelgauged]MTK_FG: [FGADC_D0](HW OCV 3848,48 SW OCV 3864,51 RTC 9, VBAT 3783,30, T_avg 34, I 6155, is_charging 0, is_charger 1, gap1 32,10,18,5,10,30 D0 52 ui_soc:48 2 embedded 0 plugout 0 )
			[ 6.476715] <0>.(0)[221:fuelgauged]MTK_FG: [FGADC_D0](HW OCV 3848,48 SW OCV 3864,51 RTC 9, VBAT 3783,30, T_avg 34, I 6155, is_charging 0, is_charger 1, gap1 32,10,18,5,10,30 D0 52 ui_soc:48 2 embedded 0 plugout 0 )
			[ 16.481772] <0>.(0)[221:fuelgauged]MTK_FG: [FGADC_D0](HW OCV 3848,48 SW OCV 3864,51 RTC 9, VBAT 3783,30, T_avg 34, I 6155, is_charging 0, is_charger 1, gap1 32,10,18,5,10,30 D0 52 ui_soc:48 2 embedded 0 plugout 0 )
			[ 26.479228] <0>.(0)[221:fuelgauged]MTK_FG: [FGADC_D0](HW OCV 3848,48 SW OCV 3864,51 RTC 9, VBAT 3783,30, T_avg 34, I 6155, is_charging 0, is_charger 1, gap1 32,10,18,5,10,30 D0 52 ui_soc:48 2 embedded 0 plugout 0 )
		
		
			开机后电量迅速下降
			<3>[   11.406829] .(0)[208:bat_routine_thr][name:battery_common_fg_20&][kernel]AvgVbat 3580,bat_vol 3546, AvgI 0, I 0, VChr 0, AvgT 25, T 26, ZCV 0, CHR_Type 0, SOC   0:-100: -1
			<3>[   12.048313] .(0)[208:bat_routine_thr][name:battery_common_fg_20&][kernel]AvgVbat 3578,bat_vol 3517, AvgI 0, I 0, VChr 0, AvgT 25, T 26, ZCV 0, CHR_Type 0, SOC   0:-100: -1
			<3>[   22.063274] .(0)[208:bat_routine_thr][name:battery_common_fg_20&][kernel]AvgVbat 3576,bat_vol 3500, AvgI 0, I 0, VChr 0, AvgT 25, T 26, ZCV 3682, CHR_Type 0, SOC   4: 47: 46
			<3>[   32.061719] .(0)[208:bat_routine_thr][name:battery_common_fg_20&][kernel]AvgVbat 3574,bat_vol 3551, AvgI 0, I 0, VChr 0, AvgT 25, T 26, ZCV 3682, CHR_Type 0, SOC   4: 47: 46
			<3>[   42.079623] .(0)[208:bat_routine_thr][name:battery_common_fg_20&][kernel]AvgVbat 3575,bat_vol 3601, AvgI 0, I 0, VChr 0, AvgT 25, T 27, ZCV 3682, CHR_Type 0, SOC   4: 47: 46
			<3>[   52.078424] .(1)[208:bat_routine_thr][name:battery_common_fg_20&][kernel]AvgVbat 3576,bat_vol 3594, AvgI 0, I 0, VChr 0, AvgT 25, T 27, ZCV 3682, CHR_Type 0, SOC   4: 47: 46
			<3>[   62.066274] .(1)[208:bat_routine_thr][name:battery_common_fg_20&][kernel]AvgVbat 3575,bat_vol 3570, AvgI 0, I 0, VChr 0, AvgT 25, T 27, ZCV 3682, CHR_Type 0, SOC   4: 47: 46
			<3>[   72.052747]  (3)[208:bat_routine_thr][name:battery_common_fg_20&][kernel]AvgVbat 3574,bat_vol 3536, AvgI 0, I 0, VChr 0, AvgT 25, T 27, ZCV 3682, CHR_Type 0, SOC   4: 47: 46


			<3>[   82.055699]  (0)[208:bat_routine_thr][name:battery_common_fg_20&][kernel]AvgVbat 3571,bat_vol 3490, AvgI 0, I 0, VChr 0, AvgT 25, T 27, ZCV 3682, CHR_Type 0, SOC   4: 47: 46
			<3>[   92.057397]  (0)[208:bat_routine_thr][name:battery_common_fg_20&][kernel]AvgVbat 3568,bat_vol 3498, AvgI 0, I 0, VChr 0, AvgT 25, T 28, ZCV 3682, CHR_Type 0, SOC   4: 46: 45
			<3>[  102.054568]  (2)[208:bat_routine_thr][name:battery_common_fg_20&][kernel]AvgVbat 3565,bat_vol 3495, AvgI 0, I 0, VChr 0, AvgT 25, T 28, ZCV 3682, CHR_Type 0, SOC   4: 46: 45
			<3>[  112.054956]  (2)[208:bat_routine_thr][name:battery_common_fg_20&][kernel]AvgVbat 3561,bat_vol 3460, AvgI 0, I 0, VChr 0, AvgT 25, T 29, ZCV 3682, CHR_Type 0, SOC   4: 45: 44
			<3>[  122.055205]  (1)[208:bat_routine_thr][name:battery_common_fg_20&][kernel]AvgVbat 3555,bat_vol 3414, AvgI 0, I 0, VChr 0, AvgT 25, T 29, ZCV 3682, CHR_Type 0, SOC   4: 45: 44
			<3>[  132.054983]  (2)[208:bat_routine_thr][name:battery_common_fg_20&][kernel]AvgVbat 3552,bat_vol 3477, AvgI 0, I 0, VChr 0, AvgT 26, T 29, ZCV 3682, CHR_Type 0, SOC   3: 44: 43
			<3>[  142.059308]  (1)[208:bat_routine_thr][name:battery_common_fg_20&][kernel]AvgVbat 3549,bat_vol 3500, AvgI 0, I 0, VChr 0, AvgT 26, T 30, ZCV 3682, CHR_Type 0, SOC   3: 43: 42
			<3>[  152.057670]  (1)[208:bat_routine_thr][name:battery_common_fg_20&][kernel]AvgVbat 3546,bat_vol 3481, AvgI 0, I 0, VChr 0, AvgT 26, T 30, ZCV 3682, CHR_Type 0, SOC   3: 42: 41
			<3>[  162.055236]  (0)[208:bat_routine_thr][name:battery_common_fg_20&][kernel]AvgVbat 3542,bat_vol 3475, AvgI 0, I 0, VChr 0, AvgT 26, T 30, ZCV 3682, CHR_Type 0, SOC   3: 41: 40
			<3>[  172.062902]  (0)[208:bat_routine_thr][name:battery_common_fg_20&][kernel]AvgVbat 3541,bat_vol 3559, AvgI 0, I 0, VChr 0, AvgT 26, T 30, ZCV 3682, CHR_Type 0, SOC   3: 40: 39
			<3>[  182.062495]  (1)[208:bat_routine_thr][name:battery_common_fg_20&][kernel]AvgVbat 3540,bat_vol 3538, AvgI 0, I 0, VChr 0, AvgT 26, T 31, ZCV 3682, CHR_Type 0, SOC   3: 38: 37
			<3>[  192.062400]  (2)[208:bat_routine_thr][name:battery_common_fg_20&][kernel]AvgVbat 3537,bat_vol 3489, AvgI 0, I 0, VChr 0, AvgT 27, T 31, ZCV 3682, CHR_Type 0, SOC   3: 37: 36
		

			从为合入patch的版本，单独合入patch，patch里面包含的lib，需要在vendor目录和out目录下面都手动替换一次
			这两个lib库，不知道out目录下是否有替换到
		
			所以现在要做的是抓一份未合入patch之前的log，然后合入patch，替换lib库，分析合入之后的log，开fuelgauge log	
		
		

		}

		相关的log关键字
		{
			int Enable_FGADC_LOG = BMLOG_TRACE_LEVEL;
		
			FGADC_D0
		
			//Gionee <gn_by_CHG> <lilubao> <20170807> add for fuelgauge begin
		
			mt_battery_set_init_vol(gFG_voltage_init);
			battery_meter_set_fg_int

			INIT_BAT_CUR_FROM_PTIM

		}


		解决方案
		{
			内置电池和外置电池的区别：
			如果是内置电池embedded_battery，开机关机的电量是从rtc里面读取的，如果电量偏差超过max_swocv就重新检测电量
			所以如果是内置电池，第一次或者长时间未充电，建议充满
			如果是外置电池，开关机的电量是硬件重新检测，而且硬件检测有偏差，就像这个电量跳变问题，插入充电器电池电压升高，然后是根据电池
			电压计算电量的，所以电量变高
		
			battery_meter_fg_20.c -> batt_meter_cust_data.embedded_battery -> batt_meter_cust_data.embedded_battery=1；
		}
	
	
		MTK：
		fuelgauge对于电量的判断流程，这个判断过程自己一定要理解，
		{
			“理论上需要走这些判断，但是电量的值直接走了都去硬件的电量
			所以出错“
		
		
		
			大概写了一下流程，请过目：
			底层电量跟上层偏差过大导致电量跳变的判断

			fg_result：
			0：电量触发跳变的
			2：rtc=1的情况下，把rtc记录的电量赋给电量计；软件电量超过最大电量的情况
			4：rtc记录的电量超过10，把电量计记录的初始化的值赋给电量计
			5：其他情况下也是把电量计记录的初始化的值赋给电量计

			if (soc_flow == HW_FG || soc_flow == SW_FG) {
				if ( (fg_plugout_status==0 || (boot_reason == BR_2SEC_REBOOT)) && (charger_exist != true)){
					if (g_rtc_fg_soc == 0) {
					    fg_capacity_by_v = fg_capacity_by_v_init;
					    fg_result = 0;
					} else {
					    if (g_rtc_fg_soc == 1) {
					        fg_capacity_by_v = g_rtc_fg_soc;
					        fg_result = 2;

					    } else if ( fg_sw_soc >= max_swocv ) {
					        fg_capacity_by_v = g_rtc_fg_soc;
					        fg_result = 2;
					    } else if (g_rtc_fg_soc > 10) {
					        fg_capacity_by_v = fg_capacity_by_v_init;
					        set_rtc = 2;
					        fg_result = 4;
					    } else {
					        fg_capacity_by_v = fg_capacity_by_v_init;
					        set_rtc = 1;
					        fg_result = 5;
					    }
					}
				} 
			
				else {	//rtc电量-硬件电量  超过  hw_rtc的阈值 同时  软件电量-rtc电量 大于 硬件 -  软件电量
					if (((abs(g_rtc_fg_soc - fg_hw_soc)) > difference_hwocv_rtc)
					    && (abs(fg_sw_soc - g_rtc_fg_soc) > abs(fg_hw_soc - fg_sw_soc))) {
					    /* compare with hw_ocv & sw_ocv, check if less than or equal to 5% tolerance */
					    if (abs(fg_sw_soc - fg_hw_soc) > difference_hwocv_swocv) {
					        fg_capacity_by_v = fg_capacity_by_v_init;
					        fg_result = 0;
					    }
					} else {
					    if (abs(fg_sw_soc - g_rtc_fg_soc) > (difference_swocv_rtc + batterypseudo1)
					        && ( abs(fg_sw_soc - g_rtc_fg_soc) > abs(fg_sw_soc - fg_vbat_soc) ) ) {
					        fg_capacity_by_v = fg_capacity_by_v_init;
					        fg_result = 0;
					    } else {
					        if (g_rtc_fg_soc == 0) {
					            fg_capacity_by_v = fg_capacity_by_v_init;
					            fg_result = 0;
					        } else {
					            if (g_rtc_fg_soc == 1) {
					                fg_capacity_by_v = g_rtc_fg_soc;
					                fg_result = 2;

					            } else if ( fg_sw_soc >= max_swocv ) {
					                fg_capacity_by_v = g_rtc_fg_soc;
					                fg_result = 2;
					            } else if (g_rtc_fg_soc > 10) {
					                fg_capacity_by_v = fg_capacity_by_v_init;
					                set_rtc = 2;
					                fg_result = 4;
					            } else {
					                fg_capacity_by_v = fg_capacity_by_v_init;
					                set_rtc = 1;
					                fg_result = 5;
					            }
					        }
					    }
					}
				}
			}

			// modify g_booting_vbat

			if (fg_capacity_by_v == 0 && charger_exist == true) {
				fg_capacity_by_v = 1;
				fg_result = 3;
				FGLOG_NOTICE("[FGADC] fg_capacity_by_v=%d\n", fg_capacity_by_v);
			}

			if (set_rtc == 1) {
				fg_capacity = g_rtc_fg_soc;
			} else if (set_rtc == 2){
				fg_capacity = g_rtc_fg_soc - 1;
			} else {
				fg_capacity = fg_capacity_by_v;
			}
			fg_dod0 = 100 - fg_capacity_by_v;
			fg_capacity_by_c_init = fg_capacity;
			fg_capacity_by_c = fg_capacity;
			fg_dod0_init = fg_dod0;
			fg_dod1 = fg_dod0;
			set_fg_soc(fg_capacity_by_v);
			ui_soc=fg_capacity_by_c_init; 

		}	

	}	
	
	
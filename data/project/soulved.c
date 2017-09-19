pass一类是几种场景下充电温度偏高
{
	这种要查找thermal是否起作用了，thermal管了那些地方，参数什么意思，怎么改？

	bcct:battery charging current throtting
	这个是在触发温度条件后就设定充电电流
	abcct:adaptive battery charging current throtting
	这个是在设定的目标温度的时候在最高和最低温度范围内动态调节电流

	根据以上两个设定chrlimt


	abcct_lcmoff 是在lcm off的时候启动的，如果灭屏的情况下温度过高设定的策略 


	 * sscanf format <klog_on> <mtk-cl-bcct00 limit> <mtk-cl-bcct01 limit> ...
	 * <klog_on> can only be 0 or 1
	 * <mtk-cl-bcct00 limit> can only be positive integer or -1 to denote no limit

	前面的1是打开thermal debug log的
	echo 1 1200 1000 800 > /proc/driver/thermal/clbcct



	config文件的相关参数

	/proc/driver/thermal/clabcct
	40000 1000 200000 5 2000 500 0 3000 0 1 5000 2000


	当前abcct的配置
	abcct

		abcct_cur_bat_chr_curr_limit 3000
		abcct_cur_chr_input_curr_limit -1
		abcct_pep30_cur_input_curr_limit 5000
	
		abcct_target_temp 44000
		abcct_kp 1000
		abcct_ki 200000
		abcct_kd 5
		abcct_max_bat_chr_curr_limit 3000
		abcct_min_bat_chr_curr_limit 0
		abcct_input_current_limit_on 0
		abcct_HW_thermal_solution 3000
		abcct_min_chr_input_curr_limit 0
		abcct_times_of_ts_polling_interval 1
		abcct_pep30_max_input_curr_limit 5000
		abcct_pep30_min_input_curr_limit 2000
	
	
	tzbts	这个是默认的参数，修改了几个策略  20170826 
	/proc/driver/thermal/tzbts
	6 100000 0 mtktsAP-sysrst 90000 0 mtk-cl-shutdown00 62000 0 mtk-cl-cam00 50000 0 abcct_lcmoff 44000 0 mtk-cl-adp-fps 42000 0 abcct 0 0 no-cooler 0 0 no-cooler 0 0 no-cooler 0 0 no-cooler 1000	


	alert diaglog

	mtk game detection service


	这个文件在devices/gionee_bj 目录下有效的，编译的时候可能要全编


	亮屏 251mA
	cam  600mA
}

	
	
	pass
	电量显示不正确  -> 要熟悉电池相关参数的上报过程
	{
		PowerManagerService -> BatteryService -> BatteryProperties.java
		->BatteryProperties.cpp 
	
		framework/base/services/core/java/com/android/server/power/PowerManagerService.java
		
		上报电池相关数据
		private void updateIsPoweredLocked(int dirty) 
		
		获取电池电量
		mBatteryLevel = mBatteryManagerInternal.getBatteryLevel();
		
		
		frameworks/base/services/core/java/com/android/server/BatteryService.java
		
		public int getBatteryLevel() {
        synchronized (mLock) {
            return mBatteryProps.batteryLevel;
        }
  	
  	
  		这两个文件对应的properties不对应，java多读了一个fastcharging属性
  		frameworks/base/core/java/android/os/BatteryProperties.java
  	
		./native/services/batteryservice/BatteryProperties.cpp

	}
	
	
	
	
	pass 
	过压测试：NG，10V时电压不不截止，还有380ma~440ma，无电压过高提醒
	{
		过压的时候要停止充电，还要上报状态显示过压
		{
			检测到充电电压大于6.5v字左右要停止充电
		
			rt5081那几个关于ovp的中断和中断函数是否被调用
			打印出那几个寄存器的值
			
			6.5V充电电流是跑sys还是battery
			
			vchr = pmic_get_vbus();
			if (vchr > info->data.max_charger_voltage) {
				info->notify_code |= 0x0001;
				pr_err("[BATTERY] charger_vol(%d) > %d mV\n",
					vchr, info->data.max_charger_voltage);
					
					
			7.5v设置寄存器，可以直接出发中断
			
			
			UUG P37  0x1d
		}
		
		
		device/gionee_bj/gnbj6757_66_n/ProjectConfig.mk
		
		
		rt5081有ovp功能，但是没有接那个脚,这个是内接的
		
		CHG_VIN:16.5v
		VBUS:14.5v

		uvp/ovp
		0x0e  bit[7:4]   uvp ,bit[3:0]ovp
		
		rt5081_pmu_chg_vinovp_irq_handler
		rt5081_pmu_chg_vbusov_irq_handler    CHARGER_DEV_NOTIFY_VBUS_OVP
		
		rt5081_pmu_ovpctrl_ovp_d_evt_irq_handler
		rt5081_pmu_ovpctrl_ovp_evt_irq_handler
		
		0xc8  ovpctrl_irq
		
		dev_err
		
		pmic_get_vbus
		
		battery_get_vbus
		battery_get_vbus
		
		数据位数不对应导致判断一直过不去
		{
			6500000 
			  11789	
			
			 6.500 000    
			11.789v
			
			
			4500000
		   13400000
		
		}


		停止充电之后还有power path功能，充电器直接给系统供电，应该直接关闭这个功能
		不进充电器也不进系统
		CHG_EN 0x12 bit[0]
		
		charger_dev_enable_powerpath(chg_dev, en);
		struct charger_device *chg_dev;
		bool en;
		chg_dev = info->chg1_dev
		
		vcdt chrin ovp
		vcdt_vthh
	
	}
		
		
	pass修改需要插入sim卡才能激活USB
	{
		#Gionee <gn_by_charging> <lilubao> <20170718> add for fixed trigger usb begin
		GN_RO_GN_USB_SIMSECURITY_SUPPORT = no
		#Gionee <gn_by_charging> <lilubao> <20170718> add for fixed trigger usb end
	}


	pass充电电流偏小-> 这个目前为止软件上只能这样，线上电流1.8A，可能线阻很大
	{
		这个问题拖了很长时间
		要理清改变充电电流跟哪些量有关
		
		aicl  [0x13]  
		mivr  [0x16]
		
		修改电池容量后，充电电流有提高到1.7A~1.8A
		mtk_battery_table.h，这里里面很多量都很重要
		
		
		jetia这个功能是disable的
		{
			检查rt5081的jieta功能是否使用了，是否有跟根据温度限流的有冲突
			07-18 10:12:21.187669   257   257 D [ 1607.744824] (1)[257:charger_thread]: rt5081_pmu_charger rt5081_pmu_charger: rt5081_dump_register: reg[0x1E] = 0x05
			07-18 10:12:21.187769   257   257 D [ 1607.744924] (1)[257:charger_thread]: rt5081_pmu_charger rt5081_pmu_charger: rt5081_dump_register: reg[0x1F] = 0x06
			
			07-18 10:12:21.187871   257   257 D [ 1607.745026] (1)[257:charger_thread]: rt5081_pmu_charger rt5081_pmu_charger: rt5081_dump_register: reg[0x20] = 0x00
			
			07-18 10:12:21.187971   257   257 D [ 1607.745126] (1)[257:charger_thread]: rt5081_pmu_charger rt5081_pmu_charger: rt5081_dump_register: reg[0x21] = 0x80
			07-18 10:12:21.188073   257   257 D [ 1607.745228] (1)[257:charger_thread]: rt5081_pmu_charger rt5081_pmu_charger: rt5081_dump_register: reg[0x22] = 0x50
		}
		
		
		合入rt5081软件版本P22  2017.7.18
		{	
		可能有问题的地方：
	//Gionee <gn_by_charging> <lilubao> <20170720> add for fixed charging current begin

			CONFIG_RT5081_PMU_CHARGER_TYPE_DETECT
			#CONFIG_DUAL_ROLE_USB_INTF=y	
			#define RT5081_PMU_REG_CHGHIDDENCTRL0	(0x30)
			RT_REG_DECL(RT5081_PMU_REG_CHGHIDDENCTRL0, 1, RT_VOLATILE, {});
		
			rgbled
			//wangguojun modify for RGB led begin
			0xe0, /* RT5081_PMU_REG_RGBCHRINDDIM: 0x92 */
			//wangguojun modify for RGB led end
		
			0x60, /* RT5081_PMU_REG_RGBCHRINDDIM: 0x92 */
		
			#if defined(__LP64__) || defined(_LP64)
		
			RT5081_APPLE_SAMSUNG_TA_SUPPORT
		
		
			软件分级
			__,_,
			#if defined(CONFIG_RT5081_PMU_CHARGER_TYPE_DETECT) && !defined(CONFIG_TCPC_CLASS)
		
			ret = rt5081_pmu_reg_test_bit(chg_data->chip, RT5081_PMU_REG_CHGSTAT1,
			RT5081_SHIFT_MIVR_STAT, &mivr_stat);
		
			/* Check DP > 2.3 V */
			ret = rt5081_pmu_reg_update_bits(
				chg_data->chip,
				RT5081_PMU_REG_QCSTATUS2,
				0x0F,
				0x0B
			);

		}
		
		
		// add for debug valid 20170724
		dev_err(chg_data->dev, " in %s by lilubao rt5081 T22\n", __func__);	
		
		充电器能力，线损的判断
		充电相关的变量变化和影响
		电池容量是否对充电电流有影响
		
		
		烧完版本不识别USB，充电没作用，adb不管用
		
		#1
		ichg = <2500000>;	/* uA 2000000->2500000*/
		ac_charger_current = <2500000>;				//2050000->2500000
		ac_charger_input_current = <2800000>;		//3200000->2800000
		
		#2
		aicl_vth = mivr + 100000; // 200000->100000->50000
		
		aicr,aicl
		aicl loop，mivr loop
		
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
	
	
	pass温升问题，根据不同场景降电流		
	{
		除了启动信息，还要加上正常情况下判断电池相关信息的log，current，voltage
		fg_drv_update_hw_status
		bm_err("[fg_drv_update_hw_status] current:%d %d state:%d %d car:%d %d bat:%d %d chr:%d %d hwocv:%d %d bat_plug_out:%d %d tmp:%d %d imix %d rac %d\n",
		fg_current, fg_current_new,
		fg_current_state, fg_current_state_new,
		fg_coulomb, fg_coulomb_new,
		bat_vol, bat_vol_new,
		chr_vol, chr_vol_new,
		hwocv, hwocv_new,
		plugout_status, plugout_status_new,
		tmp, tmp_new, (UNIT_TRANS_10 * get_imix()), get_rac()
		);

		这函数mt_battery_average_method,battery_common_fg_20.c做的什么操作？			//20170818	

		boot_reason type  中的kernel panic	BR_KERNEL_PANIC,//这个是内核错误，不知道下步该怎么走 http://blog.csdn.net/liukuan73/article/details/45537889

		cood_boot,hot_mode



	1.需要仿照G1605A添加一些boot mode,boot reason,call state,g_gn_screen_time，call_state
	[bat_routine_thr]g_gn_screenon_time=(80), g_call_state=0, g_boot_reason=4, g_boot_mode=0

		//Gionee <GN_BSP_CHG> <lilubao> <20170819> add for gn_boot info begin

		/* boot type definitions */
		typedef enum
		{
			NORMAL_BOOT = 0,
			META_BOOT = 1,
			RECOVERY_BOOT = 2,
			SW_REBOOT = 3,
			FACTORY_BOOT = 4,
			ADVMETA_BOOT = 5,		//这个是什么模式？，is_advanced_meta_mode
			ATE_FACTORY_BOOT = 6,
			ALARM_BOOT = 7,			//具体什么场景?
		#if defined (MTK_KERNEL_POWER_OFF_CHARGING)
			KERNEL_POWER_OFF_CHARGING_BOOT = 8,
			LOW_POWER_OFF_CHARGING_BOOT = 9,
		#endif
			FASTBOOT = 99,
			DOWNLOAD_BOOT = 100,
			UNKNOWN_BOOT
		} BOOTMODE;

		typedef enum {
			BR_POWER_KEY = 0,
			BR_USB,
			BR_RTC,
			BR_WDT,
			BR_WDT_BY_PASS_PWK,
			BR_TOOL_BY_PASS_PWK,
			BR_2SEC_REBOOT,
			BR_UNKNOWN,
			BR_KERNEL_PANIC,//这个是内核错误，不知道下步该怎么走 http://blog.csdn.net/liukuan73/article/details/45537889
			BR_WDT_SW,
			BR_WDT_HW
		} boot_reason_t;



		a. gn_boot_mode	=get_boot_mode();这个是通过mtk_boot_common.c 下的

		判断系统启动的boot_mode.boot_reason的流程 G1605A
		(mt_boot_common.c)dt_get_boot_common 这是从内核传参的过程中，从lk获取的atag -> init_boot_common然后初始化判断不同的启动模式，组包不同的字符串

		创建相应的节点	->  get_boot_mode这个接口函数可以在后面获取启动的信息


		#include <mt-plat/mtk_boot.h>
		/* return boot mode */
		unsigned int get_boot_mode(void)
		{
			init_boot_common(__LINE__);
			return g_boot_mode;
		}

		./drivers/misc/mediatek/boot/mt_boot_common.c:32:enum boot_mode_t g_boot_mode __nosavedata = UNKNOWN_BOOT;
		./drivers/misc/mediatek/boot/mt_boot_common.c:53:		g_boot_mode = tags->bootmode;
		./drivers/misc/mediatek/boot/mt_boot_common.c:72:			g_boot_mode);
		./drivers/misc/mediatek/boot/mt_boot_common.c:82:	if (UNKNOWN_BOOT != g_boot_mode) {
		./drivers/misc/mediatek/boot/mt_boot_common.c:84:		pr_warn("%s (%d) boot_mode = %d\n", __func__, line, g_boot_mode);
		./drivers/misc/mediatek/boot/mt_boot_common.c:88:	pr_debug("%s %d %d %d\n", __func__, line, g_boot_mode, atomic_read(&g_boot_init));
		./drivers/misc/mediatek/boot/mt_boot_common.c:94:	pr_debug("%s %d %d %d\n", __func__, line, g_boot_mode, atomic_read(&g_boot_init));
		./drivers/misc/mediatek/boot/mt_boot_common.c:102:	return g_boot_mode;
		./drivers/misc/mediatek/boot/mt_boot_common.c:111:	if (g_boot_mode == META_BOOT)
		./drivers/misc/mediatek/boot/mt_boot_common.c:122:	if (g_boot_mode == ADVMETA_BOOT)
		./drivers/misc/mediatek/boot/mt_boot_common.c:225:	switch (g_boot_mode) {
		./drivers/misc/mediatek/boot/mt_boot_common.c:275:	pr_debug("boot_mode = %d, state(%d,%d,%d)", g_boot_mode,
		./drivers/power/mediatek/switch_charging.c:142:	battery_log(BAT_LOG_CRTI, "[BATTERY] bat_charge_current_not_down, g_gn_screenon_time=%d, g_call_state=%d, is_enter_mmi_test=%d, g_boot_mode=%d !\n\r", g_gn_screenon_time, g_call_state, is_enter_mmi_test, get_boot_mode());    
		./drivers/power/mediatek/battery_common_fg_20.c:2245:	battery_log(BAT_LOG_CRTI,"g_gn_screenon_time=(%d), g_call_state=%d, g_boot_reason=%d, g_boot_mode=%d\n", g_gn_screenon_time, g_call_state, get_boot_reason(),get_boot_mode());

	
			b.gn_boot_reason 判断启动的原因
	
			(mt_boot_reason.c) dt_get_boot_reason跟boot_mode一样都是内核传参获取atag，获取字符串，然后读取指定信息

			./drivers/misc/mediatek/aee/common/reboot-reason.c:82:	int g_boot_reason = 0;
			./drivers/misc/mediatek/aee/common/reboot-reason.c:88:		g_boot_reason = br_ptr[12] - '0';
			./drivers/misc/mediatek/aee/common/reboot-reason.c:89:		LOGE("g_boot_reason=%d\n", g_boot_reason);
			./drivers/misc/mediatek/aee/common/reboot-reason.c:92:			g_boot_reason = BR_KE_REBOOT;
			./drivers/misc/mediatek/aee/common/reboot-reason.c:94:		return sprintf(buf, "%s\n", boot_reason[g_boot_reason]);
			./drivers/misc/mediatek/boot_reason/mt_boot_reason.c:33:enum boot_reason_t g_boot_reason __nosavedata = BR_UNKNOWN;
			./drivers/misc/mediatek/boot_reason/mt_boot_reason.c:51:			g_boot_reason = br_ptr[12] - '0';	/* get boot reason */
			./drivers/misc/mediatek/boot_reason/mt_boot_reason.c:82:	if (BR_UNKNOWN != g_boot_reason) {
			./drivers/misc/mediatek/boot_reason/mt_boot_reason.c:84:		pr_warn("boot_reason = %d\n", g_boot_reason);
			./drivers/misc/mediatek/boot_reason/mt_boot_reason.c:88:	pr_debug("%s %d %d %d\n", __func__, line, g_boot_reason, atomic_read(&g_br_state));
			./drivers/misc/mediatek/boot_reason/mt_boot_reason.c:94:	pr_debug("%s %d %d %d\n", __func__, line, g_boot_reason, atomic_read(&g_br_state));
			./drivers/misc/mediatek/boot_reason/mt_boot_reason.c:102:	return g_boot_reason;
			./drivers/misc/mediatek/boot_reason/mt_boot_reason.c:113:	pr_debug("boot_reason = %d, state(%d,%d,%d)", g_boot_reason,
			./drivers/misc/mediatek/include/mt-plat/battery_meter.h:271:	FG_DAEMON_CMD_GET_BOOT_REASON,	/* g_boot_reason, */
			./drivers/power/mediatek/battery_meter_fg_20.c:3895:			bm_debug("[fg_res] g_boot_reason = %d\n", boot_reason);
			./drivers/power/mediatek/battery_common_fg_20.c:2248:	battery_log(BAT_LOG_CRTI,"g_gn_screenon_time=(%d), g_call_state=%d, g_boot_reason=%d, g_boot_mode=%d\n", g_gn_screenon_time, g_call_state, get_boot_reason(),get_boot_mode());

	
	
	
			c.g_gn_screenon_time 判断亮屏时间，这个是通过led亮的时间，因为守护进程10s一次，检测led是否亮的，如果亮+10s
	
			但是这个好像有问题
			./drivers/misc/mediatek/include/mt-plat/battery_common.h:329:extern unsigned int g_gn_screenon_time;
			./drivers/power/mediatek/switch_charging.c:142:	battery_log(BAT_LOG_CRTI, "[BATTERY] bat_charge_current_not_down, g_gn_screenon_time=%d, g_call_state=%d, is_enter_mmi_test=%d, g_boot_mode=%d !\n\r", g_gn_screenon_time, g_call_state, is_enter_mmi_test, get_boot_mode());    
			./drivers/power/mediatek/switch_charging.c:144:	if (((g_gn_screenon_time <= 30) && (g_call_state == CALL_IDLE)) || (is_enter_mmi_test == KAL_TRUE) || (get_boot_mode() == KERNEL_POWER_OFF_CHARGING_BOOT || get_boot_mode() == LOW_POWER_OFF_CHARGING_BOOT))
			./drivers/power/mediatek/battery_common_fg_20.c:315:unsigned int g_gn_screenon_time = 0;
			./drivers/power/mediatek/battery_common_fg_20.c:2245:		g_gn_screenon_time = g_gn_screenon_time + 10;
			./drivers/power/mediatek/battery_common_fg_20.c:2247:		g_gn_screenon_time = 0;
			./drivers/power/mediatek/battery_common_fg_20.c:2248:	battery_log(BAT_LOG_CRTI,"g_gn_screenon_time=(%d), g_call_state=%d, g_boot_reason=%d, g_boot_mode=%d\n", g_gn_screenon_time, g_call_state, get_boot_reason(),get_boot_mode());
	
	
			./drivers/misc/mediatek/leds/mt6735/leds.c:120:unsigned int mt_get_bl_brightness(void)
			./drivers/misc/mediatek/leds/mt6735/leds_hal.h:10:extern unsigned int mt_get_bl_brightness(void);
			./drivers/misc/mediatek/leds/leds_drv.c:511:			bl_brightness = mt_get_bl_brightness();
			./drivers/power/mediatek/battery_common_fg_20.c:316:extern unsigned int mt_get_bl_brightness(void);
			./drivers/power/mediatek/battery_common_fg_20.c:2244:	if(mt_get_bl_brightness() != 0)


		
		d.gn_call_state 判断是否处于打电话的状态

		/*****************************************************************************
		 *  CallState
		 ****************************************************************************/
		#define CALL_IDLE (0)
		#define CALL_ACTIVE (1)
		
		./drivers/misc/mediatek/include/mt-plat/battery_common.h:316:extern kal_bool g_call_state;
		./drivers/power/mediatek/switch_charging.c:142:	battery_log(BAT_LOG_CRTI, "[BATTERY] bat_charge_current_not_down, g_gn_screenon_time=%d, g_call_state=%d, is_enter_mmi_test=%d, g_boot_mode=%d !\n\r", g_gn_screenon_time, g_call_state, is_enter_mmi_test, get_boot_mode());    
		./drivers/power/mediatek/switch_charging.c:144:	if (((g_gn_screenon_time <= 30) && (g_call_state == CALL_IDLE)) || (is_enter_mmi_test == KAL_TRUE) || (get_boot_mode() == KERNEL_POWER_OFF_CHARGING_BOOT || get_boot_mode() == LOW_POWER_OFF_CHARGING_BOOT))
		./drivers/power/mediatek/switch_charging.c:800:			if((BMT_status.temperature < 15) || ((g_call_state != CALL_IDLE) && (!is_enter_mmi_test)))
		./drivers/power/mediatek/switch_charging.c:805:				battery_log(BAT_LOG_CRTI, "[BATTERY] temperature = %d, g_call_state = %d, is_enter_mmi_test = %d, g_temp_CC_value = %d\n", BMT_status.temperature, g_call_state, is_enter_mmi_test, g_temp_CC_value); 
		./drivers/power/mediatek/switch_charging.c:1062:			if((BMT_status.temperature < 15) || ((g_call_state != CALL_IDLE) && (!is_enter_mmi_test)))
		./drivers/power/mediatek/switch_charging.c:1067:				battery_log(BAT_LOG_CRTI, "[BATTERY] temperature = %d, g_call_state = %d, is_enter_mmi_test = %d, g_temp_CC_value = %d\n", BMT_status.temperature, g_call_state, is_enter_mmi_test, g_temp_CC_value); 
		./drivers/power/mediatek/switch_charging.c:1327:	if (BMT_status.bat_vol < TALKING_RECHARGE_VOLTAGE || g_call_state == CALL_IDLE) {
		./drivers/power/mediatek/linear_charging.c:1317:	    || g_call_state == CALL_IDLE) {

		./drivers/power/mediatek/battery_common_fg_20.c:132:unsigned int g_call_state = CALL_IDLE;
		./drivers/power/mediatek/battery_common_fg_20.c:1535:	battery_log(BAT_LOG_CRTI, "call state = %d\n", g_call_state);
		./drivers/power/mediatek/battery_common_fg_20.c:1536:	return sprintf(buf, "%u\n", g_call_state);
		./drivers/power/mediatek/battery_common_fg_20.c:1543:	if (kstrtouint(buf, 10, &g_call_state) == 0) {
		./drivers/power/mediatek/battery_common_fg_20.c:1544:		battery_log(BAT_LOG_CRTI, "call state = %d\n", g_call_state);
		./drivers/power/mediatek/battery_common_fg_20.c:2248:	battery_log(BAT_LOG_CRTI,"g_gn_screenon_time=(%d), g_call_state=%d, g_boot_reason=%d, g_boot_mode=%d\n", g_gn_screenon_time, g_call_state, get_boot_reason(),get_boot_mode());
		./drivers/power/mediatek/battery_common_fg_20.c:2366:	if ((g_call_state == CALL_ACTIVE) && (BMT_status.bat_vol > V_CC2TOPOFF_THRES))
	./drivers/power/mediatek/battery_common_fg_20.c:4215:	if (g_call_state == CALL_ACTIVE
	
		
			
		这步应该是根据当前的一个状态判断是否要降电流		


		//Gionee <GN_BSP_CHG> <lilubao> <20170819> add for gn_boot info begin
		bool bat_charge_current_not_down(void)
		{
			int gn_boot_mode ;
			gn_boot_mode=get_boot_mode();

			pr_err("in [%s] by lilubao gn_boot_mode->%d,gn_call_state->%d\n",
						__FUNCTION__,gn_boot_mode,gn_call_state);

			if( /*(g_gn_screenon_time <= 30) && (gn_call_state == CALL_IDLE)) || */(is_enter_mmi_test == true) || (gn_boot_mode == KERNEL_POWER_OFF_CHARGING_BOOT || gn_boot_mode == LOW_POWER_OFF_CHARGING_BOOT))
			{
				pr_err("in [%s] 1111111111 \n",__FUNCTION__);
				return true;
			}	
			else{
				pr_err("in [%s] 2222222222 \n",__FUNCTION__);
				return false;
			}
	
		}
		//Gionee <GN_BSP_CHG> <lilubao> <20170819> add for gn_boot info end



		17G10A 电芯规格书，请确认一下配机参数。重点关注一下充电参数。
		0~15，0.3C，MAX to 4.2 V cutoff		900mA
		15~45 2.5A to 4.2V ,2A to 4.4V ,then CV to 0.02C
		45~60 0.5C MAX to 4.2V				1500mA

		//Gionee <GN_BSP_CHG> <lilubao> <20170819> add for gn_boot info begin
		pr_err("in [%s] by lilubao  gn_call_state->%d,temperature->%d\n,is_enter_mmi_test->%d\n",
				__FUNCTION__,gn_call_state,info->battery_temperature,is_enter_mmi_test);

		if( bat_charge_current_not_down() ){

	
			pr_err("11111111111\n");
			pr_err("normal mode,not change any action\n");
		}else{
			pr_err("222222222222\n");

			if( info->battery_temperature <=15 ){
				pdata->charging_current_limit=900000;
				pdata->input_current_limit=1000000;
				pr_err("temperature is too low ,charging_current_limit->%d,or is_enter_mmi_test is disable\n",
							pdata->charging_current_limit);
			}else if( (info->battery_temperature>15)&&(info->battery_temperature<=40) ){

				pr_err("temperature in normal range\n");
			}else if( info->battery_temperature > 40 ){

				pdata->charging_current_limit=1400000;
				pdata->input_current_limit=1500000;
				pr_err("temperature is too high ,charging_current_limit->%d\n",pdata->charging_current_limit);
			}

			if( gn_call_state != CALL_IDLE ){

				pr_err("333333333333\n");
				pr_err("calling state ,decrease charging current\n");

				pdata->charging_current_limit  = pdata->charging_current_limit/2;
				pdata->input_current_limit = pdata->input_current_limit/2;
			}

		}
		//Gionee <gn_by_charging> <lilubao> <20170705> add for fixed mmi end

	}	
    
    
    
    
    

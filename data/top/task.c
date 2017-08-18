

/*已处理完问题*/
{
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

}















/*测试上的BUG*/
{
20170817
	1.手上的BUG													20170817
		还有两类BUG：一类是低电插上充电器不识别，关机；一类是USB不识别问题
		已经提case
		
	2.温升问题，温升降电流有没有起作用							20170818
		主要是分场景降低充电电流，比如打电话，拍照，听音乐，玩游戏等(具体是否还有其他还有带确认)
		温升部分主要有两个一个是上面的根据不同场景降低充电电流问题		计划今天完成
		还有一类是MTK的 thermal regulator，bcct这部分要熟悉相关文档，代码
		
		
		
		
	3.电量显示的不准												20170818
		这个的主要问题是充电时候电池电压跟电量显示差别比较大(差别在10%以上)，放电比较准确
		这部分跟GM3.0的客制化有关，但是相关的文档还是不熟悉，必须弄清楚
		相关的软件自己首先要熟悉
		
				
	4.高通USB相关的框架，文档，代码必须弄清楚，不然很多你都不懂
	一定要下力气，下决心打通这部分
	
	
				
	5.相关测试
	17G10A
	白盒测试：													20170817
		4.LDO: VIO18/VCN33/VEMC/VCAMD2纹波略大,NG
		5.charger I2C ：fast mode plus 与high speed mode均是Trclk、Trdat、Tfclk、Tfdat偏低，NG
	整机测试：													20170817
		3.配合旅充充电测试       2PCS     NG  开机待机状态下充电，手机表面温度偏高，正面温度为38.6℃、背面温度为44.3℃，（标准为金属≤38℃；塑胶/橡胶 ≤43℃；玻璃≤41℃）。
		5.配合电脑USB充电功能测试      2PCS        NG  关机状态下手机连接电脑充电，电量充电至60%后充电电流由466mA将至370mA，跳变的原因最好能找到。	
	
	17G06A:
	眼图测试失败的case
		You can follow doc 80-PB524-1 usb_tunning_-_phy_eye_diagram_for_oems(1).pdf
		and 80-NA648-1_H_Tuning_USB_PHY_Eye_Diagram_Receiver_Sensitivity.pdf on eye diagram tunning. 	
	
	
	
	
	
	
	
	
	
	
	
	
	
2.温升问题，根据不同场景降电流
/******************************************************************************************/		
1.需要仿照G1605A添加一些boot mode,boot reason,call state,g_screen，call_state
[bat_routine_thr]g_gn_screenon_time=(80), g_call_state=0, g_boot_reason=4, g_boot_mode=0


		//Gionee <GN_BSP_CHG> <lilubao> <20170816> add for boot information begin
		但是这个g_gn_screenon_time好像是跟插入充电器有关的，不插入充电器不一定有
			
		a. gn_boot_mode				
			
			
			
			
			
			
			
		
	
	
	
	
	

	17G10A
	{
	
	
		#94044
		测机关机状态下连充电器，短按2s不开机长按11s也不开机（有振动），待1min后测机自动开机恢复		2017.8.10
		{		
		 2017-8-3	16：54~16：55
			电量1%，长按时间14+s这个超过了软件重启的时间
			电量低的情况下手机要充一段时间才能恢复，所以关机充电条件下可能有段时间不亮屏
				
		}

		
		

		
		#99363	****
		待机一晚上早上手机电量1%时，连接充2A电器不充电，连充电器30min插拔充电器不恢复，30min后待电量自动耗完再次连充电器恢复
		{
		1
		分析：
		2017-8-16 08：12左右插入充电器，不充电，然后关机，关机也不显示关机充电
		
			<6>[    5.475795]  (0)[182:irq/773-rt5081_]rt5081_pmu_charger rt5081_pmu_charger: rt5081_pmu_attachi_irq_handler
			<6>[    5.475882]  (0)[182:irq/773-rt5081_]rt5081_pmu_charger rt5081_pmu_charger: rt5081_inform_psy_changed: online = 1, type = 1
			<6>[    5.475892]  (0)[182:irq/773-rt5081_]mt_charger_set_property
			<6>[    5.475899]  (0)[182:irq/773-rt5081_]mt_charger_set_property
			<3>[    5.475906]  (0)[182:irq/773-rt5081_]dump_charger_name: charger type: 1, Standard USB Host
			<3>[    5.475922]  (0)[182:irq/773-rt5081_]mtk_charger_int_handler
			<3>[    5.475928]  (0)[182:irq/773-rt5081_]wake_up_charger
			<3>[    5.475948]  (0)[182:irq/773-rt5081_][fg_charger_in_handler] notify daemon 0 1
			<6>[    5.475965]  (0)[182:irq/773-rt5081_]rt5081_pmu_charger rt5081_pmu_charger: rt5081_enable_chgdet_flow: en = 0
			<3>[    5.476009]  (0)[245:kworker/0:1][U3D]U3D_LTSSM_INTR: 0
			<6>[    5.476122]  (6)[182:irq/773-rt5081_]rt5081_pmu_charger rt5081_pmu_charger: rt5081_set_usbsw_state: state = 1
		
		充电器可能没有插好 因为软件没有走那步流程
		MTK case ID：ALPS03468158
		
		
		
		}



		
		#99348
		手机连接电脑USB打开传文件和临时USB调试后，电脑不显示连接到手机，再次插拔USB不恢复，插其它手机可以正常识别，重启恢复
		{
			这个问题是连接到PC没有显示盘符，重启后恢复，08：03~08：05
		
			
		
		}
		
	
	}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	17G06A
	{
	  USB：
		#96132，adb端口被占用			
		2017.8.10
		{
		 问题：
			现在要解决的是如何让diag口跟adb如何共用，就是打开diag口同时可以使用adb命令
			
		 
		 分析：
		    17G06A不能用，17G02A可以使用，对比两个项目的代码，初始化到打开端口的log
		    
		    对比代码：	
		
			
		
			涉及到的文件：
			{
				init.qcom.usb.rc
				init.gn.platform.rc
				init.msm.usb.configfs.rc
				init.qcom.factory.rc
				
				
				./alps_drv/device/qcom/common/rootdir/etc/init.qcom.usb.rc
			}
		}
		
		

		#96141，*#837504#手动开启diag口，PC端和工具找不到diag口
		2017.8.10
		打开diag口之后可能是无法识别，然后USB端口无法使用 pid 9001
		是不是USB的vid，pid配置不对
		直接连接PC  pid 9039
		打开diag口  pid 9091
		
		
		
		
		
		
		
2017-08-17-14-33-04 start to build adsp
----Begin build adsp-----
cd ADSP.8953.2.8.2; cd adsp_proc; python ./build/build.py -c msm8937 -o all
Running: cd ADSP.8953.2.8.2; cd adsp_proc; python ./build/build.py -c msm8937 -o all
Build adsp failed, exit
Running: cp /home/llb/project/PRO/source/17G06A/L33_QCOM_8920_17G16A_170605_ALPS/modem_qcom_mp/L33_QCOM_8920_17G16A_170605_MODEM/BuildLogs/BJ17G06A01_2017-08-17-14-33-02 release/BJ17G06A01_MODEM_T0012

		
		
		
		
		
		
		
	
	}
}










/*待处理的问题*/
{

	中断问题，如何根据中断号准确定位哪个模块的问题,添加到log中
	
	深入理解中断子系统，中断的来龙去脉，整个流程
	
	功耗问题的分析，整理思路，丰富手段
	
	
	充电电流太小
	
	
	底电流偏高
	
	
	电量跳变问题
	
	
	整机测试出现的问题，高温报警，低温关机问题
	
	
	电池曲线的导入和fuelgauge3.0的客制化
	
	
	关机充电的指示灯
	
	

/********************************************************************************************************************************/


	G1605A  售后问题，功耗问题 healthd线程
	{
		售后问题：   电量计不准的机器

		apk发包次数，谁发的(主从)，就是链接到哪了？？
	    那个脚本怎么用


		这个还不知道怎么看，只是觉得电量跟电池电压偏差有点大，而且底层电量跟上层显示的电量差别也很大

	}
	
	







/********************************************************************************************************************************/

17G10A p1试产待解决问题
{
	对一个问题的分析，处理，解决方案要形成自己的一套处理体系，
	什么问题？什么现象？跟什么有关？时间点，复现的概率，软件还是硬件，log还有数据...
	什么方向，怎么解决，解决后的现象是什么样的？所有问题都要自己事先理清楚，想明白



	17G10A当前比较重要的问题：
	{
		1.USB的vid，pid特别是meta 模式下的0x0e05 function： adb ,acm
	
		2.fuelgauge3.0的客制化，熟悉流程，GM3.0的硬件测试点，nvram写入电量计系数
	
		3.充电后面的ibus跳变问题
	
		4.仿照G1605A，添加手机处在打电话状态
	
		5.整机测试出现的一些列问题
	}
























	电量计校准，没有测试点，这个会影响到电量计的准确度
	手上的手机：T1-004，T1-083，T1-672,
	T1-573这个机器可能NTC电阻出问题，导致一直显示battery temperature >60 无法开机
	

	电池曲线的导入		7.27
	{
		
		涉及到哪些文件,哪些细节
		{
			mtk_battery_table.h  ，mtk_battery_property.h
			mt6757_battery_prop_ext.dtsi,mt6757_battery_table_ext.dtsi
			
			GM3.0的客制化步骤
			{
				1.测量car_tune_value 系数
				
				2.利用GMAT_TOOL导出 
				-> battery_prop_ext.dtsi
				
				-> ZCV table，battery_table_ext.dtsi
				
				3.工厂模式下利用 ATE_tool校准参数Rfg ，meta模式下修改NVRAM中存储的car_tune_value
				这种应该是单独修正每一台机器的car_tune_value的值，因为这个值不同机器可能不一样
			}
		}
		
		

		电量计相关的debug
		{
			//Gionee <gn_by_CHG> <lilubao> <20170804> add for fuelgauge begin
			具体放充电库伦值大小
			[FGADC_intr_end][FG_INTR_IAVG]
			read_fg_hw_info
			
			
			打开电量计相关的log
			adb shell setprop persist.mediatek.fg.log.enable 1
			
			手动提高fg log等级
			adb shell
			echo 8 > /sys/bus/platform/devices/battery/FG_daemon_log_level
			
			Gauge Low power mode 关闭方法
			adb shell
			
			echo 3000 > /sys/devices/platform/mt-pmic/pmic_access
			
			cat /sys/devices/platform/mt-pmic/pmic_access
			
			看看出來的值是多少		regiser-> FGADC_CON  (low dropout regulator P47)
			假設出來的值是 2319
			用2進位查看這個値, 我們要更改bit 8 的值 (FG_SON_SLP_EN)  , 將其從 1 改成 0 (從 enable 改為disable)
			
			將更改後的新值(2219)透過adb command寫回register
			echo 3000 2219 > /sys/devices/platform/mt-pmic/pmic_access
			
			再讀取一次值 double confirm是否讀取出來已經為新的值
			echo 3000 > /sys/devices/platform/mt-pmic/pmic_access

			cat /sys/devices/platform/mt-pmic/pmic_access    
		}
		
		
		电量计相关参数的测量以及意思
		{
		  car_tune_value	
			1、 先帮忙连接SP_META Tool确认下这台机器的NVRAM里面的CAR_TUNE_VALUE值的大小。

			2、 通过外灌电流，然后确认下工模里读取到的电流是否有偏差。外灌电流的大小从1A->500mA->300mA-100mA>50mA->10mA往下调，

			确认电流是否有偏差。前提得保证外灌电流大小的精准度。
			
			car_tune_value 100  ->   948mA		105
			
			CALIBRATE_CAR_TUNE_VALUE_BY_META_TOOL这个宏用于在工厂模式下利用ATE_TOOL校准Rfg参数
			这个值跟板极的阻抗有关
			
			这个宏用于工厂校准value参数时打开，会把库仑计的系数写到一个全局变量里
			#ifdef CALIBRATE_CAR_TUNE_VALUE_BY_META_TOOL
			bm_notice("[fg_res] cali_car_tune = %d, default = %d, Use [cali_car_tune]\n",
				cali_car_tune, fg_cust_data.car_tune_value);
				fg_cust_data.car_tune_value = cali_car_tune;
			#else
				bm_notice("[fg_res] cali_car_tune = %d, default = %d, Use [default]\n",
						cali_car_tune, fg_cust_data.car_tune_value);
			#endif

			1、#define DIFFERENCE_FULLOCV_ITH       400         /* mA */
			
			2、#define EMBEDDED_SEL 1

			3、#define FG_METER_RESISTANCE    		75

		}
		
		
		开机充电(四个温度)，关机充电，放电等三种情况
		{
			
			1.开机充电，在四个温度下测试充到满电
			{
				电量显示的差别很大,充电为什么底层电量还会减少
				
				充电过程中温度很高
			
			}
		
		
			2.现在关机充电遇到的情况：进电池的是2A，但是ibus上电流经常跳变，有时候3A，有时候有是1A
			{
				系统功耗很大，而且是间断性的跳变，这样会导致充进电池的电流很小
				系统功耗很大可能是开机关机都有的问题，先要确定是软件还是硬件的问题
			
				也可能是充电器或者aicl检测(可能有偏差)到的能力很大，设定值很高，最后设定电流达到3A的时候，然后又被拉下来了
			
				aicl检测的是后设定一个上限，如果超过这个值就设为指定值，防止充电电流过高
			
				这个充电电流跳变的问题，开机关机充电都存在，什么把电流拉下来了，充电的时候底层跟上层电量显示的差异，开机电压，关机电压的问题
			
			
			}	
			
			
			3.放电时序，放电是否正常
			{
				建议做下放电曲线的量测，如附件是参考案例的模板。

				1、 放电曲线的量测的场景：
				一、温度25度：重载（可以重载游戏，控制电流输出为1A左右）、轻载（可以播放MP3，控制电流输出为400mA左右）

				二、温度0度：重载（可以重载游戏，控制电流输出为1A左右）、轻载（可以播放MP3，控制电流输出为400mA左右）

				2、 先充电充到满，即battery报Full后开始放电，用ttermpro串口工具（此工具记录时间）抓取Uart Log，直到手机关机。

				3、 重点关注UI_SOC放电曲线的平滑度，及最后放电几percent（5%～1%）的在每percent keep住的时间，会不会出现类似在2%时突然Drop关机的状况。若有，则需要调整参数。
			}
		
		
		}
		
		
		GM3.0相关的patch
		{
			ALPS03420707：
			修复待机是电量计计算可能出现的异常
		
			ALPS03420700：
			UI快速掉电问题，从100%迅速掉到0%
			
			ALPS03158638
			ALPS03248687
			ALPS03252445？
			ALPS03253502
			ALPS03258450？
			ALPS03245474
			ALPS03250083？
			ALPS03287248
			ALPS03285277
			
			
			Dear customer, ALPS03434294(For_gnbj6757_66_n_alps-mp-n0.mp5-V1_P3).tar.gz released. (Download ID: REQ30000429871)
			
			fuelgauge 3.0的问题,文档+邮件
			{
				pacth申请 已申请
				ALPS03411143 
				Fix gauge coulomb value may abnormal in some mode / 修復待機時電量計算可能異常issue 

				ALPS03158638   
				【L3500-Charger】voltage mode在低温下的修正
			}
		}
	}
	




	使用标准充电器充电，几十分钟之后，ibus上的充电电流频繁跳变 
	{
		case ID：ALPS03447407 
		
	}
	
	
	温升好像有问题,bcct好像没有生效		20170816
	{
	
	
	}
	
	
	

	整机测试
	{
		2PCS手机因电池温度过高关机，但环y箱里设定温度为57度，要求做高温高湿测试时手机可以有高温安全警告，但不允许执行关机机制； 
	
		低温测试，-20度，手机自动关机
	
	}
	

	
	
	底电流偏高
	{
		这里主要是android系统的睡眠唤醒机制，然后为了便于分析功耗问题
		
		一下是几个功耗底电流调试相关文档
	    80-p0897-1_a   
	    80-p0955-1sc_b 
	    高通bsp技术期刊 2014/11/28
		
	
		1.中断持锁EINT wakelock次数很多导致系统一直没有睡下去
		lk里面sc卡配置有问题，导致频繁上报中断
		
		还有一个不同阶段的gpio配置
		
		
		2.打印中断号，地址，wakeup.c
		在有中断持锁的时候知道是哪些中断
		
		
		3.系统的休眠，唤醒流程
		{
			"mem", "standby", "freeze" and "disk"
		
		}
		
		
		4.如何注掉一些驱动
		
		
		5.系统进入suspend 还有一些低功耗模式，一些流程
		
		
		5.分析思路


	
	
		平均电流9mA，lcd有3mA，其他还有1mA
		1mA这个还有待查
		
		
		1.跟系统通知有关，即使飞行模式下，系统通知也会每隔一段时间会有唤醒ap检测一些信息如网络连接，想推送一些内容
			这部分波形比较高持续时间比较长 40~60s
		  现在是每隔10min唤醒一次，去除之后电流从 11mA 降到9mA，而且稳定，电流没有跳变	
		  
		2.短的异常波形跟中断有关，eint - 150，中断唤醒的持续时间较短5~10s 
		  初步分析跟串口同modem之间的通信有关
		01-01 08:06:14.378303 01-01 08:06:14.378302  1436  1436 I [  368.329961]-(0)[1436:system_server][name:irq_mtk_eic&]EINT_STA:  
		01-01 08:06:14.378303 01-01 08:06:14.378302  1436  1436 I [  368.329961]-(0)[1436:system_server][name:irq_mtk_eic&]: EINT Module - index:128,EINT_STA = 0x400000
		01-01 08:06:14.378303 01-01 08:06:14.378302  1436  1436 I [  368.329961]-(0)[1436:system_server][name:irq_mtk_eic&]: EINT 150 is pending
		01-01 08:06:14.378303 01-01 08:06:14.378302  1436  1436 I [  368.329961]-(0)[1436:system_server][name:irq_mtk_eic&]:  
		01-01 08:06:14.378303 01-01 08:06:14.378302  1436  1436 F [  368.329961]-(0)[1436:system_server][name:mtk_spm_sleep&]: [SPM] dump ID_DUMP_MD_SLEEP_MODE
		01-01 08:06:14.378303 01-01 08:06:14.378302  1436  1436 I [  368.329961]-(0)[1436:system_server]: Resume caused by IRQ 27, SPM
		01-01 08:06:14.378303 01-01 08:06:14.378302  1436  1436 D [  368.329961]-(0)[1436:system_server][name:mtk_wdt&]: mtk_wdt_mode_config  mode value=dd, tmp:220000dd,pid=1436
		01-01 08:06:14.378303 01-01 08:06:14.378302  1436  1436 D [  368.329961]-(0)[1436:system_server][name:mtk_wdt&]: [WDT] resume(1)
		01-01 08:06:14.378325 01-01 08:06:14.378324  1436  1436 E [  368.329983]-(0)[1436:system_server][name:ccci&]: [ccci1/mcd]ccci_modem_sysresume
		01-01 08:06:14.379268 01-01 08:06:14.379267  1436  1436 E [  368.330926]-(0)[1436:system_server][name:ccci&]: [ccci1/mcd]Resume no need reset cldma for md_state=1
		01-01 08:06:14.380476 01-01 08:06:14.380475  1436  1436 I [  368.332134]-(0)[1436:system_server][name:atf_log&]atf_time_sync: resume sync
		01-01 08:06:14.381082 01-01 08:06:14.381081  1436  1436 E [  368.332740]-(0)[1436:system_server][name:mtk_cpufreq_hybrid&]: [CPUHVFS] (1) cluster0: opp = 4 (0 - 11), freq = 1508000, volt = 0x56
		01-01 08:06:14.382604 01-01 08:06:14.382603  1436  1436 E [  368.334262]-(0)[1436:system_server][name:mtk_cpufreq_hybrid&]: [CPUHVFS] (1) cluster1: opp = 15 (0 - 9), freq = 494000, volt = 0x56
		01-01 08:06:14.384157 01-01 08:06:14.384156  1436  1436 E [  368.335815]-(0)[1436:system_server][name:mtk_cpufreq_hybrid&]: [CPUHVFS] (1) [00000001] cluster0 on, pause = 0x9, swctrl = 0x20f4 (0x56bb)
		01-01 08:06:14.386208 01-01 08:06:14.386207  1436  1436 I [  368.337866].(0)[1436:system_server][name:cpu&]: Enabling non-boot CPUs ...
		01-01 08:06:14.388081 01-01 08:06:14.388080     0     0 I [  368.339739]-(1)[0:swapper/1][name:cpuinfo&]: Detected VIPT I-cache on CPU1
		01-01 08:06:14.388120 01-01 08:06:14.388119     0     0 I [  368.339778]-(1)[0:swapper/1][name:irq_gic_v3&]CPU1: found redistributor 1 region 0:0x000000000c220000
		01-01 08:06:14.388249 01-01 08:06:14.388248     0     0 I [  368.339907]-(1)[0:swapper/1][name:smp&]CPU1: Booted secondary processor [410fd034]
		01-01 08:06:14.389321 01-01 08:06:14.389320  1436  1436 I [  368.340979].(1)[1436:system_server][name:cpu&]: CPU1 is up
		
		3.lcd内屏+外屏 约有 3~4mA电流，这个导致整体的底电流在11mA+
		
		
		
		1->
		EINT wakelock	63		548		0		63		0		51272		2932		296780		0
		2->
		EINT wakelock	90		584		13		89		71		65361		2932		445681		0




		01-01 08:06:14.636166 01-01 08:06:39.186808   228   228 E [  368.587824].(2)[228:kworker/u16:5][name:msdc&]: [msdc]msdc1, some of device s pin, dat1~3 are stuck in low!
		01-01 08:06:14.637566 01-01 08:06:39.188208   228   228 E [  368.589224].(2)[228:kworker/u16:5][name:msdc&]: [msdc]msdc_ops_switch_volt msdc1 set voltage to 3.3V.

		msdc_check_dat_1to3_high
		msdc_ops_switch_volt


		wakeup_reason.c   
		irq_mtk_eic.c

		max_eint_num = <160>;

		mt_eint_print_status


		01-01 08:06:14.378303 01-01 08:06:14.378302  1436  1436 I [  368.329961]-(0)[1436:system_server][name:irq_mtk_eic&]EINT_STA:  
		01-01 08:06:14.378303 01-01 08:06:14.378302  1436  1436 I [  368.329961]-(0)[1436:system_server][name:irq_mtk_eic&]: EINT Module - index:128,EINT_STA = 0x400000
		01-01 08:06:14.378303 01-01 08:06:14.378302  1436  1436 I [  368.329961]-(0)[1436:system_server][name:irq_mtk_eic&]: EINT 150 is pending

		01-01 08:06:14.378303 01-01 08:06:14.378302  1436  1436 I [  368.329961]-(0)[1436:system_server][name:irq_mtk_eic&]:  
		01-01 08:06:14.378303 01-01 08:06:14.378302  1436  1436 F [  368.329961]-(0)[1436:system_server][name:mtk_spm_sleep&]: [SPM] dump ID_DUMP_MD_SLEEP_MODE
		01-01 08:06:14.378303 01-01 08:06:14.378302  1436  1436 I [  368.329961]-(0)[1436:system_server]: Resume caused by IRQ 27, SPM
		01-01 08:06:14.378303 01-01 08:06:14.378302  1436  1436 D [  368.329961]-(0)[1436:system_server][name:mtk_wdt&]: mtk_wdt_mode_config  mode value=dd, tmp:220000dd,pid=1436
		01-01 08:06:14.378303 01-01 08:06:14.378302  1436  1436 D [  368.329961]-(0)[1436:system_server][name:mtk_wdt&]: [WDT] resume(1)
		01-01 08:06:14.378325 01-01 08:06:14.378324  1436  1436 E [  368.329983]-(0)[1436:system_server][name:ccci&]: [ccci1/mcd]ccci_modem_sysresume

		01-01 08:06:14.379268 01-01 08:06:14.379267  1436  1436 E [  368.330926]-(0)[1436:system_server][name:ccci&]: [ccci1/mcd]Resume no need reset cldma for md_state=1
		01-01 08:06:14.380476 01-01 08:06:14.380475  1436  1436 I [  368.332134]-(0)[1436:system_server][name:atf_log&]atf_time_sync: resume sync

		01-01 08:06:14.381082 01-01 08:06:14.381081  1436  1436 E [  368.332740]-(0)[1436:system_server][name:mtk_cpufreq_hybrid&]: [CPUHVFS] (1) cluster0: opp = 4 (0 - 11), freq = 1508000, volt = 0x56
		01-01 08:06:14.382604 01-01 08:06:14.382603  1436  1436 E [  368.334262]-(0)[1436:system_server][name:mtk_cpufreq_hybrid&]: [CPUHVFS] (1) cluster1: opp = 15 (0 - 9), freq = 494000, volt = 0x56
		01-01 08:06:14.384157 01-01 08:06:14.384156  1436  1436 E [  368.335815]-(0)[1436:system_server][name:mtk_cpufreq_hybrid&]: [CPUHVFS] (1) [00000001] cluster0 on, pause = 0x9, swctrl = 0x20f4 (0x56bb)
		01-01 08:06:14.386208 01-01 08:06:14.386207  1436  1436 I [  368.337866].(0)[1436:system_server][name:cpu&]: Enabling non-boot CPUs ...
		01-01 08:06:14.388081 01-01 08:06:14.388080     0     0 I [  368.339739]-(1)[0:swapper/1][name:cpuinfo&]: Detected VIPT I-cache on CPU1
		01-01 08:06:14.388120 01-01 08:06:14.388119     0     0 I [  368.339778]-(1)[0:swapper/1][name:irq_gic_v3&]CPU1: found redistributor 1 region 0:0x000000000c220000
		01-01 08:06:14.388249 01-01 08:06:14.388248     0     0 I [  368.339907]-(1)[0:swapper/1][name:smp&]CPU1: Booted secondary processor [410fd034]
		01-01 08:06:14.389321 01-01 08:06:14.389320  1436  1436 I [  368.340979].(1)[1436:system_server][name:cpu&]: CPU1 is up

		01-01 08:06:14.393848 01-01 08:06:14.393847  1436  1436 I [  368.345506].(1)[1436:system_server][name:main&]PM: noirq resume of devices complete after 3.684 msecs



		01-01 08:06:21.488047 01-01 08:06:46.038689  1436  1436 I [  375.439705].(0)[1436:system_server][name:wakeup&]PM: Wakeup pending, aborting suspend

		01-01 08:06:21.489259 01-01 08:06:46.039901  1436  1436 I [  375.440917].(0)[1436:system_server][name:wakeup&]: last active wakeup source: EINT wakelock

		01-01 08:06:21.490324 01-01 08:06:46.040966  1436  1436 W [  375.441982].(0)[1436:system_server][name:process&]:  
		01-01 08:06:21.490956 01-01 08:06:46.041598  1436  1436 E [  375.442614].(0)[1436:system_server][name:process&]: Freezing of tasks aborted after 0.011 seconds[name:process&]
		01-01 08:06:21.491954 01-01 08:06:46.042596  1436  1436 I [  375.443612].(0)[1436:system_server][name:process&]: Restarting tasks ... [name:process&]done.
		01-01 08:06:21.508172 01-01 08:06:46.058814  1436  1436 I [  375.459830].(1)[1436:system_server][name:pmic_auxadc&]: [mt6355_get_auxadc_value] ch = 5, reg_val = 0x630, adc_result = 696
		01-01 08:06:21.511222 01-01 08:06:46.061864  1436  1436 I [  375.462880].(0)[1436:system_server][name:pmic_auxadc&]: [mt6355_get_auxadc_value] ch = 2, reg_val = 0x369, adc_result = 767
		01-01 08:06:21.512974 01-01 08:06:46.063616  1436  1436 E [  375.464632].(0)[1436:system_server][name:mtk_battery_hal&]: [fgauge_read_current] final current=691 (ratio=950)
		01-01 08:06:21.514261 01-01 08:06:46.064903  1436  1436 E [  375.465919].(0)[1436:system_server][name:mtk_battery&]: [BattVoltToTemp] 768 24000 2800 1
		01-01 08:06:21.515297 01-01 08:06:46.065939  1436  1436 E [  375.466955].(0)[1436:system_server][name:mtk_battery&]: [force_get_tbat] 767,768,0,69,100,27 r:50 100
		01-01 08:06:21.516613 01-01 08:06:46.067255  1436  1436 D [  375.468271].(0)[1436:system_server]rt5081_pmu 5-0034: rt5081_pmu_reg_block_write: reg 07 size 4
		01-01 08:06:21.518169 01-01 08:06:46.068811  1436  1436 I [  375.469827].(0)[1436:system_server]rt5081_pmu_charger rt5081_pmu_charger: rt5081_enable_hidden_mode: en = 1
		01-01 08:06:21.519456 01-01 08:06:46.070098  1436  1436 D [  375.471114].(0)[1436:system_server]rt5081_pmu 5-0034: rt5081_pmu_reg_update_bits: reg 21 data c0
		01-01 08:06:21.520657 01-01 08:06:46.071299  1436  1436 D [  375.472315].(0)[1436:system_server]rt5081_pmu 5-0034: rt5081_pmu_reg_update_bits: mask f0
		01-01 08:06:21.521933 01-01 08:06:46.072575  1436  1436 D [  375.473591].(0)[1436:system_server]rt5081_pmu 5-0034: rt5081_pmu_reg_update_bits: reg 21 data 01
		01-01 08:06:21.523070 01-01 08:06:46.073712  1436  1436 D [  375.474728].(0)[1436:system_server]rt5081_pmu 5-0034: rt5081_pmu_reg_update_bits: mask 01
		01-01 08:06:21.539516 01-01 08:06:46.090158   235   235 I [  375.491174].(0)[235:hps_main]MobiCore mcd: Cpu 1 is going to die
		01-01 08:06:21.542572 01-01 08:06:46.093214   235   235 I [  375.494230].(0)[235:hps_main][name:smp&]CPU1: shutdown
		01-01 08:06:21.543753 01-01 08:06:46.094395   235   235 I [  375.495411].(0)[235:hps_main][name:psci&]psci: CPU1 killed.
		01-01 08:06:21.546277 01-01 08:06:46.096919   235   235 I [  375.497935].(0)[235:hps_main]MobiCore mcd: Cpu 1 is dead

		
		sd卡模块去除
		cust_mt6757_msdc.dtsi
		&mmc1 {
			clk_src = /bits/ 8 <MSDC30_CLKSRC_200MHZ>;
			bus-width = <4>;
			max-frequency = <200000000>;
			msdc-sys-suspend;
			cap-sd-highspeed;
			sd-uhs-sdr12;
			sd-uhs-sdr25;
			sd-uhs-sdr50;
			sd-uhs-sdr104;
			sd-uhs-ddr50;
			pinctl = <&mmc1_pins_default>;
			pinctl_sdr104 = <&mmc1_pins_sdr104>;
			pinctl_sdr50 = <&mmc1_pins_sdr50>;
			pinctl_ddr50 = <&mmc1_pins_ddr50>;
			register_setting = <&mmc1_register_setting_default>;
			host_function = /bits/ 8 <MSDC_SD>;
			cd_level = /bits/ 8 <MSDC_CD_LOW>;
			cd-gpios = <&pio 30 0>;
			status = "okay";
			vmmc-supply = <&mt_pmic_vmch_ldo_reg>;
			vqmmc-supply = <&mt_pmic_vmc_ldo_reg>;
		};


		其中的
		status = "okay";
		修改为：
		status = "disabled";
		}
		
		这个直接这样改是有问题的
	
	
	
	开关机充电存在的问题
	{
		关机充电有问题，而且关机充电状态下，电量显示可能有问题的

		开机电压达到3.6v，电量1%很长，关机电压偏高
		很早达到100%，充电曲线的不对称
		
		关机充电的充电电流很高2.1~2.3A
		
		电量计相关的log
	    FG_DAEMON_CMD_GET_VBAT
		FG_DAEMON_CMD_GET
	
	}
	
	
	【GMS-CTS Verifier测试】OTG相关测试应该屏蔽
	 {
		这个device.mk是干什么用的?
		xml文件好像用的很多，有必要弄清楚
	
		cts对权限有要求，权限过高会有安全隐患
	
		把gionee/code/driver/project_common/BJ17G10_DRV_COMMON/device/gionee_bj/gnbj6757_66_n/device.mk中的下面的permissions注释掉。

		USB OTG
		PRODUCT_COPY_FILES += frameworks/native/data/etc/android.hardware.usb.host.xml:system/etc/permissions/android.hardware.usb.host.xml

	 }



	插入充电器后的I2C时序或者说波形有问题，涉及到I2C通信相关的内容
	{
		请问一下，17g10的充电I2C通信速率是多少（clk频率），目前情况是fh->s->fh，之后没
		有通信数据了。是否没有调好呢？
		
		因为有出现三种频率，分别对应I2C的标准模式，快速模式和高速模式的频率
		
		标准模式和高速模式对应的地址也正确，快速模式频率400K是转换到高速模式之前的出现的
		
		最后一笔数据是高速模式的，在后面就只有空闲模式，没有I2C信号数据了
		
		且高速模式下对应的时序测试结果有异常
	
	}
	

	定时器相关的内容
 	{
 		有两个kthread function:hrtimer,fgtimer这是两个定时器相关的操作
		可以适当分析时钟相关的框架
		charger_kthread_hrtimer_func
		charger_kthread_fgtimer_func
 	}



	battery ID相关的内容，根据ID选择电池曲线
	{
		battery_id  ，研读相关代码
		读battery_id 电压，匹配电池曲线
		
		int g_fg_battery_id;

		#ifdef MTK_GET_BATTERY_ID_BY_AUXADC
		void fgauge_get_profile_id(void)
	
	}

   
   这几个关键字的代码逻辑
   {
   	
		pmic_throttling_dlpt

   		FGADC_D0		开机初始化的电流，电压，电量
		fg_current_avg  平均电流
		fg_current_act	消耗电量
		
		打开fg log
		adb shell setpro persist.mediatek.fg.log.enable 1
		
		dlpt_notify_thr
		
		
	   //这应该是跟低电保护策略有关
	   DLPT_FEATURE_SUPPORT  
	   {
	   		#if defined(DLPT_FEATURE_SUPPORT)

			if (g_boot_mode != META_BOOT && g_boot_mode != FACTORY_BOOT && g_boot_mode != ATE_FACTORY_BOOT) {
				/*pmic_set_register_value(PMIC_BATON_TDET_EN, 1);*/
	
				pmic_set_register_value(PMIC_RG_BATON_EN, 1);
				if (pmic_get_register_value(PMIC_RGS_BATON_UNDET) == 1) {

					dprintf(CRITICAL, "[BATTERY] No battry plug-in. Power Off.");
					mt6575_power_off();
				}
			}

			pchr_turn_on_charging(KAL_FALSE);
			/* disable SW charger power path */

			switch_charger_power_path_enable(KAL_FALSE);
			mdelay(50);

			get_dlpt_imix_r();
			/* after get imix, re-enable SW charger power path */

			switch_charger_power_path_enable(KAL_TRUE);
			mdelay(50);

			check_bat_protect_status();
			if (is_charging == 1) {
				pchr_turn_on_charging(KAL_TRUE);
				dprintf(CRITICAL, "turn on charging \n\r");
			}
			#endif //#if defined(DLPT_FEATURE_SUPPORT)
	   }   
	   
	   
	   hps_main是干什么的
   }
   




	按键驱动以及工作流程，input子系统
	{
		按键工作主要包含哪些目录文件
		aw9523b.kl	
		
		
		1.input device和keylayout的绑定在如下文件：
		frameworks/native/libs/input/InputDevice.cpp  ：	appendInputDeviceConfigurationFileRelativePath
		2.事件通过kernel/drivers/input/input.c上报到frameworks/native/services/inputflinger/EventHub.cpp
		  然后在EventHub.cpp来获取按键对应的上层键值，其中的haveKeyLayout就是上面绑定的那个。
		int32_t EventHub::getKeyCodeState(int32_t deviceId, int32_t keyCode) const {
			AutoMutex _l(mLock);

			Device* device = getDeviceLocked(deviceId);
			if (device && !device->isVirtual() && device->keyMap.haveKeyLayout()) {
				Vector<int32_t> scanCodes;
				device->keyMap.keyLayoutMap->findScanCodesForKey(keyCode, &scanCodes);
				if (scanCodes.size() != 0) {
				    uint8_t keyState[sizeof_bit_array(KEY_MAX + 1)];
				    memset(keyState, 0, sizeof(keyState));
				    if (ioctl(device->fd, EVIOCGKEY(sizeof(keyState)), keyState) >= 0) {
				        for (size_t i = 0; i < scanCodes.size(); i++) {
				            int32_t sc = scanCodes.itemAt(i);
				            if (sc >= 0 && sc <= KEY_MAX && test_bit(sc, keyState)) {
				                return AKEY_STATE_DOWN;
				            }
				        }
				        return AKEY_STATE_UP;
				    }
				}
			}
			return AKEY_STATE_UNKNOWN;
		}
		3.上层根据上报的键值进行处理frameworks/base/services/core/java/com/android/server/policy/PhoneWindowManager.java


		例如，aw9523上报了一个253的键值，上报到EventHub.cpp后，通过查找aw9523.kl的映射值：key 253   WWW
		之后，向上层上报了WWW的键值。
		上层需要有WWW这个键值的定义才会被识别：
		gionee/code/alps/public/ROM/frameworks/native/include/android/keycodes.h:801:    AKEYCODE_WWW     = 304,
		gionee/code/alps/public/ROM/frameworks/base/core/java/android/view/KeyEvent.java:837:	public static final int KEYCODE_WWW = 304; 

		在这里处理
		gionee/code/alps/public/ROM/frameworks/base/services/core/java/com/android/server/policy/PhoneWindowManager.java:7077:           
		case KeyEvent.KEYCODE_WWW:

	}


}




/*********************************************************************************************************************/

17G06A
{
	T1-B-119

	几个高通平台稳定性文档，感兴趣的可以下载看。
	80-NM641-1
	80-P7139-1
	80-P7139-3
	80-P7139-5
	80-P7139-6
	80-P7139-7
	80-P7139-8

	
	理清思路，需要涉及到哪些文件
	原理图，芯片的spec，gpio表，dts文件，*.c, *.h 文件
	当然这里移植应该只是涉及几个人的修改
	
	
	系统启动的几个阶段对应的代码
	

	
	这个文件是一个从的dts文件里面包涵了其他需要的dts
	{
		msm8917-pmi8937-qrd-sku5.dtsi
		msm-pmi8937.dtsi
		msm8917-qrd.dtsi
		msm8937-mdss-panels.dtsi
	
	
		这些宏包含哪些文件，哪些代码？
		CONFIG_SMB1351_USB_CHARGER=y
		CONFIG_SMB135X_CHARGER=y
		CONFIG_QPNP_SMBCHARGER=y
		CONFIG_QPNP_FG=y
		CONFIG_BATTERY_BCL=y
		
		CONFIG_QPNP_HAPTIC=y
		CONFIG_BW_MONITOR=y
		CONFIG_MSM_SPMI=y
		CONFIG_MSM_SPMI_PMIC_ARB=y	
			
	}

	
	
	/*debug*/
	{
	
		1.电池电压不够，不插充电器走一小段，插充电器反复重启
		{
			这段代码干什么了？
			[   15.824467] *(1)[333:charger]charger: [15814] shutting down
			[   15.830815] SysRq : Emergency Remount R/O
			[   15.834014] *(1)[21:kworker/1:0]Emergency Remount complete
			[   15.839841] *(1)[333:charger]msm_thermal:msm_thermal_update_freq Freq mitigation task is not initialized
			[   15.854832] *(0)[333:charger]mdss_fb_release_all: try to close unopened fb 1! from pid:333 name:charger
			[   15.866762] *(0)[333:charger]FG: fg_shutdown: FG shutdown started
			[   15.871882] *(0)[333:charger]FG: fg_shutdown: FG shutdown complete
			[   15.878503] *(0)[333:charger]reboot: Power down
			[   15.882523] -(0)[333:charger]Powering off the SoC
	
			msm-poweroff.c，qpnp-fg.c
		
			(msm-poweroff.c) do_msm_poweroff  
		}

		 
		 
		2.无法充电，可能是Battery-type not identified，电池识别不对
		{
			<6>[ 3227.693053] *(0)[8648:kworker/0:1]SMBCHG: smbchg_config_chg_battery_type: Battery-type not identified

			<6>[ 3242.516708] *(2)[316:irq/223-usbin-s]SMBCHG: handle_usb_removal: triggered
			<6>[ 3242.516723] *(2)[316:irq/223-usbin-s]SMBCHG: smbchg_change_usb_supply_type: Type 0: setting mA = 2000
			<6>[ 3242.516873] *(2)[316:irq/223-usbin-s]SMBCHG: smbchg_set_usb_current_max: USB current_ma = 0
			<6>[ 3242.516910] *(2)[316:irq/223-usbin-s]SMBCHG: smbchg_set_usb_current_max: usb type = 5 current set to 0 mA
			<6>[ 3242.516936] *(2)[316:irq/223-usbin-s]SMBCHG: get_parallel_psy: parallel charger not found
			<6>[ 3242.517062] *(2)[316:irq/223-usbin-s]SMBCHG: handle_usb_removal: setting usb psy present = 0
			<6>[ 3242.517147] *(2)[316:irq/223-usbin-s]msm_otg 78db000.usb: Avail curr from USB = 0

			<6>[ 3242.517685] *(2)[11776:kworker/2:0]SMBCHG: read_usb_type: src det low
		 
		}




		
		3.给高通提case，测量电池曲线	 20170807
		{
			battery ID：15k
			NTC：9k
			电池容量：4000mAh
		}





		4.写过IMEI的机器，无法打开USB端口,BUG ID：95605
		{
			初步分析：
		    无法打开USB端口
		    
		    adb shell setprop sys.usb.config  mtp,diag,adb	//903a
		    adb shell setprop sys.usb.config diag,serial_smd,rmnet_ipa,adb
		    adb shell setprop persisit.sys.usb.config diag,serial_smd,rmnet_ipa,adb
		    
			可以加上persisit属性，重启也会生效
			diag 口是用于出modemlog的端口，是USB端口复用
			getprop sys.usb.config 
			mtp,adb
		    
		    <6>[ 2959.562317] -(0)[8233:kworker/0:1]android_usb gadget: high-speed config #1: 86000c8.android_usb
			<6>[ 2959.562347] -(0)[8233:kworker/0:1]diag: USB channel diag connected
			<3>[ 2959.562487] -(0)[8233:kworker/0:1]usb_bam_get_connection_idx: failed for 1
			<7>[ 2959.566679] *(2)[1935:PhotonicModulat]mdss_dsi_panel_bklt_pwm: bklt_ctrl=0 pwm_period=60 pwm_gpio=1021 pwm_lpg_chan=0
			<7>[ 2959.566685] *(2)[1935:PhotonicModulat]mdss_dsi_panel_bklt_pwm: ndx=0 level=40 duty=9
			<3>[ 2959.568329] -(0)[8233:kworker/0:1]usb_bam_get_connection_idx: failed for 1
			<3>[ 2959.575181] -(0)[8233:kworker/0:1]gport_rmnet_connect: usb_bam_get_connection_idx failed
			<7>[ 2959.576693] *(2)[1935:PhotonicModulat]mdss_dsi_panel_bklt_pwm: bklt_ctrl=0 pwm_period=60 pwm_gpio=1021 pwm_lpg_chan=0
			<7>[ 2959.576699] *(2)[1935:PhotonicModulat]mdss_dsi_panel_bklt_pwm: ndx=0 level=44 duty=10
			<3>[ 2959.583257] -(0)[8233:kworker/0:1]gsmd_ctrl_disconnect: gsmd_ctrl_disconnect: Invalid port_num#0
			<3>[ 2959.592018] -(0)[8233:kworker/0:1]frmnet_set_alt(): gport_rmnet_connect fail with err:-22
			<6>[ 2959.600185] -(0)[8233:kworker/0:1]diag: USB channel diag disconnected
			<3>[ 2959.600216] -(0)[8233:kworker/0:1]smd_tiocmset: Invalid channel specified
			<6>[ 2959.607888] *(0)[8233:kworker/0:1]android_work: android_work: sent uevent USB_STATE=CONNECTED
			<6>[ 2959.607941] *(0)[8233:kworker/0:1]FG: get_sram_prop_now: addr 0x594, offset 1 value 215600
			<6>[ 2959.608003] *(0)[8233:kworker/0:1]SMBCHG: smbchg_external_power_changed: usb type = SDP current_limit = 500
			
			
		}

	}
	
}






USB quick start
{
	evaluate 80-P2485-17
	
	define/design 80-P2485-17,80-P2468-5B
	
	build/bringup  80-NF238-1 ,80-NA648-1
	
	customize 80-NF283-1,80-VB717-1,80-V4609-1
	
	fine-tune/verfiy quality/cerfity 80-NF283-1
	
	
	create case for USB issues
	{
		1.You can file cases for the following problem areas:
		• PA1 – BSP/HLOS
		• PA2 – Drivers: USB/HSIC

		
		2.Remember to provide necessary information
		• Dump and symbol files (vmlinux and ELF files)
		matching to the dump
		• Test scenario, frequency of the issues,
		reproducibility
		• Software versions when the issue first reported
		• USB eye diagram for compliance issues
		• USB scopeshots for electrical issues
		• USB analyzer log for data stalls, protocol issues,
		and throughput issues

	}
	

	
	
	power management(80-P2468-5B)
	{
		常用缩写
		{
			OVP:overvoltage protection  过压保护
		
			APSD:Automatic power source detection 自动电源检测
		
			OTP：One-time programmable 一次性编程
		
			Two-line serial power management interface (MIPI SPMI)

		}
	
		USB input
		{
		
		}
	

	}
	
	
	
	
	
	
	
	
	
	
	
	
	linux_usb_implementation_guide
	{
	  常用缩写
	  {	
	  	HCD:host controller driver
		
		DCD:device controller driver

	  
	  }
	
	
	  software architectural
	  {
	  	1.Google initially developed a new framework called the "function framework", as the gadget framework in the Linux kernel was not supporting composite devices. 
		2.EHCI
		增强型主机控制器接口规范描述了一个通用串行总线（USB）2.0版的主机控制器的寄存器级接口
	  	3.OTG

	  }		
	  
	  devices tree
	  {
	  		  adress range  address start
	  	reg = <0xf9200000 0xfc000>,
		<0xfd4ab000 0x4>;
		#address-cells = <1>;
		#size-cells = <1>;
		ranges;

	  	the USB device could have several dedicated interrupts
in the Global Interrupt Controller (GIC), in addition to GPIO interrupts, etc.
 		GIC 中断管理子系统
 		
 		interrupt-parent = <&usb3>;
		interrupts = <0 1>;
		#interrupt-cells = <1>;
		interrupt-map-mask = <0x0 0xffffffff>;
							   注册到具体的中断控制器	   注册到系统的中断				
		interrupt-map = <0x0 0 &intc 0 					133 0
			  注册到具体的中断控制器					注册到系统的中断
		0x0 1 &spmi_bus 0x0 0x0 					0x9 0x0>;
		interrupt-names = "hs_phy_irq", "pmic_id_irq";

		

		
		
		ci13xxx_msm_probe() [ci13xxx_msm.c]
		Maps the USB registers from physical to kernel address space
		Calls udc_probe() [ci13xxx_udc.c]
		
		Allocates and initializes a struct ci13xxx device assigned to _udc

		Initializes structure variables
		Assigns callbacks for the USB gadget driver usb_gadget_ops
		Initializes driver hardware bank register values
		
		Saves an offsetted base address for different register I/O APIs, i.e., hw_aread(),
		hw_cread()
		Calls otg_set_peripheral()
		
	  }
	
	}
	
	
	

	
}



pmic
{
	Based on these selections, the following information can be used to help resolve common issues in this particular area:

	To download any document directly from this solution, first login to the CreatePoint and then click on the hyperlink listed against the relevant document below.

	80-P2485-18 : MSM8937 System Drivers PMIC Overview
	80-P2485-2 : MSM8937_Linux_Android_PMIC_SW_Drivers_Overview
	80-NG006-1 : System Driver PMIC NPA Overview
	80-NN255-1 : RPM PMIC SW Driver Overview
	80-NV610-47 : LA VREG CLK SW User Guide

	For a complete list of PMIC Software documents and Knowledge base solutions for all technology areas please refer to the following master documents:

	80-NR097-1 : PMIC Software Master Document
	80-NR097-2 : PMIC Software KB Solution Master Document

}










高通项目的bring up
{
	1.相关的工作流程，代码框架
	
	2.项目脚本，涉及到哪些宏
	
	3.gpio，device tree,rpm ,ldo的配置
	
	4.充电的工作流程，充电器的检测，数据的上报
	
	5.电量计相关的计算
	
	6.ti的替换方案
	

}









1.msm8917
{
	
	/*电量计*/
	{
		电量计的算法依赖于库仑计，以电压为基础的技术
		确保短期的线性变化(也就是说电量短时间变化不能太突然)
		同时长时间的精确性

		不需要一个完整的充放电循环来校准电量计的准确度

		开路电压计算初始值，库仑计的积分算法计算电量的变化	

		Disable USB on-the-go (OTG) functionality when battery SoC falls below a 		 programmable  threshold

		电量计的电流通过读取10m欧的采样电阻，读取充进电池的电量多少，电压通过读取adc转换的
		引脚的值，1500ms读取一次
	}


	/*相关概念*/
	{
	
		BAT_ID:
		通过BAT_ID检测电池的存在与否，这个检测过程内嵌在fg算法中，重复的升高基础电流（5uA->15uA->150uA）直到发现匹配的
		智能电池调控通常是提供5uA的灌电流去识别，因为5uA可以通过软件去识别然后设置使能标志位
		(Battery Serial Interface (BSI) module)
			电量计读电流通过采样电阻，读电池电压通过连接电池正负极的BATT_P，BATT_N
	放电读正极，其他时候读负极


		thermal：		
			电池温度检测，有些是将温度检测的电阻嵌入到电池内(NTC电阻)，然后通过一个thermal pin去连接，读取数值	
			thermal 相关参数的设置都是一些十六进制，总要说说是什么意思吧？pm_config_target.c
			这个文件在哪，这个文件在modem侧
			modem_qc_n_qrd8920_cs/BOOT.BF.3.3/boot_images/core/systemdrivers/pmic/config/mdm9x45/pm_config_target.c
	
		配置电量计中thermistor相关的参数(80-VT310-123).
		qcom,thermal-coefficients这是一个位数组，通过这组数据，可以读NTC电阻的一些转换信息
		{
			这个有两个地方可以setting
			一个是在kernel device tree里面
			一个是在SBL里面，但是这里要晚1.5s因为要启动SRAM，在pm_config_target.c
			里面设置
	
		}


		ESR(电池等效电阻):
			ESR可以提高电量计的准确性，对其他的也有影响，但是ESR在低电流的状态下是不需要的，而且在系统suspend的时候是无法使能的
			电池等效串联电阻ESR，这个值最好是实时的，则样才最准确等效电阻跟temperature有很大关系，同时也影响到电池的剩余电量
		power_supply 子系统是这个POWER_SUPPLY_PROP_RESISTANCE而这个数据的采集是在电量计每隔90s发一个脉冲，然后同步同一时间的
		电压和电流而这个脉冲会引起设备底电流的升高，所以当设备suspend的时候会关闭这个使能



		System termination current(系统的截止电流)：
			可以客制化电池100%状态的上报，
			qcom,fg-chg-iterm-ma这个是设置充电截止电流
			这里应该讲的是系统满电的状态，充电电流，电池电压
			显示100%，系统截止电流，充电截止电流


		FG termination current（这个使用电量计充电截止电流来判断停止充电的条件）：
		也就是说充电截止电流可以通过qcom,fg-chg-iterm-ma，或者qcom,chg-term-ua(不用)来设定
		充电截止的条件
			qcom,fg-iterm-ma = <150>; //这个是显示100%的截止电流？

			电量计的截止电流
			qcom,fg-chg-iterm-ma = <100>;	//这个是充电截止电流？


		System cutoff voltage：系统0%电量对应的电压
		qcom, fg-cutoff-voltage-mv = <3000>; /*example 3V */


		Estimate voltage at bootup(启动阶段估计电压)：
			Reload the profile for a better SoC estimate if the vbat_current_predicted differs from the actual vbat by more than a set threshold.
		如果启动阶段检测到的电压跟实际的电压偏差超过多少就重新载入数据


		CC_to_CV threshold set point
			恒流到恒压模式转变的阈值：
			qcom, fg-cc-cv-threshold-mv = <4340>;
			当然这里还有一个浮动电压，比上面的阈值高10V
			Specify the CC_CV set point for PMI895x FG to 4340 mV (currently default), which is 10 mV lower than the float voltage configured (currently default is 4350 mV). This is needed for EoC to be notified properly.


		Resume charging based on SoC
			qcom,resume-soc = <95>;
		当电量低于多少的时候，触发回充，recharging


		External/internal current sense
			这个应该是并行充电要添加的
			qcom,ext-sense-type


		IRQ_volt_empty：
			低电中断至少要比system cutoff voltage高50mV
			Recommended setting/default = System Cutoff voltage – 50 mV (may need more headroom than 50 mV value, depending on the system)
			因为上层读到0%的时候会立刻执行shutdown流程，避免电压低于UVLO
			irq-volt-empty-mv


		Battery age detection：
			1.通过等效电阻来估计电池的容量
			2.软件上通过库仑计来学习电池的容量


			The Battery Learning Capacity algorithm takes into account the following:
			1.Temperature
			2.Qualified starting point of the SoC of the battery
			3.Allowable increment and decrement of each charge cycle to qualify for each learning cycle
			这里应该是每次充满电循环所需要的时间，一次来估计电池的容量，和电池老化


			Charge cycle count
			当电量低于low-soc的时候触发算法，电量高于high-soc的时候counter增加，只有走一个完整的过程counter才会增加，如果移除电池的话数据会被清除，因为数据存储在FG SRAM

	}
	
	
	
	调用流程
	{
        (qpnp-smbcharger.c) 这个是高通充电用的主要的源代码，smbchg_init初始化模块 -> spmi_driver_register注册设备驱动 -> smbchg_probe将设备驱动跟设备挂钩，
        
		建立bind -> 从dtsi文件中获取一些参数 ->  create_votable创建了好几个投票变量 (fcc_votable,usb_icl_votable,dc_icl_votable,usb_suspend_votable,

		dc_suspend_votable,battchg_suspend_votable,hw_aicl_rerun_disable_votable,hw_aicl_rerun_enable_indirect_votable,aicl_deglitch_short_votable,hvdcp_enable_votable)
	
	
	
		高通的充电基本上都叫smbcharger
		(qpnp-smbcharger.c) smbchg_init 模块初始化->smbchg_probe	从dtsi文件中获取参数，上面还有一个投票制度貌似很重要，还有一些工作 -> smbchg_usb_update_online_work,
		
		判断usb是否插入,并上报状态到power_supply子系统 -> smbchg_parallel_usb_en_work并行充电使能，对并行充电的条件进行判断 aicl是否稳定，没有电池的条件下不允许改变
		
		并行充电的状态 -> smbchg_parallel_usb_enable -> smbchg_vfloat_adjust_work动态充电电压调节
	
	}
	
	
	
	/*debug*/
	{
	
		qpnp-smbcharger.c
	    {   
	        
	        smbcharger里面涉及到的投票变量都是干什么的
	        {
	            1.fcc_votable:这个是设置快充充电电流的，根据投票结果决定并行充电的充电电流是多少
	               并行充电好像跟taper_irq_en这个中断有关，但是这个中断是干什么的？

	               static int set_fastchg_current_vote_cb(struct device *dev,
						int fcc_ma,
						int client,
						int last_fcc_ma,
						int last_client)
	                {
	                    struct smbchg_chip *chip = dev_get_drvdata(dev);
	                    int rc;

	                    if (chip->parallel.current_max_ma == 0) {
	                        /*设置快速充电的充电电流 fcc_ma*/
	                        rc = smbchg_set_fastchg_current_raw(chip, fcc_ma);
	                        if (rc < 0) {
	                            pr_err("Can't set FCC fcc_ma=%d rc=%d\n", fcc_ma, rc);
	                            return rc;
	                        }
	                    }
	                    /*
	                    * check if parallel charging can be enabled, and if enabled,
	                    * distribute the fcc
	                    */
	                    /*高通的快充利用的是并行充电方案，主从charger*/
	                    smbchg_parallel_usb_check_ok(chip);
	                    return 0;
	                }

	            
	            2.usb_icl_votable：
	                设置usb充电电流的限制，可能要根据系统温升的情况决定
	                如果是快充充电器的会停止并行充电
	                static int set_usb_current_limit_vote_cb(struct device *dev,
						int icl_ma,
						int client,
						int last_icl_ma,
						int last_client)    
	                {
	                    struct smbchg_chip *chip = dev_get_drvdata(dev);
	                    int rc, aicl_ma, effective_id;

	                    effective_id = get_effective_client_id_locked(chip->usb_icl_votable);

	                    /* disable parallel charging if HVDCP is voting for 300mA */
	                    if (effective_id == HVDCP_ICL_VOTER)
	                        smbchg_parallel_usb_disable(chip);

	                    if (chip->parallel.current_max_ma == 0) {
	                        rc = smbchg_set_usb_current_max(chip, icl_ma);
	                        if (rc) {
	                            pr_err("Failed to set usb current max: %d\n", rc);
	                            return rc;
	                        }
	                    }

	                    /* skip the aicl rerun if hvdcp icl voter is active */
	                    if (effective_id == HVDCP_ICL_VOTER)
	                        return 0;

	                    aicl_ma = smbchg_get_aicl_level_ma(chip);
	                    if (icl_ma > aicl_ma)
	                        smbchg_rerun_aicl(chip);
	                    smbchg_parallel_usb_check_ok(chip);
	                    return 0;
	                }


	            3.dc_icl_votable
	            /*
	            * set the dc charge path's maximum allowed current draw
	            * that may be limited by the system's thermal level
	            */
	            设置直流充电器最大的充电电流，遍历数组寻找最匹配的值，写进寄存器
	            static int set_dc_current_limit_vote_cb(struct device *dev,
	                                    int icl_ma,
	                                    int client,
	                                    int last_icl_ma,
	                                    int last_client)
	            {
	                struct smbchg_chip *chip = dev_get_drvdata(dev);

	                return smbchg_set_dc_current_max(chip, icl_ma);
	            }                    

	            下面这几个大部分都是操作寄存器
	            4.usb_suspend_votable       将usb挂起
	            5.dc_suspend_votable        dc充电挂起
	            6.battchg_suspend_votable   关闭充电使能

	            7.hw_aicl_rerun_disable_votable     关闭rerun高电压aicl策略
	            8.smbchg_aicl_deglitch_config_cb    配置aicl去抖
	            9.hvdcp_enable_votable              允许高电压充电
	        }

	        smbcharger里面的几个工作函数
	        {
	            1.smbchg_usb_update_online_work 
	              检测usb的状态然后上报给power_supply子系统
	            2.smbchg_parallel_usb_en_work
	              设置并行充电使能
	            3.smbchg_vfloat_adjust_work
	              根据充电能路，目标充电电压，不断步进升压
	            4.smbchg_hvdcp_det_work
	              高压充电器j      
	        }
	    }
	

		qpnp-fg.c
        {
        	为什么高通的单个结构体都有那么多成员，还有那么多工作函数？
        	
        	
			fg_probe将电量计device跟driver挂钩,建立bind
			
			log_bat_status这个打印很多电池相关的信息
			都要从get_sram_prop_now这个接口里面去根据下标去读取数据
			fg_mem_data_index
			
			
			常用函数接口：
			充电使能
					rc = set_prop_enable_charging(chip, true);
			
			
			开始的时候初始化很多wakeup source，创建了很多work
			{
				wakeup source:需要了解机制
				
				work：
					update_jeita_setting 这是跟一个温度设置相关的
					
					update_sram_data_work 这个跟fg相关参数存储在sram中相关
						然后有的数据上报都是从sram从读取数组数据，但是这些数据是在哪写的
						利用哪些接口获取的
						
					update_temp_data 上报电池温度相关信息
					
					check_empty_work 这个是上报没电 0%？如果有vbatt-low interrupt这个  中						断触发就会调用这个函数
					
					batt_profile_init 电池相关的配置文件初始化
						
					check_sanity_work
					
			}
        }
	}
	
	
	
	
	/*调试方式*/
	{
		FG MEM_INTF access	
			adb shell "echo 0xXXX > /sys/kernel/debug/fg_memif/address"
			adb shell "echo 0xXX > /sys/kernel/debug/fg_memif/count"
			adb shell "cat /sys/kernel/debug/fg_memif/data"

		FG SRAM dump
			1.需要dumper.sh

			2.adb 命令
				adb root
				adb wait-for-devices
				adb push dumper.sh  /data/
				adb shell chmod 777 /data/dumper.sh

			3.运行脚本，将数据写到文件内
			好像不管用
			/data/dumper.sh > /data/kmsg.txt

			4.adb root
			  adb wait-for-devices
			  adb pull /data/kmsg.txt
			  
		FG debug logging
			调整qpnp-fg.c的log等级
	
			echo 0xff > /sys/module/qpnp_fg/parameters/debug_mask
			echo 8 > /proc/sys/kernel/printk
			dmesg > debug_output_filename
	}
	
	
	/*充电涉及到的代码和参考*/
	{
		./msm-3.18/drivers/power/qpnp-fg.c
		./msm-3.18/arch/arm/boot/dts/qcom/msm-pmi895x.dtsi
		./msm-3.18/Documentation/devicetree/bindings/power/qpnp-fg.txt
		
		
		 dtsi配置文件
	        ./arch/arm/boot/dts/qcom/msm-pmi8940.dtsi

        kernel代码
        ./msm-3.18/drivers/power/qpnp-smbcharger.c
        ./msm-3.18/drivers/power/pmic-voter.c  
	}
	
}


ti的替换方案
{

    1.bq27428
    {
    	The bq27426 battery fuel gauge uses the patented Impedance TrackTM algorithm for fuel gauging
    	bq27428利用的是阻抗追踪法来进行电量计算，提供相关的电池信息


		相关参数的精确度(温度范围是-40~100)： 
		电压 -0.1% ~ -0.2%
		温度 -13%  ~ 13%    这个偏差很大
		电流 0 ~ 0.6%
    
    	电量计通过读取流过采样电阻的电压来估计充电和放电时候的电池信息
    	
    	当电池接上了fuelgauge的时候。电池的阻抗是根据流进电池的电流，电池的开路电压
    	以及有负载情况下的电池电压
    	
    	P10 I2C通信要注意的地方
    	
    	fuelgauge提供四种模式：initilization,normal,sleep,shutdown
    
    	这里面应该要注意的一个是I2C通信，还有电量计的一些内容
    	
    }

}    







/*测试上的bug*/
{

	
	
	

}










/*17G10A当前存在的问题*/
{
	充电时序的关机充电和放电状态			
	{
		放电的1%位置还是有问题
		1%开始03：00 vbat=3.62V
		  结束03：52 vbat=3.3V

		1%可能相当于4%左右的电量，而且时间很长  
		1%电压还是很高
	}

	底层电量跟上层电量差别很大，但是这个没有开机和刚开是的充电的log		


	马达刷机第一次震动太弱，之后震动正常？	


	键盘按键有问题，mmi测试不通过，测键不管用


	mmi测试的调用节点好像还有问题
	{
		mmi测试读取的节点信息
		sys/class/power_supply/battery/

		充电可能读取的节点数据有问题
			几个电流，电压，电池电量，电池电压都有问题
			充电电流上不去
			{
				这个好像是主板温度过高，虽然显示的是电池温度50+，但是电池明显不热
				[    9.896490] <6>.(4)[258:charger_thread][name:mtk_charger&]Vbat=3775,I=-4446,VChr=9,T=49,Soc=0:0,CT:0:0

				[   12.371394] <6>.(6)[258:charger_thread][name:mtk_battery_hal&][fgauge_read_current] final current=5051 (ratio=950)
				[   12.372785] <6>.(6)[258:charger_thread][name:mtk_charger&]Vbat=3771,I=-5051,VChr=9,T=50,Soc=0:0,CT:0:0
				[   12.375494] <4>.(4)[258:charger_thread][name:pmic_auxadc&][mt6355_get_auxadc_value] ch = 2, reg_val = 0x1d3, adc_result = 410
				[   12.377141] <4>.(4)[258:charger_thread][name:mtk_battery_hal&][fgauge_read_current] final current=4735 (ratio=950)
				[   12.378491] [name:mtk_charger&][BATTERY] Battery over Temperature or NTC fail 50 50!!

				这个是电池的pin脚有问题
			}

		键盘mmi测试不能通过
			键盘的工作，以及涉及哪些目录下的文件
	}

        
	计步器，sd卡相关内容？
	{
		set_counter.c
		sd.c
	}


    从满电放电到关机的情况是否有异常


    计步器，sd卡相关内容？
    {
        set_counter.c
        sd.c
    }


	快充升压问题


	去掉OTG中断
}























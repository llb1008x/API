

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
	
	

	
	G1605A电量跳变问题
	{
	
		87038，86321从1%充电10分钟充电到4%,再关机充电5分钟左右电量变为52%
	
		底层电量跟上层显示差别较大，然后触发电量跳变问题	
		mt_battery_meter.h
		已提交eservices长时间未解决,case ID:ALPS03373196
		
		G1605A-T0114-170310AB
		gitk  --author=liteng   2017.3.15   73194 P80,2017.5.22 84712 P96
		
		{
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
		
		
		
		FGADC_D0
		
		有变化的地方
		{
		
			//Gionee <gn_by_charging> <lilubao> <20170726> add for debug battery status begin

			#ifndef DIFFERENCE_VBAT_RTC
			#define DIFFERENCE_VBAT_RTC 10
			#endif

			#ifndef DIFFERENCE_SWOCV_RTC_POS
			#define DIFFERENCE_SWOCV_RTC_POS 15
			#endif
			
			#define TRK_POINT_EN 0
			#define TRK_POINT_THR 5
			
			多了一个函数接口
			BATTERY_METER_CTRL_CMD->BATTERY_METER_CMD_GET_IS_HW_OCV_READY
			signed int pmic_is_battery_plugout(void);
			
			#ifndef MAX_SMOOTH_TIME
			#define MAX_SMOOTH_TIME 1800
			#endif
			
			batt_meter_cust_data.difference_vbat_rtc = DIFFERENCE_VBAT_RTC;
			batt_meter_cust_data.difference_swocv_rtc_pos = DIFFERENCE_SWOCV_RTC_POS;
			
			batt_meter_cust_data.max_smooth_time = MAX_SMOOTH_TIME;
			batt_meter_cust_data.trk_point_en = TRK_POINT_EN;
			batt_meter_cust_data.trk_point_thr = TRK_POINT_THR;
			
			？？mt_battery_set_init_vol(gFG_voltage_init);
			
			fgauge_algo_run_get_init_data();
			每次wakeup的时候移到外面了
			
			static ssize_t show_FG_drv_force25c(struct device *dev, struct device_attribute *attr, char *buf)
			创建一个新的节点
			FG_BAT_INT		FG_BAT_INT_OLD
			
			？？battery_meter_set_fg_int <- fg_bat_int_handler
			
			/* read HW ocv ready bit here, daemon resume flow will get it later */
			battery_meter_ctrl(BATTERY_METER_CMD_GET_IS_HW_OCV_READY, &is_hwocv_update);
			
			mt_battery_set_init_vol
			
			#ifdef USING_SMOOTH_UI_SOC2
				battery_meter_smooth_uisoc2();
			#endif
			
			？？INIT_BAT_CUR_FROM_PTIM
			
			extern int do_ptim_ex(bool isSuspend, unsigned int *bat, signed int *cur);
			
			这是判断关机电压
			if (previous_SOC == -1 && bat_vol <= SYSTEM_OFF_VOLTAGE) {	//Gionee GuoJianqiu 20160217 modify for GNSPR4723
			previous_SOC = 0;
				if (ZCV != 0) {
					battery_log(BAT_LOG_CRTI,
							"battery voltage too low, use ZCV to init average data.\n");
					BMT_status.bat_vol =
						mt_battery_average_method(BATTERY_AVG_VOLT, &batteryVoltageBuffer[0],
									  ZCV, &bat_sum, batteryIndex);
				} else {
					battery_log(BAT_LOG_CRTI,
							"battery voltage too low, use V_0PERCENT_TRACKING + 100 to init average data.\n");
					BMT_status.bat_vol =
						mt_battery_average_method(BATTERY_AVG_VOLT, &batteryVoltageBuffer[0],
									  V_0PERCENT_TRACKING + 100, &bat_sum,
									  batteryIndex);
				}
			} else {
				BMT_status.bat_vol =
					mt_battery_average_method(BATTERY_AVG_VOLT, &batteryVoltageBuffer[0], bat_vol,
								  &bat_sum, batteryIndex);
		   }
		}
	}







/********************************************************************************************************************************/

17G10A p1试产待解决问题
{
	
		
	电池曲线的导入		7.27
	{
		mtk_battery_table.h  		fg_profile_t0
	
		mt6757.dtsi文件也有类似的 	battery_profile_t0
		
		mtk还有两个patch要合入
	
		电量计系数的测量
	}


	USB  pid，vid添加到驱动中
	{
	   		gionee_usb_uid_pid 
	   		不同功能对应不同的pid
	   
	}
	
	
	



	
	整机测试
	{
		2PCS手机因电池温度过高关机，但环y箱里设定温度为57度，要求做高温高湿测试时手机可以有高温安全警告，但不允许执行关机机制； 
	
		低温测试，-20度，手机自动关机
	
	}
	

	
	
	底电流偏高
	{
		这里主要是android系统的睡眠唤醒机制，然后为了便于分析功耗问题
		
	
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
	
	


	
	过温测试


	上层显示的电池容量有问题 3450mAh，但是实际的是3000mAh
	改变电池容量之后，电流有提高但要知道原因

	
	USB眼图：NG
	
	
	
	
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

	
    fuelgauge 3.0的问题,文档+邮件
    {
    	pacth申请 已申请
		ALPS03411143 
		Fix gauge coulomb value may abnormal in some mode / 修復待機時電量計算可能異常issue 

		ALPS03158638   
		【L3500-Charger】voltage mode在低温下的修正
		
    }
    
}




/*********************************************************************************************************************/

17G06A
{

	

	理清思路，需要涉及到哪些文件
	原理图，芯片的spec，gpio表，dts文件，*.c, *.h 文件
	当然这里移植应该只是涉及几个人的修改
	
	
	系统启动的几个阶段对应的代码
	
	
	解BUG
	
	
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
		
		
		
		
		几个高通平台稳定性文档，感兴趣的可以下载看。

			80-NM641-1
			80-P7139-1
			80-P7139-3
			80-P7139-5
			80-P7139-6
			80-P7139-7
			80-P7139-8


	}

	
	
	/*debug*/
	{
	
		1.电池电压不够，不插充电器走一小段，插充电器反复重启
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


	电池曲线的导入
}























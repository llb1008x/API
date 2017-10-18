
17G10A log关键字
{
	fg_drv_update_hw_status  电量计上报一些硬件信息
	
	[FGADC_intr_end][FG_INTR_IAVG]
	read_fg_hw_info
	
	FG_DAEMON_CMD_GET_VBAT
	FG_DAEMON_CMD_GET

	控制fuel_gauge的log级
	FG_daemon_log_level 	

	rtc记录的电量
	BATTERY_METER_CMD_SET_RTC_UI_SOC	
	获取上层电量
	FG_status.ui_soc=battery_get_bat_uisoc

	可以通过检索Call trace，察看内存堆栈函数的调用


	ibus，ibat可以检索相关的电流电压信息，这写都在一块

	[248:charger_thread][name:mtk_charger&]Vbat=4321,I=3597,VChr=492,T=33,Soc100,CT:0:1

	healthd线程检测电池的状态
	healthd]: healthd: battery l=5 v=3796 t=41.0 h=2 st=4 chg=	

	获取lte rspr信号强弱	
	GnGetLteLevel(), rsrpIconLevel =1, standard LTE, mLteRsrp =-116,isCampOnLte =true
	
}






17G06A log关键字
{
	src_detect_handler	，handle_usb_insertion	充电检测
	
	Enable USB ID pin 
	
	FG: log_bat_status
	
	
	关闭sensors
	2. Please follow below steps to disable sensor to test again.
	#1
	LINUX\android\device\qcom\common\rootdir\etc\init.qcom.sh
	...
	//start_sensor
	#2.
	adsp_proc\qdsp6\main\src\main.c
	#if defined(SIM)
	volatile boolean RunSensors = FALSE;
	#else
	volatile boolean RunSensors = TRUE; // Change to FALSE from TRUE
	#endif



	缩写	
	RPM:Resource Power Manager
	PBL:Primary Boot Loader	
	SBL:Secondary Boot Loader
	RMR:Reset Management Register
	SDI:系统调试镜像
	MBA：modem boot authenticator
	FLCB：perform fast low current boot
	APSD：performs automatic power source detection
	MPSS:modem processor subsystem
	APSS:Applications processor subsystem
}












/*********************************************************************************************************************************/
17G10A 
{

	#105124
	卡1移动4G，电量耗尽，标配旅充2A充电》第四晚待机，电量耗尽-第二天早08：30关机充电-两台机同时充电，15min后测试机电量1%，对比机电量15%-18min后测试机电量4%，对比机电量18%-开机充电-26min后测试机电量11%，对比机电量26%》再次操作恢复 对比S10无此现象 
	


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
		

	#103678	接听或者挂断电话时，振动的同时会有崩的一声。
	{
		降低震动强度或者震动时间
		//Gionee <GN_BSP_CHG> <lilubao> <20170829> modify for vibrator begin
		
		还有一个问题是开机振动偏弱
		
		首先要知道是调用的过程，然后是如何减小振动的强度？
	
	
		pDrv2604Platdata->GpioTrigger=0;
		pDrv2604Platdata->loop=CLOSE_LOOP;
		pDrv2604Platdata->RTPFormat=Signed;
		pDrv2604Platdata->BIDIRInput=BiDirectional;
			
	}
	
	
		
	#99348
	手机连接电脑USB打开传文件和临时USB调试后，电脑不显示连接到手机，再次插拔USB不恢复，插其它手机可以正常识别，重启恢复
	{
		这个问题是连接到PC没有显示盘符，重启后恢复，08：03~08：05
	
		MTK case ID：ALPS03469265
	}
	


	
	#功耗相关的case还没解决
	{
			ALPS03466994 【GIONEEBJ】【待机功耗】【联通卡不开数据，不开wifi，平均电流高】

			ALPS03467151 【GIONEEBJ】【待机功耗】【不插sim卡，开wifi，待机平均电流高】 

			ALPS03467419 【GIONEEBJ】【待机功耗】【不插sim卡，不开wifi，飞行模式下待机平均电流高】
	
	}

	#关机充电的logo
	关机充电logo分几档，0~10,10~35,35~70,70~99,100，如果二十几是这种应该正常


	#电量显示有问题	
	常温 充电到5%、15%、30%、50%时，电池电量与ZCV曲线对应电量百分比差值大于5%


	#USB充电充电电流频繁跳变，不稳定。USB是2.0的


	#充电到100%离截止时间太长，充到100%时间应该延长点

}












/********************************************************************************************************************************/
17G06A
{

	高通平台显示效果qdcm工具安装以及调试具体参数相关视频和文档
	VD80-NJ550-10SC
	VD80-NJ550-11SC
	VD80-NJ550-12SC
	VD80-NJ550-13SC
	VD80-NJ550-14SC
	80-NJ550-2 W
	80-NJ550-1 V

}




































/********************************************************************************************************************************/
其他问题
{

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
	   	
			pmic_throttling_dlpt  其实是想弄清楚低电保护的策略，以及做了什么事

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
		   
	   }


	{
		马达刷机第一次震动太弱，之后震动正常？	


		键盘按键有问题，mmi测试不通过，测键不管用

	 
		计步器，sd卡相关内容？
		{
			set_counter.c
			sd.c
		}



		计步器，sd卡相关内容？
		{
		    set_counter.c
		    sd.c
		}


		alert diaglog

		mtk game detection service


		这函数mt_battery_average_method,battery_common_fg_20.c做的什么操作？			//20170818	

		boot_reason type  中的kernel panic	BR_KERNEL_PANIC,//这个是内核错误，不知道下步该怎么走 http://blog.csdn.net/liukuan73/article/details/45537889

		cood_boot,hot_mode	

		hps_main是干什么的


		功耗解析相关log需要配置的环境	
		1.先到网站http://strawberryperl.com/下载perl解析环境.
		2.讲main log放到文件夹里面，点击parse，会生成datalink_apk.log
		主要原理是统计Posix_connect Debug里面的包名.

	}

}
















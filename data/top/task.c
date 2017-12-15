
17G10A log关键字
{
	fg_drv_update_hw_status  电量计上报一些硬件信息
	[fg_drv_update_hw_status] current:18726 19824 state:1 1 car:164 217 bat:3888 3926 chr:4891 4901 hwocv:1234 1234 bat_plug_out:1 1 tmp:31 32 imix 16550 rac 448
	car是库伦计变化 coulomb count 相当于实际电量除10
	
	[FGADC_intr_end][FG_INTR_COULOMB_C]soc:10000 fg_c_soc:10018 fg_v_soc:10000 ui_soc:10000 vc_diff:18 vc_mode 0 VBAT 44030 T:[29 V 29 C 34] D0_C -30 D0_V 3 CAR_V 28826 Q:[28835 28835] aging 10000 bat_cycle 0 Trk[0:0:0] UI[1:0] Chr[1:10000:0] pseudo1 0  DC_ratio 100!
	Q:[28835 28835] 是最大电量 同样要除以10
	
	最好直接搜MTK_FG: [FGADC_intr_end]:
	
	GM3.0相关的log
	[FGADC_intr_end]		
	MTK_FG: [dod_init] swocv									初始化
	FG_INTR_COULOMB_LT											
	MTK_FG: [FGADC_intr_end]: 		
	dod_init_result
	
	FG_INTR_FG_ZCV
	
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
	
	
	插入充电器
	mtk_charger_int_handler，dump_charger_name，mt_charger_set_property
	rt5081_enable_chgdet_flow
	
	系统初始化的电压电量
	dod_init_result
	
	
	老化测试
	05-01 09:44:57.365345  3321  3321 I oldtest.TAG: AgingTestMainActivity=>onActivityResult:requestCode=1;resultCode=0
	
}






17G06A log关键字
{
	src_detect_handler	，handle_usb_insertion	充电检测
	
	Enable USB ID pin 
	
	FG: log_bat_status
	
	usbid_change_handler		插入otg检测，usbid引脚变化会产生这个中断
	
	MediaScannerReceiver		扫描u盘文件

	MediaScannerReceiver		扫描多媒体文件
	MediaProvider,FileManager_IntentBuilder
	INTERNAL_VOLUME，EXTERNAL_VOLUME内置盘符，外置盘符

	VIB_DRV						马达相关的log

	acquireWakeLockInternal 获取wakelock
}




17G08A
{
	fg_charge_full_update
	FG: 
	PMI:
	
	
	
	
	
	
	缩写	
	{
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
	
		VINTF Vendor Interface
		HIDL HAL Interface Definition Language
		VTS Vendor Test Suite
		NDK Native Development Kit
		VNDK Vendor NDK
		LKM Loadable Kernel Modules
		ABI Application Binary Interface
		
		电量计的配置在：UEFI，XBL,HLOS
		UEFI：unified extensible firmware interface
		全称“统一的可扩展固件接口”(Unified Extensible Firmware Interface)， 是一种详细描述类型接口的标准。
		这种接口用于操作系统自动从预启动的操作环境，加载到一种操作系统上。
	}
	
	extcon 插拔检测

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
	Dear customer,
	这题贵司需要解决的问题：电量小于50%时，扣掉电池静置30min后，测量电池电压查ZCV table得出的DOD0，和将电池放回到手机后手机显示的D0相差较大，认为这个电量显示不准确。

	针对这个问题，可以认为为D0 init误差过大，可以开启daemon log，复现抓取Log，搜索关键字dod_init_result，对比下ui_soc的值取得hwocv/swocv/rtc中的哪个，理论上你的上述操作应该取hwocv才对。


	#USB充电充电电流频繁跳变，不稳定。USB是2.0的


	#充电到100%离截止时间太长，充到100%时间应该延长点

}











{

android o:
	ANDROID TREBLE OVERVIEW                                          80-PE644-1
	ANDROID TREBLE VNDK OVERVIEW                                     80-PE644-2
	ANDROID TREBLE HIDL OVERVIEW                                     80-PE644-3
	ANDROID TREBLE KERNEL OVERVIEW                                   80-PE644-4
	ANDROID TREBLE VTS OVERVIEW                                      80-PE644-5
	VERIFIED BOOT 2.0 INTRODUCTION                                   80-PE644-6
	SELINUX OVERVIEW AND UPDATE FOR ANDROID O        				 80-PE644-7
	
	{
		 1.Kernel modularization
		 2.HAL re-architecture (VINTF/HAL re-arch to make it as versioned & standardized APIs to Framework modules).
		 3.Vendor/OEM module separation from system image
	
	}
	
	
	
	
	Bus 003 Device 021: ID 058f:6387 Alcor Micro Corp. Flash Drive


pmic:	
	GNSPR#122265
	To download any document directly from this solution, first login to the CreatePoint and then click on the hyperlink listed against the relevant document below.

	80-P2485-18 : MSM8937 System Drivers PMIC Overview
	80-P2485-2 : MSM8937_Linux_Android_PMIC_SW_Drivers_Overview
	80-NV610-48 : PMIC GPIO and MPP Software Configuration

	For a complete list of PMIC Software documents and Knowledge base solutions for all technology areas please refer to the following master documents:

	80-NR097-1 : PMIC Software Master Document
	80-NR097-2 : PMIC Software KB Solution Master Document


	To download any document directly from this solution, first login to the CreatePoint and then click on the hyperlink listed against the relevant document below.

	80-P2485-18 : MSM8937 System Drivers PMIC Overview
	80-P2485-2 : MSM8937_Linux_Android_PMIC_SW_Drivers_Overview
	80-NV610-43 : System Drivers PMIC Dead Battery Charging Overview
	80-NV610-44 : MSM8937.LA Charger SW User Guide

	For a complete list of PMIC Software documents and Knowledge base solutions for all technology areas please refer to the following master documents:

	80-NR097-1 : PMIC Software Master Document
	80-NR097-2 : PMIC Software KB Solution Master Document


	80-NL708-1		dump pimc register	
	
	
	decidegc
	
	NTC温度电压转换函数，看看过程什么样的
	force_get_tbat_internal 温度转换函数



	
	SDM660  pmic
	{
		
		
		相关的宏
		{
			CONFIG_QPNP_FG_GEN3		电量计	pm660_fg
				qpnp-fg-gen3.c  fg-memif.c  fg-util.c
			
			CONFIG_QPNP_SMB2		charger	pm660_charger
				step-chg-jeita.c   battery.c  qpnp-smb2.c  smb-lib.c  pmic-voter.c  storm-watch.c 

			
			CONFIG_WLS_CHARGER_ECH  无线充电
				ech-wls-charger.c 
			
			
			
			dtsi
				msm-pm660.dtsi	
				fg-gen3-batterydata-gionee-17g08a-atl-4v4-5060mAh.dtsi
				sdm660-common.dtsi
				msm-smb138x.dtsi
				sdm660-pinctrl.dtsi
				sdm660-qrd.dtsi
		}
		
		

		
		extcon 外设插拔检测
		
		文档号
		{
			evaluate 
				80-P7905-5A
				80-P7905-1
			design
				80-P8754-59
				80-P8754-6
				80-NN255-1
				80-P8754-30
				80-NR097-1
				80-NN139-1
			bringup
				80-P7905-2X
				80-P7905-4
				80-NM620-1
			customize
				80-P2484-42
				80-P2484-77
				80-P2484-74
				80-P8754-43
				80-NM328-53
			verify
				80-P7747-7
				80-P8754-34
			
			debug tool 这个很重要
				The following debug tools for PMIC are available:
				• How to collect a PMIC register dump?
				(Solution #00029307)
				• How to collect to read/write PMIC registers using
				ADB?
				(Solution #00029310)
				• How to enable PMIC driver logs for debugging?
				(Solution #00029311)	
				For more debug information, refer to the debug
				sections in the individual PMIC module User Guide
				documents listed in PMIC Software Master Document
				(80-NR097-1).			
						
		}

	}
	
	
	
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
















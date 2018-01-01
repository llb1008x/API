


遇到一个问题站在设计者的角度还是使用者的角度，转换思考的角度有些问题就可以想清楚点



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
	
	NTC温度电压转换函数，看看过程什么样的 这个是mtk平台的
	force_get_tbat_internal 温度转换函数 相关参数知道什么意思
		12-07 16:47:12.320362 <3>[22144.642470]  (0)[70:pmic_thread][BattVoltToTemp] 498 24000 2773 -35
		12-07 16:47:12.320370 <3>[22144.642478]  (0)[70:pmic_thread][force_get_tbat] 533,498,1,2049,100,43 r:75 100

	
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
		
		
	    battery ID相关的内容，根据ID选择电池曲线
		{
			battery_id  ，研读相关代码
			读battery_id 电压，匹配电池曲线
		
			int g_fg_battery_id;

			#ifdef MTK_GET_BATTERY_ID_BY_AUXADC
			void fgauge_get_profile_id(void)
	
		}

	}
	

	usb相关
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
		
		
		extcon 插拔检测
		
		
		KeyguardUpdateMonitor

	}
	
	

}
















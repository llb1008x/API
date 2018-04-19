
POWER_SUPPLY_PROP_FG_RESET_CLOCK

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
















个人技能：
1. 熟悉linux环境下开发，熟悉Linux系统常用命令。
2. 掌握linux下c/c++语言编程。
3. 熟悉高通，MTK平台开发。
4. 掌握linux平台下字符设备驱动的编写，熟悉i2c，spi等总线协议。
5. 熟悉bootloader和kernel启动和工作的大致过程，移植，裁剪过linux内核。
6. 掌握常见进程间通信的方法，了解多线程，多进程程序设计的设计思想。
7. 了解tcp/ip通信协议,了解c/s架构流程,熟练使用socket套接字进行编程。
8. 熟练使用makefile,shell脚本进行项目的开发。
9. 良好的英语阅读能力,能够快速阅读相关英文文档。
10. 熟练掌握git,vim,gdb,source insight,wireshark等开发工具的使用,能看懂硬件相关的原理图。



工作经历：
公司名称:深圳金立通信设备有限公司
在职时间：2016年9月到2018年2月
职位名称：android驱动工程师
工作职责：
1.主要负责公司智能机项目，电源管理，usb，thermal，功耗等模块驱动的开发和调试。
2.实现领导要求的部分功能。
3.参与相关模块第三方IC的评估，调试。

实现的一些功能：OTG开关；客制化关机充电logo；延长电池使用寿命；适配平均电流apk，上报电量计的值，主要用于电流和功耗分析；
根据不同场景控制充电电流。



培训经历：
公司名称：北京凌阳爱普教育
时间：2016年5月到2016年9月
培训内容：培训嵌入式linux，包括C/C++，linux系统编程，linux网络编程，linux平台驱动等。



项目经历：
项目一：金刚3(GN5005）
项目时间：2017年6月到2017年11月
项目描述：
基于高通MSM8917+PM8937平台，android N的智能机项目。
1.充电存在充电电流跳变的问题，充电电流设置，aicl，可能的风险
2.usb眼图不过，usb眼图的一些参数意思，如何调整参数
3.diag口打不开，usb的一些权限，vid/pid，init.rc,功能
4.PMI8937平台，手机battery ID接触稍晚，手机不开机，battery id的检测流程
5.电量在93%+ ，插充电器重启之后，显示充电已满，但实际电量仍为93%。
6.插入充电器需要较长时间才显示插入。


项目二：W919
项目时间：2017年1月到2017年12月
项目描述：
基于MTK mt6757+mt6355+rt5081平台，android N的智能机项目。
1.温升相关问题，高低温不过标准，整机使用发热较严重。
2.过压测试：NG，10V时电压不不截止，还有380ma~440ma，无电压过高提醒。
3.低电量开启闪光灯，导致照片较黑。
4.关机状态，连接充电器，显示电量灭屏后，短按两次电源键1s，出现开机画面，再次操作恢复。
5.高温高湿回温后，LED灯一直亮。



项目三：基于tcp/ip协议的路由器项目
项目描述: 通过查阅资料并模拟路由器工作的相关过程，熟悉了路由器的工作原理。采用多线程并行程序设计，一个负责数据包的发送和接收，
另一个负责终端发送的命令，配置路由器。路由器接收局域网内的数据包请求，查询ARP缓存表，如果有就修改mac地址，再发送数据包，
没有就发送ARP广播请求，逐级发送直到找要的目的mac，然后再转发数据包。
通过设置过滤IP，不转发数据包，实现防火墙的功能。
项目收获:
1. 熟悉了简单路由器工作的原理，加深了对tcp/ip协议栈工作的理解。
2. 网络数据包传递的过程,熟练了链表和文件的相关的操作。
3. 熟悉抓包工具的使用，对于数据包的组包，拆包，解包，分析。
























{




	ANDROID TREBLE OVERVIEW                                          80-PE644-1
	ANDROID TREBLE VNDK OVERVIEW                                     80-PE644-2
	ANDROID TREBLE HIDL OVERVIEW                                     80-PE644-3
	ANDROID TREBLE KERNEL OVERVIEW                                   80-PE644-4
	ANDROID TREBLE VTS OVERVIEW                                      80-PE644-5
	VERIFIED BOOT 2.0 INTRODUCTION                                   80-PE644-6
	SELINUX OVERVIEW AND UPDATE FOR ANDROID O        				 80-PE644-7
	
	Bus 003 Device 021: ID 058f:6387 Alcor Micro Corp. Flash Drive

	
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





	近期W919项目为了满足生产需求，T3-3更新了一颗speaker 2557的物料，此物料不可兼容，因此区分版本维护：
	1.T3-3前面批次机器，请刷版本尾号为AB的版本
	2.T3-3以及之后批次机器，请刷版本尾号AA的版本

	目前大家手里机器都是T3-3前的，所以测试和开发请根据手中机器状态刷AB版本。

	如果刷错版本会遇到扬声器各类场景无声的问题！！！
	版本情况：
	BJ17G10A-T0148-171116AB --可刷T3-3之前机器
	BJ17G10A-T0147-171116AB --代码问题，扬声器无声
	BJ17G10A-T0146-171114AA --代码问题，扬声器无声
	BJ17G10A-T0145-171114AA --可刷T3-3及之后机器

}	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
17G10A
{

	extcon
	
	GNSPR#128052,测机连接电脑USB端口，下拉状态栏显示USB已连接，传文件打开，电脑不显示便捷设备，换电脑，插拔USB端口不恢复，
	重启恢复
	{

		Dear Customer，

		您好，如果您的测试版本是eng版本，请把selinux关掉试试，即adb shell setenforce 0；
		或者是加上如下selinux 规则试试，谢谢！
		#====================== untrusted_app.te ======================
		allow untrusted_app mtp_device:chr_file rw_file_perms; 	
		
		
		等待测试复测,这里有一个selinux是什么，有什么作用
		1.user版本不能修改selinux的开关；
		2.如果要编译untrust_app.te，单独编译boot就可以生效；
		
		
		selinux qcom有一个fatab.com里面是一些挂载执行的命令     
		
		代码路径
		external/sepolicy – Google define
		device/qcom/sepolicy/common – Qualcomm define
		
		
		Disable/enable
		Kernel command line
		adb shell setenforce 0
		
		device/qcom/sepolicy/common/qmuxd.te
		
		
		G1605B_sign.mk
		#========== Security Boot switch start==================
		#gionee, duanyh, for verified boot, begin
		CONFIG_GN_BSP_MTK_SBC_FLAG=y
		MTK_SECURITY_SW_SUPPORT=yes
		CONFIG_MTK_SECURITY_SW_SUPPORT=yes
		MTK_SEC_CHIP_SUPPORT=yes	
		MTK_SEC_USBDL=ATTR_SUSBDL_ENABLE
		MTK_SEC_BOOT=ATTR_SBOOT_ENABLE
		MTK_SEC_MODEM_AUTH=no
		MTK_SEC_SECRO_AC_SUPPORT=yes
		CUSTOM_SEC_AUTH_SUPPORT=no 
		MTK_VERIFIED_BOOT_SUPPORT=yes
		MTK_SEC_FASTBOOT_UNLOCK_SUPPORT=no 
		MTK_SEC_FASTBOOT_UNLOCK_KEY_SUPPORT=no
		#gionee, duanyh, for verified boot, end

		#gionee ranyi add for gionee security boot start
		GN_SECURITY_BOOT_SW_SUPPORT=yes
		GN_SECURITY_BOOT_1.1_TRUSTZONE_KEY_SUPPORT=yes
		#gionee ranyi add for gionee security boot end

		#wangguojun add for secure boot
		GN_VERIFIED_BOOT_GN_MP_KEY = yes

		#wangguojun add for kph build
		GN_KPH_BUILD_SUPPORT=yes
		#========== Security Boot switch end==================
		
		
		How do I disable and enable verity via adb?
		A. adb disable-verity
		disable dm-verity checking on USERDEBUG builds
		adb enable-verity
		re-enable dm-verity checking on USERDEBUG builds
		source code: system/core/adb/commandline.cpp
	}











	GNSPR#134414，【内部体验】低电量关机状态下》连接充电器后》手动点亮屏幕一直在闪花屏 （开机起来显示28%电量）
	{
		
	
	}








	GNSPR#103678,通话：接听或者挂断电话时，振动的同时会有崩的一声
	{
		这里主要是调试马达的震动强度，因为马达的震感有点强
	
		文档
		{	
			输出的强度应该跟output voltage有关，
			The output voltage is based on the duty cycle of the provided PWM signal, where the OD_CLAMP[7:0] bit in
			 register 0x17 sets the full-scale amplitude
			 
			pwm模式强度靠调整占空比改变 the strength of vibration is determined by the duty cycle 
		
			A waveform identifier is an integer value referring to the index position of a waveform in the RAM library
			 
	
			The smart-loop architecture is an advanced closed-loop system that optimizes the performance of the actuator
		and allows for failure detection. The architecture consists of automatic resonance tracking and reporting (for an
		LRA), automatic level calibration, accelerated startup and braking, diagnostics routines, and other proprietary
		algorithms.
	
	
			The RATED_VOLTAGE[7:0] bit in register 0x16 sets the rated voltage for the closed-loop drive modes.
		
			In open-loop mode, the RATED_VOLTAGE[7:0] bit is ignored. Instead, the OD_CLAMP[7:0] bit (in register 0x17)
		is used to set the rated voltage for the open-loop drive modes.

			The DRV2604L slave address is 0x5A (7-bit), or 1011010 in binary.
		
		}
		
		
		相关的代码
		{
			关键字
			{
				vibrate,haptic,pwm,drv2604l
				
				OD_CLAMP[7:0],DATA_FORMAT_RTP,RATED_VOLTAGE[7:0]
				
				//Gionee <GN_BY_CHG> <lilubao> <20171114> add for change vibrate end
			}
			
			当前项目的配置是什么样的
			{
				static struct actuator_data DRV2604L_actuator={
					.device_type = LRA,
					.rated_vol = 0x46,	//1.8V
					.over_drive_vol = 0x7a,
					.LRAFreq = 235,
				};
				
				
				pDrv2604Platdata->GpioTrigger=0;
				pDrv2604Platdata->loop=CLOSE_LOOP;
				pDrv2604Platdata->RTPFormat=Signed;
				pDrv2604Platdata->BIDIRInput=BiDirectional;
				
				P30 各种模式配置的解释
				
				P22 Rated Voltage Programming 0x16 额定电压
				
				Overdrive Voltage-Clamp Programming
				
				CLAMP voltage s	钳位电压
				
				控制输出的主要是Rated Voltage
			
			}
		}
		
		
		现在确定是硬件结构的问题还是软件的问题？
		[FAQ11345][Speech] 来电接听瞬间概率会有咔的一声
		
		
		同音频，硬件，结构的工程师共同确认了一下
		这个在挂断电话的时候听筒能听到挂断声音，同时还有震动的声音
		是因为，17G10A整机大部分是金属结构，特别是外壳，马达震动的时候，震动很容易传递
		到整个机器

		结构工程师有一个减弱的方案，就是修改转轴部分材料，但是容易影响外观，而且效果减小不多

		做了一个对比实验：
		1.关掉震动，没有问题
		2.调低震动强度，有问题
		3.扣掉听筒，飞线远离主板，没有问题
		4.在3的基础上，听原来听筒的位置，有问题，是整机震动的声音

		即是说这个问题与软件无关，是金属材料机身容易传递震动

	}












	
	省电适配参数导入
	{
		    电量百分比 	电池满电情况下的放电时间（hour）（home界面、最大亮度，永不灭屏）电池满电情况下的放电时间 （h） 	备注
			100% - 90% 	1:30:00 	1.第一次格式化升级完成后测试
			2.插入单张SIM卡（信号稳定）和T卡
			3.默认设置，显示--休眠设为“无限时”
			4.处于home界面 关闭桌面动态天气
			5.亮度调整为最大
			6.尽可能多几台机器一起测试，取平均值
			7.静置放电
			90% - 80% 	1:01:00
			80% - 70% 	1:05:00
			70% - 60% 	1:08:00
			60% - 50% 	1:04:00
			50% - 40% 	1:06:00
			40% - 30% 	1:06:00
			30% - 20% 	1:06:00
			20% - 10% 	1:07:00
			10% - 关机 	1:18:00



			进入极致省电模式 	待机电流（mA）(灭屏，均在home界面） 	备注
			出厂状态下，电量1%，进入极致省电模式 	8.52 	1.插入单张SIM卡（信号稳定）和T卡
			2.电量在1%
			3.每组数据测试15分钟以上；


			电池电量 	mAh
			查看电池毫安数 	3000mAh

			充电方式 	mA
			AC充电 	2097.902mA
			USB充电 	652.173mA

			初始屏幕亮度 	整数值 	备注
			出厂设置亮度值 	337.61mA 	1.此亮度值为 设置--显示 内的亮度值范围0~255   

			　 	测试条件 	整机电流(mA)(亮屏，均在home界面) 	备注
			情形1 	第一次格式化升级完成后，插入单张SIM卡和T卡状态下的工作电流(100%亮度) 	240.49 	1.去掉桌面动态天气
			2.显示-休眠改为“无限时”
			3.home界面（除亮度外，其他条件相同）
			4.每组数据测试15分钟以上；
			情形2 	  情形1 + 75%屏幕亮度 	214.89
			情形3 	  情形1 + 50%屏幕亮度 	182.74
			情形4 	  情形1 + 25%屏幕亮度 	158.79

			这里面一系列参数都是在
			packages_mtk_mp/gionee/private/BJ17G10A01_A/apps/Amigo_SystemManager/config.xml 里面配置的
	}








	GNSPR#109502,后台播放音乐，进浏览器，插充电器无振动，不显示充电，也没有指示灯，拔掉充电器手机振动一下，再次插充电器恢复
	{
		//Gionee <GN_BSP_CHG> <lilubao> <20171120> add for debug begin
		增加了部分log， rt5081_pmu_charger.c ,gn_ti_drv2604l.c
	
	}







	GNSPR#113442,【品质压力】待机界面》手机连接电脑USB端口-手机下拉通知栏-无USB连接显示-插拔数据线仍如此》
	清除后台未恢复-清除数据未恢复-重启恢复 	
	{
		
		从log上看，mIsMonkeyTest running !!!! updateUsbNotification id = 0，可能测试之前做了monkeytest或者cts的测试
		测试条例没有跑完
		在monkeytest和cts的测试中会disable usb，停止发送广播导致连接usb没有弹框

		09-14 15:58:49.984703 1127 1437 V UsbDeviceManager: USB UEVENT: {USB_STATE=CONNECTED, SUBSYSTEM=android_usb, SEQNUM=3224, ACTION=change, DEVPATH=/devices/virtual/android_usb/android0}
		09-14 15:58:49.984939 1127 1437 D UsbDeviceManager: gndb,start send MSG_UPDATE_STATE, state=CONNECTED
		09-14 15:58:49.986844 1127 1224 D UsbDeviceManager: gndb, receive MSG_UPDATE_STATE, state=connect
		09-14 15:58:49.988841 1127 1224 D UsbDeviceManager: mIsMonkeyTest running !!!! updateUsbNotification id = 0
		09-14 15:58:49.989022 1127 1224 D UsbDeviceManager: monkey test is running!!! updateBsbState return here.
		09-14 15:58:49.991562 1127 1437 V UsbDeviceManager: USB UEVENT: {USB_STATE=DISCONNECTED, SUBSYSTEM=android_usb, SEQNUM=3225, ACTION=change, DEVPATH=/devices/virtual/android_usb/android0}
		09-14 15:58:49.991680 1127 1437 D UsbDeviceManager: gndb,start send MSG_UPDATE_STATE, state=DISCONNECTED
		09-14 15:58:50.093651 1127 1437 V UsbDeviceManager: USB UEVENT: {USB_STATE=CONNECTED, SUBSYSTEM=android_usb, SEQNUM=3227, ACTION=change, DEVPATH=/devices/virtual/android_usb/android0}
		09-14 15:58:50.093784 1127 1437 D UsbDeviceManager: gndb,start send MSG_UPDATE_STATE, state=CONNECTED
		09-14 15:58:50.093982 1127 1224 D UsbDeviceManager: gndb, receive MSG_UPDATE_STATE, state=connect
		09-14 15:58:50.094170 1127 1224 D UsbDeviceManager: mIsMonkeyTest running !!!! updateUsbNotification id = 0
		09-14 15:58:50.094220 1127 1224 D UsbDeviceManager: monkey test is running!!! updateBsbState return here.
		09-14 15:58:50.173255 1127 1437 V UsbDeviceManager: USB UEVENT: {USB_STATE=CONFIGURED, SUBSYSTEM=android_usb, SEQNUM=3228, ACTION=change, DEVPATH=/devices/virtual/android_usb/android0}
		09-14 15:58:50.173373 1127 1437 D UsbDeviceManager: gndb,start send MSG_UPDATE_STATE, state=CONFIGURED
		09-14 15:58:50.173519 1127 1224 D UsbDeviceManager: gndb, receive MSG_UPDATE_STATE, state=configure
		09-14 15:58:50.173661 1127 1224 D UsbDeviceManager: mIsMonkeyTest running !!!! updateUsbNotification id = 0
		09-14 15:58:50.173702 1127 1224 D UsbDeviceManager: monkey test is running!!! updateBsbState return here.
		09-14 15:58:50.175024 1127 1517 V WindowManager: Changing focus from Window{13ef238 u0 com.gionee.amisystem/com.android.launcher2.Launcher} to Window{bab8a35 u0 StatusBar} Callers=com.android.server.wm.WindowManagerService.relayoutWindow:3179 com.android.server.wm.Session.relayout:265 android.view.IWindowSession$Stub.onTransact:286 com.android.server.wm.Session.onTransact:178
		09-14 15:58:50.175166 1127 1517 D WindowManager: Input focus has changed to Window{bab8a35 u0 StatusBar}
		09-14 15:58:57.360301 1127 1437 V UsbDeviceManager: USB UEVENT: {USB_STATE=DISCONNECTED, SUBSYSTEM=android_usb, SEQNUM=3234, ACTION=change, DEVPATH=/devices/virtual/android_usb/android0}
		09-14 15:58:57.360415 1127 1437 D UsbDeviceManager: gndb,start send MSG_UPDATE_STATE, state=DISCONNECTED
		09-14 15:58:57.360589 1127 1437 V UsbDeviceManager: USB UEVENT: {USB_STATE=HWDISCONNECTED, SUBSYSTEM=android_usb, SEQNUM=3235, ACTION=change, DEVPATH=/devices/virtual/android_usb/android0}
	
	
		本地通过*#837004#暗码进入后选择monkeytest会disable usb，而monkeytest需要重启或者跑完才会关闭，而cts相当于apk
		有很多条，很多apk，跑完一条后会卸载相应的apk
		mIsMonkeyTest running !!!! updateUsbNotification id 在usbdevicemanager.java 里面有updateUsbNotification 根据不同的usb配置
		设置id为不同的title上后update 
	}


	
	
	
	
	
	GNSPR#135244,【老化测试】W919 机型 T0143 版本，充电测试未通过在电流测试中，老化测试异常
	{
		这里的问题是battery plug out 还是charger out
		bat_plug_out这个是电池拔出的log吗？
		mtk_battery.c
		battery_meter_ctrl(BATTERY_METER_CMD_GET_BOOT_BATTERY_PLUG_STATUS, &plugout_status_new);
		
		
		[fg_drv_update_hw_status] current:534 270 state:0 0 car:-220 -220 bat:4237 4243 chr:4834 4834 hwocv:1234 1234 bat_plug_out:1 1 tmp:34 34 imix 18750 rac 546
		
		[dod_init_result] NVRAM_ready 1 Embedded 1 plug_out 1 is_hwocv_unreliable 0 rtc_invalid 0 rtc_ui_soc 4300 two_sec_reboot 0 old_data.ui_soc 4374
	
	}
	
	
	
	
	
	
	
	
	
	
	
	
	GNSPR#100830,充电时按开机键开机，测试值是6.83s，标准值是4.5s，超出标准值2.33s
	{
		现在的关机充电条件下按powerkey到亮logo时间太长
		
		相关代码
		{
			key_chontrol.cpp  key_control
			
			linux-event-codes.h描述event
		
		}
		
		
		现在主要分两部分分析：powerkey+usb handshake
		{
			//powerkey
			
			

			[PMIC] pl pmic powerkey Release
			[PMIC] pmic_IsUsbCableIn 1
			[PLFM] USB/charger boot!
			[PMIC] PMIC_POWER_HOLD ON



			mtk detect key function pmic_detect_homekey MTK_PMIC_RST_KEY = 17
			[PMIC] pl pmic FCHRKEY Release




			[   11.483398] <4>.(4)[295:kpoc_charger]charger: draw_with_interval... key_trigger_suspend = 0
			[   11.499852] <4>.(4)[70:pmic_thread][name:pmic_irq&][PMIC] [PMIC_INT] addr[0x854]=0x1
			[   11.500840] <4>.(4)[70:pmic_thread][name:kpd&]kpd: Power Key generate, pressed=1
			[   11.501783] <4>.(4)[70:pmic_thread][name:hal_kpd&]kpd: kpd: (pressed) HW keycode =116 using PMIC
			[   11.502868] <4>.(4)[70:pmic_thread][name:aed&](pressed) HW keycode powerkey
			[   11.503837] <4>.(4)[276:kpoc_charger]charger: key_control: event.type:1,116:1
			[   11.504766] <4>.(4)[276:kpoc_charger]charger: key_control: event.type:0,0:0
			[   11.505666] <4>.(4)[296:kpoc_charger]charger: pwr key long press check start




			//usb handshake

			[SEC] read '0x8800000'
			0x4D,0x4D,0x4D,0x4D,0x4,0x0,0x0,0x0,
			[LIB] seclib_img_auth_load_sig [LIB] CFG read size '0x2000' '0x3C'
			0x4D4D4D4D
			[LIB] SEC CFG 'v4' exists
			[LIB] HW DEC
			GCPU Enhance,V1.1
			[LIB] SEC CFG is valid. Lock state is 1 
			[BLDR] Starting tool handshake.
			€€€€€€€€€€€[BLDR] Tool connection is unlocked
			[platform_vusb_on] VUSB33 is on
			[platform_vusb_on] VA10 is on
			[platform_vusb_on] VA10 select to 0.9V
			rt5081_enable_chgdet_flow: en = 0
			rt5081_enable_chgdet_flow: en = 1
			mtk_ext_chgdet: usb_stats = 0x00000020
			mtk_ext_chgdet: chg type = 1

			[PLFM] USB cable in
			[TOOL] USB enum timeout (Yes), handshake timeout(Yes)
			[TOOL] Enumeration(Start)
			HS is detected
			HS is detected
			[TOOL] Enumeration(End): OK 524ms 
			usbdl_flush timeoutintrep :0, IntrTx[0] IntrRx [0]usbdl_flush timeoutintrep :0, IntrTx[0] IntrRx [0]usbdl_flush timeoutintrep :0, IntrTx[0] IntrRx [0]usbdl_flush timeoutintrep :0, IntrTx[0] IntrRx [0]usbdl_flush timeoutintrep :0, IntrTx[0] IntrRx [0][TOOL] : usb listen timeout
			[TOOL] <USB> cannot detect tools!
			[TOOL] <UART> listen  ended, receive size:0!

			[TOOL] <UART> wait sync time 150ms->5ms
			[TOOL] <UART> receieved data: ()
		
		}
		
		
	
	}
	
	



	GNSPR #128052,【品质压力】测机连接电脑USB端口，下拉状态栏显示USB已连接，传文件打开，电脑不显示便捷设备，换电脑，插拔USB端口不恢复，重启恢复
	{
		(mtk_chg_type_det.c) mt_charger_set_property,power_supply_property
		->
		dump_charger_name -> (mtk_usb.c) mt_usb_connect
	
		(mtk_charger.c) mtk_charger_int_handler
	
		wakeup_fg_algo 唤醒电量计的各种原因 然后封装fgd_msg上报给nl_send_to_user  user space
		typedef enum {
			FG_INTR_0 = 0,
			FG_INTR_TIMER_UPDATE  = 1,
			FG_INTR_BAT_CYCLE = 2,
			FG_INTR_CHARGER_OUT = 4,
			FG_INTR_CHARGER_IN = 8,
			FG_INTR_FG_TIME =		16,
			FG_INTR_BAT_INT1_HT =	32,
			FG_INTR_BAT_INT1_LT =	64,
			FG_INTR_BAT_INT2_HT =	128,
			FG_INTR_BAT_INT2_LT =	256,
			FG_INTR_BAT_TMP_HT =	512,
			FG_INTR_BAT_TMP_LT =	1024,
			FG_INTR_BAT_TIME_INT =	2048,
			FG_INTR_NAG_C_DLTV =	4096,
			FG_INTR_FG_ZCV = 8192,
			FG_INTR_SHUTDOWN = 16384,
			FG_INTR_RESET_NVRAM = 32768,
			FG_INTR_BAT_PLUGOUT = 65536,
			FG_INTR_IAVG = 0x20000,
			FG_INTR_VBAT2_L = 0x40000,
			FG_INTR_VBAT2_H = 0x80000,
			FG_INTR_CHR_FULL = 0x100000,
			FG_INTR_DLPT_SD = 0x200000,
			FG_INTR_BAT_TMP_C_HT = 0x400000,
			FG_INTR_BAT_TMP_C_LT = 0x800000,
			FG_INTR_BAT_INT1_CHECK = 0x1000000,
			FG_INTR_DUMP_INFO = 0x2000000,

		} FG_INTERRUPT_FLAG;

		FG_POWER_SUPPLY
		FG_MEM_DEBUG_READS
	}

	
	
	
	
	

	

}	
	
	
	
	
	
	

	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	









	
























/*********************************************************************************************************************************/
17G06A
{

	PPT 测试 power performance  thermal
	{
		usb-type-c-pericom
		CONFIG_USB_EXT_TYPE_C_PERICOM
		
		fuseblower qcom  的secure boot
		
		
		pericom-type-c@1d {
			compatible = "pericom,usb-type-c";
			reg = <0x1d>;
			vdd_io-supply = <&pm8917_l5>;
			interrupt-parent = <&tlmm>;
			interrupts = <97 2>;	/* TLMM 97, TRIGGER_FALLING */
			pericom,enb-gpio = <&tlmm 131 0x1>;	/* active low */
			pinctrl-names = "default";
			pinctrl-0 = <&usbc_int_default>;
			//Gionee <GN_BSP_CHG> <lilubao> <20171115> modify for remove typr-c begin
			status = "disabled";
			//Gionee <GN_BSP_CHG> <lilubao> <20171115> modify for remove typr-c end
		};
		
		06量产分支
		bj17g06a_main_dev
		
		status = "disabled";

		对于高通平台项目的签名方式要注意以下几点：
		{
			1. 压缩包名以 BJ_G1602A （具体项目名）打头；
			2. 必须 zip 格式（一定要在linux下压缩）；
			3. zip压缩包内不能包含目录；
			4. 压缩包内必须包含的文件列表：
				8976_fuseblower_USER.xml  ,8976_secimage.xml   
				这两个文件在L33_QCOM_8920_17G16A_170605_MODEM/gionee/BJ17G06/MSM8917.LA.3.0/common/sectools/config/目录下
				有高通平台的签名工具，xml里面是各种key和相关的镜像
				
				sign_img_list.txt
				需要签名的镜像文件
		}

	}






	金刚3续航问题
	{
		1.GNSPR#133023,【视频续航】金刚3视频续航（外放）比华为畅享7平均电流高50mA左右
	
		2.对比M7续航时间差很多
		
		3.中批比T2机械手臂时间差一个小时

	}








	dump pimc register,80-NL708-1
	{
		未成功，还是有问题，寄存器信息没有打印出来

		Mount debug file system
			adb shell mount -t debugfs none /sys/kernel/debug
			cd /sys/kernel/debug/spmi/spmi-0
			
		Set number of bytes to read/write
			echo 1 > count
		
		Set address
			echo 0x8041 > address
		
		write 
			echo 0x11 > data
		
		read 
			cat data
			
			
		Example – Set GPIO1 to output high and read back other parameters
			adb shell mount -t debugfs none /sys/kernel/debug
			cd /sys/kernel/debug/spmi/spmi-0
			echo 1 > count
			echo 0xC040 > address // MODE_CTL
			echo 0x11 > data
			// DO + HIGH
			echo 0xC041 > address // DIG_VIN_CTL
			cat data	
					



		SPMI Debug-FS support
		
		Hierarchy schema:
		/sys/kernel/debug/spmi
			   /help            -- Static help text
			   /spmi-0          -- Directory for SPMI bus 0
			   /spmi-0/address  -- Starting register address for reads or writes
			   /spmi-0/count    -- Number of registers to read (only used for reads)
			   /spmi-0/data     -- Initiates the SPMI read (formatted output)
			   /spmi-0/data_raw -- Initiates the SPMI raw read or write
			   /spmi-n          -- Directory for SPMI bus n

		To perform SPMI read or write transactions, you need to first write the
		address of the slave device register to the 'address' file.  For read
		transactions, the number of bytes to be read needs to be written to the
		'count' file.

		The 'address' file specifies the 20-bit address of a slave device register.
		The upper 4 bits 'address[19..16]' specify the slave identifier (SID) for
		the slave device.  The lower 16 bits specify the slave register address.

		Reading from the 'data' file will initiate a SPMI read transaction starting
		from slave register 'address' for 'count' number of bytes.

		Writing to the 'data' file will initiate a SPMI write transaction starting
		from slave register 'address'.  The number of registers written to will
		match the number of bytes written to the 'data' file.

		Example: Read 4 bytes starting at register address 0x1234 for SID 2

		echo 0x21234 > address
		echo 4 > count
		cat data

		Example: Write 3 bytes starting at register address 0x1008 for SID 1

		echo 0x11008 > address
		echo 0x01 0x02 0x03 > data

		Note that the count file is not used for writes.  Since 3 bytes are
		written to the 'data' file, then 3 bytes will be written across the
		SPMI bus.
		
		
		
		static struct fg_mem_data fg_data[FG_DATA_MAX] = {
			/*       ID           Address, Offset, Length, Value*/
			DATA(BATT_TEMP,       0x550,   2,      2,     -EINVAL),
			DATA(OCV,             0x588,   3,      2,     -EINVAL),
			DATA(VOLTAGE,         0x5CC,   1,      2,     -EINVAL),
			DATA(CURRENT,         0x5CC,   3,      2,     -EINVAL),
			DATA(BATT_ESR,        0x554,   2,      2,     -EINVAL),
			DATA(BATT_ESR_COUNT,  0x558,   2,      2,     -EINVAL),
			DATA(BATT_SOC,        0x56C,   1,      3,     -EINVAL),
			DATA(CC_CHARGE,       0x570,   0,      4,     -EINVAL),
			DATA(VINT_ERR,        0x560,   0,      4,     -EINVAL),
			DATA(CPRED_VOLTAGE,   0x540,   0,      2,     -EINVAL),
			DATA(BATT_ID,         0x594,   1,      1,     -EINVAL),
			DATA(BATT_ID_INFO,    0x594,   3,      1,     -EINVAL),
		};
		
	//Gionee <GN_BSP_CHG> <lilubao> <20171101> modify for pmic register dump begin
	dev_err(&sdev->dev, "in [%s] by lilubao addr->%d,buf->%s,len->%d,sid->%d\n",
		__FUNCTION__,addr,buf,len,sid);
	//Gionee <GN_BSP_CHG> <lilubao> <20171101> modify for pmic register dump end
	
	}







		GNSPR#122265，连接充电器，长按电源键关机，关机完成后，长按电源键5s测机不开机，只显示在关机充电图标界面，
		（在充电图标界面长按电源键则可以开机），用户体检不佳 暂未恢复 对比17G16-T0119版本有此现象，对比17G02-T2638版本无此现象，
		验证10台10台100%
		{
			分析：
				S10C 在关机充电条件下，长按powerkey，不会亮屏，长按时间从按下开始，4~5s内重启
				而17G06A长按会先亮屏，灯灭屏之后，才重新计时，导致时间较长
				从log上看，长按powerkey，先是down，0.21s之后又up，导致认为是短按，亮屏
				
			{	
				亮屏条件下检测到up，开始计时
				[ 15.993615] *(2)[324:charger]charger: [15986] key[116] down
				[ 16.054725] *(2)[324:charger]charger: reboot_timeout->17986,now->16026

				这个长按之后可以在4~5s内重启

				但是在灭屏条件下长按有问题
				[ 13.154409] *(1)[324:charger]charger: [13154] key[116] down
				[ 13.154498] *(1)[324:charger]charger: reboot_timeout->15154,now->13154

				灭屏条件下长按先亮屏但是同时会上报按键抬起的动作，但是按键一直是按着的
				[ 13.760229] *(1)[324:charger]charger: [13760] key[116] up (was down for 0.606sec)
			}
			
			
			[   14.206991] *(0)[337:charger]charger: [14206] key[116] down

			[   14.430061] *(0)[365:charger]msm_thermal:msm_thermal_update_freq Freq mitigation task is not initialized
			[   14.444072] *0)PM: Some devices failed to suspend, or early wake event detected
			[   14.452183] *(2)[365:charger]msm_thermal:msm_thermal_update_freq Freq mitigation task is not initialized

			[   14.452223] *(0)[337:charger]charger: [14452] key[116] up (was down for 0.246sec)
			
			
			healthd目录下有main函数，根据传入的字符串决定是关机充电还是recovery，然后传递相应的函数接口
			switch (ch) {
		        case 'c':
		            healthd_mode_ops = &charger_ops;
		            break;
		        case 'r':
		            healthd_mode_ops = &recovery_ops;
		            break;
		        case '?':
		        default: ...
            }
            
            static struct healthd_mode_ops charger_ops = {
				.init = healthd_mode_charger_init,
				.preparetowait = healthd_mode_charger_preparetowait,
				.heartbeat = healthd_mode_charger_heartbeat,
				.battery_update = healthd_mode_charger_battery_update,
			};
			
			
			这应该跟healthd目录下的按键处理有关
			按键处理的主要在healthd_mode_charger_heartbeat，
			(healthd_mode_charger.cpp) handle_input_state  ->  process_key
			主要问题应该在process_key这个判断里面
			
			
			
			(healthd_mode_charger.cpp) set_key_callback -> update_input_state -> input_callback
			
			
			//Gionee <GN_BSP_CHG> <lilubao> <20171025> modify for healthd begin
			LOGE("in [%s] by lilubao after\n",__FUNCTION__);
			//Gionee <GN_BSP_CHG> <lilubao> <20171025> modify for healthd end
			
			
			//Gionee <GN_BSP_CHG> <lilubao> <20171025> modify for healthd begin
			pr_err("in [%s] by lilubao after\n",__FUNCTION__);
			//Gionee <GN_BSP_CHG> <lilubao> <20171025> modify for healthd end
			
			
			
			drivers/video/msm/mdss/mdss_dsi_panel.c 
			mdss_dsi_panel_bl_ctrl  控制背光		
			
			[   13.154827] *(1)[324:charger]charger: in [healthd_mode_charger_heartbeat] by lilubao before
			[   13.154837] *(1)[324:charger]charger: in [handle_input_state] by lilubao before
			[   13.154846] *(1)[324:charger]charger: in [process_key] by lilubao before
			[   13.154855] *(1)[324:charger]charger: 1111111111111 by lilubao
			[   13.154866] *(1)[324:charger]charger: reboot_timeout->15154,now->13154
			[   13.154879] *(1)[324:charger]charger: charger->batt_anim->capacity->37,charger->boot_min_cap->0
			[   13.154888] *(1)[324:charger]charger: 6666666666666666 by lilubao
			[   13.154897] *(1)[324:charger]charger: in [set_next_key_check] by lilubao before
			[   13.154907] *(1)[324:charger]charger: in [set_next_key_check] by lilubao after
			[   13.154916] *(1)[324:charger]charger: bbbbbbbbbbbbbb by lilubao
			[   13.154925] *(1)[324:charger]charger: in [process_key] by lilubao after
			[   13.154934] *(1)[324:charger]charger: in [handle_input_state] by lilubao after
			[   13.154944] *(1)[324:charger]charger: in [healthd_mode_charger_heartbeat] by lilubao after
			[   13.740015] *(0)[352:charger]msm_thermal:msm_thermal_update_freq Freq mitigation task is not initialized

			[   13.752729] *0)PM: Some devices failed to suspend, or early wake event detected

			[   13.760122] *(0)[352:charger]msm_thermal:msm_thermal_update_freq Freq mitigation task is not initialized
			[   13.760161] *(1)[324:charger]charger: in [input_callback] by lilubao before
			[   13.760175] *(1)[324:charger]charger: 11111111111
			[   13.760191] *(1)[324:charger]charger: in [update_input_state] by lilubao before 
			[   13.760203] *(1)[324:charger]charger: ev->code->116,ev->value->0
			[   13.760216] *(1)[324:charger]charger: in [set_key_callback] by lilubao before
			[   13.760229] *(1)[324:charger]charger: [13760] key[116] up (was down for 0.606sec)
			
			
			连接PC的条件下，关机充电灭屏长按powerkey正常，4~5s震动

		
		
			please disable CHARGER_ENABLE_SUSPEND test, modify as below:

			/device/qcom/msm8917/BoardConfig.mk
			- BOARD_CHARGER_ENABLE_SUSPEND := true
			+ #BOARD_CHARGER_ENABLE_SUSPEND := true
			
			
			这个问题的主要原因是，灭屏时候系统休眠，这时候按power键唤醒，但是由于关机充电唤醒没有任何地方去加锁导致系统会马上休眠，
			休眠后系统的按键信息没有来的及上报到关机充电里面，导致没有检测到power键按下唤醒的动作，所以会导致长按不能重启，只有在长按10S强制S2 reset 重启，
			所以我们disable 掉关机充电的 suspend功能，由于关机充电系统是由充电器供电，所以不会影响充电电流，目前都是disable 掉关机充电的suspend功能。
			
			
			
			Case Type: Bug/Issue

			New Comment: Dear customer
			wakelock 就是给节点 sys/power/wake_lock 写加锁或者解锁, 例如，
			echo  mmm > sys/power/wake_lock  加锁锁的名字叫mmm
			echo  mmm > sys/power/wake_unlock  解锁mmm

			本质都是关机充电情况下组织系统进入深度休眠。

			thanks.

		}






		
		GNSPR#121275,开启手电筒后台播放音乐》进入游戏捕鱼电玩城后，按home键退出，点击app键卡顿，手电筒自动关闭，下拉状态栏手电筒图标显示开启，
		点击关闭后再次开启无异常 1台出现1次
		{
			首先要确定问题的log在哪个位置，跟哪些代码有关
			
			闪光灯
			drivers/media/platform/msm/camera_v2/sensor/flash/msm_flash.c
			
			相关的代码：
			kernel/msm-3.18/drivers/leds/leds-qpnp.c
			
			lut:Look Up Table参考表格
			LPG:light pulse generator
			VDD_TORCH，VDD_FLASH
			
			
			VPH_PWR can droop when the flash strobes and battery are low, or if other high current
			applications are running. If VPH_PWR droops too low, such as less than 2.5 V, UVLO might
			occur and cause the device to crash.
			To prevent UVLO events, the flash module monitors the VPH_PWR. If VPH_PWR is less than
			the VPH_PWR_DROOP_THRESHOLD, the hardware clamps the current to the programmed
			clamp current (software default is 200 mA). The VPH_PWR_DROOP_THRESHOLD software
			default is 3 V.
			If the current is less than the programmed clamp current when VPH_PWR hits the threshold, the
			current does not continue to rise to match the clamp current. Instead, it levels off until the flash
			strobe event ends.
			
			
			电源管理关掉了闪光灯
			frameworks/base/core/java/android/os/PowerManager.java
			10-08 11:51:35.249  1804  1804 E PowerManager: setTorchBrightness:0
			
			
			drivers/media/platform/msm/camera_v2/sensor/flash/msm_flash.c
			
			
			
			Run the following commands to test each flash or torch LED individually:
			adb shell "echo 255 > /sys/class/leds/led:flash_0/brightness"
			adb shell "echo 1 > /sys/class/leds/led:switch/brightness"
			
			Run the following commands to enable flash:
			adb shell "echo 255 > /sys/class/leds/led:flash_0/brightness"
			adb shell "echo 255 > /sys/class/leds/led:flash_1/brightness"
			adb shell "echo 1 > /sys/class/leds/led:switch/brightness"
			
			Run the following commands to disable flash
			adb shell "echo 0 > /sys/class/leds/led:flash_0/brightness"
			adb shell "echo 0 > /sys/class/leds/led:flash_1/brightness"
			adb shell "echo 0 > /sys/class/leds/led:switch/brightness"
			
			Run the following commands to turn on torch:
			adb shell "echo 100 > /sys/class/leds/led:torch_0/brightness"
			adb shell "echo 100 > /sys/class/leds/led:torch_1/brightnes"

			//Gionee <GN_BSP_CHG> <lilubao> <20171024> modify for torch begin
		}
		
		
		

		
	


		温升问题：
		
		GNSPR#120797,待机界面进相机 慢动作摄影,摄影过程中双指进行缩放,操作1min后一直重复提示电池温度过高,
		查看电池温度55度》退出相机后再操作恢复 验证2台,1台出现,共出现1次
		{
			2017-10-06 15:39	电池温度55度，LCD有点偏黄，插着充电器
			
			情况如何：
			    插着充电器，开摄像，反复调整焦距，一段时间后提示温度过高
		
			但是这个是压力测试，所以需要这样的强度，之前测试温度较高，紧接着做下一个测试	    
			 
			 
			请问是在室温下测试的吗？？手机整体温度在55度附近，我们的软测、硬测、包括整机测试部，目前在室温下 还没有出现这么高的情况；
			如果可以复现，请帮忙找一个debug版本，安排使用附件脚本 抓一份温升数据，谢谢！
			操作方法：
			adb push msm_tsens_logging /data/
			adb shell
			chmod 777 /data/msm_tsens_logging
			/data/msm_tsens_logging 500 10800000 &
			参数解释： 500ms的间隔，10800000--10800s运行总时间（3个小时 能够复现了吧）;

			测完后，取出数据：
			adb pull /data/tsens_logger.csv
			发给我；   
			
			
			109601 GNSPR 【品质压力】开启录屏,待机界面进相机摄像》摄像5min后提示电池温度过高》等待15min后点击确定关闭后再操作恢复 验证2台,2台出现,共出现3次 Q20 Q21 分配 高 G_国 建秋 S2-严重 W_吴 能田 2017-09-07 BJ17G16A（大金钢2）
			109632 GNSPR 【品质压力】开启录屏》待机界面进相机摄像,摄像15min后录屏自动停止(另一台未停止),等待30min后查看录屏文件,录屏中无显示摄像界面的报错提示(实际上一直有提示电池温度过高)》重复操作恢复 验证2台,2台出现,共出现2次 Q20 Q21 申请裁决 高 G_国 建秋 S2-严重 W_吴 能田 2017-09-07 BJ17G16A（大金钢2）

			目前来看 主要是 摄像负载大 热量大，导致温升较高，8940平台没有温升限制 camera 的 策略，可以考虑让camera模块 根据温度 自己控制 调节温升；
			
		
			高通给了一套限制camera及CPU的温升方案，可否安排使用版本测试一下； 到时候 可能会有一些 无响应 黑屏 卡顿情况； 主要 是 连拍和摄像 方面；不知道 camera 模块 是否愿意呢；

			[SS-CPUS]
			#algo_type ss
			sampling 50
			sensor tsens_tz_sensor9
			device cluster1
			set_point 85000
			set_point_clr 55000
			time_constant 0

			CAMERA_CAMCORDER_MONITOR]
			algo_type monitor
			sampling 250
			sensor tsens_tz_sensor3
			thresholds 80000 85000 88000
			thresholds_clr 75000 80000 85000
			actions camera+camcorder camera+camcorder camera+camcorder
			action_info 1+1 2+2 10+10
			
			
			问题属于相机和结构问题，虽然相机没有报错，
			相机是否可以优化摄像电流和功耗，如不能优化硬件和结构进行改善，

			这个问题驱动电源是无法改善的，更该会影响系统性能，请 从相机功耗优化的出发点考虑
			

			{
				msm-thermal
				
				modem thermal manage
					Level 0 – No restriction, full modem performance
					Level 1 – Requests the modem to run the data throughput reduction algorithms
					Level 2 – MTPL back off / PUCCH back off
					Level 3 – Puts the modem into Limited Service mode, in which only emergency 911
					calls allowed
					
				 thermal-engine -o
				 
				 /vendor/qcom/proprietary/thermal-engine/	
					
				80-N9649-1 Thermal Tuning Procedure	

			}
			
			
			debug
			{ 
				Enable more logging for KTM
				echo 8 > /proc/sys/kernel/printk
				echo 'file msm_thermal.c +p' > /sys/kernel/debug/dynamic_debug/control
				
				
				adb	logcat –v time –s ThermalEngine
				
				
				adb shell thermal-engine –o
				
				
				adb pull /etc/thermal-engine.conf
				adb
				remount
				<edit>
				adb push thermal-engine.conf /etc/
			
			
				adb push msm_tsens_logging /data
				adb shell
				chmod 777 /data/msm_tsens_logging
				./msm_tsens_logging a b &
				
				
				stop thermal-engine
				start thermal-engine –d
			}
	
		
		}

}






/*****************************************************************************************************/	




关机充电的流程
{
	Android Bootloader - UART_DM Initialized!!!
	[0] welcome to lk

	[10] platform_init()
	[10] target_init()


	[40] pm8x41_get_is_cold_boot: cold boot
	[40] Qseecom Init Done in Appsbl


	[90] Waiting for the RPM to populate smd channel table


	高通关机重启
	/* In case of fastboot reboot,adb reboot or if we see the power key
	* pressed we do not want go into charger mode.
	* fastboot reboot is warm boot with PON hard reset bit not set
	* adb reboot is a cold boot with PON hard reset bit set
	*/

	80-nm620-1_b_pmic_pon-reset_software_drivers_overview.pdf

	重启的类型：OTST3, KPD_PWR_N, PS_HOLD


	dVdd_rb，xVdd_rb

	pon_resin_n_reset_s1_timer


	Kernel command line: sched_enable_hmp=1 console=ttyHSL0,115200,n8 androidboot.console=ttyHSL0 androidboot.hardware=qcom msm_rtb.filter=0x237 ehci-hcd.park=3 lpm_levels.sleep_disabled=1 
	androidboot.bootdevice=7824900.sdhci earlycon=msm_hsl_uart,0x78B0000 buildvariant=eng androidboot.emmc=true androidboot.verifiedbootstate=green androidboot.veritymode=enforcing 

	androidboot.keymaster=1 androidboot.serialno=665cd078

	androidboot.mode=charger androidboot.authorized_kernel=true androidboot.baseband=msm printk.disable_uart=9 mdss_mdp.panel=1:dsi:0:qcom,mdss_dsi_ft8006m_720p_video:1:none:cfg:single_dsi

	这些参数在 （aboot.c）unsigned char *update_cmdline(const char * cmdline) , 获取cmdline
	（init.c）target_pause_for_battery_charge获取关机状态插入充电器的动作还是fastboot，adb等pon	
	init.c文件还有获取组合按键动作

	Every reset happens in different stages and each stage timer is
	configurable.

	Stage 1 – For each of the triggers, an interrupt is sent to the MSM to warn it about the
	shutdown. We can configure the amount of time (S1_Timer) the debounced trigger must
	be held before a bark is sent to the MSM using this register:
	PON_XXX_RESET_S1_TIMER.
	Stage 2 – Once the interrupt is sent to the MSM, the PMIC waits for a reply for S2_TIMER
	period before it performs the desired shutdown. We can configure the amount of time the
	debounced trigger using this register: PON_XXX_RESET_S2_TIMER.
	Stage 3 – This stage is independent of S1 and S2. If trigger is held for an extended period
	of time (default 64 sec), a forced xVdd shutdown occurs. Which means it is an abrupt
	power down and it does not follow a graceful shutdown procedure.

	PAGE 20
	This guarantees that there is a way out for unexpected hangups
	Reset all PMIC registers	


	这个地方还有healthd层的代码，这部分上层代码很重要，上传电量相关参数，还有就是按键的处理主要是powerkey
}








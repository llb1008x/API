

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
	I oldtest.TAG: ChargeActivity=>chargeCurrent
	
	aicl最后设定的最大电流
	OK, aicr upper bound
	
	
	高低温跟过压的充电使能情况
	<3>[ 6498.557056]  (1)[238:charger_thread]tmp:49 (jeita:0 sm:0 cv:0 en:0) (sm:1) en:1
	
	
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
	
}














17G10A
{
	

	
	
	GNSPR#100830,充电时按开机键开机，测试值是6.83s，标准值是4.5s，超出标准值2.33s
	{
		未插usb： 4~4.5s出logo
		枚举那段log基本上没停顿
		插usb： 7.5~8s出logo
		长按重启 枚举那段log停顿2.5~3s
		第一次开机停顿2.5~3s
		插充电器： 6~7s出logo
		枚举那段停顿0.3~0.5s左右	
		
		//Gionee <GN_BY_CHG> <lilubao> <20180101> add  for fixed #100830 begin
		加点代码，计算这段用了多少时间
		
		unsigned int time_bat_init;
		get_timer(time_bat_init)
		
		
		preloader:			
		{
			vendor/mediatek/proprietary/bootable/bootloader/preloader/platform/mt6757/src/core/main.c
			
			CFG_USB_TOOL_HANDSHAKE
			
			bldr_pre_process()  -> bldr_handshake(&handler) 
			
			usb_cable_in() -> mt_charger_type_detection() ->  hw_charger_type_detection bc1.1协议，全都是一些操作寄存器的 
			
			-> usb_connect 
			
			platform.h
			这里有两个一个是usb握手时间，一个是枚举的时间
			/* if not defined in cust_usb.h, use default setting */
			#if !defined(CFG_USB_ENUM_TIMEOUT)
			//GioneeDrv LiLuBao 20161121 modify for fixed GNSPR53131 begin         
			#define CFG_USB_ENUM_TIMEOUT            (4000)           
			//GioneeDrv LiLuBao 20161121 modify for fixed GNSPR53131 end
			#endif

			/* if not defined in cust_usb.h, use default setting */
			#if !defined(CFG_USB_HANDSHAKE_TIMEOUT)
			//GioneeDrv LiLuBao 20161121 modify for fixed GNSPR53131 begin
			#define CFG_USB_HANDSHAKE_TIMEOUT       (1500)       
			//GioneeDrv LiLuBao 20161121 modify for fixed GNSPR53131 end
			#endif
		}
		
		
		
		
		
		
		
		
		 	[   10.830067] <4>.(4)[69:pmic_thread][name:pmic_irq&][PMIC] [PMIC_INT] addr[0x854]=0x1
			[   10.831035] <4>.(4)[69:pmic_thread][name:kpd&]kpd: Power Key generate, pressed=1
			[   10.831971] <4>.(4)[69:pmic_thread][name:hal_kpd&]kpd: kpd: (pressed) HW keycode =116 using PMIC
			[   10.833102] <4>.(4)[69:pmic_thread][name:aed&](pressed) HW keycode powerkey
			[   10.834083] <4>.(4)[277:kpoc_charger]charger: key_control: event.type:1,116:1
			[   10.835000] <4>.(4)[277:kpoc_charger]charger: key_control: event.type:0,0:0
			[   10.835911] <4>.(4)[297:kpoc_charger]charger: pwr key long press check start
		 
		 
			[LIB] Loading SEC config
			[LIB] Name = 
			[LIB] Config = 0x22, 0x22
			[LIB] SECRO (ac, ac_offset, ac_length) = (0x1, 0x40, 0x40)
			0x31,0x41,0x35,0x35
			[SEC] DBGPORT 00000051 0000FFFF 00000101 00000101 0022EF45 00236705 00000051 0000FFFF 00000101 0000FFFF 00000101 00000042 00000000
			[SEC] DBGPORT (0 1)
			[SEC] DBGPORT 00000051 0000FFFF 00000101 00000101 0022EF45 00236705 00000051 0000FFFF 00000101 0000FFFF 00000101 00000042 00000000

			[SEC] read '0x8800000'
			0x4D,0x4D,0x4D,0x4D,0x4,0x0,0x0,0x0,
			[LIB] seclib_img_auth_load_sig [LIB] CFG read size '0x2000' '0x3C'
			0x4D4D4D4D
			[LIB] SEC CFG 'v4' exists
			[LIB] HW DEC
			GCPU Enhance,V1.1
			[LIB] SEC CFG is valid. Lock state is 1 

			
			//tool handshake	这段时间比较长
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
			[TOOL] Enumeration(End): OK 521ms 
			
			usbdl_flush timeoutintrep :0, IntrTx[0] IntrRx [0]usbdl_flush timeoutintrep :0, IntrTx[0] IntrRx [0]usbdl_flush timeoutintrep :0, IntrTx[0] IntrRx [0]usbdl_flush timeoutintrep :0, IntrTx[0] IntrRx [0]usbdl_flush timeoutintrep :0, IntrTx[0] IntrRx [0][TOOL] : usb listen timeout
			
			
			[TOOL] <USB> cannot detect tools!
			[TOOL] <UART> listen  ended, receive size:0!

			[TOOL] <UART> wait sync time 150ms->5ms
			[TOOL] <UART> receieved data: ()
		
	
	}
	
	
	
	
	
	
	
	
	
	
	
	

	GNSPR#139202，去除冗余log
	{
		//Gionee <GN_BY_CHG> <lilubao> <20171130> remove redundant log begin
		1.修改log等级 5 可以减少一部分
		FG_daemon_log_level 
		Enable_BATDRV_LOG	
		
		2.mt6757.dtsi
		pwrap@1000d000 {
			compatible = "mediatek,pwrap";
			reg = <0 0x1000d000 0 0x1000>;
			//Gionee <GN_BY_CHG> <lilubao> <20171130> remove redundant log begin
			interrupts = <GIC_SPI 152 IRQ_TYPE_LEVEL_HIGH>;
			//Gionee <GN_BY_CHG> <lilubao> <20171130> remove redundant log end
			mt6351_pmic: mt6351 {
				compatible = "mediatek,mt6351-pmic";
				interrupt-controller;
			};
			mt6355_pmic: mt6355 {
				compatible = "mediatek,mt6355-pmic";
				interrupt-controller;
			};
		};
		
		
		
		3.还有这个问题，irq不匹配的问题
		<6>[21329.846831]  (5)[239:charger_thread]rt5081_pmu_charger rt5081_pmu_charger: rt5081_enable_irq: (chg_mivr) en = 1
		<7>[21329.846844]  (5)[239:charger_thread]rt5081_pmu 5-0034: rt5081_pmu_reg_block_read: reg e0 size 16
		<4>[21329.846857] -(5)[239:charger_thread]------------[ cut here ]------------
		<4>[21329.846862] -(5)[239:charger_thread]WARNING: CPU: 5 PID: 239 at /data/MAIN_GIT_REPO_CODE/BJ17G10A_MAIN_REPO/L31_6757_66_N_17G10A_NO.MP5_V1.53_170512_ALPS/L31_6757_66_N_17G10A_NO.MP5_V1.53_170512_ALPS/android_mtk_mp/kernel-4.4/kernel/irq/manage.c:513 enable_irq+0x88/0xcc()
		<4>[21329.846880] -(5)[239:charger_thread]Unbalanced enable for IRQ 166
		<4>[21329.846887] -(5)[239:charger_thread]CPU: 5 PID: 239 Comm: charger_thread Tainted: G        W       4.4.15 #1
		<4>[21329.846894] -(5)[239:charger_thread]Hardware name: MT6757CD (DT)
		<0>[21329.846899] -(5)[239:charger_thread]Call trace:
		<4>[21329.846903] -(5)[239:charger_thread][<ffffffc00008a328>] dump_backtrace+0x0/0x14c
		<4>[21329.846913] -(5)[239:charger_thread][<ffffffc00008a488>] show_stack+0x14/0x1c
		<4>[21329.846919] -(5)[239:charger_thread][<ffffffc0003379b0>] dump_stack+0x8c/0xb0
		<4>[21329.846930] -(5)[239:charger_thread][<ffffffc00009e5f8>] warn_slowpath_fmt+0xc0/0xf4
		<4>[21329.846940] -(5)[239:charger_thread][<ffffffc0000fd598>] enable_irq+0x88/0xcc
		<4>[21329.846945] -(5)[239:charger_thread][<ffffffc0004d8310>] rt5081_enable_power_path+0x148/0x190
		<4>[21329.846957] -(5)[239:charger_thread][<ffffffc000945750>] charger_dev_enable_powerpath+0x24/0x34
		<4>[21329.846968] -(5)[239:charger_thread][<ffffffc000948ba0>] charger_routine_thread+0x378/0x6a8
		<4>[21329.846976] -(5)[239:charger_thread][<ffffffc0000be268>] kthread+0xdc/0xf0
		<4>[21329.846987] -(5)[239:charger_thread][<ffffffc000085cd0>] ret_from_fork+0x10/0x40
		<4>[21329.846994] -(5)[239:charger_thread]---[ end trace a4903c9f998f3193 ]---	
		
	}
	





	
	
	电量显示不准                                             
	{
		特别是低电量充电的时候
		
		MTK case ID:ALPS03624068
		{
			Dear Customer
				如电话所谈：
				[dod_init_result] HW(38596, 4779) SW(38637, 4850) OLD(38698, 4949) VBAT(37942, 3172)
				[dod_init_result]: NVRAM_ready 1 Embedded 1 plug_out 1 is_hwocv_unreliable 0 rtc_invalid 1 rtc_ui_soc 0 two_sec_reboot 0 old_data.ui_soc 4970
				[dod_init_result]: T_new 27 T_old 39 T_d0 27 shutdown_time 0 pmic_shutdown_time 1800 plugout_time 31 plugout_time_th 32 swocv_oldocv_diff_emb 1000
				[dod_init_result]: <1> 38663 38698 4970 0 0 0 0
				[dod_init_result]: fg_c_d0(38663, 4893) fg_v_d0(38698, 4950) ui_d0_soc 4970 ui_soc_opt 0
				[dod_init_result]: force_vc_mode 0 vc_mode 0 d0_sel 0

				从log：[dod_init_result]: <1> 38663 38698 4970 0 0 0 0 可以看出，开机定位到第1路，即采用关机之前NVRAM里面的old_data。所以就5%这个点来说，
				判断到HW_OCV-OLD_SOC_TO_OCV<30mv，init flow就会认为是同一颗电池，就会拿NVRAM里面的值来进行开机电量的定位，保持5%。
				这样做的原因是我们不希望同一颗电池在拔出和插入之后，电量会有跳变。如果判定为同一颗电池，不会根据HW_OCV去定位新的电量。
				你们可以通过[dod_init_result] HW(38596, 4779) SW(38637, 4850) OLD(38698, 4949) VBAT(37942, 3172) 里面的HW_OCV 的值去和你们测量的OCV的值做对比，这个值显示真实的电量，看两者之间的差距。
				如果按照电话中您所说，希望开机会尽量去逼近真实的电量，一定要通过HW_OCV去重新定位开机电量，这个时候定位的电量虽然比较准确，但是就会有很大的概率发生UI电量的跳变（尤其是在电量比较低的情况下）。
				所以这就需要你们选择：是要UI比较平滑，客户看不到UI的跳变，认为电量比较准确，还是要UI开机时比较准确的显示电池的真实电量，但是会有很大的概率发生UI跳变。


				关于dod_init的大概flow和GM30的测试手法，我已经申请，等有结果我会告诉您。
				如果您有其他的问题，轻提出。

				感谢！
				
				
			如电话所谈：如果想修改仅为测试，可以修改mtk_battery_property.h里面的BAT_PLUG_OUT_TIME，改为30。这样修改后，开机就会根据hw_ocv的值重新定位开机电量，所以可能会引起UI的跳变。	
		
		
		
				实际判断电量到底准不准，应该用底层的SOC与查表得到的值进行对比，而不应该采用UI_SOC与查表的值进行对比。因为充电时，

			电流比较大，怀疑底层的SOC与UI_SOC有比较大的偏差，因为在算法上UI_SOC会在SOC的基础上作一些藏百分比的行为。建议先看Log。

			 

			1、进行实验，例如充电到UI_SOC到10%时，把Log导入出来，查看此时对应的SOC大小。

			2、再静置电池30分钟，查表得出值，与SOC大小进行比较。

			 

			搜索MTK_FG: [FGADC_intr_end]，如下方红色标注，可看出SOC、UI_SOC分别大小，看下他们的差距。

			MTK_FG: [FGADC_intr_end][FG_INTR_SHUTDOWN]soc:503 fg_c_soc:2184 fg_v_soc:503 ui_soc:348 vc_diff:1681 vc_mode 0 VBAT 33730 T:[11 V 10 C 17] D0_C 10000 D0_V 10000 CAR_V -37026 Q:[38986 38986] aging 10000 bat_cycle 0 Trk[0(-201):1:0] UI[0:1] Chr[0:10000:9978] pseudo1 301  DC_ratio 100

		
		
		

		
			定位开机的dod_init
			12-09 17:31:09.029360     0     0 E [   40.798592][dod_init_result]: <5> 39834 39834 6210 1 0 1 1

			5 代表第五路，用hw_ocv重新点位D0,后面三个数字c_ocv=hw_ocv=39834，v_ocv=sw_ocv=hw_ocv=39834，UI=ocv_to_soc(hw_ocv)

			标红的0 代表adc_reset没有发生，1代表vc_mode为c_mode，知道这两个就可以了

			 

			12-09 16:58:54.382134     0     0 E [  341.016964][dod_init_result]: <1> 38052 38169 3258 0 0 1 0

			1代表定位到第1路开机定位采用old_data的值，UI保持与关机前的一致。标红0 1 和上一条一致




			//Gionee <GN_BY_CHG> <lilubao> <20171127> add for debug soc begin
			
			
			 debug:
			 {
			 adb pull  /sdcard/mtklog/mobilelog    /home/llb/project/DEBUG/1.test_data/5.电量显示/log2/
			 
			  1. dod_init_result  这个定位的后面判断是几个类型是什么？还有后面的参数分别是什么意思
			12-09 17:31:09.029360     0     0 E [   40.798592][dod_init_result]: <5> 39834 39834 6210 1 0 1 1
			12-09 16:58:54.382134     0     0 E [  341.016964][dod_init_result]: <1> 38052 38169 3258 0 0 1 0

			   2.手机里面最后用的table 是mt6757_battery_table_ext.dtsi 还是哪个？我看log里面有一个
			fg_dump_table 跟之前zcv table不一样

			   3.
			我根据car的变化算流过电量计的电量，就是delta UI soc 的变化跟库伦计的变化量接近，但是delta soc的变化不同，请问，这两个soc，我看之前的
			文档说GM3.0的 电量计算 coulomb count + voltage 算的，这个soc是根据什么算
			2-09 17:34:13.192588   480   480 E [  122.151072] (0)[480:fuelgauged]MTK_FG: [FGADC_intr_end]: [FG_INTR_COULOMB_C]soc:6252 fg_c_soc:6252 fg_v_soc:6091 ui_soc:6382 vc_diff:161 vc_mode 1 VBAT 43190 T:[33 V 32 C 28] D0_C 6210 D0_V 6091 CAR_V 0 Q:[28155 28155] aging 10000 bat_cycle 0 Trk[0:0:0] UI[0:0] Chr[1:0:0] pseudo1 0  DC_ratio 100!

			12-09 17:31:09.024791   186   186 E [   40.794023] (0)[186:battery_thread]: [fg_drv_update_hw_status] current:9071 3166 state:0 0 car:-58 -76 bat:3822 3918 chr:0 0 hwocv:1234 1234 bat_plug_out:1 1 tmp:29 29 imix 24070 rac 436

				
				
				
				
		01-01 08:00:26.827611   483   483 E [    8.402427] (3)[483:fuelgauged]: MTK_FG: [fg_dump_table]table idx:255 (i,mah,voltage,resistance,percentage)
		01-01 08:00:26.827670   483   483 E [    8.402486] (3)[483:fuelgauged]: MTK_FG:  ( 0,    0,43850, 1510,  0) ( 1,  305,43705, 1509,105) ( 2,  609,43577, 1513,210) ( 3,  914,43460, 1509,315) ( 4, 1219,43343, 1516,420)
		01-01 08:00:26.827730   483   483 E [    8.402546] (3)[483:fuelgauged]: MTK_FG:  ( 5, 1524,43228, 1519,526) ( 6, 1828,43109, 1515,631) ( 7, 2133,42990, 1515,736) ( 8, 2438,42870, 1515,841) ( 9, 2742,42750, 1519,946)
		01-01 08:00:26.827791   483   483 E [    8.402607] (3)[483:fuelgauged]: MTK_FG:  (10, 3047,42630, 1528,1051) (11, 3352,42512, 1522,1157) (12, 3657,42394, 1525,1262) (13, 3961,42277, 1533,1367) (14, 4266,42160, 1553,1472)
		01-01 08:00:26.827851   483   483 E [    8.402667] (3)[483:fuelgauged]: MTK_FG:  (15, 4571,42042, 1568,1577) (16, 4875,41925, 1575,1682) (17, 5180,41809, 1588,1788) (18, 5485,41694, 1595,1893) (19, 5790,41580, 1600,1998)
		01-01 08:00:26.827913   483   483 E [    8.402729] (3)[483:fuelgauged]: MTK_FG:  (20, 6094,41467, 1609,2103) (21, 6399,41356, 1618,2208) (22, 6704,41245, 1628,2314) (23, 7008,41134, 1636,2419) (24, 7313,41025, 1643,2524)
		01-01 08:00:26.827975   483   483 E [    8.402791] (3)[483:fuelgauged]: MTK_FG:  (25, 7618,40918, 1651,2629) (26, 7923,40811, 1647,2735) (27, 8227,40708, 1653,2840) (28, 8532,40610, 1677,2945) (29, 8837,40516, 1712,3050)
		01-01 08:00:26.828037   483   483 E [    8.402853] (3)[483:fuelgauged]: MTK_FG:  (30, 9142,40415, 1723,3155) (31, 9446,40306, 1741,3260) (32, 9751,40198, 1747,3366) (33,10056,40095, 1758,3471) (34,10360,40004, 1774,3576)
		01-01 08:00:26.828097   483   483 E [    8.402913] (3)[483:fuelgauged]: MTK_FG:  (35,10665,39922, 1788,3681) (36,10970,39842, 1802,3786) (37,11275,39762, 1822,3892) (38,11579,39679, 1827,3997) (39,11884,39592, 1839,4102)
		01-01 08:00:26.828160   483   483 E [    8.402976] (3)[483:fuelgauged]: MTK_FG:  (40,12189,39501, 1836,4207) (41,12493,39403, 1816,4312) (42,12798,39289, 1762,4417) (43,13103,39158, 1681,4523) (44,13408,39030, 1604,4628)
		01-01 08:00:26.828221   483   483 E [    8.403037] (3)[483:fuelgauged]: MTK_FG:  (45,13712,38923, 1558,4733) (46,14017,38836, 1536,4838) (47,14322,38759, 1520,4944) (48,14626,38690, 1509,5049) (49,14931,38626, 1507,5154)
		01-01 08:00:26.828282   483   483 E [    8.403098] (3)[483:fuelgauged]: MTK_FG:  (50,15236,38566, 1505,5259) (51,15541,38511, 1502,5364) (52,15845,38457, 1506,5469) (53,16150,38405, 1511,5575) (54,16455,38356, 1511,5680)
		01-01 08:00:26.828344   483   483 E [    8.403160] (3)[483:fuelgauged]: MTK_FG:  (55,16759,38308, 1513,5785) (56,17064,38263, 1523,5890) (57,17369,38220, 1531,5995) (58,17674,38179, 1536,6101) (59,17978,38139, 1543,6206)
		01-01 08:00:26.828407   483   483 E [    8.403223] (3)[483:fuelgauged]: MTK_FG:  (60,18283,38101, 1550,6311) (61,18588,38065, 1564,6416) (62,18892,38029, 1573,6521) (63,19197,37996, 1576,6626) (64,19502,37964, 1587,6732)
		01-01 08:00:26.828469   483   483 E [    8.403285] (3)[483:fuelgauged]: MTK_FG:  (65,19807,37934, 1589,6837) (66,20111,37902, 1596,6942) (67,20416,37877, 1609,7047) (68,20721,37846, 1612,7153) (69,21025,37813, 1611,7258)
		01-01 08:00:26.828532   483   483 E [    8.403348] (3)[483:fuelgauged]: MTK_FG:  (70,21330,37776, 1588,7363) (71,21635,37734, 1568,7468) (72,21940,37683, 1547,7573) (73,22244,37640, 1531,7678) (74,22549,37597, 1531,7784)
		01-01 08:00:26.828594   483   483 E [    8.403410] (3)[483:fuelgauged]: MTK_FG:  (75,22854,37552, 1523,7889) (76,23159,37508, 1513,7994) (77,23463,37468, 1515,8099) (78,23768,37432, 1521,8204) (79,24073,37401, 1537,8310)
		01-01 08:00:26.828657   483   483 E [    8.403473] (3)[483:fuelgauged]: MTK_FG:  (80,24377,37365, 1553,8415) (81,24682,37320, 1555,8520) (82,24987,37264, 1550,8625) (83,25292,37201, 1539,8731) (84,25596,37143, 1542,8835)
		01-01 08:00:26.828720   483   483 E [    8.403536] (3)[483:fuelgauged]: MTK_FG:  (85,25901,37087, 1561,8941) (86,26206,37016, 1563,9046) (87,26510,36938, 1540,9151) (88,26815,36895, 1539,9256) (89,27120,36879, 1562,9362)
		01-01 08:00:26.828783   483   483 E [    8.403599] (3)[483:fuelgauged]: MTK_FG:  (90,27425,36866, 1593,9467) (91,27729,36853, 1636,9572) (92,28034,36832, 1692,9677) (93,28339,36790, 1753,9782) (94,28643,36645, 1751,9887)
		01-01 08:00:26.828846   483   483 E [    8.403662] (3)[483:fuelgauged]: MTK_FG:  (95,28948,36290, 1714,9993) (96,29253,35797, 1774,10098) (97,29558,35140, 1888,10203) (98,29862,34209, 2085,10308) (99,30167,32634, 2726,10413)
		01-01 08:00:26.828963   483   483 E [    8.403779] (3)[483:fuelgauged]: MTK_FG: [fg_dump_table]table idx:254 (i,mah,voltage,resistance,percentage)
		01-01 08:00:26.829030   483   483 E [    8.403846] (3)[483:fuelgauged]: MTK_FG:  ( 0,    0,43850, 1510,  0) ( 1,  305,43705, 1509,105) ( 2,  609,43577, 1513,210) ( 3,  914,43460, 1509,315) ( 4, 1219,43343, 1516,420)
		01-01 08:00:26.829089   483   483 E [    8.403905] (3)[483:fuelgauged]: MTK_FG:  ( 5, 1524,43228, 1519,526) ( 6, 1828,43109, 1515,631) ( 7, 2133,42990, 1515,736) ( 8, 2438,42870, 1515,841) ( 9, 2742,42750, 1519,946)
		01-01 08:00:26.829149   483   483 E [    8.403965] (3)[483:fuelgauged]: MTK_FG:  (10, 3047,42630, 1528,1051) (11, 3352,42512, 1522,1157) (12, 3657,42394, 1525,1262) (13, 3961,42277, 1533,1367) (14, 4266,42160, 1553,1472)
		01-01 08:00:26.829211   483   483 E [    8.404027] (3)[483:fuelgauged]: MTK_FG:  (15, 4571,42042, 1568,1577) (16, 4875,41925, 1575,1682) (17, 5180,41809, 1588,1788) (18, 5485,41694, 1595,1893) (19, 5790,41580, 1600,1998)
		01-01 08:00:26.829277   483   483 E [    8.404093] (3)[483:fuelgauged]: MTK_FG:  (20, 6094,41467, 1609,2103) (21, 6399,41356, 1618,2208) (22, 6704,41245, 1628,2314) (23, 7008,41134, 1636,2419) (24, 7313,41025, 1643,2524)
		01-01 08:00:26.829338   483   483 E [    8.404154] (3)[483:fuelgauged]: MTK_FG:  (25, 7618,40918, 1651,2629) (26, 7923,40811, 1647,2735) (27, 8227,40708, 1653,2840) (28, 8532,40610, 1677,2945) (29, 8837,40516, 1712,3050)
		01-01 08:00:26.829399   483   483 E [    8.404215] (3)[483:fuelgauged]: MTK_FG:  (30, 9142,40415, 1723,3155) (31, 9446,40306, 1741,3260) (32, 9751,40198, 1747,3366) (33,10056,40095, 1758,3471) (34,10360,40004, 1774,3576)
		01-01 08:00:26.829460   483   483 E [    8.404276] (3)[483:fuelgauged]: MTK_FG:  (35,10665,39922, 1788,3681) (36,10970,39842, 1802,3786) (37,11275,39762, 1822,3892) (38,11579,39679, 1827,3997) (39,11884,39592, 1839,4102)
		01-01 08:00:26.829521   483   483 E [    8.404337] (3)[483:fuelgauged]: MTK_FG:  (40,12189,39501, 1836,4207) (41,12493,39403, 1816,4312) (42,12798,39289, 1762,4417) (43,13103,39158, 1681,4523) (44,13408,39030, 1604,4628)
		01-01 08:00:26.829589   483   483 E [    8.404405] (3)[483:fuelgauged]: MTK_FG:  (45,13712,38923, 1558,4733) (46,14017,38836, 1536,4838) (47,14322,38759, 1520,4944) (48,14626,38690, 1509,5049) (49,14931,38626, 1507,5154)
		01-01 08:00:26.829650   483   483 E [    8.404466] (3)[483:fuelgauged]: MTK_FG:  (50,15236,38566, 1505,5259) (51,15541,38511, 1502,5364) (52,15845,38457, 1506,5469) (53,16150,38405, 1511,5575) (54,16455,38356, 1511,5680)
		01-01 08:00:26.829713   483   483 E [    8.404529] (3)[483:fuelgauged]: MTK_FG:  (55,16759,38308, 1513,5785) (56,17064,38263, 1523,5890) (57,17369,38220, 1531,5995) (58,17674,38179, 1536,6101) (59,17978,38139, 1543,6206)
		01-01 08:00:26.829775   483   483 E [    8.404591] (3)[483:fuelgauged]: MTK_FG:  (60,18283,38101, 1550,6311) (61,18588,38065, 1564,6416) (62,18892,38029, 1573,6521) (63,19197,37996, 1576,6626) (64,19502,37964, 1587,6732)
		01-01 08:00:26.829837   483   483 E [    8.404653] (3)[483:fuelgauged]: MTK_FG:  (65,19807,37934, 1589,6837) (66,20111,37902, 1596,6942) (67,20416,37877, 1609,7047) (68,20721,37846, 1612,7153) (69,21025,37813, 1611,7258)
		01-01 08:00:26.829899   483   483 E [    8.404715] (3)[483:fuelgauged]: MTK_FG:  (70,21330,37776, 1588,7363) (71,21635,37734, 1568,7468) (72,21940,37683, 1547,7573) (73,22244,37640, 1531,7678) (74,22549,37597, 1531,7784)
		01-01 08:00:26.829962   483   483 E [    8.404778] (3)[483:fuelgauged]: MTK_FG:  (75,22854,37552, 1523,7889) (76,23159,37508, 1513,7994) (77,23463,37468, 1515,8099) (78,23768,37432, 1521,8204) (79,24073,37401, 1537,8310)
		01-01 08:00:26.830025   483   483 E [    8.404841] (3)[483:fuelgauged]: MTK_FG:  (80,24377,37365, 1553,8415) (81,24682,37320, 1555,8520) (82,24987,37264, 1550,8625) (83,25292,37201, 1539,8731) (84,25596,37143, 1542,8835)
		01-01 08:00:26.830087   483   483 E [    8.404903] (3)[483:fuelgauged]: MTK_FG:  (85,25901,37087, 1561,8941) (86,26206,37016, 1563,9046) (87,26510,36938, 1540,9151) (88,26815,36895, 1539,9256) (89,27120,36879, 1562,9362)
		01-01 08:00:26.830148   483   483 E [    8.404964] (3)[483:fuelgauged]: MTK_FG:  (90,27425,36866, 1593,9467) (91,27729,36853, 1636,9572) (92,28034,36832, 1692,9677) (93,28339,36790, 1753,9782) (94,28643,36645, 1751,9887)
		01-01 08:00:26.830212   483   483 E [    8.405028] (3)[483:fuelgauged]: MTK_FG:  (95,28948,36290, 1714,9993) (96,29253,35797, 1774,10098) (97,29558,35140, 1888,10203) (98,29862,34209, 2085,10308) (99,30167,32634, 2726,10413)
			 
			 	
			 }	

		}
	
	}	





	
	GNSPR#101075，待机界面进入相机，相机黑屏5s后自动恢复
	{
		压力测试,之前的温度已经很高了
		106386
		
		case ID:ALPS03628177
		{
			Dear customer
				您的意思是压力测试，手机升温后此问题才会发生么？那有可能是升温后，cpu性能被限制打出的。
			您帮忙抓一份userdebug版本的systrace+mtklog，正常与不正常时候的对比log给我们check下。感谢
		
		
			Dear customer
				抓systrace的方式可以上我司DCC下载文档How_to_use_ftrace_to_Customer.pptx 后面几页有介绍具体操作步骤：
				基本上来说就是使用我司ftrace_all_in_one工具，
				1、设置tag，文档中介绍的是手动使用adb命令设置。我给您release一个bat文件，设置camera需要的tag，您在抓取之前直接先运行此bat文件就好。
				2、运行catch.bat
				3、复现问题
				4、停止抓取
				具体操作请参考文档中的说明（请帮忙使用userdebug版本的load）。

				感谢
				
			复测步骤
			{
				2.
				MTK 获取root
				– adb shell setprop service.adb.root 1
				– 进入Settings->Developer options,找到USB
				Debugging,将此选项先取消勾选,并再次勾选上

				3.adb root 查看是否已经有了root权限

				4.解压压缩包里软件，

				5.测试之前开启mobilelog
				先运行 systrace-setup-tk-camera-only.bat，这个是设置需要抓取的log

				6.然后运行01-catch.bat，按照press any key开始抓log，

				7.然后复测这个case

				8.再press any key 停止抓取ftrace

				9.把这个目录下的SYS_TRACE和cputime.txt ，moblelog都发给我
			}		
		
		}

	}








	



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


}	











18Y07A
{
    还有一个otg功能好像还有问题，需要抽时间调试一下 
}
























17G08A
{

	
	3.U盘识别时间过长 (32s)        暂时先不跟
	{
		usb眼图通过，同平台其他无此问题，
		本地验证了，大概4~6此出现一次不识别或识别慢的问题，不是必现的
		
		//Gionee <GN_BY_CHG> <lilubao> <20171220> add for debug usb begin
		
		case ID:03272648,03264824
		
			1.
			Many of the information provided in the log is not printed.

			Please provide the log after adding the following code.
			drivers/usb/gadget/configfs.c
			drivers/usb/dwc3/dwc3-msm.c
			drivers/usb/dwc3/gadget.c
			kernel/drivers/usb/core/hub.c
			#undef dev_dbg
			#undef pr_debug
			#define dev_dbg dev_err
			#define pr_debug pr_err

			drivers/usb/gadget/function/f_mtp.c
			#undef DBG
			#undef VDBG
			#define VDBG ERROR
			#define DBG ERROR

			drivers/usb/gadget/composite.c
			#undef INFO
			#undef DBG
			#undef pr_debug
			#define INFO ERROR
			#define DBG ERROR
			#define pr_debug pr_err

			diff --git a/drivers/usb/gadget/configfs.c b/drivers/usb/gadget/configfs.c
			index fa05070..0ebdb2f 100644
			--- a/drivers/usb/gadget/configfs.c
			+++ b/drivers/usb/gadget/configfs.c
			@@ -443,6 +443,7 @@ static int config_usb_cfg_link(
			goto out;
			}

			+ pr_info("[oem] %s: func:%s\n", _func_, f->name);
			/ stash the function until we bind it to the gadget /
			list_add_tail(&f->list, &cfg->func_list);
			ret = 0; 
			
			
			See some exception information in log.
			<3>[ 561.336568] msm-dwc3 a800000.ssusb: set core clk rate 133333333
			<3>[ 561.507213] msm-dwc3 a800000.ssusb: block_reset ASSERT
			<3>[ 561.508793] msm-dwc3 a800000.ssusb: block_reset DEASSERT
			<3>[ 561.526782] msm-dwc3 a800000.ssusb: DWC3_CONTROLLER_RESET_EVENT received
			<3>[ 561.526820] msm-dwc3 a800000.ssusb: DWC3_CONTROLLER_POST_RESET_EVENT received
			<3>[ 561.546967] msm-dwc3 a800000.ssusb: b_idle state
			<3>[ 561.558286] msm-dwc3 a800000.ssusb: Cable disconnected

			Does the debug switch of hub.c have not been opened? No information was seen.

			-------------------------------------------------------------------------------
			2.
			Please provide the log after adding the following code. Do not remove the dbug switch that has been added before.
			To improve efficiency, please leave your phone number.

			drivers/usb/core/hub.c
			#undef dev_dbg
			#undef pr_debug
			#define dev_dbg dev_err
			#define pr_debug pr_err

			drivers/usb/host/xhci.h

			#define xhci_dbg(xhci, fmt, args...) \
			- dev_dbg(xhci_to_hcd(xhci)->self.controller , fmt , ## args)
			+ dev_err(xhci_to_hcd(xhci)->self.controller , fmt , ## args) 
			
			
			------------------------------------------------------------------------------
			3.
			Please add the following modifications and test.

			static void
			sd_spinup_disk(struct scsi_disk *sdkp)
			{
			unsigned char cmd[10];
			unsigned long spintime_expire = 0;
			int retries, spintime;
			unsigned int the_result;
			struct scsi_sense_hdr sshdr;
			int sense_valid = 0;

			spintime = 0;

			/* Spin up drives, as required. Only do this at boot time */
			/* Spinup needs to be done for module loads too. */
			do {
			retries = 0;

			do {
			cmd[0] = TEST_UNIT_READY;
			memset((void *) &cmd[1], 0, 9);

			the_result = scsi_execute_req(sdkp->device, cmd,
			DMA_NONE, NULL, 0,
			&sshdr, SD_TIMEOUT,
			SD_MAX_RETRIES, NULL);

			/*
			* If the drive has indicated to us that it
			* doesn't have any media in it, don't bother
			* with any more polling.
			*/
			+if(retries >25)
			+{
			if (media_not_present(sdkp, &sshdr))
			return;
			+}
			if (the_result)
			sense_valid = scsi_sense_valid(&sshdr);
			retries++;
			- } while (retries < 3 &&
			+ } while (retries < 30 &&
			(!scsi_status_is_good(the_result) ||
			((driver_byte(the_result) & DRIVER_SENSE) &&
			sense_valid && sshdr.sense_key == UNIT_ATTENTION)));

			Thanks!
	
	}
	


}	
	








msm8917 替换IC
{
	刘工 你好

	请看下面我们产品线负责软件的工程师的说明

	谢谢


	MSM8917平台的USB  phy可以做BC1.2的检测，usb phy驱动在drivers/usb/phy/phy-msm-usb.c文件

	 

	Charger驱动只要在检测到Adapter/USB插入后调用power_supply_set_present(usb, true)通知usb phy驱动去启动BC1.2的检测，
	检测完毕后会回调charger驱动的external_power_changed函数。

	针对使用bq24157的情况，我们会使用平台CBLPWR_N信号作为Adapter/USB插入判断，CBLPWR_N中断会调用bq2415x_cblpwr_changed()函数，
	这个函数schdeule delay work，执行bq2415x_vbus_changed_workfunc函数，

	在这个函数里会读取cblpwr_n的状态qpnp_pon_get_cblpwr_status(&bq->power_good); 让后根据cblpwr_n的状态判断adapter是插入还是拔出，
	然后调用power_supply_set_present 通知USB phy驱动
	
	
}














M2018
{
	1.调试PD协议是否通
	{
		1.相关的代码
		{
			相关的宏：
			CONFIG_USB_PD_POLICY=y
			CONFIG_QPNP_USB_PDPHY=y
	
			相关的代码：
			drivers/usb/pd/
				policy_engine.c
				qpnp-pdphy.c
			
			/sys/kernel/debug/ipc_logging/usb_pd
		
			cat /d/ipc_logging/usb_pd/log
			cat /sys/class/usbpd/usbpd0
			
			
			
			
			1.msm-pm660.dsti
			pm660_charger: qcom,qpnp-smb2
				qcom,micro-usb;
				类型应该是type-c
				
			pm660_pdphy: qcom,usb-pdphy@1700 {
					compatible = "qcom,qpnp-pdphy";
					reg = <0x1700 0x100>;
					vdd-pdphy-supply = <&pm660l_l7>;
					vbus-supply = <&smb2_vbus>;
					vconn-supply = <&smb2_vconn>;

					status = "ok";
					
			2.policy_engine.c
				module_param(disable_usb_pd, bool, S_IRUGO|S_IWUSR);
				模块传参 应该不是disable的	
				
				
			pd的投票
	
				pd_allowed_votable:
						PD_DISALLOWED_INDIRECT_VOTER
						PD_VOTER
						CC_DETACHED_VOTER
						
				pd_disallowed_votable_indirect:
						HVDCP_TIMEOUT_VOTER
						LEGACY_CABLE_VOTER
						VBUS_CC_SHORT_VOTER
	
			/* reset power delivery voters */
			vote(chg->pd_allowed_votable, PD_VOTER, false, 0);
			vote(chg->pd_disallowed_votable_indirect, CC_DETACHED_VOTER, true, 0);
			vote(chg->pd_disallowed_votable_indirect, HVDCP_TIMEOUT_VOTER, true, 0);


			1.Enable PD negotiation when the Type-C cable is inserted and HVDCP
			detection timeout
			
			smblib_handle_hvdcp_check_timeout(chg,(bool)(stat & HVDCP_CHECK_TIMEOUT_BIT),(bool)(stat & QC_CHARGER_BIT));
			SCHG_USB_APSD_STATUS -> 6 HVDCP_CHECK_TIMEOUT

		
			2.PD negotiation works now
			
			smb-lib.c
			int smblib_get_pe_start(struct smb_charger *chg,
		       union power_supply_propval *val)
			{
				/*
				 * hvdcp timeout voter is the last one to allow pd. Use its vote
				 * to indicate start of pe engine
				 */
				val->intval
					= !get_client_vote_locked(chg->pd_disallowed_votable_indirect,
						HVDCP_TIMEOUT_VOTER);
				return 0;
			}
			
			policy_engine.c
			
			
			这个函数应该是比较重要的,设置pd的状态，有很多投票
			smblib_set_prop_pd_active
			
			
			相关的寄存器
			TYPE_C_STATUS_4_REG							0x0000130E
			TYPE_C_INTRPT_ENB_SOFTWARE_CTRL_REG			0x00001368
			
			pd 的状态
			enum usbpd_state {
				PE_UNKNOWN,
				PE_ERROR_RECOVERY,
				PE_SRC_DISABLED,
				PE_SRC_STARTUP,
				PE_SRC_SEND_CAPABILITIES,
				PE_SRC_SEND_CAPABILITIES_WAIT, /* substate to wait for Request */
				PE_SRC_NEGOTIATE_CAPABILITY,
				PE_SRC_TRANSITION_SUPPLY,
				PE_SRC_READY,
				PE_SRC_HARD_RESET,
				PE_SRC_SOFT_RESET,
				PE_SRC_SEND_SOFT_RESET,
				PE_SRC_DISCOVERY,
				PE_SRC_TRANSITION_TO_DEFAULT,
				PE_SNK_STARTUP,
				PE_SNK_DISCOVERY,
				PE_SNK_WAIT_FOR_CAPABILITIES,
				PE_SNK_EVALUATE_CAPABILITY,
				PE_SNK_SELECT_CAPABILITY,
				PE_SNK_TRANSITION_SINK,
				PE_SNK_READY,
				PE_SNK_HARD_RESET,
				PE_SNK_SOFT_RESET,
				PE_SNK_SEND_SOFT_RESET,
				PE_SNK_TRANSITION_TO_DEFAULT,
				PE_DRS_SEND_DR_SWAP,
				PE_PRS_SNK_SRC_SEND_SWAP,
				PE_PRS_SNK_SRC_TRANSITION_TO_OFF,
				PE_PRS_SNK_SRC_SOURCE_ON,
				PE_PRS_SRC_SNK_SEND_SWAP,
				PE_PRS_SRC_SNK_TRANSITION_TO_OFF,
				PE_PRS_SRC_SNK_WAIT_SOURCE_ON,
				PE_VCS_WAIT_FOR_VCONN,
			};
			
			
			type-c的状态
			/* Indicates USB Type-C CC connection status */
			enum power_supply_typec_mode {
				POWER_SUPPLY_TYPEC_NONE,

				/* Acting as source */
				POWER_SUPPLY_TYPEC_SINK,			/* Rd only */
				POWER_SUPPLY_TYPEC_SINK_POWERED_CABLE,		/* Rd/Ra */
				POWER_SUPPLY_TYPEC_SINK_DEBUG_ACCESSORY,	/* Rd/Rd */
				POWER_SUPPLY_TYPEC_SINK_AUDIO_ADAPTER,		/* Ra/Ra */
				POWER_SUPPLY_TYPEC_POWERED_CABLE_ONLY,		/* Ra only */

				/* Acting as sink */
				POWER_SUPPLY_TYPEC_SOURCE_DEFAULT,		/* Rp default */
				POWER_SUPPLY_TYPEC_SOURCE_MEDIUM,		/* Rp 1.5A */
				POWER_SUPPLY_TYPEC_SOURCE_HIGH,			/* Rp 3A */
				POWER_SUPPLY_TYPEC_NON_COMPLIANT,
			};
			
			//Gionee <GN_BY_CHG> <lilubao> <20180101> add for debug usb pd begin	

		}
	
	
		
		PD3.0/PPS/or QC4.0	了解一下pps的一些内容，干什么的，有什么要求
		{
			The CC line communication between Sink and Source will look like below :

				1. The Sink will first request Source capabilities.

				2. The Source will provide its source capabilities.

				3. The Sink will then request a suitable power profile from the Source capability list.

				4. The Source will accept the request and start to make changes to the BUS voltage. The Sink will minimize the bus loading during BUS voltage change. The Source will increase the VBUS voltage with a defined slew rate.

				5. After the BUS voltage has reached its final value, the source will wait until the BUS has stabilized and then will send a Power Supply Ready signal. The Sink will now increase the BUS loading again. 
				
				
				Bi-phase Mark Code (BMC) 这个是什么编码方式
				The BMC data can be decoded by USB PD decoders like Ellisys EX350 Analyzer
		
		}
		
	}


}
	
	






	
17G16A
{
	GNSPR#139252,【待机耗电】手机满电100%，插一张移动卡（卡未欠费，手机一直是搜网状态），
	开启数据业务，连接WIFI-AP，待机15小时，耗电19%
	{
		msm_otg 在待机时候一直持锁，wakelock
		
		msm_otg这个锁在pyh-msm-usb.c  申请了锁
		{

			wake_lock_init(&motg->wlock, WAKE_LOCK_SUSPEND, "msm_otg");
			
			关键字是：
			motg->wlock 
			
			两个相关的宏：
			CONFIG_PM_RUNTIME
			CONFIG_PM_SLEEP
		
			probe:
			
				device_init_wakeup(&pdev->dev, 1);
			
			
				/*
				 * PHY DVDD is supplied by a always on PMIC LDO (unlike
				 * vddcx/vddmx). PHY can keep D+ pull-up and D+/D-
				 * pull-down during suspend without any additional
				 * hardware re-work.
				 */
				if (motg->pdata->phy_type == SNPS_FEMTO_PHY)
					motg->caps |= ALLOW_BUS_SUSPEND_WITHOUT_REWORK;

				wake_lock(&motg->wlock);
			
			
				destroy_wlock:
				wake_lock_destroy(&motg->wlock);
		
			释放锁只有在suspend里面
			suspend：
				wake_unlock(&motg->wlock);
				
			待机时候一直持锁，就是没有进入suspend	
			
			
			<6>[  265.291149] *5)Calling platform_pm_suspend taking 3.309 msecs (c051000.sound)
			<6>[  265.291162] *5)        |- snd_soc_suspend
			<6>[  265.302587] *6)Calling mmc_bus_suspend taking 10.865 msecs (mmc0:0001)
			<3>[  265.303008] *6)dpm_run_callback(): platform_pm_suspend+0x0/0x50 returns -16
			<3>[  265.303018] *6)PM: Device alarmtimer failed to suspend: error -16
			<3>[  265.303026] *6)PM: Some devices failed to suspend, or early wake event detected
			<6>[  265.305296] *6)PM: resume of devices complete after 2.257 msecs
			
			
			CONFIG_PM_AUTOSLEEP=y
			CONFIG_PM_WAKELOCKS=y
			CONFIG_PM_WAKELOCKS_LIMIT=0
			# CONFIG_PM_WAKELOCKS_GC is not set
			CONFIG_PM_RUNTIME=y
			CONFIG_PM_DEBUG=y
			CONFIG_SUSPEND_TIME=y
			
			
			
			//Gionee <GN_BY_CHG> <lilubao> <20171205> add for debug otg wake lock begin
			
			
			adb root
			adb shell

			# Set the events to trace.
			echo "power:wakeup_source_activate" >> /d/tracing/set_event
			echo "power:wakeup_source_deactivate" >> /d/tracing/set_event

			# The default trace size for most devices is 1MB, which is relatively low and might cause the logs to overflow.
			# 8MB to 10MB should be a decent size for 5-6 hours of logging.

			echo 8192 > /d/tracing/buffer_size_kb

			echo 1 > /d/tracing/tracing_on


			
			adb shell dumpsys batterystats --reset
			adb shell dumpsys batterystats --enable full-wake-history
			
			
			
			echo 1 > /sys/module/msm_show_resume_irq/parameters/debug_mask
			echo 1 > /sys/kernel/debug/clk/debug_suspend
			echo 8 > /sys/module/mpm_of/parameters/debug_mask
			echo 32 > /sys/module/msm_pm/parameters/debug_mask

			CONFIG_USB_OTG_WAKELOCK
			
			申请专项测试
		
		}
		
	}

}	
	
	
	
	
	
	




/*********************************************************************************************************************************/
17G06A
{



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

}






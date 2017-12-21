/*********************************************************************************
		毕业后第一份工作的第一个正式项目，一定要认真细致负责，做好每一个环节
**********************************************************************************/

/*注意事项*/
{


	由于目前大家手里面有17G05A（前期领到的整机和板子），17G05Q（板子上能看到丝印，17G05A QORVO），以及射频工程师手里的17G05Q+PA77010的机器。并且modem不能兼容。
		所以，为了满足17G10A尽快开始ROM调试的需求。做了三种编译选项：
		AA：17G10A的主干版本,是SKY的PA，也就是17G05A QORVO需要用到的；驱动部在这个上面调试驱动；
		AB：17G10A的77010PA的版本，也就是射频工程师调试77010需要用到的；
		AC：17G10A分支为17G05A出的版本，也就是前期那一批整机需要用到的，方便软件同事适配ROM用；


		gionee/config/17G10A.mk ： GIONEE_VERSION_BY=17G10A-T0003-170522AA    //修改最后两位字母，对应编译选项，就是AA

		主要是modem代码不同，先删除不一样的
		rm -rf vendor/mediatek/proprietary/custom/gnbj6757_66_n/modem/*				*/

		然后拷贝17G10A_AA 目录下的modem到vender目录下

		AA目录下拷贝clkbuf_ctl.h，mtk_clkbuf_ctl.h到外面

		git checkout vendor/mediatek/proprietary/bootable/bootloader/preloader/platform/mt6757/src/drivers/inc/clkbuf_ctl.h
		git checkout kernel-4.4/drivers/misc/mediatek/base/power/mt6757/mtk_clkbuf_ctl.h 

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

		/*有时候要看源码目录下的makefile文件，他制定了编译的规则，哪些是需要的*/
		项目脚本里哪些宏是需要的，宏包含的代码有哪些？这点很重要
}





/*重要的概念*/
{

	log关键字
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



	这几个概念有必要理解一下
	{   
		
	->  AICR:Average Input Current Regulation (AICR) : 0.1A to
		3.25A in 50mA steps
		充电电流不能大于此值,此功能是检测充电器的input current limit，也就是输出电流的最大值，是由rt5081来做；
	  但是只是在插入充电器的时候去做检测，直到拔出充电器再次插入才会再一次检测，同一个充电器不会多次检测输出能力，
	  因为同一个充电器此值肯定是相同的，没必要再检测

		控制的寄存器：0x13
		Average Input Current Regulation (AICR) levels (0x13,
		bit[7:2]) and output charge current (ICHG) (0x17, bit[7:2])
		are all user-programmable.

		通过I2C接口控制
		The AICR current setting is programmed via the I2C 
		interface. For example, AICR 100mA Mode limits the input 
		current to 100mA, and AICR 500mA Mode to 500mA. If 
		not needed, this function can be disabled. The AICR 
		current levels are in the range of 100mA to 3250mA with a 
		resolution of 50mA. 

		这个接口是设置充电电流的
		_rt5081_set_aicr


	-> aicl:average input current levels
			When IAICR set to large current and the VBUS drop to VMIVR
		level, AICL measurement will decrease IAICR level step by step
		automatically until VBUS voltage is large than AICL threshold
		voltage



	->mivr: Minimum Input Voltage Regulation 
		MIVR是通过代码直接设置的，当Vbus电压小于此值就会自动减少充电电流,用来维持充电电压在阈值之上，
		如果电压低于阈值(UVLO)可能就不能充电了  

		可以通过I2C接口控制，调控范围3.9V~13.4V  
		控制的寄存器：0x16  

		->vmir:input MIVR threshold setting，就是上面那个设定的最低的值，低于这个值就触发条件
		但是 最低不能低于4.5V，因为电池电压充满在4.4V左右，如果设低了，永远充不满


	Use MIVR to enable/disable power path
		Disable MIVR IRQ -> enable direct charge
		Enable MIVR IRQ -> disable direct charge




	/**************************************************************************************/
		info->enable_dynamic_cv = true；
		mtk_get_dynamic_cv

	“所以这时候要看MIVR中断是否触发，触发后AICL是否也对应着改变了？”
		Do AICL in a workqueue after receiving MIVR IRQ
		rt5081_run_aicl
		/* Enable MIVR IRQ ，stop AICL*/
		ret = rt5081_pmu_reg_clr_bit(chg_data->chip,
			RT5081_PMU_CHGMASK1, RT5081_MASK_CHG_MIVRM);


		If PE20/30 is connected, do not run AICL
		就是说如果是PE20那就不会执行AICL，所以在PE20 connect的代码里肯定有关闭AICL的


		Read USB STATUS(0x27) instead of device type(0x22)
		to check charger type
		通过读USB的status判断插入的充电器的类型


		？hidden_mode_lock这个锁包含的代码做了哪些事
		？rt5081_enable_hidden_mode，这是干什么用的
		{
		hidden_mode跟这几个寄存器相关，而这几个寄存器是跟type-c和usb pd有关
		PD Part Register Detail Description

			#define RT5081_PMU_REG_HIDDENPASCODE1	(0x07)
			#define RT5081_PMU_REG_HIDDENPASCODE2	(0x08)
			#define RT5081_PMU_REG_HIDDENPASCODE3	(0x09)
			#define RT5081_PMU_REG_HIDDENPASCODE4	(0x0A)
		}

		RT5081_PMU_REG_CHGCTRL2这个寄存器写1可以将充电线上的电直接提供给系统不给电池
		1. TE (0x12, bit[4]) : If this bit is enabled, the power path
		will be turned off, and the buck of the charger will keep
		providing power to the system


		RT5081是放在哪一路I2C上？这路I2C您在dws中是配置成多高的速度？IO方式是opendrain还是push pull?

		I2C_CHANNEL_5  ,0x34
		speed   3400kbps
		pull&push en

	}


	充电器类型检测
	{
		RT5081定义的充电器检测类型
		enum rt5081_pmu_chg_type {
			RT5081_CHG_TYPE_NOVBUS = 0,				//0:NOVBUS
			RT5081_CHG_TYPE_UNDER_GOING,			//1 001:
			RT5081_CHG_TYPE_SDP,					//2 010:
			RT5081_CHG_TYPE_SDPNSTD,				//3 011:
			RT5081_CHG_TYPE_DCP,					//4 100:
			RT5081_CHG_TYPE_CDP,					//5 101:
			RT5081_CHG_TYPE_MAX,					//6 110:
		};
		
		函数调用时检测的充电器类型
		typedef enum {
			CHARGER_UNKNOWN = 0,
			STANDARD_HOST,		/* USB : 450mA */
			CHARGING_HOST,
			NONSTANDARD_CHARGER,/* AC : 450mA~1A */
			STANDARD_CHARGER,	/* AC : ~1A */
			APPLE_2_1A_CHARGER,	/* 2.1A apple charger */
			APPLE_1_0A_CHARGER,	/* 1A apple charger */
			APPLE_0_5A_CHARGER,	/* 0.5A apple charger */
			WIRELESS_CHARGER,
		} CHARGER_TYPE;


		如何通过USB口检测出是标准充电器和非标准充电器
		BC1.2中定义的充电类型
		SDP：标准下行接开口，电脑USB
		CDP：充电下行接口
		DCP：标准的座充
		OCP：other chargng port非标准充电器
	
	}


	boot_reason
	{
		enum boot_reason_t {
			BR_POWER_KEY = 0,	//按power键导致的重启
			BR_USB,
			BR_RTC,
			BR_WDT,
			BR_WDT_BY_PASS_PWK,
			BR_TOOL_BY_PASS_PWK,
			BR_2SEC_REBOOT,	   //长按power键关机两秒内松手
			BR_UNKNOWN
		};
	}

	这几个量应该是设置充电限流相关的
	struct charger_data {
		int force_charging_current;
		int thermal_input_current_limit;
		int thermal_charging_current_limit;
		int input_current_limit;
		int charging_current_limit;
		int disable_charging_count;
	};

	相关的宏和gpio
	{
		type-c定义的宏
		CONFIG_TCPC_CLASS=y
		CONFIG_USB_POWER_DELIVERY=y
		CONFIG_TCPC_RT5081=y
		CONFIG_DUAL_ROLE_USB_INTF=y
		
		快充定义的宏	
		CONFIG_MTK_PUMP_EXPRESS_PLUS_20_SUPPORT
		CONFIG_MTK_PUMP_EXPRESS_PLUS_30_SUPPORT

		USB_ID					23
		DRV_BUS					43
		GPIO_CHG_EN				100
		EINT_CHG_STAT			101(EINT)
		EINT_RT5081_PD_IRQB		8(EINT)	
		TYPEC_HEADSET_CTRL_EN	86(EINT)
		
		switching_charger	I2C_CHANNEL_1		0X6A
	}


	相关的函数接口
	{
		battery_meter_hal.h 定义的电量计能执行的函数指针
		typedef enum {
			BATTERY_METER_CMD_HW_FG_INIT,

			BATTERY_METER_CMD_GET_HW_FG_CURRENT,	/* fgauge_read_current */
			BATTERY_METER_CMD_GET_HW_FG_CURRENT_SIGN,	/*  */
			BATTERY_METER_CMD_GET_HW_FG_CAR,	/* fgauge_read_columb */

			BATTERY_METER_CMD_HW_RESET,	/* FGADC_Reset_SW_Parameter */

			BATTERY_METER_CMD_GET_ADC_V_BAT_SENSE,
			BATTERY_METER_CMD_GET_ADC_V_I_SENSE,
			BATTERY_METER_CMD_GET_ADC_V_BAT_TEMP,
			BATTERY_METER_CMD_GET_ADC_V_CHARGER,

			BATTERY_METER_CMD_GET_HW_OCV,
			BATTERY_METER_CMD_DUMP_REGISTER,
			BATTERY_METER_CMD_SET_COLUMB_INTERRUPT,
			BATTERY_METER_CMD_GET_BATTERY_PLUG_STATUS,
			BATTERY_METER_CMD_GET_HW_FG_CAR_ACT,	/* fgauge_read_columb */
			BATTERY_METER_CMD_SET_LOW_BAT_INTERRUPT,
			BATTERY_METER_CMD_GET_LOW_BAT_INTERRUPT_STATUS,
			BATTERY_METER_CMD_GET_REFRESH_HW_OCV,
			BATTERY_METER_CMD_GET_IS_HW_OCV_READY,

			BATTERY_METER_CMD_SET_META_CALI_CURRENT,
			BATTERY_METER_CMD_META_CALI_CAR_TUNE_VALUE,
			BATTERY_METER_CMD_GET_ZCV_INT_HW_OCV,

			BATTERY_METER_CMD_NUMBER
		} BATTERY_METER_CTRL_CMD;


		rt5081_pmu_charger.c  rt5081_chg_ops  rt5081提供的函数接口
		static struct charger_ops rt5081_chg_ops = {
			/* Normal charging */
			.plug_out = rt5081_plug_out,
			.plug_in = rt5081_plug_in,
			.dump_registers = rt5081_dump_register,
			.enable = rt5081_enable_charging,
			.get_charging_current = rt5081_get_ichg,
			.set_charging_current = rt5081_set_ichg,
			.get_input_current = rt5081_get_aicr,
			.set_input_current = rt5081_set_aicr,
			.get_constant_voltage = rt5081_get_cv,
			.set_constant_voltage = rt5081_set_cv,
			.kick_wdt = rt5081_kick_wdt,
			.set_mivr = rt5081_set_mivr,
			.is_charging_done = rt5081_is_charging_done,
			.get_zcv = rt5081_get_zcv,
			/* Safety timer */
			.enable_safety_timer = rt5081_enable_safety_timer,
			.is_safety_timer_enabled = rt5081_is_safety_timer_enable,
			/* Power path */
			.enable_powerpath = rt5081_enable_power_path,
			.is_powerpath_enabled = rt5081_is_power_path_enable,
			/* Charger type detection */
			.enable_chg_type_det = rt5081_enable_chg_type_det,
			/* OTG */
			.enable_otg = rt5081_enable_otg,
			.set_boost_current_limit = rt5081_set_otg_current_limit,
			.enable_discharge = rt5081_enable_discharge,
			/* PE+/PE+20 */
			.send_ta_current_pattern = rt5081_set_pep_current_pattern,
			.set_pe20_efficiency_table = rt5081_set_pep20_efficiency_table,
			.send_ta20_current_pattern = rt5081_set_pep20_current_pattern,
			.set_ta20_reset = rt5081_set_pep20_reset,
			.enable_cable_drop_comp = rt5081_enable_cable_drop_comp,
			/* ADC */
			.get_tchg_adc = rt5081_get_tchg,
			.get_ibus_adc = rt5081_get_ibus,
		};
	}



	电池曲线相关的table,i,mah,voltage,resistance,percentage
	<3>[    8.439090]  (4)[478:fuelgauged]MTK_FG: [fg_dump_table]table idx:255 (i,mah,voltage,resistance,percentage)
	<3>[    8.439132]  (4)[478:fuelgauged]MTK_FG:  ( 0,    0,43857, 1455,  0) ( 1,  305,43711, 1454,105) ( 2,  609,43581, 1456,211) ( 3,  914,43462, 1452,317) ( 4, 1219,43345, 1458,423)
	<3>[    8.439174]  (4)[478:fuelgauged]MTK_FG:  ( 5, 1524,43230, 1462,529) ( 6, 1828,43111, 1458,634) ( 7, 2133,42992, 1459,740) ( 8, 2438,42872, 1459,846) ( 9, 2742,42752, 1462,951)
	<3>[    8.439217]  (4)[478:fuelgauged]MTK_FG:  (10, 3047,42632, 1470,1057) (11, 3352,42514, 1466,1163) (12, 3657,42396, 1469,1269) (13, 3961,42278, 1476,1375) (14, 4266,42161, 1493,1480)
	<3>[    8.439319]  (4)[478:fuelgauged]MTK_FG:  (15, 4571,42044, 1506,1586) (16, 4875,41927, 1513,1692) (17, 5180,41811, 1525,1798) (18, 5485,41696, 1531,1904) (19, 5790,41581, 1536,2009)
	<3>[    8.439366]  (4)[478:fuelgauged]MTK_FG:  (20, 6094,41468, 1544,2115) (21, 6399,41357, 1552,2221) (22, 6704,41246, 1562,2327) (23, 7008,41135, 1570,2432) (24, 7313,41026, 1577,2538)
	<3>[    8.439427]  (4)[478:fuelgauged]MTK_FG:  (25, 7618,40919, 1584,2644) (26, 7923,40812, 1582,2750) (27, 8227,40709, 1588,2855) (28, 8532,40610, 1609,2961) (29, 8837,40515, 1640,3067)
	<3>[    8.439472]  (4)[478:fuelgauged]MTK_FG:  (30, 9142,40414, 1650,3173) (31, 9446,40307, 1667,3279) (32, 9751,40202, 1673,3384) (33,10056,40100, 1685,3490) (34,10360,40009, 1699,3596)
	<3>[    8.439517]  (4)[478:fuelgauged]MTK_FG:  (35,10665,39926, 1714,3702) (36,10970,39844, 1728,3808) (37,11275,39763, 1746,3913) (38,11579,39680, 1754,4019) (39,11884,39593, 1765,4125)
	<3>[    8.439561]  (4)[478:fuelgauged]MTK_FG:  (40,12189,39503, 1765,4231) (41,12493,39405, 1749,4336) (42,12798,39292, 1699,4442) (43,13103,39161, 1619,4548) (44,13408,39031, 1543,4654)
	<3>[    8.439604]  (4)[478:fuelgauged]MTK_FG:  (45,13712,38924, 1500,4759) (46,14017,38837, 1480,4865) (47,14322,38761, 1466,4971) (48,14626,38692, 1456,5077) (49,14931,38628, 1454,5183)
	<3>[    8.439649]  (4)[478:fuelgauged]MTK_FG:  (50,15236,38568, 1452,5288) (51,15541,38512, 1450,5394) (52,15845,38458, 1454,5500) (53,16150,38407, 1459,5606) (54,16455,38357, 1460,5712)
	<3>[    8.439693]  (4)[478:fuelgauged]MTK_FG:  (55,16759,38309, 1463,5817) (56,17064,38263, 1472,5923) (57,17369,38220, 1480,6029) (58,17674,38179, 1485,6135) (59,17978,38139, 1493,6240)
	<3>[    8.439740]  (4)[478:fuelgauged]MTK_FG:  (60,18283,38100, 1500,6346) (61,18588,38063, 1513,6452) (62,18892,38028, 1522,6558) (63,19197,37994, 1526,6664) (64,19502,37962, 1537,6769)
	<3>[    8.439784]  (4)[478:fuelgauged]MTK_FG:  (65,19807,37931, 1541,6875) (66,20111,37899, 1548,6981) (67,20416,37873, 1560,7087) (68,20721,37842, 1563,7193) (69,21025,37808, 1560,7298)
	<3>[    8.439829]  (4)[478:fuelgauged]MTK_FG:  (70,21330,37766, 1534,7404) (71,21635,37717, 1508,7510) (72,21940,37662, 1490,7616) (73,22244,37617, 1479,7721) (74,22549,37574, 1480,7827)
	<3>[    8.439874]  (4)[478:fuelgauged]MTK_FG:  (75,22854,37529, 1474,7933) (76,23159,37484, 1465,8039) (77,23463,37443, 1466,8144) (78,23768,37407, 1472,8250) (79,24073,37374, 1488,8356)
	<3>[    8.439919]  (4)[478:fuelgauged]MTK_FG:  (80,24377,37338, 1502,8462) (81,24682,37293, 1505,8568) (82,24987,37236, 1498,8673) (83,25292,37174, 1488,8779) (84,25596,37116, 1492,8885)
	<3>[    8.439963]  (4)[478:fuelgauged]MTK_FG:  (85,25901,37059, 1509,8991) (86,26206,36986, 1506,9097) (87,26510,36911, 1481,9202) (88,26815,36872, 1482,9308) (89,27120,36857, 1507,9414)
	<3>[    8.440008]  (4)[478:fuelgauged]MTK_FG:  (90,27425,36844, 1541,9520) (91,27729,36831, 1587,9625) (92,28034,36809, 1647,9731) (93,28339,36761, 1703,9837) (94,28643,36595, 1680,9943)
	<3>[    8.440053]  (4)[478:fuelgauged]MTK_FG:  (95,28948,36230, 1654,10048) (96,29253,35729, 1715,10154) (97,29558,35060, 1825,10260) (98,29862,34097, 2025,10366) (99,30167,32486, 2730,10472)
	<3>[    8.440083]  (4)[478:fuelgauged]MTK_FG: [fg_dump_table]table idx:254 (i,mah,voltage,resistance,percentage)
	<3>[    8.440124]  (4)[478:fuelgauged]MTK_FG:  ( 0,    0,43857, 1455,  0) ( 1,  305,43711, 1454,105) ( 2,  609,43581, 1456,211) ( 3,  914,43462, 1452,317) ( 4, 1219,43345, 1458,423)
	<3>[    8.440165]  (4)[478:fuelgauged]MTK_FG:  ( 5, 1524,43230, 1462,529) ( 6, 1828,43111, 1458,634) ( 7, 2133,42992, 1459,740) ( 8, 2438,42872, 1459,846) ( 9, 2742,42752, 1462,951)
	<3>[    8.440209]  (4)[478:fuelgauged]MTK_FG:  (10, 3047,42632, 1470,1057) (11, 3352,42514, 1466,1163) (12, 3657,42396, 1469,1269) (13, 3961,42278, 1476,1375) (14, 4266,42161, 1493,1480)
	<3>[    8.440254]  (4)[478:fuelgauged]MTK_FG:  (15, 4571,42044, 1506,1586) (16, 4875,41927, 1513,1692) (17, 5180,41811, 1525,1798) (18, 5485,41696, 1531,1904) (19, 5790,41581, 1536,2009)
	<3>[    8.440298]  (4)[478:fuelgauged]MTK_FG:  (20, 6094,41468, 1544,2115) (21, 6399,41357, 1552,2221) (22, 6704,41246, 1562,2327) (23, 7008,41135, 1570,2432) (24, 7313,41026, 1577,2538)
	<3>[    8.440343]  (4)[478:fuelgauged]MTK_FG:  (25, 7618,40919, 1584,2644) (26, 7923,40812, 1582,2750) (27, 8227,40709, 1588,2855) (28, 8532,40610, 1609,2961) (29, 8837,40515, 1640,3067)
	<3>[    8.440387]  (4)[478:fuelgauged]MTK_FG:  (30, 9142,40414, 1650,3173) (31, 9446,40307, 1667,3279) (32, 9751,40202, 1673,3384) (33,10056,40100, 1685,3490) (34,10360,40009, 1699,3596)
	<3>[    8.440434]  (4)[478:fuelgauged]MTK_FG:  (35,10665,39926, 1714,3702) (36,10970,39844, 1728,3808) (37,11275,39763, 1746,3913) (38,11579,39680, 1754,4019) (39,11884,39593, 1765,4125)
	<3>[    8.440479]  (4)[478:fuelgauged]MTK_FG:  (40,12189,39503, 1765,4231) (41,12493,39405, 1749,4336) (42,12798,39292, 1699,4442) (43,13103,39161, 1619,4548) (44,13408,39031, 1543,4654)
	<3>[    8.440532]  (4)[478:fuelgauged]MTK_FG:  (45,13712,38924, 1500,4759) (46,14017,38837, 1480,4865) (47,14322,38761, 1466,4971) (48,14626,38692, 1456,5077) (49,14931,38628, 1454,5183)
	<3>[    8.440579]  (4)[478:fuelgauged]MTK_FG:  (50,15236,38568, 1452,5288) (51,15541,38512, 1450,5394) (52,15845,38458, 1454,5500) (53,16150,38407, 1459,5606) (54,16455,38357, 1460,5712)
	<3>[    8.440623]  (4)[478:fuelgauged]MTK_FG:  (55,16759,38309, 1463,5817) (56,17064,38263, 1472,5923) (57,17369,38220, 1480,6029) (58,17674,38179, 1485,6135) (59,17978,38139, 1493,6240)
	<3>[    8.440667]  (4)[478:fuelgauged]MTK_FG:  (60,18283,38100, 1500,6346) (61,18588,38063, 1513,6452) (62,18892,38028, 1522,6558) (63,19197,37994, 1526,6664) (64,19502,37962, 1537,6769)
	<3>[    8.440712]  (4)[478:fuelgauged]MTK_FG:  (65,19807,37931, 1541,6875) (66,20111,37899, 1548,6981) (67,20416,37873, 1560,7087) (68,20721,37842, 1563,7193) (69,21025,37808, 1560,7298)
	<3>[    8.440757]  (4)[478:fuelgauged]MTK_FG:  (70,21330,37766, 1534,7404) (71,21635,37717, 1508,7510) (72,21940,37662, 1490,7616) (73,22244,37617, 1479,7721) (74,22549,37574, 1480,7827)
	<3>[    8.440801]  (4)[478:fuelgauged]MTK_FG:  (75,22854,37529, 1474,7933) (76,23159,37484, 1465,8039) (77,23463,37443, 1466,8144) (78,23768,37407, 1472,8250) (79,24073,37374, 1488,8356)
	<3>[    8.440846]  (4)[478:fuelgauged]MTK_FG:  (80,24377,37338, 1502,8462) (81,24682,37293, 1505,8568) (82,24987,37236, 1498,8673) (83,25292,37174, 1488,8779) (84,25596,37116, 1492,8885)
	<3>[    8.440890]  (4)[478:fuelgauged]MTK_FG:  (85,25901,37059, 1509,8991) (86,26206,36986, 1506,9097) (87,26510,36911, 1481,9202) (88,26815,36872, 1482,9308) (89,27120,36857, 1507,9414)
	<3>[    8.440936]  (4)[478:fuelgauged]MTK_FG:  (90,27425,36844, 1541,9520) (91,27729,36831, 1587,9625) (92,28034,36809, 1647,9731) (93,28339,36761, 1703,9837) (94,28643,36595, 1680,9943)
	<3>[    8.440980]  (4)[478:fuelgauged]MTK_FG:  (95,28948,36230, 1654,10048) (96,29253,35729, 1715,10154) (97,29558,35060, 1825,10260) (98,29862,34097, 2025,10366) (99,30167,32486, 2730,10472)


}





/*调用流程，代码逻辑*/
{
充电代码相关的初始化和状态机控制	
	(mtk_charger.c)mtk_charger_probe: starts 初始化charger_manager结构体，首先是从mtk6757.dtsi 的charger开始对不同成员赋值，如果没有定义就从
	
	mtk_charging.h的宏赋值包括一些电池参数和调用充电的算法--->(mtk_switch_charging.c)选择不同的充电算法,mtk_switch_charging_init,从rt5081.dtsi
	
	(里面的一些参数是从rt5081_pmu_core.c获取匹配，然后再到不同的项里面匹配)获取一些参数，然后调用不同的函数指针--->创建一个常用的线程charger_routine_thread
	 
	检查充电与否，充电器的类型，和一些电池参数-->(mtk_battery_intf.c)battery_get_bat_current,获取充电电流--->(mtk_battery_hal.c)bm_ctrl_cmd
	
	定义了一些调用fuelgauge的函数指针--->charger_update_data获取电量计的状态，某种温度下，再调用force_get_tbat--->charger_check_status检测充电器的状态，
	
	都是基于battery thermal protection进行的检测--->kpoc_power_off_check应该是关机充电的检测

rt5081.dtsi参数在rt5081_pmu*的of_device_id里面获取的
meter,table：这两个都是电量计算的相关参数



充电器插入：
	（rt5081_pmu_charger.c）rt5081_pmu_ovpctrl_uvp_d_evt_irq_handler 这段走的是非type-c接口 -> rt5081_pmu_attachi_irq_handler
    
	这个应该是rt5081接受到中断后，就是接口插入的动作还有一个rt5081_pmu_detachi_irq_handler是接口把出的动作 -> rt5081_inform_psy_changed
    
	将检测到的信息上报到power_supply子系统 -> (mtk_charger.c)mtk_charger_int_handler, mt_charger对应的中断回调函数 -> _wake_up_charger
    
	唤醒mt_charger -> rt5081_enable_chgdet_flow 进行USB充电使能的检测 -> rt5081_set_usbsw_state 设置USB的状态

    mtk_is_charger_on->mtk_is_charger_in-> (rt5081_pmu_charger.c) rt5081_plug_in rt5081检测到充电器插入所要进行的动作

	(mtk_charger.c) battery_callback 对电池状态变化上报给power_supply子系统 -> rt5081_enable_charging (0x12) 充电使能，
	
	这个充电使能只是简单的对寄存器使用set或者clear，置位或者清空 -> rt5081_enable_ilim 充电电流是否限制 -> (mtk_switch_charging.c)

    mtk_switch_charging_run运行充电正常的状态机，这些数据的初始化应该在init内 -> mtk_switch_chr_cc恒流充电模式

    ->swchg_turn_on_charging(info)充电使能 -> swchg_select_charging_current_limit设置充电电流的限制



键盘的代码执行流程：

    (platform.c) platform_init这个是很多平台相关的一个初始化 -> /*init kpd PMIC mode support*/ set_kpd_pmic_mode,是否支持外置键盘
    
    -> mtk_kpd_gpio_set设置键盘gpio的控制状态 ,这个地方应该是可以使能那些按键然后可以进入factory ，recovery模式 -> platform_boot_status
    
    系统启动的原因，powerkey，restart还是wdt引起的启动 -> (keypad.c) mtk_detect_key这个应该是检测主要的那几个物理按键powerkey还是rst之类的
    
    -> pmic_detect_homekey 检测powerkey是否按下 -> (boot_mode.c) boot_mode_select启动模式的选择 ...... -> (kpd.c)这个文件应该是键盘相
    
    关很重要的代码，kpd_mod_init这个是linux 驱动模块初始化 ->  kpd_pdrv_probe初始化kpd驱动，初始化kpd内存，哪些按键值要上报->kpd_irq_handler

    注册对应的中断回调函数，产生中断后调用注册的工作队列tasklet->kpd_keymap_handler,检测所有的按键，比较按键的状态跟之前是否有变化 -> 

    kpd_aee_handler 对VOLUMEUP，VOLUMEDOWN这两个键是按下还是松开处理并上报状态 -> (hal_kpd.c) mt_eint_register 注册一个中断在powerykey为
    
    一个中断的时候，而且这里面是一个dump操作 -> long_press_reboot_function_setting 正常模式还是其他 -> long_press_reboot_function_init_pmic
    
    这里是长按powerkey对应什么什么操作重启还是直接关机 -> gn_hall_eint_handler ,kpd_hall_eint_handler 这两个是霍尔开关，控制量实现开关的开闭

    -> aw9523b_mod_init 这是按键相关芯片的初始化 -> aw9523b_pinctrl_probe从dts文件里获取引脚的状态	pinctrl-0 = <&gpio_aw9523b_default>;
	
    pinctrl-1 = <&gpio_aw9523b_eint_as_int>; pinctrl-2 = <&gpio_aw9523b_reset_high>; pinctrl-3 = <&gpio_aw9523b_reset_low>;

    这是键盘IC控制的几个引脚的状态 -> aw9523b_i2c_probe 这是挂在I2C总线下的一个设备驱动 -> aw9523b_input_event_init 核心应该是这个，将底层事件上报

    给input子系统，注册设备驱动，可以上报哪些事件  -> set_p0_x_p1_y设置行列 -> aw9523b_init初始化 -> aw9523b_isr中断



马达震动:
lk
    (platform.c)platform_init 系统平台相关的初始化 -> gn_lk_vibrate 马达震动 -> 输出引脚使能，设置MODE_REG改成相应的模式idle，standby，ready
    哪种模式然后设置几个量，延时一段时间，然后关闭引脚使能，mode变成ready


kernel
    (gn_ti_drv2604l.c)drv2604l_init，pinctrl初始化引脚状态，往I2C总线上注册驱动->drv2604l_probe芯片驱动相关的初始化，检测I2C是否正常，设置client数据
    
    获取dev相关的状态 -> drv2604l_change_mode(struct DRV2604L_data *pDrv2604ldata, char work_mode, char dev_mode)这个是改变马达模式都要调用的
    
    一个函数，work_mode,dev_mode -> schedule_timeout_interruptible调用一段时间 -> dev_init_platform_data 平台设备相关的初始化 -> Haptics_init

    震动效果相关的初始化，创建一个字符设备节点，注册几个函数enable ，工作函数vibrator_timer_func -> vibrator_work_routine,传进不同work_mode,dev_mode

    调用不同的震动效果	

}

	






/**************************************************************************************************************************************************/
/*相关的文件*/
{
	dtsi文件(这个目录下很多文件都用到了):
		kernel-4.4/archarm64/boot/dts/mediatek/mt6757.dtsi  ，rt5081.dtsi  , mt6355.dtsi
		
	dws文件:
		vendor/mediatek/proprietary/bootable/bootloader/preloader/custom/k57pv1_6mtee_pre/dct/dct/codegen.dws
		vendor/mediatek/proprietary/bootable/bootloader/lk/target/k57pv1_6mtee_pre/dct/dct/codegen.dws
		kernel-4.4/drivers/misc/mediatek/dws/mt6757/k57pv1_6mtee_pre.dws
	
	
	电池参数相关的文件
			mt6757.dtsi,rt5081.dtsi
			mtk_charging.h ，mtk_charging_intf.h(快充)
			mtk_battery_meter.h ,mtk_battery_meter_table.h
			pmic_throttling_dlpt.c读电池客制化参数，mtk_battery_property.h
	
	
			  mtk_charger.c跟mtk_charger_intf.h对应
			mtk_battery.c跟mtk_battery.h对应 
			
	
	GM3.0 定义的宏CONFIG_MTK_GAUGE_VERSION=30
			./arch/arm64/configs/k57pv1_6mtee_pre_debug_defconfig:377:CONFIG_MTK_GAUGE_VERSION=30
			./arch/arm64/configs/k57pv1_6mtee_pre_defconfig:361:CONFIG_MTK_GAUGE_VERSION=30
			./arch/arm64/boot/dts/mediatek/mt6757.dtsi:1216:#if (CONFIG_MTK_GAUGE_VERSION == 30)
	
	rt5081的目录
	17G05A/L30_6757_17G05A_N0.MP5_161227_ALPS/android_mtk_6757_mp/kernel-4.4/drivers/misc/mediatek/pmic/rt5081/
		
	
	keyboard 的目录
	17G05A/L30_6757_17G05A_N0.MP5_161227_ALPS/android_mtk_6757_mp/kernel-4.4/drivers/input/keyboard/mediatek/
	
	
	马达的目录
	17G05A/L30_6757_17G05A_N0.MP5_161227_ALPS/android_mtk_6757_mp/kernel-4.4/drivers/misc/mediatek/vibrator
	
	
	温升相关源码的目录
	17G10A/L31_6757_66_N_17G10A_NO.MP5_V1.53_170512_ALPS/android_mtk_mp/kernel-4.4/drivers/misc/mediatek/thermal
	配置文件(这个目录下有很多初始化的东西)
	17G10A/L31_6757_66_N_17G10A_NO.MP5_V1.53_170512_ALPS/android_mtk_mp/device/gionee_bj/
	
	
	编译相关的脚本工具
		android_mtk_6757_mp/gionee/config/tools
	
	电池曲线
		mtk_battery_property.h	
		mt6757_battery_table_ext.dtsi
		mt6757_battery_prop_ext.dtsi
	
	
	performance相关的代码目录
	kernel-4.4/drivers/misc/mediatek/base/power
	vendor/mediatek/proprietary/hardware/perfservice/mt6757/app_list/perfservapplist.txt
		
}
	






















/*debug*/
/*************************************************************************************************************************************/
1.关闭软件关机重启
{
		(kpd.c)kpd_pwrkey_pmic_handler  -> kpd_pmic_pwrkey_hal -> 

		keypad_dts_data对应这几个数据成员，而这几个数据成员可能跟powerkey有关
		u32 kpd_sw_pwrkey;
		u32 kpd_hw_pwrkey;
		u32 kpd_sw_rstkey;
		u32 kpd_hw_rstkey;


	G1605A上定义的
	/* boot type definitions */
	enum boot_mode_t {
		NORMAL_BOOT = 0,
		META_BOOT = 1,
		RECOVERY_BOOT = 2,
		SW_REBOOT = 3,
		FACTORY_BOOT = 4,
		ADVMETA_BOOT = 5,
		ATE_FACTORY_BOOT = 6,
		ALARM_BOOT = 7,
	#if defined(CONFIG_MTK_KERNEL_POWER_OFF_CHARGING)
		KERNEL_POWER_OFF_CHARGING_BOOT = 8,
		LOW_POWER_OFF_CHARGING_BOOT = 9,
	#endif
		DONGLE_BOOT = 10,
		UNKNOWN_BOOT
	};


	typedef enum {
		BR_POWER_KEY = 0,
		BR_USB,
		BR_RTC,
		BR_WDT,
		BR_WDT_BY_PASS_PWK,
		BR_TOOL_BY_PASS_PWK,
		BR_2SEC_REBOOT,
		BR_UNKNOWN,
		BR_KERNEL_PANIC,
		BR_WDT_SW,
		BR_WDT_HW
	} boot_reason_t;


	reason=6，mode=0
	17G05A上定义
	char g_boot_reason[][16]= {"power_key","usb",
								"rtc","wdt",
								"wdt_by_pass_pwk",
								"tool_by_pass_pwk",
								"2sec_reboot","unknown",
								"kernel_panic","reboot",
								"watchdog"};

	修改：
	pmic_mt6355.c  
		PMIC_enable_long_press_reboot函数设置寄存器MT6355_TOP_RST_MISC相应的位为0

	hal_kpd.c
		long_press_reboot_function_init_pmic函数，改变宏定义或者设置寄存器


	分析：
	最后是pmic这边将pwrkey的软件重启关闭了，但是pwrkey还有一根线连接到了rt5081上，而rt5081的CHG_QONB 
	引脚长按15s会断开同系统侧的供电，所以引起了重启动作
	CHG_QONB low time to enable full system reset 

	rt5081这边可以选择路径直接将充电器电给电池还是系统，
	所以最后的解决办法是断开pwrey同rt5081的连接

	搜索的关键字主要是boot方面的，而mtk搜索的是[PMIC]just_rst = 0


	关掉2sec断电重启
	{
		cust_rtc.h

		#define RTC_DEFAULT_YEA		2010
		#define RTC_DEFAULT_MTH		1
		#define RTC_DEFAULT_DOM		1
		#define RTC_2SEC_REBOOT_ENABLE  1		//控制使能
		#define RTC_2SEC_MODE		2			//控制时间
	}


}










/*******************************************************************************************************************/
2.充电电流太小
{
	充电电流太小：标准充电器（1.9A） USB充电（500mA）
	实际只有 标充1.6A~1.8A ，USB充电400mA~460mA

两个重要的功能寄存器的位置
aicl  [0x13]  ，mivr  [0x16]		
		
_rt5081_set_aicr这个是设置电流的，dts文件中是设置初始化的值，但是关于充电电流的设置它是每次插入充电器的时候检测
不断提高充电电流step by step,直到达到最大能力，拔除的时候就将aicr这个值清零，所以每次充电电流都会重新检测充电能力
但是aicr这个值不是很准确，有一定的误差，并把最大值赋值给aicr_limit，
chg_data->aicr_limit = aicr;
dev_info(chg_data->dev, "%s: OK, aicr upper bound = %dmA\n", __func__,aicr / 1000);

rt5081_pmu_charger: _rt5081_set_aicr: 500000mA over TA s cap, can only be 300000mA

if (chg_data->aicr_limit != -1 && uA > chg_data->aicr_limit) {
	dev_err(chg_data->dev,
		"%s: %dmA over TA's cap, can only be %dmA\n",
		__func__, uA, chg_data->aicr_limit);
	uA = chg_data->aicr_limit;
}
	
mivr这个值最低在4.3~4.4V之间，电池电压在4.3V左右，而电池的充电电流是根据充电器电压和电池电压之间的电压差决定的，
MIVR是根据充电器的供电能力，设置最低的充电电压，如果电压小于这个值就降电流不降电压，保持充电电压的稳定，而mivr<4.3V
的话电池永远充不满


这个是每次有充电器插入的时候都会调用的工作函数，这个函数很关键
static int rt5081_run_aicl(struct rt5081_pmu_charger_data *chg_data)
{
	int ret = 0;
	u32 mivr = 0, aicl_vth = 0, aicr = 0;

	dev_info(chg_data->dev, "%s\n", __func__);

	ret = rt5081_get_mivr(chg_data, &mivr);
	if (ret < 0)
		goto out;

	/* Check if there's a suitable AICL_VTH */
	aicl_vth = mivr + 200000;
	if (aicl_vth > RT5081_AICL_VTH_MAX) {
		dev_info(chg_data->dev, "%s: no suitable VTH, vth = %d\n",
			__func__, aicl_vth);
		ret = -EINVAL;
		goto out;
	}

	ret = rt5081_set_aicl_vth(chg_data, aicl_vth);
	if (ret < 0)
		goto out;

	/* Clear AICL measurement IRQ */
	rt5081_chg_irq_clr_flag(chg_data,
		&chg_data->irq_flag[RT5081_CHG_IRQIDX_CHGIRQ5],
		RT5081_MASK_CHG_AICLMEASI);
		
//Gionee <gn_by_charging> <lilubao> <20170416> add for platform begin
	mutex_lock(&chg_data->pe_access_lock);
	mutex_lock(&chg_data->aicr_access_lock);

	/* Set AICR to max */
	ret = _rt5081_set_aicr(chg_data, RT5081_AICR_MAX);
	if (ret < 0)
		goto unlock_out;
//Gionee <gn_by_charging> <lilubao> <20170416> add for platform end		

	ret = rt5081_pmu_reg_set_bit(chg_data->chip, RT5081_PMU_REG_CHGCTRL14,
		RT5081_MASK_AICL_MEAS);
	if (ret < 0)
		goto unlock_out;

	ret = wait_event_interruptible_timeout(chg_data->wait_queue,
		chg_data->irq_flag[RT5081_CHG_IRQIDX_CHGIRQ5] & RT5081_MASK_CHG_AICLMEASI,
		msecs_to_jiffies(2500));
	if (ret <= 0) {
		dev_err(chg_data->dev, "%s: wait AICL time out, ret = %d\n",
			__func__, ret);
		ret = -EIO;
		goto unlock_out;
	}

	ret = rt5081_get_aicr(chg_data->chg_dev, &aicr);
	if (ret < 0)
		goto unlock_out;

	chg_data->aicr_limit = aicr;
	dev_info(chg_data->dev, "%s: OK, aicr upper bound = %dmA\n", __func__,
		aicr / 1000);

unlock_out:
	mutex_unlock(&chg_data->aicr_access_lock);
//Gionee <gn_by_charging> <lilubao> <20170416> add for platform begin	
	mutex_unlock(&chg_data->pe_access_lock);
//Gionee <gn_by_charging> <lilubao> <20170416> add for platform end	
out:
	return ret;
}


修改：
	//Gionee <gn_by_charging> <lilubao> <20170414> add for modify charging begin
	mivr = <4400000>;	/* uV 4500000->4400000*/
	//Gionee <gn_by_charging> <lilubao> <20170414> add for modify charging end

	//Gionee <gn_by_charging> <lilubao> <20170418> add for charging change begin	
	/*chg_data->aicr_limit = aicr;
	dev_info(chg_data->dev, "%s: OK, aicr upper bound = %dmA\n", __func__,
		aicr / 1000);
	*/	
	//force setting aicr_limit
	chg_data->aicr_limit=2050000;
	//Gionee <gn_by_charging> <lilubao> <20170418> add for charging change end
	这样做是强制设定aicr_limit避免将最开始尝试的最大电流设为限定值

	//Gionee <gn_by_charging> <lilubao> <20170417> add for charging change begin
	if (chg_data->aicr_limit != -1 && uA > chg_data->aicr_limit) {
		dev_err(chg_data->dev,
			"%s: %dmA over TA's cap, can only be %dmA\n",
			__func__, uA, chg_data->aicr_limit);
		uA = chg_data->aicr_limit;
	}
	//Gionee <gn_by_charging> <lilubao> <20170417> add for charging change end

	jetia这个功能是disable的，不影响充电电流问题
	{
		检查rt5081的jieta功能是否使用了，是否有跟根据温度限流的有冲突
		07-18 10:12:21.187669   257   257 D [ 1607.744824] (1)[257:charger_thread]: rt5081_pmu_charger rt5081_pmu_charger: rt5081_dump_register: reg[0x1E] = 0x05
		07-18 10:12:21.187769   257   257 D [ 1607.744924] (1)[257:charger_thread]: rt5081_pmu_charger rt5081_pmu_charger: rt5081_dump_register: reg[0x1F] = 0x06
		
		07-18 10:12:21.187871   257   257 D [ 1607.745026] (1)[257:charger_thread]: rt5081_pmu_charger rt5081_pmu_charger: rt5081_dump_register: reg[0x20] = 0x00
		
		07-18 10:12:21.187971   257   257 D [ 1607.745126] (1)[257:charger_thread]: rt5081_pmu_charger rt5081_pmu_charger: rt5081_dump_register: reg[0x21] = 0x80
		07-18 10:12:21.188073   257   257 D [ 1607.745228] (1)[257:charger_thread]: rt5081_pmu_charger rt5081_pmu_charger: rt5081_dump_register: reg[0x22] = 0x50
	}
		


	而这个问题最后确定是由于串接的仪器阻抗太大，有0.4V导致进电池点电压太小，触发mivr，使aicr检测到的认为充电噐的能力很弱
    而中间修改电池容量不是引起充电电流变化的根本原因


这里有几个需要弄清楚的：			20170924
	充电器能力，线损的判断？
	充电相关的变量变化和影响？
	电池容量是否对充电电流有影响？
}













/***************************************************************************************************************/
3.按键功能
{
   kpd.c
		内核创建的设备节点
		platform总线下
		/sys/devices/platform/10010000.keypad/input/input1

		虚拟文件系统
		/sys/devices/virtual/input/

	dts文件内定义的一些重要数据	
		struct keypad_dts_data {
			u32 kpd_key_debounce;
			u32 kpd_sw_pwrkey;
			u32 kpd_hw_pwrkey;
			u32 kpd_sw_rstkey;
			u32 kpd_hw_rstkey;
			u32 kpd_use_extend_type;
			u32 kpd_hw_map_num;
			u32 kpd_hw_init_map[72];
			u32 kpd_pwrkey_eint_gpio;
			u32 kpd_pwrkey_gpio_din;
			u32 kpd_hw_dl_key1;
			u32 kpd_hw_dl_key2;
			u32 kpd_hw_dl_key3;
			u32 kpd_hw_recovery_key;
			u32 kpd_hw_factory_key;
		}


	slide qwerty侧键
		kpd: key-debounce = 1024, sw-pwrkey = 116, hw-pwrkey = 8, hw-rstkey = 17, sw-rstkey = 115

	键盘相关的宏
		CONFIG_KEYBOARD_MTK=y
		CONFIG_ONEKEY_REBOOT_NORMAL_MODE=y
		CONFIG_ONEKEY_REBOOT_OTHER_MODE=y
		CONFIG_KPD_PWRKEY_USE_PMIC=y
		CONFIG_MTK_MRDUMP_KEY=y
		# CONFIG_KEYBOARD_ATKBD is not set


	cat/proc/bus/input/devices 察看input子系统下挂的设备
	{
		I: Bus=0019 Vendor=0000 Product=0000 Version=0000
		N: Name="ACCDET"
		P: Phys=
		S: Sysfs=/devices/virtual/input/input0
		U: Uniq=
		H: Handlers=gpufreq_ib event0 
		B: PROP=0
		B: EV=3
		B: KEY=40 0 0 0 0 0 0 1000000000 c000000000000 0

		I: Bus=0019 Vendor=2454 Product=6500 Version=0010
		N: Name="mtk-kpd"
		P: Phys=
		S: Sysfs=/devices/platform/10010000.keypad/input/input1
		U: Uniq=
		H: Handlers=gpufreq_ib event1 
		B: PROP=0
		B: EV=3
		B: KEY=1000000 0 b00000000 0 0 1c000000000000 0

		I: Bus=0000 Vendor=0000 Product=0000 Version=0000
		N: Name="gf-keys"
		P: Phys=
		S: Sysfs=/devices/virtual/input/input2
		U: Uniq=
		H: Handlers=gpufreq_ib event2 
		B: PROP=0
		B: EV=3
		B: KEY=3100000 100040000800 1c168000000000 0

		I: Bus=0019 Vendor=0000 Product=0000 Version=0000
		N: Name="aw9523b"
		P: Phys=
		S: Sysfs=/devices/virtual/input/input3
		U: Uniq=
		H: Handlers=gpufreq_ib event3 
		B: PROP=0
		B: EV=3
		B: KEY=2000019800100000 40000800 168000000000 4ffc

		I: Bus=0000 Vendor=0000 Product=0000 Version=0000
		N: Name="hwmdata"
		P: Phys=
		S: Sysfs=/devices/virtual/input/input4
		U: Uniq=
		H: Handlers=gpufreq_ib event4 
		B: PROP=0
		B: EV=5
		B: REL=6

		I: Bus=0000 Vendor=0000 Product=0000 Version=0000
		N: Name="m_alsps_input"
		P: Phys=
		S: Sysfs=/devices/virtual/input/input5
		U: Uniq=
		H: Handlers=gpufreq_ib event5 
		B: PROP=0
		B: EV=d
		B: REL=6
		B: ABS=101

		I: Bus=0000 Vendor=0000 Product=0000 Version=0000
		N: Name="m_acc_input"
		P: Phys=
		S: Sysfs=/devices/virtual/input/input6
		U: Uniq=
		H: Handlers=gpufreq_ib event6 
		B: PROP=0
		B: EV=d
		B: REL=c1
		B: ABS=107

		I: Bus=0000 Vendor=0000 Product=0000 Version=0000
		N: Name="m_mag_input"
		P: Phys=
		S: Sysfs=/devices/virtual/input/input7
		U: Uniq=
		H: Handlers=gpufreq_ib event7 
		B: PROP=0
		B: EV=d
		B: REL=3c9
		B: ABS=17f

		I: Bus=0000 Vendor=0000 Product=0000 Version=0000
		N: Name="m_step_c_input"
		P: Phys=
		S: Sysfs=/devices/virtual/input/input8
		U: Uniq=
		H: Handlers=gpufreq_ib event8 
		B: PROP=0
		B: EV=d
		B: REL=7
		B: ABS=100

		I: Bus=0000 Vendor=0000 Product=0000 Version=0000
		N: Name="mtk-tpd"
		P: Phys=
		S: Sysfs=/devices/virtual/input/input9
		U: Uniq=
		H: Handlers=gpufreq_ib event9 
		B: PROP=2
		B: EV=b
		B: KEY=10 0 0 0 400 0 0 100040000000 0 0
		B: ABS=263800001000003

		I: Bus=0019 Vendor=0001 Product=0001 Version=0100
		N: Name="mtk-tpd-kpd"
		P: Phys=
		S: Sysfs=/devices/virtual/input/input10
		U: Uniq=
		H: Handlers=gpufreq_ib event10 
		B: PROP=0
		B: EV=3
		B: KEY=10 0 0 0 0 0 0 100040000000 0 0	



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












/**************************************************************************************************/
4.调试马达震动效果
{
	1.基本概念
	{
		LRA (Linear Resonance Actuator) 线性制动器

		ERM (Eccentric Rotating Mass) 偏转转动惯量

		Back-EMF detection 反电动势检测

		actuator n.激励者； [电脑]执行机构； [电]（电磁铁）螺线管； [机]促动器

		braking  n. 刹车，制动，（用闸）减速；
				v. 刹（车）( brake的现在分词 )；

		calibration  n. 校准，标准化； 刻度，标度； 测量口径；变形         

		resonant adj.洪亮的； 回响的； 共振的； 能共鸣的


		The ERM_LRA bit in register 0x1A must be
		configured to select the type of actuator that the device uses.

		The smart-loop architecture makes the resonant frequency of the LRA available through I2C (see the LRA
		Resonance Period (Address: 0x22) section)

		A key feature of the DRV2604L is the “smart-loop architecture” which employs actuator feedback control for both
		ERMs and LRAs. The feedback control desensitizes the input waveform from the motor-response behavior by
		providing automatic overdrive and automatic braking.

		The FB_BRAKE_FACTOR[2:0] bits can be adjusted to set the brake factor.

		the start-time characteristic may be different for each actuator, the AUTO_CAL_TIME[1:0] bit can change the duration of the
		automatic level-calibration routine to optimize calibration performance.

		DRV2604L 器件是一款低压触觉驱动器，其闭环致动器控制系统，可为 ERM 和 LRA 提供高质量的触觉反馈。此方案有助于提升致动器在加速度稳定性、启动时间和制动时间方面的
		性能，通过共用的 I2C 兼容总线或 PWM 输入信号即可触发该方案。

		DRV2604L 器件集成有足够的 RAM，用户能够预装载超过 100 个定制智能环路架构波形。这些波形可通过 I2C 即时回放，或者也可选择由硬件触发引脚来触发。

		此外，主机处理器可利用实时回放模式绕过存储器回放引擎并通过 I2C 从主机直接播放波形。

		DRV2604L 器件内部采用智能环路架构，可轻松实现自动谐振 LRA 驱动，以及优化反馈的 ERM 驱动，从而提供自动过驱动和制动。这种智能环路架构可构建简化的输入波形接口，
		并且能够提供可靠的电机控制和稳定的电机性能。此外，DRV2604L 器件还 能够 在 LRA 致动器不产生有效反电动势电压时自动切换至开环系统。当 LRA 产生有效反电动势电压时，
		DRV2604L 器件会自动与 LRA 同步。 DRV2604L 还可以利用内部生成的 PWM 信号实现开环驱动。


	}

	2.重要的控制
	{
		当然下面很多变量最重要的是gn_ti_drv2604l.h / gn_ti_drv2604l_customer.h + 芯片手册
		static void drv2604l_change_mode(struct DRV2604L_data *pDrv2604ldata, char work_mode, char dev_mode)
		这个里面有两个mode ，work和dev，什么意思
		dev_mode 这个是马达设备所处的状态，idle闲置中断来了也不会有响应，standby应该是待机模式这个是低功耗随时处在待命模式，中断可以响应
		ready这个应该是active模式了
		#define DEV_IDLE	                0 // default
		#define DEV_STANDBY					1
		#define DEV_READY					2

		这个是相应的工作模式 
		#define	WORK_IDLE					0x00
		#define WORK_RTP			      	0x06
		#define WORK_CALIBRATION	      	0x07
		#define WORK_VIBRATOR		      	0x08
		#define	WORK_PATTERN_RTP_ON			0x09
		#define WORK_PATTERN_RTP_OFF      	0x0a
		#define WORK_SEQ_RTP_ON		      	0x0b
		#define WORK_SEQ_RTP_OFF    	  	0x0c
		#define WORK_SEQ_PLAYBACK    	  	0x0d

		I2C的地址和适配器
		#define DRV2604L_I2C_BUS_ID         4
		#define DRV2604L_I2C_ADDR			0x5A
	}



	这是振动器的配置
	static struct actuator_data DRV2604L_actuator={
			.device_type = LRA,
			.rated_vol = 0x46,
			.over_drive_vol = 0x7a,
			.LRAFreq = 235,
	};


	相关的代码
	{
			1.vibrator提供了两套mtk自带的和第三方的
			CONFIG_GN_BSP_MTK_VIBRATOR_DRV2604L
			CONFIG_MTK_VIBRATOR


	2.lk和kernel有两套代码

	lk阶段
	效果：就是让马达震动一会然后关闭
	这里调用的是更直接的过程
	拉高使能引脚，配置几个必须的参数，往0x01 mode_reg写相应的工作模式，然后设置成idle模式，关闭使引脚
	void gn_lk_vibrate(void)
	{    
		printf("%s.\n",__func__);
		
		unsigned int result_tmp;
		char reg_address;
		char reg_data;

		//set gpio，设置drv2604_en引脚输出使能
		mt_set_gpio_out(DRV2604L_GPIO_ENABLE_PIN,1);
		mdelay(2);

		//往寄存器0x01写 0 为DEV_IDLE模式，DEV_STANDBY  1， DEV_READY  2
		drv2604l_i2c_write(MODE_REG,DEV_IDLE);

		//init regs 这两个参数都要在标准化之前设定好
		//这个是设置额定电压 rated_voltage: 0x16
		drv2604l_i2c_write(RATED_VOLTAGE_REG, 0x30);

		//超速的钳位电压 ，在闭环操作中这个电压可以超过额定电压 Overdrive Clamp Voltage ：0x17
		drv2604l_i2c_write(OVERDRIVE_CLAMP_VOLTAGE_REG,0x53);

		/*drv2604l_set_bits(FEEDBACK_CONTROL_REG,	
							FEEDBACK_CONTROL_DEVICE_TYPE_MASK|FEEDBACK_CONTROL_FB_BRAKE_MASK|FEEDBACK_CONTROL_LOOP_GAIN_MASK,
							FEEDBACK_CONTROL_MODE_LRA|FB_BRAKE_FACTOR|LOOP_GAIN);

		drv2604l_set_bits(Control3_REG,	
							Control3_REG_LOOP_MASK|Control3_REG_FORMAT_MASK,
							BIDIR_INPUT_BIDIRECTIONAL|ERM_OpenLoop_Disable|LRA_OpenLoop_Disable|RTP_FORMAT_SIGNED);
		*/

		//real time payback：0x02 给这个模式写一个值
		drv2604l_i2c_write(REAL_TIME_PLAYBACK_REG, 0x5F);
		//进入RTP模式 ， 实时的反馈
		drv2604l_i2c_write(MODE_REG,MODE_REAL_TIME_PLAYBACK);
		mdelay(300);	// 100 -> 300

		//让设备进入idle模式
		drv2604l_i2c_write(MODE_REG,DEV_IDLE);

		//set gpio
		//关闭en使能引脚
		mt_set_gpio_out(DRV2604L_GPIO_ENABLE_PIN,0);
	}



	kernel阶段

	1.通过write写值进入不同的case，这个就要弄清写什么样的值，如何正确的写进去
	2.或者在原有函数基础上调用自己写的一个函数，ioctl (这里ioctl比write更方便一点)
	ioctl通过写进不同的cmd，传进值，在不同的case里面调用不同的效果

	gn_ti_drv2604l.c
	这样直接调用drv2604l_change_mode，传进不同的work_mode，然后调用work
	{
		drv2604l_change_mode(pDrv2604ldata, work_mode, DEV_READY);
		value = (value>MAX_TIMEOUT)?MAX_TIMEOUT:value;
		hrtimer_start(&pDrv2604ldata->timer, ns_to_ktime((u64)value * NSEC_PER_MSEC), HRTIMER_MODE_REL);
		schedule_work(&pDrv2604ldata->vibrator_work);
	}

	//Gionee <gn_by_charging> <lilubao> <20170504> add for change vibrate begin
	#define MOTOR_TEST
	#if defined(MOTOR_TEST)

	long dev2604_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
	{
		struct DRV2604L_data *pDrv2604ldata = (struct DRV2604L_data *)filp->private_data;
		int value=(int )arg;
		int cnt=10;
		
		printk(KERN_ERR"in %s before by lilubao\n", __FUNCTION__);
		
		switch(cmd)
		{
		// test vibrator_enable	
		case 1:
			printk(KERN_ERR"in %s case 1 vibrator_enable ,value->%d\n", __FUNCTION__,value);
			while(cnt--){

				vibrator_enable( &(pDrv2604ldata->to_dev),value );
				mdelay(500);
				vibrator_enable( &(pDrv2604ldata->to_dev),0);
				mdelay(300);
			}

			break;
			
		// test drv2604l_change_mode
		case 2:
			printk(KERN_ERR"in %s case 2 enable drv2604l_change_mode ,value->%d\n", __FUNCTION__,value);
			
			pDrv2604ldata->should_stop = YES;
			hrtimer_cancel(&pDrv2604ldata->timer);
			cancel_work_sync(&pDrv2604ldata->vibrator_work);
			
			mutex_lock(&pDrv2604ldata->lock);
			
			drv2604l_stop(pDrv2604ldata);
			
			//wake_lock(&pDrv2604ldata->wklock);
			
			drv2604l_change_mode(pDrv2604ldata, WORK_VIBRATOR, DEV_READY);
			pDrv2604ldata->vibrator_is_playing = YES;
			switch_set_state(&pDrv2604ldata->sw_dev, SW_STATE_RTP_PLAYBACK);
			
			value = (value>MAX_TIMEOUT)?MAX_TIMEOUT:value;
			hrtimer_start(&pDrv2604ldata->timer, ns_to_ktime((u64)value * NSEC_PER_MSEC), HRTIMER_MODE_REL);
			schedule_work(&pDrv2604ldata->vibrator_work);
			
			mutex_unlock(&pDrv2604ldata->lock);

			break;
			
		// test vibrator_off
		case 3:
			printk(KERN_ERR"in %s case 3 enable vibrator_off ,value->%d\n", __FUNCTION__,value);
			vibrator_off(pDrv2604ldata);
			break;

		// test play_effect
		case 4:
			printk(KERN_ERR"in %s case 4 enable play_effect ,value->%d\n", __FUNCTION__,value);
			play_effect(pDrv2604ldata);
			break;
			
		// test play_Pattern_RTP
		case 5:	
			printk(KERN_ERR"in %s case 5 enable play_Pattern_RTP ,value->%d\n", __FUNCTION__,value);
			play_Pattern_RTP(pDrv2604ldata);
			break;

		// test play_Seq_RTP
		case 6:
			printk(KERN_ERR"in %s case 6 enable play_Seq_RTP ,value->%d\n", __FUNCTION__,value);
			play_Seq_RTP(pDrv2604ldata);
			break;

		// test drv2604l_change_mode	
		case 7:
			printk(KERN_ERR"in %s case 7 enable drv2604l_change_mode ,value->%d\n", __FUNCTION__,value);
			
			pDrv2604ldata->should_stop = YES;
			hrtimer_cancel(&pDrv2604ldata->timer);
			cancel_work_sync(&pDrv2604ldata->vibrator_work);
			
			mutex_lock(&pDrv2604ldata->lock);
			
			drv2604l_stop(pDrv2604ldata);
			
			//wake_lock(&pDrv2604ldata->wklock);
			
			drv2604l_change_mode(pDrv2604ldata, WORK_VIBRATOR, DEV_READY);
			pDrv2604ldata->vibrator_is_playing = YES;
			switch_set_state(&pDrv2604ldata->sw_dev, SW_STATE_RTP_PLAYBACK);
			
			value = (value>MAX_TIMEOUT)?MAX_TIMEOUT:value;
			hrtimer_start(&pDrv2604ldata->timer, ns_to_ktime((u64)value * NSEC_PER_MSEC), HRTIMER_MODE_REL);
			schedule_work(&pDrv2604ldata->vibrator_work);
			
			mutex_unlock(&pDrv2604ldata->lock);

			break;	

		// test drv2604l_change_mode	for work_mode
		case 8:
			printk(KERN_ERR"in %s case 8 enable drv2604l_change_mode for work_mode,value->%d\n", __FUNCTION__,value);
			cnt=10;
			drv2604l_change_mode(pDrv2604ldata, value, DEV_READY);
			value = MAX_TIMEOUT;
			hrtimer_start(&pDrv2604ldata->timer, ns_to_ktime((u64)value * NSEC_PER_MSEC), HRTIMER_MODE_REL);
			schedule_work(&pDrv2604ldata->vibrator_work);
			break;

		default:
			printk(KERN_ERR"in %s default no cmd\n", __FUNCTION__);
			break;
			
		}

		mutex_unlock(&pDrv2604ldata->lock);
		printk(KERN_ERR"in %s after by lilubao\n", __FUNCTION__);

		return 0;
	}

	#endif
	//Gionee <gn_by_charging> <lilubao> <20170504> add for change vibrate end


	mmi测试有调用马达震动的接口，而ftm*应该是mmi测试相关的源码
	ftm_vibrator.c

	这个是往enable节点写value，震动多长时间
	static int write_int(char const* path, int value)
	{
		int fd;

		if (path == NULL)
			return -1;

		fd = open(path, O_RDWR);
		if (fd >= 0) {
			char buffer[20];
			int bytes = sprintf(buffer, "%d\n", value);
			int amt = write(fd, buffer, bytes);
			close(fd);
			return amt == -1 ? -errno : 0;
		}

		LOGE("write_int failed to open %s\n", path);
		return -errno;
	}

	默认是打开mmi测试，震动，也可以手动写值
	static void *update_vibrator_thread_default(void *priv)
	{
		LOGD("%s: Start\n", __FUNCTION__);

		if(vibrator_time == 0)
		{
		do {
			write_int(VIBRATOR_ENABLE, 8000); // 1 seconds
			if (vibrator_test_exit)
				break;
			sleep(1);
			} while (1);	
			write_int(VIBRATOR_ENABLE, 0);
		}
		else
		{
			LOGD("%s: write vibrator_enable=%d\n", __FUNCTION__, vibrator_time);
			write_int(VIBRATOR_ENABLE, vibrator_time);
			sleep(1);
			write_int(VIBRATOR_ENABLE, 0);
			LOGD("%s: write vibrator_enable=0\n", __FUNCTION__);
		}

		pthread_exit(NULL);

		LOGD("%s: Exit\n", __FUNCTION__);

		return NULL;
	}

	}


}







/*************************************************************************************/
5.调节充电时序状态
{
	恒流充电时间，按power关机不应该是kpoc	，手机完整的充放电过程是否正常，开机充电和关机充电

    //==============================================================================
    // PMIC define
    //==============================================================================
    typedef enum {
        AUXADC_LIST_BATADC,     //检测电池
        AUXADC_LIST_VCDT,       //VCDT：充电电压检测脚
        AUXADC_LIST_BATTEMP,    //电池温度
        AUXADC_LIST_BATID,      //battery ID
        AUXADC_LIST_VBIF,       //应该是跟USB相关的
        AUXADC_LIST_CHIP_TEMP,  //芯片温度
        AUXADC_LIST_DCXO,       // 数字温度补偿晶体震荡器
        AUXADC_LIST_ACCDET,     //耳机检测
        AUXADC_LISTTSX,        
        AUXADC_LIST_HP,
    } AUXADC_LIST;
    auxadc定义的采样channel


    G1605A上电池相关参数在battery_common.h mt_battery_meter.h上，但是17G05A这些参数在哪？
    mtk_charging.h，mtk_battery_meter.h


    0%关机电压太高
    {
        这个应该需要修改电量0%参数
        mtk_battery_meter.h ，mtk_battery_property.h

        mtk_battery_property.h这个里面包含很多电池电量计相关的参数


        关机的几个判断shutdown_event_handler
        {   
            1.执行关机的回调函数，判断一些关机条件是否满足或触发然后执行kernel_power_off -> machine_power_off
            mtk_power_misc_init     power_misc      power_misc_thread


			(mtk_power_mis.c ) power_misc_routine_thread 这个线程里面用于执行等待队列，和shutdown的回调函数shutdown_event_handler
			
			->shutdown_event_handler 回调函数里面对上面物种关机条件进行判断  -> set_shutdown_cond 这个是在(mtk_battery.c)由上层写相应的值传达指令
			
			这里还有一个shutdown_cond_flag 这个标志位是从节点传过来的，应该是是否执行关机条件的判断



            2.会创建一个power_misc_thread线程，这个进程应该是在低电条件下创建的，低电出发了条件，高电的时候没有这个
            这个是关机的条件 
                struct shutdown_condition {
                    bool is_overheat;
                    bool is_soc_zero_percent;
                    bool is_uisoc_one_percent;
                    bool is_under_shutdown_voltage;
                    bool is_dlpt_shutdown;
                };

                /* ============================================================ */
                /* power misc related */
                /* ============================================================ */
                #define BAT_VOLTAGE_LOW_BOUND 3400
                #define BAT_VOLTAGE_HIGH_BOUND 3450
                #define SHUTDOWN_TIME 40
                #define AVGVBAT_ARRAY_SIZE 30
                #define INIT_VOLTAGE 3450
                #define BATTERY_SHUTDOWN_TEMPERATURE 60

            3.shutdown_event_handler  
                这里面都是对那些关机条件进行判断，符合条件的轮训一段时间然后判断是否大于SHUTDOWN_TIME
                符合条件就  kernel_power_off

            4.main.log里面搜索ShutdownThread，上层传达的关机流程
            01-01 07:43:19.967753  2009  2460 V MediaScannerService: handleShutdownThreadpool()
            01-01 07:57:58.074049   962  1080 D ShutdownThread: !!! Request to shutdown !!!
            01-01 07:57:58.074354   962  1080 D ShutdownThread:     |----com.android.server.power.ShutdownThread.shutdown(ShutdownThread.java:201)
            01-01 07:57:58.074429   962  1080 D ShutdownThread:     |----com.android.server.power.PowerManagerService$9.run(PowerManagerService.java:2731)
            01-01 07:57:58.074473   962  1080 D ShutdownThread:     |----android.os.Handler.handleCallback(Handler.java:836)
            01-01 07:57:58.074521   962  1080 D ShutdownThread:     |----android.os.Handler.dispatchMessage(Handler.java:103)
            01-01 07:57:58.074551   962  1080 D ShutdownThread:     |----android.os.Looper.loop(Looper.java:203)
            01-01 07:57:58.074596   962  1080 D ShutdownThread:     |----android.os.HandlerThread.run(HandlerThread.java:61)
            01-01 07:57:58.074639   962  1080 D ShutdownThread:     |----com.android.server.ServiceThread.run(ServiceThread.java:46)
            01-01 07:57:58.074789   962  1080 D ShutdownThread: Notifying thread to start shutdown longPressBehavior=1
            01-01 07:57:58.079296   962  1080 D ShutdownThread: IBootAnim get screenTurnOffTime : 5000
            01-01 07:57:58.134895   962  3204 D ShutdownThread: ro.crypto.state: unencrypted
            01-01 07:57:58.137951   962  3204 D ShutdownThread: checkShutdownFlow: IPO_Support=true mReboot=false sys.ipo.disable= isSafeMode=false passEncryptionCondition=true
            01-01 07:57:58.138904   962  3204 D ShutdownThread: checkShutdownFlow: isIPOEnabled=true mShutdownFlow=0
            01-01 07:57:58.145422   962  3204 I ShutdownThread: set launcher as foreground
            01-01 07:57:58.172181   962  3204 I ShutdownThread: Sending shutdown broadcast...
            01-01 07:57:58.501727   962  3204 I ShutdownThread: Shutting down activity manager...
            01-01 07:57:58.710893   962  3204 I ShutdownThread: Shutting down package manager...
            01-01 07:57:58.720345   962  3204 I ShutdownThread: Shutting down radios...
            01-01 07:57:58.722303   962  3220 W ShutdownThread: task run
            01-01 07:57:58.725504   962  3220 W ShutdownThread: Turning off cellular radios...
            01-01 07:57:58.744406   962  3220 I ShutdownThread: Waiting for NFC, Bluetooth and Radio...
            01-01 07:57:59.247395   962  3220 I ShutdownThread: Radio turned off.
            01-01 07:57:59.248435   962  3220 I ShutdownThread: NFC, Radio and Bluetooth shutdown complete.
            01-01 07:57:59.249857   962  3204 I ShutdownThread: Shutting down MountService
            01-01 07:58:03.139331   962  1080 D ShutdownThread: setBacklightBrightness: Off
        
        }


    修改：
        1.关机的判断条件有很多，这里通常电量为0%，或者低于关机电压
        一般上报电量0%的时候，上层会下达关机的指令，然后层层调用进行关机，有时候也会是检测到关机条件后底层直接关机的
        SHUTDOWN_GAUGE0这个是不支持0%关机，只是电压低于关机电压后关机这个条件
        SHUTDOWN_GAUGE0_VOLTAGE这个定义的关机电压

        2. fg_cust_data.pseudo1_t0 = UNIT_TRANS_100 * g_FG_PSEUDO1_T0[g_fg_battery_id];
        这个根据battery_id选择底层多少的偏移量相当于上层的1% ，100%
        //Gionee <gn_by_charging> <lilubao> <20170518> add for change charging process begin
        /* 0~0.5V for battery 0, 0.5~1V for battery 1*/
        /* 1~1.5V for battery 2, -1 for the last one (battery 3) */
        int g_battery_id_voltage[TOTAL_BATTERY_NUMBER] = {
            500000, 1000000, 1500000, -1};

        int g_FG_PSEUDO1_T0[TOTAL_BATTERY_NUMBER] = { 1, 6, 7, 8};		// 5->1
        int g_FG_PSEUDO1_T1[TOTAL_BATTERY_NUMBER] = { 2, 10, 11, 12};	// 9->2
        int g_FG_PSEUDO1_T2[TOTAL_BATTERY_NUMBER] = { 3, 14, 15, 16};	// 13->3
        int g_FG_PSEUDO1_T3[TOTAL_BATTERY_NUMBER] = { 5, 18, 19, 20};	// 17->5
        int g_FG_PSEUDO1_T4[TOTAL_BATTERY_NUMBER] = { 7, 22, 23, 24};	// 21->7

        int g_FG_PSEUDO100_T0[TOTAL_BATTERY_NUMBER] = { 99, 98, 98, 97};// 98->99
        int g_FG_PSEUDO100_T1[TOTAL_BATTERY_NUMBER] = { 98, 95, 94, 93};// 95->98	
        int g_FG_PSEUDO100_T2[TOTAL_BATTERY_NUMBER] = { 96, 90, 90, 89};// 90->96
        int g_FG_PSEUDO100_T3[TOTAL_BATTERY_NUMBER] = { 93, 80, 86, 85};// 80->93
        int g_FG_PSEUDO100_T4[TOTAL_BATTERY_NUMBER] = { 93, 80, 82, 81};// 80->93	
        //Gionee <gn_by_charging> <lilubao> <20170510> add for change charging process end
    }




    开机充电过程恒流的充电电流变化不稳定，没有达到预定要求    
    {
        ****需要定位这些参在哪里改，编译的脚本，需要的文件

        具体说就是刚开始可以达到1.9A，但是充了一段时间后充电电流下降到1.6A左右，开机电压偏高

        1.开机电压偏高 3711mV，电压都上3.7V了，开机电压偏高
        这个是都去开机时候的电压不是开机要求的临界值，插上充电器电池电压有可能偏高在3.5V左右算正常
        修改开机电压BATTERY_LOWVOL_THRESOLD
        mt_battery.c    ->   check_bat_protect_status


        2.充电电流不能稳定在1.8A左右
        充电电流大部分时间维持在1.85A左右，这里修改了电池容量

        
        3.充电显示达到100%的时候电流还是很大，从这点看出软件设置的电池容量应该比实际的容量小
        实际情况应该是充电如果达到100%肯定处于恒压阶段，充电电流很小，充电达到100%但是电流很大
        明显电量没有达到100%，电池设定的容量偏小

        之前17W05A上还有一个BUG是电池很快就截止了，90%~95%就截止，这种情况跟上面相反，电池
        设定的容量比实际的大
        mtk_battery_table.h
        修改
        {
            int g_Q_MAX_T0[TOTAL_BATTERY_NUMBER] = { 2946, 2712, 2490, 1965};   // 2946->3100
            int g_Q_MAX_T1[TOTAL_BATTERY_NUMBER] = { 2796, 2851, 2468, 1984};   // 2796->2936
            int g_Q_MAX_T2[TOTAL_BATTERY_NUMBER] = { 2718, 2432, 2310, 1946};   // 2718->2854
            int g_Q_MAX_T3[TOTAL_BATTERY_NUMBER] = { 2535, 1991, 1858, 1873};   // 2535->2662
            int g_Q_MAX_T4[TOTAL_BATTERY_NUMBER] = { 2523, 1960, 1843, 1851};   // 2523->2649

            int g_Q_MAX_T0_H_CURRENT[TOTAL_BATTERY_NUMBER] = { 2646, 2412, 2190, 1665}; 
            int g_Q_MAX_T1_H_CURRENT[TOTAL_BATTERY_NUMBER] = { 2496, 2551, 2168, 1684};
            int g_Q_MAX_T2_H_CURRENT[TOTAL_BATTERY_NUMBER] = { 2418, 2132, 2010, 1646};
            int g_Q_MAX_T3_H_CURRENT[TOTAL_BATTERY_NUMBER] = { 2235, 1691, 1558, 1573};
            int g_Q_MAX_T4_H_CURRENT[TOTAL_BATTERY_NUMBER] = { 2223, 1660, 1543, 1551};
        }


      
        4.电量计系数偏大，偏小(电量计是用来计算电池准确的电量的一个参数，是为了让显示的尽量准确)     
            mt6757_battery_prop.dtsi -> CAR_TUNE_VALUE
            一起测量电量计系数的条件：
                两个电源，一路供电4V电压，维持手机正常工作，一路给电量计采样电阻两端供1A的电流，这时候看手机显示的
                电流是多少。
                大于1A，电量计系数偏大，小于1A，电量计系数偏小

            从log分析充电到100%的时候充电电流的条件：
                如果充到100%的时候还有很大的充电电流，电量计系数可能偏大。
                充到100%是电量计计算的100%，如果还有很大的电流可能是电池还没充满，认为的电池容量(偏小) * 电量计系数（偏大） = 实际的电池容量 

         修改：
            //Gionee <gn_by_charging> <lilubao> <201700518> add for change charging process begin
            CAR_TUNE_VALUE = <(95)>;	/*	Tune value for current measurement.	101->95	*/
            //Gionee <gn_by_charging> <lilubao> <201700518> add for change charging process end                         


        5.电池容量，充到100%，截止
            充到100%，软件显示的充满，这里面既有参数的偏差也考虑了用户的体验(底层上层映射)，充电截止则是真正的电池充满（电池容量）
       所以如果充到100%跟充电截止差距很大，充到100%跟充电截止之间时间差不能大于30min，实际情况充到100%应该处于恒压，充电电流也应该很小
        充到100%的时候充电电流应该在电池容量的10%~15%，0.1C


        6.放电到关机，看1%电池电压，耗到关机
            如果放电到1%，关机后电池电压还是很大，则用户还有很大的电量没有用到。            
        1%，两端对称，用户的电是用完的，放电关机后的电池电压不能太高



    调用流程    
    电池相关参数的初始化，这部分跟电池参数相关的应该是之前的meter那一类
    （mtk_battery.c）battery_probe 电池参数的初始化 -> (mtk_battery_hal.c)battery_meter_ctrl = bm_ctrl_cmd,电池电量参数相关计算的一组函数接口

    -> fg_custom_init_from_header 从头文件中获得电池参数 -> battery_update_routine,这是一个线程，是负责检测硬件电池参数变化，然后上报给power_supply

    根据debug等级可以调整轮训时间 bat_get_debug_level() >= BMLOG_DEBUG_LEVEL -> 后面是一串中断回调函数，都是跟电量计相关的 -> (mtk_power_misc.c)
    
    mtk_power_misc_init ,难道是power相关的misc ，这个地方执行了很多关机条件检测 ，shutdown_event_handler这个是关机检测执行的回调函数 


    fg这两个中断干什么？
    {
        fg_iavg_int_ht_handler      FG_IAVG_H_NO
        fg_iavg_int_lt_handler      FG_IAVG_L_NO

        应该是跟high，low相关的
        电量增加减少变化
    }

}
	









/****************************************************************************************************************/
6.通过哪个变量或函数可以判断用户处在打电话状态
{
	这个应该是创建一个节点，如果是打电话，上层会写这个节点，通知底层用户正在打电话

	unsigned int g_call_state = CALL_IDLE;
	accdet.h定义
	#define CALL_IDLE 0
	#define CALL_RINGING 1
	#define CALL_ACTIVE 2

	battery_common.h定义(最终调用的应该是这个)
	#define CALL_IDLE (0)
	#define CALL_ACTIVE (1)

	static ssize_t show_Charging_CallState(struct device *dev, struct device_attribute *attr, char *buf)
	{
		battery_log(BAT_LOG_CRTI, "call state = %d\n", g_call_state);
		return sprintf(buf, "%u\n", g_call_state);
	}

	static ssize_t store_Charging_CallState(struct device *dev, struct device_attribute *attr,
						const char *buf, size_t size)
	{

		if (kstrtouint(buf, 10, &g_call_state) == 0) {
			battery_log(BAT_LOG_CRTI, "call state = %d\n", g_call_state);
			return size;
		}

		/* hidden else, for sscanf format error */
		{
			battery_log(BAT_LOG_CRTI, "  bad argument, echo [enable] > current_cmd\n");
		}

		return 0;
	}

	static DEVICE_ATTR(Charging_CallState, 0664, show_Charging_CallState, store_Charging_CallState);


	#if defined(STOP_CHARGING_IN_TAKLING)
	static PMU_STATUS mt_battery_CheckCallState(void)
	{
		PMU_STATUS status = PMU_STATUS_OK;

		if ((g_call_state == CALL_ACTIVE) && (BMT_status.bat_vol > V_CC2TOPOFF_THRES))
			status = PMU_STATUS_FAIL;

		return status;
	}
	#endif


	/* phone call last than x min */
	if (g_call_state == CALL_ACTIVE
	    && (bat_time_after_sleep.tv_sec - g_bat_time_before_sleep.tv_sec >=
		TALKING_SYNC_TIME)) {
		//Gionee GuoJianqiu 20150103 modify for GNSPR00028091 begin
		SOC = battery_meter_get_battery_percentage();	
		if(BMT_status.charger_exist == KAL_FALSE && BMT_status.UI_SOC > SOC) 
			{
				BMT_status.UI_SOC = battery_meter_get_battery_percentage();
			}										
		//Gionee GuoJianqiu 20150103 modify for GNSPR00028091 end
		battery_log(BAT_LOG_CRTI, "Sync UI SOC to SOC immediately\n");
	}


}








/**********************************************************************************************/
7.将电池维护代码移植到17G05A上
{

	主要涉及的文件
	mtk_switch_charging.c
		mtk_switch_charging_run充电状态机的调整
    

    mtk_battery.c
    init.mt6757.rc 创建节点，上层读写这个节点设置调整的充电范围
    mtk_battery.h 这个是变量声明的地方
}













/**********************************************************************************************/
8.温升相关问题
{
    1.根据当前的温度简单的限定充电电流
    {
        低温（小于15）：1.5A
        正常温度（25左右）：2A
        高温（大于45）：1A

        mtk_switch_charging.c -> swchg_select_charging_current_limit

        这其中还要考虑其他情况：打电话，mmi测试等等（1605上是这样考虑）


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
	}



    2.充电温度调节策略相关代码，文档  
    {
        mtk_cooler_bcct_v1.c这个文件应该充电调节温度有关
        （mtk_cooler_bcct_v1.c）mtk_cooler_bcct_init初始化充电温升 降电流 -> mtk_cooler_bcct_register_ltf这个是注册一个中断，设置回调函数

        struct chrlmt_handle{chr_input_curr_limit;bat_chr_curr_limit;pep30_input_curr_limit;}当温度达到某个状态的时候设置限定的充电电流
        
        -> 这边注册了三个降温相关的策略 bcct，abcct，lcmoff的中断 -> 创建proc file节点 时时反应设备的状态 -> chrlmt_set_limit_handler

        触发条件就设置限定电流的回调函数 -> charger_manager_set_input_current_limit 通过这个函数设置进充电器的和进电池的 

	 （mtk_thermal_monitor.c） mtkthermal_init，mtk thermal相关的初始化，创建调试调用节点/proc/driver/thermal，在这个目录下

      建立一系列的proc 节点，/proc/mtkcooler这个目录下是降温策略的设备节点 -> (mtk_cooler_shutdown.c) mtk_cooler_shutdown_init

      这个应该跟过温启动关机策略相关的，温度过高触发条件，关机策略应该是最直接，最有效的，但是影响很大，后面还有几个相关的模块的初始化，都是在proc目录下

      创建一系列节点，然后注册函数，相关的操作函数指针 -> (mtk_ts_cpu.c) tscpu_init注册驱动,这个对整个系统的温升有很大影响，tscpu是一个虚拟的thermal_zone，
      
      主要是监控cpu的状态 -> tscpu_thermal_probe 


        device/mediatek/mt6757/thermal.conf这个是温升调节策略相关的配置文件,这个文件参数的意思


        bcct,abcct
         1、bcct：Thermal config tool中bcct策略最多设置3个温度点分别调用3种充电电流，当温度触发条件满足的时候直接降电流
         2、abcct：Thermal config tool中abcct策略是使用当前板温与目标板温的差值计算下一时刻要调节的充电电流，循环调节直到当前板温=目标板温。
         可设定充电电流的最大值和最小值。

        Note：bcct和abcct两套机制可以共存，如两套机制同时生效，则取较小的充电电流值进行调节
    }  
        

    3.相关的文档
    {
        Thermal_Management_MT6757.pdf

        MT6757CH(CD)_Thermal_Design_Notices_V0.1.pdf
    }



	4.modify code
	{
	  (mtk_switch_charging.c)  

	  	判断是否该降电流，主要是灭屏，mmi，关机充电不能降电流
		//Gionee <GN_BSP_CHG> <lilubao> <20170819> add for thermal charging begin
		bool bat_charge_current_not_down(void)
		{
			int gn_boot_mode ;
			gn_boot_mode=get_boot_mode();

			pr_err("in [%s] gn_boot_mode->%d,gn_call_state->%d,gn_screenon_time->%d\n",
						__FUNCTION__,gn_boot_mode,gn_call_state,gn_screenon_time);
			
			if( (g_gn_screenon_time <= 30) && (gn_call_state == CALL_IDLE)) || (is_enter_mmi_test == true) || (gn_boot_mode == KERNEL_POWER_OFF_CHARGING_BOOT || gn_boot_mode == LOW_POWER_OFF_CHARGING_BOOT))
			{
				return true;
			}    
			else{
				return false;
			}
				
		}
		//Gionee <GN_BSP_CHG> <lilubao> <20170819> add for thermal charging end

		这里主要是判断条件，同时控制充电电流
		//Gionee <GN_BSP_CHG> <lilubao> <20170819> add for thermal charging begin
		pr_err("in [%s] gn_call_state->%d,temperature->%d,is_enter_mmi_test->%d,gn_screenon_time->%d\n",
				__FUNCTION__,gn_call_state,info->battery_temperature,is_enter_mmi_test,gn_screenon_time);

		if( bat_charge_current_not_down() ){

			pr_err("normal mode,not change any action\n");
		}else{

		/* 但是大于45度的时候电流还是很大，所以要调小点
		17G10A 电芯规格书，请确认一下配机参数。重点关注一下充电参数。
		0~15，0.3C，MAX to 4.2 V cutoff		900mA
		15~45 2.5A to 4.2V ,2A to 4.4V ,then CV to 0.02C
		45~60 0.5C MAX to 4.2V				1500mA
		*/
			if( info->battery_temperature <=15 ){
				pdata->charging_current_limit=900000;
				pdata->input_current_limit=1000000;
				pr_err("temperature is too low ,charging_current_limit->%d,or is_enter_mmi_test is disable\n",
							pdata->charging_current_limit);
			}else if( (info->battery_temperature>15)&&(info->battery_temperature<=45) ){

				pr_err("temperature in normal range\n");
			}else if( info->battery_temperature > 45 ){

				pdata->charging_current_limit=1100000;
				pdata->input_current_limit=1200000;
				pr_err("temperature is too high ,charging_current_limit->%d\n",pdata->charging_current_limit);
			}

			if( gn_call_state != CALL_IDLE ){

				pr_err("calling state or normal use when the mobile was charging ,decrease charging current\n");

				pdata->charging_current_limit  = pdata->charging_current_limit/2;
				pdata->input_current_limit = pdata->input_current_limit/2;
			}


			一定要把温升的判断移到这边，不然灭屏触发了温升还是会降电流，温升触发abcct之后限制的是充进电池的电流
			if (pdata->thermal_charging_current_limit != -1)
				if (pdata->thermal_charging_current_limit < pdata->charging_current_limit)
					pdata->charging_current_limit = pdata->thermal_charging_current_limit;

			if (pdata->thermal_input_current_limit != -1)
				if (pdata->thermal_input_current_limit < pdata->input_current_limit)
					pdata->input_current_limit = pdata->thermal_input_current_limit;
		}
		//Gionee <GN_BSP_CHG> <lilubao> <20170819> add for thermal charging end

	}


	5.添加一些log
	{
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
	}


}







/************************************************************************************************************/
9.17G10A主板显示电池温度50~55，温度过高导致的停止充电问题
{
	NTC faild
	
	在mtk_charger.c  charger_check_status会检查充电的状态
	发现电池的温度为50度超过了最大的充电温度，启动保护措施，然后就停止充电了
	(但是这里电池温度没有50度，检测可能有问题，先提高充电的最大温度)
	这个值在dts文件里获取的


	modify code
	{
		(mt6757.dtsi) 这里应该注意的是最高温，最低温的控制，温度变化要有一定的缓冲区，不然变化会很频繁
		要记录一个之前的状态变量和当前的状态变量
		/* Battery Temperature Protection */
        mtk_temperature_recharge_support = <1 >;
        //Gionee <gn_by_charging> <lilubao> <20170621> add fixed #87763 begin
        max_charge_temperature = <55 >;            // 50->55
        //Gionee <gn_by_charging> <lilubao> <20170621> add fixed #87763 end
        max_charge_temperature_minus_x_degree = <47 >;
        min_charge_temperature = <0 >;
        min_charge_temperature_plus_x_degree = <6 >;
        err_charge_temperature = <0xff >;

		(mtk_charger.c）
		if (thermal->enable_min_charge_temperature) {
            if (temperature < thermal->min_charge_temperature) {
                pr_err("[BATTERY] Battery Under Temperature or NTC fail %d %d!!\n\r", temperature,
                    thermal->min_charge_temperature);
                thermal->sm = BAT_TEMP_LOW;
                charging = false;
                goto stop_charging;
            } else if (thermal->sm == BAT_TEMP_LOW) {
                if (temperature >= thermal->min_charge_temperature_plus_x_degree) {
                    pr_err("[BATTERY] Battery Temperature raise from %d to %d(%d), allow charging!!\n\r",
                            thermal->min_charge_temperature,
                            temperature,
                            thermal->min_charge_temperature_plus_x_degree);
                    thermal->sm = BAT_TEMP_NORMAL;
                } else {
                    charging = false;
                    goto stop_charging;
                }
            }
        }


        if (temperature >= thermal->max_charge_temperature) {
            pr_err("[BATTERY] Battery over Temperature or NTC fail %d %d!!\n\r", temperature,
                thermal->max_charge_temperature);
            thermal->sm = BAT_TEMP_HIGH;
            charging = false;
            goto stop_charging;
        } else if (thermal->sm == BAT_TEMP_HIGH) {
            if (temperature < thermal->max_charge_temperature_minus_x_degree) {
                pr_err("[BATTERY] Battery Temperature raise from %d to %d(%d), allow charging!!\n\r",
                        thermal->max_charge_temperature,
                        temperature,
                        thermal->max_charge_temperature_minus_x_degree);
                thermal->sm = BAT_TEMP_NORMAL;
            } else {
                charging = false;
                goto stop_charging;
            }
        }
	}
}

















/************************************************************************************************************/
10.17G10A mmi测试读取的数据有问题
{
	mmi测试读取的节点在（这几个接口有问题？）
	sys/class/power_supply/battery/

	BatteryAverageCurrent  平均电流为0？
	ChargerVoltage InstatVolt      TemperatureR batt_temp capacity     

	device power   present_smb status_smb technology uevent 

	BatterySenseVoltage         ISenseVoltage  TempBattVoltage adjust_power batt_vol  

	capacity_smb health present status      subsystem  type  

	//17G10A    要加一个POWER_SUPPLY_PROP_BatteryPresentCurrent节点
	上面那一组变量属于power_supply的特性(mtk_battery.c)
	static enum power_supply_property battery_props[] = {
		POWER_SUPPLY_PROP_STATUS,
		POWER_SUPPLY_PROP_HEALTH,
		POWER_SUPPLY_PROP_PRESENT,
		POWER_SUPPLY_PROP_TECHNOLOGY,
		POWER_SUPPLY_PROP_CAPACITY,
		/* Add for Battery Service */
		POWER_SUPPLY_PROP_batt_vol,
		POWER_SUPPLY_PROP_batt_temp,
		/* Add for EM */
		POWER_SUPPLY_PROP_TemperatureR,             // 7
		POWER_SUPPLY_PROP_TempBattVoltage,          // 8
		POWER_SUPPLY_PROP_InstatVolt,               // 9
		POWER_SUPPLY_PROP_BatteryAverageCurrent,    // 10
		POWER_SUPPLY_PROP_BatterySenseVoltage,      // 11
		POWER_SUPPLY_PROP_ISenseVoltage,            // 12
		POWER_SUPPLY_PROP_ChargerVoltage,           // 13
		/* Dual battery */
		POWER_SUPPLY_PROP_status_smb,
		POWER_SUPPLY_PROP_capacity_smb,
		POWER_SUPPLY_PROP_present_smb,
		/* ADB CMD Discharging */
		POWER_SUPPLY_PROP_adjust_power,
	};

	//这些变量后面会在sys节点下呈现
	void battery_update_psd(struct battery_data *bat_data)
	{
		bat_data->BAT_batt_vol = battery_get_bat_voltage();
		bat_data->BAT_InstatVolt = bat_data->BAT_batt_vol;
		bat_data->BAT_BatterySenseVoltage = bat_data->BAT_batt_vol;
		bat_data->BAT_batt_temp = battery_get_bat_temperature();
		bat_data->BAT_TempBattVoltage = battery_meter_get_tempV();
		bat_data->BAT_TemperatureR = battery_meter_get_tempR(bat_data->BAT_TempBattVoltage);
		bat_data->BAT_BatteryAverageCurrent = battery_get_ibus();//ibus返回0，不用这个
		bat_data->BAT_ISenseVoltage = battery_meter_get_VSense();
		bat_data->BAT_ChargerVoltage = battery_get_vbus();
	}

	打印log用的这些函数接口
	pr_err("Vbat=%d,I=%d,VChr=%d,T=%d,Soc=%d:%d,CT:%d:%d\n", battery_get_bat_voltage(),
		curr_sign ? bat_current : -1 * bat_current,
		battery_get_vbus(), battery_get_bat_temperature(),
		battery_get_bat_soc(), battery_get_bat_uisoc(),
		mt_get_charger_type(), info->chr_type);

	/*debug*/    
	{
		1.节点数据没有及时上报，导致有的数据一直显示初始化时候的值            
			battery_update_psd获取充电，电池的数据然后赋值给power_supply子系统，battery_main数据结构

			在battery_update函数里面添加 battery_update_psd，每隔10s获取的数据可以上报上去

		2.添加时时的充电电流数据这个选项
		POWER_SUPPLY_PROP_BatteryPresentCurrent
		val->intval = data->BAT_BatteryPresentCurrent

		power_supply_sysfs.c和power_supply.h里面要添加properity特性
		要把这个节点特性呈现到sysfs里面

		3.mmi测试读取的数据有问题，可能是读取的节点不对
		{
			power_supply 节点路径
			充电电流：  /sys/class/power_supply/battery/Battery/BatteryPresentCurrent
			充电器电压：/sys/class/power_supply/battery/Battery/ChargerVoltage
			电量：     /sys/class/power_supply/battery/Battery/capacity
			电池电压：  /sys/class/power_supply/battery/Battery/BatterySenseVoltage
			电池技术：  /sys/class/power_supply/battery/Battery/technology
			电池温度：  /sys/class/power_supply/battery/Battery/batt_temp


			1.mmi显示充电电流数据应该少一位
				电池温度应该不用*10，虽然不需要温度的数据，这个数据power_supply上报的原本这个数据
				尽量不要做修改，谁需要，谁去修改
				val->intval = data->BAT_batt_temp ;// remove *10
				充电电流数据应该要除10，不然好几安，太大了	
				val->intval = data->BAT_BatteryPresentCurrent/10;	
				
				
			2.分析log，mmi测试走了哪些流程
				mmi测试的log主要在mainlog里面可以找到一些流程

				
			3.对比G1605A 还有哪些是缺少的
				a.缺少enter_mmi_test,exit_mmi_test这两个节点
				已在mtk_batteruy.c 文件里面添加了两个节点
				
				b.  is_enter_mmi_test
				这个变量使用来告诉充电其他部分现在正在进行mmi测试，不用降电流等操作
		
				c.刚插入充电器的时候，检测到中断，充电器检测线程会在刚开始的15s内，每秒都会更新
				需要在相关的线程里面添加这个判断，并调用gn_update_BatteryPresentCurrent
				
		}



		modify code
		{
			1.往power_supply的属性里面提供新的属性
			power_supply_property -> POWER_SUPPLY_PROP_BatteryPresentCurrent, 

			2.修改函数街口，添加新的数据
			//battery_get_ibus -> pmic_get_ibus return 0 这个函数返回值就是0，没意义
			//bat_data->BAT_BatteryAverageCurrent = battery_get_ibus();
			bat_data->BAT_BatteryAverageCurrent = battery_get_bat_avg_current();
			bat_data->BAT_BatteryPresentCurrent = battery_get_bat_current();

			3.数据没有及时上报，所以要在线程里面添加上报
			static void battery_update(struct battery_data *bat_data)
			{
				struct power_supply *bat_psy = bat_data->psy;

				bat_data->BAT_TECHNOLOGY = POWER_SUPPLY_TECHNOLOGY_LION;
				bat_data->BAT_HEALTH = POWER_SUPPLY_HEALTH_GOOD;
				bat_data->BAT_PRESENT = 1;

				//Gionee <gn_by_charging> <lilubao> <20170619> add fixed mmi begin	
				battery_update_psd(bat_data);
				//Gionee <gn_by_charging> <lilubao> <20170619> add fixed mmi end

			#if defined(CONFIG_POWER_EXT)
				bat_data->BAT_CAPACITY = 50;
			#endif
				power_supply_changed(bat_psy);
			}

			4.mmi测试刚开始插入充电器，数据上报应该快点
			mtk_charger.c    charger_routine_thread
			检测到有充电器插入，前10s加快上报的速度，后面10s上报一次
			static bool mtk_is_charger_on(struct charger_manager *info)
			{
				CHARGER_TYPE chr_type;
				//Gionee <gn_by_charging> <lilubao> <20170705> add for platform change begin
				static unsigned int gn_update_counter = 0;
				pr_err("in [%s],by lilubao\n",__FUNCTION__);
				//Gionee <gn_by_charging> <lilubao> <20170705> add for platform change end
					
				chr_type = mt_get_charger_type();
				if (chr_type == CHARGER_UNKNOWN) {
					if (info->chr_type != CHARGER_UNKNOWN)
						mtk_charger_plug_out(info);
				} else {
					if (info->chr_type == CHARGER_UNKNOWN)
						mtk_charger_plug_in(info, chr_type);
				}

				//Gionee <gn_by_charging> <lilubao> <20170705> add for platform change begin
				if( (upmu_is_chr_det() == true)&&(is_enter_mmi_test==1))
				{
					//check_battery_exist();		
					pr_err("enter mmi test ,accelate update param\n");
					if (!(g_platform_boot_mode == KERNEL_POWER_OFF_CHARGING_BOOT || g_platform_boot_mode == LOW_POWER_OFF_CHARGING_BOOT))
					{
						if(++gn_update_counter<15){
							pr_err("gn_update_counter->%d\n",gn_update_counter);
							gn_update_BatteryPresentCurrent();
						}    
					}else{
						gn_update_counter = 0;
					}
				}else {

					gn_update_counter=0;
					pr_err("unknown stop accelate update\n");
				}	
				//Gionee <gn_by_charging> <lilubao> <20170705> add for platform change end


				if (chr_type == CHARGER_UNKNOWN)
					return false;

				return true;
			}

		}    
	}
}   













/*****************************************************************************************/
11.meta模式，写号插假电要接NTC电阻才行
{

	这个问题的原因是天线校准用的夹具范围大，导致TEMP就是NTC的测试点无法接上
	而这段在进入meta模式的时候并没有修改，要检测NTC电阻才能进入meta。
	
	/******************************************************************/
	修改：preloader   charging_bat_mt6355.c
	
	没有电池但是return 1 认为有电池
	//这个地方就是检测电池是否存在 ，meta
	int hw_check_battery(void)
	{
		/* ask shin-shyu programming guide */

		#ifdef MTK_DISABLE_POWER_ON_OFF_VOLTAGE_LIMITATION
			print("ignore bat check !\n");
			return 1;
		#else
			#if CFG_EVB_PLATFORM
				print("ignore bat check\n");
				return 1;
			#else
				U32 val=0;
				U32 ret_val;

		ret_val=pmic_config_interface( (U32)(MT6355_LDO_VBIF28_CON0),
									(U32)(1),
									(U32)(PMIC_RG_LDO_VBIF28_EN_MASK),
									(U32)(PMIC_RG_LDO_VBIF28_EN_SHIFT)
									);

		//这个是使能ADC去检测将外置的供应作为输入源，去检测
		mt6355_upmu_set_rg_baton_en(1);
		/*mt6355_upmu_set_baton_tdet_en(1);*//*--After 6335, it's unnecessary--*/
		/*mdelay(100);*/
		//这个是检测电池 返回1是没有电池，0有电池
		val = mt6355_upmu_get_rgs_baton_undet();


		if(val == 0)
		{
			print("bat is exist.\n");
			return 1;
		}
		else
		{
			//Gionee <gn_by_charging> <lilubao> <20170702> add for platform change begin
			print("bat NOT exist,but return 1\n");
			return 1;
			//Gionee <gn_by_charging> <lilubao> <20170702> add for platform change end  
		}
			#endif
		#endif

	}


	
	MTK_DISABLE_POWER_ON_OFF_VOLTAGE_LIMITATION 这个宏是干什么的，包含哪些代码
	
	关闭电池是否存在才能开关机的限制,打开这个宏直接跳出检查
	这个宏在preloader,lk,kernel三个地方后有检测，判断。
	
	preloader		charging_bat_mt6355.c
	lk				mt_battery_6355.c
	kernel 			
	
	电池检测，NTC检测有哪些过程
	{
		这里是直接都写pmic的寄存器的值，返回1没有电池，返回0有电池
		
		mt6355_upmu_set_rg_baton_en(1);
		val = mt6355_upmu_get_rgs_baton_undet();
	}


	还有两处，这是pmic寄存器上电池存在与否的判断，判断还是要有的，但是不关机
	mt_battery_6355.c  check_bat_protect_status
	/*pmic_set_register_value(PMIC_BATON_TDET_EN, 1);*/
	pmic_set_register_value(PMIC_RG_BATON_EN, 1);
	
	//Gionee <GN_BSP_CHG> <lilubao> <20170830> modify for platform change begin
	dprintf(CRITICAL, "[BATTERY] No battry plug-in.but don't power off\n");
		
	if (pmic_get_register_value(PMIC_RGS_BATON_UNDET) == 1) {
		dprintf(CRITICAL, "[BATTERY] No battry plug-in. Power Off.");
		//mt6575_power_off();
		break;
	}
	//Gionee <GN_BSP_CHG> <lilubao> <20170830> modify for platform change end


	mt_battery_6355.c   mt65xx_bat_init
	if (g_boot_mode != META_BOOT && g_boot_mode != FACTORY_BOOT && g_boot_mode != ATE_FACTORY_BOOT) {
		/*pmic_set_register_value(PMIC_BATON_TDET_EN, 1);*/
		pmic_set_register_value(PMIC_RG_BATON_EN, 1);
		//Gionee <GN_BSP_CHG> <lilubao> <20170830> modify for platform change begin
		dprintf(CRITICAL, "[BATTERY] No battry plug-in.but don't power off\n");

		if (pmic_get_register_value(PMIC_RGS_BATON_UNDET) == 1) {
			dprintf(CRITICAL, "[BATTERY] No battry plug-in. Power Off.");
			//mt6575_power_off();
		}
		//Gionee <GN_BSP_CHG> <lilubao> <20170826> modify for platform change end
	}

}













/***********************************************************************************************/
12.	usb眼图问题
{
	近端跟远端的测试要求不一样
	近端的要求肯定高很多，远端的衰减多


	MTK case ID：ALPS03412756


	17G10A相关代码
	{
	 这几个虽然是USB3.0的控制宏但是默认是USB2.0
	 kernel-4.4/drivers/misc/mediatek/mu3d/Kconfig文件中，如果 USB_MU3D_DEFAULT_U2_MODE设为y，
	 表示当前使用的是USB2.0,。敝司平台code默认是关闭USB3.0的，请知悉，感谢！	

		CONFIG_USB_MU3D_DRV=y
		CONFIG_MU3_PHY=y
		CONFIG_U3_PHY_AHB_SUPPORT=y
		CONFIG_USB_MTK_DUALMODE=y
		CONFIG_USB_MTK_IDDIG=y
		CONFIG_USB_XHCI_MTK_SUSPEND_SUPPORT=y
		CONFIG_SND_USB_AUDIO=y

		CONFIG_USB_GADGET=y
		CONFIG_USB_GADGET_VBUS_DRAW=500
		CONFIG_USB_G_ANDROID=y

		kernel-4.4/drivers/misc/mediatek/mu3d/
		kernel-4.4/drivers/misc/mediatek/mu3phy/
	}
	

	modify code
	{
		这两个对应的寄存器和操作的函数
		USB_DP_P0		D31
		USB_DM_P0		D32
	
		这边是从USB转uart这个功能开始的
		1.
		/kernel-4.4/drivers/misc/mediatek/mu3phy/mt6757/Makefile
		23#For USB HQA Driving Tuning Mode 1 Settings
		24#EXTRA_CFLAGS += -DMTK_USB_MODE1 //取消这一行的注释

		2.
		/kernel-4.4/drivers/misc/mediatek/mu3phy/mt6757/mtk-phy-asic.c
		645#if defined(CONFIG_MTK_HDMI_SUPPORT) || defined(MTK_USB_MODE1)
		646    os_printk(K_INFO, "%s- USB PHY Driving Tuning Mode 1 Settings.\n", __func__);
		647 U3PhyWriteField32((phys_addr_t) (uintptr_t) U3D_USBPHYACR5, RG_USB20_HS_100U_U3_EN_OFST,
		648             RG_USB20_HS_100U_U3_EN, 0);
		649    U3PhyWriteField32((phys_addr_t) (uintptr_t) U3D_USBPHYACR1, RG_USB20_VRT_VREF_SEL_OFST,
		650             RG_USB20_VRT_VREF_SEL, 5); //驱动电流调节，这里最大可以设置为7
		651    U3PhyWriteField32((phys_addr_t) (uintptr_t) U3D_USBPHYACR1, RG_USB20_TERM_VREF_SEL_OFST,
		652             RG_USB20_TERM_VREF_SEL, 5); //电压幅度调节，这里最大可以设置为7 

		//Gionee <gn_by_charging> <lilubao> <20170719> add for fixed usb begin
		os_printk(K_ERR, "in [%s] before switch,by lilubao\n", __func__);
		U3PhyWriteField32((phys_addr_t) (uintptr_t) U3D_USBPHYACR5, RG_USB20_HS_100U_U3_EN_OFST,
					RG_USB20_HS_100U_U3_EN, 0);
		U3PhyWriteField32((phys_addr_t) (uintptr_t) U3D_USBPHYACR1, RG_USB20_VRT_VREF_SEL_OFST,
					RG_USB20_VRT_VREF_SEL, 7);// 5->7
		U3PhyWriteField32((phys_addr_t) (uintptr_t) U3D_USBPHYACR1, RG_USB20_TERM_VREF_SEL_OFST,
					RG_USB20_TERM_VREF_SEL, 7);//5->7
		//Gionee <gn_by_charging> <lilubao> <20170719> add for fixed usb end			
	}

}












/********************************************************************************************************************/
13.几种场景下充电温度偏高 -> mtk thermal manager
{
	1.确定thermal是否起作用了，thermal管了那些地方
		因为thermal.mtc 位于device目录而且有好几个地方都有相关的文件
	  	所以限确定项目编译用的是哪个？

		先搜AndroidProducts.mk：定义一个变量——>PRODUCT_MAKEFILES，该变量的值为产品版本定义文件名的列表  
        产品版本定义文件：对特定产品版本的定义（可多个文件，多个版本）。一般情况下，我们不需要定义所有变量（版本相关），，Build系统已经预先定义了一些组合，
		位于build/target/product，该目录下每个文件都定义了一个组合，我们只需要继承这些预置的定义，然后再覆盖自己想要的变量定义即可
        BroadConfig.mk：该文件用来配置硬件主板，它定义的都是设备底层的硬件特性，如设备的主板相关信息，wifi，bootloader，内核等

		最后确定thermal 用的gionee_bj这个目录，之前一直以为是mediatek mt6757目录下面
		而且编译要全编


		这是编译之后生成的项目脚本
		device/gionee_bj/gnbj6757_66_n/ProjectConfig.mk


     2.相关参数什么意思，怎么改？
	 首先跟电流相关的很重要的两个概念abcct，bcct，根据以上两个设定chrlimt
	 {
		bcct:battery charging current throtting
		这个是在触发温度条件后就设定充电电流
		abcct:adaptive battery charging current throtting
		这个是在设定的目标温度的时候在最高和最低温度范围内动态调节电流
		
	 }

	abcct_lcmoff 是在lcm off的时候启动的，如果灭屏的情况下温度过高设定的策略 
	 * sscanf format <klog_on> <mtk-cl-bcct00 limit> <mtk-cl-bcct01 limit> ...
	 * <klog_on> can only be 0 or 1
	 * <mtk-cl-bcct00 limit> can only be positive integer or -1 to denote no limit


    相关参数的意思
	{	
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
	}
}














/*******************************************************************************************************************************/
14.	电量显示不正确  -> 要熟悉电池相关参数的上报过程
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













/********************************************************************************************************************************/
15.过压测试：NG，10V时电压不不截止，还有380ma~440ma，无电压过高提醒
{
	1.大致过程是先开log，然后将充电电压缓慢拧到7V～10V 然后拧到有过压提示的时候，然后分析log


	2.过压的时候要停止充电，还要上报状态显示过压
	{
		软件要设定检测到充电电压大于6.5v字左右要停止充电
	
		rt5081那几个关于ovp的中断和中断函数是否被调用
		打印出那几个寄存器的值
		
		CHG_VIN:16.5v		VBUS:14.5v

		uvp/ovp
		0x0e  bit[7:4]   uvp ,bit[3:0]ovp
		
		rt5081_pmu_chg_vinovp_irq_handler
		rt5081_pmu_chg_vbusov_irq_handler    CHARGER_DEV_NOTIFY_VBUS_OVP
		
		rt5081_pmu_ovpctrl_ovp_d_evt_irq_handler
		rt5081_pmu_ovpctrl_ovp_evt_irq_handler
		
		0xc8  ovpctrl_irq
	
			
	
		UUG P37  0x1d 这个是断开充电同时断开power path功能

		rt5081有ovp功能，但是没有接那个脚,这个是内接的，ovp设定的电压是14.5V，而且不能修改
	}
	
	
	3,最后在mtk_charger.c 里面有几个notify，高低温，过压的判断加log，发现是
	数据位数不对应导致判断一直过不去，始终相差三个数量级
	{
		vchr = pmic_get_vbus();
		if (vchr > info->data.max_charger_voltage) {
			info->notify_code |= 0x0001;
			pr_err("[BATTERY] charger_vol(%d) > %d mV\n",
				vchr, info->data.max_charger_voltage);


		6500000 
			11789	
		
			6.500 000    
		11.789v
		
		
		4500000
		13400000
	}


	4.	停止充电之后还有power path功能，充电器直接给系统供电，应该直接关闭这个功能
	不进充电器也不进系统
	CHG_EN 0x12 bit[0]
	

	5.modify code
	{	
		首先是两个变量的数量级不一样，导致判断一直进不去，然后一点是现在的pmic很多都有power path功能，
		给电池充电同时也会给系统供电，所以关闭给电池充电的时候还要关闭power path

		(mtk_charger.c)  mtk_battery_notify_VCharger_check
		//Gionee <gn_by_charging> <lilubao> <20170712> add for debug ovp begin
		// difference degree between vchr and charger_voltage
		if (vchr*1000 > info->data.max_charger_voltage) {
			info->notify_code |= 0x0001;
			pr_err("[BATTERY] charger_vol(%d) > %d mV\n",
				vchr*1000, info->data.max_charger_voltage);
		} else {
			info->notify_code &= ~(0x0001);
		}
		if (info->notify_code != 0x0000)
			pr_err("[BATTERY] BATTERY_NOTIFY_CASE_0001_VCHARGER (%x)\n",
				info->notify_code);
		//Gionee <gn_by_charging> <lilubao> <20170712> add for debug ovp end

		(mtk_charger.c) charger_check_status
		//Gionee <gn_by_charging> <lilubao> <20170712> add for debug ovp begin
		int vchr=0;
		bool en=true;
		struct charger_device *chg_dev;
		
		struct battery_thermal_protection_data *thermal;
		
		chg_dev = info->chg1_dev;
		//Gionee <gn_by_charging> <lilubao> <20170712> add for debug ovp end

			//Gionee <gn_by_charging> <lilubao> <20170712> add for debug ovp begin
			vchr = pmic_get_vbus();
			//pr_err("vchr->%d,info->data.max_charger_voltage->%d\n",vchr*1000,info->data.max_charger_voltage);
			if(vchr*1000 > info->data.max_charger_voltage){

				pr_err("vchr->%d,info->data.max_charger_voltage->%d\n",vchr*1000,info->data.max_charger_voltage);
				charging = false;
				en=false;
				pr_err("charging->%d,en->%d\n",charging,en);
				
				goto stop_charging;
			}else {
				charging = true;
				en=true;
				pr_err("charging->%d,en->%d\n",charging,en);
				goto stop_charging;
			}
			//Gionee <gn_by_charging> <lilubao> <20170712> add for debug ovp end

		charger_dev_enable_powerpath(chg_dev, en);
		mtk_battery_notify_check(info);

	}
}
















/********************************************************************************************************************************/
16.	修改需要插入sim卡才能激活USB
{
	#Gionee <gn_by_charging> <lilubao> <20170718> add for fixed trigger usb begin
	GN_RO_GN_USB_SIMSECURITY_SUPPORT = no
	#Gionee <gn_by_charging> <lilubao> <20170718> add for fixed trigger usb end

	这个是额外加的判断，如果去掉这个宏就没有这个功能，后面应该是根据系统实时判断标志位去决定是否要这个功能而不是一开始就写死
	
}













/**********************************************************************************************************************************/
17.【GMS-CTS Verifier测试】OTG相关测试应该屏蔽
{
	这个device.mk是干什么用的?
	xml文件好像用的很多，有必要弄清楚,这个应该是读取相关的配置然后加载

	cts对权限有要求，权限过高会有安全隐患

	把gionee/code/driver/project_common/BJ17G10_DRV_COMMON/device/gionee_bj/gnbj6757_66_n/device.mk中的下面的permissions注释掉。

	USB OTG
	PRODUCT_COPY_FILES += frameworks/native/data/etc/android.hardware.usb.host.xml:system/etc/permissions/android.hardware.usb.host.xml

}














/**********************************************************************************************************************************/
18.高低温关机问题
{
	首先明确高低温测试要求

	1.高温高湿测试：
	{
		1.手机先关机放在温度57度的温箱里面，一小时左右，大概环境温度稳定在57度，
		2.开机，将手机放在温箱里面待机2h，此时手机要不关机

		
		这中间有几个思考分析的方向
		{
			首先要确定的是关机的原因

			1.手机整体温度跟电池温度差多少
			a.主板温度多少 ,电池温度多少 ,cpu温度多少 -> b.是电池温度过高导致的关机，还调电引起的 ->c. 中间出现瞬时关机现象，但是有没有关机流程，
			cpu温度多少， 是否主板或者电池有问题,
			{	
				a. 搜themral相关的关键字，T_AP主板,加点log
				b. 搜fg_drv_update_hw_status ，确定当时时间点的电池温度，整机电流，之前确实有电池温度达到60度
				c.对比测试，插假电确定电池是否有问题，ntc接地固定确定主板，飞行模式确定整机功耗
			}


			2.NTC是否准确
			{
				这个pmic读的值是准确的的，经过校准后，温度偏低了1度
				(mtk_battery.c)
				force_get_tbat_internal
			}


			3.手机整体功耗偏高
			飞行模式测试过程中没有关机	


			4.对于极端条件下的温升测试
			{
				关闭方法如下：
				adb shell
				thermal_manager vendor/etc/.tp/.ht120.mtc

				可以使用下面的方法来check是否关闭成功
				cat /data/.tp/.settings

				Success: it shows "vendor/etc/.tp/.ht120.mtc"
				Fail: it shows nothing	
			}


			5.mtk提供的方法
			{	
				1.有没有在高温场景抓下uart log看？
				高温条件下，mtklog可能录不到抓不到相关的log

				2.掉电的过程中帮忙量一下vcore 26m vio18的电压，看一下掉电时序
				vcore是buck电路
				26m是给始终供电的
				vio18是给pmic给 射频收发器供电的	
			}
	
		}


		这个在测试的时候没有关机，但是亮屏看温度的时候手机关机，电池温度达到了60度，操作手机一会手机瞬间关机也没有关机流程
		但事实上手机确实是触发电池60度关机的条件的
		
		[  563.736795] <3>.(3)[1293:system_server][name:mtk_ts_battery&][Thermal/TZ/BATTERY][mtktsbattery_get_hw_temp] counter=1, first_time =0

		[  563.736802] <3>.(3)[1293:system_server][name:mtk_ts_battery&][Thermal/TZ/BATTERY][mtktsbattery_get_hw_temp] T_Battery, 60000

		[  563.736853] <3>.(3)[1293:system_server][name:mtk_ts_battery&]Power/battery_Thermal: reset, reset, reset!!!


		这个问题确定是电池跟主板的连接接触多，导致主板很多热传递到电池上导致电池过热，所以第一点很接近，手机整体的温度跟电池的温度
		因为以前是电池跟主板相差2度，但是这次有四五度

		这个是因为电池连接器的正负极跟NTC靠的比较近，而NTC在电池保护板的上，在电池连接器的下面，所以亮屏大电流的时候靠的比较近，温度很容易升高
	}

	


	
	2.低温关机：
	{
		1.手机放在温箱内，温度设置为-20,放置12h后，开机放12h，中间操作手机不能关机
		现在是飞行模式没有关机，插卡没有关机，不插卡却关机

		2.结论
		这其中是待机过程中的系统不稳定，低温环境下，电池活性低容量小，电阻大，如果电流很大，电池电压很容易被拉的很低
		从log上看虽然关机原因是低电量，但是开机过后电池电压恢复到4.1V，如果是消耗的电池电压开机后应该也很低，关机的前前突然出现亮屏
		系统可能被唤醒，电流有500mA左右，持续时间20s+，电池电压被拉低

		所以最好能确定唤醒的原因，电池也需要改版，
		至于唤醒主要的区别在与插卡，不插卡，这个跟modem应该有关
	}


}











/**********************************************************************************************************************************/
19.	fuel gauge3.0相关
{
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
			L3500-Charger】voltage mode在低温下的修正
		}
	}
}









/*******************************************************************************************************************************/
20.	底电流偏高 -> 这个主要是MTK电流的分析思路        未完成  20170924
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
}








/*****************************************************************************************************************************/
21.低电量开启闪光灯，导致照片较黑
{
	94168
	【品质压力】MMI测试，后摄像头拍照，照片显示黑色，再次拍照恢复【验证3台，2台出现3次】


	100297
	【品质压力】桌面进相机，前摄像头开闪光灯，按拍照键单拍照片后进小图库查看，照片显示全黑，再次拍照恢复【验证2台1台出现2次】A16


	case ID:alps03515512

	Dear customer:
	1.请问贵司在设置 预闪 和 主闪 时间的时候是以什么为依据的？
	-----------------闪光灯预闪和主闪的时间主要是根据3A HAl自己决定的，在precapture state 的时候进行预闪，并且计算flash ae/AWB；然后在capture state 状态会打主闪
	2.主闪的时候电流会比较大，请问贵司时候在设置主闪时间的时候，是否会参考电源电量等相关的数据？
	-----------------正常情况下设置主闪的duty是不会参考电源电量等相关的数据的，主闪的duty主要preflash 计算出来的；但是有两个地方回去检测电池电量，然后对应调整flash的行为：
	1)在flash_mgr_m.cpp文件中会通过g_pStrobe->isLowPower(&isLow);接口去获取当前的电池电量，如果为低电量会关闭闪光灯
	2)PMIC也会通过中断的方式定时检测低电量，如果为低电量就会关闭闪光灯！




	该问题在系统相机中也存在，且电量较低的时候出现概率更高。
	引起该现象的原因是：闪光灯打闪的时候AE会设置得很低，然后抓取拍照帧，概率性出现抓取的时候主闪已结束，但AE还没设置回来，导致照片整体偏暗（如果微距拍摄会误认为是全黑）。
	我在闪光灯驱动的disable函数中加log ,发现当出现该现象的时候，被PMIC线程切断了，导致闪光灯主闪持续时间不够长：
	05-03 04:35:17.162253 0 0 I [ 2567.040248] (2)[9632:Cam@P1NodeImp][name:flashlights_rt5081&]: [FLASHLIGHT] rt5081_disable: shenweikun rt5081_disable 《====== 正常log
	05-03 04:35:17.337183 0 0 I [ 2567.215178] (0)[9632:Cam@P1NodeImp][name:flashlights_rt5081&]: [FLASHLIGHT] rt5081_disable: shenweikun rt5081_disable
	05-03 04:35:22.976299 0 0 I [ 2572.854294] (1)[9632:Cam@P1NodeImp][name:flashlights_rt5081&]: [FLASHLIGHT] rt5081_disable: shenweikun rt5081_disable
	05-03 04:35:23.151764 0 0 I [ 2573.029759] (1)[9632:Cam@P1NodeImp][name:flashlights_rt5081&]: [FLASHLIGHT] rt5081_disable: shenweikun rt5081_disable
	05-03 04:35:23.152745 69 69 I [ 2573.030740] (0)[69:pmic_thread][name:flashlights_rt5081&]: [FLASHLIGHT] rt5081_disable: shenweikun rt5081_disable 《====== 异常log
	05-03 04:35:28.759604 0 0 I [ 2578.637599] (3)[9632:Cam@P1NodeImp][name:flashlights_rt5081&]: [FLASHLIGHT] rt5081_disable: shenweikun rt5081_disable
	05-03 04:35:28.932398 0 0 I [ 2578.810393] (2)[9632:Cam@P1NodeImp][name:flashlights_rt5081&]: [FLASHLIGHT] rt5081_disable: shenweikun rt5081_disable
	05-03 04:35:37.408493 0 0 I [ 2587.286488] (2)[9632:Cam@P1NodeImp][name:flashlights_rt5081&]: [FLASHLIGHT] rt5081_disable: shenweikun rt5081_disable
	05-03 04:35:37.584241 0 0 I [ 2587.462236] (2)[9632:Cam@P1NodeImp][name:flashlights_rt5081&]: [FLASHLIGHT] rt5081_disable: shenweikun rt5081_disable
	05-03 04:35:43.465451 0 0 I [ 2593.343446] (3)[9632:Cam@P1NodeImp][name:flashlights_rt5081&]: [FLASHLIGHT] rt5081_disable: shenweikun rt5081_disable
	05-03 04:35:43.645849 0 0 I [ 2593.523844] (2)[9632:Cam@P1NodeImp][name:flashlights_rt5081&]: [FLASHLIGHT] rt5081_disable: shenweikun rt5081_disable
	05-03 04:35:51.008684 0 0 I [ 2600.886679] (2)[9632:Cam@P1NodeImp][name:flashlights_rt5081&]: [FLASHLIGHT] rt5081_disable: shenweikun rt5081_disable
	05-03 04:35:51.220072 0 0 I [ 2601.098067] (4)[9632:Cam@P1NodeImp][name:flashlights_rt5081&]: [FLASHLIGHT] rt5081_disable: shenweikun rt5081_disable
	05-03 04:35:56.661642 0 0 I [ 2606.539637] (3)[9632:Cam@P1NodeImp][name:flashlights_rt5081&]: [FLASHLIGHT] rt5081_disable: shenweikun rt5081_disable
	请电源的同事帮忙看一下。




	//disable flashlight
	09-19 16:45:20.805227    70    70 E [  118.092780] (0)[70:pmic_thread][name:pmic_irq&]: [PMIC] 1111111111
	09-19 16:45:20.805250    70    70 E [  118.092803] (0)[70:pmic_thread][name:pmic_irq&]: [PMIC] 0
	09-19 16:45:20.805262    70    70 E [  118.092815] (0)[70:pmic_thread][name:pmic_irq&]: [PMIC] 1
	09-19 16:45:20.805274    70    70 E [  118.092827] (0)[70:pmic_thread][name:pmic_irq&]: [PMIC] 2
	09-19 16:45:20.805285    70    70 E [  118.092838] (0)[70:pmic_thread][name:pmic_irq&]: [PMIC] 3
	09-19 16:45:20.805297    70    70 E [  118.092850] (0)[70:pmic_thread][name:pmic_irq&]: [PMIC] 4
	09-19 16:45:20.805308    70    70 E [  118.092861] (0)[70:pmic_thread][name:pmic_irq&]: [PMIC] 5
	09-19 16:45:20.805319    70    70 E [  118.092872] (0)[70:pmic_thread][name:pmic_irq&]: [PMIC] 6
	09-19 16:45:58.761135    70    70 E [  156.048688] (7)[70:pmic_thread][name:pmic_irq&]: [PMIC] in [pmic_thread_kthread] by lilubao
	09-19 16:45:58.761172    70    70 E [  156.048725] (7)[70:pmic_thread][name:pmic_irq&]: [PMIC] [PMIC_INT] addr[0x856]=0x8
	09-19 16:45:58.761257    70    70 D [  156.048810] (7)[70:pmic_thread][name:mtk_ppm_main&]: [Power/PPM] (0x23c)(610)(1)[4L_LL]: (10)(10)(3)(4) (15)(10)(4)(4)
	09-19 16:45:58.761670    70    70 E [  156.049223] (7)[70:pmic_thread][name:flashlight&]: [FLASHLIGHT] pt_trigger: PT disable flashlight: (1,0,0)
	09-19 16:45:58.761704    70    70 E [  156.049257] (7)[70:pmic_thread][name:flashlights_rt5081&]: [FLASHLIGHT] rt5081_ioctl: FLASH_IOC_SET_ONOFF(0): 0
	09-19 16:45:58.761717    70    70 E [  156.049270] (7)[70:pmic_thread][name:flashlights_rt5081&]: [FLASHLIGHT] rt5081_operate: in [rt5081_operate] by lilubao

									fl_info("PT disable flashlight: (%d,%d,%d)\n",pt_low_vol, pt_low_bat, pt_over_cur);
	09-19 16:45:58.761727    70    70 E [  156.049280] (7)[70:pmic_thread][name:flashlight&]: [FLASHLIGHT] pt_trigger: PT disable flashlight: (1,0,0)

	09-19 16:45:58.761841    70    70 E [  156.049394] (7)[70:pmic_thread][name:flashlights_rt5081&]: [FLASHLIGHT] rt5081_ioctl: FLASH_IOC_SET_ONOFF(1): 0
	09-19 16:45:58.761851    70    70 E [  156.049404] (7)[70:pmic_thread][name:flashlights_rt5081&]: [FLASHLIGHT] rt5081_operate: in [rt5081_operate] by lilubao
	09-19 16:45:58.761858    70    70 E [  156.049411] (7)[70:pmic_thread][name:flashlights_rt5081&]: [FLASHLIGHT] rt5081_operate: in [rt5081_operate] by lilubao
	09-19 16:45:58.761865    70    70 E [  156.049418] (7)[70:pmic_thread][name:flashlights_rt5081&]: [FLASHLIGHT] rt5081_operate: 11111111111111
	09-19 16:45:58.761872    70    70 E [  156.049425] (7)[70:pmic_thread][name:flashlights_rt5081&]: [FLASHLIGHT] rt5081_disable: in [rt5081_disable] by lilubao
	09-19 16:45:58.762223    70    70 I [  156.049776] (7)[70:pmic_thread]: rt5081_pmu_fled rt5081_pmu_fled.0: set to off mode
	09-19 16:45:58.762342    70    70 I [  156.049895] (7)[70:pmic_thread]: rt5081_pmu_fled rt5081_pmu_fled.1: set to off mode
	09-19 16:45:58.762391    70    70 D [  156.049944] (7)[70:pmic_thread][name:kd_camera_hw&]: [kd_camera_hw]set_camera_flash_torch : on=0
	09-19 16:45:58.762415    70    70 E [  156.049968] (7)[70:pmic_thread][name:flashlight&]: [FLASHLIGHT] flashlight_machine_ops: new_state_0x50, ret=0(allowed:0)


	//Gionee <GN_BSP_CHG> <lilubao> <20170919> add for debug begin
	fl_err("in [%s] by lilubao\n",__FUNCTION__);
	//Gionee <GN_BSP_CHG> <lilubao> <20170919> add for debug end


	./drivers/misc/mediatek/flashlight/flashlights-rt5081.c
	rt5081_disable ->
	{
		1.rt5081_release -> rt5081_torch_store 
		-> 这个最后是读写闪光灯节点来打开或者关闭闪光灯
	
		2.rt5081_work_disable_ch1 -> INIT_WORK(&rt5081_work_ch1, rt5081_work_disable_ch1)
		-> rt5081_timer_func_ch1 -> rt5081_timer_start -> rt5081_operate 这边是对闪光灯的一些操作
	
		使能，只亮一个，前置后置闪光灯定时只亮一个，应该是拍照的自动打开闪光灯
	
		->创建了两个工作队列
	}


	./drivers/misc/mediatek/pmic/mt6355/v1/pmic_irq.c
	pmic_thread_kthread ->
	{
		[PMIC_INT] addr[0x856]=0x8
	
		struct pmic_interrupts interrupts[] = {
	PMIC_M_INTS_GEN(MT6355_INT_STATUS0, MT6355_INT_RAW_STATUS0,			//0x0854
			MT6355_INT_CON0, MT6355_INT_MASK_CON0, interrupt_status0),	//0x0856
	PMIC_M_INTS_GEN(MT6355_INT_STATUS1, MT6355_INT_RAW_STATUS1,
			MT6355_INT_CON1, MT6355_INT_MASK_CON1, interrupt_status1),
	PMIC_M_INTS_GEN(MT6355_INT_STATUS2, MT6355_INT_RAW_STATUS2,
			MT6355_INT_CON2, MT6355_INT_MASK_CON2, interrupt_status2),
	PMIC_M_INTS_GEN(MT6355_INT_STATUS3, MT6355_INT_RAW_STATUS3,
			MT6355_INT_CON3, MT6355_INT_MASK_CON3, interrupt_status3),
	PMIC_M_INTS_GEN(MT6355_INT_STATUS4, MT6355_INT_RAW_STATUS4,
			MT6355_INT_CON4, MT6355_INT_MASK_CON4, interrupt_status4),
	PMIC_M_INTS_GEN(MT6355_INT_STATUS5, MT6355_INT_RAW_STATUS5,
			MT6355_INT_CON5, MT6355_INT_MASK_CON5, interrupt_status5),
	PMIC_M_INTS_GEN(MT6355_INT_STATUS6, MT6355_INT_RAW_STATUS6,
			MT6355_INT_CON6, MT6355_INT_MASK_CON6, interrupt_status6),
	};


		09-19 16:46:07.209264    70    70 E [  165.765006] (0)[70:pmic_thread][name:pmic_irq&]: [PMIC] [PMIC_INT] addr[0x856]=0x200
	
	}



	低电条件下电池电压低，闪光灯瞬间的大电流会把电池电压拉到很低，触发了pmic的中断，
	跟哪些中断有关？这些中断是干什么的？设定的阈值是多少？是否能修改？如何修改的合理？


	1.RG_INT_STATUS_NAG_C_DLTV:这个中断应该是检测电池电压如果短时间内电池电压变化如果超过了阈值
	2.还有一个可能跟低电保护有关，开闪光灯电池电压可以拉低0.4V
	3.bat_temp_l,bat_h有几个相关的参数在mtk_battery_property.h文件内设置的
	最后确定的中断是bat_l_int_handler
	/* ex. 3400/5400*4096*/
	#define BAT_HV_THD   (POWER_INT0_VOLT*4096/5400)	/*ex: 3400mV */
	#define BAT_LV_1_THD (POWER_INT1_VOLT*4096/5400)	/*ex: 3250mV */
	#define BAT_LV_2_THD (POWER_INT2_VOLT*4096/5400)	/*ex: 3000mV */



	(flashlight-core.c)pt_trigger
	fl_info("PT disable flashlight: (%d,%d,%d)\n",
								pt_low_vol, pt_low_bat, pt_over_cur);


	09-19 16:45:58.761727    70    70 E [  156.049280] (7)[70:pmic_thread][name:flashlight&]: [FLASHLIGHT] pt_trigger: PT disable flashlight: (1,0,0)

	确定是low battery，pt_low_vol = LOW_BATTERY_LEVEL_0;


	pmic_throttling_dlpt 低电保护线程



	pt_trigger -> pt_low_vol_callback -> flashlight_pt_store -> register_low_battery_notify(&pt_low_vol_callback, LOW_BATTERY_PRIO_FLASHLIGHT);

		
	-> exec_low_battery_callback(LOW_BATTERY_LEVEL_0) -> bat_l_int_handler 

	typedef enum LOW_BATTERY_PRIO_TAG {
		LOW_BATTERY_PRIO_CPU_B = 0,
		LOW_BATTERY_PRIO_CPU_L = 1,
		LOW_BATTERY_PRIO_GPU = 2,
		LOW_BATTERY_PRIO_MD = 3,
		LOW_BATTERY_PRIO_MD5 = 4,
		LOW_BATTERY_PRIO_FLASHLIGHT = 5,
		LOW_BATTERY_PRIO_VIDEO = 6,
		LOW_BATTERY_PRIO_WIFI = 7,
		LOW_BATTERY_PRIO_BACKLIGHT = 8
	} LOW_BATTERY_PRIO;


	闪光灯有双闪，哪双闪？电流分别是多少？开启闪光灯的时间大概是多少？怎么修改低电保护延迟的判断时间？

	而低电量开启闪光灯，电流很大，rt5081要输出很大的功率，但是低电条件下输入能力不足，导致电池电压被拉的很低，


	(pmic_throttling_dlpt.c) dlpt_check_power_off


	modify test
	{
		修改FLED_VMID是0x7A的bit[5:0]，目前寄存器默认是5V（bit[5:0]=110111）。

		请按建议修改，试试看

		FL_VMID=4.0V（bit[5:0]=001111），FL_VMID=4.1V（bit[5:0]=010011），FL_VMID=4.2V（bit[5:0]=010111），FL_VMID=3.9V（bit[5:0]=001011）.

		(rt5081.dtsi)

			rt5081_pmu_fled1 {
			compatible = "richtek,rt5081_pmu_fled1";
			interrupt-names = "fled_lvf", "fled2_short",
						"fled1_short";
			fled_enable = <1>;
			torch_cur = <300000>;  /*25000 ~ 400000 uA */
			strobe_cur = <1200000>; /* 100000 ~ 1500000 uA */
			strobe_timeout = <2400>; /* 64 ~ 2432 ms */
		};
		rt5081_pmu_fled2 {
			compatible = "richtek,rt5081_pmu_fled2";
			fled_enable = <1>;
			torch_cur = <200000>;
			strobe_cur = <1000000>;
			strobe_timeout = <1200>;





		(rt5081_pmu_fled.c) rt5081_pmu_reg_update_bits这个是写寄存器的
		int rt5081_pmu_reg_update_bits(struct rt5081_pmu_chip *chip, u8 addr,u8 mask, u8 data)
	
		//Gionee <GN_BSP_CHG> <lilubao> <20170929> add for debug flashlight begin

		
		dev_dbg_ratelimited(chip->dev, "%s: reg %02x data %02x\n", __func__,
	addr, data);
	
	
			ret=rt5081_pmu_reg_update_bits(fled_data->chip,RT5081_PMU_REG_FLEDVMIDTRKCTRL1,
		RT5081_FLED_FIXED_MODE_MASK,0x4f);	// vmid -> 4.0v
		
		
		int rt5081_pmu_reg_read(struct rt5081_pmu_chip *chip, u8 addr)
		
		rt5081_fled_set_mode
		
			
	
		#define RT5081_PMU_REG_FLEDVMIDTRKCTRL1	(0x7A)
		#define RT5081_PMU_REG_FLEDVMIDRTM	(0x7B)
		#define RT5081_PMU_REG_FLEDVMIDTRKCTRL2	(0x7C)

	}



	这里有一个很重要的地方就是低电保护的代码逻辑，具体跟什么有关，怎么判断，
	触发后有做了什么
}	










/**************************************************************************************************************/
22.GNSPR#102837,测试机电量高于30%》关机状态插充电器后，屏幕弹出低电量充电图标，5s后到测试机正常充电图标界面（对比m7无此现象），多次操作仍如此》
重启未恢复，暂未恢复 验证10台10台100%
{
	这个应该是关机充电logo仍然用MTK原始的，需要适配成gionee amigo的关机充电logo
	MTK关机充电主要是在vendor目录下的，lk显示充电，external显示关机充电动画
	
	首先确定关机充电跟哪些相关
	{
		项目脚本BJ17G10A01_A.mk
			BOOT_LOGO = hd720
		   #Gionee <GN_BSP_CHG> <lilubao> <20171105> modify for gionee kpoc_logo begin
		   CONFIG_GN_BSP_AMIGO_CHARGING_SUPPORT=yes
		   #Gionee <GN_BSP_CHG> <lilubao> <20171105> modify for gionee kpoc_logo end
		
		gnbj6757_66_n.mk	
			BOOT_LOGO := hd720
			#Gionee <GN_BSP_CHG> <lilubao> <20171105> modify for gionee kpoc_logo begin
		   CONFIG_GN_BSP_AMIGO_CHARGING_SUPPORT = yes
		   DEFINES += CONFIG_GN_BSP_AMIGO_CHARGING_SUPPORT
		   #Gionee <GN_BSP_CHG> <lilubao> <20171105> modify for gionee kpoc_logo end
   
		   external目录下的charger，libshowlogo这两个目录的宏不知到为什么不生效，所以在Android.mk
	   加了这个宏
   
   
	   logo图片
		android_mtk_mp/vendor/mediatek/proprietary/bootable/bootloader/lk/dev/logo/
		
		编译
		out/target/product/gnbj6757_66_n/system/vendor/bin/kpoc_charger
	   out/target/product/gnbj6757_66_n/system/vendor/lib/libshowlogo.so
		
		./TmakeGionee    BJ17G10A01_A   -p  vendor/mediatek/proprietary/external/charger/
		 ./TmakeGionee    BJ17G10A01_A   -p  vendor/mediatek/proprietary/external/libshowlogo/	
		
		log关键字
		charger_thread,kpoc_charger
		
		//Gionee <GN_BSP_CHG> <lilubao> <20171105> modify for gionee kpoc_logo begin	
		LOG_ANIM("11 in [%s] by lilubao before\n",__FUNCTION__);
		//Gionee <GN_BSP_CHG> <lilubao> <20171105> modify for gionee kpoc_logo end

	}

	   

	G1605A上的修改code
	{
		bootloader/lk
	
	   vendor/mediatek/proprietary/bootable/bootloader/lk/lib/libshowlogo/show_animation_common.h:86://GioneeDrv LiLuBao 20161113 modify for change gioneelogo begin
	   vendor/mediatek/proprietary/bootable/bootloader/lk/lib/libshowlogo/show_animation_common.h:96://GioneeDrv LiLuBao 20161113 modify for change gioneelogo end
	   vendor/mediatek/proprietary/bootable/bootloader/lk/project/gnbj6737t_66_m0.mk:13:#GioneeDrv LiLuBao 20161113 modify for change gioneelogo begin
	   vendor/mediatek/proprietary/bootable/bootloader/lk/project/gnbj6737t_66_m0.mk:16:#GioneeDrv LiLuBao 20161113 modify for change gioneelogo end
	   vendor/mediatek/proprietary/bootable/bootloader/lk/lib/libshowlogo/show_animation_common.c:260://GioneeDrv LiLuBao 20161113 modify for change gioneelogo begin
	   vendor/mediatek/proprietary/bootable/bootloader/lk/lib/libshowlogo/show_animation_common.c:328://GioneeDrv LiLuBao 20161113 modify for change gioneelogo end
	   vendor/mediatek/proprietary/bootable/bootloader/lk/dev/logo/rules.mk:88:#GioneeDrv LiLuBao 20161113 modify for change gioneelogo begin
	   vendor/mediatek/proprietary/bootable/bootloader/lk/dev/logo/rules.mk:184:#GioneeDrv LiLuBao 20161113 modify for change gioneelogo end
	   vendor/mediatek/proprietary/bootable/bootloader/lk/platform/mt6735/mt_logo.c:225:	//GioneeDrv LiLuBao 20161113 modify for change gioneelogo begin
	   vendor/mediatek/proprietary/bootable/bootloader/lk/platform/mt6735/mt_logo.c:231:	//GioneeDrv LiLuBao 20161113 modify for change gioneelogo end
	   vendor/mediatek/proprietary/bootable/bootloader/lk/platform/mt6735/mt_logo.c:238://GioneeDrv LiLuBao 20161113 modify for change gioneelogo begin
	   vendor/mediatek/proprietary/bootable/bootloader/lk/platform/mt6735/mt_logo.c:257://GioneeDrv LiLuBao 20161113 modify for change gioneelogo end
	   vendor/mediatek/proprietary/bootable/bootloader/lk/platform/mt6735/mt_logo.c:295:	//GioneeDrv LiLuBao 20161113 modify for change gioneelogo begin
	   vendor/mediatek/proprietary/bootable/bootloader/lk/platform/mt6735/platform.c:782:			//GioneeDrv LiLuBao 20161113 modify for change gioneelogo begin
	   vendor/mediatek/proprietary/bootable/bootloader/lk/platform/mt6735/platform.c:785:			//GioneeDrv LiLuBao 20161113 modify for change gioneelogo end	


	   external/libshowlogo/
   
	   vendor/mediatek/proprietary/external/libshowlogo/show_animation_common.h:109://GioneeDrv LiLuBao 20161113 modify for change gioneelogo begin
	   vendor/mediatek/proprietary/external/libshowlogo/show_animation_common.h:123://GioneeDrv LiLuBao 20161113 modify for change gioneelogo end
	   vendor/mediatek/proprietary/external/libshowlogo/charging_animation.cpp:647://GioneeDrv LiLuBao 20161113 modify for change gioneelogo begin
	   vendor/mediatek/proprietary/external/libshowlogo/charging_animation.cpp:701://GioneeDrv LiLuBao 20161113 modify for change gioneelogo end
	   vendor/mediatek/proprietary/external/libshowlogo/charging_animation.cpp:734:	//GioneeDrv LiLuBao 20161113 modify for change gioneelogo begin
	   vendor/mediatek/proprietary/external/libshowlogo/charging_animation.cpp:740:	//GioneeDrv LiLuBao 20161113 modify for change gioneelogo end
	   vendor/mediatek/proprietary/external/libshowlogo/charging_animation.cpp:754:	//GioneeDrv LiLuBao 20161113 modify for change gioneelogo begin
	   vendor/mediatek/proprietary/external/libshowlogo/charging_animation.cpp:760:	//GioneeDrv LiLuBao 20161113 modify for change gioneelogo end
	   vendor/mediatek/proprietary/external/libshowlogo/charging_animation.h:110://GioneeDrv LiLuBao 20161113 modify for change gioneelogo begin
	   vendor/mediatek/proprietary/external/libshowlogo/charging_animation.h:115://GioneeDrv LiLuBao 20161113 modify for change gioneelogo end
	   vendor/mediatek/proprietary/external/libshowlogo/show_animation_common.c:269://GioneeDrv LiLuBao 20161113 modify for change gioneelogo begin
	   vendor/mediatek/proprietary/external/libshowlogo/show_animation_common.c:574://GioneeDrv LiLuBao 20161113 modify for change gioneelogo end
	   vendor/mediatek/proprietary/external/libshowlogo/show_animation_common.c:620:    //GioneeDrv LiLuBao 20161113 modify for change gioneelogo begin
	   vendor/mediatek/proprietary/external/libshowlogo/show_animation_common.c:669:    //GioneeDrv LiLuBao 20161113 modify for change gioneelogo end


	   external/charger/

	   vendor/mediatek/proprietary/external/charger/common.cpp:143:	//GioneeDrv LiLuBao 20161113 modify for change gioneelogo begin
	   vendor/mediatek/proprietary/external/charger/common.cpp:153:	//GioneeDrv LiLuBao 20161113 modify for change gioneelogo end
	   vendor/mediatek/proprietary/external/charger/bootlogo.cpp:132:    //GioneeDrv LiLuBao 20161113 modify for change gioneelogo begin
	   vendor/mediatek/proprietary/external/charger/bootlogo.cpp:159:    //GioneeDrv LiLuBao 20161113 modify for change gioneelogo end
	
	}
	

	 关机充电logo相关过程
	 {
		关机充电logo的档位
		typedef enum
	   {
		   AMIGO_EMPTY_LEVEL=0,     //capacity from 0% to 9%
		   AMIGO_LOW_LEVEL,         //capacity from 10% to 30%
		   AMIGO_MID_LEVEL,         //capacity from 31% to60%
		   AMIGO_HIGH_LEVEL,        //capacity from 61% to 99%
		   AMIGO_FULL_LEVEL,		 // 100%
		   AMIGO_FAILED_LEVEL		 // error
	   }AMIGO_BATTERY_LEVEL;


	   关机充电动画在各个挡的位移
	   typedef enum
	   {
		   AMIGO_BATFULL_ANIM = 0,
		   AMIGO_BATEMPTY_START_ANIM = 1,
		   AMIGO_BATLOW_START_ANIM = 13,
		   AMIGO_BATMID_START_ANIM = 25,
		   AMIGO_BATHIGH_START_ANIM = 37,
		   AMIGO_BATLOW_PERCENT = 49,
		   AMIGO_BATLOW_NUM_0 = 50,
		   AMIGO_BATMID_PERCENT = 60,
		   AMIGO_BATMID_NUM_0 = 61,
		   AMIGO_BATHIGH_PERCENT = 71,
		   AMIGO_BATHIGH_NUM_0 = 72
	   }AMIGO_PIC_OFFSET_MAP;

	   $(BOOT_LOGO_DIR)/$(BASE_LOGO)/$(BASE_LOGO)_amigo_charger_ov.raw \		82
	   $(BOOT_LOGO_DIR)/$(BASE_LOGO)/$(BASE_LOGO)_amigo_kpoc_charge.raw \		83
	   $(BOOT_LOGO_DIR)/$(BASE_LOGO)/$(BASE_LOGO)_amigo_low_battery.raw \		84
	   $(BOOT_LOGO_DIR)/$(BASE_LOGO)/$(BASE_LOGO)_black.raw \					85
	   $(BOOT_LOGO_DIR)/$(BASE_LOGO)/$(BASE_LOGO)_amigo_lowtemp.raw \			86
	   $(BOOT_LOGO_DIR)/$(BASE_LOGO)/$(BASE_LOGO)_amigo_hightemp.raw			87
   
   
	   MTK平台的关机充电
	   {
		   关键字：
			   kpoc_charger
			   ChargingAnimation 充电动画
		   代码：	
		   vendor/mediatek/proprietary/external
		   vendor/mediatek/proprietary/bootable/bootloader/lk
	   
	   
		   ro.hardware,famebuffer，surface flinger
		   这两个动画模式	应该是famebuffer模式
		   DRAW_ANIM_MODE_SURFACE
		   DRAW_ANIM_MODE_FB
	   
	   
		   从charger/main.cpp 开始
		   main
		}

			修改高低温提示语，和温度门限
			{
				1.修改关机充电的提示语
				hd720_amigo_higtemp.bmp
				hd720_amigo_lowtemp.bmp
				
				hd720_amigo_hightemp.bmp
				hd720_amigo_low_battery.bmp
				
				2.开机充电的保护门限
				min_charge_temperature = <(-3)>;
				min_charge_temperature_plus_x_degree = <0>;
				max_charge_temperature = <55>;
				max_charge_temperature_minus_x_degree = <50>;
				
				3.关机充电的第一张logo刷错了
				应该是lk的 mk跟platform 
				
				#Gionee <GN_BSP_CHG> <lilubao> <20171105> modify for gionee kpoc_logo begin
				CONFIG_GN_BSP_AMIGO_CHARGING_SUPPORT = yes
				DEFINES += CONFIG_GN_BSP_AMIGO_CHARGING_SUPPORT
				#Gionee <GN_BSP_CHG> <lilubao> <20171105> modify for gionee kpoc_logo end
				
				#if defined(CONFIG_GN_BSP_AMIGO_CHARGING_SUPPORT)
				mt_disp_show_kpoc_charge_logo();

				#elif defined(GN_MTK_BSP_LK_CHARGE_GIONEELOGO)
					gn_disp_kpoc_lk_charge_logo();			
				#else
					mt_disp_show_low_battery();
				#endif
				
				
				/sys/devices/platform/charger/BatteryNotify 
				
				echo 1 > BatteryNotify  电池电压过高
				echo 2 > BatteryNotify 	电池温度过高
				
				static void mtk_battery_notify_UI_test(struct charger_manager *info)
				{
					switch (info->notify_test_mode) {
					case 1:
						info->notify_code = 0x0001;
						pr_debug("[%s] CASE_0001_VCHARGER\n", __func__);
						break;
					case 2:
						info->notify_code = 0x0002;
						pr_debug("[%s] CASE_0002_VBATTEMP\n", __func__);
						break;
					case 3:
						info->notify_code = 0x0004;
						pr_debug("[%s] CASE_0003_ICHARGING\n", __func__);
						break;
					case 4:
						info->notify_code = 0x0008;
						pr_debug("[%s] CASE_0004_VBAT\n", __func__);
						break;
					case 5:
						info->notify_code = 0x0010;
						pr_debug("[%s] CASE_0005_TOTAL_CHARGINGTIME\n", __func__);
						break;
					default:
						pr_debug("[%s] Unknown BN_TestMode Code: %x\n",
							__func__, info->notify_test_mode);
					}
				}
	
					vendor/mediatek/proprietary/packages/apps/BatteryWarning/res/values-zh-rCN/strings.xml
					./system/vendor/app/BatteryWarning
	
				
			}
	   
	 }
		   
}	





/**************************************************************************************************************************/
23.	GNSPR#110553,【GMS-CTS测试】CtsHostsideNetworkTests测试项faileds
{
	先了解一下CTS干什么的，
	{
		google官网关于cts的介绍，android设备的兼容性测试
		https://source.android.google.cn/compatibility/
		
		device.mk这个文件很重要，内容也很多
		xml文件好像用的很多，有必要弄清楚,这个应该是读取相关的配置然后加载
		xml文件配置了系统所需要的资源，rc,mk等需要的脚本，宏和默认的设置
	

		cts对权限有要求，权限过高会有安全隐患

		把gionee/code/driver/project_common/BJ17G10_DRV_COMMON/device/gionee_bj/gnbj6757_66_n/device.mk中的下面的permissions注释掉。

		USB OTG
		PRODUCT_COPY_FILES += frameworks/native/data/etc/android.hardware.usb.host.xml:system/etc/permissions/android.hardware.usb.host.xml
		
	}


	MTK提case
	{
		若是CTS问题,请提供:
		1.单跑此项case的mtklog
		2.单跑此项case的cts log(在android-cts\repository\logs目录下)
		3.单跑此项case的cts report(在android-cts\repository\results 目录下)

		Please help to provide below informations when submitting CTS issues:
		1.the mtklog after running the failed case individually 
		2.the cts log（under the folder:android-cts\repository\logs ）
		3.the cts report(unnder the folder:android-cts\repository\results )



		我们同平台的另一个项目S10 也fail了
		但是两个高通平台的项目17G08A，17G06A，ok，环境应该没有问题
		android版本是否有影响，17G10A用的是7.0，17G08A用的是7.1.1 
	


		您好，据我了解和其他用户反馈，高通平台有专门合了一个O版本的patch，所以会和MTK平台表现不一样，
		合入该patch之后就算在IPv4下也可以测试pass，贵司可以试一下：

		https://android.googlesource.com/platform/libcore/+/3a50b32e2abec315948e1947450cd0f7c0c82b2d
		建議可以參考這個patch 改動.
		只要合入 ojluni/src/main/java/java/net/DatagramSocket.java 的patch. 
	
	
	
		diff --git a/ojluni/src/main/java/java/net/DatagramSocket.java b/ojluni/src/main/java/java/net/DatagramSocket.java
		old mode 100644
		new mode 100755
		index 7577e63..a6f09fa
		--- a/ojluni/src/main/java/java/net/DatagramSocket.java
		+++ b/ojluni/src/main/java/java/net/DatagramSocket.java
		@@ -679,6 +679,11 @@
		public void send(DatagramPacket p) throws IOException {
		InetAddress packetAddress = null;
		synchronized (p) {
		+ // ----- BEGIN android -----
		+ if (pendingConnectException != null) {
		+ throw new SocketException("Pending connect failure", pendingConnectException);
		+ }
		+ // ----- END android -----
		if (isClosed())
		throw new SocketException("Socket is closed");
		checkAddress (p.getAddress(), "send");
	
		DatagramSocket.java
		//Gionee <GN_BSP_CHG> <lilubao> <20171113> modify for CTS begin
		
		
		贵司提GTS问题前可以先关注MOL
		进入MediaTek On-Line-> Quick Start-> GMS快速入门->GMS认证相关->GTS5.0r2常见问题.
		请参考如下branch自行到PMS上申请patch
		alps-mp-n0.mp5 ALPS03603867 
	
	
	}

}






/*******************************************************************************************************************/
24.省电适配参数导入
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

	主要看变化是否匀称

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






/*********************************************************************************************************/
25.	GNSPR#103678,通话：接听或者挂断电话时，振动的同时会有崩的一声
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






/****************************************************************************************************************/
26.	GNSPR#113442,【品质压力】待机界面》手机连接电脑USB端口-手机下拉通知栏-无USB连接显示-插拔数据线仍如此》
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





/**************************************************************************************************************************/
27.	GNSPR#109803,关机状态，连接充电器，显示电量灭屏后，短按两次电源键1s，出现开机画面，再次操作恢复
{
	启动之后的boot_reason 是4,wdt_by_pass_pwk,然后这个现象是在系统跑到kernel之后的
	按键的检测问题应该是在kernel出现问题的		
	{
		首先这个boot_reason 有哪些原因？
		SW,HW,kernel panic 按键检测到短按所以会重启，这个应该是系统硬件初始化的时候读取寄存器传递的值g_boot_arg
		
		kernel 部分按键的处理在kpd.c 中断回调函数 kpd_pwrkey_pmic_handler
		void kpd_pwrkey_pmic_handler(unsigned long pressed)
		{
		//Gionee <GN_BY_DRV> <wangguojun> <2017-10-17> modify for 124138 begin
			if(1==s_gn_clam)
			{
				return ;
			}
			else
			{
				kpd_print("Power Key generate, pressed=%ld\n", pressed);
				if (!kpd_input_dev) {
					kpd_print("KPD input device not ready\n");
					return;
			}
			kpd_pmic_pwrkey_hal(pressed);
		#if (defined(CONFIG_ARCH_MT8173) || defined(CONFIG_ARCH_MT8163))
			if (pressed) /* keep the lock while the button in held pushed */
				wake_lock(&pwrkey_lock);
			else /* keep the lock for extra 500ms after the button is released */
				wake_lock_timeout(&pwrkey_lock, HZ/2);
				//这一部分的处理就是说在释放powerkey之后还有500ms持锁
		#endif
			}
		}
		//Gionee <GN_BY_DRV> <wangguojun> <2017-10-17> modify for 124138 end
	}		
	
	6355的 MT6355_TOPSTATUS这个寄存器有homekey跟powerkey的检测
	preloader的keypad.c mtk_detect_key 检测按键
	
	按键相关的input子系统内定的编号
	linux-event-codes.h
	
	
	{
		相关的宏	
		BR_WDT_BY_PASS_PWK,MTK_KERNEL_POWER_OFF_CHARGING,kpoc_flag
		

		<3>[   10.779316]  (7)[185:battery_thread]in [fg_drv_update_hw_status] gn_boot_reason->4,gn_boot_mode->0,gn_call_state->0,gn_screenon_time->10
		<3>[   20.786597]  (5)[185:battery_thread]in [fg_drv_update_hw_status] gn_boot_reason->4,gn_boot_mode->0,gn_call_state->0,gn_screenon_time->20
		<3>[   30.792217]  (0)[185:battery_thread]in [fg_drv_update_hw_status] gn_boot_reason->4,gn_boot_mode->0,gn_call_state->0,gn_screenon_time->30
	}
	
	按键检测的流程大概是这样的：
	底层6355接收到按键触发的中断然后通过handler回调到(kpd.c)kpd_pwrkey_pmic_handler，这里有wake_lock持锁，按下的时候持锁，
	释放的时候还会持锁500ms，然后时（hal_kpd.c）kpd_pmic_pwrkey_hal 将按键的时间上报给input子系统的，(key_control.cpp) 之后应该是通过input
	子系统下面的fd节点，这里面是一个线程通过轮询的方式，如果有变化就会判断下一步亮屏关机充电的动画还是灭屏或者是重启之类的

}







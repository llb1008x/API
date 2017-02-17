

双芯片管理充电MT6355和RT5081

/*GM3.0*/
{
	step:
		1.测试调整库仑计参数car_tune_value
		2.调整电池参数，导入battery_prop.dtsi
		3.导入电池曲线ZCV table battery_table.dtsi
		4.Rfg？
		
		
	电量计检测的NTC电阻10k，47k	
		
		
		
	定义了GM3.0的地方
	./arch/arm64/configs/k57pv1_6mtee_pre_debug_defconfig:377:CONFIG_MTK_GAUGE_VERSION=30
	./arch/arm64/configs/k57pv1_6mtee_pre_defconfig:361:CONFIG_MTK_GAUGE_VERSION=30
	./arch/arm64/boot/dts/mediatek/mt6757.dtsi:1216:#if (CONFIG_MTK_GAUGE_VERSION == 30)
	./drivers/misc/mediatek/xhci/xhci-mtk-driver.c:36:#if CONFIG_MTK_GAUGE_VERSION == 30
	./drivers/misc/mediatek/xhci/xhci-mtk-driver.c:69:#if CONFIG_MTK_GAUGE_VERSION == 30
	./drivers/misc/mediatek/xhci/xhci-mtk-driver.c:102:#if CONFIG_MTK_GAUGE_VERSION == 30
	./drivers/misc/mediatek/xhci/xhci-mtk-driver.c:115:#if CONFIG_MTK_GAUGE_VERSION == 30
	./drivers/misc/mediatek/xhci/xhci-mtk-driver.c:354:#if CONFIG_MTK_GAUGE_VERSION == 30
	./drivers/misc/mediatek/xhci/xhci-mtk-driver.c:460:#if CONFIG_MTK_GAUGE_VERSION == 30
	./drivers/misc/mediatek/xhci/xhci-mtk-driver.c:507:#if CONFIG_MTK_GAUGE_VERSION == 30
	./drivers/misc/mediatek/xhci/xhci-mtk-driver.h:62:#if CONFIG_MTK_GAUGE_VERSION == 30
	./drivers/misc/mediatek/rtc/mtk_rtc_common.c:221:#if (CONFIG_MTK_GAUGE_VERSION != 30)
	./drivers/misc/mediatek/mu3d/drv/musb_core.h:48:#if (CONFIG_MTK_GAUGE_VERSION != 30)
	./drivers/misc/mediatek/mu3d/drv/musb_core.h:51:#if (CONFIG_MTK_GAUGE_VERSION == 30)
	./drivers/misc/mediatek/thermal/common/coolers/Makefile:128:	ifeq ($(CONFIG_MTK_GAUGE_VERSION),30)
	./drivers/misc/mediatek/thermal/common/coolers/mtk_cooler_bcct_v1.c:30:#if (CONFIG_MTK_GAUGE_VERSION == 30)
	./drivers/misc/mediatek/thermal/common/coolers/mtk_cooler_bcct_v1.c:55:#if (CONFIG_MTK_GAUGE_VERSION == 30)
	./drivers/misc/mediatek/thermal/common/coolers/mtk_cooler_bcct_v1.c:266:#if (CONFIG_MTK_GAUGE_VERSION == 30)
	./drivers/misc/mediatek/thermal/common/coolers/mtk_cooler_bcct_v1.c:328:#if (CONFIG_MTK_GAUGE_VERSION == 30)
	./drivers/misc/mediatek/thermal/common/coolers/mtk_cooler_bcct_v1.c:338:#if (CONFIG_MTK_GAUGE_VERSION == 30)
	./drivers/misc/mediatek/thermal/common/coolers/mtk_cooler_bcct_v1.c:391:#if (CONFIG_MTK_GAUGE_VERSION == 30)
	./drivers/misc/mediatek/thermal/common/coolers/mtk_cooler_bcct_v1.c:568:#if (CONFIG_MTK_GAUGE_VERSION == 30)
	./drivers/misc/mediatek/thermal/common/coolers/mtk_cooler_bcct_v1.c:667:#if (CONFIG_MTK_GAUGE_VERSION == 30)
	./drivers/misc/mediatek/thermal/common/coolers/mtk_cooler_bcct_v1.c:822:#if (CONFIG_MTK_GAUGE_VERSION == 30)
	./drivers/misc/mediatek/thermal/common/coolers/mtk_cooler_bcct_v1.c:1335:#if (CONFIG_MTK_GAUGE_VERSION == 30)
	./drivers/misc/mediatek/thermal/common/coolers/mtk_cooler_bcct_v1.c:1493:#if (CONFIG_MTK_GAUGE_VERSION == 30)
	./drivers/misc/mediatek/thermal/common/coolers/mtk_cooler_bcct_v1.c:1501:#if (CONFIG_MTK_GAUGE_VERSION == 30)
	./drivers/misc/mediatek/thermal/common/thermal_zones/mtk_ts_battery.c:198:#if (CONFIG_MTK_GAUGE_VERSION == 30)
	./drivers/misc/mediatek/usb_c/tcpc/inc/tcpm.h:774:#if CONFIG_MTK_GAUGE_VERSION == 30
	./drivers/misc/mediatek/usb_c/tcpc/inc/tcpm.h:782:#if CONFIG_MTK_GAUGE_VERSION == 30
	./drivers/misc/mediatek/usb_c/tcpc/inc/tcpm.h:796:#if CONFIG_MTK_GAUGE_VERSION == 20
	./drivers/misc/mediatek/usb_c/tcpc/inc/mtk_direct_charge_vdm.h:73:#if CONFIG_MTK_GAUGE_VERSION == 20
	./drivers/misc/mediatek/usb_c/tcpc/inc/mtk_direct_charge_vdm.h:171:#if CONFIG_MTK_GAUGE_VERSION == 20
	./drivers/misc/mediatek/usb_c/tcpc/inc/mtk_direct_charge_vdm.h:273:#if CONFIG_MTK_GAUGE_VERSION == 20
	./drivers/misc/mediatek/usb_c/tcpc/mtk_direct_charge_vdm.c:179:#if CONFIG_MTK_GAUGE_VERSION == 30
	./drivers/misc/mediatek/usb_c/tcpc/tcpm.c:900:#if CONFIG_MTK_GAUGE_VERSION == 30
	./drivers/misc/mediatek/usb_c/tcpc/tcpm.c:916:#if CONFIG_MTK_GAUGE_VERSION == 30
	./drivers/misc/mediatek/include/mt-plat/mt6757/include/mach/mtk_battery_meter.h:60:#if (CONFIG_MTK_GAUGE_VERSION == 20)
	./drivers/misc/mediatek/include/mt-plat/battery_common.h:350:#if (CONFIG_MTK_GAUGE_VERSION == 20)
	./drivers/misc/mediatek/include/mt-plat/battery_meter.h:84:#if (CONFIG_MTK_GAUGE_VERSION == 10)
	./drivers/misc/mediatek/include/mt-plat/battery_meter.h:155:#if (CONFIG_MTK_GAUGE_VERSION == 20)
	./drivers/misc/mediatek/include/mt-plat/battery_meter.h:346:#if (CONFIG_MTK_GAUGE_VERSION == 10)
	./drivers/misc/mediatek/include/mt-plat/battery_meter.h:353:#if (CONFIG_MTK_GAUGE_VERSION == 10)
	./drivers/misc/mediatek/include/mt-plat/battery_meter.h:357:#if (CONFIG_MTK_GAUGE_VERSION == 20)
	./drivers/misc/mediatek/include/mt-plat/battery_meter.h:424:#if (CONFIG_MTK_GAUGE_VERSION == 20)
	./drivers/misc/mediatek/include/mt-plat/charging.h:55:#if (CONFIG_MTK_GAUGE_VERSION != 30)
	./drivers/misc/mediatek/pmic/mtk_battery_adc_intf.c:32:#if defined(CONFIG_POWER_EXT) || (CONFIG_MTK_GAUGE_VERSION != 30)
	./drivers/misc/mediatek/pmic/rt5081/Makefile:1:ifeq ($(CONFIG_MTK_GAUGE_VERSION),30)
	./drivers/misc/mediatek/pmic/rt5081/Makefile:17:ifeq ($(CONFIG_MTK_GAUGE_VERSION),30)
	./drivers/misc/mediatek/pmic/mt6355/v1/Makefile:11:ifeq ($(CONFIG_MTK_GAUGE_VERSION),30)
	./drivers/misc/mediatek/pmic/mt6355/v1/pmic_throttling_dlpt.c:67:#if (CONFIG_MTK_GAUGE_VERSION == 30)
	./drivers/misc/mediatek/pmic/mt6355/v1/pmic_throttling_dlpt.c:229:#if (CONFIG_MTK_GAUGE_VERSION == 30)
	./drivers/misc/mediatek/pmic/mt6355/v1/pmic_throttling_dlpt.c:1307:		#if (CONFIG_MTK_GAUGE_VERSION == 30)
	./drivers/misc/mediatek/pmic/mt6355/v1/pmic_throttling_dlpt.c:2106:	#if (CONFIG_MTK_GAUGE_VERSION == 30)
	./drivers/misc/mediatek/pmic/mt6355/v1/pmic_irq.c:299:#if (CONFIG_MTK_GAUGE_VERSION != 30)
	./drivers/misc/mediatek/pmic/mt6355/v1/pmic_irq.c:609:#if (CONFIG_MTK_GAUGE_VERSION != 30)
	./drivers/misc/mediatek/pmic/mt6355/v1/pmic_irq.c:631:#if (CONFIG_MTK_GAUGE_VERSION != 30)
	./drivers/misc/mediatek/power/mt6757/Makefile:29:ifeq ($(CONFIG_MTK_GAUGE_VERSION),30)
	./drivers/misc/mediatek/power/mt6757/pmic.c:101:#if (CONFIG_MTK_GAUGE_VERSION == 30)
	./drivers/misc/mediatek/power/mt6757/rt_pd_manager.c:29:#if CONFIG_MTK_GAUGE_VERSION == 30
	./drivers/misc/mediatek/power/mt6757/rt_pd_manager.c:38:#endif /* CONFIG_MTK_GAUGE_VERSION */
	./drivers/misc/mediatek/power/mt6757/rt_pd_manager.c:69:#if CONFIG_MTK_GAUGE_VERSION == 30
	./drivers/misc/mediatek/power/mt6757/rt_pd_manager.c:81:#if CONFIG_MTK_GAUGE_VERSION == 20
	./drivers/misc/mediatek/power/mt6757/rt_pd_manager.c:288:#if CONFIG_MTK_GAUGE_VERSION == 30
	./drivers/misc/mediatek/power/mt6757/rt_pd_manager.c:298:#if CONFIG_MTK_GAUGE_VERSION == 30
	./drivers/misc/mediatek/power/mt6757/rt_pd_manager.c:313:#if CONFIG_MTK_GAUGE_VERSION == 30
	./drivers/misc/mediatek/power/mt6757/rt_pd_manager.c:332:#if CONFIG_MTK_GAUGE_VERSION == 20
	./drivers/misc/mediatek/power/mt6757/rt_pd_manager.c:363:#if CONFIG_MTK_GAUGE_VERSION == 20
	./drivers/misc/mediatek/power/mt6757/rt_pd_manager.c:373:#if CONFIG_MTK_GAUGE_VERSION == 30
	./drivers/misc/mediatek/power/mt6757/rt_pd_manager.c:390:#if CONFIG_MTK_GAUGE_VERSION == 20
	./drivers/misc/mediatek/power/mt6757/rt_pd_manager.c:403:#if CONFIG_MTK_GAUGE_VERSION == 20
	./drivers/misc/mediatek/power/mt6757/rt_pd_manager.c:416:#if CONFIG_MTK_GAUGE_VERSION == 30
	./drivers/misc/mediatek/power/mt6757/rt_pd_manager.c:420:#endif /* CONFIG_MTK_GAUGE_VERSION == 30 */
	./drivers/misc/mediatek/power/mt6757/rt_pd_manager.c:427:#if CONFIG_MTK_GAUGE_VERSION == 30
	./drivers/misc/mediatek/power/mt6757/rt_pd_manager.c:436:#if CONFIG_MTK_GAUGE_VERSION == 30
	./drivers/misc/mediatek/power/mt6757/rt_pd_manager.c:468:#if CONFIG_MTK_GAUGE_VERSION == 30
	./drivers/misc/mediatek/power/mt6757/rt_pd_manager.c:479:#endif /* CONFIG_MTK_GAUGE_VERSION == 30 */
	./drivers/misc/mediatek/power/mt6757/rt_pd_manager.c:523:#if CONFIG_MTK_GAUGE_VERSION == 20
	./drivers/power/mediatek/Makefile:20:ifeq ($(CONFIG_MTK_GAUGE_VERSION),30)
	./drivers/power/mediatek/Makefile:23:else ifeq ($(CONFIG_MTK_GAUGE_VERSION),20)
	./drivers/power/mediatek/Makefile:29:ifeq ($(CONFIG_MTK_GAUGE_VERSION),30)
	./drivers/power/mediatek/switch_charging.c:88:#if (CONFIG_MTK_GAUGE_VERSION == 20)
	./drivers/power/mediatek/switch_charging.c:326:#if (CONFIG_MTK_GAUGE_VERSION == 20)
	./drivers/power/mediatek/switch_charging.c:1031:#if (CONFIG_MTK_GAUGE_VERSION == 20)
	./drivers/power/mediatek/switch_charging.c:1123:#if (CONFIG_MTK_GAUGE_VERSION != 20)
	./drivers/power/mediatek/battery_meter_fg_20.c:1717:#if (CONFIG_MTK_GAUGE_VERSION == 20)
	./drivers/power/mediatek/linear_charging.c:119:#if (CONFIG_MTK_GAUGE_VERSION == 20)
	./drivers/power/mediatek/linear_charging.c:185:#if (CONFIG_MTK_GAUGE_VERSION == 20)
	./drivers/power/mediatek/linear_charging.c:753:#if (CONFIG_MTK_GAUGE_VERSION == 20)
	./drivers/power/mediatek/linear_charging.c:1140:			#if (CONFIG_MTK_GAUGE_VERSION == 20)
	./drivers/power/mediatek/charger/Makefile:27:ifeq ($(CONFIG_MTK_GAUGE_VERSION),30)
	



}




















/*MT6355*/
{







}







/*RT5081*/
{
集成的功能很多
	
	RT5081的几个基本功能：
	switching battery charger ;USB Type-C &&Power Delivery (PD) controller;
	dual Flash LED current sources;a RGB LED driver;a backlight WLED driver;
	a display bias driver and a general LDO for portable devices.

		
	UVP < VDDA <OVP


	MRSTB pin to manually reset the hardware or registers
	这个应该是复位功能
	
	switching charger 有三种模式：
	charge mode, boost mode (OTG-Boost),and high-impedance mode.
	high-impedance mode:高阻抗模式为了降低功耗，产生极低的电流
	
	charge mode :
	Minimum Input Voltage Regulation (MIVR):最小输入电压的控制
	为了阻止输入电压因充电器供电能力不足而下降


	{
	   Strobe Mode ：选通模式

	   BMC Power Delivery protocol

	   CHG_VMID：从这个引脚提供电流
	
	   0.1A到3.25A的输入电流管理

	   OTG输出电压4.42V到5.58V，输出电流2A

	   电池电压在2.3V到3.8V低电保护，会启动升压操作

	   17V，21V，25V，29V四档过压保护

	   900mA，1200mA,1500mA,1800mA四档过流保护
	}







代码：
	/home/llb/project/PRO/source/17G05A/L30_6757_17G05A_N0.MP5_161227_ALPS/android_mtk_6757_mp/kernel-4.4/drivers/misc/mediatek/pmic/rt5081/
	
	
	
	按照移植17W05A的思路应该包括一下几部分：
	1.charger能充电，三个阶段的dws配置是否正确，初始化是否有问题
	2.OTG功能使能
	3.关机充电logo
	4.fuelgauge电量计的移植
	
	
	打印寄存器信息在哪，调用哪个函数
	rt5081充电相关的几个中断			P98
	enum rt5081_pmu_charger_irqidx {
		RT5081_CHG_IRQIDX_CHGIRQ1 = 0,
		RT5081_CHG_IRQIDX_CHGIRQ2,
		RT5081_CHG_IRQIDX_CHGIRQ3,
		RT5081_CHG_IRQIDX_CHGIRQ4,
		RT5081_CHG_IRQIDX_CHGIRQ5,
		RT5081_CHG_IRQIDX_CHGIRQ6,
		RT5081_CHG_IRQIDX_QCIRQ,
		RT5081_CHG_IRQIDX_DICHGIRQ7,
		RT5081_CHG_IRQIDX_OVPCTRLIRQ,
		RT5081_CHG_IRQIDX_MAX,
	};
	
	
	RT5081定义的充电器检测类型
	enum rt5081_pmu_chg_type {
		RT5081_CHG_TYPE_NOVBUS = 0,
		RT5081_CHG_TYPE_UNDER_GOING,
		RT5081_CHG_TYPE_SDP,
		RT5081_CHG_TYPE_SDPNSTD,
		RT5081_CHG_TYPE_DCP,
		RT5081_CHG_TYPE_CDP,
		RT5081_CHG_TYPE_MAX,
	};
	
	
	
	
	进行充电器类型检测时调用的是mtk_chg_type_det.c，没有调用battery_common_fg_20.c
	根据makfile选项应该是用 CONFIG_MTK_GAUGE_VERSION  GM3.0
	函数调用时检测的充电器类型
	typedef enum {
		CHARGER_UNKNOWN = 0,
		STANDARD_HOST,		/* USB : 450mA */
		CHARGING_HOST,
		NONSTANDARD_CHARGER,	/* AC : 450mA~1A */
		STANDARD_CHARGER,	/* AC : ~1A */
		APPLE_2_1A_CHARGER,	/* 2.1A apple charger */
		APPLE_1_0A_CHARGER,	/* 1A apple charger */
		APPLE_0_5A_CHARGER,	/* 0.5A apple charger */
		WIRELESS_CHARGER,
	} CHARGER_TYPE;
	
	
	
	
	
	
函数功能：	
	mtk_get_dynamic_cv
	
	
	
	battery_get_bat_current_sign这个应该是fuelgauge在读取寄存器g_fg_is_charging，读取IC是充电(负)还是放电(正)
	
	底层一些操作接口在rt5081_pmu_charger.c定义
	
	
	
	charger_manager是专门的管理充电器的子系统？
	{	
		单充电芯片还是双充电芯片工作：919是单充电芯片
		mtk_switch_charging_init
		mtk_dual_switch_charging_init
		
		
	
	}
	
	
	
	
	
	

########################################################################################################	
	去掉了Type-c改成micro USB，接口改了，有些功能应该用不了 (dws文件，gpio表，中断,dtsi文件)
	
	dtsi文件:
	kernel-4.4/archarm64/boot/dts/mediatek/mt6757.dtsi
	
	dws文件:
	vendor/mediatek/proprietary/bootable/bootloader/preloader/custom/k57pv1_6mtee_pre/dct/dct/codegen.dws
	vendor/mediatek/proprietary/bootable/bootloader/lk/target/k57pv1_6mtee_pre/dct/dct/codegen.dws
	kernel-4.4/drivers/misc/mediatek/dws/mt6757/k57pv1_6mtee_pre.dws
	
	

	
 	
 	需要用到的gpio			Type-C						mirco USB
 	
 	USB_ID					23
 	DRV_BUS					43
	GPIO_CHG_EN				100
	EINT_CHG_STAT			101(EINT)
	EINT_RT5081_PD_IRQB		8(EINT)	
	TYPEC_HEADSET_CTRL_EN	86(EINT)
	
	
	switching_charger	I2C_CHANNEL_1		0X6A
	
*****************************************************************************************************************	
	
	
	
	
	
	
	#1关机充电的流程：
	

	

	
	
	
	
	
	
   
}














/*debug*/
{
		关掉OTG功能



--->代码：

		#1编译的宏:
			充电能识别接口类型，座充有充电，USB无充电电流，对比将Type-C修改为micro USB的代码
		去掉一下的宏
		CONFIG_TCPC_CLASS=y
		CONFIG_USB_POWER_DELIVERY=y
		CONFIG_TCPC_RT5081=y
		CONFIG_DUAL_ROLE_USB_INTF=y
		
			快充定义的宏	
		CONFIG_MTK_PUMP_EXPRESS_PLUS_20_SUPPORT
		CONFIG_MTK_PUMP_EXPRESS_PLUS_30_SUPPORT
	
	
	
			充电器接口类型检测要修改P75要检测USB status	0x27
		rt5081_pmu_attachi_irq_handler这个中断回调函数会进行USB状态检测，判断是充电器还是USB   P76
	
		Device Type					0x22
	
	
	
		#2变量
		上层充电器的类型
		typedef enum {
			CHARGER_UNKNOWN = 0,
			STANDARD_HOST,		/* USB : 450mA */
			CHARGING_HOST,
			NONSTANDARD_CHARGER,	/* AC : 450mA~1A */非标准充电器
			STANDARD_CHARGER,	/* AC : ~1A */			标准充电器
			APPLE_2_1A_CHARGER,	/* 2.1A apple charger */
			APPLE_1_0A_CHARGER,	/* 1A apple charger */
			APPLE_0_5A_CHARGER,	/* 0.5A apple charger */
			WIRELESS_CHARGER,
		} CHARGER_TYPE;
	
	
	
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
	
	
		如何通过USB口检测出是标准充电器和非标准充电器
		BC1.2中定义的充电类型
		SDP：标准下行接开口，电脑USB
		CDP：充电下行接口
		DCP：标准的座充
		OCP：other chargng port非标准充电器
	
	
		根据寄存器打印的usb_status
		000: No VBUS
		001: VBUS flow is under going
	
		010: SDP (by input pin of (sVBUSPG_syn
		& sCHGDETB & DCDTB)=1)
		011: SDP NSTD (by input pin of
		(sVBUSPG_syn & sCHGDETB &
		DCDT)=1)
		100: DCP (by input pin of sVBUSGP_syn
		& CHGDET & sDCPORT_CHD)
		101: CDP (by input pin of sVBUSGP_syn
		& CHGDET & sCDPORT_CHD)
	
		110: reserved
		111: reserved
	
	
	
		switch (usb_status) {
		case RT5081_CHG_TYPE_SDP:
			chg_data->chg_type = STANDARD_HOST;
			break;
		case RT5081_CHG_TYPE_SDPNSTD:
			chg_data->chg_type = NONSTANDARD_CHARGER;
			break;
		case RT5081_CHG_TYPE_CDP:
			chg_data->chg_type = CHARGING_HOST;
			break;
		case RT5081_CHG_TYPE_DCP:
			chg_data->chg_type = STANDARD_CHARGER;
			break;
		default:
			chg_data->chg_type = CHARGER_UNKNOWN;
			break;
		}
		
		
				
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




		mtk_switch_charging.c调用不同的算法
			info->algorithm_data = swch_alg;
			info->do_algorithm = mtk_switch_charging_run;
			info->plug_in = mtk_switch_charging_plug_in;
			info->plug_out = mtk_switch_charging_plug_out;
			info->do_charging = mtk_switch_charging_do_charging;
			info->do_event = charger_dev_event;
			info->change_current_setting = mtk_switch_charging_current;


		定义电池相关参数，当然这些参数有两个方向dtsi(mt6757.dtsi,rt5081.dtsi)，和宏
			mtk_charging.h 	，mtk_charging_intf.h(快充)
			#define USB_CHARGER_CURRENT					CHARGE_CURRENT_500_00_MA	/* 500mA */
			/* #define AC_CHARGER_CURRENT					CHARGE_CURRENT_650_00_MA */
			#define AC_CHARGER_CURRENT					CHARGE_CURRENT_2050_00_MA
			#define AC_CHARGER_INPUT_CURRENT				CHARGE_CURRENT_3200_00_MA
			#define NON_STD_AC_CHARGER_CURRENT			CHARGE_CURRENT_500_00_MA
			#define CHARGING_HOST_CHARGER_CURRENT       CHARGE_CURRENT_650_00_MA
			#define APPLE_0_5A_CHARGER_CURRENT          CHARGE_CURRENT_500_00_MA
			#define APPLE_1_0A_CHARGER_CURRENT          CHARGE_CURRENT_650_00_MA
			#define APPLE_2_1A_CHARGER_CURRENT          CHARGE_CURRENT_800_00_MA



		电池计电量计算的相关参数是在哪个文件
		mtk_battery_meter.h ,mtk_battery_meter_table.h



		
		#3函数功能	
		{

	
			mtk_switch_charging_run
		
		
			(mtk_battery_hal.c) fgauge_read_current，读取充电的电量和一些算法处理
		
			mtk_switch_charging.c定义的充电状态机的各各函数
		
			mtk_battery.c force_get_tbat
		}
		
		
		设置充电电流，充电电压，充电器的检测


		
		#4log关键字
		
			ICHG(P68), 察看充电电流
			fgauge_read_current这个是电量计读出的数据，
	



--->文档

		这里有几个概念AICR，MIVR怎么设置的
			AICR就是input current limit,充电电流不能大于此值,此功能是检测充电器的input current limit，也就是输出电流的最大值，是由5081来做；
		但是只是在插入充电器的时候去做检测，直到拔出充电器再次插入才会再一次检测，同一个充电器不会多次检测输出能力，因为同一个充电器此值肯定是相同的，没必要再检测
			MIVR就是根据充电器的供电能力，设置最低的充电电压，如果电压小于这个值就降电流不降电压


		gpio，dws，dtsian 文件在preloader,lk,kernel三个阶段的配置







--->逻辑
	
	调用流程：
	{
		(
		
		
		mtk_charger.c)mtk_charger_probe: starts 初始化charger_manager结构体，首先是从mtk6757.dtsi 的charger开始对不同成员赋值，如果没有定义就从mtk_charging.h的宏赋值
		包括一些电池参数和调用充电的算法--->(mtk_switch_charging.c)选择不同的充电算法,mtk_switch_charging_init,从rt5081.dtsi获取一些参数，然后调用不同的函数指针--->
		创建一个常用的线程charger_routine_thread 检查充电与否，充电器的类型，和一些电池参数-->(mtk_battery_intf.c)battery_get_bat_current,获取充电电流--->(mtk_battery_hal.c)
		bm_ctrl_cmd定义了一些调用fuelgauge的函数指针--->charger_update_data获取电量计的状态，某种温度下，再调用force_get_tbat--->charger_check_status检测充电器的状态，
		都是基于battery thermal protection进行的检测
	
	}
	
	





--->测试场景

	

	#1
	{
		
							17G05A(71%)			W909(69%)  			G1605A(70%)
		关机充电电流:		0.1A~0.3A		0.6A~0.75A			0.75A~0.85A
		座充充电电流：		0.1A~0.3A		0.6A~0.8A			0.8A~0.9A
		USB充电电流：		0.1A~0.3A		0.2A~0.3A			0.2A~0.3A
	
		USB没有电流是串口地接到了电源地，拔掉串口后有电流，但应该不是这个原因吧
	
		没有电池是充电电流小的原因吗？(好吧这个是真实的原因，因为用的假电，所以电量肯定充不进去)
	
	
	}
	
	
	#2
	{
		手机功耗大，开机过程能达到1A，息屏200mA~300mA,而USB充电电流太小，最后充的没有耗的多
	
	}
	


	充电头				5V/2A				5V/1A				9V/2A
		
	
	
	
	
}












双芯片管理充电MT6355和RT5081
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
	
	
	
	battery_get_bat_current_sign这个应该是fuelgauge在读取寄存器g_fg_is_charging，读取IC是充电还是放电
	
	
	
	
	
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
	
	#1关机充电的流程：
	

	

	
	
	
	
	
	
   
}





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










/*debug*/
{


BUG#1
						17G05A(71%)			W909(69%)  			G1605A(70%)
	关机充电电流:		0.1A~0.3A		0.6A~0.75A			0.75A~0.85A
	座充充电电流：		0.1A~0.3A		0.6A~0.8A			0.8A~0.9A
	USB充电电流：		0.1A~0.3A		0.2A~0.3A			0.2A~0.3A
	
	USB没有电流是串口地接到了电源地，拔掉串口后有电流，但应该不是这个原因吧
	
	没有电池是充电电流小的原因吗？(好吧这个是真实的原因，因为用的假电，所以电量肯定充不进去)
	
	{
		Average Input Current Regulation (AICR) : 0.1A to 3.25A in 50mA steps
	
		CHG_ILIM :Input current limit setting input. A resistor is connected from CHG_ILIM pin
		to ground to set the maximum input current limit. The actual input current
		limit is the lower value set through the CHG_ILIM pin and IAICR register
		bits.

		IAICR:

	}
	
	

BUG#2
	充电能识别接口类型，座充有充电，USB无充电电流，对比将Type-C修改为micro USB的代码
	
	
	
	
	
	

}










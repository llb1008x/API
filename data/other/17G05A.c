/*********************************************************************************
		毕业后第一份工作的第一个正式项目，一定要认真细致负责，做好每一个环节
**********************************************************************************/


/*重要的概念*/
{
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


	AICR，MIVR
	{
		AICR:input current limit,充电电流不能大于此值,此功能是检测充电器的input current limit，也就是输出电流的最大值，是由5081来做；
	  但是只是在插入充电器的时候去做检测，直到拔出充电器再次插入才会再一次检测，同一个充电器不会多次检测输出能力，因为同一个充电器此值肯定是相同的，没必要再检测
	  MIVR就是根据充电器的供电能力，设置最低的充电电压，如果电压小于这个值就降电流不降电压

	}



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


		rt5081提供的函数指针
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


	log搜索关键字
	{


		
	}



}





/*调用流程，代码逻辑*/
{
	(mtk_charger.c)mtk_charger_probe: starts 初始化charger_manager结构体，首先是从mtk6757.dtsi 的charger开始对不同成员赋值，如果没有定义就从
	
	mtk_charging.h的宏赋值包括一些电池参数和调用充电的算法--->(mtk_switch_charging.c)选择不同的充电算法,mtk_switch_charging_init,从rt5081.dtsi
	
	(里面的一些参数是从rt5081_pmu_core.c获取匹配，然后再到不同的项里面匹配)获取一些参数，然后调用不同的函数指针--->创建一个常用的线程charger_routine_thread
	 
	检查充电与否，充电器的类型，和一些电池参数-->(mtk_battery_intf.c)battery_get_bat_current,获取充电电流--->(mtk_battery_hal.c)bm_ctrl_cmd
	
	定义了一些调用fuelgauge的函数指针--->charger_update_data获取电量计的状态，某种温度下，再调用force_get_tbat--->charger_check_status检测充电器的状态，
	
	都是基于battery thermal protection进行的检测--->kpoc_power_off_check应该是关机充电的检测



rt5081.dtsi参数在rt5081_pmu*的of_device_id里面获取的
meter,table：这两个都是电量计算的相关参数
}

	






/*debug*/
{
1.关闭软件关机重启

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

/*******************************************************************************************************************/
2.充电电流太小
	充电电流太小：标准充电器（1.9A） USB充电（500mA）

_rt5081_set_aicr这个是设置电流的，dts文件中是设置初始化的值，但是关于充电电流的设置它是每次插入充电器的时候检测
不断提高充电电流step by step,直到达到最大能力，拔除的时候就将aicr这个值清零，所以每次充电电流都会重新检测充电能力
但是aicr这个值不是很准确，有一定的误差

 rt5081_pmu_charger: _rt5081_set_aicr: 500000mA over TA's cap, can only be 300000mA

if (chg_data->aicr_limit != -1 && uA > chg_data->aicr_limit) {
	dev_err(chg_data->dev,
		"%s: %dmA over TA's cap, can only be %dmA\n",
		__func__, uA, chg_data->aicr_limit);
	uA = chg_data->aicr_limit;
}
	
mivr这个值最低在4.3~4.4V之间，电池电压在4.3V左右，而电池的充电电流是根据充电器电压和电池电压之间的电压差，决定

	






/****************************************************************************************************************/
3.mmi测试
	mmi测试代码宏
	{
		GN_RW_GN_MMI_BACKUP_TO_PRODUCTINFO=yes
		GN_RO_GN_GNROMVERNUMBER=GIONEE ROM5.0.16
		GN_APK_MMI_SUPPORT_BJ=yes
		GN_APK_AUTOMMI_SUPPORT=yes
	}

	一是添加BAT_BatteryPresentCurrent，类似mt_battery_update_EM，看mmi测试界面是否有充电部分

	二是根据着mmi测试宏包含的代码







/****************************************************************************************************************/
	4.快充升压问题



}







/*涉及到的一些文件*/
{
dtsi文件:
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


GM3.0 定义的宏CONFIG_MTK_GAUGE_VERSION=30
		./arch/arm64/configs/k57pv1_6mtee_pre_debug_defconfig:377:CONFIG_MTK_GAUGE_VERSION=30
		./arch/arm64/configs/k57pv1_6mtee_pre_defconfig:361:CONFIG_MTK_GAUGE_VERSION=30
		./arch/arm64/boot/dts/mediatek/mt6757.dtsi:1216:#if (CONFIG_MTK_GAUGE_VERSION == 30)


rt5081的目录
	/home/llb/project/PRO/source/17G05A/L30_6757_17G05A_N0.MP5_161227_ALPS/android_mtk_6757_mp/kernel-4.4/drivers/misc/mediatek/pmic/rt5081/
	

keyboard 的目录
/home/llb/project/PRO/source/17G05A/L30_6757_17G05A_N0.MP5_161227_ALPS/android_mtk_6757_mp/kernel-4.4/drivers/input/keyboard/mediatek/mt6757


}





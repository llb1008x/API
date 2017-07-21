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


		/*有时候要看源码目录下的makefile文件，他制定了编译的规则，哪些是需要的*/
		项目脚本里哪些宏是需要的，宏包含的代码有哪些？这点很重要
}




/*重要的概念*/
{
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




	**************************************************************************************
		info->enable_dynamic_cv = true；
		mtk_get_dynamic_cv

	？“所以这时候要看MIVR中断是否触发，触发后AICL是否也对应着改变了？”
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


	log搜索关键字
	{
		可以通过检索Call trace，察看内存堆栈函数的调用


		ibus，ibat可以检索相关的电流电压信息，这写都在一块

		[248:charger_thread][name:mtk_charger&]Vbat=4321,I=3597,VChr=492,T=33,Soc100,CT:0:1

		控制fuel_gauge的log等级
    	FG_daemon_log_level 	

		rtc记录的电量
		BATTERY_METER_CMD_SET_RTC_UI_SOC	
		获取上层电量
		FG_status.ui_soc=battery_get_bat_uisoc
    

		healthd线程检测电池的状态
		healthd]: healthd: battery l=5 v=3796 t=41.0 h=2 st=4 chg=	


		FG_DAEMON_CMD_GET_VBAT
		FG_DAEMON_CMD_GET


	}



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


关掉2sec断电重启
{
	cust_rtc.h

	#define RTC_DEFAULT_YEA		2010
	#define RTC_DEFAULT_MTH		1
	#define RTC_DEFAULT_DOM		1
	#define RTC_2SEC_REBOOT_ENABLE  1		//控制使能
	#define RTC_2SEC_MODE		2			//控制时间
}

/*******************************************************************************************************************/
2.充电电流太小
	充电电流太小：标准充电器（1.9A） USB充电（500mA）
	实际只有 标充1.6A~1.8A ，USB充电400mA~460mA

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


/***************************************************************************************************************/
3.按键功能
	因为framework层还有部分未导入，所以检测到一些未知的键值，出现异常？
	mmi测试显示一些按键还有问题？
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






	{
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
		gionee/code/alps/public/ROM/frameworks/base/services/core/java/com/android/server/policy/PhoneWindowManager.java:7077:            case KeyEvent.KEYCODE_WWW:
	}














/**************************************************************************************************/
4.调试马达震动效果

基本概念
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

重要的控制
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


        shutdown_event_handler做了哪些事：
        {   
            1.执行关机的回调函数，判断一些关机条件是否满足或触发然后执行kernel_power_off -> machine_power_off
            mtk_power_misc_init     power_misc      power_misc_thread


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

        int g_FG_PSEUDO1_T0[TOTAL_BATTERY_NUMBER] = { 1, 6, 7, 8};		// 5->2->1
        int g_FG_PSEUDO1_T1[TOTAL_BATTERY_NUMBER] = { 2, 10, 11, 12};	// 9->3->2
        int g_FG_PSEUDO1_T2[TOTAL_BATTERY_NUMBER] = { 3, 14, 15, 16};	// 13->5->3
        int g_FG_PSEUDO1_T3[TOTAL_BATTERY_NUMBER] = { 5, 18, 19, 20};	// 17->7->5
        int g_FG_PSEUDO1_T4[TOTAL_BATTERY_NUMBER] = { 7, 22, 23, 24};	// 21->10->7

        int g_FG_PSEUDO100_T0[TOTAL_BATTERY_NUMBER] = { 99, 98, 98, 97};// 98->99
        int g_FG_PSEUDO100_T1[TOTAL_BATTERY_NUMBER] = { 98, 95, 94, 93};// 95->98	
        int g_FG_PSEUDO100_T2[TOTAL_BATTERY_NUMBER] = { 96, 90, 90, 89};// 90->95->96
        int g_FG_PSEUDO100_T3[TOTAL_BATTERY_NUMBER] = { 93, 80, 86, 85};// 80->90->93
        int g_FG_PSEUDO100_T4[TOTAL_BATTERY_NUMBER] = { 93, 80, 82, 81};// 80->90->93	
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
            int g_Q_MAX_T0[TOTAL_BATTERY_NUMBER] = { 2946, 2712, 2490, 1965};   // 2946->2800->3100
            int g_Q_MAX_T1[TOTAL_BATTERY_NUMBER] = { 2796, 2851, 2468, 1984};   // 2796->2657->2936
            int g_Q_MAX_T2[TOTAL_BATTERY_NUMBER] = { 2718, 2432, 2310, 1946};   // 2718->2583->2854
            int g_Q_MAX_T3[TOTAL_BATTERY_NUMBER] = { 2535, 1991, 1858, 1873};   // 2535->2410->2662
            int g_Q_MAX_T4[TOTAL_BATTERY_NUMBER] = { 2523, 1960, 1843, 1851};   // 2523->2400->2649

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
    }
 
     17G05A

     //Gionee <gn_by_charging> <lilubao> <20170519> add for thermal charging begin
	 pr_err("in %s info->battery_temperature->%d\n",__FUNCTION__,info->battery_temperature);

	 if(info->chr_type==STANDARD_CHARGER){

		if( info->battery_temperature <=15 ){

			pdata->charging_current_limit=1500000;
			pdata->input_current_limit=1600000;
			pr_err("in %s ,temperature is too low ,we need limit charging current->1\n",__FUNCTION__);
		}else if( (info->battery_temperature>15)&&(info->battery_temperature<=45) ){

			pr_err("in %s ,temperature in normal range,do not limit charging current->2 \n",__FUNCTION__);
		}else if( info->battery_temperature > 45 ){

			pdata->charging_current_limit=1000000;
			pdata->input_current_limit=1200000;
			pr_err("in %s ,temperature is too high ,we need limit charging current->3\n",__FUNCTION__);
		}
	 }
	 pr_err("in %s setting charging_current_limit->%d,input_current_limit->%d\n",
				__FUNCTION__,pdata->charging_current_limit,pdata->input_current_limit);
	 //Gionee <gn_by_charging> <lilubao> <20170519> add for thermal charging end	


    2.充电温度调节策略相关代码，文档  
    {
        mtk_cooler_bcct_v1.c这个文件应该充电调节温度有关
        （mtk_cooler_bcct_v1.c）mtk_cooler_bcct_init初始化充电温升 降电流 -> mtk_cooler_bcct_register_ltf这个是注册一个中断，设置回调函数

        struct chrlmt_handle{chr_input_curr_limit;bat_chr_curr_limit;pep30_input_curr_limit;}当温度达到某个状态的时候设置限定的充电电流
        
        -> 这边注册了三个降温相关的策略 bcct，abcct，lcmoff的中断 -> 创建proc file节点 时时反应设备的状态 -> chrlmt_set_limit_handler

        触发条件就设置限定电流的回调函数 -> charger_manager_set_input_current_limit 通过这个函数设置进充电器的和进电池的 


        device/mediatek/mt6757/thermal.conf这个是温升调节策略相关的配置文件,这个文件参数的意思


        bcct,abcct
         1、bcct：Thermal config tool中bcct策略最多设置3个温度点分别调用3种充电电流，当温度触发条件满足的时候直接降电流
         2、abcct：Thermal config tool中abcct策略是使用当前板温与目标板温的差值计算下一时刻要调节的充电电流，循环调节直到当前板温=目标板温。
         可设定充电电流的最大值和最小值。

        Note：bcct和abcct两套机制可以共存，如两套机制同时生效，则取较小的充电电流值进行调节
    }  
        

    3.充电温度调节调用流程 
    {
     （mtk_thermal_monitor.c） mtkthermal_init，mtk thermal相关的初始化，创建调试调用节点/proc/driver/thermal，在这个目录下

      建立一系列的proc 节点，/proc/mtkcooler这个目录下是降温策略的设备节点 -> (mtk_cooler_shutdown.c) mtk_cooler_shutdown_init

      这个应该跟过温启动关机策略相关的，温度过高触发条件，关机策略应该是最直接，最有效的，但是影响很大，后面还有几个相关的模块的初始化，都是在proc目录下

      创建一系列节点，然后注册函数，相关的操作函数指针 -> (mtk_ts_cpu.c) tscpu_init注册驱动,这个对整个系统的温升有很大影响，tscpu是一个虚拟的thermal_zone，
      
      主要是监控cpu的状态 -> tscpu_thermal_probe   
    }   


    4.相关的文档
    {
        Thermal_Management_MT6757.pdf

        MT6757CH(CD)_Thermal_Design_Notices_V0.1.pdf
    }

/************************************************************************************************************/
	17G10A主板显示电池温度50~55，温度过高导致的停止充电问题
    {
        NTC faild
        
        在mtk_charger.c  charger_check_status会检查充电的状态
        发现电池的温度为50度超过了最大的充电温度，启动保护措施，然后就停止充电了
        (但是这里电池温度没有50度，检测可能有问题，先提高充电的最大温度)
        这个值在dts文件里获取的

        /*debug*/
        {
            过温保护门限设为55

            mt6757.dts
            //Gionee <gn_by_charging> <lilubao> <20170621> add fixed over temperature begin
		    max_charge_temperature = <55 >;			// 50 -> 55
		    //Gionee <gn_by_charging> <lilubao> <20170621> add fixed over temperature end

        }
    }


}




/************************************************************************************************************/
9.17G10A mmi测试读取的数据有问题
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

		/*修改*/
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

}


/***********************************************************************************************/
12.	usb眼图问题
{
	近端跟远端的测试要求不一样
	
	高通USB眼图 80-PB524-1 A
	phy,override
	
	17G10A相关的宏,通过相关的宏找代码
	{
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
	
		hal,drv,phy
	}
	
	17G10A的USB体系架构
	{
		这两个对应的寄存器和操作的函数
		USB_DP_P0		D31
		USB_DM_P0		D32
	
		MTK case ID：ALPS03412756 

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















/*涉及到的一些文件*/
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
17G10A/L31_6757_66_N_17G10A_NO.MP5_V1.53_170512_ALPS/android_mtk_mp/device/mediatek/mt6757


编译相关的脚本工具
	android_mtk_6757_mp/gionee/config/tools

电池曲线
	mtk_battery_table.h	
}





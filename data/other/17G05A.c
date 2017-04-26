/*********************************************************************************
		毕业后第一份工作的第一个正式项目，一定要认真细致负责，做好每一个环节
**********************************************************************************/


/*重要的概念*/
{
		这几个概念有必要理解一下
	{   
		

	->  AICR:Average Input Current Regulation (AICR) : 0.1A to
		3.25A in 50mA steps
		充电电流不能大于此值,此功能是检测充电器的input current limit，也就是输出电流的最大值，是由5081来做；
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

    ->swchg_turn_on_charging(info)充电使能



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


/****************************************************************************************************************/
	5.按键功能
	因为framework层还有部分未导入，所以检测到一些未知的键值，出现异常

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
17G05A/L30_6757_17G05A_N0.MP5_161227_ALPS/android_mtk_6757_mp/kernel-4.4/drivers/misc/mediatek/pmic/rt5081/
	

keyboard 的目录
17G05A/L30_6757_17G05A_N0.MP5_161227_ALPS/android_mtk_6757_mp/kernel-4.4/drivers/input/keyboard/mediatek/


马达的目录
17G05A/L30_6757_17G05A_N0.MP5_161227_ALPS/android_mtk_6757_mp/kernel-4.4/drivers/misc/mediatek/vibrator


}





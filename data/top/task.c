





17G05A  软件关机重启,充电调试 (这个是重点),17G05A的mmi测试
/*debug*/
{
软件关机重启:解决



mmi测试：加入充电测试选项





充电电流太小：标准充电器（1.9A） USB充电（500mA）
{
->2017.4.14
    {
         标准充电器（1.6A） USB充电（450mA）
         修改了：
         MTK_PUMP_EXPRESS_PLUS_20_SUPPORT = yes
         mivr = <4400000>;	/* uV 4500000->4400000*/
    }
   


rt5081_pmu_irq_.c    
    2.rt5081_pmu_irq_handler
    中断回调函数

rt5081_pmu_charger.c
    1.rt5081_pmu_ovpctrl_uvp_d_evt_irq_handler
    这段走的是非type-c接口
    rt5081_pmu_ovpctrl_uvp_evt_irq_handler

    3.rt5081_pmu_attachi_irq_handler
    这个应该是rt5081接受到中断后，就是接口插入的动作还有一个rt5081_pmu_detachi_irq_handler是接口把出的动作

        4.rt5081_inform_psy_changed
        将检测到的信息上报到power_supply子系统

mtk_charger.c
        5.mtk_charger_int_handler
        mt_charger对应的中断回调函数

        6._wake_up_charger
        唤醒mt_charger

        /* Turn on USB charger detection */
        7.rt5081_enable_chgdet_flow
        进行USB充电使能的检测

        8.rt5081_set_usbsw_state
        设置USB的状态

        9.mtk_is_charger_on->mtk_is_charger_in->rt5081_plug_in
 
 rt5081_pmu_charger.c       
        10.rt5081_plug_in
        rt5081检测到充电器插入所要进行的动作

mtk_charger.c
        11.battery_callback
        对电池状态变化上报给power_supply子系统

        12.rt5081_enable_charging                               0x12
        充电使能，这个充电使能只是简单的对寄存器使用set或者clear，置位或者清空

        13.rt5081_enable_ilim
        充电电流是否限制

mtk_switch_charging.c
        14.mtk_switch_charging_run
        运行充电正常的状态机，这些数据的初始化应该在init内

        15.mtk_switch_chr_cc
        恒流充电模式

        /* turn on LED */
	    swchg_turn_on_charging(info);








charger_routine_thread这个是常用的电池状态检测线程

(rt5081_pmu_charger.c) rt5081_chg_ops  rt5081提供的函数接口


RT5081_PMU_REG_CHGCTRL2这个寄存器写1可以将充电线上的电直接提供给系统不给电池
1. TE (0x12, bit[4]) : If this bit is enabled, the power path
will be turned off, and the buck of the charger will keep
providing power to the system



设置点电流跟aicr这个变量相关
{   
    这几个概念有必要理解一下

->  AICR:Average Input Current Regulation (AICR) : 0.1A to
    3.25A in 50mA steps

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


->Enable Bits for Charger
    充电使能控制的三个寄存器：
    1.CFO_EN
    2.CHG_EN
    3.HZ

Use MIVR to enable/disable power path
    Disable MIVR IRQ -> enable direct charge
    Enable MIVR IRQ -> disable direct charge


**************************************************************************************
    info->enable_dynamic_cv = true；
    mtk_get_dynamic_cv

   ？“所以这时候要看MIVR中断是否触发，触发后AICL是否也对应着改变了？”
    Do AICL in a workqueue after receiving MIVR IRQ
    rt5081_run_aicl


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

    RT5081_PMU_REG_LG_CONTROL新加的

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


struct charger_manager {
	bool init_done;
	const char *algorithm_name;
	struct platform_device *pdev;
	void	*algorithm_data;
	int usb_state;
	bool usb_unlimited;     <---这个量应该跟USB限流有关的

	struct charger_device *chg1_dev;
	struct notifier_block chg1_nb;
	struct charger_data chg1_data;
	struct charger_consumer *chg1_consumer;

	struct charger_device *chg2_dev;
	struct notifier_block chg2_nb;
	struct charger_data chg2_data;

	CHARGER_TYPE chr_type;
	bool can_charging;

	int (*do_algorithm)(struct charger_manager *);
	int (*plug_in)(struct charger_manager *);
	int (*plug_out)(struct charger_manager *);
	int (*do_charging)(struct charger_manager *, bool en);
	int (*do_event)(struct notifier_block *nb, unsigned long event, void *v);
	int (*change_current_setting)(struct charger_manager *);

	/*notify charger user*/
	struct srcu_notifier_head evt_nh;
	/*receive from battery*/
	struct notifier_block psy_nb;

	/* common info */
	int battery_temperature;

	/* sw jeita */
	bool enable_sw_jeita;
	struct sw_jeita_data sw_jeita;

	/* dynamic_cv */
	bool enable_dynamic_cv;

	bool cmd_discharging;
	bool safety_timeout;
	bool vbusov_stat;

	/* battery warning */
	unsigned int notify_code;
	unsigned int notify_test_mode;

	/* battery thermal protection */
	struct battery_thermal_protection_data thermal;

	/* dtsi custom data */
	struct charger_custom_data data;

	bool enable_sw_safety_timer;
	bool enable_pe_plus;

	/* High voltage charging */
	bool enable_hv_charging;

	/* pe 2.0 */
	bool enable_pe_2;
	struct mtk_pe20 pe2;

	/* pe 3.0 */
	bool enable_pe_3;
	struct mtk_pe30 pe3;
	struct charger_device *dc_chg;

	/* thread related */
	struct hrtimer charger_kthread_timer;
	struct fgtimer charger_kthread_fgtimer;

	struct wake_lock charger_wakelock;
	struct mutex charger_lock;
	spinlock_t slock;
	unsigned int polling_interval;
	bool charger_thread_timeout;
	wait_queue_head_t  wait_que;
	bool charger_thread_polling;
};

默认的值
/* These default values will be used if there's no property in dts */
static struct rt5081_pmu_charger_desc rt5081_default_chg_desc = {
	.ichg = 2000000,		/* uA */
	.aicr = 500000,			/* uA */
	.mivr = 4400000,		/* uV */
	.cv = 4350000,			/* uA */
	.ieoc = 250000,			/* uA */
	.safety_timer = 12,		/* hour */
#ifdef CONFIG_MTK_BIF_SUPPORT
	.ircmp_resistor = 0,		/* uohm */
	.ircmp_vclamp = 0,		/* uV */
#else
	.ircmp_resistor = 25000,	/* uohm */
	.ircmp_vclamp = 32000,		/* uV */
#endif
	.dc_wdt = 4000000,		/* us */
	.en_te = true,
	.en_wdt = true,
};






}

















按键不管用


马达振动


关掉OTG功能



电池曲线的导入    

}



G1605A  售后问题，功耗问题 ， 电池维护特性，healthd线程
/*debug*/
{
    售后问题：   电量计不准的机器
    这个还不知道怎么看，只是觉得电量跟电池电压偏差有点大，而且底层电量跟上层显示的电量差别也很大



    apk发包次数，谁发的(主从)，就是链接到哪了？？




    电池维护特性：
        机器有问题了，tp不好使怎么办，万一还不了QAQ
        正在验证

    

}


















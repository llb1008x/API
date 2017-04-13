





17G05A  软件关机重启,充电调试 (这个是重点),17G05A的mmi测试
/*debug*/
{
软件关机重启:解决



mmi测试：加入充电测试选项


充电电流太小：标准充电器（1.9A） USB充电（500mA）
{

rt5081_pmu_irq_.c    
    2.rt5081_pmu_irq_handler
    中断回调函数

rt5081_pmu_charger.c
    1.rt5081_pmu_ovpctrl_uvp_d_evt_irq_handler
    这段走的是非type-c接口
    rt5081_pmu_ovpctrl_uvp_evt_irq_handler

    3.rt5081_pmu_attachi_irq_handler
    这个应该是rt5081接受到中断后，就是接口插入的

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
    AICR:Average Input Current Regulation (AICR) : 0.1A to
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







}

















按键不管用


马达振动


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


















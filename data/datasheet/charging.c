/*************************************************************************************************
								充电相关的知识
*************************************************************************************************/

->/*电源管理的整个框架(涉及的面很广)*/
{
-->基本概念
{
	hibernate：冬眠	hibernation image

}



-->linux时钟系统（这个不是不想看，有点困难）


{
   MT6351芯片的接口 charging_hw_bw25896.c

   CONFIG_MTK_UART_USB_SWITCH，USB转串口开关

   fg_coulomb：库仑计这个变量，记录电池电量的变化，充电为正递增，放电为负递减，插拔充电器新计数，电量计的多少表示电池电量变化的
   gFG_coulomb库伦积分值通过都去IC的寄存器就可以获得，battery_meter_ctrl(BATTERY_METER_CMD_GET_HW_FG_CAR_ACT, &fg_coulomb);

   init.mt6735.rc系统初始化用到的，创建一些设备节点，赋值一定的权限，配置节点，和其他初始化要用到的init文件
   batterymonitor.cpp上层管理的
   
	/sys/kernel/debug/   有一系列调试的点
	usb   wakeupsource
	这些点就是dump打印出来的文件

	pmic子系统和battery_meter都要初始化完成才可以充电

	功耗，降电流问题
	飞行模式下的电流是否异常；应用是否经常唤醒，唤醒源；隔离大的驱动模块，看各各部分耗电是否异常；gpio的配置问题，不工作时的suspend模式，
	gpio引脚是否被上拉导致一直在耗电，probe函数。suspend,resume

	android drone： android 无人机
	access point：桥接器，访问节点

}






-->linux 下的suspend模式，降低功耗：

     在Linux中,休眠主要分三个主要的步骤:

     1、冻结用户态进程和内核态任务

     2、调用注册的设备的suspend的回调函数

     3、顺序是按照注册顺序


	 freeze task，suspend_to_idle

	 进程冻结技术：冻结的对象是内核中可以被调度执行的实体，包括用户进程、内核线程和work_queue。
	 标记系统freeze状态的有三个重要的全局变量：pm_freezing、system_freezing_cnt和pm_nosig_freezing，如果全为0，
	 表示系统未进入冻结；system_freezing_cnt>0表示系统进入冻结，pm_freezing=true表示冻结用户进程，pm_nosig_freezing=true
	 表示冻结内核线程和workqueue。它们会在freeze_processes和freeze_kernel_threads中置位，在thaw_processes和
	 thaw_kernel_threads中清零。

	 判断标志位，信号处理

	 调用__refrigerator进入冻结
	 /* Refrigerator is place where frozen processes are stored :-). */
	bool __refrigerator(bool check_kthr_stop)
	{
		/* Hmm, should we be allowed to suspend when there are realtime
		processes around? */
		bool was_frozen = false;
		long save = current->state;

		pr_debug("%s entered refrigerator\n", current->comm);

		//死循环相当于while（1）
		for (;;) {
			set_current_state(TASK_UNINTERRUPTIBLE);

			spin_lock_irq(&freezer_lock);
			current->flags |= PF_FROZEN;
			if (!freezing(current) ||
				(check_kthr_stop && kthread_should_stop()))
				current->flags &= ~PF_FROZEN;
			spin_unlock_irq(&freezer_lock);

			if (!(current->flags & PF_FROZEN))
				break;
			was_frozen = true;
			schedule();
		}

		pr_debug("%s left refrigerator\n", current->comm);

		/*
		* Restore saved task state before returning.  The mb'd version
		* needs to be used; otherwise, it might silently break
		* synchronization which depends on ordered task state change.
		*/
		set_current_state(save);

		return was_frozen;
	}

Q：
	 1.android处于suspend或者休眠状态时有哪些还是处在active的？
	 	modem会定期唤醒，还有些暂时不清楚，CPU不读取指令

	 2.没有使用冻结技术的哪些线程，进程要怎样处理
	   系统休眠的时候基本上是要冻结的，没有冻结的系统就没有睡下去，异常唤醒。


-->cpu频率的动态调节，cpufreq,big+little

	高性能的CPU，频率高，性能高，功耗也高，big
	低性能的CPU，频率低，性能低，功耗低，little

	有时候CPU分大核，小核的，性能不都一样，但是kernel 的schedule调度的时候是分辨不了的，视所有的CPU都一样，
	正常工作情况下，CPU都有自己的cpufreq table，根据相应的需求使用不同的freq，但是对SMP需要littel+big的组合，
	虚拟CPU core
	{
	当bL switching处于enable状态时，该driver变成一个特殊的cpufreq driver，在调整频率的时候，可以根据情况，切换core的cluster。


    此时只有4个虚拟的core对系统可见（由arm bL switcher driver控制，3.2节会介绍），系统不关心这些core属于哪个cluster、是big core还是Little core；

    确切的说，每一个虚拟的core，代表了属于两个cluster的CPU对，可以想象为big+Little组合，只是同一时刻，只有一个core处于enable状态（big or Little）；

    该driver会搜集2个cluster的frequency table，并合并成一个（保存在freq_table[2]中）。合并后，找出这些frequency中big core最小的那个（clk_big_min）
	以及Little core最大的那个（clk_little_max）；

    基于cpufreq framework进行频率调整时，如果所要求的频率小于clk_big_min，则将该虚拟core所对应的Little core使能，如果所要求得频率大于clk_little_max，
	则将该虚拟core所对应的big core使能。	

	}
	



}






/*qualcomm高通相关*/
{

  1.pm8998,pm8005,and pmi8998_power_management_ics_design_guidelines.pdf
  PM8998，PM8005作为pmic子系统供电管理的核心
  PMI8998管理系统供电的接口，充电，SMB1381充电

  整个系统比较耗电的部分：CPU,modem，LCD,audio,haptic,LED?

  pmic电源管理子系统的框架及IC
  {
	PM8998:  Most of the housekeeping (including system clocks) and
	output power management functions(core负责整个系统的日常电源管理包括系统时钟，和输出电能管理  可能是整个系统个模块能量的供应)

	PMI8998：Most of the input power management functions (including
	charger) and user interfaces（绝大部分充电的管理，还有一部分是SMB1381负责charging和提供用户接口）

	PM8005：Regulator integration on slave PMIC helps to balance
	power and thermal dissipation on either side of the MSM
	and optimize PDN routing（这个应该是起到辅助作用，充电时分散接口部分过热，降低充电过程不必要的功耗提高充电效率，优化给整个系统供电）

	SMB1381：Simultaneous DC_IN charging + OTG support via SMB1381 in parallel charging configuration
    （通过SMB1381支持OTG功能，分摊另一部分的充电功能）
  }


  Input Power Management  
  1.三个独立引脚可以输入供应能量USB_IN, DC_IN, and Vbatt
  2.modem模块的能量供应通过VPH_PWR而不是VBAT
  3.DC_IN and USB_IN的输入电压必须在UVLO，DVLO之间，P46
  4.当充电器移除后立刻停止充电，PMI8998进入低功耗模式，阻止电池的漏电流，延长寿命


  USB type-c的检测：
  设备的连接和断开通过检测CC引脚上的电压，并管理VBUS，当设备没有插入时关闭VBUS
DRP在待机模式下每50ms（50~100ms）在DFP和UFP间切换一次。当切换至DFP时，CC管脚上必须有一个上拉至VBUS的电阻Rp或者输出一个电流源，当切换至UFP时，
CC管脚上必须有一个下拉至GND的电阻Rd。此切换动作必须由CC Logic芯片来完成。当DFP检测到UFP插入之后才可以输出VBUS，当UFP拔出以后必须关闭VBUS。
此动作必须由CC Logic芯片来完成。

  sink mode：在tCCDebounce (100 to 200 ms)的检测中，CC引脚的电压在0.25V~2.04V，则为充电模式
  source mode：在tCCDebounce (100 to 200 ms)的检测中，CC引脚的电压小于1.65V，则为反向充电模式

  PMI8998可以配置一个低功耗模式，降低充电反向充电所消耗的能量

  上面有一个crude sensor好像会产生一些异常波形，正是的要关闭或者修改一些代码


  BC.1.2检测


  QC2.0 3.0检测
  手机端发送几个升降时序，充电器端回应一定的时序则是QC3.0，否则是QC2.0


  Power Sink Mode流程太长... 

  Power source mode

  Dead or Weak Battery Mode


  17G08A充电的过程：
  		5v，0.1A接入-> 1s后对电源进行第一次批量脉冲电流探测，开始QC握手->1.5s后限制输入电流为0.42A，
  尝试启动QC 9V（启动成功）->0.5s后尝试输入大电流(2A)->如果发现电源不足18W，就会进行第二次
  批量脉冲电流探测模式->3s后，根据探测结果限制输入电流为一个值，开始正式快速充电

}






-->/*pmic*/
{

	1.充电的基本流程，电量的校准，库仑计参数，电量上报

充电相关的结构体：每个变量对应要调用的函数？在那改变的，log打印出来
struct PMU_ChargerStruct {
	bool bat_exist;//电池是否存在
	bool bat_full;//电池是否充满
	s32 bat_charging_state;//电池充电的状态
	u32 bat_vol;//电池的平均电压
	bool bat_in_recharging_state;//电池是否在回充
	u32 Vsense;//电池的瞬间电压
	bool charger_exist;//充电器是否存在
	u32 charger_vol;//充电器的电压
	s32 charger_protect_status;//充电保护状态，过温，过压保护状态
	s32 ICharging;//充电电流
	s32 IBattery;//流过电池的电流 
	s32 temperature;//电池的温度
	s32 temperatureR;//电阻的温度？
	s32 temperatureV;//？
	u32 total_charging_time;//充电的总时间
	u32 PRE_charging_time;//pre充电时间，充电刚开始的涓流充电，事电池电压达到3.4v
	u32 CC_charging_time;//恒流充电
	u32 TOPOFF_charging_time;//恒压充电最后的补充充电
	u32 POSTFULL_charging_time;//postfull？
	u32 charger_type;//充电器的类型
	s32 SOC;//底层电量
	s32 UI_SOC;//上层UI显示的电量
	u32 nPercent_ZCV;//N%同步点对应的电压
	u32 nPrecent_UI_SOC_check_point;//N%同步点
	u32 ZCV;//当前的开路电压


电量计算这里基本上会设备几battery_meter_fg_20.c这个文件，就是电量计




ZCV表格，定义的数组，电池曲线数据，这组数据在mt_battery_meter.h  和mt_battery_meter_table.h两个文件内，记录不同-10，0，25，50三个温度下的开机log内的fuel_gauge参数，通过软件
导出修改数据后的文件，替换原来的文件。

电池曲线的导入？

放电深度
	/* T2 25C */
BATTERY_PROFILE_STRUCT battery_profile_t2[] = {
	{0,4383},
	{1,4370},	
	{2,4359},
	{3,4348},
	{4,4337},
	{5,4326},
	{6,4315},
	{7,4303},
	{8,4292},
	{9,4281},
	{9,4270},
	{10,4258},
	{11,4247},
	{12,4236},
	{13,4225},
	{14,4214},
	{15,4203},
	{16,4192},
	{17,4182},
	{18,4171},
	{19,4161},
	{20,4150},
	{21,4139},
	{22,4129},
	{23,4119},
	{24,4108},
	{25,4098},
	{26,4088},
	{26,4079},
	{27,4071},
	{28,4066},
	{29,4058},
	{30,4045},
	{31,4029},
	{32,4015},
	{33,4003},
	{34,3994},
	{35,3988},
	{36,3983},
	{37,3977},
	{38,3971},
	{39,3963},
	{40,3955},
	{41,3946},
	{42,3936},
	{43,3925},
	{43,3913},
	{44,3901},
	{45,3891},
	{46,3883},
	{47,3875},
	{48,3869},
	{49,3863},
	{50,3857},
	{51,3852},
	{52,3847},
	{53,3842},
	{54,3838},
	{55,3833},
	{56,3829},
	{57,3825},
	{58,3821},
	{59,3817},
	{60,3813},
	{61,3810},
	{61,3807},
	{62,3803},
	{63,3800},
	{64,3797},
	{65,3795},
	{66,3792},
	{67,3789},
	{68,3787},
	{69,3784},
	{70,3781},
	{71,3778},
	{72,3775},
	{73,3771},
	{74,3768},
	{75,3765},
	{76,3762},
	{77,3758},
	{78,3755},
	{78,3751},
	{79,3747},
	{80,3745},
	{81,3742},
	{82,3738},
	{83,3733},
	{84,3727},
	{85,3721},
	{86,3716},
	{87,3710},
	{88,3703},
	{89,3695},
	{90,3693},
	{91,3691},
	{92,3690},
	{93,3689},
	{94,3688},
	{95,3685},
	{95,3678},
	{96,3654},
	{97,3612},
	{98,3557},
	{99,3484},
	{100,3374}
}


内阻曲线
/* T2 25C */
R_PROFILE_STRUCT r_profile_t2[] = {
	{ 131 ,4383 },
	{ 131 ,4370 },	
	{ 133 ,4359 },
	{ 134 ,4348 },
	{ 133 ,4337 },
	{ 134 ,4326 },
	{ 134 ,4315 },
	{ 134 ,4303 },
	{ 135 ,4292 },
	{ 135 ,4281 },
	{ 136 ,4270 },
	{ 135 ,4258 },
	{ 135 ,4247 },
	{ 134 ,4236 },
	{ 135 ,4225 },
	{ 136 ,4214 },
	{ 137 ,4203 },
	{ 138 ,4192 },
	{ 139 ,4182 },
	{ 140 ,4171 },
	{ 141 ,4161 },
	{ 142 ,4150 },
	{ 142 ,4139 },
	{ 143 ,4129 },
	{ 144 ,4119 },
	{ 145 ,4108 },
	{ 146 ,4098 },
	{ 147 ,4088 },
	{ 149 ,4079 },
	{ 151 ,4071 },
	{ 158 ,4066 },
	{ 158 ,4058 },
	{ 155 ,4045 },
	{ 153 ,4029 },
	{ 153 ,4015 },
	{ 154 ,4003 },
	{ 156 ,3994 },
	{ 159 ,3988 },
	{ 163 ,3983 },
	{ 164 ,3977 },
	{ 164 ,3971 },
	{ 164 ,3963 },
	{ 164 ,3955 },
	{ 164 ,3946 },
	{ 162 ,3936 },
	{ 157 ,3925 },
	{ 149 ,3913 },
	{ 142 ,3901 },
	{ 137 ,3891 },
	{ 134 ,3883 },
	{ 132 ,3875 },
	{ 132 ,3869 },
	{ 131 ,3863 },
	{ 131 ,3857 },
	{ 131 ,3852 },
	{ 131 ,3847 },
	{ 131 ,3842 },
	{ 132 ,3838 },
	{ 133 ,3833 },
	{ 133 ,3829 },
	{ 133 ,3825 },
	{ 133 ,3821 },
	{ 134 ,3817 },
	{ 134 ,3813 },
	{ 135 ,3810 },
	{ 135 ,3807 },
	{ 136 ,3803 },
	{ 136 ,3800 },
	{ 138 ,3797 },
	{ 138 ,3795 },
	{ 138 ,3792 },
	{ 138 ,3789 },
	{ 139 ,3787 },
	{ 139 ,3784 },
	{ 139 ,3781 },
	{ 138 ,3778 },
	{ 136 ,3775 },
	{ 134 ,3771 },
	{ 133 ,3768 },
	{ 132 ,3765 },
	{ 132 ,3762 },
	{ 131 ,3758 },
	{ 131 ,3755 },
	{ 131 ,3751 },
	{ 131 ,3747 },
	{ 132 ,3745 },
	{ 133 ,3742 },
	{ 133 ,3738 },
	{ 133 ,3733 },
	{ 132 ,3727 },
	{ 132 ,3721 },
	{ 133 ,3716 },
	{ 135 ,3710 },
	{ 134 ,3703 },
	{ 132 ,3695 },
	{ 133 ,3693 },
	{ 135 ,3691 },
	{ 136 ,3690 },
	{ 139 ,3689 },
	{ 142 ,3688 },
	{ 147 ,3685 },
	{ 150 ,3678 },
	{ 147 ,3654 },
	{ 151 ,3612 },
	{ 156 ,3557 },
	{ 166 ,3484 },
	{ 189 ,3374 }
}


--->USB充电阶段
    do_chrdet_int_task（）充电器插入触发的中断，检测--->upmu_is_chr_det（）充电器端检测，充电器是否正常--->wakeup_fg_algo（） FG_CHARGING ,FG_RESUME
	
	唤醒电量计算，重置参数-->mt_battery_charger_detect_check()对充电器的检测-->mt_battery_type_detection()充电器类型的检测-->charging_get_charger_type
	
	通过宏传递函数指针，调用相应的函数，IC--->mt_usb_connect()如果充电器的类型是stand_host或者charging_host就连接USB--->mt_battery_update_status（）
	
	连接USB后就对电池当前的状态进行检测,通过一系列的设备节点wireless_main,ac_main,usb_main等节点写值这些上报的值进入power_supply子系统处理，
	
	power_supply_changed()改变节点的值--->回调函数。input=nl_data_handler--->bmd_ctrl_cmd_from_user,nl_send_to_user上层子系统接受到uevent事件，
	
	然后发送命令--->两层通过封装的数据结构（忘了skb？还是）这些数据都是Pmu_chargerstruct这个结构体封装完成从msg_fgd_cnd分离参数给pmu_hargerstruct赋值，
	
	一段很长的switch_case语句：init_flag电量计初始化是否完成，bat_is_chager_exist，baton_count三次循环检测=0，存在
	
	Icharging,battery_meter_get_charging_current获取电流以前是软件算法取平均，现在通过IC读取电量的值.....--->而这个上报的检测，现在通过一个healthd的进程监听
	
	pmic子系统的状态（监听是epoll不是socket）--->mainloop 一个while(1)循环检测，状态有变化上报状态update（）--->batterymonitor读取不同路径下各种电池属性的值
	
	--->batteryserivice java层，对整个android系统的电源进行管理



	座充充电阶段：
	USB充电阶段比座充阶段，应该多了USB枚举，识别的接口不一样，充电电流不一样

	简化：
	底层IC--》pmic驱动--》power_supply子系统监听底层的状态，统一不同IC上报的值，healthd进程监听batterystatus--》有uevent事件，update读取各各路径下电池参数的值

	--》binder机制--》batteryservice通过广播发布消息，需要的对电池做相应的操作	


}










->/*MTK，QC两个平台快充的对比*/
{

	
-->MTK平台快充方案MTK  pump express：
	
	软件控制流程：
	充电器检测-->根据BC1.2协议进行充电端口检测,是否是标准的交流充电器-->电量是否大于95%-->升高充电电压电流发送一定的指令”current pattern“-->
	检查MTK识别点的电压是否有响应，有响应就继续升压，如果不能就恢复充电电压到5V-->电量达到快满电的时候应该无法进行这样的通信了
	通过发送current pattern（特定的电流波形） 控制充电电压的up down来进行通信 VBUS端，所以当充电满的时候就无法通信，不能快充

	当看门狗检测到充电器输出的电流小于130mA，时间超过240ms后，充电电压就下降到5V


	软件流程：

	battery_pump_express_charger_check()快充充电器检测-->mtk_ta_reset_vchr（）充电电压恢复到5V-->mtk_ta_init（）充电器端初始化，判断标志位电压

	能否升到9V/12V-->mtk_ta_detector()对充电器进行判断-->mtk_ta_retry_increase（）调整VBUS上的电压进行升压判断-->charging_set_ta_current_pattern()

	调整电流时序也就是所谓的curent  pattern 指令信号-->调整三次，如果每次升压超过3V，累计三次表示成功，则判断是快充充电器。
	
		-->快充充电算法：
		代码定义的相关的宏：CONFIG_MTK_PUMP_EXPRESS_PLUS_SUPPORT

			{

		pwd  pump express  快充技术 好像switch charge 也是快充技术

		1.------->[1]MTK Pump Express Plus Introduction V01.pdf
		pump express plus 使用PSR架构
		pump express  使用P-charge架构
		是不是苹果出了个plus什么都有plus了。。。。。。

		adaptor 适配器，电源
		适配器

		SW快充的流程
		1.设置提高输出电压的模式
		2.检查b点电压是否是MTK平台的TA的标示
		3.设置升压模式到TA =9，设置本地化相应的参数，充电使能
		4.电压保持在5V的状态
		5.当电池接近充电电满的的时候，充电电流很小，充进去的电量很小，所以需要设置一个充电截止的标准，高于这个标准，反复插拔都不充电，低于的时候就充电

		通过对充电点的检查决定是用什么方式充电，检查应该是轮询的方式



		充电的硬件保护
		1.如果充电器意外的接到了高电压，看门狗会立刻启动应急预案将电压降低到5V
		2.温度监控等相关安全措施会保护充电安全


		使用pump express 快充技术还要相关的IC，修改一点配置等
		充电的过程开始也是要通信的，建立沟通，IC发指令控制电压电流的变化
			1.快充用更大的功率，降低更多的充电时间
			2.usb设备传输的电压是有限制的，唯一的办法是提高输出电量，电源管理IC端，提高电压输出能力。
			3.大的适配器输出电量可以获得大的充电电压，降低充电时间
			4.pump express充电技术跟switch charge充电技术好像可以兼容
			5.允许充电器根据电流决定充电所需的初始电压，由PMIC发出脉冲电流指令通过USB的Vbus传送给充电器，充电器依照这个指令调变输出电压，
			电压逐渐增加至高达5V 达到最大充电电流。
			

		2.------>[1]MTK Pump Express Plus Verify Guideline.pdf
		充电算法？这个文档好像都是些概述

		3.------>[1]MTK Pump Express Verify Guideline V1.0.pdf
		标题是说对快充的修改，
			1.对于TA电压的设置依赖于开路电压进行调整，TA？
			2.开路电压提高0.1V
			3.手机上电压的模式，没看明白哪个对哪个？
			4.伏安特性曲线，不同的量
			5.先进行识别，电压小于3.7v，迅速升高电压

		4.------>MTK Pump Express  20131212.pdf
		这篇文章，中文版的简介

		5.------>MTK Pump Express introduction and HW design guide_V1.1.pdf
		跟1.MTK Pump Express Plus Introduction V01.pdf很像


		6.------>MTK Pump Express Plus Introduction V01- English.pdf
		有一个充电IC，手机，适配器相关的原理图。整个内容跟前面的很相似
			1.操作的原则，多大电流范围哪可以操作，操作的流程，这是英文版的

		快充的一些细节还没讲？
	}	




-->高通平台快充方案QC3.0：

	一些概念：	
	{
		INOV：The POR Intelligent Negotiation for Optimum Voltage 最优电压记录智能协商方案
		Quick Charge 4采用高通技术公司独有的第三版最佳电压智慧协商(INOV)电源管理演算法。INOV现已包括首创的即时散热管理技术，
		将能够在既定的散热条件下，自主选择最佳的电力传输能力，从而优化充电效能。

		POR：the Plan of Record 记录计划   记忆学习

		PMI SMB：好像是高通平台手机端和充电适配器端的IC

		HVDCP：high voltage dedicated charger port高电压可检测的充电接口

		DCP：专用充电端口模式

		paraller  charging  同步充电

	}


	检测USB接口-->手机检测充电器的类型-->通过一定的协议确定主机充电的电流，可操作的点（这中间好像有一个握手协议）-->手机从发送要求给电源适配器所需要的充电电压-->电源适配器调整电压

	充电升压

	QC3.0提供从5v到20v每一步调节200mV的电压的挡位（充电电压跨度过大容易引起IC过热，损坏机器）
	QC3.0充电算法

	通过一系列的系统信号（还是前面的通信协议），决定给手机设备提供最合适的充电电压，同步充电配置



	高通的QC2.0快速充电需要手机端和充电器都要支持才行。充电的时候手机端的IC发送一定的命令时序，充电适配器端也有相应的时序回应，识别是否为专用的充电器

	当将充电器端通过数据线连到手机上时，充电器默认的是将D+和D-短接的，这样手机端探测充电器类型是DCP(参见本人另一篇博文《高通平台USB2.0和3.0接口充电器识别原理》)，
	手机以默认的5V电压充电，接着过程如下：

	1) 如果手机端使能了快速充电协议，Android用户空间的hvdcp(high voltage dedicated charger port)进程启动，并且在D+上加载0.325V的电压维持超过1.25s上；

	2) 充电器检测到D+上电压0.325V维持超过了1.25s，就断开D+和D-的短接，由于D+和D-断开，所以D-上的电压不再跟随D+上的电压0.325V变动，此时开始下降；

	3) 手机端检测到D-上的电压从0.325V开始下降维持1ms以上时，hvdcp读取/sys/class/power_supply/usb/voltage_max的值，如果是9000000mV（？有这么大吗），
	就设置D+上的电压为3.3V，D-上 的电压为0.6V，			否则设置D+为0.6V，D-为0V；

	4) 充电器检测到D+和D-上的电压后，就调整充电器输出电压，具体D+和D-上的电压和充电器输出电压对应如表1所示：
	表1
		D+			D-			output
		0.6V		0.6V		12V
		3.3V		0.6V		9V
		3.3V		3.3V		20V
		0.6V		GND			5V(default)

	QC3.0采用200mV步进的方式升压

	DP升压，DM降压，发送一条指令升降多少电压（QC3.0是200mV），应该就是通过USB的D+/D-电压判断，在进行选择适当的充电电压
	当电池负载较大时充电电压会升高，进行高电压充电，当电池负载下降时，充电电压会下降VBUS，但是这个根据负载功耗调整充电电压是有问题把？

	
	QC3.0充电协议




}


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






/**************************************************************************************
								充电相关的代码
**************************************************************************************/


->/*mtk*/
G1605A
{
	底层对硬件操作最好能和芯片手册对应这看

-->1.hw_charger_type_detection底层检测充电器类型
	这些bc11就是指BC1.1协议
	检测充电器类型根据BC1.1协议来的，给D+一个电压，检测D-上的电压，如果是低电平非标准充电；如果电压相同，标准充电；
	有一定的分压，专用的或其他类型的充电器。
	
	int hw_charger_type_detection(void)
	{
		CHARGER_TYPE charger_tye = CHARGER_UNKNOWN;

		/********* Step initial  ***************/
		hw_bc11_init();

		/********* Step DCD ***************/   
		//DCD：  数据载波检测
		if(1 == hw_bc11_DCD())
		{
			/********* Step A1 ***************/
			if(1 == hw_bc11_stepA1())
			{
				charger_tye = APPLE_2_1A_CHARGER;
				print("step A1 : Apple 2.1A CHARGER!\r\n");
			}
			else
			{
				charger_tye = NONSTANDARD_CHARGER;
				print("step A1 : Non STANDARD CHARGER!\r\n");
			}
		}
		else
		{
			/********* Step A2 ***************/
			if(1 == hw_bc11_stepA2())
			{
				/********* Step B2 ***************/
				if(1 == hw_bc11_stepB2())
				{
					charger_tye = STANDARD_CHARGER;
					print("step B2 : STANDARD CHARGER!\r\n");
				}
				else
				{
					charger_tye = CHARGING_HOST;
					print("step B2 :  Charging Host!\r\n");
				}
			}
			else
			{
				charger_tye = STANDARD_HOST;
				print("step A2 : Standard USB Host!\r\n");
			}
		}

		/********* Finally setting *******************************/
		hw_bc11_done();

		return charger_tye;
	}



-->检测充电器
	void do_chrdet_int_task(void)
	{
		if (g_bat_init_flag == KAL_TRUE) {
	#if !defined(CONFIG_MTK_DUAL_INPUT_CHARGER_SUPPORT)
			if (upmu_is_chr_det() == KAL_TRUE) {
	#else
			battery_charging_control(CHARGING_CMD_GET_DISO_STATE, &DISO_data);
			if ((DISO_data.diso_state.cur_vusb_state == DISO_ONLINE) ||
				(DISO_data.diso_state.cur_vdc_state == DISO_ONLINE)) {
	#endif
				battery_log(BAT_LOG_CRTI, "[do_chrdet_int_task] charger exist!\n");
				BMT_status.charger_exist = KAL_TRUE;

				wake_lock(&battery_suspend_lock);

	#if defined(CONFIG_POWER_EXT)
				mt_usb_connect();
				battery_log(BAT_LOG_CRTI,
						"[do_chrdet_int_task] call mt_usb_connect() in EVB\n");
	#elif defined(CONFIG_MTK_POWER_EXT_DETECT)
				if (KAL_TRUE == bat_is_ext_power()) {
					mt_usb_connect();
					battery_log(BAT_LOG_CRTI,
							"[do_chrdet_int_task] call mt_usb_connect() in EVB\n");
					return;
				}
	#endif
			} else {
				battery_log(BAT_LOG_CRTI, "[do_chrdet_int_task] charger NOT exist!\n");
				BMT_status.charger_exist = KAL_FALSE;

	#if defined(CONFIG_MTK_DUAL_INPUT_CHARGER_SUPPORT)
				battery_log(BAT_LOG_CRTI,
						"turn off charging for no available charging source\n");
				battery_charging_control(CHARGING_CMD_ENABLE, &BMT_status.charger_exist);
	#endif

	#ifdef CONFIG_MTK_KERNEL_POWER_OFF_CHARGING
				if (g_platform_boot_mode == KERNEL_POWER_OFF_CHARGING_BOOT
					|| g_platform_boot_mode == LOW_POWER_OFF_CHARGING_BOOT) {
					battery_log(BAT_LOG_CRTI,
							"[pmic_thread_kthread] Unplug Charger/USB In Kernel Power Off Charging Mode!  Shutdown OS!\r\n");
					battery_charging_control(CHARGING_CMD_SET_POWER_OFF, NULL);
					/* mt_power_off(); */
				}
	#endif

				wake_unlock(&battery_suspend_lock);

	#if defined(CONFIG_POWER_EXT)
				mt_usb_disconnect();
				battery_log(BAT_LOG_CRTI,
						"[do_chrdet_int_task] call mt_usb_disconnect() in EVB\n");
	#elif defined(CONFIG_MTK_POWER_EXT_DETECT)
				if (KAL_TRUE == bat_is_ext_power()) {
					mt_usb_disconnect();
					battery_log(BAT_LOG_CRTI,
							"[do_chrdet_int_task] call mt_usb_disconnect() in EVB\n");
					return;
				}
	#endif
	#if defined(CONFIG_MTK_PUMP_EXPRESS_SUPPORT) || defined(CONFIG_MTK_PUMP_EXPRESS_PLUS_SUPPORT)
				is_ta_connect = KAL_FALSE;
				ta_check_chr_type = KAL_TRUE;
				ta_cable_out_occur = KAL_TRUE;
	#endif

			}

			/* reset_parameter_dod_charger_plug_event(); */
			wakeup_fg_algo(FG_CHARGER);
			/* Place charger detection and battery update here is used to speed up charging icon display. */

			mt_battery_charger_detect_check();
			if (BMT_status.UI_SOC2 == 100 && BMT_status.charger_exist == KAL_TRUE) {
				BMT_status.bat_charging_state = CHR_BATFULL;
				BMT_status.bat_full = KAL_TRUE;
				g_charging_full_reset_bat_meter = KAL_TRUE;
			}

			if (g_battery_soc_ready == KAL_FALSE) {
				if (BMT_status.nPercent_ZCV == 0)
					battery_meter_initial();

				BMT_status.SOC = battery_meter_get_battery_percentage();
			}

			if (BMT_status.bat_vol > 0)
				mt_battery_update_status();

	#if defined(CONFIG_MTK_DUAL_INPUT_CHARGER_SUPPORT)
			DISO_data.chr_get_diso_state = KAL_TRUE;
	#endif

			wake_up_bat();
		} else {
	#if defined(CONFIG_MTK_DUAL_INPUT_CHARGER_SUPPORT)
			g_vcdt_irq_delay_flag = KAL_TRUE;
	#endif
			battery_log(BAT_LOG_CRTI,
					"[do_chrdet_int_task] battery thread not ready, will do after bettery init.\n");
		}

	}





	-->healthd进程负责监听底层上报的事件，uevent，periodic_chores负责将相应的事件上报给batterymonitor
		主循环
		static void healthd_mainloop(void) {
			while (1) {
				struct epoll_event events[eventct];
				int nevents;
				int timeout = awake_poll_interval;
				int mode_timeout;

				mode_timeout = healthd_mode_ops->preparetowait();
				if (timeout < 0 || (mode_timeout > 0 && mode_timeout < timeout))
					timeout = mode_timeout;
				nevents = epoll_wait(epollfd, events, eventct, timeout);

				if (nevents == -1) {
					if (errno == EINTR)
						continue;
					KLOG_ERROR(LOG_TAG, "healthd_mainloop: epoll_wait failed\n");
					break;
				}

				for (int n = 0; n < nevents; ++n) {
					if (events[n].data.ptr)
						//这段代码是强制类型转换，转换成相应类型的指针
						(*(void (*)(int))events[n].data.ptr)(events[n].events);//这个语句想干啥？
				}

				if (!nevents)
					periodic_chores();

				healthd_mode_ops->heartbeat();
			}

			return;
		}




	-->MTK不同充电算法的切换：
		void mt_battery_charging_algorithm(void)
		{
			battery_charging_control(CHARGING_CMD_RESET_WATCH_DOG_TIMER, NULL);

		#if defined(CONFIG_MTK_PUMP_EXPRESS_PLUS_SUPPORT)
		#if defined(PUMPEX_PLUS_RECHG)
			if (BMT_status.bat_in_recharging_state == KAL_TRUE && pep_det_rechg == KAL_TRUE)
				ta_check_chr_type = KAL_TRUE;
		#endif
			battery_pump_express_charger_check();
		#endif
			switch (BMT_status.bat_charging_state) {
			case CHR_PRE:
				BAT_PreChargeModeAction();
				break;

			case CHR_CC:
				BAT_ConstantCurrentModeAction();
				break;

			case CHR_BATFULL:
				BAT_BatteryFullAction();
				break;

			case CHR_HOLD:
				BAT_BatteryHoldAction();
				break;

			case CHR_ERROR:
				BAT_BatteryStatusFailAction();
				break;
			}

			battery_charging_control(CHARGING_CMD_DUMP_REGISTER, NULL);
		}


		static void battery_pump_express_charger_check(void)
		{
			if (KAL_TRUE == ta_check_chr_type &&
				STANDARD_CHARGER == BMT_status.charger_type &&
				BMT_status.SOC >= batt_cust_data.ta_start_battery_soc &&
				BMT_status.SOC < batt_cust_data.ta_stop_battery_soc) {
				battery_log(BAT_LOG_CRTI, "[PE+]Starting PE Adaptor detection\n");

				mutex_lock(&ta_mutex);
				wake_lock(&TA_charger_suspend_lock);

				mtk_ta_reset_vchr();

				mtk_ta_init();
				mtk_ta_detector();

				/* need to re-check if the charger plug out during ta detector */
				if (KAL_TRUE == ta_cable_out_occur)
					ta_check_chr_type = KAL_TRUE;
				else
					ta_check_chr_type = KAL_FALSE;
		#if defined(PUMPEX_PLUS_RECHG)
				/*PE detection disable in the event of recharge after 1st PE detection is finished */
				pep_det_rechg = KAL_FALSE;
		#endif
				wake_unlock(&TA_charger_suspend_lock);
				mutex_unlock(&ta_mutex);
			} else {
				battery_log(BAT_LOG_CRTI,
						"[PE+]Stop battery_pump_express_charger_check, SOC=%d, ta_check_chr_type = %d, charger_type = %d\n",
						BMT_status.SOC, ta_check_chr_type, BMT_status.charger_type);
			}
		}

		升压代码，current pattern ，通过VBUS上的一定规律的电流变化进行握手识别，通信
		static u32 charging_set_ta_current_pattern(void *data)
		{
			u32 increase = *(u32 *) (data);
			u32 charging_status = false;

		#if defined(HIGH_BATTERY_VOLTAGE_SUPPORT)
			u32 cv_voltage = BATTERY_VOLT_04_340000_V;
		#else
			u32 cv_voltage = BATTERY_VOLT_04_200000_V;
		#endif

			charging_get_charging_status(&charging_status);
			if (false == charging_status) {
				charging_set_cv_voltage(&cv_voltage);	/* Set CV */
				bq24196_set_ichg(0x0);	/* Set charging current 500ma */
				bq24196_set_chg_config(0x1);	/* Enable Charging */
			}

			if (increase == true) {
				bq24196_set_iinlim(0x0);	/* 100mA */
				msleep(85);

				bq24196_set_iinlim(0x2);	/* 500mA */
				pr_debug("mtk_ta_increase() on 1");
				msleep(85);

				bq24196_set_iinlim(0x0);	/* 100mA */
				pr_debug("mtk_ta_increase() off 1");
				msleep(85);

				bq24196_set_iinlim(0x2);	/* 500mA */
				pr_debug("mtk_ta_increase() on 2");
				msleep(85);

				bq24196_set_iinlim(0x0);	/* 100mA */
				pr_debug("mtk_ta_increase() off 2");
				msleep(85);

				bq24196_set_iinlim(0x2);	/* 500mA */
				pr_debug("mtk_ta_increase() on 3");
				msleep(281);

				bq24196_set_iinlim(0x0);	/* 100mA */
				pr_debug("mtk_ta_increase() off 3");
				msleep(85);

				bq24196_set_iinlim(0x2);	/* 500mA */
				pr_debug("mtk_ta_increase() on 4");
				msleep(281);

				bq24196_set_iinlim(0x0);	/* 100mA */
				pr_debug("mtk_ta_increase() off 4");
				msleep(85);

				bq24196_set_iinlim(0x2);	/* 500mA */
				pr_debug("mtk_ta_increase() on 5");
				msleep(281);

				bq24196_set_iinlim(0x0);	/* 100mA */
				pr_debug("mtk_ta_increase() off 5");
				msleep(85);

				bq24196_set_iinlim(0x2);	/* 500mA */
				pr_debug("mtk_ta_increase() on 6");
				msleep(485);

				bq24196_set_iinlim(0x0);	/* 100mA */
				pr_debug("mtk_ta_increase() off 6");
				msleep(50);

				pr_debug("mtk_ta_increase() end\n");

				bq24196_set_iinlim(0x2);	/* 500mA */
				msleep(200);
			} else {
				bq24196_set_iinlim(0x0);	/* 100mA */
				msleep(85);

				bq24196_set_iinlim(0x2);	/* 500mA */
				pr_debug("mtk_ta_decrease() on 1");
				msleep(281);

				bq24196_set_iinlim(0x0);	/* 100mA */
				pr_debug("mtk_ta_decrease() off 1");
				msleep(85);

				bq24196_set_iinlim(0x2);	/* 500mA */
				pr_debug("mtk_ta_decrease() on 2");
				msleep(281);

				bq24196_set_iinlim(0x0);	/* 100mA */
				pr_debug("mtk_ta_decrease() off 2");
				msleep(85);

				bq24196_set_iinlim(0x2);	/* 500mA */
				pr_debug("mtk_ta_decrease() on 3");
				msleep(281);

				bq24196_set_iinlim(0x0);	/* 100mA */
				pr_debug("mtk_ta_decrease() off 3");
				msleep(85);

				bq24196_set_iinlim(0x2);	/* 500mA */
				pr_debug("mtk_ta_decrease() on 4");
				msleep(85);

				bq24196_set_iinlim(0x0);	/* 100mA */
				pr_debug("mtk_ta_decrease() off 4");
				msleep(85);

				bq24196_set_iinlim(0x2);	/* 500mA */
				pr_debug("mtk_ta_decrease() on 5");
				msleep(85);

				bq24196_set_iinlim(0x0);	/* 100mA */
				pr_debug("mtk_ta_decrease() off 5");
				msleep(85);

				bq24196_set_iinlim(0x2);	/* 500mA */
				pr_debug("mtk_ta_decrease() on 6");
				msleep(485);

				bq24196_set_iinlim(0x0);	/* 100mA */
				pr_debug("mtk_ta_decrease() off 6");
				msleep(50);

				pr_debug("mtk_ta_decrease() end\n");

				bq24196_set_iinlim(0x2);	/* 500mA */
			}

			return STATUS_OK;
		}



		//控制充电使能
		static void pchr_turn_on_charging(void)
		{
		#if !defined(CONFIG_MTK_JEITA_STANDARD_SUPPORT)
			BATTERY_VOLTAGE_ENUM cv_voltage;
		#endif
			unsigned int charging_enable = KAL_TRUE;

		#if defined(CONFIG_MTK_DUAL_INPUT_CHARGER_SUPPORT)
			if (KAL_TRUE == BMT_status.charger_exist)
				charging_enable = KAL_TRUE;
			else
				charging_enable = KAL_FALSE;
		#endif

			if (BMT_status.bat_charging_state == CHR_ERROR) {
				battery_log(BAT_LOG_CRTI, "[BATTERY] Charger Error, turn OFF charging !\n");

				charging_enable = KAL_FALSE;

			} else if ((g_platform_boot_mode == META_BOOT) || (g_platform_boot_mode == ADVMETA_BOOT)) {
				battery_log(BAT_LOG_CRTI,
						"[BATTERY] In meta or advanced meta mode, disable charging.\n");
				charging_enable = KAL_FALSE;
			} else {
				/*HW initialization */
				battery_charging_control(CHARGING_CMD_INIT, NULL);

				battery_log(BAT_LOG_FULL, "charging_hw_init\n");

		#if defined(CONFIG_MTK_PUMP_EXPRESS_PLUS_SUPPORT)
				battery_pump_express_algorithm_start();
		#endif

				/* Set Charging Current */
				if (get_usb_current_unlimited()) {
					if (batt_cust_data.ac_charger_input_current != 0)
						g_temp_input_CC_value = batt_cust_data.ac_charger_input_current;
					else
						g_temp_input_CC_value = batt_cust_data.ac_charger_current;

					g_temp_CC_value = batt_cust_data.ac_charger_current;
					battery_log(BAT_LOG_FULL,
							"USB_CURRENT_UNLIMITED, use batt_cust_data.ac_charger_current\n");
				//GioneeDrv GuoJianqiu 20151223 modify for platform change begin
				}
		#ifndef CONFIG_MTK_SWITCH_INPUT_OUTPUT_CURRENT_SUPPORT
				else if (g_bcct_flag == 1) {
					select_charging_current_bcct();

					battery_log(BAT_LOG_CRTI, "[BATTERY] select_charging_current_bcct !\n");
				} else {
					select_charging_current();

					battery_log(BAT_LOG_CRTI, "[BATTERY] select_charging_current !\n");
				}
		#else
				else if (g_bcct_flag == 1 || g_bcct_input_flag == 1) {
				//GioneeDrv GuoJianqiu 20151223 modify for platform change end
					select_charging_current();
					select_charging_current_bcct();
					battery_log(BAT_LOG_FULL, "[BATTERY] select_charging_curret_bcct !\n");
				} else {
					select_charging_current();
					battery_log(BAT_LOG_FULL, "[BATTERY] select_charging_curret !\n");
				}
		#endif
				battery_log(BAT_LOG_CRTI,
						"[BATTERY] Default CC mode charging : %d, input current = %d\r\n",
						g_temp_CC_value, g_temp_input_CC_value);
				if (g_temp_CC_value == CHARGE_CURRENT_0_00_MA
					|| g_temp_input_CC_value == CHARGE_CURRENT_0_00_MA) {

					charging_enable = KAL_FALSE;

					battery_log(BAT_LOG_CRTI,
							"[BATTERY] charging current is set 0mA, turn off charging !\r\n");
				} else {
					battery_charging_control(CHARGING_CMD_SET_INPUT_CURRENT,
								&g_temp_input_CC_value);
					battery_charging_control(CHARGING_CMD_SET_CURRENT, &g_temp_CC_value);

					/*Set CV Voltage */
		#if !defined(CONFIG_MTK_JEITA_STANDARD_SUPPORT)
					//GioneeDrv GuoJianqiu 20160429 modify for platform change begin
					if (batt_cust_data.high_battery_voltage_support)
						cv_voltage = BATTERY_VOLT_04_400000_V;
					else
						cv_voltage = BATTERY_VOLT_04_200000_V;

					#ifdef CONFIG_MTK_DYNAMIC_BAT_CV_SUPPORT
					//GioneeDrv GuoJianqiu 20160429 modify for platform change end
					cv_voltage = get_constant_voltage() * 1000;
					battery_log(BAT_LOG_CRTI, "[BATTERY][BIF] Setting CV to %d\n", cv_voltage / 1000);
					#endif
					battery_charging_control(CHARGING_CMD_SET_CV_VOLTAGE, &cv_voltage);

					#if defined(CONFIG_MTK_HAFG_20)
					g_cv_voltage = cv_voltage;
					#endif
		#endif
				}
			}

			/* enable/disable charging */
			battery_charging_control(CHARGING_CMD_ENABLE, &charging_enable);

			battery_log(BAT_LOG_CRTI, "[BATTERY] pchr_turn_on_charging(), enable =%d !\r\n",	//Gionee GuoJianqiu 20150318 modify for fix bugs
					charging_enable);
		}


}




fuelgauge
{
FAQ09221	
	1. 开机过程中,由于电流越来越大,开机过程中电流跳变很大,导致系统读到的电压偏差很大(即使通过目前MT6573/MT6575/MT6577平台的算法也无法正确补偿) ,目前发现偏差有概率超过40%的(偏低)
	---MT6589导入HW ZCV在PMU 尚未放開resetb时系統极轻载下量測電池電壓 ,取代SW 补偿IR drop方式所得到之ZCV
	
	2.开关机或者拔插电池 再开机,电量显示百分比偏差较多,影响用户体验.
	---MT6589將百分比存放在RTC memory, 開機會去判斷是否為更換電池, 若判斷不是的話會依照上次存在RTC memory 之百分比顯示, 避免重開機或同一顆電池插拔百分比不同之問題
	
	3.MT6589放电时的15%校验偏差10%以上(很固定的偏低) ,比如实际电量25%,显示也25%,但触发15%校验而导致显示由25%变为15%.
	---MT6589將15% 校整改為客製化方式校整, 0~100% 皆可以填寫, 若填20%, 即會在20%時做FG百分比校整, 若填0%, 即代表取消此校整機制  
	
	4.电池老化容量與原廠新電池容量減少之誤差
	---由手機使用至15%以下後, 將手機充電至100% , 完整的充電過程,FG ADC記錄電池實際容量後, 即可將此誤差校整回來
	
	5.15% 校整客製化更改方式，客製化更改方法
	---在custom_fuel_gauge.h 檔案最後面加上以下指令, 紅色粗體數值0~100 皆可以填寫, 此範例為更該為20% tracking
	#define CHANGE_TRACKING_POINT
	#define CUST_TRACKING_POINT 20
	Note: default 15% tracking 無需新增以上指令
	
	6.开机过程中,由于电流越来越大,开机过程中电流跳变很大,导致系统读到的电压偏差很大(即使通过目前77平台的算法 也无法正确补偿) ,目前发现偏差有概率超过40%的(偏低)
	---MT6572導入HW ZCV在PMU 尚未放開resetb時系統極輕載下量測電池電壓 ,取代SW 補償IR drop方式所得到之ZCV
	
	7.开关机或者拔插电池 再开机,电量显示百分比偏差较多,影响用户体验.
	---MT6572及之后平台有將百分比存放在RTC memory, 開機會去判斷是否為更換電池, 若判斷不是的話會依照上次存在RTC memory 之百分比顯示, 避免重開機或同一顆電池插拔百分比不同之問題
	
	8.77平台放电时的15%校验偏差10%以上(很固定的偏低) ,比如实际电量25%,显示也25%,但触发15%校验而导致显示由25%变为15%.
	---MT6572無15% 校整, 因此無此問題存在
	
	9.MT6572电池连接器的接触阻抗與PCB BAT trace导致電阻内阻增加，需要补偿?
	---Yes, 補償可讓Novel Adaptive Gauge準確度可再提升1~3%左右
	需要提eService release patch
	補償方法: 客製化檔案cust_battery_meter.h
	•#define FG_METER_RESISTANCE   0(補償數值)
	•Example:电池连接器+ PCB BAT trace 寄生阻抗約
	30+5=35m Ω
	將補償數值填寫35, 如下所示
	#define FG_METER_RESISTANCE   35


FAQ10137
	DCC路径有什么电量计的文档可以参考？

	Document:
	1.Fuel Gauge Application Notes_V1.0
	2.Fuel Gauge introduce
	3.Mediatek HW Fuel Gauge User Guide
	4.Mediatek SW Fuel Gauge User Guide
	5.Customer_Training_Battery_Charing


FAQ05616
	首先应该熟悉两个变量，bat_volt_check_point是UI上面显示的值。
	gFG_capacity_by_C,库仑计计算的电量值.也等于SOC的值.
	公式 DOD1 = DOD0 + (-Car/Qmax).
	DOD1对应当前的电量值.
	DOD0对应初始的电量值.
	Car 为t时间内, 流过Rfg电阻电流的电量.
	Qmax为电池的容量.
	下面几种情况会更新DOD0的值，
	1.开机头10S.
	2.插拔USB.
	3.电池充满的状态
	4.电池容量为15%和0%的情况

	正常情况下如果，库仑计获取的初始电量DOD0的值比较准确，
	那么，gFG_capacity_by_c的值会很准，
	实际上，gFG_capacity_by_c跟实际的电量会有点小偏差。不过该值
	会在充电过程中与实际电量同步起来。譬如充电过程中是在100%
	不充电过程中是在15%和0%
	
	[PLATFORM]
	MT6575 MT6515 MT6577 MT6517  MT6589
	
	
	请结合
	alps\mediatek\platform\mt6589(mt6575/mt6577)\kernel\driver\power\
	mt6320_battery.c
	mt6575_battery.c
	mt6577_battery.c
	中mt6575_battery_update或是mt6320_battery_update来理解.

	采用Fuel gauge方案。
	1.充电情况
	A.充电到99%，可能需要等上一段比较长的时间才能到100%.
	----->采用FG的计算电量的变量gFG_capacity_by_C先到达100，而实际上，电池还没有充满，此时会
	先将UI显示即Bat_Volt_Check_point定格在99.直到电池满足充满条件时，才会让Bat_Volt_Check_point
	值变成100，并在UI上面显示100，此时底层的值为gFG_capacity_by_C以及Bat_Volt_check_point为100.
	上层会show满的图片，并停止充电。
	电池充满的条件是，进入top_off mode,并且充电电流小于200mA，这个值具体看软件的定义。

	B.充电从90%(可能是其他值)到100%需要的时间比较长
	----->Battery充电进入硬件的CV阶段，此时充电电流减小。对于库仑计的方式来计算电量，
	DOD1 = DOD0 + Car/Qmax，Car = I*t，每增加一%，如果电流恒定，那么电量增加的时间是相等，在CC阶段，电流比较大，每增加1%的时间，
	所需要的时间比较短，在CV阶段，电流减小，每增加1%的时间就会变长。
	当battery 充满即满足charging full的条件。此时电量会每10S，Bat_Volt_check_point增加1%，在UI上面显示也会相应每10S增加1%一直到100%.
	假设FG电量的方式gFG_capacity_by_c为96,电池满足充满的条件，此时，
	Bat_volt_check_point(即UI显示)会每10S增加1，当Bat_volt_check_point为100时，
	此时会调用FGADC_SW_RESET_parameter,直接将DOD1变成0，也即是
	gFG_capacity_by_c变成100.
	
	C.Recharging
	----->电池充满后，会停止充电，此时系统由电池来供电，当电量低于4.11时，此时
	又会重新充电，但是，UI上面会一直显示100%，假设这个时候拔掉USB，这个时候
	电量会从100%每1min down 1%去syc FG的电量。
	最差的情况，如果FG的电量变成96%，那么4min后，UI显示就变成96.
	

	2.不充电情况
	A.出现在20或者其他值，很快下降到15%，或者是在15%处停留时间比较久。
	---->
	这是因为15%是同步点，当电池真实电量成为15%，FG计算电量方式会同步到15%.
	
	B.出现在8%或者其他值，然后很快掉到1%.
	---->
	当实际的电量达到0%，也即电池电压的值小于3.4V，此时，UI上面会每10S，下降1直到
	0%，而FG的电量也会每10S，update一次，直到0.
	
	C.1%停留的时间比较久.
	---->
	当FG的计算方式先达到0%,那么UI上面会保持为1%，直到真实的电池容量变为0，也即是
	电池电压小于3.4V
	
	以上的情况都是软件的正常处理流程.初始的电量DOD0获取正确, 电量就会准确.
	而当有点小偏差，那么就会出现上述的一些情况.
 
	

FAQ02506
	(以下简述以MT6575+Android2.3为例)
	1、Fuel Gauge 算法需要相关软、硬件支持，
			硬件方面：正常的电池温度检测电路，正常的Fuel Gauge 电流检测电路

			软件方面：开启FG功能（默认情况下已打开），由检测客户提供的样机及标配电池而得出的若干个FG数组（送检请与CPM联系）

	2、Fuel Gauge 算法相关文件

			mediatek\platform\mt65xx\kernel\drivers\power\mt65xx_fuel_gauge.c

			mediatek\custom\[PROJECT]\kernel\battery\battery\Custom_fuel_gauge.h

			Custom_fuel_gauge.h 中的数组由检测样机及标配电池后生成。

	3、Fuel Gauge 算法相关介绍资料

			Mediatek MT65xx Fuel Gauge      ；该资料可向CPM申请。

	4、如何关闭 Fuel Gauge

			请参考相关FAQ



FAQ02469
	[DESCRIPTION]

	从spare parts/battery information查看充电时电池电量，从99%到100%需要较长时间，如何改善？

	[PLATFORM]

	MT6573 MT6513 MT6575 MT6515

	[SOLUTION]
	在完成fuel gauge及battery 克制化后，首先需要说明，这是正常现象，若您为缩短从99%到100%的用戶体验时间，请在mediatek/platform/mt657*/kernel/drivers/power/mt657*_battery.c文件中，

	mt6573_battery_update这支API中，mark掉下面整段code：

	if(bat_volt_check_point >= 100 )
	{     
	bat_volt_check_point=99;
	//BMT_status.SOC=99;
	gSyncPercentage=1;

		//if (Enable_BATDRV_LOG == 1) {
	printk("[Battery] Use gas gauge : gas gague get 100 first (%d)\r\n", bat_volt_check_point);
		//}
	}

	else{   ..................
				
	}

	note: 该改动完成后UI show 100% , charging full 沒有真正完成（driver还在进行小电流充电）

	For more information, please contact us, thanks.		


}



17G05A
{
		rt5081_dump_register
		{
		
			ichg		ret = rt5081_get_ichg(chg_dev, &ichg);--->rt5081_find_closest_real_value(RT5081_ICHG_MIN, RT5081_ICHG_MAX,RT5081_ICHG_STEP, reg_ichg);
			reg_ichg是寄存器上的值，经过ret_val = min + reg_val * step得到最接近真实的值
			
			aicr		ret = rt5081_get_aicr(chg_dev, &aicr);
			ieoc		ret = rt5081_get_ieoc(chg_data, &ieoc);
			mivr		ret = rt5081_get_mivr(chg_data, &mivr);
		
			chg_status	ret = rt5081_get_charging_status(chg_data, &chg_status);
			获取充电状态
			cv			ret = rt5081_get_cv(chg_dev, &cv);
			恒压充电的电压
			chg_en		ret = rt5081_is_charging_enable(chg_data, &chg_en);
			充电使能
			vsys		ret = rt5081_get_adc(chg_data, RT5081_ADC_VSYS, &adc_vsys);
			系统的负载电压
			vbat		ret = rt5081_get_adc(chg_data, RT5081_ADC_VBAT, &adc_vbat);
			电池的开路电压
			ibat		ret = rt5081_get_adc(chg_data, RT5081_ADC_IBAT, &adc_ibat);
			进入电池的电流
			ibus		ret = rt5081_get_adc(chg_data, RT5081_ADC_IBUS, &adc_ibus);
			线上的充电电流
		}
		
		
		mtk_switch_charging.c定义的充电状态机的各各函数
		（根据从dtsi文件传过来的配置选用不同的充电算法），最后很多的ops都是调用到了rt5081上
		{
			mtk_switch_charging.c调用不同的算法
			info->algorithm_data = swch_alg;
			info->do_algorithm = mtk_switch_charging_run;
			info->plug_in = mtk_switch_charging_plug_in;
			info->plug_out = mtk_switch_charging_plug_out;
			info->do_charging = mtk_switch_charging_do_charging;
			info->do_event = charger_dev_event;
			info->change_current_setting = mtk_switch_charging_current;
			
			mtk_switch_charging_init	初始化充电调用相关的函数指针
		-->	
			mtk_switch_charging_run
		
			swchg_turn_on_charging 		
			充电使能检测swchgalg->state充电器的状态，启动pe20算法mtk_pe20_start_algorithm，设置充电电流swchg_select_charging_current_limit
			swchg_select_cv设定恒压充电的电压，首先是动态获取电压，然后设定充电电压
			
		}



}









-->/*qc*/

{
	   	pmi8950_charger: qcom,qpnp-smbcharger {
		spmi-dev-container;
		compatible = "qcom,qpnp-smbcharger";
		#address-cells = <1>;
		#size-cells = <1>;

		qcom,iterm-ma = <100>;		//充电截止时的电流
		//Gionee <GN_BSP_CHG> <liujiang> <20161025> modify for CR01748202 begin
		qcom,float-voltage-mv = <4400>;
		//Gionee <GN_BSP_CHG> <liujiang> <20161025> modify for CR01748202 end
		qcom,resume-delta-mv = <200>;
		qcom,chg-inhibit-fg;
		qcom,rparasitic-uohm = <100000>;
		qcom,bms-psy-name = "bms";
		//Gionee <GN_BSP_CHG> <liujiang> <2016-11-28> add for 35113 begin
		qcom,thermal-mitigation = <3000 2500 2000 1500 1000 0>;
		//Gionee <GN_BSP_CHG> <liujiang> <2016-11-28> add for 35113 end
		//Gionee <GN_BSP_CHG> <liujiang> <20161025> modify for CR01748202 begin
		qcom,parallel-usb-min-current-ma = <2000>; //<1400>;
		//Gionee <GN_BSP_CHG> <liujiang> <20161025> modify for CR01748202 end
		qcom,parallel-usb-9v-min-current-ma = <900>;
		qcom,parallel-allowed-lowering-ma = <500>;
		qcom,autoadjust-vfloat;
		qcom,pmic-revid = <&pmi8950_revid>;
		qcom,charge-unknown-battery;
		qcom,force-aicl-rerun;
		qcom,aicl-rerun-period-s = <180>;
		//Gionee <GN_BSP_CHG> <liujiang> <20161025> add for CR01764517 begin
		qcom,usbin-vadc = <&pmi8950_vadc>;
		qcom,usbin-adc-channel-id = <0>;
		//Gionee <GN_BSP_CHG> <liujiang> <20161025> add for CR01764517 end
		//Gionee BSP wuxh modify for chg led control by triple ic in kernel on 20161009 begin
		qcom,chg-led-support;
		qcom,chg-led-sw-controls;
		//Gionee BSP wuxh modify for chg led control by triple ic in kernel on 20161009 end
		qcom,chgr@1000 {
			reg = <0x1000 0x100>;
			interrupts =	<0x2 0x10 0x0>,
					<0x2 0x10 0x1>,
					<0x2 0x10 0x2>,
					<0x2 0x10 0x3>,
					<0x2 0x10 0x4>,
					<0x2 0x10 0x5>,
					<0x2 0x10 0x6>,
					<0x2 0x10 0x7>;

			interrupt-names =	"chg-error",
						"chg-inhibit",
						"chg-prechg-sft",
						"chg-complete-chg-sft",
						"chg-p2f-thr",
						"chg-rechg-thr",
						"chg-taper-thr",
						"chg-tcc-thr";
		};

		qcom,otg@1100 {
			reg = <0x1100 0x100>;
			interrupts =	<0x2 0x11 0x0>,
					<0x2 0x11 0x1>,
					<0x2 0x11 0x3>;
			interrupt-names =	"otg-fail",
						"otg-oc",
					"usbid-change";
		};

		qcom,bat-if@1200 {
			reg = <0x1200 0x100>;
			interrupts =	<0x2 0x12 0x0>,
					<0x2 0x12 0x1>,
					<0x2 0x12 0x2>,
					<0x2 0x12 0x3>,
				<0x2 0x12 0x4>,
					<0x2 0x12 0x5>,
					<0x2 0x12 0x6>,
					<0x2 0x12 0x7>;

			interrupt-names =	"batt-hot",
						"batt-warm",
						"batt-cold",
						"batt-cool",
					"batt-ov",
						"batt-low",
						"batt-missing",
						"batt-term-missing";
		};

		qcom,usb-chgpth@1300 {
			reg = <0x1300 0x100>;
			interrupts =	<0x2 0x13 0x0>,
					<0x2 0x13 0x1>,
				<0x2 0x13 0x2>,
					<0x2 0x13 0x5>;

			interrupt-names =	"usbin-uv",
					"usbin-ov",
						"usbin-src-det",
						"aicl-done";
		};

		qcom,dc-chgpth@1400 {
			reg = <0x1400 0x100>;
			interrupts =	<0x2 0x14 0x0>,
					<0x2 0x14 0x1>;
			interrupt-names =	"dcin-uv",
						"dcin-ov";
		};

		qcom,chgr-misc@1600 {
			reg = <0x1600 0x100>;
			interrupts =	<0x2 0x16 0x0>,
					<0x2 0x16 0x1>,
					<0x2 0x16 0x2>,
				<0x2 0x16 0x3>,
					<0x2 0x16 0x4>,
					<0x2 0x16 0x5>;

			interrupt-names =	"power-ok",
						"temp-shutdown",
						"wdog-timeout",
						"flash-fail",
						"otst2",
						"otst3";
		};
	};



	voters相关这个相关的函数，变量的作用

	struct client_vote {
		int	state;
		int	value;
	};


	struct votable {
		struct client_vote	votes[NUM_MAX_CLIENTS];
		struct device		*dev;
		const char		*name;
		int			num_clients;
		int			type;
		int			effective_client_id;
		int			effective_result;
		int			default_result;
		struct mutex		vote_lock;
		int			(*callback)(struct device *dev,
							int effective_result,
							int effective_client,
							int last_result,
							int last_client);
	};


	/* voters */
	struct votable			*fcc_votable;
	struct votable			*usb_icl_votable;
	struct votable			*dc_icl_votable;
	struct votable			*usb_suspend_votable;
	struct votable			*dc_suspend_votable;
	struct votable			*battchg_suspend_votable;






	struct votable *create_votable(struct device *dev, const char *name,
					int votable_type,
					int num_clients,
					int default_result,
					int (*callback)(struct device *dev,
							int effective_result,
							int effective_client,
							int last_result,
							int last_client)
					)
	{
		int i;
		struct votable *votable = devm_kzalloc(dev, sizeof(struct votable),
								GFP_KERNEL);

		if (!votable)
			return ERR_PTR(-ENOMEM);

		if (!callback) {
			dev_err(dev, "Invalid callback specified for voter\n");
			return ERR_PTR(-EINVAL);
		}

		if (votable_type >= NUM_VOTABLE_TYPES) {
			dev_err(dev, "Invalid votable_type specified for voter\n");
			return ERR_PTR(-EINVAL);
		}

		if (num_clients > NUM_MAX_CLIENTS) {
			dev_err(dev, "Invalid num_clients specified for voter\n");
			return ERR_PTR(-EINVAL);
		}

		votable->dev = dev;
		votable->name = name;
		votable->num_clients = num_clients;
		votable->callback = callback;
		votable->type = votable_type;
		votable->default_result = default_result;
		mutex_init(&votable->vote_lock);

		/*
		* Because effective_result and client states are invalid
		* before the first vote, initialize them to -EINVAL
		*/
		votable->effective_result = -EINVAL;
		votable->effective_client_id = -EINVAL;

		for (i = 0; i < votable->num_clients; i++)
			votable->votes[i].state = -EINVAL;

		return votable;



	}



	int vote(struct votable *votable, int client_id, bool state, int val)
	{
		int effective_id, effective_result;
		int rc = 0;

		lock_votable(votable);

		if (votable->votes[client_id].state == state &&
					votable->votes[client_id].value == val) {
			pr_debug("%s: votes unchanged; skipping\n", votable->name);
			goto out;
		}

		votable->votes[client_id].state = state;
		votable->votes[client_id].value = val;

		pr_debug("%s: %d voting for %d - %s\n",
				votable->name,
				client_id, val, state ? "on" : "off");
		switch (votable->type) {
		case VOTE_MIN:
			effective_id = vote_min(votable);
			break;
		case VOTE_MAX:
			effective_id = vote_max(votable);
			break;
		case VOTE_SET_ANY:
			votable->votes[client_id].value = state;
			effective_result = vote_set_any(votable);
			if (effective_result != votable->effective_result) {
				votable->effective_client_id = client_id;
				votable->effective_result = effective_result;
				rc = votable->callback(votable->dev,
							effective_result, client_id,
							state, client_id);
			}
			goto out;
		}

		/*
		* If the votable does not have any votes it will maintain the last
		* known effective_result and effective_client_id
		*/
		if (effective_id < 0) {
			pr_debug("%s: no votes; skipping callback\n", votable->name);
			goto out;
		}

		effective_result = votable->votes[effective_id].value;

		if (effective_result != votable->effective_result) {
			votable->effective_client_id = effective_id;
			votable->effective_result = effective_result;
			pr_debug("%s: effective vote is now %d voted by %d\n",
					votable->name, effective_result, effective_id);
			rc = votable->callback(votable->dev, effective_result,
						effective_id, val, client_id);
		}

	out:
		unlock_votable(votable);
		return rc;
	}

}
整理的各种资料文档






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
	
	
		linux  mount 的用法？
	
	
	
		功耗，降电流问题
		飞行模式下的电流是否异常；应用是否经常唤醒，唤醒源；隔离大的驱动模块，看各各部分耗电是否异常；gpio的配置问题，不工作时的suspend模式，
		gpio引脚是否被上拉导致一直在耗电，probe函数。suspend,resume
	
		android drone： android 无人机
		access point：桥接器，访问节点

}






->/*电源管理的整个框架(涉及的面很广)*/
{
-->基本概念
{
	hibernate：冬眠	hibernation image

}



-->linux时钟系统（这个不是不想看，有点困难）








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
	
	唤醒电量计算，重置参数--->mt_battery_charger_detect_check()对充电器的检测--->mt_battery_type_detection()充电器类型的检测--->charging_get_charger_type
	
	通过宏传递函数指针，调用相应的函数，IC--->mt_usb_connect()如果充电器的类型是stand_host或者charging_host就连接USB--->mt_battery_update_status（）连接USB后就对电池当前的状态进行检测,
	
	通过一系列的设备节点wireless_main,ac_main,usb_main等节点写值这些上报的值进入power_supply子系统处理，power_supply_changed()改变节点的值--->回调函数。input=nl_data_handler--->
	
	bmd_ctrl_cmd_from_user,nl_send_to_user上层子系统接受到uevent事件，然后发送命令--->两层通过封装的数据结构（忘了skb？还是）这些数据都是Pmu_chargerstruct这个结构体封装完成

	从msg_fgd_cnd分离参数给pmu_hargerstruct赋值，一段很长的switch_case语句：init_flag电量计初始化是否完成，bat_is_chager_exist，baton_count三次循环检测=0，存在

	Icharging,battery_meter_get_charging_current获取电流以前是软件算法取平均，现在通过IC读取电量的值.....--->而这个上报的检测，现在通过一个healthd的进程监听pmic子系统

	的状态（监听是epoll不是socket）--->mainloop 一个while(1)循环检测，状态有变化上报状态update（）--->batterymonitor读取不同路径下各种电池属性的值--->batteryserivice java

	层，对整个android系统的电源进行管理



	

	座充充电阶段：
	USB充电阶段比座充阶段，应该多了USB枚举，识别的接口不一样，充电电流不一样




	简化：
	底层IC--》pmic驱动--》power_supply子系统监听底层的状态，统一不同IC上报的值，healthd进程监听batterystatus--》有uevent事件，update读取各各路径下电池参数的值

	--》binder机制--》batteryservice通过广播发布消息，需要的对电池做相应的操作	
	
	
	{
		底层涉及三个方面：硬件IC(ADC)，fuel_gauge,pmic子系统

		中间层：power_supply子系统

		上层：batterymonitor，batteryservice


		底层硬件

		pmic子系统

		powe_supply子系统

		batterymonitor

		batteryservice

	}





--->battery_driver，battery_meter_driver 几个重要的驱动启动，工作过程



   

	2.power_supply子系统

	3.各种场景
		温升导致的降电流
		打电话导致的降电流
		关机充电
		OTG反向充电
		低电保护

    4.大的场景
	  标准充电
	  快速充电
	  无线充电
	  非标准充电
	  USB BC1.2协议


    5.移植G1605A充电代码



}













->/*type-C协议*/
{

-->基本概念
    type-C有24个引脚（下面几个是主要的还有Tx/Rx）
    {
      VBUS
	  CC1->
	  CC2->{
	    数据传输主要有TX/RX两组差分信号，CC1和CC2是两个关键引脚，作用很多：

		• 探测连接，区分正反面，区分DFP和UFP，也就是主从

		• 配置Vbus，有USB Type-C和USB Power Delivery两种模式

		• 配置Vconn，当线缆里有芯片的时候，一个cc传输信号，一个cc变成供电Vconn

		• 配置其他模式，如接音频配件时，dp，pcie时

		电源和地都有4个，这就是为什么可以支持到100W的原因。

	  }
	  D+
	  D-
	  GND
	  Tx/Rx 两组
	}

	type-c的几个模式：
		DFP(Downstream Facing Port): 下行端口，可以理解为Host，DFP提供VBUS，也可以提供数据。
		UFP(Upstream Facing Port): 上行端口，可以理解为Device，UFP从VBUS中取电，并可提供数据。
		DRP(Dual Role Port): 双角色端口，DRP既可以做DFP(Host)，也可以做UFP(Device)，也可以在DFP与UFP间动态切换


	CC（Configuration Channel）：配置通道，这是USB Type-C里新增的关键通道，它的作用有检测USB连接，检测正反插，USB设备间数据与VBUS的连接建立与管理等。

	USB PD(USB Power Delivery): PD是一种通信协议，它是一种新的电源和通讯连接方式，它允许USB设备间传输最高至100W（20V/5A）的功率，同时它可以改变端口的属性，
	也可以使端口在DFP与UFP之间切换，它还可以与电缆通信，获取电缆的属性。


	type-c方便正反插，传输速率高（信号会有干扰），供电能力强（USB pd）正反充电，核心的两个是CC引脚配置，检测和usb pd协议的支持


-->type-c协议，检测端口
		设备的连接和断开通过检测CC引脚上的电压，并管理VBUS，当设备没有插入时关闭VBUS
	DRP在待机模式下每50ms在DFP和UFP间切换一次。当切换至DFP时，CC管脚上必须有一个上拉至VBUS的电阻Rp或者输出一个电流源，当切换至UFP时，CC管脚上必须有一个下拉至GND
	的电阻Rd。此切换动作必须由CC Logic芯片来完成。当DFP检测到UFP插入之后才可以输出VBUS，当UFP拔出以后必须关闭VBUS。此动作必须由CC Logic芯片来完成。

	Vconn

}









/*BC1.2协议*/
{




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











->/*USB&&OTG*/
{
    
-->usb设备驱动相关

1.基本概念：
	1.endpoint：端点描述符？

	2.usb_gadget:？

	3.usb descriptor:usb设备描述符 

	4.DMA:？
	  QMU：？	

	5.scatter_gather:

	6.HID:人际交互设备，人际交互接口

	7.USB的集中传输协议：OHCI，UHCI，EHCI，xHCI
	
	8.fstab:通常情况下，一个Linux系统将拥有很多的文件系统，然而，仅仅通过一个或非常少的文件系统来配
	置Linux系统也是可能的，你希望创建多个文件系统的一个原因就是通过使用/etc/fstab文件中指定的mount
	选项来控制对它们的访问???

	# /etc/fstab: static file system information.
	#
	# Use 'blkid' to print the universally unique identifier for a
	# device; this may be used with UUID= as a more robust way to name devices
	# that works even if disks are added and removed. See fstab(5).
	#
	# <file system> <mount point>   <type>  <options>       <dump>  <pass>
	proc            /proc           proc    nodev,noexec,nosuid 0       0
	# / was on /dev/sda1 during installation
	UUID=c373f7b8-df79-4de2-a2dc-aa6053f02594 /               ext4    errors=remount-ro 0       1
	# swap was on /dev/sda5 during installation
	UUID=7127cb00-5843-443d-b479-bbf35ef89748 none            swap    sw              0       0

	9.GIC：通用中断子系统



2.usb_otg设备的处理过程和GPIO的配置

	1.设备树相关的配置(基本的设备树的配置)

	usb0:usb20@11200000 {				//USB20的基地址为11200000
		compatible = "mediatek,mt6735-usb20";	//通过设备树匹配的名称mt6735-usb20
		cell-index = <0>;			//子树有几个	
		reg = <0x11200000 0x10000>,		//寄存器的地址和范围	
		      <0x11210000 0x10000>;
		interrupts = <GIC_SPI 72 IRQ_TYPE_LEVEL_LOW>;//设置中断模式：中断类型，中断号，中断触发的类型
		mode = <2>;
		multipoint = <1>;
		num_eps = <16>;				//设备树添加的一些属性，共有16个端点描述符
		clocks = <&perisys PERI_USB0>;
		clock-names = "usb0";
		vusb33-supply = <&mt_pmic_vusb33_ldo_reg>;//这个是设置的什么意思？
		iddig_gpio = <0 1>;			//iddig_gpio = <46 1>;	 //寄存器的位置改了，			
		drvvbus_gpio = <83 2>;			//drvvbus_gpio = <60 2>;
	};

	/* USB GPIO Kernal Standardization start */
	/* GioneeDrv GuoJianqiu 20160425 modify for OTG function begin */
	&pio {
		usb_default: usb_default {
		};

		gpio46_mode1_iddig: iddig_irq_init {
			pins_cmd_dat {
				pins = <PINMUX_GPIO46__FUNC_IDDIG>;
				slew-rate = <0>;
				bias-pull-up = <00>;
			};
		};

		gpio60_mode2_drvvbus: drvvbus_init {
			pins_cmd_dat {
				pins = <PINMUX_GPIO60__FUNC_GPIO60>;
				slew-rate = <1>;
				bias-pull-down = <00>;
			};
		};

		gpio60_mode2_drvvbus_low: drvvbus_low {
			pins_cmd_dat {
				pins = <PINMUX_GPIO60__FUNC_GPIO60>;
				slew-rate = <1>;
				output-low;
				bias-pull-down = <00>;
			};
		};

		gpio60_mode2_drvvbus_high: drvvbus_high {
			pins_cmd_dat {
				pins = <PINMUX_GPIO60__FUNC_GPIO60>;
				slew-rate = <1>;
				output-high;
				bias-pull-down = <00>;
			};
		};
	};

	&usb0 {
		iddig_gpio = <46 1>;
		pinctrl-names = "usb_default", "iddig_irq_init", "drvvbus_init", "drvvbus_low", "drvvbus_high";
		pinctrl-0 = <&usb_default>;
		pinctrl-1 = <&gpio46_mode1_iddig>;
		pinctrl-2 = <&gpio60_mode2_drvvbus>;
		pinctrl-3 = <&gpio60_mode2_drvvbus_low>;
		pinctrl-4 = <&gpio60_mode2_drvvbus_high>;
		status = "okay";
	};

	
	
	3.开启OTG功能相关的配置
		a.编译
		/home/llb/project/PRO/source/G1605A/L28_6737M_65_G1605A_M0.MP1_V2.140.3_160908_ALPS/android_mtk_6737m_65_mp/kernel-3.18/arch/arm/configs
		gnbj6737t_xx_m0_defconfig文件 
		打开相关的宏
		//打开OTG功能，硬件使能
		CONFIG_USB_MTK_OTG=y
		CONFIG_USB_MTK_HDRC_HCD=y
		//人际交互使能
		CONFIG_USB_HID=y
		//USB大容量存储使能
		CONFIG_USB_STORAGE=y
		//支持一些接口
		CONFIG_SCSI=y
		CONFIG_USB_ACM=y
		
		//这些宏要打开吗？但是编译文档上没有
		CONFIG_USB_MU3D_DRV=y
		CONFIG_MTK_SIB_USB_SWITCH=y
		CONFIG_USB_MU3D_ONLY_U2_MODE=y
		CONFIG_USB_XHCI_MTK=y
		CONFIG_USB_MTK_DUALMODE=y
		CONFIG_USB_XHCI_HCD=y

		文档上打开的是另一些宏


	4.相关的函数
	void mt_usb_otg_init(struct musb *musb)


-->OTG功能的检测过程:

    该设备支持OTG，下面说下设备的发现过程：

    作为从设备插入PC端口时：

    1.  系统检测到VBUS上的XEINT28上升沿触发中断，因为PC端会有一个5V从VBUS给过来，进入中断处理函数进一步确认ID脚状态，ID脚为低则状态错误，ID脚为高表示设备应该切换到从设备模式

    2. 通知usb gadget使能vbus，按照device模式使能PHY。gadget在probe时注册了一个SPI软中断IRQ_USB_HSOTG，用于响应数据接收

    3. 开启usb clk，使能PHY，此时外部5V电源供给系统XuotgVBUS，gadget收到IRQ_USB_HSOTG中断要求重启OTG core

    4. USB DP（高速设备为DP，低速设备为DM）上产生一个高电平脉冲，此时PC识别到一个USB设备插入，windows会提示用户

    5. 后续就是SETUP，GET DISCRIPTOR的过程

    
    作为主设备发现设备插入时：

    1. 系统检测到ID脚上XEINT29下降沿触发中断（实际是插入的usb公口第四脚直接连接到第五脚地上面），进入中断处理，切换到主设备模式

    2. 关中断，使能DC5V给VBUS上电，唤醒ehci与ohci

    3. usb core在内核初始化时注册了一个名为khubd的内核线程，由khubd监控port event。（实际过程我理解是从设别由VUBS供电后，会在DP或DM上产生一个高电平脉冲

    ehci在接收到脉冲信号后识别到设备插入，仅仅是理解，这一点未验证）

    3. khubd获取port，speed后交给ehci，接下来就是usb的SETUP，GET DISCRIPTOR过程



/* boot type definitions 几个常见的启动模式*/
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





	开OTG使能	
	[ 1778.094714] <1>.(1)[3202:Binder_10][name:battery_common_fg_20&]show_otg_charge_switch_State = 0
	[ 1778.202733] <1>.(1)[1647:Binder_1][name:usb20_host&]****before gn_Open_Otg_Irq!
	[ 1778.312694] <1>.(1)[1647:Binder_1][name:usb20_host&][MUSB]gn_Open_Otg_Irq 751: iddig_state = 1
	[ 1778.352742] <1>.(1)[1647:Binder_1][name:usb20_host&][MUSB]gn_Open_Otg_Irq 759: gn_Open_Otg_Irq is done
	[ 1778.353909] <1>.(1)[1647:Binder_1][name:battery_common_fg_20&]store_otg_charge_switch_State = 1


	[ 1782.082073] <0>-(0)[0:swapper/0][name:usb20_host&][MUSB]mt_usb_ext_iddig_int 500: id pin interrupt assert
	[ 1782.472966] <0>.(0)[8137:kworker/0:0][name:usb20_host&][MUSB]musb_id_pin_work 412: work start, is_host=0, boot mode(0)
	[ 1782.474333] <0>.(0)[8137:kworker/0:0][name:usb20_host&][MUSB]musb_is_host 271: will mask PMIC charger detection
	[ 1782.475602] <0>.(0)[8137:kworker/0:0][name:usb20&][MUSB]mt_usb_enable 292: <0,0>,<4,4,3,3>
	[ 1782.476687] <0>.(0)[8137:kworker/0:0][name:usb20&][MUSB]mt_usb_enable 313: enable UPLL before connect
	[ 1782.477830] <0>.(0)[8137:kworker/0:0][name:usb20&][MUSB]vcore_hold 154: before releasing
	[ 1782.478838] <0>.(0)[8137:kworker/0:0][name:usb20&][MUSB]vcore_hold 157: after releasing
	[ 1782.487633] <0>.(0)[8137:kworker/0:0][name:usb20&][MUSB]vcore_hold 163: hold VCORE ok
	[ 1782.501709] <0>.(0)[8137:kworker/0:0][name:usb20_phy&][MUSB]HQA_special 22: HQA, 0x18, before:86
	[ 1782.502961] <0>.(0)[8137:kworker/0:0][name:usb20_phy&][MUSB]HQA_special 26: HQA, 0x18, after:86
	[ 1782.504054] <0>.(0)[8137:kworker/0:0][name:usb20_phy&][MUSB]hs_slew_rate_cal 271: [USBPHY]slew calibration:FM_OUT =326,x=4221,value=4
	[ 1782.505550] <0>.(0)[8137:kworker/0:0][name:usb20_phy&][MUSB]usb_phy_recover 644: usb recovery success
	[ 1782.506695] <0>.(0)[8137:kworker/0:0][name:usb20&][MUSB]mt_usb_enable 328: <4,4,4,3>

	[ 1782.507664] <0>.(0)[8137:kworker/0:0][name:usb20_host&][MUSB]musb_is_host 289: iddig_state = 0
	[ 1782.508733] <0>.(0)[8137:kworker/0:0][name:usb20_host&][MUSB]musb_is_host 326: usb_is_host = 1
	[ 1782.509807] <0>.(0)[8137:kworker/0:0][name:usb20_host&][MUSB]musb_id_pin_work 425: musb is as host
	[ 1782.511989] <0>.(0)[8137:kworker/0:0][name:usb20_host&][MUSB]mt_usb_set_vbus 91: mt65xx_usb20_vbus++,is_on=1
	[ 1782.625000] <0>.(0)[8137:kworker/0:0][name:usb20_host&][MUSB]musb_id_pin_work 446: force PHY to idle, 0x6d=3f, 0x6c=11
	[ 1782.631378] <0>.(0)[8137:kworker/0:0][name:usb20_host&][MUSB]musb_id_pin_work 457: force PHY to host mode, 0x6d=3f, 0x6c=2d
	[ 1782.633531] <0>.(0)[8137:kworker/0:0][name:musb_hdrc&][MUSB]musb_start 1221: start, is_host=1 is_active=0
	[ 1782.634807] <0>.(0)[8137:kworker/0:0][name:usb20&][MUSB]mt_usb_enable 292: <1,1>,<5,4,4,3>
	[ 1782.635877] <0>.(0)[8137:kworker/0:0][name:musb_hdrc&][MUSB]musb_start 1286: set ignore babble MUSB_ULPI_REG_DATA=89
	[ 1782.637236] <0>.(0)[8137:kworker/0:0][name:usb20_host&][MUSB]switch_int_to_device 362: switch_int_to_device is done
	[ 1782.638566] <0>.(0)[8137:kworker/0:0][name:usb20_host&][MUSB]musb_id_pin_work 487: work end, is_host=1

	[ 1782.804798] <0>-(0)[8137:kworker/0:0][name:musb_hdrc&][MUSB]musb_stage0_irq 967: MUSB_INTR_CONNECT (b_idle)
	[ 1782.804816] <0>-(0)[8137:kworker/0:0][name:musb_qmu&]QMU_WARN,<musb_disable_q_all 55>, disable_q_all
	[ 1782.805098] <0>-(0)[8137:kworker/0:0][name:musb_hdrc&][MUSB]musb_stage0_irq 1042: CONNECT (a_host) devctl 3d
	
	
关闭OTG使能
	[ 1783.421102] <0>.(0)[211:mtk charger_hv_][name:battery_common_fg_20&][upmu_is_chr_det] Charger exist but USB is host
	[ 1784.426584] <0>.(0)[211:mtk charger_hv_][name:battery_common_fg_20&][upmu_is_chr_det] Charger exist but USB is host
	[ 1784.734018] <0>.(0)[933:batterywarning][name:battery_common_fg_20&][Battery] show_BatteryNotify : 0
	[ 1785.435856] <0>.(0)[211:mtk charger_hv_][name:battery_common_fg_20&][upmu_is_chr_det] Charger exist but USB is host
	[ 1785.912684] <2>.(2)[2884:Binder_A][name:usb20_host&]****before gn_Close_Otg_Irq!
	[ 1785.913612] <2>.(2)[2884:Binder_A][name:usb20_host&][MUSB]gn_Close_Otg_Irq 695: iddig_state = 0
	[ 1785.914715] <1>-(1)[0:swapper/1][name:usb20_host&][MUSB]mt_usb_ext_iddig_int 500: id pin interrupt assert
	[ 1786.312716] <1>.(1)[6996:kworker/1:4][name:usb20_host&][MUSB]musb_id_pin_work 412: work start, is_host=1, boot mode(0)
	[ 1786.314066] <1>.(1)[6996:kworker/1:4][name:usb20_host&][MUSB]musb_is_host 271: will mask PMIC charger detection
	[ 1786.315340] <1>.(1)[6996:kworker/1:4][name:usb20&][MUSB]mt_usb_enable 292: <1,1>,<6,4,4,3>
	[ 1786.316415] <1>.(1)[6996:kworker/1:4][name:usb20_host&][MUSB]musb_is_host 289: iddig_state = 0
	[ 1786.317484] <1>.(1)[6996:kworker/1:4][name:usb20_host&][MUSB]musb_is_host 317: will unmask PMIC charger detection
	[ 1786.318774] <1>.(1)[6996:kworker/1:4][name:usb20_host&][MUSB]musb_is_host 326: usb_is_host = 0
	[ 1786.319840] <1>.(1)[6996:kworker/1:4][name:usb20_host&][MUSB]musb_id_pin_work 425: musb is as device
	[ 1786.321262] <1>.(1)[6996:kworker/1:4][name:usb20_host&][MUSB]musb_id_pin_work 465: devctl is 3d
	[ 1786.322349] <1>.(1)[6996:kworker/1:4][name:usb20_host&][MUSB]mt_usb_set_vbus 91: mt65xx_usb20_vbus++,is_on=0
	[ 1786.327596] <1>.(1)[6996:kworker/1:4][name:usb20_host&][MUSB]musb_id_pin_work 478: force PHY to idle, 0x6d=3f, 0x6c=11
	[ 1786.328928] <1>.(1)[6996:kworker/1:4][name:usb20&][MUSB]mt_usb_disable 340: <1,1>,<6,5,4,3>
	[ 1786.330809] <1>.(1)[6996:kworker/1:4][name:usb20_phy&][MUSB]usb_phy_savecurrent 536: usb save current success
	[ 1786.332041] <1>.(1)[6996:kworker/1:4][name:usb20&][MUSB]mt_usb_disable 358: <6,5,4,4>
	[ 1786.333122] <1>-(1)[6996:kworker/1:4][name:usb20&][MUSB]vcore_release 195: musb lock get, release it, mtk_musb:ffffffc0b5efc308
	[ 1786.346376] <1>.(1)[6996:kworker/1:4][name:usb20&][MUSB]vcore_release 202: release VCORE ok
	[ 1786.348502] <1>.(1)[6996:kworker/1:4][name:musb_hdrc&][MUSB]musb_stop 1357: HDRC disabled
	[ 1786.349825] <1>.(1)[6996:kworker/1:4][name:usb20_host&][MUSB]switch_int_to_host 380: switch_int_to_host is done
	[ 1786.351234] <1>.(1)[6996:kworker/1:4][name:usb20_host&][MUSB]musb_id_pin_work 487: work end, is_host=0
	[ 1786.352618] <1>.(1)[0:swapper/1][name:usb20&][MUSB]musb_do_idle 248: otg_state b_idle

	
	
	gn_otg_charge_switch_State：上层写设备节点，传下来的状态
	gn_otg_charge_switch_Closing这个是为了关闭开关后让检测引脚看成是高电平实际上是低电平
	iddig_state：iddig引脚的状态
	otg_charge_state：otg充电状态
	
	
	
	
	void gn_Close_Otg_Irq(void)
	{
		int iddig_state = 1;
		pr_notice("****before gn_Close_Otg_Irq!\n");	
	#ifdef ID_PIN_USE_EX_EINT
		#ifdef CONFIG_OF
		#if defined(CONFIG_MTK_LEGACY)
		iddig_state = mt_get_gpio_in(iddig_pin);
		#else
		iddig_state = __gpio_get_value(iddig_pin);
		#endif
		DBG(0, "iddig_state = %d\n", iddig_state);

		if(iddig_state)
		{
			//Gionee GuoJianqiu 201601026 modify for GNSPR #70216 begin
			gn_otg_charge_switch_Closing = KAL_TRUE;
			//Gionee GuoJianqiu 201601026 modify for GNSPR #70216 end
			disable_irq(usb_iddig_number);
			irq_set_irq_type(usb_iddig_number, IRQF_TRIGGER_LOW);
			msleep(10);
			pinctrl_select_state(pinctrl, pinctrl_iddig_close);
			msleep(20);
		}
		else
		{
			gn_otg_charge_switch_Closing = KAL_TRUE;
			irq_set_irq_type(usb_iddig_number, IRQF_TRIGGER_LOW);
			msleep(500);
			pinctrl_select_state(pinctrl, pinctrl_iddig_close);
			msleep(100);
		}

		/*
		disable_irq(usb_iddig_number);
		irq_set_irq_type(usb_iddig_number, IRQF_TRIGGER_LOW);
		msleep(10);
		if(!iddig_state)
		{
		gn_musb_id_pin_work();
		msleep(300);
		}
		pinctrl_select_state(pinctrl, pinctrl_iddig_close);
		if(iddig_state)
			msleep(20);
		else
			msleep(200);
		*/
		#endif
	#endif
		DBG(0, "gn_Close_Otg_Irq is done\n");
	}



	void gn_Open_Otg_Irq(void)
	{
		int iddig_state = 1;
		pr_notice("****before gn_Open_Otg_Irq!\n");	
	#ifdef ID_PIN_USE_EX_EINT
		#ifdef CONFIG_OF		
		pinctrl_select_state(pinctrl, pinctrl_iddig);
		msleep(100);
		gn_otg_charge_switch_Closing = KAL_FALSE;
		irq_set_irq_type(usb_iddig_number, IRQF_TRIGGER_LOW);
		//Gionee GuoJianqiu 201601026 modify for GNSPR #70216 begin
		irq_set_irq_type(usb_iddig_number, IRQF_TRIGGER_LOW);
		//Gionee GuoJianqiu 201601026 modify for GNSPR #70216 end
		enable_irq(usb_iddig_number);	
		//Gionee GuoJianqiu 201601026 modify for GNSPR #70216 begin
		enable_irq(usb_iddig_number);	
		DBG(0, "gn_Set_Otg_Irq again!!\n");
		//Gionee GuoJianqiu 201601026 modify for GNSPR #70216 end
	
		#if defined(CONFIG_MTK_LEGACY)
		iddig_state = mt_get_gpio_in(iddig_pin);
		#else
		iddig_state = __gpio_get_value(iddig_pin);
		#endif
		DBG(0, "iddig_state = %d\n", iddig_state);
	
		if(iddig_state)
			msleep(30);
		else
			msleep(350);
		#endif
	#endif
		DBG(0, "gn_Open_Otg_Irq is done\n");
	}
	#endif
	//Gionee GuoJianqiu 201601026 modify for OTG SWITCH end



}





























->/*code*/
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





}



























2016.12.19

11.59				USB充电			77%
13：36			   拔充电器			  94%
13：41			   座充充电			  94%
13：50			   拔充电器			  95%




2016.12.22		OTG反向充电

09：45~47
主机		960mA~1100mA
从机		150mA~250mA







手机插上充电器的过程：
12-19 11:59:01.738186 <3>[  234.867987]  (0)[54:pmic_thread][name:battery_common_fg_20&][do_chrdet_int_task] charger exist!
12-19 11:59:01.738248 <7>[  234.868049]  (0)[54:pmic_thread][name:battery_meter_fg_20&][battery_meter_driver] malloc size=16
12-19 11:59:01.738336 <3>[  234.868137]  (0)[54:pmic_thread][name:charging_hw_bq24158&][is_chr_det] 1
12-19 11:59:01.738659 <7>[  234.868460]  (0)[422:fuelgauged][name:battery_meter_fg_20&][fg_res] init_flag = 1
12-19 11:59:01.738761 <7>[  234.868562]  (0)[422:fuelgauged][name:battery_meter_fg_20&][fg_res] charger_exist = 1
12-19 11:59:01.739112 <7>[  234.868913]  (0)[422:fuelgauged][name:battery_meter_hal&][dump_nter] mt6328_upmu_get_fg_nter_29_16 = 0x0
12-19 11:59:01.739133 <7>[  234.868934]  (0)[422:fuelgauged][name:battery_meter_hal&][dump_nter] mt6328_upmu_get_fg_nter_15_00 = 0xe41
12-19 11:59:01.739152 <7>[  234.868953]  (0)[422:fuelgauged][name:battery_meter_hal&][dump_car] upmu_get_fg_car_31_16 = 0xff9f
12-19 11:59:01.739171 <7>[  234.868972]  (0)[422:fuelgauged][name:battery_meter_hal&][dump_car] upmu_get_fg_car_15_00 = 0x5742
12-19 11:59:01.739183 <7>[  234.868984]  (0)[422:fuelgauged][name:battery_meter_fg_20&][fg_res] fg_coulomb = -356
12-19 11:59:01.739274 <7>[  234.869075]  (0)[422:fuelgauged][name:battery_meter_fg_20&][fg_res] set suspend time
12-19 11:59:01.739592 <7>[  234.869393]  (0)[422:fuelgauged][name:battery_meter_hal&][dump_nter] mt6328_upmu_get_fg_nter_29_16 = 0x0
12-19 11:59:01.739612 <7>[  234.869413]  (0)[422:fuelgauged][name:battery_meter_hal&][dump_nter] mt6328_upmu_get_fg_nter_15_00 = 0xe41
12-19 11:59:01.739631 <7>[  234.869432]  (0)[422:fuelgauged][name:battery_meter_hal&][dump_car] upmu_get_fg_car_31_16 = 0xff9f
12-19 11:59:01.739649 <7>[  234.869450]  (0)[422:fuelgauged][name:battery_meter_hal&][dump_car] upmu_get_fg_car_15_00 = 0x5742
12-19 11:59:01.739659 <7>[  234.869460]  (0)[422:fuelgauged][name:battery_meter_fg_20&][fg_res] fg_coulomb = -356
12-19 11:59:01.739748 <7>[  234.869549]  (0)[422:fuelgauged][name:battery_meter_fg_20&][fg_res] fgadc_reset
12-19 11:59:01.740025 <7>[  234.869826]  (0)[422:fuelgauged][name:battery_meter_hal&][dump_nter] mt6328_upmu_get_fg_nter_29_16 = 0x0
12-19 11:59:01.740044 <7>[  234.869845]  (0)[422:fuelgauged][name:battery_meter_hal&][dump_nter] mt6328_upmu_get_fg_nter_15_00 = 0x0
12-19 11:59:01.740063 <7>[  234.869864]  (0)[422:fuelgauged][name:battery_meter_hal&][dump_car] upmu_get_fg_car_31_16 = 0x0
12-19 11:59:01.740082 <7>[  234.869883]  (0)[422:fuelgauged][name:battery_meter_hal&][dump_car] upmu_get_fg_car_15_00 = 0x0
12-19 11:59:01.740177 <7>[  234.869978]  (0)[422:fuelgauged][name:battery_meter_fg_20&][fg_res] charger_exist = 1
12-19 11:59:01.740264 <7>[  234.870065]  (0)[422:fuelgauged][name:battery_meter_fg_20&][fg_res] is_charging = 1
12-19 11:59:01.740351 <7>[  234.870152]  (0)[422:fuelgauged][name:battery_meter_fg_20&][fg_res] gFG_DOD0 = 23
12-19 11:59:01.740438 <7>[  234.870239]  (0)[422:fuelgauged][name:battery_meter_fg_20&][fg_res] gFG_DOD1 = 23
12-19 11:59:01.758650 <7>[  234.888451]  (0)[58:cfinteractive][name:mt_cpufreq&][Power/cpufreq] @_mt_cpufreq_set_locked(): Vproc = 950mv, freq = 442000 KHz
12-19 11:59:01.775238 <7>[  234.905039]  (0)[587:AALLightSensor][name:aal_control&][ALS/AAL]Get als dat :65
12-19 11:59:01.921178 <3>[  235.050979]  (0)[58:cfinteractive][name:aed&]AEE_MONITOR_SET[status]: 0x1
12-19 11:59:01.928973 <7>[  235.058774]  (0)[58:cfinteractive][name:mt_cpufreq&][Power/cpufreq] @_mt_cpufreq_set_locked(): Vproc = 950mv, freq = 299000 KHz
12-19 11:59:01.976519 <7>[  235.106320]  (0)[587:AALLightSensor][name:aal_control&][ALS/AAL]Get als dat :65
12-19 11:59:02.004936 <3>[  235.134737]  (0)[210:mtk charger_hv_][name:battery_common_fg_20&][check_battery_exist] baton_count = 0
12-19 11:59:02.004936 <3>[  235.134737]  
12-19 11:59:02.007603 <7>[  235.137404]  (0)[210:mtk charger_hv_][name:battery_meter_fg_20&][g_Get_I_Charging] gFG_current_inout_battery : 1283
12-19 11:59:02.007653 <3>[  235.137454]  (0)[210:mtk charger_hv_][name:battery_common_fg_20&][gn_update_BatteryPresentCurrent] BAT_ChargerVoltage = (4588), PresentCurrent = (128)
12-19 11:59:02.015409 <12>[  235.145210]  (0)[266:healthd]healthd: Unknown battery status '3'
12-19 11:59:02.025315 <14>[  235.155116]  (0)[266:healthd]healthd: battery l=76 v=4067 t=26.0 h=2 st=4 chg=
12-19 11:59:02.025983 <14>[  235.155784]  (0)[266:healthd]healthd: send_already = 0
12-19 11:59:02.029223 <7>[  235.159024]  (0)[58:cfinteractive][name:mt_cpufreq&][Power/cpufreq] @_mt_cpufreq_set_locked(): Vproc = 1156mv, freq = 1300000 KHz
12-19 11:59:02.048215 <3>[  235.178016]  (0)[54:pmic_thread][name:pmic_chr_type_det&]CDP, PASS
12-19 11:59:02.048325 <4>[  235.178126]  (0)[54:pmic_thread][name:usb20_phy&][MUSB]Charger_Detect_Init 656: Charger_Detect_Init
12-19 11:59:02.141577 <4>[  235.271378]  (0)[95:hps_main][name:mt_hotplug_strategy_algo&][HPS] (0000)(1)DBG_HRT(7)(10)(0)(0) (4)(4)(4)(4)(1) (39)(8)(0) (0)(0)(0) (0)(10)(8)(0)(10) wifi_base(0)
12-19 11:59:02.148653 <7>[  235.278454]  (0)[58:cfinteractive][name:mt_cpufreq&][Power/cpufreq] @_mt_cpufreq_set_locked(): Vproc = 950mv, freq = 299000 KHz
12-19 11:59:02.178381 <7>[  235.308182]  (0)[587:AALLightSensor][name:aal_control&][ALS/AAL]Get als dat :65
12-19 11:59:02.328273 <7>[  235.458074]  (0)[108:display_idle_de][name:primary_display&][LP] - enter: 61, flag:0,1
12-19 11:59:02.328370 <7>[  235.458171]  (0)[108:display_idle_de][name:primary_display&][DISPCHECK]primary_display_switch_mode sess_mode 2, session 0x10000
12-19 11:59:02.361369 <7>[  235.491170]  (0)[108:display_idle_de][name:primary_display&][DISPCHECK]dpmgr create ovl memout path SUCCESS(ffffffc04c33c000)
12-19 11:59:02.361988 <7>[  235.491789]  (0)[108:display_idle_de][MMDVFS][name:mmdvfs_mgr&][pid=108]Set vol scen:17,step:0,final:0(0x0),CMD(0,0,0x0),INFO(0,0)
12-19 11:59:02.362044 <7>[  235.491845]  (0)[108:display_idle_de][VcoreFS] [name:mt_vcore_dvfs_1&]feature_en: 1(0), recover_en: 1, kicker: 1, new_opp: -1(-1)
12-19 11:59:02.362088 <7>[  235.491889]  (0)[108:display_idle_de][VcoreFS] [name:mt_vcore_dvfs_1&]*** VCORE DVFS OPP NO CHANGE (0x0) ***
12-19 11:59:02.362684 <7>[  235.492485]  (0)[108:display_idle_de][name:primary_display&][LP] end: 61, flag:1,0
12-19 11:59:02.368398 <3>[  235.498199]  (0)[54:pmic_thread][name:pmic_chr_type_det&]step A2 : Standard USB Host!
12-19 11:59:02.368590 <4>[  235.498391]  (0)[54:pmic_thread][name:usb20_phy&][MUSB]Charger_Detect_Release 666: Charger_Detect_Release
12-19 11:59:02.368633 <3>[  235.498434]  (0)[54:pmic_thread][name:battery_common_fg_20&][mt_charger_type_detection] charger type(1), detection again!! 
12-19 11:59:02.368684 <3>[  235.498485]  (0)[54:pmic_thread][name:charging_hw_bq24158&][is_chr_det] 1
12-19 11:59:02.380487 <7>[  235.510288]  (0)[587:AALLightSensor][name:aal_control&][ALS/AAL]Get als dat :65
12-19 11:59:02.488382 <7>[  235.618183]  (0)[1109:kworker/0:2][name:mtk_ts_bts&][Power/BTS_Thermal] T_AP=44000
12-19 11:59:02.581798 <7>[  235.711599]  (0)[587:AALLightSensor][name:aal_control&][ALS/AAL]Get als dat :65
12-19 11:59:02.608743 <7>[  235.738544]  (0)[58:cfinteractive][name:mt_cpufreq&][Power/cpufreq] @_mt_cpufreq_set_locked(): Vproc = 950mv, freq = 442000 KHz
12-19 11:59:02.628711 <7>[  235.758512]  (0)[58:cfinteractive][name:mt_cpufreq&][Power/cpufreq] @_mt_cpufreq_set_locked(): Vproc = 1156mv, freq = 1300000 KHz
12-19 11:59:02.668619 <3>[  235.798420]  (0)[1752:watchdog][name:aed&]AEEIOCTL_RT_MON_Kick ( 300)
12-19 11:59:02.668640 <3>[  235.798441]  (0)[1752:watchdog][name:aed&][Hang_Detect] hang_detect enabled 10
12-19 11:59:02.678222 <3>[  235.808023]  (0)[54:pmic_thread][name:pmic_chr_type_det&]CDP, PASS
12-19 11:59:02.678330 <4>[  235.808131]  (0)[54:pmic_thread][name:usb20_phy&][MUSB]Charger_Detect_Init 656: Charger_Detect_Init
12-19 11:59:02.782728 <7>[  235.912529]  (0)[587:AALLightSensor][name:aal_control&][ALS/AAL]Get als dat :65
12-19 11:59:02.788527 <7>[  235.918328]  (0)[58:cfinteractive][name:mt_cpufreq&][Power/cpufreq] @_mt_cpufreq_set_locked(): Vproc = 950mv, freq = 299000 KHz
12-19 11:59:02.858276 <7>[  235.988077]  (0)[102:display_esd_che][name:primary_display&][DISPCHECK][ESD]ESD check begin
12-19 11:59:02.858708 <7>[  235.988509]  (0)[102:display_esd_che][name:primary_display&][DISPCHECK][ESD]ESD config thread=ffffffc04c2de000
12-19 11:59:02.860101 <7>[  235.989902]  (0)[102:display_esd_che][name:primary_display&][DISPCHECK][ESD]_esd_check_config_handle_vdo ret=0
12-19 11:59:02.860149 <7>[  235.989950]  (0)[102:display_esd_che][name:primary_display&][DISPCHECK][ESD]ESD config thread done~
12-19 11:59:02.860229 <7>[  235.990030]  (0)[102:display_esd_che][name:ddp_dsi&][DISPCHECK][DSI]enter cmp i=0
12-19 11:59:02.860443 <7>[  235.990244]  (0)[102:display_esd_che][name:primary_display&][DISPCHECK][ESD]ESD check end
12-19 11:59:02.922615 <3>[  236.052416]  (0)[587:AALLightSensor][name:aed&]AEE_MONITOR_SET[status]: 0x1
12-19 11:59:02.984007 <7>[  236.113808]  (0)[587:AALLightSensor][name:aal_control&][ALS/AAL]Get als dat :65
12-19 11:59:02.998304 <3>[  236.128105]  (0)[54:pmic_thread][name:pmic_chr_type_det&]step A2 : Standard USB Host!
12-19 11:59:02.998498 <4>[  236.128299]  (0)[54:pmic_thread][name:usb20_phy&][MUSB]Charger_Detect_Release 666: Charger_Detect_Release
12-19 11:59:02.998549 <4>[  236.128350]  (0)[54:pmic_thread][name:usb20&][MUSB]mt_usb_connect 415: is ready 1 is_host 0 power 0
12-19 11:59:02.998590 <4>[  236.128391]  (0)[54:pmic_thread][name:usb20&][MUSB]mt_usb_connect 439: cable_mode=1
12-19 11:59:02.998640 <4>[  236.128441]  (0)[54:pmic_thread][name:usb20&][MUSB]mt_usb_connect 481: lock
12-19 11:59:02.998681 <4>[  236.128482]  (0)[54:pmic_thread][name:musb_hdrc&][MUSB]musb_start 1221: start, is_host=0 is_active=0
12-19 11:59:02.998727 <4>[  236.128528]  (0)[54:pmic_thread][name:usb20&][MUSB]mt_usb_enable 292: <0,0>,<2,3,1,1>
12-19 11:59:02.998867 <4>[  236.128668]  (0)[54:pmic_thread][name:usb20&][MUSB]mt_usb_enable 313: enable UPLL before connect
12-19 11:59:02.998906 <4>[  236.128707]  (0)[54:pmic_thread][name:usb20&][MUSB]vcore_hold 154: before releasing
12-19 11:59:02.998944 <4>[  236.128745]  (0)[54:pmic_thread][name:usb20&][MUSB]vcore_hold 157: after releasing




拔掉充电器
12-19 13:36:04.798953 <3>[ 6057.928755]  (0)[54:pmic_thread][name:battery_common_fg_20&][do_chrdet_int_task] charger NOT exist!
12-19 13:36:04.799003 <7>[ 6057.928805]  (0)[54:pmic_thread][name:battery_meter_fg_20&][battery_meter_driver] malloc size=16
12-19 13:36:04.799071 <3>[ 6057.928873]  (0)[54:pmic_thread][name:battery_common_fg_20&][BAT_thread]Cable out 
12-19 13:36:04.799083 <4>[ 6057.928885]  (0)[54:pmic_thread][name:usb20&][MUSB]mt_usb_disconnect 501: is ready 1 is_host 0 power 1
12-19 13:36:04.799101 <6>[ 6057.928903] -(0)[54:pmic_thread]android_usb gadget: disable function 'mtp'/ffffffc0b1e8a980
12-19 13:36:04.799110 <4>[ 6057.928912] -(0)[54:pmic_thread]mtp_function_disable
12-19 13:36:04.799122 <4>[ 6057.928924] -(0)[54:pmic_thread][name:mtk_qmu&]QMU_WARN,<mtk_qmu_stop 468>, TQ 1 already inactive
12-19 13:36:04.799164 <4>[ 6057.928966] -(0)[54:pmic_thread][name:mtk_qmu&]QMU_WARN,<mtk_qmu_stop 473>, Stop RQ 1
12-19 13:36:04.799203 <4>[ 6057.929005] -(0)[54:pmic_thread][name:musb_hdrc&][MUSB]nuke 615: call musb_g_giveback on function nuke ep is ep1out
12-19 13:36:04.799215 <4>[ 6057.929017] -(0)[54:pmic_thread][name:mtk_qmu&]QMU_WARN,<mtk_qmu_stop 468>, TQ 2 already inactive
12-19 13:36:04.799227 <6>[ 6057.929029] -(0)[54:pmic_thread]android_usb gadget: disable function 'Function FS Gadget'/ffffffc0ad6ed1f8
12-19 13:36:04.799241 <4>[ 6057.929043] -(0)[54:pmic_thread][name:mtk_qmu&]QMU_WARN,<mtk_qmu_stop 473>, Stop RQ 2
12-19 13:36:04.799272 <4>[ 6057.929074] -(0)[54:pmic_thread][name:musb_hdrc&][MUSB]nuke 615: call musb_g_giveback on function nuke ep is ep2out
12-19 13:36:04.799284 <4>[ 6057.929086] -(0)[54:pmic_thread][name:mtk_qmu&]QMU_WARN,<mtk_qmu_stop 468>, TQ 3 already inactive
12-19 13:36:04.799298 <5>[ 6057.929100] -(0)[54:pmic_thread][name:g_android&][g_android][USB]android_disconnect: 
12-19 13:36:04.799323 <5>[ 6057.929125] -(0)[54:pmic_thread][name:g_android&][g_android][USB]android_disconnect: dev->connected = 0 
12-19 13:36:04.799333 <6>[ 6057.929135] -(0)[54:pmic_thread]android_usb gadget: [COM]composite_disconnect: reassign the complete function!!
12-19 13:36:04.799345 <4>[ 6057.929147]  (0)[54:pmic_thread][name:usb20&][MUSB]mt_usb_disable 340: <1,1>,<2,4,2,1>
12-19 13:36:04.800182 <4>[ 6057.929984]  (0)[54:pmic_thread][name:usb20_phy&][MUSB]usb_phy_savecurrent 536: usb save current success
12-19 13:36:04.800193 <4>[ 6057.929995]  (0)[54:pmic_thread][name:usb20&][MUSB]mt_usb_disable 358: <2,4,2,2>
12-19 13:36:04.800204 <4>[ 6057.930006] -(0)[54:pmic_thread][name:usb20&][MUSB]vcore_release 195: musb lock get, release it, mtk_musb:ffffffc0b5f1c308
12-19 13:36:04.800218 <7>[ 6057.930020]  (0)[54:pmic_thread][VcoreFS] [name:mt_vcore_dvfs_1&]feature_en: 1(0), recover_en: 1, kicker: 4, new_opp: -1(0)
12-19 13:36:04.800231 <7>[ 6057.930033]  (0)[54:pmic_thread][VcoreFS] [name:mt_vcore_dvfs_1&][-1, 0, -1, -1, -1, -1]
12-19 13:36:04.800243 <7>[ 6057.930045]  (0)[54:pmic_thread][VcoreFS] [name:mt_vcore_dvfs_1&]kicker: 4, opp: 0, curr_opp: 0(0)
12-19 13:36:04.800261 <7>[ 6057.930063]  (0)[54:pmic_thread][VcoreFS] [name:mt_vcore_dvfs_1&]opp: 0, vcore: 1150000(1150000) 
12-19 13:36:04.800275 <4>[ 6057.930077]  (0)[54:pmic_thread][name:usb20&][MUSB]vcore_release 202: release VCORE ok
12-19 13:36:04.800286 <4>[ 6057.930088]  (0)[54:pmic_thread][name:musb_hdrc&][MUSB]musb_stop 1357: HDRC disabled
12-19 13:36:04.800299 <4>[ 6057.930101]  (0)[54:pmic_thread][name:usb20&][MUSB]mt_usb_disconnect 531: unlock
12-19 13:36:04.800310 <4>[ 6057.930112]  (0)[54:pmic_thread][name:usb20&][MUSB]mt_usb_disconnect 537: cable_mode=1
12-19 13:36:04.800320 <4>[ 6057.930122]  (0)[54:pmic_thread][name:usb20&][MUSB]mt_usb_disconnect 546: [MUSB] USB disconnect
12-19 13:36:04.800557 <7>[ 6057.930359]  (0)[54:pmic_thread][name:battery_meter_fg_20&][g_Get_I_Charging] gFG_current_inout_battery : -3790
12-19 13:36:04.800568 <3>[ 6057.930370]  (0)[54:pmic_thread][name:battery_common_fg_20&][kernel][battery_update] SOC 93,UI_SOC2 95, status 3
12-19 13:36:04.800577 <3>[ 6057.930379]  (0)[54:pmic_thread][name:battery_common_fg_20&][DLPT_POWER_OFF_EN] run
12-19 13:36:04.800621 <3>[ 6057.930423]  (0)[54:pmic_thread][name:battery_common_fg_20&][BATTERY] wake_up_bat. 
12-19 13:36:04.800661 <4>[ 6057.930463]  (0)[54:pmic_thread][PWRAP] clear EINT flag mt_pmic_wrap_eint_status=0x0
12-19 13:36:04.802008 <7>[ 6057.931810]  (0)[422:fuelgauged][name:battery_meter_fg_20&][fg_res] init_flag = 1
12-19 13:36:04.802108 <7>[ 6057.931910]  (0)[422:fuelgauged][name:battery_meter_fg_20&][fg_res] charger_exist = 0
12-19 13:36:04.802201 <7>[ 6057.932003]  (0)[422:fuelgauged][name:battery_meter_fg_20&][fg_res] set suspend time
12-19 13:36:04.802516 <7>[ 6057.932318]  (0)[422:fuelgauged][name:battery_meter_hal&][dump_nter] mt6328_upmu_get_fg_nter_29_16 = 0x1
12-19 13:36:04.802534 <7>[ 6057.932336]  (0)[422:fuelgauged][name:battery_meter_hal&][dump_nter] mt6328_upmu_get_fg_nter_15_00 = 0x6bf2
12-19 13:36:04.802551 <7>[ 6057.932353]  (0)[422:fuelgauged][name:battery_meter_hal&][dump_car] upmu_get_fg_car_31_16 = 0x702
12-19 13:36:04.802568 <7>[ 6057.932370]  (0)[422:fuelgauged][name:battery_meter_hal&][dump_car] upmu_get_fg_car_15_00 = 0x13d8
12-19 13:36:04.802579 <7>[ 6057.932381]  (0)[422:fuelgauged][name:battery_meter_fg_20&][fg_res] fg_coulomb = 6647
12-19 13:36:04.802666 <7>[ 6057.932468]  (0)[422:fuelgauged][name:battery_meter_fg_20&][fg_res] fgadc_reset
12-19 13:36:04.802907 <7>[ 6057.932709]  (0)[422:fuelgauged][name:battery_meter_hal&][dump_nter] mt6328_upmu_get_fg_nter_29_16 = 0x0
12-19 13:36:04.802924 <7>[ 6057.932726]  (0)[422:fuelgauged][name:battery_meter_hal&][dump_nter] mt6328_upmu_get_fg_nter_15_00 = 0x0
12-19 13:36:04.802940 <7>[ 6057.932742]  (0)[422:fuelgauged][name:battery_meter_hal&][dump_car] upmu_get_fg_car_31_16 = 0x0
12-19 13:36:04.802957 <7>[ 6057.932759]  (0)[422:fuelgauged][name:battery_meter_hal&][dump_car] upmu_get_fg_car_15_00 = 0x0
12-19 13:36:04.803050 <7>[ 6057.932852]  (0)[422:fuelgauged][name:battery_meter_fg_20&][fg_res] charger_exist = 0
12-19 13:36:04.803136 <7>[ 6057.932938]  (0)[422:fuelgauged][name:battery_meter_fg_20&][fg_res] is_charging = 1
12-19 13:36:04.803226 <7>[ 6057.933028]  (0)[422:fuelgauged][name:battery_meter_fg_20&][fg_res] gFG_DOD0 = 7
12-19 13:36:04.803314 <7>[ 6057.933116]  (0)[422:fuelgauged][name:battery_meter_fg_20&][fg_res] gFG_DOD1 = 7
12-19 13:36:04.803425 <4>[ 6057.933227]  (0)[4447:kworker/0:0][name:usb20&][MUSB]usb_cable_connected 594: type(0)
12-19 13:36:04.803436 <5>[ 6057.933238]  (0)[4447:kworker/0:0][name:g_android&][g_android][USB]android_work: is_hwconnected=0 
12-19 13:36:04.803637 <5>[ 6057.933439]  (0)[4447:kworker/0:0][name:g_android&][g_android][USB]android_work: sent uevent USB_STATE=DISCONNECTED
12-19 13:36:04.803727 <5>[ 6057.933529]  (0)[4447:kworker/0:0][name:g_android&][g_android][USB]android_work: sent uevent USB_STATE=HWDISCONNECTED
12-19 13:36:04.803738 <5>[ 6057.933540]  (0)[4447:kworker/0:0][name:g_android&][g_android][USB]android_work: did not send zero uevent
12-19 13:36:04.804690 <6>[ 6057.934492]  (0)[3909:MtpServer]mtp_release
12-19 13:36:04.805991 <3>[ 6057.935793]  (0)[208:bat_routine_thr][name:battery_common_fg_20&][fg2.0]CUST_TRACKING_POINT: 0.
12-19 13:36:04.806010 <3>[ 6057.935812]  (0)[208:bat_routine_thr][name:battery_common_fg_20&][Battery] mt_battery_update_duration_time , last_time=6057 current_time=6057 duration=0
12-19 13:36:04.806051 <7>[ 6057.935853]  (0)[208:bat_routine_thr][name:battery_meter_fg_20&][battery_meter_driver] malloc size=16
12-19 13:36:04.806118 <3>[ 6057.935920]  (0)[208:bat_routine_thr][name:battery_common_fg_20&][BAT_thread]Cable out 
12-19 13:36:04.806130 <4>[ 6057.935932]  (0)[208:bat_routine_thr][name:usb20&][MUSB]mt_usb_disconnect 501: is ready 1 is_host 0 power 0
12-19 13:36:04.808142 <4>[ 6057.937944]  (0)[1167:UEventObserver][name:usb20&][MUSB]musb_do_idle 248: otg_state b_idle





座充阶段：

12-19 13:36:54.228068 <3>[ 6107.357856]  (0)[54:pmic_thread][name:battery_common_fg_20&][do_chrdet_int_task] charger exist!
12-19 13:36:54.228123 <7>[ 6107.357911]  (0)[54:pmic_thread][name:battery_meter_fg_20&][battery_meter_driver] malloc size=16
12-19 13:36:54.228198 <3>[ 6107.357986]  (0)[54:pmic_thread][name:charging_hw_bq24158&][is_chr_det] 1
12-19 13:36:54.228612 <7>[ 6107.358400]  (0)[422:fuelgauged][name:battery_meter_fg_20&][fg_res] init_flag = 1
12-19 13:36:54.228716 <7>[ 6107.358504]  (0)[422:fuelgauged][name:battery_meter_fg_20&][fg_res] charger_exist = 1
12-19 13:36:54.228999 <7>[ 6107.358787]  (0)[422:fuelgauged][name:battery_meter_hal&][dump_nter] mt6328_upmu_get_fg_nter_29_16 = 0x0
12-19 13:36:54.229016 <7>[ 6107.358804]  (0)[422:fuelgauged][name:battery_meter_hal&][dump_nter] mt6328_upmu_get_fg_nter_15_00 = 0x12ba
12-19 13:36:54.229033 <7>[ 6107.358821]  (0)[422:fuelgauged][name:battery_meter_hal&][dump_car] upmu_get_fg_car_31_16 = 0xffef
12-19 13:36:54.229049 <7>[ 6107.358837]  (0)[422:fuelgauged][name:battery_meter_hal&][dump_car] upmu_get_fg_car_15_00 = 0xd613
12-19 13:36:54.229059 <7>[ 6107.358847]  (0)[422:fuelgauged][name:battery_meter_fg_20&][fg_res] fg_coulomb = -57
12-19 13:36:54.229151 <7>[ 6107.358939]  (0)[422:fuelgauged][name:battery_meter_fg_20&][fg_res] set suspend time
12-19 13:36:54.229454 <7>[ 6107.359242]  (0)[422:fuelgauged][name:battery_meter_hal&][dump_nter] mt6328_upmu_get_fg_nter_29_16 = 0x0
12-19 13:36:54.229471 <7>[ 6107.359259]  (0)[422:fuelgauged][name:battery_meter_hal&][dump_nter] mt6328_upmu_get_fg_nter_15_00 = 0x12ba
12-19 13:36:54.229487 <7>[ 6107.359275]  (0)[422:fuelgauged][name:battery_meter_hal&][dump_car] upmu_get_fg_car_31_16 = 0xffef
12-19 13:36:54.229502 <7>[ 6107.359290]  (0)[422:fuelgauged][name:battery_meter_hal&][dump_car] upmu_get_fg_car_15_00 = 0xd613
12-19 13:36:54.229512 <7>[ 6107.359300]  (0)[422:fuelgauged][name:battery_meter_fg_20&][fg_res] fg_coulomb = -57
12-19 13:36:54.229600 <7>[ 6107.359388]  (0)[422:fuelgauged][name:battery_meter_fg_20&][fg_res] fgadc_reset
12-19 13:36:54.229850 <7>[ 6107.359638]  (0)[422:fuelgauged][name:battery_meter_hal&][dump_nter] mt6328_upmu_get_fg_nter_29_16 = 0x0
12-19 13:36:54.229866 <7>[ 6107.359654]  (0)[422:fuelgauged][name:battery_meter_hal&][dump_nter] mt6328_upmu_get_fg_nter_15_00 = 0x0
12-19 13:36:54.229882 <7>[ 6107.359670]  (0)[422:fuelgauged][name:battery_meter_hal&][dump_car] upmu_get_fg_car_31_16 = 0x0
12-19 13:36:54.229898 <7>[ 6107.359686]  (0)[422:fuelgauged][name:battery_meter_hal&][dump_car] upmu_get_fg_car_15_00 = 0x0
12-19 13:36:54.229990 <7>[ 6107.359778]  (0)[422:fuelgauged][name:battery_meter_fg_20&][fg_res] charger_exist = 1
12-19 13:36:54.230080 <7>[ 6107.359868]  (0)[422:fuelgauged][name:battery_meter_fg_20&][fg_res] is_charging = 1
12-19 13:36:54.230169 <7>[ 6107.359957]  (0)[422:fuelgauged][name:battery_meter_fg_20&][fg_res] gFG_DOD0 = 7
12-19 13:36:54.230256 <7>[ 6107.360044]  (0)[422:fuelgauged][name:battery_meter_fg_20&][fg_res] gFG_DOD1 = 7
12-19 13:36:54.248415 <7>[ 6107.378203]  (0)[577:AALServiceMain][name:ddp_pwm&][PWM] (latest= 5):   980(  63, 235)  981(  63, 260)  982(  63, 269)  983(  63, 284)  984(  63, 300)
12-19 13:36:54.258183 <7>[ 6107.387971]  (0)[4447:kworker/0:0][VcoreFS] [name:mt_vcore_dvfs_1&]feature_en: 1(0), recover_en: 1, kicker: 2, new_opp: -1(0)
12-19 13:36:54.258202 <7>[ 6107.387990]  (0)[4447:kworker/0:0][VcoreFS] [name:mt_vcore_dvfs_1&][-1, 0, -1, -1, -1, -1]
12-19 13:36:54.258215 <7>[ 6107.388003]  (0)[4447:kworker/0:0][VcoreFS] [name:mt_vcore_dvfs_1&]kicker: 2, opp: 0, curr_opp: 0(0)
12-19 13:36:54.258234 <7>[ 6107.388022]  (0)[4447:kworker/0:0][VcoreFS] [name:mt_vcore_dvfs_1&]opp: 0, vcore: 1150000(1150000) 
12-19 13:36:54.258248 <7>[ 6107.388036]  (0)[4447:kworker/0:0][VcoreFS] [name:mt_vcore_dvfs_1&][-1, 0, -1, -1, -1, -1]
12-19 13:36:54.258261 <7>[ 6107.388049]  (0)[4447:kworker/0:0][VcoreFS] [name:mt_vcore_dvfs_1&]opp: 0, faxi: 218000(218000), fvenc: 300000(300000) 
12-19 13:36:54.258277 <7>[ 6107.388065]  (0)[4447:kworker/0:0][VcoreFS] [name:mt_vcore_dvfs_1&]opp: 0, fddr: 1280000(1280000) 
12-19 13:36:54.258288 <2>[ 6107.388076]  (0)[4447:kworker/0:0][name:fliper&]
12-19 13:36:54.258288 <2>[ 6107.388076] <<SOC DVFS FLIPER>> flip to E(0), 500
12-19 13:36:54.318342 <7>[ 6107.448130]  (0)[58:cfinteractive][name:mt_cpufreq&][Power/cpufreq] @_mt_cpufreq_set_locked(): Vproc = 950mv, freq = 598000 KHz
12-19 13:36:54.332000 <7>[ 6107.461788]  (0)[577:AALServiceMain][name:ddp_pwm&][PWM] (latest= 5):   985(  63, 317)  986(  63, 334)  987(  63, 350)  988(  63, 367)  989(  63, 384)
12-19 13:36:54.354935 <7>[ 6107.484723]  (0)[587:AALLightSensor][name:aal_control&][ALS/AAL]Get als dat :65
12-19 13:36:54.398441 <7>[ 6107.528229]  (0)[58:cfinteractive][name:mt_cpufreq&][Power/cpufreq] @_mt_cpufreq_set_locked(): Vproc = 950mv, freq = 299000 KHz
12-19 13:36:54.416202 <7>[ 6107.545990]  (0)[577:AALServiceMain][name:ddp_pwm&][PWM] (latest= 5):   990(  63, 401)  991(  63, 417)  992(  63, 434)  993(  63, 452)  994(  63, 468)
12-19 13:36:54.499424 <7>[ 6107.629212]  (0)[577:AALServiceMain][name:ddp_pwm&][PWM] (latest= 5):   995(  63, 484)  996(  63, 501)  997(  63, 518)  998(  63, 537)  999(  63, 551)
12-19 13:36:54.518592 <7>[ 6107.648380]  (0)[58:cfinteractive][name:mt_cpufreq&][Power/cpufreq] @_mt_cpufreq_set_locked(): Vproc = 1143mv, freq = 1300000 KHz
12-19 13:36:54.519947 <4>[ 6107.649735]  (0)[95:hps_main][name:mt_hotplug_strategy_algo&][HPS] (0000)(1)DBG_HRT(57)(158)(0)(0) (4)(4)(4)(4)(1) (80)(7)(1) (0)(0)(0) (0)(158)(7)(0)(158) wifi_base(0)
12-19 13:36:54.538033 <3>[ 6107.667821]  (0)[54:pmic_thread][name:pmic_chr_type_det&]CDP, PASS
12-19 13:36:54.538136 <4>[ 6107.667924]  (0)[54:pmic_thread][name:usb20_phy&][MUSB]Charger_Detect_Init 656: Charger_Detect_Init
12-19 13:36:54.555750 <7>[ 6107.685538]  (0)[587:AALLightSensor][name:aal_control&][ALS/AAL]Get als dat :65
12-19 13:36:54.556362 <7>[ 6107.686150]  (0)[56:dlpt_notify_thr][name:pmic&][dlpt] get_dlpt_imix_charging 9648 4365 3100 118
12-19 13:36:54.558076 <2>[ 6107.687864]  (0)[56:dlpt_notify_thr][name:mt_pbm&][PBM] [ma_to_mw] 4367(mV) * 5500(mA) = 24018(mW)
12-19 13:36:54.558104 <7>[ 6107.687892]  (0)[56:dlpt_notify_thr][name:pmic&][exec_dlpt_callback] g_dlpt_val=5500
12-19 13:36:54.558116 <7>[ 6107.687904]  (0)[56:dlpt_notify_thr][name:pmic&][DLPT_final] 5500,0,94,94,5500
12-19 13:36:54.580984 <7>[ 6107.710772]  (0)[577:AALServiceMain][name:ddp_pwm&][PWM] (latest= 5):  1000(  63, 568) 1001(  63, 583) 1002(  63, 600) 1003(  63, 616) 1004(  63, 633)
12-19 13:36:54.648480 <7>[ 6107.778268]  (0)[58:cfinteractive][name:mt_cpufreq&][Power/cpufreq] @_mt_cpufreq_set_locked(): Vproc = 950mv, freq = 299000 KHz
12-19 13:36:54.653996 <3>[ 6107.783784]  (0)[577:AALServiceMain][name:aed&]AEE_MONITOR_SET[status]: 0x1
12-19 13:36:54.665189 <7>[ 6107.794977]  (0)[577:AALServiceMain][name:ddp_pwm&][PWM] (latest= 5):  1005(  63, 649) 1006(  63, 667) 1007(  63, 683) 1008(  63, 699) 1009(  63, 717)
12-19 13:36:54.688233 <7>[ 6107.818021]  (0)[102:display_esd_che][name:primary_display&][DISPCHECK][ESD]ESD check begin
12-19 13:36:54.688533 <7>[ 6107.818321]  (0)[102:display_esd_che][name:primary_display&][DISPCHECK][ESD]ESD config thread=ffffffc0540e7000
12-19 13:36:54.697916 <7>[ 6107.827704]  (0)[102:display_esd_che][name:primary_display&][DISPCHECK][ESD]_esd_check_config_handle_vdo ret=0
12-19 13:36:54.698135 <7>[ 6107.827923]  (0)[102:display_esd_che][name:primary_display&][DISPCHECK][ESD]ESD config thread done~
12-19 13:36:54.698214 <7>[ 6107.828002]  (0)[102:display_esd_che][name:ddp_dsi&][DISPCHECK][DSI]enter cmp i=0
12-19 13:36:54.698417 <7>[ 6107.828205]  (0)[102:display_esd_che][name:primary_display&][DISPCHECK][ESD]ESD check end
12-19 13:36:54.718782 <4>[ 6107.848570]  (0)[4447:kworker/0:0][name:gt9xx_driver&]<<-GTP-DEBUG->> [2174][Esd]0x8040 = 0xFF, 0x8041 = 0xAA
12-19 13:36:54.749515 <7>[ 6107.879303]  (0)[577:AALServiceMain][name:ddp_pwm&][PWM] (latest= 5):  1010(  63, 735) 1011(  63, 751) 1012(  63, 767) 1013(  63, 784) 1014(  63, 801)
12-19 13:36:54.756924 <7>[ 6107.886712]  (0)[587:AALLightSensor][name:aal_control&][ALS/AAL]Get als dat :65
12-19 13:36:54.831460 <7>[ 6107.961248]  (0)[577:AALServiceMain][name:ddp_pwm&][PWM] (latest= 5):  1015(  63, 817) 1016(  63, 834) 1017(  63, 851) 1018(  63, 867) 1019(  63, 883)
12-19 13:36:54.948260 <3>[ 6108.078048]  (0)[54:pmic_thread][name:pmic_chr_type_det&]step B2 : STANDARD CHARGER!
12-19 13:36:54.948450 <4>[ 6108.078238]  (0)[54:pmic_thread][name:usb20_phy&][MUSB]Charger_Detect_Release 666: Charger_Detect_Release
12-19 13:36:54.948494 <3>[ 6108.078282]  (0)[54:pmic_thread][name:battery_common_fg_20&][BAT_thread]Cable in, CHR_Type_num=4
12-19 13:36:54.948891 <7>[ 6108.078679]  (0)[54:pmic_thread][name:battery_meter_fg_20&][g_Get_I_Charging] gFG_current_inout_battery : 3139
12-19 13:36:54.948936 <3>[ 6108.078724]  (0)[54:pmic_thread][name:battery_common_fg_20&][kernel][battery_update] SOC 93,UI_SOC2 94, status 1
12-19 13:36:54.948973 <3>[ 6108.078761]  (0)[54:pmic_thread][name:battery_common_fg_20&][DLPT_POWER_OFF_EN] run
12-19 13:36:54.949142 <3>[ 6108.078930]  (0)[54:pmic_thread][name:battery_common_fg_20&][BATTERY] wake_up_bat. 
12-19 13:36:54.949267 <4>[ 6108.079055]  (0)[54:pmic_thread][PWRAP] clear EINT flag mt_pmic_wrap_eint_status=0x0
12-19 13:36:54.950564 <3>[ 6108.080352]  (0)[208:bat_routine_thr][name:battery_common_fg_20&][fg2.0]CUST_TRACKING_POINT: 0.
12-19 13:36:54.950614 <3>[ 6108.080402]  (0)[208:bat_routine_thr][name:battery_common_fg_20&][Battery] mt_battery_update_duration_time , last_time=6350 current_time=6357 duration=6
12-19 13:36:54.950764 <7>[ 6108.080552]  (0)[208:bat_routine_thr][name:battery_meter_fg_20&][battery_meter_driver] malloc size=16
12-19 13:36:54.951002 <3>[ 6108.080790]  (0)[208:bat_routine_thr][name:battery_common_fg_20&][BAT_thread]Cable in, CHR_Type_num=4
12-19 13:36:54.951989 <7>[ 6108.081777]  (0)[422:fuelgauged][name:battery_meter_fg_20&][fg_res] init_flag = 1
12-19 13:36:54.952385 <7>[ 6108.082173]  (0)[422:fuelgauged][name:battery_meter_fg_20&][fg_res] charge_tracking_time = 60
12-19 13:36:54.952787 <5>[ 6108.082575] -(0)[422:fuelgauged][name:mtk_rtc_hal_common&]mtk_rtc_hal_common: rtc_spare_reg[0] = {16412, 127, 8}
12-19 13:36:54.952841 <7>[ 6108.082629]  (0)[422:fuelgauged][name:battery_meter_fg_20&][fg_res] rtc_fg_soc = 93
12-19 13:36:54.953348 <7>[ 6108.083136]  (0)[422:fuelgauged][name:battery_meter_fg_20&][fg_res] charger_exist = 1
12-19 13:36:54.953740 <7>[ 6108.083528]  (0)[422:fuelgauged][name:battery_meter_fg_20&][fg_res] shutdown_system_voltage = 3385
12-19 13:36:54.954124 <7>[ 6108.083912]  (0)[422:fuelgauged][name:battery_meter_fg_20&][fg_res] update = 1
12-19 13:36:54.958901 <7>[ 6108.088689]  (0)[208:bat_routine_thr][name:battery_meter_fg_20&][g_Get_I_Charging] gFG_current_inout_battery : 3009
12-19 13:36:54.959871 <7>[ 6108.089659]  (0)[587:AALLightSensor][name:aal_control&][ALS/AAL]Get als dat :65
12-19 13:36:54.968556 <7>[ 6108.098344]  (0)[208:bat_routine_thr][name:battery_meter_fg_20&][BattVoltToTemp] 16900
12-19 13:36:54.968615 <7>[ 6108.098403]  (0)[208:bat_routine_thr][name:battery_meter_fg_20&][force_get_tbat] 672,670,1,297,10,24
12-19 13:36:54.971225 <3>[ 6108.101013]  (0)[208:bat_routine_thr][name:battery_common_fg_20&][kernel]AvgVbat 4370,bat_vol 4367, AvgI 976, I 300, VChr 4759, AvgT 24, T 24, ZCV 4331, CHR_Type 4, SOC  93: 93: 94
12-19 13:36:54.971264 <4>[ 6108.101052]  (0)[422:fuelgauged][bq24158] 
12-19 13:36:54.973957 <7>[ 6108.103745]  (0)[422:fuelgauged][name:battery_meter_fg_20&][BattVoltToTemp] 16900
12-19 13:36:54.974010 <7>[ 6108.103798]  (0)[422:fuelgauged][name:battery_meter_fg_20&][force_get_tbat] 672,669,1,301,10,25
12-19 13:36:54.974051 <7>[ 6108.103839]  (0)[422:fuelgauged][name:battery_meter_fg_20&][fg_res] temperture = 25
12-19 13:36:54.975510 <7>[ 6108.105298]  (0)[422:fuelgauged][name:battery_meter_fg_20&][fg_res] fg_current = 3005
12-19 13:36:54.975886 <7>[ 6108.105674]  (0)[422:fuelgauged][name:battery_meter_fg_20&][fg_res] current_state = 1
12-19 13:36:54.976551 <7>[ 6108.106339]  (0)[422:fuelgauged][name:battery_meter_hal&][dump_nter] mt6328_upmu_get_fg_nter_29_16 = 0x0
12-19 13:36:54.976608 <7>[ 6108.106396]  (0)[422:fuelgauged][name:battery_meter_hal&][dump_nter] mt6328_upmu_get_fg_nter_15_00 = 0xc
12-19 13:36:54.976664 <7>[ 6108.106452]  (0)[422:fuelgauged][name:battery_meter_hal&][dump_car] upmu_get_fg_car_31_16 = 0x0
12-19 13:36:54.976717 <7>[ 6108.106505]  (0)[422:fuelgauged][name:battery_meter_hal&][dump_car] upmu_get_fg_car_15_00 = 0x1f47
12-19 13:36:54.976757 <7>[ 6108.106545]  (0)[422:fuelgauged][name:battery_meter_fg_20&][fg_res] fg_coulomb = 0
12-19 13:36:54.977150 <7>[ 6108.106938]  (0)[422:fuelgauged][name:battery_meter_fg_20&][fg_res] SOC = 93
12-19 13:36:54.977517 <7>[ 6108.107305]  (0)[422:fuelgauged][name:battery_meter_fg_20&][fg_res] gFG_DOD0 = 7
12-19 13:36:54.977878 <7>[ 6108.107666]  (0)[422:fuelgauged][name:battery_meter_fg_20&][fg_res] gFG_DOD1 = 7
12-19 13:36:54.978590 <7>[ 6108.108378]  (0)[58:cfinteractive][name:mt_cpufreq&][Power/cpufreq] @_mt_cpufreq_set_locked(): Vproc = 1143mv, freq = 1300000 KHz
12-19 13:36:54.978735 <4>[ 6108.108523]  (0)[266:healthd][0x0]=0x50 [0x1]=0xf8 [0x2]=0xb6 
12-19 13:36:54.980734 <12>[ 6108.110522]  (0)[266:healthd]healthd: Unknown battery status '3'
12-19 13:36:54.981014 <4>[ 6108.110802]  (0)[266:healthd][0x3]=0x51 [0x4]=0xb [0x5]=0x4 
12-19 13:36:54.982256 <14>[ 6108.112044]  (0)[266:healthd]healthd: battery l=94 v=4371 t=25.0 h=2 st=2 chg=a
12-19 13:36:54.982409 <14>[ 6108.112197]  (0)[266:healthd]healthd: send_already = 0
12-19 13:36:54.982584 <4>[ 6108.112372]  (0)[208:bat_routine_thr][0x6]=0x7a 
12-19 13:36:54.982603 <3>[ 6108.112391]  (0)[208:bat_routine_thr][name:battery_common_fg_20&]g_gn_screenon_time=(20), g_call_state=0, g_boot_reason=0, g_boot_mode=0
12-19 13:36:54.982727 <3>[ 6108.112515]  (0)[210:mtk charger_hv_][name:battery_common_fg_20&][check_battery_exist] baton_count = 0
12-19 13:36:54.982727 <3>[ 6108.112515]  
12-19 13:36:54.995135 <3>[ 6108.124923]  (0)[210:mtk charger_hv_][name:battery_common_fg_20&][check_battery_exist] baton_count = 0
12-19 13:36:54.995135 <3>[ 6108.124923]  <7>[ 6108.128093]  (0)[210:mtk charger_hv_][name:battery_meter_fg_20&][g_Get_I_Charging] gFG_current_inout_battery : 2138
12-19 13:36:54.998324 <3>[ 6108.128112]  (0)[210:mtk charger_hv_][name:battery_common_fg_20&][gn_update_BatteryPresentCurrent] BAT_ChargerVoltage = (4749), PresentCurrent = (213)
12-19 13:36:54.999262 <3>[ 6108.129050]  (0)[208:bat_routine_thr][name:switch_charging&][BATTERY] Pre-CC mode charge, timer=0 on 0 !!
12-19 13:36:54.999262 <3>[ 6108.129050] 
12-19 13:36:55.003748 <3>[ 6108.133536]  (0)[208:bat_routine_thr][name:switch_charging&][BATTERY] select_charging_current !
12-19 13:36:55.003766 <3>[ 6108.133554]  (0)[208:bat_routine_thr][name:switch_charging&][BATTERY] Default CC mode charging : 100000, input current = 100000
12-19 13:36:55.014520 <3>[ 6108.144308]  (0)[208:bat_routine_thr][name:switch_charging&][BATTERY] pchr_turn_on_charging(), enable =1 !
12-19 13:36:55.014535 <4>[ 6108.144323]  (0)[266:healthd][bq24158] [0x0]=0x50 
12-19 13:36:55.016345 <12>[ 6108.146133]  (0)[266:healthd]healthd: Unknown battery status '3'
12-19 13:36:55.016548 <4>[ 6108.146336]  (0)[208:bat_routine_thr][0x1]=0xf8 [0x2]=0xb6 [0x3]=0x51 [0x4]=0x4b [0x5]=0x4 [0x6]=0x7a 
12-19 13:36:55.019199 <3>[ 6108.148987]  (0)[208:bat_routine_thr][name:battery_common_fg_20&][mt_kpoc_power_off_check] , chr_vol=4759, boot_mode=0
12-19 13:36:55.019215 <3>[ 6108.149003]  (0)[208:bat_routine_thr][name:battery_common_fg_20&]wait event 1
12-19 13:36:55.029915 <7>[ 6108.159703]  (0)[422:fuelgauged][name:battery_meter_fg_20&][fg_res] tracking time = 0
12-19 13:36:55.030008 <7>[ 6108.159796]  (0)[422:fuelgauged][name:battery_meter_fg_20&][fg_res] difference_voltage_update = 20
12-19 13:36:55.030103 <7>[ 6108.159891]  (0)[422:fuelgauged][name:battery_meter_fg_20&][fg_res] duration_type = 0
12-19 13:36:55.030114 <7>[ 6108.159902]  (0)[422:fuelgauged][name:battery_meter_fg_20&][fg_res] duration time = 6
12-19 13:36:55.030204 <7>[ 6108.159992]  (0)[422:fuelgauged][name:battery_meter_fg_20&][fg_res] charger_exist = 1
12-19 13:36:55.030291 <7>[ 6108.160079]  (0)[422:fuelgauged][name:battery_meter_fg_20&][fg_res] battery_full = 0
12-19 13:36:55.030377 <7>[ 6108.160165]  (0)[422:fuelgauged][name:battery_meter_fg_20&][fg_res] update = 0
12-19 13:36:55.030388 <7>[ 6108.160176]  (0)[422:fuelgauged][name:battery_meter_fg_20&][fg_res] voltage = 4370
12-19 13:36:55.030475 <7>[ 6108.160263]  (0)[422:fuelgauged][name:battery_meter_fg_20&][fg_res] UI_SOC = 93
12-19 13:36:55.030560 <7>[ 6108.160348]  (0)[422:fuelgauged][name:battery_meter_fg_20&][fg_res] UI_SOC2 = 94
12-19 13:36:55.030572 <3>[ 6108.160360]  (0)[422:fuelgauged][name:battery_common_fg_20&]******** battery : bat_update_thread_wakeup  ********
12-19 13:36:55.030832 <7>[ 6108.160620]  (0)[209:bat_update_thre][name:battery_meter_fg_20&][g_Get_I_Charging] gFG_current_inout_battery : 3015
12-19 13:36:55.030843 <3>[ 6108.160631]  (0)[209:bat_update_thre][name:battery_common_fg_20&][kernel][battery_update] SOC 93,UI_SOC2 94, status 1
12-19 13:36:55.030853 <3>[ 6108.160641]  (0)[209:bat_update_thre][name:battery_common_fg_20&][DLPT_POWER_OFF_EN] run
12-19 13:36:55.030899 <3>[ 6108.160687]  (0)[209:bat_update_thre][name:battery_common_fg_20&]wait event 2
12-19 13:36:55.045081 <14>[ 6108.174869]  (0)[266:healthd]healthd: battery l=94 v=4371 t=25.0 h=2 st=2 chg=a
12-19 13:36:55.045256 <14>[ 6108.175044]  (0)[266:healthd]healthd: send_already = 0
12-19 13:36:55.069119 <5>[ 6108.198907] -(0)[422:fuelgauged][name:mtk_rtc_hal_common&]mtk_rtc_hal_common: rtc_spare_reg[0] = {16412, 127, 8}
12-19 13:36:55.069210 <5>[ 6108.198998]  (0)[422:fuelgauged][name:battery_meter_fg_20&][fg_res] set rtc = 93
12-19 13:36:55.096930 <12>[ 6108.226718]  (0)[266:healthd]healthd: Unknown battery status '3'
12-19 13:36:55.097903 <14>[ 6108.227691]  (0)[266:healthd]healthd: battery l=94 v=4370 t=24.0 h=2 st=2 chg=a
12-19 13:36:55.105050 <14>[ 6108.234838]  (0)[266:healthd]healthd: send_already = 0
12-19 13:36:55.107420 <12>[ 6108.237208]  (0)[266:healthd]healthd: Unknown battery status '3'
12-19 13:36:55.109627 <14>[ 6108.239415]  (0)[266:healthd]healthd: battery l=94 v=4370 t=24.0 h=2 st=2 chg=a
12-19 13:36:55.109791 <14>[ 6108.239579]  (0)[266:healthd]healthd: send_already = 0
12-19 13:36:55.111303 <12>[ 6108.241091]  (0)[266:healthd]healthd: Unknown battery status '3'
12-19 13:36:55.112225 <14>[ 6108.242013]  (0)[266:healthd]healthd: battery l=94 v=4370 t=24.0 h=2 st=2 chg=a
12-19 13:36:55.112376 <14>[ 6108.242164]  (0)[266:healthd]healthd: send_already = 0
12-19 13:36:55.121748 <4>[ 6108.251536] -(1)[0:swapper/1]CPU1: Booted secondary processor
12-19 13:36:55.121770 <6>[ 6108.251558] -(1)[0:swapper/1][name:cpuinfo&]Detected VIPT I-cache on CPU1
12-19 13:36:55.121882 <6>[ 6108.251670] -(1)[0:swapper/1][name:topology&]CPU1: update cpu_capacity 1024
12-19 13:36:55.122331 <7>[ 6108.252119]  (0)[95:hps_main][name:wd_common_drv&][wdk]bind kicker thread[212] to cpu[1]
12-19 13:36:55.123291 <4>[ 6108.253079] -(2)[0:swapper/2]CPU2: Booted secondary processor
12-19 13:36:55.123309 <6>[ 6108.253097] -(2)[0:swapper/2][name:cpuinfo&]Detected VIPT I-cache on CPU2
12-19 13:36:55.123415 <6>[ 6108.253203] -(2)[0:swapper/2][name:topology&]CPU2: update cpu_capacity 1024
12-19 13:36:55.123928 <7>[ 6108.253716]  (0)[95:hps_main][name:wd_common_drv&][wdk]bind kicker thread[213] to cpu[2]
12-19 13:36:55.124908 <4>[ 6108.254696] -(3)[0:swapper/3]CPU3: Booted secondary processor
12-19 13:36:55.124929 <6>[ 6108.254717] -(3)[0:swapper/3][name:cpuinfo&]Detected VIPT I-cache on CPU3
12-19 13:36:55.125043 <6>[ 6108.254831] -(3)[0:swapper/3][name:topology&]CPU3: update cpu_capacity 1024
12-19 13:36:55.125645 <7>[ 6108.255433]  (0)[95:hps_main][name:wd_common_drv&][wdk]bind kicker thread[214] to cpu[3]
12-19 13:36:55.126228 <4>[ 6108.256016]  (0)[95:hps_main][name:mt_hotplug_strategy_algo&][HPS] (0020)(1)action end(100)(643)(0)(0) (4)(4)(4)(4)(1) (104)(12)(0) (0)(0)(0) (1)(643)(13)(0)(643) wifi_base(0)
12-19 13:36:55.127210 <12>[ 6108.256998]  (3)[266:healthd]healthd: Unknown battery status '3'
12-19 13:36:55.128577 <14>[ 6108.258365]  (3)[266:healthd]healthd: battery l=94 v=4370 t=24.0 h=2 st=2 chg=a
12-19 13:36:55.128742 <14>[ 6108.258530]  (3)[266:healthd]healthd: send_already = 0
12-19 13:36:55.129909 <12>[ 6108.259697]  (3)[266:healthd]healthd: Unknown battery status '3'
12-19 13:36:55.130858 <14>[ 6108.260646]  (3)[266:healthd]healthd: battery l=94 v=4370 t=24.0 h=2 st=2 chg=a
12-19 13:36:55.131024 <14>[ 6108.260812]  (3)[266:healthd]healthd: send_already = 0
12-19 13:36:55.132158 <12>[ 6108.261946]  (3)[266:healthd]healthd: Unknown battery status '3'
12-19 13:36:55.133459 <14>[ 6108.263247]  (3)[266:healthd]healthd: battery l=94 v=4370 t=24.0 h=2 st=2 chg=a
12-19 13:36:55.133630 <14>[ 6108.263418]  (3)[266:healthd]healthd: send_already = 0
12-19 13:36:55.134711 <12>[ 6108.264499]  (3)[266:healthd]healthd: Unknown battery status '3'
12-19 13:36:55.135667 <14>[ 6108.265455]  (3)[266:healthd]healthd: battery l=94 v=4370 t=24.0 h=2 st=2 chg=a
12-19 13:36:55.135842 <14>[ 6108.265630]  (3)[266:healthd]healthd: send_already = 0

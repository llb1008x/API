整理的各种资料文档


逻辑和文档都弄的很乱



/*1.USB&&OTG*/
{
    
--->OTG功能的检测过程:

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



--->usb设备驱动相关

1.基本概念：
	1.endpoint：端点描述符？

	2.usb_gadget:？

	3.usb descriptor:

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

	MTK平台设备树的添加
	
	
	

	2.开启OTG功能相关的配置
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


	3.相关的函数
	void mt_usb_otg_init(struct musb *musb)


3.limition
	1.该芯片不支持HNP（主机导向协议）协议，所以当A端插入时手机仅仅当作主设备，所以我们的产品并不是完全匹配OTG条例的（：OTG应该是一个双向的工作，主设备，从设备）
	2.如果插入USB设备，手机不会在USB总线上发送挂起和重启信号，直到A端拔除。

  android M limition
	1.通过OTG可以插入集线器，控制传输给多个USB设备
	2.但是不能通过简单的操作就控制U盘





	USB2.0 USB3.0协议？






}























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

	}








-->/*charging*/
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


	USB充电阶段
    do_chrdet_int_task（）充电器插入触发的中断，检测--->upmu_is_chr_det（）充电器端检测，充电器是否正常--->wakeup_fg_algo（） FG_CHARGING ,FG_RESUME
	
	唤醒电量计算，重置参数--->mt_battery_charger_detect_check()对充电器的检测--->mt_battery_type_detection()充电器类型的检测--->charging_get_charger_type
	
	通过宏传递函数指针，调用相应的函数，IC--->mt_usb_connect()如果充电器的类型是stand_host或者charging_host就连接USB--->mt_battery_update_status（）连接USB后就对电池当前的状态进行检测,
	
	通过一系列的设备节点wireless_main,ac_main,usb_main等节点写值这些上报的值进入power_supply子系统处理，power_supply_changed()改变节点的值--->回调函数。input=nl_data_handler--->
	
	bmd_ctrl_cmd_from_user,nl_send_to_user上层子系统接受到uevent事件，然后发送命令--->两层通过封装的数据结构（忘了skb？还是）这些数据都是Pmu_chargerstruct这个结构体封装完成

	从msg_fgd_cnd分离参数给pmu_hargerstruct赋值，一段很长的switch_case语句：init_flag电量计初始化是否完成，bat_is_chager_exist，baton_count三次循环检测=0，存在

	Icharging,battery_meter_get_charging_current获取电流以前是软件算法取平均，现在通过IC读取电量的值.....--->而这个上报的检测，现在通过一个healthd的进程监听pmic子系统

	的状态（监听是epoll不是socket）--->mainloop 一个while(1)循环检测，状态有变化上报状态update（）--->batterymonitor读取不同路径下各种电池属性的值--->batteryserivice java

	层，对整个android系统的电源进行管理



	

	USB充电阶段比座充阶段，应该多了USB枚举，识别的接口不一样，充电电流不一样






	座充充电阶段：














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




   

	2.power_supply子系统

	3.各种场景
		温升导致的降电流
		打电话导致的降电流
		关机充电
		OTG反向充电

    4.大的场景
	  标准充电
	  快速充电
	  无线充电
	  非标准充电
	  USB BC1.2协议


    5.移植G1605A充电代码





	


}



















-->/*MTK，QC两个平台快充的对比*/
{

	
--->MTK平台快充方案MTK  pump express：
	
	软件控制流程：
	充电器检测-->根据BC1.2协议进行充电端口检测,是否是标准的交流充电器-->电量是否大于95%-->升高充电电压电流发送一定的指令”current pattern“-->
	检查MTK识别点的电压是否有响应，有响应就继续升压，如果不能就恢复充电电压到5V-->电量达到快满电的时候应该无法进行这样的通信了
	通过发送current pattern（特定的电流波形） 控制充电电压的up down来进行通信 VBUS端，所以当充电满的时候就无法通信，不能快充

	当看门狗检测到充电器输出的电流小于130mA，时间超过240ms后，充电电压就下降到5V


	软件流程：

	battery_pump_express_charger_check()快充充电器检测-->mtk_ta_reset_vchr








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

		这两个单词都有电压的意思?
		voltage
		current


		充电的硬件保护
		1.如果充电器意外的接到了高电压，看门狗会立刻启动应急预案将电压降低到5V
		2.温度监控等相关安全措施会保护充电安全


		使用pump express 快充技术还要相关的IC，修改一点配置等
		充电的过程开始也是要通信的，建立沟通，IC发指令控制电压电流的变化
			1.快充用更大的功率，降低更多的充电时间
			2.usb设备传输的电压是有限制的，唯一的办法是提高输出电量，电源管理IC端，提高电压输出能力。
			3.大的适配器输出电量可以获得大的充电电压，降低充电时间
			4.pump express充电技术跟switch charge充电技术好像可以兼容
			5.允许充电器根据电流决定充电所需的初始电压，由PMIC发出脉冲电流指令通过USB的Vbus传送给充电器，充电器依照这个指令调变输出电压，电压逐渐增加至高达5V 达到最大充电电流。
			

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






--->高通平台快充方案QC3.0：



	一些概念：	
	{
		INOV：The POR Intelligent Negotiation for Optimum Voltage 最优电压记录智能协商方案

		POR：the Plan of Record 记录计划   记忆学习

		PMI SMB：好像是高通平台手机端和充电适配器端的IC

		HVDCP：high voltage dedicated charger port高电压可检测的充电接口

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

	paraller  charging  同步充电



	QC3.0充电协议







}








fuelgauge service














/*对硬件的操作*/
{
	底层对硬件操作最好能和芯片手册对应这看

--->1.hw_charger_type_detection底层检测充电器类型
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


--->  healthd进程负责监听底层上报的事件，uevent，periodic_chores负责将相应的事件上报给batterymonitor
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



	--->MTK不同充电算法的切换：
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



	--->快充充电算法：
		代码定义的相关的宏：CONFIG_MTK_PUMP_EXPRESS_PLUS_SUPPORT

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






}











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






座充阶段：


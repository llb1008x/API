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









}





-->/*charging*/
{
	1.充电的基本流程，电量的校准，库仑计参数，电量上报

	2.power_supply子系统

	3.各种场景
		温升导致的降电流
		打电话导致的降电流
		关机充电

    4.大的场景
	  标准充电
	  快速充电
	  无线充电
	  非标准充电
	  USB BC1.2协议


    5.移植G1605A充电代码


}

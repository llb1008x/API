/**********************************************************************************************************
								USB&&OTG相关的
**********************************************************************************************************/


{

需要注意的地方：
	1.USB眼图？
	2.OTG开关不能只开关中断，原因跟USB的热插拔相似，插入USB（插入模块，创建设备文件，节点），拔除USB（关闭中断，想上层上报信息，，关闭中断，卸载模块）。
	3.一些问题有待新版本测试，以前版本OTG和反向充电开关默认是关闭的



GNSPR#54386 	
	现象：测机连接USB至显示充电不弹选项，计算机查看无法识别USB，插拔数据线不恢复 ，重启恢复



GNSPR#57228
	现象：【自动化monkey 72h测试】手机adb连接异常，导致测试不通过。
		手机数据能连上，能充电，就是adb会断开，之前不进行monkey72h正常，但是进行了monkey之后部分机器会出现adb连击不上的
		情况
		（是否是应用软件冲突的原因-monkey72h，其他都能连接上，应该不是硬件的原因（硬件一般是什么原因会造成连接不上））	
		
		手机IMEI1和IMEI2默认设置为999999999999994（此操作目的：自动化测试服务器会把该IMEI号手机的USB调试默认为打开状态）
	
	手机adb链接出现问题的常见原因：
		adb：		
	首先adb的意思是手机打开USB调试并连接电脑后，在电脑上自动运行的一个进程，相当于驱动程序。
	出现上述情况的原因一般是驱动程序安装错误或者电脑及手机上有其他程序发生了冲突


GNSPR #53212 	 OTG：连接2A旅充头，简单操作测机10min，未充进电，充电前电量显示57%，充电后点电量显示57%（未做过大量重启、开关机操作）【随机一次】A29



GNSPR #52967	 OTG：G1605手机插上一拖一反充线给M6手机反充电，充电10分钟后，查看充电电流，其中有值低于800mA（附图）。



GNSPR #53732 	 OTG：设置中反向充电和ＭＭＩ反充充电，切换后，设置中开启反向充电和ＭＭＩ里面都不能反向充电。


这两个问题都是充电频繁断开，跟OTG底层接口有关
GNSPR #52977	 OTG：测试机G1605给另一个G1605和S8充电，两台被充电的手机一直很频繁充电/不充电（附视频）
GNSPR #52885	 OTG：使用G1605给S8反向充电，频繁充电过程出现中断，但很快又连接上的问题（附视频）


OTG失败，USB也不显示，USB五个引脚的作用？


GNSPR#53131
			现象：关机状态连充电器，长按电源键3s开机，10s后才有振动提示和开机画面 对比S9无【6台必现】A9

	这个问题跟平台有一定的关系
MTK提供的解决方法
	1.方案一：这种方法在于在握手之前先判断是否是长按了音量下（强制刷机）,个人觉得这种方案是可以的，刷机本来就是用组合键
	   Sure, you can add keypad check function before usb_handshake() as you point out.
	We already provided the keypad check API in preloader. For example, you can use: mtk_detect_key (MTK_PMIC_RST_KEY)
	We have found the cause as below. We will commit it soon, you can also take a try.
	bootable/bootloader/preloader / platform/mt6735/src/drivers/pmic.c
	Only when KCOL is enable(force download), it is possible to use the MTK download tool.
	We will modify that keypad check for mtk download tool is disable. (CFG_USB_DOWNLOAD :=0 / CFG_EMERGENCY_DL_SUPPORT :=0)
	Please modify that USB handshake function is possible when keypad is pressed,
	For example
		    #if CFG_USB_TOOL_HANDSHAKE
		    (Add keypad check function)
		    if (TRUE == usb_handshake(handler))
		           g_meta_com_type = META_USB_COM;
		           BOOTING_TIME_PROFILING_LOG("USB handshake");
		    #endif
		    #if CFG_
	disable “CFG_USB_TOOL_HANDSHAKE”, the above problems will not exist anymore.
	But you will not be able to use USB download and USB Meta tool via preloader.
	If you think that is ok, we don’t have other concerns.

	cust_bldr.mak

	alps\bootable\bootloader\preloader\custom\[project]\cust_bldr.mak
	CFG_UART_TOOL_HANDSHAKE :=0


		cust_bldr.mak:
		CFG_UART_TOOL_HANDSHAKE :=0

	default.mak
		MTK_PMIC_RST_KEY :=1

	main.c

	    #if CFG_USB_TOOL_HANDSHAKE
	    if( mtk_detect_key (MTK_PMIC_RST_KEY) ){
		if (TRUE == usb_handshake(handler))
                g_meta_com_type = META_USB_COM;
		    BOOTING_TIME_PROFILING_LOG("USB handshake");
	    }
	   /*GioneeDrv LiLuBao 20161121 modify for fixed GNSPR#53131 end
        #endif


	2.方案二：修改检测延迟的时间timeout，不见效
	修改usb tool hanshake timour的时间为1s，这个修改可能是影响tool的连接使用，所以建议修改不能比1s少
	这里设置为1s
	#if !defined(CFG_USB_HANDSHAKE_TIMEOUT)
	-#define CFG_USB_HANDSHAKE_TIMEOUT (2500) /* 2.5s */
	+#define CFG_USB_HANDSHAKE_TIMEOUT (1000) /* 1s */
	#endif
	b>去掉uart hanshake
	/vendor/mediatek/proprietary/bootable/bootloader/preloader/platform/mt6755/default.mak
	CFG_UART_TOOL_HANDSHAKE :=0
	c>把dlpt初始化的位置挪到后面（从preloader移到lk阶段，但是刷机延迟时间就长了）

	#define CFG_USB_ENUM_TIMEOUT            (8000)           /* 8s */
	USB枚举时间超时，改变之后影响座充充电时间

	#define CFG_USB_HANDSHAKE_TIMEOUT       (2500)    	 /* 2.5s */
	USB握手时间超时，改变之后影响了用USB充电的时间

	3.方案三：
	将dlpt_init_inLK()移动到如下，在6s左右:如上传log：session--usb-44444.log
	#ifdef LK_PROFILING
	dprintf(CRITICAL, "[PROFILE] ------- show logo takes %d ms --------
	", (int)get_timer(time_show_logo));
	#endif
	/*add bebug by mtk ALPS02765977 20160624*/
	dlpt_init_inLK();


FAQ18845
	[REPRODUCTIVE]
          1, 6735/6753/6735M 手机
          2， 手机USB线连接 AC Adapter 充电器
          3， 查看uart log
 
          抓取uart log 看到：
        force STANDARD_HOST

		[PLFM] USB cable in
		[14:49:50][pmic_IsUsbCableIn] 1
		[14:49:51][pmic_IsUsbCableIn] 1
		pl pmic powerkey Release
		[14:49:52][pmic_IsUsbCableIn] 1
		pl pmic powerkey Release
		[14:49:53][pmic_IsUsbCableIn] 1
		pl pmic powerkey Release
		[14:49:54][pmic_IsUsbCableIn] 1
		pl pmic powerkey Release
		[14:49:55][pmic_IsUsbCableIn] 1
		pl pmic powerkey Release
		[14:49:56][pmic_IsUsbCableIn] 1
		pl pmic powerkey Release
		[14:49:57][pmic_IsUsbCableIn] 1
		pl pmic powerkey Release
 
[ANALYSIS]          
          为减小preloader size 默认强制USB 类型赋值为 STANDARD_HOST ，此情况下会去执行USB 枚举。 但是实际上是AC Adapter，因此会不断的枚举直到超时。枚举时间正好是8S。
 
[SOLUTION]
          /vendor/mediatek/proprietary/bootable/bootloader/preloader/platform/mt6735/src/drivers/pmic.c
           

118#ifndef PMIC_TYPE_DETECTION
119
120int hw_charger_type_detection(void)
121{
122    print("force STANDARD_HOST\r\n");
123    return STANDARD_HOST;
124}

      在make file 中定义 PMIC_TYPE_DETECTION 宏即可。
 
      如果开启该宏后引起preloader size 过大导致build 失败或者下载失败，则请参考FAQ15079 FAQ11794  处理。






GNSPR#70216
	mmi测试导致的充电失败
	
	OTG开关的工作流程：
	(battery_common_fg_20.c)show_otg_charge_switch_State,底层驱动提供给上层的节点，上层写值后通过调用这里向下调用，有两个状态位
	gn_otg_charge_switch_State，otg_charge_state；判断接受到的otg_charge_state状态是否为1，与gn_otg_charge_switch_State状态是否一样
	->(usb20_host.c)打开还是关闭这里的open和close函数之重点,关于中断使能，控制逻辑还要理解->mt_usb_ext_iddig_int当有otg中断来的时候
	调用musb_id_pin_work,对应的工作队列 ->musb_is_host判断插入的usb是host还是devices->musb_start配置一些usb传输的信息，改变usb状态信息，
	将usb主设备转变成从设备->switch_int_to_device设置iddig的中断使能，完成转变成从设备->工作队列任务完成
	
	为什么要设置vbus
	
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

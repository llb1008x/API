

/*********************************************************************************************/
1.bringup
{
	vendor/mediatek/proprietary/custom/gnbj6763_66_n1/kernel/dct/dct/codegen.dws
	vendor/mediatek/proprietary/bootable/bootloader/preloader/custom/gnbj6763_66_n1/dct/dct/codegen.dws
	vendor/mediatek/proprietary/bootable/bootloader/lk/target/gnbj6763_66_n1/dct/dct/codegen.dws
	kernel-4.4/drivers/misc/mediatek/dws/mt6763/gnbj6763_66_n1.dws
	
	gpio:
	 drv_vbus
	 usb_id		AA27			
	 gpio_chg_en
	 eint_chg_stat
	还差一个霍尔元件搜kpd.c  flip
	
	eint:
		
	
	I2C:
		
		modify:
		rt6370_pmu 	channel_5 appm,0x34			3400Kbits->400Kbits
		EINT41				rt5081_pd			OTG_IDDIG
		EINT91				RT5081_PMU_	EINT	RT6370_PMU_EINT
		

 	cpu :mt6763
	pmic:mt6356
	charger:mt6370
	 
	{
	17G10A
		先搜AndroidProducts.mk：定义一个变量——>PRODUCT_MAKEFILES，该变量的值为产品版本定义文件名的列表  
        产品版本定义文件：对特定产品版本的定义（可多个文件，多个版本）。一般情况下，我们不需要定义所有变量（版本相关），，Build系统已经预先定义了一些组合，
		位于build/target/product，该目录下每个文件都定义了一个组合，我们只需要继承这些预置的定义，然后再覆盖自己想要的变量定义即可
        BroadConfig.mk：该文件用来配置硬件主板，它定义的都是设备底层的硬件特性，如设备的主板相关信息，wifi，bootloader，内核等

		最后确定thermal 用的gionee_bj这个目录，之前一直以为是mediatek mt6757目录下面
		而且编译要全编
		
		The I 2 C interface bus must be connect a resistor 2.2kΩ to power node and independent connection to processor. The
		I 2 C interface also supports High-Speed (HS) mode for transfer up to 3.4Mbits. If don't use USB_PD function that allow to
		use 400kbits to write or read the bit.	
	
		
		preloader,lk ,kernel的宏开关
		{
			vendor/mediatek/proprietary/bootable/bootloader/preloader/custom/gnbj6763_66_n1/gnbj6763_66_n1.mk
			vendor/mediatek/proprietary/bootable/bootloader/lk/project/gnbj6763_66_n1.mk
		
			lk
				MTK_MT6370_PMU_CHARGER_SUPPORT := yes
				MTK_CHARGER_INTERFACE := yes
				MTK_MT6370_PMU_CHARGER_SUPPORT
				SWCHR_POWER_PATH
			
			kernel
				GN_MTK_BSP_HALL_KEY_SUPPORT	

		}
		
	
		//Gionee <GN_BY_CHG> <lilubao> <20171223> add for platform change begin

		modify：
			preloader ,lk修改了dws 关于gpio，eint,i2c的配置
			lk  gnbj6763_66_n1.mk 注掉MTK_PUMP_EXPRESS_PLUS_SUPPORT := yes
		
			kernel
				1.mt6370.dtsi 的gpio	
				mt6370,intr_gpio_num = <91>; /* direct defined GPIO num */
				mt6370,intr_gpio = <&pio 91 0x0>; /* GPIO */
				
				2.注掉了快充相关的几个宏开关	
				CONFIG_MTK_PUMP_EXPRESS_PLUS_SUPPORT=y
				CONFIG_MTK_PUMP_EXPRESS_PLUS_20_SUPPORT=y
			
			霍尔开关
				1.使用了eint2 gpio2 	修改了三个阶段的dws文件
				2.mt6763.dtsi添加一个flip节点
				3.添加霍尔开关相关的代码 kpd.c
				GN_MTK_BSP_HALL_KEY_SUPPORT	
					但是有些是用在双屏的代码，只需要知道状态
					s_gn_clam_switched
					现在首先要确定的是可以出发霍尔开关gpio的状态可以读出来
					现在状态是有变化的
					
		
	
	}
	
	
	代码和相关的文档
	{
		GM3.0相关
		mtk_battery.c，mtk_gauge_class.c,mt6355_gauge.c
		kernel-4.4/drivers/power/mediatek/mtk_battery.c
		kernel-4.4/drivers/misc/mediatek/pmic/mtk_gauge_class.c
		kernel-4.4/drivers/misc/mediatek/pmic/mt6355/v1/mt6355_gauge.c
		
		preloader
			is_battery_exist = hw_check_battery(); 定义了MTK_DISABLE_POWER_ON_OFF_VOLTAGE_LIMITATION
			所以直接返回了ignore bat check 不检测电池 (就是检测PMIC_RG_LDO_VBIF28_EN_ADDR ，ldo28接到bat的)
			
			fuel gauge 是否复位过
			[fg_init] fg_reset_status 0 do_init_fgadc_reset 1 fg_curr_time 1 shutdown_pmic_time 1 hw_id 0x5630 sw_id 0x5630, 4068 0 0x1 0x2329 1 0
			
			
			plcharg_status 阶段是否充电
			plcharg_status = upmu_is_chr_det();
			print("[fg_init] fg_reset_status %d do_init_fgadc_reset %d fg_curr_time %d shutdown_pmic_time %d hw_id 0x%x sw_id 0x%x, %d %d 0x%x 0x%x %d %d\n",
			fg_reset_status, do_init_fgadc_reset, fg_curr_time, shutdown_pmic_time, hw_id, sw_id,
			boot_vbat, shutdowntime, reset_sel, slp_en, b_moniter_pl_charg_bit, plcharg_status);
			

			1.判斷電池是否存在
			2.對Gauge hw進行init
			3.判斷gauge是否被reset過( 判斷是否曾拔過電池 )
			4.讀取開機電壓 boot_vbat
			5.讀取關機時間 shutdowntime
			6.判斷有無發生2sec reboot
			7.Preloader init順序有dependency,請勿更動init順序
			
		lk
			(platform.c) platform_init -> (mt_battery.c) mt65xx_bat_init  -> 
			lk阶段会停止充电，关闭power path算出电池电阻还有ocv
			
			
			
		kernel
			(mtk_battery.c) battery_init 建立netlink handler 用于数据交互
			daemo_nl_sk = netlink_kernel_create(&init_net, NETLINK_FGD, &cfg);
			然后加载dts，初始化driver
			
			mtk_power_misc.c 有处理shutdown的时间，fuel guage 负责上报时间uevent，然后有这里的线程负责
			处理关机事件 
			pmic_throttling_dlp 低电保护，set_shutdown_cond
			
			
			
	}




	log
	{
		[PMIC]disable usbdl wo battery
		
		[PMIC]init_setting
		[PMIC] init setting date: 2017-05-10, Done
		
		power key is pressed
		ignore bat check
		[PLFM] Power key boot!
		[PMIC]POWER_HOLD ON
		[PMIC]PowerHold = 0x1
		[pl_battery_init] is_fg_init:0 , force_init:0 bat:1
		
		
		usb握手
		[LIB] seclib_img_auth_load_sig [LIB] CFG read size '0x2000' '0x3C'
		[LIB] SEC CFG doesn't exist
		[SEC] init fail '0x3000'
		[BLDR] Starting tool handshake.
		€€€€€€€€€€€[MT6356] get volt 2, 48, 800000
		[BLDR] Tool connection is unlocked
		[platform_vusb_on] VUSB33 is on
		[PMIC]IsUsbCableIn 0
		[TOOL] PMIC not dectect usb cable!
		
		platform 
		[25] initializing platform
		[25] platform_init()
	
		
		rt5081
		[275] [DISP]func|disp_lcm_init
		[275] [DISPCHECK]lcm init_power 
		lcm_init_power:lk
		[276] [DISPCHECK]lcm init 
		[276] liuyuntao [LCD] lcm_init 
		[291] liuyuntao [LCD] power_on_rt5081 
		
		
		logo
		[863] [lk logo: mt_disp_show_boot_logo 131]
		[863] [lk logo: init_fb_screen 59]
		[864] mt_get_logo_db_addr: 0x5e900000
		[864] [lk logo: init_fb_screen 77]MTK_LC
		
		mt_charger
		[1092] mt_charger_dump_register: ICHG = 2000mA, AICR = 500mA, MIVR = 4500mV, IEOC = 250mA
		[1093] mt_charger_dump_register: CHG_EN = 1, CHG_STATUS = ready
		[1094] [mt65xx_bat_init] check VBAT=4039 mV with 3450 mV
		
		[1098] [BATTERY] No battry plug-in. Power Off.[1099] mt6370_i2c_read_byte: I2CR[0x12] = 0x0B
		
		[    0.967642] <1>.(4)[1:swapper/0][name:mtk_wdt&]mtk_wdt_init ok
		
		
		[    2.548796] <4>.(4)[1:swapper/0][name:mtk_chg_type_det&]mt_charger_probe
		
		[    2.933065] <5>.(5)[1:swapper/0][name:bootprof&]BOOTPROF:      2933.065006:initcall: pmic_mt_init    69.600000ms
		[    2.934433] <5>.(5)[1:swapper/0][name:usb20&][MUSB]usb20_init 1658: usb20 init
		[    2.936973] <5>.(5)[1:swapper/0][name:usb20&][MUSB]mt_usb_probe 1578: init connection_work
		[    2.938001] <5>.(5)[1:swapper/0][name:usb20&][MUSB]mt_usb_probe 1581: keep musb->power & mtk_usb_power in the samae value
		[    2.939792] <5>.(5)[1:swapper/0][name:usb20&][MUSB]mt_usb_probe 1609: USB probe done!
		[    2.942409] <5>.(5)[1:swapper/0][name:usb20&][MUSB]usb20_init 1673: usb20 init ret:0
		
		
		[    3.560140] <5>.(5)[1:swapper/0][name:mt6356_gauge&]mt6356_gauge_probe: starts
		[    3.561043] <5>.(5)[1:swapper/0][name:mt6356_gauge&]mt6356_parse_dt: starts
		
		
		[    3.992063] <5>.(5)[1:swapper/0][name:bootprof&]BOOTPROF:      3992.063624:initcall: imgsensor_init    38.000692ms
		[    4.005208] <5>.(5)[1:swapper/0][name:pmic_throttling_dlpt&][register_low_battery_notify] prio_val=5
		[    4.006346] <5>.(5)[1:swapper/0][name:pmic_throttling_dlpt&][register_battery_percent_notify] prio_val=5
		[    4.007525] <5>.(5)[1:swapper/0][name:pmic_throttling_dlpt&][register_battery_oc_notify] prio_val=5
		
		
		
		[    4.602833] <5>.(5)[1:swapper/0]mt6370_pmu_core mt6370_pmu_core: mt6370_pmu_core_probe successfully
		[    4.605053] <5>.(5)[1:swapper/0][name:mt6370_pmu_charger&]mt6370_pmu_charger_probe: (1.1.22_MTK)
		[    4.606175] <5>.(5)[1:swapper/0]mt6370_pmu_charger mt6370_pmu_charger: mt_parse_dt
		[    4.607126] <5>.(5)[1:swapper/0]mt6370_pmu_charger mt6370_pmu_charger: mt_parse_dt: no chg alias name
		[    4.608296] <5>.(5)[1:swapper/0]mt6370_pmu_charger mt6370_pmu_charger: mt_parse_dt: no ls alias name
		
		[    4.829488] <5>.(5)[1:swapper/0][name:mtk_battery&]******** battery_dts_probe!! ********
		[    4.832300] <5>.(5)[1:swapper/0][name:mtk_rtc_hal_common&]mtk_rtc_hal_common: rtc_spare_reg[14] = {a24, 255, 8}
		
	
		
		[    3.611478] <5>.(5)[1:swapper/0][name:usb20_host&][MUSB]mt_usb_otg_init 749: host controlled by TYPEC
		[    3.614049] <1>.(1)[6:kworker/u16:0][name:usb20_host&][MUSB]do_register_otg_work 59: get type_c_port0 fail
		
		
	}


}


	

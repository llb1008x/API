




17G10A
{
	GNSPR#94168,100297
	{
		94168
		【品质压力】MMI测试，后摄像头拍照，照片显示黑色，再次拍照恢复【验证3台，2台出现3次】
	
	
	
	
		100297
		【品质压力】桌面进相机，前摄像头开闪光灯，按拍照键单拍照片后进小图库查看，照片显示全黑，再次拍照恢复【验证2台1台出现2次】A16
	
	
		case ID:alps03515512
	
		Dear customer:
		1.请问贵司在设置 预闪 和 主闪 时间的时候是以什么为依据的？
		-----------------闪光灯预闪和主闪的时间主要是根据3A HAl自己决定的，在precapture state 的时候进行预闪，并且计算flash ae/AWB；然后在capture state 状态会打主闪
		2.主闪的时候电流会比较大，请问贵司时候在设置主闪时间的时候，是否会参考电源电量等相关的数据？
		-----------------正常情况下设置主闪的duty是不会参考电源电量等相关的数据的，主闪的duty主要preflash 计算出来的；但是有两个地方回去检测电池电量，然后对应调整flash的行为：
		1)在flash_mgr_m.cpp文件中会通过g_pStrobe->isLowPower(&isLow);接口去获取当前的电池电量，如果为低电量会关闭闪光灯
		2)PMIC也会通过中断的方式定时检测低电量，如果为低电量就会关闭闪光灯！
	
	
	
	
		该问题在系统相机中也存在，且电量较低的时候出现概率更高。
		引起该现象的原因是：闪光灯打闪的时候AE会设置得很低，然后抓取拍照帧，概率性出现抓取的时候主闪已结束，但AE还没设置回来，导致照片整体偏暗（如果微距拍摄会误认为是全黑）。
		我在闪光灯驱动的disable函数中加log ,发现当出现该现象的时候，被PMIC线程切断了，导致闪光灯主闪持续时间不够长：
		05-03 04:35:17.162253 0 0 I [ 2567.040248] (2)[9632:Cam@P1NodeImp][name:flashlights_rt5081&]: [FLASHLIGHT] rt5081_disable: shenweikun rt5081_disable 《====== 正常log
		05-03 04:35:17.337183 0 0 I [ 2567.215178] (0)[9632:Cam@P1NodeImp][name:flashlights_rt5081&]: [FLASHLIGHT] rt5081_disable: shenweikun rt5081_disable
		05-03 04:35:22.976299 0 0 I [ 2572.854294] (1)[9632:Cam@P1NodeImp][name:flashlights_rt5081&]: [FLASHLIGHT] rt5081_disable: shenweikun rt5081_disable
		05-03 04:35:23.151764 0 0 I [ 2573.029759] (1)[9632:Cam@P1NodeImp][name:flashlights_rt5081&]: [FLASHLIGHT] rt5081_disable: shenweikun rt5081_disable
		05-03 04:35:23.152745 69 69 I [ 2573.030740] (0)[69:pmic_thread][name:flashlights_rt5081&]: [FLASHLIGHT] rt5081_disable: shenweikun rt5081_disable 《====== 异常log
		05-03 04:35:28.759604 0 0 I [ 2578.637599] (3)[9632:Cam@P1NodeImp][name:flashlights_rt5081&]: [FLASHLIGHT] rt5081_disable: shenweikun rt5081_disable
		05-03 04:35:28.932398 0 0 I [ 2578.810393] (2)[9632:Cam@P1NodeImp][name:flashlights_rt5081&]: [FLASHLIGHT] rt5081_disable: shenweikun rt5081_disable
		05-03 04:35:37.408493 0 0 I [ 2587.286488] (2)[9632:Cam@P1NodeImp][name:flashlights_rt5081&]: [FLASHLIGHT] rt5081_disable: shenweikun rt5081_disable
		05-03 04:35:37.584241 0 0 I [ 2587.462236] (2)[9632:Cam@P1NodeImp][name:flashlights_rt5081&]: [FLASHLIGHT] rt5081_disable: shenweikun rt5081_disable
		05-03 04:35:43.465451 0 0 I [ 2593.343446] (3)[9632:Cam@P1NodeImp][name:flashlights_rt5081&]: [FLASHLIGHT] rt5081_disable: shenweikun rt5081_disable
		05-03 04:35:43.645849 0 0 I [ 2593.523844] (2)[9632:Cam@P1NodeImp][name:flashlights_rt5081&]: [FLASHLIGHT] rt5081_disable: shenweikun rt5081_disable
		05-03 04:35:51.008684 0 0 I [ 2600.886679] (2)[9632:Cam@P1NodeImp][name:flashlights_rt5081&]: [FLASHLIGHT] rt5081_disable: shenweikun rt5081_disable
		05-03 04:35:51.220072 0 0 I [ 2601.098067] (4)[9632:Cam@P1NodeImp][name:flashlights_rt5081&]: [FLASHLIGHT] rt5081_disable: shenweikun rt5081_disable
		05-03 04:35:56.661642 0 0 I [ 2606.539637] (3)[9632:Cam@P1NodeImp][name:flashlights_rt5081&]: [FLASHLIGHT] rt5081_disable: shenweikun rt5081_disable
		请电源的同事帮忙看一下。
	
	
	
	
		//disable flashlight
		09-19 16:45:20.805227    70    70 E [  118.092780] (0)[70:pmic_thread][name:pmic_irq&]: [PMIC] 1111111111
		09-19 16:45:20.805250    70    70 E [  118.092803] (0)[70:pmic_thread][name:pmic_irq&]: [PMIC] 0
		09-19 16:45:20.805262    70    70 E [  118.092815] (0)[70:pmic_thread][name:pmic_irq&]: [PMIC] 1
		09-19 16:45:20.805274    70    70 E [  118.092827] (0)[70:pmic_thread][name:pmic_irq&]: [PMIC] 2
		09-19 16:45:20.805285    70    70 E [  118.092838] (0)[70:pmic_thread][name:pmic_irq&]: [PMIC] 3
		09-19 16:45:20.805297    70    70 E [  118.092850] (0)[70:pmic_thread][name:pmic_irq&]: [PMIC] 4
		09-19 16:45:20.805308    70    70 E [  118.092861] (0)[70:pmic_thread][name:pmic_irq&]: [PMIC] 5
		09-19 16:45:20.805319    70    70 E [  118.092872] (0)[70:pmic_thread][name:pmic_irq&]: [PMIC] 6
		09-19 16:45:58.761135    70    70 E [  156.048688] (7)[70:pmic_thread][name:pmic_irq&]: [PMIC] in [pmic_thread_kthread] by lilubao
		09-19 16:45:58.761172    70    70 E [  156.048725] (7)[70:pmic_thread][name:pmic_irq&]: [PMIC] [PMIC_INT] addr[0x856]=0x8
		09-19 16:45:58.761257    70    70 D [  156.048810] (7)[70:pmic_thread][name:mtk_ppm_main&]: [Power/PPM] (0x23c)(610)(1)[4L_LL]: (10)(10)(3)(4) (15)(10)(4)(4)
		09-19 16:45:58.761670    70    70 E [  156.049223] (7)[70:pmic_thread][name:flashlight&]: [FLASHLIGHT] pt_trigger: PT disable flashlight: (1,0,0)
		09-19 16:45:58.761704    70    70 E [  156.049257] (7)[70:pmic_thread][name:flashlights_rt5081&]: [FLASHLIGHT] rt5081_ioctl: FLASH_IOC_SET_ONOFF(0): 0
		09-19 16:45:58.761717    70    70 E [  156.049270] (7)[70:pmic_thread][name:flashlights_rt5081&]: [FLASHLIGHT] rt5081_operate: in [rt5081_operate] by lilubao

										fl_info("PT disable flashlight: (%d,%d,%d)\n",pt_low_vol, pt_low_bat, pt_over_cur);
		09-19 16:45:58.761727    70    70 E [  156.049280] (7)[70:pmic_thread][name:flashlight&]: [FLASHLIGHT] pt_trigger: PT disable flashlight: (1,0,0)

		09-19 16:45:58.761841    70    70 E [  156.049394] (7)[70:pmic_thread][name:flashlights_rt5081&]: [FLASHLIGHT] rt5081_ioctl: FLASH_IOC_SET_ONOFF(1): 0
		09-19 16:45:58.761851    70    70 E [  156.049404] (7)[70:pmic_thread][name:flashlights_rt5081&]: [FLASHLIGHT] rt5081_operate: in [rt5081_operate] by lilubao
		09-19 16:45:58.761858    70    70 E [  156.049411] (7)[70:pmic_thread][name:flashlights_rt5081&]: [FLASHLIGHT] rt5081_operate: in [rt5081_operate] by lilubao
		09-19 16:45:58.761865    70    70 E [  156.049418] (7)[70:pmic_thread][name:flashlights_rt5081&]: [FLASHLIGHT] rt5081_operate: 11111111111111
		09-19 16:45:58.761872    70    70 E [  156.049425] (7)[70:pmic_thread][name:flashlights_rt5081&]: [FLASHLIGHT] rt5081_disable: in [rt5081_disable] by lilubao
		09-19 16:45:58.762223    70    70 I [  156.049776] (7)[70:pmic_thread]: rt5081_pmu_fled rt5081_pmu_fled.0: set to off mode
		09-19 16:45:58.762342    70    70 I [  156.049895] (7)[70:pmic_thread]: rt5081_pmu_fled rt5081_pmu_fled.1: set to off mode
		09-19 16:45:58.762391    70    70 D [  156.049944] (7)[70:pmic_thread][name:kd_camera_hw&]: [kd_camera_hw]set_camera_flash_torch : on=0
		09-19 16:45:58.762415    70    70 E [  156.049968] (7)[70:pmic_thread][name:flashlight&]: [FLASHLIGHT] flashlight_machine_ops: new_state_0x50, ret=0(allowed:0)
	
	
		//Gionee <GN_BSP_CHG> <lilubao> <20170919> add for debug begin
		fl_err("in [%s] by lilubao\n",__FUNCTION__);
		//Gionee <GN_BSP_CHG> <lilubao> <20170919> add for debug end
	
	
		./drivers/misc/mediatek/flashlight/flashlights-rt5081.c
		rt5081_disable ->
		{
			1.rt5081_release -> rt5081_torch_store 
			-> 这个最后是读写闪光灯节点来打开或者关闭闪光灯
		
			2.rt5081_work_disable_ch1 -> INIT_WORK(&rt5081_work_ch1, rt5081_work_disable_ch1)
			-> rt5081_timer_func_ch1 -> rt5081_timer_start -> rt5081_operate 这边是对闪光灯的一些操作
		
			使能，只亮一个，前置后置闪光灯定时只亮一个，应该是拍照的自动打开闪光灯
		
			->创建了两个工作队列
		}
	
	
		./drivers/misc/mediatek/pmic/mt6355/v1/pmic_irq.c
		pmic_thread_kthread ->
		{
			[PMIC_INT] addr[0x856]=0x8
		
			struct pmic_interrupts interrupts[] = {
		PMIC_M_INTS_GEN(MT6355_INT_STATUS0, MT6355_INT_RAW_STATUS0,			//0x0854
				MT6355_INT_CON0, MT6355_INT_MASK_CON0, interrupt_status0),	//0x0856
		PMIC_M_INTS_GEN(MT6355_INT_STATUS1, MT6355_INT_RAW_STATUS1,
				MT6355_INT_CON1, MT6355_INT_MASK_CON1, interrupt_status1),
		PMIC_M_INTS_GEN(MT6355_INT_STATUS2, MT6355_INT_RAW_STATUS2,
				MT6355_INT_CON2, MT6355_INT_MASK_CON2, interrupt_status2),
		PMIC_M_INTS_GEN(MT6355_INT_STATUS3, MT6355_INT_RAW_STATUS3,
				MT6355_INT_CON3, MT6355_INT_MASK_CON3, interrupt_status3),
		PMIC_M_INTS_GEN(MT6355_INT_STATUS4, MT6355_INT_RAW_STATUS4,
				MT6355_INT_CON4, MT6355_INT_MASK_CON4, interrupt_status4),
		PMIC_M_INTS_GEN(MT6355_INT_STATUS5, MT6355_INT_RAW_STATUS5,
				MT6355_INT_CON5, MT6355_INT_MASK_CON5, interrupt_status5),
		PMIC_M_INTS_GEN(MT6355_INT_STATUS6, MT6355_INT_RAW_STATUS6,
				MT6355_INT_CON6, MT6355_INT_MASK_CON6, interrupt_status6),
		};
	
	
			09-19 16:46:07.209264    70    70 E [  165.765006] (0)[70:pmic_thread][name:pmic_irq&]: [PMIC] [PMIC_INT] addr[0x856]=0x200
		
		
		}
	

	
		低电条件下电池电压低，闪光灯瞬间的大电流会把电池电压拉到很低，触发了pmic的中断，
		跟哪些中断有关？这些中断是干什么的？设定的阈值是多少？是否能修改？如何修改的合理？
	
	
		1.RG_INT_STATUS_NAG_C_DLTV:这个中断应该是检测电池电压如果短时间内电池电压变化如果超过了阈值
		2.还有一个可能跟低电保护有关，开闪光灯电池电压可以拉低0.4V
		3.bat_temp_l,bat_h有几个相关的参数在mtk_battery_property.h文件内设置的
		最后确定的中断是bat_l_int_handler
		/* ex. 3400/5400*4096*/
		#define BAT_HV_THD   (POWER_INT0_VOLT*4096/5400)	/*ex: 3400mV */
		#define BAT_LV_1_THD (POWER_INT1_VOLT*4096/5400)	/*ex: 3250mV */
		#define BAT_LV_2_THD (POWER_INT2_VOLT*4096/5400)	/*ex: 3000mV */
	
	
	
		(flashlight-core.c)pt_trigger
		fl_info("PT disable flashlight: (%d,%d,%d)\n",
									pt_low_vol, pt_low_bat, pt_over_cur);
	
	
		09-19 16:45:58.761727    70    70 E [  156.049280] (7)[70:pmic_thread][name:flashlight&]: [FLASHLIGHT] pt_trigger: PT disable flashlight: (1,0,0)
	
		确定是low battery，pt_low_vol = LOW_BATTERY_LEVEL_0;
	
	
		pmic_throttling_dlpt 低电保护线程
	
	
	
		pt_trigger -> pt_low_vol_callback -> flashlight_pt_store -> register_low_battery_notify(&pt_low_vol_callback, LOW_BATTERY_PRIO_FLASHLIGHT);
	
		 
		-> exec_low_battery_callback(LOW_BATTERY_LEVEL_0) -> bat_l_int_handler 
	
		typedef enum LOW_BATTERY_PRIO_TAG {
			LOW_BATTERY_PRIO_CPU_B = 0,
			LOW_BATTERY_PRIO_CPU_L = 1,
			LOW_BATTERY_PRIO_GPU = 2,
			LOW_BATTERY_PRIO_MD = 3,
			LOW_BATTERY_PRIO_MD5 = 4,
			LOW_BATTERY_PRIO_FLASHLIGHT = 5,
			LOW_BATTERY_PRIO_VIDEO = 6,
			LOW_BATTERY_PRIO_WIFI = 7,
			LOW_BATTERY_PRIO_BACKLIGHT = 8
		} LOW_BATTERY_PRIO;


		闪光灯有双闪，哪双闪？电流分别是多少？开启闪光灯的时间大概是多少？怎么修改低电保护延迟的判断时间？

		而低电量开启闪光灯，电流很大，rt5081要输出很大的功率，但是低电条件下输入能力不足，导致电池电压被拉的很低，
	
	
		(pmic_throttling_dlpt.c) dlpt_check_power_off
	
	
		modify test
		{
			修改FLED_VMID是0x7A的bit[5:0]，目前寄存器默认是5V（bit[5:0]=110111）。

			请按建议修改，试试看

			FL_VMID=4.0V（bit[5:0]=001111），FL_VMID=4.1V（bit[5:0]=010011），FL_VMID=4.2V（bit[5:0]=010111），FL_VMID=3.9V（bit[5:0]=001011）.
	
			(rt5081.dtsi)
	
				rt5081_pmu_fled1 {
				compatible = "richtek,rt5081_pmu_fled1";
				interrupt-names = "fled_lvf", "fled2_short",
						  "fled1_short";
				fled_enable = <1>;
				torch_cur = <300000>;  /*25000 ~ 400000 uA */
				strobe_cur = <1200000>; /* 100000 ~ 1500000 uA */
				strobe_timeout = <2400>; /* 64 ~ 2432 ms */
			};
			rt5081_pmu_fled2 {
				compatible = "richtek,rt5081_pmu_fled2";
				fled_enable = <1>;
				torch_cur = <200000>;
				strobe_cur = <1000000>;
				strobe_timeout = <1200>;
	
	
	
	
	
			(rt5081_pmu_fled.c) rt5081_pmu_reg_update_bits这个是写寄存器的
			int rt5081_pmu_reg_update_bits(struct rt5081_pmu_chip *chip, u8 addr,u8 mask, u8 data)
		
			//Gionee <GN_BSP_CHG> <lilubao> <20170929> add for debug flashlight begin
	
			
			dev_dbg_ratelimited(chip->dev, "%s: reg %02x data %02x\n", __func__,
		addr, data);
		
		
				ret=rt5081_pmu_reg_update_bits(fled_data->chip,RT5081_PMU_REG_FLEDVMIDTRKCTRL1,
			RT5081_FLED_FIXED_MODE_MASK,0x4f);	// vmid -> 4.0v
			
			
			int rt5081_pmu_reg_read(struct rt5081_pmu_chip *chip, u8 addr)
			
			rt5081_fled_set_mode
			
				
		
			#define RT5081_PMU_REG_FLEDVMIDTRKCTRL1	(0x7A)
			#define RT5081_PMU_REG_FLEDVMIDRTM	(0x7B)
			#define RT5081_PMU_REG_FLEDVMIDTRKCTRL2	(0x7C)
	
		}

	}	


}


























/*********************************************************************************************************************************/
17G06A
{
	OTG
	{
		GNSPR#120454_打开OTG开关未识别U盘
		
		
		
		
		
		
		
	
	
	}






	充电器检测流程：
	 {
		 (qpnp-smbcharger.c) src_detect_handler 电压是上升的还是下降-> handle_usb_insertion  USB插入工作
		 
		 读取充电器类型，过滤杂波 -> smbchg_change_usb_supply_type 设置充电电流
		 
		  * src_detect_handler() - this is called on rising edge when USB charger type
		 is detected and on falling edge when USB voltage falls
		 below the coarse detect voltage(1V), use it for
		 handling USB charger insertion and removal.
		 
		 
		 充电器的类型：
		 enum power_supply_type {
			POWER_SUPPLY_TYPE_UNKNOWN = 0,
			POWER_SUPPLY_TYPE_BATTERY,
			POWER_SUPPLY_TYPE_UPS,
			POWER_SUPPLY_TYPE_MAINS,
			POWER_SUPPLY_TYPE_USB,		/* Standard Downstream Port */
			POWER_SUPPLY_TYPE_USB_DCP,	/* Dedicated Charging Port */
			POWER_SUPPLY_TYPE_USB_CDP,	/* Charging Downstream Port */
			POWER_SUPPLY_TYPE_USB_ACA,	/* Accessory Charger Adapters */
			POWER_SUPPLY_TYPE_USB_HVDCP,	/* High Voltage DCP */
			POWER_SUPPLY_TYPE_USB_HVDCP_3,	/* Efficient High Voltage DCP */
			POWER_SUPPLY_TYPE_WIRELESS,	/* Accessory Charger Adapters */
			POWER_SUPPLY_TYPE_BMS,		/* Battery Monitor System */
			POWER_SUPPLY_TYPE_USB_PARALLEL,		/* USB Parallel Path */
			POWER_SUPPLY_TYPE_WIPOWER,		/* Wipower */
			POWER_SUPPLY_TYPE_TYPEC,	/*Type-C */
			POWER_SUPPLY_TYPE_UFP,		/* Type-C UFP */
			POWER_SUPPLY_TYPE_DFP,		/* TYpe-C DFP */
		};
	
	}	
	
	
	
	
	
		
	关机充电的流程
	{
		Android Bootloader - UART_DM Initialized!!!
		[0] welcome to lk

		[10] platform_init()
		[10] target_init()
	
	
		[40] pm8x41_get_is_cold_boot: cold boot
		[40] Qseecom Init Done in Appsbl
	
	
		[90] Waiting for the RPM to populate smd channel table
	
	
		高通关机重启
		/* In case of fastboot reboot,adb reboot or if we see the power key
		* pressed we do not want go into charger mode.
		* fastboot reboot is warm boot with PON hard reset bit not set
		* adb reboot is a cold boot with PON hard reset bit set
		*/
	
		80-nm620-1_b_pmic_pon-reset_software_drivers_overview.pdf
	
		重启的类型：OTST3, KPD_PWR_N, PS_HOLD
	
	
		dVdd_rb，xVdd_rb
	
		pon_resin_n_reset_s1_timer
	
	
		Kernel command line: sched_enable_hmp=1 console=ttyHSL0,115200,n8 androidboot.console=ttyHSL0 androidboot.hardware=qcom msm_rtb.filter=0x237 ehci-hcd.park=3 lpm_levels.sleep_disabled=1 
		androidboot.bootdevice=7824900.sdhci earlycon=msm_hsl_uart,0x78B0000 buildvariant=eng androidboot.emmc=true androidboot.verifiedbootstate=green androidboot.veritymode=enforcing 

		androidboot.keymaster=1 androidboot.serialno=665cd078

		androidboot.mode=charger androidboot.authorized_kernel=true androidboot.baseband=msm printk.disable_uart=9 mdss_mdp.panel=1:dsi:0:qcom,mdss_dsi_ft8006m_720p_video:1:none:cfg:single_dsi

		这些参数在 （aboot.c）unsigned char *update_cmdline(const char * cmdline) , 获取cmdline
		（init.c）target_pause_for_battery_charge获取关机状态插入充电器的动作还是fastboot，adb等pon	
		init.c文件还有获取组合按键动作

		Every reset happens in different stages and each stage timer is
		configurable.

		Stage 1 – For each of the triggers, an interrupt is sent to the MSM to warn it about the
		shutdown. We can configure the amount of time (S1_Timer) the debounced trigger must
		be held before a bark is sent to the MSM using this register:
		PON_XXX_RESET_S1_TIMER.
		Stage 2 – Once the interrupt is sent to the MSM, the PMIC waits for a reply for S2_TIMER
		period before it performs the desired shutdown. We can configure the amount of time the
		debounced trigger using this register: PON_XXX_RESET_S2_TIMER.
		Stage 3 – This stage is independent of S1 and S2. If trigger is held for an extended period
		of time (default 64 sec), a forced xVdd shutdown occurs. Which means it is an abrupt
		power down and it does not follow a graceful shutdown procedure.

		PAGE 20
		This guarantees that there is a way out for unexpected hangups
		Reset all PMIC registers	
	}


}








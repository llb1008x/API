














18Y07A
{
	1.bringup
	vendor/mediatek/proprietary/custom/gnbj6763_66_n1/kernel/dct/dct/codegen.dws
	vendor/mediatek/proprietary/bootable/bootloader/preloader/custom/gnbj6763_66_n1/dct/dct/codegen.dws
	vendor/mediatek/proprietary/bootable/bootloader/lk/target/gnbj6763_66_n1/dct/dct/codegen.dws
	
	gpio:
	 drv_vbus
	 usb_id
	 gpio_chg_en
	 eint_chg_stat
	 
 	cpu :mt6763
	pmic:mt6356
	 


	
	{
		先搜AndroidProducts.mk：定义一个变量——>PRODUCT_MAKEFILES，该变量的值为产品版本定义文件名的列表  
        产品版本定义文件：对特定产品版本的定义（可多个文件，多个版本）。一般情况下，我们不需要定义所有变量（版本相关），，Build系统已经预先定义了一些组合，
		位于build/target/product，该目录下每个文件都定义了一个组合，我们只需要继承这些预置的定义，然后再覆盖自己想要的变量定义即可
        BroadConfig.mk：该文件用来配置硬件主板，它定义的都是设备底层的硬件特性，如设备的主板相关信息，wifi，bootloader，内核等

		最后确定thermal 用的gionee_bj这个目录，之前一直以为是mediatek mt6757目录下面
		而且编译要全编
		
		preloader 的宏开关
		vendor/mediatek/proprietary/bootable/bootloader/preloader/custom/gnbj6763_66_n1/gnbj6763_66_n1.mk
	
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
		
		
		
		
	}

	

}
























17G08A
{


	1.电量计的问题
	{
		相关的bug：140834,138519,138239,138056
	
		140834,待机界面》APP键进后台-手动重启完成后-手机未继续充电（手机显示96%-呼吸灯为绿色-进MMI显示已充满)》插拔充电器恢复 
		138519,连接充电器重启手机后，锁屏界面显示充电完毕，实际上手机电量是93%，截屏后恢复
		138239,手机电量98%，关机充电，手机开机，锁屏界面与状态栏电量显示不一致
		138056,电量94%以上100%以下连接充电器手动重启手机，重启后充电图标不显示充电，呼吸灯显示为绿色，插拔充电器恢复。
	
		分析：
		高电量开关机重启电量不一定会变，但是充电状态会改变，显示充电截止，呼吸灯颜色变化
		高电量也会增加
	
		电量多少，是否插充电器，是长按还是短按？长按重启的电量状态如何
		之前还有xdvdd很radc的问题先了解是什么,就是输入端应该有usbin还有其他几个？原理图
	
	
	
		相关的case：03257510,03238858,03249227
		{
			20171212
			  1.目前分析可能跟重启启动的时候系统电流过大，而aicl电流小，导致充进电池的
			  小于截止电流，所以认为是 TERMINATION或INHIBIT
			  msm-pm660.dtsi 这几个电流目前是
			  	qcom,fg-chg-term-current = <200>;	/* 100mA > 0.02C 200->150 */ 充电截止电流
				qcom,fg-sys-term-current = <(-225)>;	/* 100mA + 25mA */ 显示100%
				qcom,fg-chg-term-base-current = <175>;	/* 100mA - 25mA */ 
			   还修改了满电跟回充的条件
			   	qcom,hold-soc-while-full;
				qcom,fg-recharge-soc-thr = <99>; 	
			
			  2.重启之后显示充电满了
			   所以现在要确定充满的判断条件和充电逻辑	
			   charge_done 是判断BATTERY_CHARGER_STATUS_1_REG这个寄存器的[2:0],读寄存器
				000 - TRICKLE
				001 - PRECHARGE
				010 - FAST
				011 - FULLON
				100 - TAPER
				101 - TERMINATION
				110 - INHIBIT
			
				这里还有一个状态是recharge_soc，就是说如果输入还在但是充电已经截止了，要去调整回充电量
			
				(qpnp-fg-gen3.c)power_supply_get_property -> psy->desc->get_property(psy, psp, val)  -> (qpnp-smb2.c)get_property = smb2_batt_get_prop
			
				charge status
				enum {
					POWER_SUPPLY_STATUS_UNKNOWN = 0,
					POWER_SUPPLY_STATUS_CHARGING,
					POWER_SUPPLY_STATUS_DISCHARGING,
					POWER_SUPPLY_STATUS_NOT_CHARGING,
					POWER_SUPPLY_STATUS_FULL,
				};
				
				
				charger type
				enum {
					POWER_SUPPLY_CHARGE_TYPE_UNKNOWN = 0,
					POWER_SUPPLY_CHARGE_TYPE_NONE,
					POWER_SUPPLY_CHARGE_TYPE_TRICKLE,
					POWER_SUPPLY_CHARGE_TYPE_FAST,
					POWER_SUPPLY_CHARGE_TYPE_TAPER,
				};
			
				chip->charge_done,  chip->charge_full 这两个有什么判断条件
				判断charge done 的条件是读取寄存器 smblib_get_prop_batt_charge_done，POWER_SUPPLY_PROP_CHARGE_DONE 这一位
				如果是TERMINATE_CHARGE,INHIBIT_CHARGE，这两个都是停止充电
				判断chip->charge_full的条件是fg_charge_full_update,
					if (chip->charge_done && !chip->charge_full) {

						if (msoc >= 99 && chip->health == POWER_SUPPLY_HEALTH_GOOD) {

							fg_dbg(chip, FG_STATUS, "Setting charge_full to true\n");
							chip->charge_full = true;
							/*
							 * Lower the recharge voltage so that VBAT_LT_RECHG
							 * signal will not be asserted soon.
							 */
							rc = fg_set_recharge_voltage(chip,
									AUTO_RECHG_VOLT_LOW_LIMIT_MV);
							if (rc < 0) {
								pr_err("Error in reducing recharge voltage, rc=%d\n",
									rc);
								goto out;
							}
						}else ...	
			
			
				modem 侧也有控制充电的，
				QcomChargerConfig_VbattTh_Sdm660.cfg  chargingtermcurrent
			
			
				好几个soc 弄清楚什么意思：
				msoc：monotonic SOC 这个是显示到UI的soc，单调的增强用户体验，比如100%跟1%
				maintsoc:maintenance SOC 维持soc的稳定，这个soc通常跟delta_soc两个一同判断
				用来判断电量变化的稳定
				bsoc:battery soc
			


				3.check 0x1006 in probe function, if it’s INHIBIT status, then re-enable charging.(toggle 0x1042)
				就是说在probe的时候如果检测到inhibit的状态，重新时能充电寄存器的状态，

				check  0x1006
				rc = smblib_read(chg, BATTERY_CHARGER_STATUS_1_REG, &stat);
				if (rc < 0) {
					smblib_err(chg, "Couldn't read BATTERY_CHARGER_STATUS_1 rc=%d\n",
						rc);
					return rc;
				}
			
				smb-reg.h   
				stat = stat & BATTERY_CHARGER_STATUS_MASK;
				stat == TERMINATE_CHARG	
			
				smb-lib.c
				re-enable charging
				#define CHARGING_ENABLE_CMD_REG				(CHGR_BASE + 0x42)
				#define CHARGING_ENABLE_CMD_BIT				BIT(0)	
				rc = smblib_write(chg, CHARGING_ENABLE_CMD_REG,
							CHARGING_ENABLE_CMD_BIT);
				if (rc < 0) {
					pr_err("Couldn't enable charging rc=%d\n", rc);
					return rc;
				}
			
					
				qpnp-smb2.c
				modify 			
				//Gionee <GN_BY_CHG> <lilubao> <20171215> add for debug fuel gauge begin
				#if defined(GN_FG_DEBUG)
					pr_err("in [%s] before GN_FG_DEBU \n",__FUNCTION__);

					rc = smblib_read(chg, BATTERY_CHARGER_STATUS_1_REG, &stat);

					pr_err("stat->%d\n",stat);
					if (rc < 0) {
						pr_err("Couldn't read BATTERY_CHARGER_STATUS_1 rc=%d  by lilubao\n",rc);
						return rc;
					}
			
					stat = stat & BATTERY_CHARGER_STATUS_MASK;

					if( stat == INHIBIT_CHARGE){

						rc = smblib_write(chg, CHARGING_ENABLE_CMD_REG,
										CHARGING_ENABLE_CMD_BIT);
						if (rc < 0) {
							pr_err("Couldn't enable charging rc=%d\n", rc);
							return rc;
						}
					}
					pr_err("in [%s] after GN_FG_DEBUG \n",__FUNCTION__);
	
				#endif
				//Gionee <GN_BY_CHG> <lilubao> <20171215> add for debug fuel gauge end
				
				这里出现一个问题就是修改编译是可以，但是重启之后进入wathc dog 然后进入死机
				怀疑应该是通过power supply获取smbcharger chip 这个结构体有问题，不是全局变量，
				现在需要清楚注册到power supply子系统，然后如何通过名字去获取这个结构的指针？
				
				re-reable 不行而且应该在qpnp-smb2.c里面改
				
				struct power_supply *usb_psy = power_supply_get_by_name("usb");
				struct smb2 *chip = power_supply_get_drvdata(usb_psy);
				struct smb_charger *chg = &chip->chg;
				
				"usb" ,"parallel" ,"bms" ,"battery"
				struct power_supply	*fg_psy;
				struct power_supply	*batt_psy;
				struct power_supply	*usb_psy;
				struct power_supply	*dc_psy;
				struct power_supply	*parallel_psy;
				
				
				struct *bms_psy = power_supply_get_by_name("bms");
				struct fg_chip *chip = power_supply_get_drvdata(bms_psy);
				int msoc;
				rc = fg_get_msoc(chip, &msoc);
	
				
				
				power supply 子系统的框架,注册，管理，调用
				
				
				
				首先确定在停止充电的时候re-enable可以充电？
				加一个delay work 
				chg->bb_removal_work		smblib_bb_removal_work			
				INIT_DELAYED_WORK(&chg->bb_removal_work, smblib_bb_removal_work);
				
				smblib_handle_switcher_power_ok
					schedule_delayed_work(&chg->bb_removal_work,
					msecs_to_jiffies(BOOST_BACK_UNVOTE_DELAY_MS));
				
				
	 
			   4.这个问题需要先diable然后enable，之前只是enable可能有问题
				当然还要加一个电量跟充电器是否存在的判断
				rc = smblib_write(chg, CHARGING_ENABLE_CMD_REG, 0);
				if (rc < 0) {
					smblib_err(chg, "Couldn't disable charging rc=%d\n",
								rc);
					return rc;
				}
				rc = smblib_write(chg, CHARGING_ENABLE_CMD_REG,
								CHARGING_ENABLE_CMD_BIT);
				if (rc < 0) {
					smblib_err(chg, "Couldn't enable charging rc=%d\n",
								rc);
					return rc;
				}
				
				判断充电器是否存在
				int smblib_get_prop_usb_present(struct smb_charger *chg,
				union power_supply_propval *val)
				rc = smblib_get_prop_usb_present(chg, val);
				

				smblib_get_prop_batt_capacity
				
				获取当前的电量 power_supply子系统应该更方便一点
				static int smb2_batt_get_prop(struct power_supply *psy,enum power_supply_property psp,union power_supply_propval *val)
				
				qpnp-fg-gen3.c
				rc = fg_get_msoc(chip, &msoc);
				
				
				
				modify
				//Gionee <GN_BY_CHG> <lilubao> <20171219> add for debug fuel gauge begin
				static void smb2_re_enable_charging(struct work_struct *work)
				{
	
					int rc,usb_present,gn_fg_msoc;
					u8 stat;
					union power_supply_propval val;
					struct smb_charger *chg = container_of(work, struct smb_charger,
										re_enable_charging.work);
	
					pr_err("in [%s]\n",__FUNCTION__);

					rc = smblib_get_prop_usb_present(chg, &val);
					if (rc < 0) {
						pr_err("Couldn't get usb present rc=%d\n", rc);
						//return rc;
					}
					usb_present = val.intval;

					rc = smblib_get_prop_batt_capacity(chg, &val);
					if (rc < 0) {
						pr_err("Couldn't get battery soc rc=%d\n", rc);
						//return rc;
					}
					gn_fg_msoc = val.intval;

					pr_err("in [%s] gn_fg_msoc->%d,usb_present->%d\n",__FUNCTION__,gn_fg_msoc,usb_present);
	
					//Gionee <GN_BY_CHG> <lilubao> <20171219> add for debug fuel gauge begin
					if( usb_present && (gn_fg_msoc < 100) ){
					//Gionee <GN_BY_CHG> <lilubao> <20171219> add for debug fuel gauge end
						rc = smblib_read(chg, BATTERY_CHARGER_STATUS_1_REG, &stat);

						if (rc < 0) {
							pr_err("Couldn't read BATTERY_CHARGER_STATUS_1 rc=%d\n",rc);
							//return rc;
						}

						stat = stat & BATTERY_CHARGER_STATUS_MASK;

						if( stat == INHIBIT_CHARGE){

			
							rc = smblib_write(chg, CHARGING_ENABLE_CMD_REG, 0);
							if (rc < 0) {
								pr_err("Couldn't disable charging rc=%d\n",
											rc);
								//return rc;
							}
			
							rc = smblib_write(chg, CHARGING_ENABLE_CMD_REG,
											CHARGING_ENABLE_CMD_BIT);
							if (rc < 0) {
								pr_err( "Couldn't enable charging rc=%d\n",
											rc);
								//return rc;
							}
						
						}
					}

					//return 0; 	
				}
				//Gionee <GN_BY_CHG> <lilubao> <20171219> add for debug fuel gauge end
				
				INIT_DELAYED_WORK(&chg->re_enable_charging,smb2_re_enable_charging);

				#define  BOOST_RECHARGING_DELAY_MS	1000
	
				schedule_delayed_work(&chg->re_enable_charging,
							msecs_to_jiffies(BOOST_RECHARGING_DELAY_MS));
				//Gionee <GN_BY_CHG> <lilubao> <20171215> add for debug fuel gauge end
				
				
				应该要思考一下debug的方式 代码要漂亮，简洁，逻辑要完整
				添加宏，设备节点，还是在hal层添加一个调试的应用程序
						
		}			
		
		
		

		相关的内容
		{
		 代码文件：
			batterydata
			uefi :BATTERY.PROVISION
			kernel:fg-gen3-batterydata-gionee-17g08a-atl-4v4-5060mAh.dtsi
	
			qpnp-fg-gen3.c  , smb-lib.c	,smb135x-charger.c ,qpnp-smb2.c
		
			smb-lib.c
				smblib_get_prop_batt_status
				BATTERY_CHARGER_STATUS_1_REG 充电器时电池的状态
				[2:0] 充进电池的状态
			
				smb2_batt_get_prop
				很多获取电池相关的信息
			
		
			SCHG_MISC_AICL_STATUS  				0x0000160A
			SCHG_CHGR_CHARGING_ENABLE_CMD		0x00001042
			SCHG_CHGR_BATTERY_CHARGER_STATUS_1	0x00001006
			SCHG_MISC_POWER_PATH_STATUS			0x0000160B
			SCHG_USB_INT_RT_STS					0x00001310
			
			
			kba-170418012921  			一些关于SDM660 pmic相关的kba
			80-VT310-138				Understanding PMI8998 Fuel Gauge		
	
		 调试：		
			打印寄存器信息	
			/sys/kernel/debug/regmap/spmi0-00 
			echo 0x1006 > address 
			echo 100    >  count 
			cat  		data
		
		
			mem_intf
			adb shell "echo 0 > /sys/kernel/debug/fg/sram/address"
			adb shell "echo 4 > /sys/kernel/debug/fg/sram/count"
			adb shell "cat /sys/kernel/debug/fg/sram/data"
		
		
			kernek debug log
		
			echo 0x7f > /sys/module/qpnp_fg_gen3/parameters/debug_mask
			echo 8 > /proc/sys/kernel/printk
			dmesg > debug_log_filename
		
		
		
			dmesg | tree  1.txt   这个应该是把开机的一些log导到一个文件内
			dump.sh dump  fuel gauge寄存器   ./dump.sh > /data/kmsg.txt &
		
			fg_gen3_debug_mask=0x19F;  0001 1001 1111
			static int __debug_mask=0xFF;
		
				
			//Gionee <GN_BY_CHG> <lilubao> <20171213> add for debug fuel gauge begin
			dev_err(chip->dev, "in [%s] by lilubao \n",__FUNCTION__);	
			smblib_err(chg, "in [%s] by lilubao \n",__FUNCTION__);
		}

		

		{

		 dtsi:
			qcom,fg-chg-term-current
			Usage:      optional
			Value type: <u32>
			Definition: Battery current (in mA) at which the fuel gauge will issue
					an end of charge if the charger is configured to use the
					fuel gauge ADC for end of charge detection. If this
					property is not specified, then the default value used
					will be 100mA.

			qcom,fg-sys-term-current
				Usage:      optional
				Value type: <u32>
				Definition: Battery current (in mA) at which the fuel gauge will try to
						scale towards 100%. When the charge current goes above this
						the SOC should be at 100%. If this property is not
						specified, then the default value used will be -125mA.
						This value has to be specified in negative values for
						the charging current.

			qcom,fg-chg-term-base-current
				Usage:      optional
				Value type: <u32>
				Definition: Battery current (in mA) upper boundary at which the fuel
						gauge will issue an end of charge during discharging. If
						this property is not specified, then the default value used
						will be 75mA.
					
		
			qcom,hold-soc-while-full;
				Definition: A boolean property that when defined holds SOC at 100% when
				the battery is full.

			
			qcom,fg-recharge-soc-thr = <99>; 
				Definition: Percentage of monotonic SOC upon which the charging will
				will be resumed once the charging is complete. If this
				property is not specified, then the default value will be
				95.
			
			(qpnp-fg-gen3.c) fg_adjust_recharge_soc
				/*
				 * If the input is present and charging had been terminated, adjust
				 * the recharge SOC threshold based on the monotonic SOC at which
				 * the charge termination had happened.
				 */		

		

			log关键字：
			msoc，bsoc,maint_soc,delta_soc
			smblib_get_charge_param: usb input current limit 	aicl调节电流
			smblib_get_charge_param: input current limit status 当前充电电流的状态
			fg_charge_full_update								上报电量
			ech_wls_charger_external_power_changed  			充电器检测
			FG:
			PMI:
		}

	}

	

	
	
	
		
	

	
	
	2.BC1.2 手机usb检测时间太长
	{
	
		qc充电器检测的过程有哪些？各阶段用了多长时间，能不能减少？
		{
			apsd:Automatic Power Source Detection
	
			整个检测的时间：
			slow plugin timer + enumeration timer + adapter voltage change + authentication algorithm complete + HVDCP detection
			#define HVDCP_DET_MS 2500	-> smblib_handle_apsd_done

			usb检测时间是 1.5s  0->4 
									 	0.6s       2.4s
			charger检测时间是 3s左右，0   ->    5   ->   9
				
			上层显示的时间：2.54s 显示充电，2.08s显示快充
				
			
			
			系统输入的USBIN ，DCIN 这两个分别是什么
			usbin 从外部的 vbus usbin接入，有两个分别时pm660跟smb138x两个充电芯片输入
			dcin 搜索直流电源输入接口,外接电源输入口，这个好像没有
			
			
			apsd的状态
			7 USBIN_LVOV 							The USBIN LVOV detected
			6 HVDCP_CHECK_TIMEOUT 					HVDCP Detection Timeout Expired
			5 SLOW_PLUGIN_TIMEOUT 					Slow Plugin Timeout Expired
			4 ENUMERATION_DONE 						SDP Enumeration Done
			3 VADP_CHANGE_DONE_AFTER_AUTH 			Adaptive Voltage Change Done after Authentication
			2 QC_AUTH_DONE_STATUS					QC Authentication Algorithm Done
			1 QC_CHARGER							Quick Charge Detection Done
			0 APSD_DTC_STATUS_DONE 					APSD Detection Done
			
			
			charger 充电器插入是smblib_handle_usbin_uv 
			insert
			<6>[ 1384.829967] PMI: smblib_handle_usbin_uv: IRQ: usbin-uv

			<6>[ 1384.830987] PMI: smblib_usb_plugin_locked: enabling DPDM regulator
			<6>[ 1384.854058] PMI: smblib_usb_plugin_locked: IRQ: usbin-plugin attached
			
			<6>[ 1385.468371] PMI: smblib_handle_chg_state_change: IRQ: chg-state-change
			<3>[ 1385.567325] PMI: smblib_handle_usb_source_change: APSD_STATUS = 0x01

			<6>[ 1385.567406] PMI: smblib_rerun_apsd: re-running APSD
			<6>[ 1386.848078] PMI: smblib_get_apsd_result: APSD_STATUS = 0x00
			<6>[ 1386.848131] PMI: smblib_get_charge_param: input current limit status = 75000 (0x03)

			<3>[ 1386.848161] usb real_charger_type = 0
			
			<6>[ 1386.947384] PMI: smblib_update_usb_type: APSD=DCP PD=0
			
			<6>[ 1387.308580] PMI: smblib_handle_debug: IRQ: usbin-collapse
			<6>[ 1387.308602] PMI: smblib_handle_debug: IRQ: aicl-done
			
			<3>[ 1387.438065] usb real_charger_type = 5
			
			<3>[ 1388.742128] PMI: smblib_handle_usb_source_change: APSD_STATUS = 0x43
			<6>[ 1388.742478] PMI: smblib_get_apsd_result: APSD_STATUS = 0x43
			<3>[ 1388.742590] [ech_wls_chrg_get_property] psp = 4.
			<3>[ 1388.755432] ech_wls_is_online = 0
			<6>[ 1388.760377] PMI: smblib_handle_hvdcp_check_timeout: IRQ: smblib_handle_hvdcp_check_timeout rising
			
			<6>[ 1389.200808] PMI: smblib_handle_hvdcp_3p0_auth_done: IRQ: hvdcp-3p0-auth-done rising; HVDCP3 detected
			<6>[ 1389.200824] PMI: smblib_handle_sdp_enumeration_done: IRQ: sdp-enumeration-done falling
			<6>[ 1389.200832] PMI: smblib_handle_slow_plugin_timeout: IRQ: slow-plugin-timeout falling
			<6>[ 1389.200932] PMI: smblib_set_charge_param: buck switching frequency = 600 (0x0f)
			<3>[ 1389.200993] PMI: smblib_handle_usb_source_change: APSD_STATUS = 0x47
			
			
			<6>[ 1389.204694] PMI: smblib_get_apsd_result: APSD_STATUS = 0x47
			<3>[ 1389.204732] [ech_wls_chrg_get_property] psp = 4.
			<6>[ 1389.263439] PMI: smblib_handle_apsd_done: IRQ: apsd-done rising; HVDCP3 detected

			
			通过power supply 早点上报充电器的类型,现在的问题是如何上报这个事件
			
			smblib_update_usb_type(chg);
			power_supply_changed(chg->usb_psy);
			
			rerun apsd 有
			smblib_rerun_apsd
			smblib_rerun_apsd_if_required	这个是初始化的时候，可以看到会有两次apsd检测
			
			插入充电器的调用流程
			smblib_handle_usb_plugin ->  smblib_usb_plugin_locked (dp dm ,parallel charging)  ->  smblib_handle_usb_source_change 
		}


		相关的代码逻辑和文档
		{
			关于usb的检测
			smblib_handle_usb_plugin
			smblib_handle_usb_source_change
			
			寄存器：
			SCHG_USB_APSD_STATUS						0x00001307	apsd是否走完了和相关流程的判断
			SCHG_USB_APSD_RESULT_STATUS					0x00001308 	充电器类型
			SCHG_USB_USBIN_INPUT_STATUS					0x00001306	usbin的电压输入范围
			SCHG_MISC_AICL_STATUS						0x0000160A	
			SCHG_USB_USBIN_SOURCE_CHANGE_INTRPT_ENB		0x00001369	这个是关于充电检测几个阶段的一些中断使能
			
			相关寄存器的基地址
			#define CHGR_BASE	0x1000
			#define OTG_BASE	0x1100
			#define BATIF_BASE	0x1200
			#define USBIN_BASE	0x1300
			#define DCIN_BASE	0x1400
			#define MISC_BASE	0x1600
			#define CHGR_FREQ_BASE	0x1900
			
				
			irqreturn_t smblib_handle_usb_source_change(int irq, void *data)
			{
				struct smb_irq_data *irq_data = data;
				struct smb_charger *chg = irq_data->parent_data;
				int rc = 0;
				u8 stat;

				rc = smblib_read(chg, APSD_STATUS_REG, &stat);
				if (rc < 0) {
					smblib_err(chg, "Couldn't read APSD_STATUS rc=%d\n", rc);
					return IRQ_HANDLED;
				}
				//Gionee <GN_BY_CHG> <lilubao> <20171218> add for debug charger detect begin
				smblib_err(chg, PR_REGISTER, "APSD_STATUS = 0x%02x\n", stat);
				//Gionee <GN_BY_CHG> <lilubao> <20171218> add for debug charger detect end

				if (chg->micro_usb_mode && (stat & APSD_DTC_STATUS_DONE_BIT)
						&& !chg->uusb_apsd_rerun_done) {
					/*
					 * Force re-run APSD to handle slow insertion related
					 * charger-mis-detection.
					 */
					chg->uusb_apsd_rerun_done = true;
					smblib_rerun_apsd(chg);
					return IRQ_HANDLED;
				}

				smblib_handle_apsd_done(chg,
					(bool)(stat & APSD_DTC_STATUS_DONE_BIT));

				smblib_handle_hvdcp_detect_done(chg,
					(bool)(stat & QC_CHARGER_BIT));

				smblib_handle_hvdcp_check_timeout(chg,
					(bool)(stat & HVDCP_CHECK_TIMEOUT_BIT),
					(bool)(stat & QC_CHARGER_BIT));

				smblib_handle_hvdcp_3p0_auth_done(chg,
					(bool)(stat & QC_AUTH_DONE_STATUS_BIT));

				smblib_handle_sdp_enumeration_done(chg,
					(bool)(stat & ENUMERATION_DONE_BIT));

				smblib_handle_slow_plugin_timeout(chg,
					(bool)(stat & SLOW_PLUGIN_TIMEOUT_BIT));

				smblib_hvdcp_adaptive_voltage_change(chg);

				power_supply_changed(chg->usb_psy);

				rc = smblib_read(chg, APSD_STATUS_REG, &stat);
				if (rc < 0) {
					smblib_err(chg, "Couldn't read APSD_STATUS rc=%d\n", rc);
					return IRQ_HANDLED;
				}
				smblib_dbg(chg, PR_REGISTER, "APSD_STATUS = 0x%02x\n", stat);

				return IRQ_HANDLED;
			}


			充电器的类型
			enum power_supply_type {
				POWER_SUPPLY_TYPE_UNKNOWN = 0,										0
				POWER_SUPPLY_TYPE_BATTERY,											1
				POWER_SUPPLY_TYPE_UPS,												2
				POWER_SUPPLY_TYPE_MAINS,											3
				POWER_SUPPLY_TYPE_USB,		/* Standard Downstream Port */			4
				POWER_SUPPLY_TYPE_USB_DCP,	/* Dedicated Charging Port */			5
				POWER_SUPPLY_TYPE_USB_CDP,	/* Charging Downstream Port */			6
				POWER_SUPPLY_TYPE_USB_ACA,	/* Accessory Charger Adapters */		7
				POWER_SUPPLY_TYPE_USB_HVDCP,	/* High Voltage DCP */				8
				POWER_SUPPLY_TYPE_USB_HVDCP_3,	/* Efficient High Voltage DCP */	9
				POWER_SUPPLY_TYPE_USB_PD,	/* Power Delivery */
				POWER_SUPPLY_TYPE_WIRELESS,	/* Accessory Charger Adapters */
				POWER_SUPPLY_TYPE_USB_FLOAT,	/* Floating charger */
				POWER_SUPPLY_TYPE_BMS,		/* Battery Monitor System */
				POWER_SUPPLY_TYPE_PARALLEL,	/* Parallel Path */
				POWER_SUPPLY_TYPE_MAIN,		/* Main Path */
				POWER_SUPPLY_TYPE_WIPOWER,	/* Wipower */
				POWER_SUPPLY_TYPE_TYPEC,	/* Type-C */
				POWER_SUPPLY_TYPE_UFP,		/* Type-C UFP */
				POWER_SUPPLY_TYPE_DFP,		/* TYpe-C DFP */
			};
			
			power supply 中的type-c 和pd
			{
				POWER_SUPPLY_PROP_TYPEC_MODE,
				POWER_SUPPLY_PROP_TYPEC_CC_ORIENTATION, /* 0: N/C, 1: CC1, 2: CC2 */
				POWER_SUPPLY_PROP_TYPEC_POWER_ROLE,
				POWER_SUPPLY_PROP_PD_ALLOWED,
				POWER_SUPPLY_PROP_PD_ACTIVE,
				POWER_SUPPLY_PROP_PD_IN_HARD_RESET,
				POWER_SUPPLY_PROP_PD_CURRENT_MAX,
				POWER_SUPPLY_PROP_PD_USB_SUSPEND_SUPPORTED,	
			}

			
	
		}
				

			debug
			{
				//Gionee <GN_BY_CHG> <lilubao> <20171218> add for debug charger detect begin
			
				case :03267108
			
				echo 'file smb-lib.c +p' > /d/dynamic_debug/control
				echo 'file qpnp-smb2.c +p' > /d/dynamic_debug/control
				echo 8 > /proc/sys/kernel/printk 
				
				setprop "persist.hvdcp.log_level" 1
				echo 0xFF > /sys/module/qpnp_smb2/parameters/debug_mask
				
				
				int smblib_set_prop_pd_active( )
				{
				......................
				--- chg->pd_active = val->intval;
				+++ chg->pd_active = 0;
				......................
				}
			}
			
			
			modify
			smb-lib.c  smblib_handle_usb_source_change
			//Gionee <GN_BY_CHG> <lilubao> <20171218> add for debug charger detect begin
			smblib_err(chg, "APSD_STATUS before = 0x%02x\n", stat);
			if (chg->micro_usb_mode && (stat & APSD_DTC_STATUS_DONE_BIT)
					&& !chg->uusb_apsd_rerun_done) {
				/*
				 * Force re-run APSD to handle slow insertion related
				 * charger-mis-detection.
				 */

				//Gionee <GN_BY_CHG> <lilubao> <20171218> add for debug charger detect begin
				//update usb type for decrease time 
				smblib_update_usb_type(chg);
				//Gionee <GN_BY_CHG> <lilubao> <20171218> add for debug charger detect end

				chg->uusb_apsd_rerun_done = true;
				smblib_rerun_apsd(chg);
				return IRQ_HANDLED;
			}
			//Gionee <GN_BY_CHG> <lilubao> <20171218> add for debug charger detect end
			
		
	}
	
	
	
	
	
	

	
	3.U盘识别时间过长 (32s)
	{
		usb眼图通过，同平台其他无此问题，
		本地验证了，大概4~6此出现一次不识别或识别慢的问题，不是必现的
		
		//Gionee <GN_BY_CHG> <lilubao> <20171220> add for debug usb begin
		
		case ID:03272648,03264824
		
			1.
			Many of the information provided in the log is not printed.

			Please provide the log after adding the following code.
			drivers/usb/gadget/configfs.c
			drivers/usb/dwc3/dwc3-msm.c
			drivers/usb/dwc3/gadget.c
			kernel/drivers/usb/core/hub.c
			#undef dev_dbg
			#undef pr_debug
			#define dev_dbg dev_err
			#define pr_debug pr_err

			drivers/usb/gadget/function/f_mtp.c
			#undef DBG
			#undef VDBG
			#define VDBG ERROR
			#define DBG ERROR

			drivers/usb/gadget/composite.c
			#undef INFO
			#undef DBG
			#undef pr_debug
			#define INFO ERROR
			#define DBG ERROR
			#define pr_debug pr_err

			diff --git a/drivers/usb/gadget/configfs.c b/drivers/usb/gadget/configfs.c
			index fa05070..0ebdb2f 100644
			--- a/drivers/usb/gadget/configfs.c
			+++ b/drivers/usb/gadget/configfs.c
			@@ -443,6 +443,7 @@ static int config_usb_cfg_link(
			goto out;
			}

			+ pr_info("[oem] %s: func:%s\n", _func_, f->name);
			/ stash the function until we bind it to the gadget /
			list_add_tail(&f->list, &cfg->func_list);
			ret = 0; 
			
			
			See some exception information in log.
			<3>[ 561.336568] msm-dwc3 a800000.ssusb: set core clk rate 133333333
			<3>[ 561.507213] msm-dwc3 a800000.ssusb: block_reset ASSERT
			<3>[ 561.508793] msm-dwc3 a800000.ssusb: block_reset DEASSERT
			<3>[ 561.526782] msm-dwc3 a800000.ssusb: DWC3_CONTROLLER_RESET_EVENT received
			<3>[ 561.526820] msm-dwc3 a800000.ssusb: DWC3_CONTROLLER_POST_RESET_EVENT received
			<3>[ 561.546967] msm-dwc3 a800000.ssusb: b_idle state
			<3>[ 561.558286] msm-dwc3 a800000.ssusb: Cable disconnected

			Does the debug switch of hub.c have not been opened? No information was seen.

			-------------------------------------------------------------------------------
			2.
			Please provide the log after adding the following code. Do not remove the dbug switch that has been added before.
			To improve efficiency, please leave your phone number.

			drivers/usb/core/hub.c
			#undef dev_dbg
			#undef pr_debug
			#define dev_dbg dev_err
			#define pr_debug pr_err

			drivers/usb/host/xhci.h

			#define xhci_dbg(xhci, fmt, args...) \
			- dev_dbg(xhci_to_hcd(xhci)->self.controller , fmt , ## args)
			+ dev_err(xhci_to_hcd(xhci)->self.controller , fmt , ## args) 
			
			
			------------------------------------------------------------------------------
			3.
			Please add the following modifications and test.

			static void
			sd_spinup_disk(struct scsi_disk *sdkp)
			{
			unsigned char cmd[10];
			unsigned long spintime_expire = 0;
			int retries, spintime;
			unsigned int the_result;
			struct scsi_sense_hdr sshdr;
			int sense_valid = 0;

			spintime = 0;

			/* Spin up drives, as required. Only do this at boot time */
			/* Spinup needs to be done for module loads too. */
			do {
			retries = 0;

			do {
			cmd[0] = TEST_UNIT_READY;
			memset((void *) &cmd[1], 0, 9);

			the_result = scsi_execute_req(sdkp->device, cmd,
			DMA_NONE, NULL, 0,
			&sshdr, SD_TIMEOUT,
			SD_MAX_RETRIES, NULL);

			/*
			* If the drive has indicated to us that it
			* doesn't have any media in it, don't bother
			* with any more polling.
			*/
			+if(retries >25)
			+{
			if (media_not_present(sdkp, &sshdr))
			return;
			+}
			if (the_result)
			sense_valid = scsi_sense_valid(&sshdr);
			retries++;
			- } while (retries < 3 &&
			+ } while (retries < 30 &&
			(!scsi_status_is_good(the_result) ||
			((driver_byte(the_result) & DRIVER_SENSE) &&
			sense_valid && sshdr.sense_key == UNIT_ATTENTION)));

			Thanks!
	
	}
	


}	
	







msm8917 替换IC
{
	刘工 你好

	请看下面我们产品线负责软件的工程师的说明

	谢谢


	MSM8917平台的USB  phy可以做BC1.2的检测，usb phy驱动在drivers/usb/phy/phy-msm-usb.c文件

	 

	Charger驱动只要在检测到Adapter/USB插入后调用power_supply_set_present(usb, true)通知usb phy驱动去启动BC1.2的检测，
	检测完毕后会回调charger驱动的external_power_changed函数。

	针对使用bq24157的情况，我们会使用平台CBLPWR_N信号作为Adapter/USB插入判断，CBLPWR_N中断会调用bq2415x_cblpwr_changed()函数，
	这个函数schdeule delay work，执行bq2415x_vbus_changed_workfunc函数，

	在这个函数里会读取cblpwr_n的状态qpnp_pon_get_cblpwr_status(&bq->power_good); 让后根据cblpwr_n的状态判断adapter是插入还是拔出，
	然后调用power_supply_set_present 通知USB phy驱动
	
	
}














M2018
{
	1.调试PD协议是否通
	{
		相关的宏：
		CONFIG_USB_PD_POLICY=y
		CONFIG_QPNP_USB_PDPHY=y
	
		相关的代码：
		drivers/usb/pd/
			policy_engine.c
			qpnp-pdphy.c
			
		/sys/kernel/debug/ipc_logging/usb_pd
		
		cat /d/ipc_logging/usb_pd/log
		cat /sys/class/usbpd/usbpd0
			
		//Gionee <GN_BY_CHG> <lilubao> <20171206> add for debug usb pd begin	
		#define GN_USB_PD_DEBUG
		
		usbpd_err(&pd->dev, "in [%s] by lilubao \n",__FUNCTION__);
		
		
		1.msm-pm660.c
			pm660_charger: qcom,qpnp-smb2
				qcom,micro-usb;
				类型应该是type-c
				
			pm660_pdphy: qcom,usb-pdphy@1700 {
				compatible = "qcom,qpnp-pdphy";
				reg = <0x1700 0x100>;
				vdd-pdphy-supply = <&pm660l_l7>;
				vbus-supply = <&smb2_vbus>;
				vconn-supply = <&smb2_vconn>;

				status = "ok";	
		2.policy_engine.c
			module_param(disable_usb_pd, bool, S_IRUGO|S_IWUSR);
			模块传参 应该不是disable的	
			
		voter
		{
			pd_allowed_votable
				PD_DISALLOWED_INDIRECT_VOTER
				PD_VOTER
				CC_DETACHED_VOTER
				
			pd_disallowed_votable_indirect
				HVDCP_TIMEOUT_VOTER
				LEGACY_CABLE_VOTER
				VBUS_CC_SHORT_VOTER	
		
		}	
			
		PD3.0/PPS/or QC4.0	了解一下pps的一些内容，干什么的，有什么要求
		{
			
		
		}
	}


}
	
	






	
17G16A
{
	GNSPR#139252,【待机耗电】手机满电100%，插一张移动卡（卡未欠费，手机一直是搜网状态），
	开启数据业务，连接WIFI-AP，待机15小时，耗电19%
	{
		msm_otg 在待机时候一直持锁，wakelock
		
		msm_otg这个锁在pyh-msm-usb.c  申请了锁
		{

			wake_lock_init(&motg->wlock, WAKE_LOCK_SUSPEND, "msm_otg");
			
			关键字是：
			motg->wlock 
			
			两个相关的宏：
			CONFIG_PM_RUNTIME
			CONFIG_PM_SLEEP
		
			probe:
			
				device_init_wakeup(&pdev->dev, 1);
			
			
				/*
				 * PHY DVDD is supplied by a always on PMIC LDO (unlike
				 * vddcx/vddmx). PHY can keep D+ pull-up and D+/D-
				 * pull-down during suspend without any additional
				 * hardware re-work.
				 */
				if (motg->pdata->phy_type == SNPS_FEMTO_PHY)
					motg->caps |= ALLOW_BUS_SUSPEND_WITHOUT_REWORK;

				wake_lock(&motg->wlock);
			
			
				destroy_wlock:
				wake_lock_destroy(&motg->wlock);
		
			释放锁只有在suspend里面
			suspend：
				wake_unlock(&motg->wlock);
				
			待机时候一直持锁，就是没有进入suspend	
			
			
			<6>[  265.291149] *5)Calling platform_pm_suspend taking 3.309 msecs (c051000.sound)
			<6>[  265.291162] *5)        |- snd_soc_suspend
			<6>[  265.302587] *6)Calling mmc_bus_suspend taking 10.865 msecs (mmc0:0001)
			<3>[  265.303008] *6)dpm_run_callback(): platform_pm_suspend+0x0/0x50 returns -16
			<3>[  265.303018] *6)PM: Device alarmtimer failed to suspend: error -16
			<3>[  265.303026] *6)PM: Some devices failed to suspend, or early wake event detected
			<6>[  265.305296] *6)PM: resume of devices complete after 2.257 msecs
			
			
			CONFIG_PM_AUTOSLEEP=y
			CONFIG_PM_WAKELOCKS=y
			CONFIG_PM_WAKELOCKS_LIMIT=0
			# CONFIG_PM_WAKELOCKS_GC is not set
			CONFIG_PM_RUNTIME=y
			CONFIG_PM_DEBUG=y
			CONFIG_SUSPEND_TIME=y
			
			
			
			//Gionee <GN_BY_CHG> <lilubao> <20171205> add for debug otg wake lock begin
			
			
			adb root
			adb shell

			# Set the events to trace.
			echo "power:wakeup_source_activate" >> /d/tracing/set_event
			echo "power:wakeup_source_deactivate" >> /d/tracing/set_event

			# The default trace size for most devices is 1MB, which is relatively low and might cause the logs to overflow.
			# 8MB to 10MB should be a decent size for 5-6 hours of logging.

			echo 8192 > /d/tracing/buffer_size_kb

			echo 1 > /d/tracing/tracing_on


			
			adb shell dumpsys batterystats --reset
			adb shell dumpsys batterystats --enable full-wake-history
			
			
			
			echo 1 > /sys/module/msm_show_resume_irq/parameters/debug_mask
			echo 1 > /sys/kernel/debug/clk/debug_suspend
			echo 8 > /sys/module/mpm_of/parameters/debug_mask
			echo 32 > /sys/module/msm_pm/parameters/debug_mask

			CONFIG_USB_OTG_WAKELOCK
			
			申请专项测试
		
		}
		
	}

}	
	
	
	
	
	
	
	
	
	
	
	

	
	
	
	
	
	
17G10A
{
	




	
	
	电量显示不准                                             
	{
		特别是低电量充电的时候
		
		MTK case ID:ALPS03624068
		{
			Dear Customer
				如电话所谈：
				[dod_init_result] HW(38596, 4779) SW(38637, 4850) OLD(38698, 4949) VBAT(37942, 3172)
				[dod_init_result]: NVRAM_ready 1 Embedded 1 plug_out 1 is_hwocv_unreliable 0 rtc_invalid 1 rtc_ui_soc 0 two_sec_reboot 0 old_data.ui_soc 4970
				[dod_init_result]: T_new 27 T_old 39 T_d0 27 shutdown_time 0 pmic_shutdown_time 1800 plugout_time 31 plugout_time_th 32 swocv_oldocv_diff_emb 1000
				[dod_init_result]: <1> 38663 38698 4970 0 0 0 0
				[dod_init_result]: fg_c_d0(38663, 4893) fg_v_d0(38698, 4950) ui_d0_soc 4970 ui_soc_opt 0
				[dod_init_result]: force_vc_mode 0 vc_mode 0 d0_sel 0

				从log：[dod_init_result]: <1> 38663 38698 4970 0 0 0 0 可以看出，开机定位到第1路，即采用关机之前NVRAM里面的old_data。所以就5%这个点来说，
				判断到HW_OCV-OLD_SOC_TO_OCV<30mv，init flow就会认为是同一颗电池，就会拿NVRAM里面的值来进行开机电量的定位，保持5%。
				这样做的原因是我们不希望同一颗电池在拔出和插入之后，电量会有跳变。如果判定为同一颗电池，不会根据HW_OCV去定位新的电量。
				你们可以通过[dod_init_result] HW(38596, 4779) SW(38637, 4850) OLD(38698, 4949) VBAT(37942, 3172) 里面的HW_OCV 的值去和你们测量的OCV的值做对比，这个值显示真实的电量，看两者之间的差距。
				如果按照电话中您所说，希望开机会尽量去逼近真实的电量，一定要通过HW_OCV去重新定位开机电量，这个时候定位的电量虽然比较准确，但是就会有很大的概率发生UI电量的跳变（尤其是在电量比较低的情况下）。
				所以这就需要你们选择：是要UI比较平滑，客户看不到UI的跳变，认为电量比较准确，还是要UI开机时比较准确的显示电池的真实电量，但是会有很大的概率发生UI跳变。


				关于dod_init的大概flow和GM30的测试手法，我已经申请，等有结果我会告诉您。
				如果您有其他的问题，轻提出。

				感谢！
				
				
			如电话所谈：如果想修改仅为测试，可以修改mtk_battery_property.h里面的BAT_PLUG_OUT_TIME，改为30。这样修改后，开机就会根据hw_ocv的值重新定位开机电量，所以可能会引起UI的跳变。	
		
		
		
				实际判断电量到底准不准，应该用底层的SOC与查表得到的值进行对比，而不应该采用UI_SOC与查表的值进行对比。因为充电时，

			电流比较大，怀疑底层的SOC与UI_SOC有比较大的偏差，因为在算法上UI_SOC会在SOC的基础上作一些藏百分比的行为。建议先看Log。

			 

			1、进行实验，例如充电到UI_SOC到10%时，把Log导入出来，查看此时对应的SOC大小。

			2、再静置电池30分钟，查表得出值，与SOC大小进行比较。

			 

			搜索MTK_FG: [FGADC_intr_end]，如下方红色标注，可看出SOC、UI_SOC分别大小，看下他们的差距。

			MTK_FG: [FGADC_intr_end][FG_INTR_SHUTDOWN]soc:503 fg_c_soc:2184 fg_v_soc:503 ui_soc:348 vc_diff:1681 vc_mode 0 VBAT 33730 T:[11 V 10 C 17] D0_C 10000 D0_V 10000 CAR_V -37026 Q:[38986 38986] aging 10000 bat_cycle 0 Trk[0(-201):1:0] UI[0:1] Chr[0:10000:9978] pseudo1 301  DC_ratio 100


			//Gionee <GN_BY_CHG> <lilubao> <20171127> add for debug soc begin
			
			
			 debug:
			 {
			 	adb pull  /sdcard/mtklog/mobilelog    /home/llb/project/DEBUG/1.test_data/5.电量显示/log2/
			 
			  1. dod_init_result  这个定位的后面判断是几个类型是什么？还有后面的参数分别是什么意思
			12-09 17:31:09.029360     0     0 E [   40.798592][dod_init_result]: <5> 39834 39834 6210 1 0 1 1
			12-09 16:58:54.382134     0     0 E [  341.016964][dod_init_result]: <1> 38052 38169 3258 0 0 1 0

			   2.手机里面最后用的table 是mt6757_battery_table_ext.dtsi 还是哪个？我看log里面有一个
			fg_dump_table 跟之前zcv table不一样

			   3.
			我根据car的变化算流过电量计的电量，就是delta UI soc 的变化跟库伦计的变化量接近，但是delta soc的变化不同，请问，这两个soc，我看之前的
			文档说GM3.0的 电量计算 coulomb count + voltage 算的，这个soc是根据什么算
			2-09 17:34:13.192588   480   480 E [  122.151072] (0)[480:fuelgauged]MTK_FG: [FGADC_intr_end]: [FG_INTR_COULOMB_C]soc:6252 fg_c_soc:6252 fg_v_soc:6091 ui_soc:6382 vc_diff:161 vc_mode 1 VBAT 43190 T:[33 V 32 C 28] D0_C 6210 D0_V 6091 CAR_V 0 Q:[28155 28155] aging 10000 bat_cycle 0 Trk[0:0:0] UI[0:0] Chr[1:0:0] pseudo1 0  DC_ratio 100!

			12-09 17:31:09.024791   186   186 E [   40.794023] (0)[186:battery_thread]: [fg_drv_update_hw_status] current:9071 3166 state:0 0 car:-58 -76 bat:3822 3918 chr:0 0 hwocv:1234 1234 bat_plug_out:1 1 tmp:29 29 imix 24070 rac 436

				
				
				
				
		01-01 08:00:26.827611   483   483 E [    8.402427] (3)[483:fuelgauged]: MTK_FG: [fg_dump_table]table idx:255 (i,mah,voltage,resistance,percentage)
		01-01 08:00:26.827670   483   483 E [    8.402486] (3)[483:fuelgauged]: MTK_FG:  ( 0,    0,43850, 1510,  0) ( 1,  305,43705, 1509,105) ( 2,  609,43577, 1513,210) ( 3,  914,43460, 1509,315) ( 4, 1219,43343, 1516,420)
		01-01 08:00:26.827730   483   483 E [    8.402546] (3)[483:fuelgauged]: MTK_FG:  ( 5, 1524,43228, 1519,526) ( 6, 1828,43109, 1515,631) ( 7, 2133,42990, 1515,736) ( 8, 2438,42870, 1515,841) ( 9, 2742,42750, 1519,946)
		01-01 08:00:26.827791   483   483 E [    8.402607] (3)[483:fuelgauged]: MTK_FG:  (10, 3047,42630, 1528,1051) (11, 3352,42512, 1522,1157) (12, 3657,42394, 1525,1262) (13, 3961,42277, 1533,1367) (14, 4266,42160, 1553,1472)
		01-01 08:00:26.827851   483   483 E [    8.402667] (3)[483:fuelgauged]: MTK_FG:  (15, 4571,42042, 1568,1577) (16, 4875,41925, 1575,1682) (17, 5180,41809, 1588,1788) (18, 5485,41694, 1595,1893) (19, 5790,41580, 1600,1998)
		01-01 08:00:26.827913   483   483 E [    8.402729] (3)[483:fuelgauged]: MTK_FG:  (20, 6094,41467, 1609,2103) (21, 6399,41356, 1618,2208) (22, 6704,41245, 1628,2314) (23, 7008,41134, 1636,2419) (24, 7313,41025, 1643,2524)
		01-01 08:00:26.827975   483   483 E [    8.402791] (3)[483:fuelgauged]: MTK_FG:  (25, 7618,40918, 1651,2629) (26, 7923,40811, 1647,2735) (27, 8227,40708, 1653,2840) (28, 8532,40610, 1677,2945) (29, 8837,40516, 1712,3050)
		01-01 08:00:26.828037   483   483 E [    8.402853] (3)[483:fuelgauged]: MTK_FG:  (30, 9142,40415, 1723,3155) (31, 9446,40306, 1741,3260) (32, 9751,40198, 1747,3366) (33,10056,40095, 1758,3471) (34,10360,40004, 1774,3576)
		01-01 08:00:26.828097   483   483 E [    8.402913] (3)[483:fuelgauged]: MTK_FG:  (35,10665,39922, 1788,3681) (36,10970,39842, 1802,3786) (37,11275,39762, 1822,3892) (38,11579,39679, 1827,3997) (39,11884,39592, 1839,4102)
		01-01 08:00:26.828160   483   483 E [    8.402976] (3)[483:fuelgauged]: MTK_FG:  (40,12189,39501, 1836,4207) (41,12493,39403, 1816,4312) (42,12798,39289, 1762,4417) (43,13103,39158, 1681,4523) (44,13408,39030, 1604,4628)
		01-01 08:00:26.828221   483   483 E [    8.403037] (3)[483:fuelgauged]: MTK_FG:  (45,13712,38923, 1558,4733) (46,14017,38836, 1536,4838) (47,14322,38759, 1520,4944) (48,14626,38690, 1509,5049) (49,14931,38626, 1507,5154)
		01-01 08:00:26.828282   483   483 E [    8.403098] (3)[483:fuelgauged]: MTK_FG:  (50,15236,38566, 1505,5259) (51,15541,38511, 1502,5364) (52,15845,38457, 1506,5469) (53,16150,38405, 1511,5575) (54,16455,38356, 1511,5680)
		01-01 08:00:26.828344   483   483 E [    8.403160] (3)[483:fuelgauged]: MTK_FG:  (55,16759,38308, 1513,5785) (56,17064,38263, 1523,5890) (57,17369,38220, 1531,5995) (58,17674,38179, 1536,6101) (59,17978,38139, 1543,6206)
		01-01 08:00:26.828407   483   483 E [    8.403223] (3)[483:fuelgauged]: MTK_FG:  (60,18283,38101, 1550,6311) (61,18588,38065, 1564,6416) (62,18892,38029, 1573,6521) (63,19197,37996, 1576,6626) (64,19502,37964, 1587,6732)
		01-01 08:00:26.828469   483   483 E [    8.403285] (3)[483:fuelgauged]: MTK_FG:  (65,19807,37934, 1589,6837) (66,20111,37902, 1596,6942) (67,20416,37877, 1609,7047) (68,20721,37846, 1612,7153) (69,21025,37813, 1611,7258)
		01-01 08:00:26.828532   483   483 E [    8.403348] (3)[483:fuelgauged]: MTK_FG:  (70,21330,37776, 1588,7363) (71,21635,37734, 1568,7468) (72,21940,37683, 1547,7573) (73,22244,37640, 1531,7678) (74,22549,37597, 1531,7784)
		01-01 08:00:26.828594   483   483 E [    8.403410] (3)[483:fuelgauged]: MTK_FG:  (75,22854,37552, 1523,7889) (76,23159,37508, 1513,7994) (77,23463,37468, 1515,8099) (78,23768,37432, 1521,8204) (79,24073,37401, 1537,8310)
		01-01 08:00:26.828657   483   483 E [    8.403473] (3)[483:fuelgauged]: MTK_FG:  (80,24377,37365, 1553,8415) (81,24682,37320, 1555,8520) (82,24987,37264, 1550,8625) (83,25292,37201, 1539,8731) (84,25596,37143, 1542,8835)
		01-01 08:00:26.828720   483   483 E [    8.403536] (3)[483:fuelgauged]: MTK_FG:  (85,25901,37087, 1561,8941) (86,26206,37016, 1563,9046) (87,26510,36938, 1540,9151) (88,26815,36895, 1539,9256) (89,27120,36879, 1562,9362)
		01-01 08:00:26.828783   483   483 E [    8.403599] (3)[483:fuelgauged]: MTK_FG:  (90,27425,36866, 1593,9467) (91,27729,36853, 1636,9572) (92,28034,36832, 1692,9677) (93,28339,36790, 1753,9782) (94,28643,36645, 1751,9887)
		01-01 08:00:26.828846   483   483 E [    8.403662] (3)[483:fuelgauged]: MTK_FG:  (95,28948,36290, 1714,9993) (96,29253,35797, 1774,10098) (97,29558,35140, 1888,10203) (98,29862,34209, 2085,10308) (99,30167,32634, 2726,10413)
		01-01 08:00:26.828963   483   483 E [    8.403779] (3)[483:fuelgauged]: MTK_FG: [fg_dump_table]table idx:254 (i,mah,voltage,resistance,percentage)
		01-01 08:00:26.829030   483   483 E [    8.403846] (3)[483:fuelgauged]: MTK_FG:  ( 0,    0,43850, 1510,  0) ( 1,  305,43705, 1509,105) ( 2,  609,43577, 1513,210) ( 3,  914,43460, 1509,315) ( 4, 1219,43343, 1516,420)
		01-01 08:00:26.829089   483   483 E [    8.403905] (3)[483:fuelgauged]: MTK_FG:  ( 5, 1524,43228, 1519,526) ( 6, 1828,43109, 1515,631) ( 7, 2133,42990, 1515,736) ( 8, 2438,42870, 1515,841) ( 9, 2742,42750, 1519,946)
		01-01 08:00:26.829149   483   483 E [    8.403965] (3)[483:fuelgauged]: MTK_FG:  (10, 3047,42630, 1528,1051) (11, 3352,42512, 1522,1157) (12, 3657,42394, 1525,1262) (13, 3961,42277, 1533,1367) (14, 4266,42160, 1553,1472)
		01-01 08:00:26.829211   483   483 E [    8.404027] (3)[483:fuelgauged]: MTK_FG:  (15, 4571,42042, 1568,1577) (16, 4875,41925, 1575,1682) (17, 5180,41809, 1588,1788) (18, 5485,41694, 1595,1893) (19, 5790,41580, 1600,1998)
		01-01 08:00:26.829277   483   483 E [    8.404093] (3)[483:fuelgauged]: MTK_FG:  (20, 6094,41467, 1609,2103) (21, 6399,41356, 1618,2208) (22, 6704,41245, 1628,2314) (23, 7008,41134, 1636,2419) (24, 7313,41025, 1643,2524)
		01-01 08:00:26.829338   483   483 E [    8.404154] (3)[483:fuelgauged]: MTK_FG:  (25, 7618,40918, 1651,2629) (26, 7923,40811, 1647,2735) (27, 8227,40708, 1653,2840) (28, 8532,40610, 1677,2945) (29, 8837,40516, 1712,3050)
		01-01 08:00:26.829399   483   483 E [    8.404215] (3)[483:fuelgauged]: MTK_FG:  (30, 9142,40415, 1723,3155) (31, 9446,40306, 1741,3260) (32, 9751,40198, 1747,3366) (33,10056,40095, 1758,3471) (34,10360,40004, 1774,3576)
		01-01 08:00:26.829460   483   483 E [    8.404276] (3)[483:fuelgauged]: MTK_FG:  (35,10665,39922, 1788,3681) (36,10970,39842, 1802,3786) (37,11275,39762, 1822,3892) (38,11579,39679, 1827,3997) (39,11884,39592, 1839,4102)
		01-01 08:00:26.829521   483   483 E [    8.404337] (3)[483:fuelgauged]: MTK_FG:  (40,12189,39501, 1836,4207) (41,12493,39403, 1816,4312) (42,12798,39289, 1762,4417) (43,13103,39158, 1681,4523) (44,13408,39030, 1604,4628)
		01-01 08:00:26.829589   483   483 E [    8.404405] (3)[483:fuelgauged]: MTK_FG:  (45,13712,38923, 1558,4733) (46,14017,38836, 1536,4838) (47,14322,38759, 1520,4944) (48,14626,38690, 1509,5049) (49,14931,38626, 1507,5154)
		01-01 08:00:26.829650   483   483 E [    8.404466] (3)[483:fuelgauged]: MTK_FG:  (50,15236,38566, 1505,5259) (51,15541,38511, 1502,5364) (52,15845,38457, 1506,5469) (53,16150,38405, 1511,5575) (54,16455,38356, 1511,5680)
		01-01 08:00:26.829713   483   483 E [    8.404529] (3)[483:fuelgauged]: MTK_FG:  (55,16759,38308, 1513,5785) (56,17064,38263, 1523,5890) (57,17369,38220, 1531,5995) (58,17674,38179, 1536,6101) (59,17978,38139, 1543,6206)
		01-01 08:00:26.829775   483   483 E [    8.404591] (3)[483:fuelgauged]: MTK_FG:  (60,18283,38101, 1550,6311) (61,18588,38065, 1564,6416) (62,18892,38029, 1573,6521) (63,19197,37996, 1576,6626) (64,19502,37964, 1587,6732)
		01-01 08:00:26.829837   483   483 E [    8.404653] (3)[483:fuelgauged]: MTK_FG:  (65,19807,37934, 1589,6837) (66,20111,37902, 1596,6942) (67,20416,37877, 1609,7047) (68,20721,37846, 1612,7153) (69,21025,37813, 1611,7258)
		01-01 08:00:26.829899   483   483 E [    8.404715] (3)[483:fuelgauged]: MTK_FG:  (70,21330,37776, 1588,7363) (71,21635,37734, 1568,7468) (72,21940,37683, 1547,7573) (73,22244,37640, 1531,7678) (74,22549,37597, 1531,7784)
		01-01 08:00:26.829962   483   483 E [    8.404778] (3)[483:fuelgauged]: MTK_FG:  (75,22854,37552, 1523,7889) (76,23159,37508, 1513,7994) (77,23463,37468, 1515,8099) (78,23768,37432, 1521,8204) (79,24073,37401, 1537,8310)
		01-01 08:00:26.830025   483   483 E [    8.404841] (3)[483:fuelgauged]: MTK_FG:  (80,24377,37365, 1553,8415) (81,24682,37320, 1555,8520) (82,24987,37264, 1550,8625) (83,25292,37201, 1539,8731) (84,25596,37143, 1542,8835)
		01-01 08:00:26.830087   483   483 E [    8.404903] (3)[483:fuelgauged]: MTK_FG:  (85,25901,37087, 1561,8941) (86,26206,37016, 1563,9046) (87,26510,36938, 1540,9151) (88,26815,36895, 1539,9256) (89,27120,36879, 1562,9362)
		01-01 08:00:26.830148   483   483 E [    8.404964] (3)[483:fuelgauged]: MTK_FG:  (90,27425,36866, 1593,9467) (91,27729,36853, 1636,9572) (92,28034,36832, 1692,9677) (93,28339,36790, 1753,9782) (94,28643,36645, 1751,9887)
		01-01 08:00:26.830212   483   483 E [    8.405028] (3)[483:fuelgauged]: MTK_FG:  (95,28948,36290, 1714,9993) (96,29253,35797, 1774,10098) (97,29558,35140, 1888,10203) (98,29862,34209, 2085,10308) (99,30167,32634, 2726,10413)
			 
			 	
			 }	

		}
	
	}	
	
	
	
	
	
	
	
	
	
	
	
	不充电问题
	{
		GNSPR #141086,电源管理：玩王者荣耀的时候插标配充电器充电玩游戏1H，电量只增加0%，查看充电记录，
		玩游戏的时候充电电流为不足10MA
		{
		
			这部分跟thermal有关，但是input设为0关闭了充电
			//lilubao 
			<3>[20189.850296]  (0)[239:charger_thread]force:0 thermal:-1 300000 setting:2300000 0 type:4 usb_unlimited:0 usbif:0 usbsm:0 aicl:-1
			<6>[20189.850308]  (0)[239:charger_thread]rt5081_pmu_charger rt5081_pmu_charger: __rt5081_set_aicr: aicr = 2300000 (0x2C)
			<7>[20189.850319]  (0)[239:charger_thread]rt5081_pmu 5-0034: rt5081_pmu_reg_update_bits: reg 13 data b0
			<7>[20189.850327]  (0)[239:charger_thread]rt5081_pmu 5-0034: rt5081_pmu_reg_update_bits: mask fc
			<7>[20189.850624]  (0)[239:charger_thread]rt5081_pmu 5-0034: rt5081_pmu_reg_block_write: reg 07 size 4
			<6>[20189.850890]  (0)[239:charger_thread]rt5081_pmu_charger rt5081_pmu_charger: rt5081_enable_hidden_mode: en = 1
			<7>[20189.850902]  (0)[239:charger_thread]rt5081_pmu 5-0034: rt5081_pmu_reg_write: reg 07 data 00
			<6>[20189.850973]  (0)[239:charger_thread]rt5081_pmu_charger rt5081_pmu_charger: rt5081_enable_hidden_mode: en = 0
			<6>[20189.850982]  (0)[239:charger_thread]rt5081_pmu_charger rt5081_pmu_charger: __rt5081_set_ichg: ichg = 500000 (0x04)
			<7>[20189.850992]  (0)[239:charger_thread]rt5081_pmu 5-0034: rt5081_pmu_reg_update_bits: reg 17 data 10
			<7>[20189.850999]  (0)[239:charger_thread]rt5081_pmu 5-0034: rt5081_pmu_reg_update_bits: mask fc
			<7>[20189.851136]  (0)[239:charger_thread]rt5081_pmu 5-0034: rt5081_pmu_reg_read: reg 17

			//lilubao 
			<3>[20189.851216]  (0)[239:charger_thread][charger]charging current is set 0mA, turn off charging !
			<6>[20189.851225]  (0)[239:charger_thread]rt5081_pmu_charger rt5081_pmu_charger: rt5081_enable_charging: en = 0
			<7>[20189.851234]  (0)[239:charger_thread]rt5081_pmu 5-0034: rt5081_pmu_reg_update_bits: reg 12 data 00
			
			
			0x27=0x60   0110 0000 
			 SDP NSTD (by input pin of (sVBUSPG_syn & sCHGDETB & DCDT)=1)
			 Charger port is not detected
			 
			 
			 
			 static void swchg_turn_on_cha
			{
				struct switch_charging_alg_data *
				bool charging_enable = true;

				if (swchgalg->state == CHR_ERRO
					charging_enable = false;
					pr_err("[charger]Charger Erro
				} else if ((get_boot_mode() == 
					charging_enable = false;
					pr_err("[charger]In meta or a
				} else {
					mtk_pe20_start_algorithm(i
					mtk_pe_start_algorithm(inf

					swchg_select_charging_cur
					if (info->chg1_data.input_cur
						charging_enable = false;
						pr_err("[charger]chargin
					} else {
						swchg_select_cv(info);
					}
				}

				charger_dev_enable(info->chg1
			}
		
			充电相关的参数
			{
				 DIFFERENCE_FULLOCV_ITH
			
			}


		
		
		

		
			还有这个问题，irq不匹配的问题
			<6>[21329.846831]  (5)[239:charger_thread]rt5081_pmu_charger rt5081_pmu_charger: rt5081_enable_irq: (chg_mivr) en = 1
			<7>[21329.846844]  (5)[239:charger_thread]rt5081_pmu 5-0034: rt5081_pmu_reg_block_read: reg e0 size 16
			<4>[21329.846857] -(5)[239:charger_thread]------------[ cut here ]------------
			<4>[21329.846862] -(5)[239:charger_thread]WARNING: CPU: 5 PID: 239 at /data/MAIN_GIT_REPO_CODE/BJ17G10A_MAIN_REPO/L31_6757_66_N_17G10A_NO.MP5_V1.53_170512_ALPS/L31_6757_66_N_17G10A_NO.MP5_V1.53_170512_ALPS/android_mtk_mp/kernel-4.4/kernel/irq/manage.c:513 enable_irq+0x88/0xcc()
			<4>[21329.846880] -(5)[239:charger_thread]Unbalanced enable for IRQ 166
			<4>[21329.846887] -(5)[239:charger_thread]CPU: 5 PID: 239 Comm: charger_thread Tainted: G        W       4.4.15 #1
			<4>[21329.846894] -(5)[239:charger_thread]Hardware name: MT6757CD (DT)
			<0>[21329.846899] -(5)[239:charger_thread]Call trace:
			<4>[21329.846903] -(5)[239:charger_thread][<ffffffc00008a328>] dump_backtrace+0x0/0x14c
			<4>[21329.846913] -(5)[239:charger_thread][<ffffffc00008a488>] show_stack+0x14/0x1c
			<4>[21329.846919] -(5)[239:charger_thread][<ffffffc0003379b0>] dump_stack+0x8c/0xb0
			<4>[21329.846930] -(5)[239:charger_thread][<ffffffc00009e5f8>] warn_slowpath_fmt+0xc0/0xf4
			<4>[21329.846940] -(5)[239:charger_thread][<ffffffc0000fd598>] enable_irq+0x88/0xcc
			<4>[21329.846945] -(5)[239:charger_thread][<ffffffc0004d8310>] rt5081_enable_power_path+0x148/0x190
			<4>[21329.846957] -(5)[239:charger_thread][<ffffffc000945750>] charger_dev_enable_powerpath+0x24/0x34
			<4>[21329.846968] -(5)[239:charger_thread][<ffffffc000948ba0>] charger_routine_thread+0x378/0x6a8
			<4>[21329.846976] -(5)[239:charger_thread][<ffffffc0000be268>] kthread+0xdc/0xf0
			<4>[21329.846987] -(5)[239:charger_thread][<ffffffc000085cd0>] ret_from_fork+0x10/0x40
			<4>[21329.846994] -(5)[239:charger_thread]---[ end trace a4903c9f998f3193 ]---	
			
			
		
		}	
	}



	
	GNSPR#139202，去除冗余log
	{
		//Gionee <GN_BY_CHG> <lilubao> <20171130> remove redundant log begin
		mtk_battery.c 
		
		g_FG_PSEUDO100_T0  这个量产之前去掉
	
	}
	
	
	
	
	


	
	GNSPR#101075，待机界面进入相机，相机黑屏5s后自动恢复
	{
		压力测试,之前的温度已经很高了
		106386
		
		case ID:ALPS03628177
		{
			Dear customer
				您的意思是压力测试，手机升温后此问题才会发生么？那有可能是升温后，cpu性能被限制打出的。
			您帮忙抓一份userdebug版本的systrace+mtklog，正常与不正常时候的对比log给我们check下。感谢
		
		
			Dear customer
				抓systrace的方式可以上我司DCC下载文档How_to_use_ftrace_to_Customer.pptx 后面几页有介绍具体操作步骤：
				基本上来说就是使用我司ftrace_all_in_one工具，
				1、设置tag，文档中介绍的是手动使用adb命令设置。我给您release一个bat文件，设置camera需要的tag，您在抓取之前直接先运行此bat文件就好。
				2、运行catch.bat
				3、复现问题
				4、停止抓取
				具体操作请参考文档中的说明（请帮忙使用userdebug版本的load）。

				感谢
				
			复测步骤
			{
				2.
				MTK 获取root
				– adb shell setprop service.adb.root 1
				– 进入Settings->Developer options,找到USB
				Debugging,将此选项先取消勾选,并再次勾选上

				3.adb root 查看是否已经有了root权限

				4.解压压缩包里软件，

				5.测试之前开启mobilelog
				先运行 systrace-setup-tk-camera-only.bat，这个是设置需要抓取的log

				6.然后运行01-catch.bat，按照press any key开始抓log，

				7.然后复测这个case

				8.再press any key 停止抓取ftrace

				9.把这个目录下的SYS_TRACE和cputime.txt ，moblelog都发给我
			}		
		
		}

	}

















	GNSPR#100830,充电时按开机键开机，测试值是6.83s，标准值是4.5s，超出标准值2.33s
	{
		现在的关机充电条件下按powerkey到亮logo时间太长
		
		相关代码
		{
			key_chontrol.cpp  key_control
			
			linux-event-codes.h描述event
		
		}
		
		
		现在主要分两部分分析：powerkey+usb handshake
		{
			//powerkey
			
			

			[PMIC] pl pmic powerkey Release
			[PMIC] pmic_IsUsbCableIn 1
			[PLFM] USB/charger boot!
			[PMIC] PMIC_POWER_HOLD ON



			mtk detect key function pmic_detect_homekey MTK_PMIC_RST_KEY = 17
			[PMIC] pl pmic FCHRKEY Release




			[   11.483398] <4>.(4)[295:kpoc_charger]charger: draw_with_interval... key_trigger_suspend = 0
			[   11.499852] <4>.(4)[70:pmic_thread][name:pmic_irq&][PMIC] [PMIC_INT] addr[0x854]=0x1
			[   11.500840] <4>.(4)[70:pmic_thread][name:kpd&]kpd: Power Key generate, pressed=1
			[   11.501783] <4>.(4)[70:pmic_thread][name:hal_kpd&]kpd: kpd: (pressed) HW keycode =116 using PMIC
			[   11.502868] <4>.(4)[70:pmic_thread][name:aed&](pressed) HW keycode powerkey
			[   11.503837] <4>.(4)[276:kpoc_charger]charger: key_control: event.type:1,116:1
			[   11.504766] <4>.(4)[276:kpoc_charger]charger: key_control: event.type:0,0:0
			[   11.505666] <4>.(4)[296:kpoc_charger]charger: pwr key long press check start




			//usb handshake

			[SEC] read '0x8800000'
			0x4D,0x4D,0x4D,0x4D,0x4,0x0,0x0,0x0,
			[LIB] seclib_img_auth_load_sig [LIB] CFG read size '0x2000' '0x3C'
			0x4D4D4D4D
			[LIB] SEC CFG 'v4' exists
			[LIB] HW DEC
			GCPU Enhance,V1.1
			[LIB] SEC CFG is valid. Lock state is 1 
			[BLDR] Starting tool handshake.
			€€€€€€€€€€€[BLDR] Tool connection is unlocked
			[platform_vusb_on] VUSB33 is on
			[platform_vusb_on] VA10 is on
			[platform_vusb_on] VA10 select to 0.9V
			rt5081_enable_chgdet_flow: en = 0
			rt5081_enable_chgdet_flow: en = 1
			mtk_ext_chgdet: usb_stats = 0x00000020
			mtk_ext_chgdet: chg type = 1

			[PLFM] USB cable in
			[TOOL] USB enum timeout (Yes), handshake timeout(Yes)
			[TOOL] Enumeration(Start)
			HS is detected
			HS is detected
			[TOOL] Enumeration(End): OK 524ms 
			usbdl_flush timeoutintrep :0, IntrTx[0] IntrRx [0]usbdl_flush timeoutintrep :0, IntrTx[0] IntrRx [0]usbdl_flush timeoutintrep :0, IntrTx[0] IntrRx [0]usbdl_flush timeoutintrep :0, IntrTx[0] IntrRx [0]usbdl_flush timeoutintrep :0, IntrTx[0] IntrRx [0][TOOL] : usb listen timeout
			[TOOL] <USB> cannot detect tools!
			[TOOL] <UART> listen  ended, receive size:0!

			[TOOL] <UART> wait sync time 150ms->5ms
			[TOOL] <UART> receieved data: ()
		
		}
		
		
	
	}
	



	GNSPR#128052,测机连接电脑USB端口，下拉状态栏显示USB已连接，传文件打开，电脑不显示便捷设备，换电脑，插拔USB端口不恢复，
	重启恢复
	{

		Dear Customer，

		您好，如果您的测试版本是eng版本，请把selinux关掉试试，即adb shell setenforce 0；
		或者是加上如下selinux 规则试试，谢谢！
		#====================== untrusted_app.te ======================
		allow untrusted_app mtp_device:chr_file rw_file_perms; 	
		
		
		等待测试复测,这里有一个selinux是什么，有什么作用
		1.user版本不能修改selinux的开关；
		2.如果要编译untrust_app.te，单独编译boot就可以生效；
		
		
		selinux qcom有一个fatab.com里面是一些挂载执行的命令     
		
		代码路径
		external/sepolicy – Google define
		device/qcom/sepolicy/common – Qualcomm define
		
		
		Disable/enable
		Kernel command line
		adb shell setenforce 0
		
		device/qcom/sepolicy/common/qmuxd.te
		
		
		G1605B_sign.mk
		#========== Security Boot switch start==================
		#gionee, duanyh, for verified boot, begin
		CONFIG_GN_BSP_MTK_SBC_FLAG=y
		MTK_SECURITY_SW_SUPPORT=yes
		CONFIG_MTK_SECURITY_SW_SUPPORT=yes
		MTK_SEC_CHIP_SUPPORT=yes	
		MTK_SEC_USBDL=ATTR_SUSBDL_ENABLE
		MTK_SEC_BOOT=ATTR_SBOOT_ENABLE
		MTK_SEC_MODEM_AUTH=no
		MTK_SEC_SECRO_AC_SUPPORT=yes
		CUSTOM_SEC_AUTH_SUPPORT=no 
		MTK_VERIFIED_BOOT_SUPPORT=yes
		MTK_SEC_FASTBOOT_UNLOCK_SUPPORT=no 
		MTK_SEC_FASTBOOT_UNLOCK_KEY_SUPPORT=no
		#gionee, duanyh, for verified boot, end

		#gionee ranyi add for gionee security boot start
		GN_SECURITY_BOOT_SW_SUPPORT=yes
		GN_SECURITY_BOOT_1.1_TRUSTZONE_KEY_SUPPORT=yes
		#gionee ranyi add for gionee security boot end

		#wangguojun add for secure boot
		GN_VERIFIED_BOOT_GN_MP_KEY = yes

		#wangguojun add for kph build
		GN_KPH_BUILD_SUPPORT=yes
		#========== Security Boot switch end==================
		
		
		How do I disable and enable verity via adb?
		A. adb disable-verity
		disable dm-verity checking on USERDEBUG builds
		adb enable-verity
		re-enable dm-verity checking on USERDEBUG builds
		source code: system/core/adb/commandline.cpp
	}



	

}	
	
	
	
	
	
	

	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	









	
























/*********************************************************************************************************************************/
17G06A
{





	金刚3续航问题
	{
		1.GNSPR#133023,【视频续航】金刚3视频续航（外放）比华为畅享7平均电流高50mA左右
	
		2.对比M7续航时间差很多
		
		3.中批比T2机械手臂时间差一个小时

	}




	dump pimc register,80-NL708-1
	{
		未成功，还是有问题，寄存器信息没有打印出来

		Mount debug file system
			adb shell mount -t debugfs none /sys/kernel/debug
			cd /sys/kernel/debug/spmi/spmi-0
			
		Set number of bytes to read/write
			echo 1 > count
		
		Set address
			echo 0x8041 > address
		
		write 
			echo 0x11 > data
		
		read 
			cat data
			
			
		Example – Set GPIO1 to output high and read back other parameters
			adb shell mount -t debugfs none /sys/kernel/debug
			cd /sys/kernel/debug/spmi/spmi-0
			echo 1 > count
			echo 0xC040 > address // MODE_CTL
			echo 0x11 > data
			// DO + HIGH
			echo 0xC041 > address // DIG_VIN_CTL
			cat data	
					



		SPMI Debug-FS support
		
		Hierarchy schema:
		/sys/kernel/debug/spmi
			   /help            -- Static help text
			   /spmi-0          -- Directory for SPMI bus 0
			   /spmi-0/address  -- Starting register address for reads or writes
			   /spmi-0/count    -- Number of registers to read (only used for reads)
			   /spmi-0/data     -- Initiates the SPMI read (formatted output)
			   /spmi-0/data_raw -- Initiates the SPMI raw read or write
			   /spmi-n          -- Directory for SPMI bus n

		To perform SPMI read or write transactions, you need to first write the
		address of the slave device register to the 'address' file.  For read
		transactions, the number of bytes to be read needs to be written to the
		'count' file.

		The 'address' file specifies the 20-bit address of a slave device register.
		The upper 4 bits 'address[19..16]' specify the slave identifier (SID) for
		the slave device.  The lower 16 bits specify the slave register address.

		Reading from the 'data' file will initiate a SPMI read transaction starting
		from slave register 'address' for 'count' number of bytes.

		Writing to the 'data' file will initiate a SPMI write transaction starting
		from slave register 'address'.  The number of registers written to will
		match the number of bytes written to the 'data' file.

		Example: Read 4 bytes starting at register address 0x1234 for SID 2

		echo 0x21234 > address
		echo 4 > count
		cat data

		Example: Write 3 bytes starting at register address 0x1008 for SID 1

		echo 0x11008 > address
		echo 0x01 0x02 0x03 > data

		Note that the count file is not used for writes.  Since 3 bytes are
		written to the 'data' file, then 3 bytes will be written across the
		SPMI bus.
		
		
		
		static struct fg_mem_data fg_data[FG_DATA_MAX] = {
			/*       ID           Address, Offset, Length, Value*/
			DATA(BATT_TEMP,       0x550,   2,      2,     -EINVAL),
			DATA(OCV,             0x588,   3,      2,     -EINVAL),
			DATA(VOLTAGE,         0x5CC,   1,      2,     -EINVAL),
			DATA(CURRENT,         0x5CC,   3,      2,     -EINVAL),
			DATA(BATT_ESR,        0x554,   2,      2,     -EINVAL),
			DATA(BATT_ESR_COUNT,  0x558,   2,      2,     -EINVAL),
			DATA(BATT_SOC,        0x56C,   1,      3,     -EINVAL),
			DATA(CC_CHARGE,       0x570,   0,      4,     -EINVAL),
			DATA(VINT_ERR,        0x560,   0,      4,     -EINVAL),
			DATA(CPRED_VOLTAGE,   0x540,   0,      2,     -EINVAL),
			DATA(BATT_ID,         0x594,   1,      1,     -EINVAL),
			DATA(BATT_ID_INFO,    0x594,   3,      1,     -EINVAL),
		};
		
	//Gionee <GN_BSP_CHG> <lilubao> <20171101> modify for pmic register dump begin
	dev_err(&sdev->dev, "in [%s] by lilubao addr->%d,buf->%s,len->%d,sid->%d\n",
		__FUNCTION__,addr,buf,len,sid);
	//Gionee <GN_BSP_CHG> <lilubao> <20171101> modify for pmic register dump end
	
	}

}






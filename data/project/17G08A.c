

/**********************************************************************************************************************/
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

	

	
	
	
		
	
/***********************************************************************************************************************************/
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
			POWER_SUPPLY_TYPE_USB_PD,	/* Power Delivery */					10
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




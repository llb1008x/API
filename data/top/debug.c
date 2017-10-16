


17G10A
{

		适配平均电流apk，上报电量计的值
	{
		{
			1.首先要知道电量计是什么函数或者变量获取的，单位，数值是什么？
			
			2.创建相关的节点，上层怎么读取数据，要不要经过转换
			
			
			qpnp-smbcharger.c
			
			static ssize_t coulomb_count_show(struct device* dev, struct device_attribute* attr, char* buf)
			{
				struct power_supply *psy = dev_get_drvdata(dev);
				struct smbchg_chip *chip = container_of(psy,
							struct smbchg_chip, batt_psy);
				int coulomb_count, rc;
				rc = get_property_from_fg(chip, POWER_SUPPLY_PROP_CHARGE_NOW_RAW, &coulomb_count);
				//Gionee <GN_BSP_CHG> <lilubao> <20171016> add for update coulomb_count begin
				pr_err("in [%s] by lilubao use coulomb count\n",__FUNCTION__);
				//Gionee <GN_BSP_CHG> <lilubao> <20171016> add for update coulomb_count end
				return sprintf(buf, "%d\n", coulomb_count/1000);
			}
			
			static DEVICE_ATTR(coulomb_count, 0664, coulomb_count_show, NULL);
			
			//Gionee <GN_BSP_CHG> <liujiang> <20170408> add for 107204 begin
			rc = device_create_file(chip->batt_psy.dev, &dev_attr_coulomb_count); 
			//Gionee <GN_BSP_CHG> <liujiang> <20170408> add for 107204 end
			
			
			frameworks/base/services/core/java/com/android/server/DrvInspectCoulomb_count.java
			
			
			MTK平台通过什么获取电量计参数的
			
			battery_meter_ctrl(BATTERY_METER_CMD_GET_FG_HW_CAR, &fg_coulomb);
			
			
			//Gionee <GN_BSP_CHG> <lilubao> <20171016> add for update coulomb_count begin
			static ssize_t show_coulomb_count(struct device* dev, struct device_attribute* attr, char* buf)
			{
				int coulomb_count;
	
				pr_err("in show_coulomb_count before by lilubao\n");

				battery_meter_ctrl(BATTERY_METER_CMD_GET_FG_HW_CAR, &coulomb_count);
				return sprintf(buf, "%d\n", coulomb_count);

			#if	0
				struct power_supply *psy = dev_get_drvdata(dev);
				struct smbchg_chip *chip = container_of(psy,
							struct smbchg_chip, batt_psy);
				int coulomb_count, rc;
				rc = get_property_from_fg(chip, POWER_SUPPLY_PROP_CHARGE_NOW_RAW, &coulomb_count);
				//Gionee <GN_BSP_CHG> <lilubao> <20171016> add for update coulomb_count begin
				pr_err("in [%s] by lilubao use coulomb count\n",__FUNCTION__);
				//Gionee <GN_BSP_CHG> <lilubao> <20171016> add for update coulomb_count end
				return sprintf(buf, "%d\n", coulomb_count/1000);
			#endif	
			}

			static DEVICE_ATTR(coulomb_count, 0664,show_coulomb_count, NULL);
			//Gionee <GN_BSP_CHG> <lilubao> <20171016> add for update coulomb_count end
			
			
			//Gionee <GN_BSP_CHG> <lilubao> <20171016> add for update coulomb_count begin
			ret_device_file = device_create_file(&(dev->dev), &dev_attr_coulomb_count);
			//Gionee <GN_BSP_CHG> <lilubao> <20171016> add for update coulomb_count begin
			
			
		}
		
	
	
	
	
	
	
		/sys/class/power_supply/battery/coulomb_count
		向上层暴露库仑计的值，单位0.1mAh。mtk平台和高通平台统一

		log_bat_status
		
		fg_cap_learning_check
		
		//Gionee <GN_BSP_CHG> <lilubao> <20171016> add for update coulomb_count begin
		
		DEVICE_ATTR
		
		rc = get_property_from_fg(chip, POWER_SUPPLY_PROP_CHARGE_NOW_RAW, &coulomb_count);
		./sys/devices/soc/qpnp-smbcharger-17/power_supply/battery/coulomb_count
	
	}











}


























/*********************************************************************************************************************************/
17G06A
{





	
		
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


		这个地方还有healthd层的代码，这部分上层代码很重要，上传电量相关参数，还有就是按键的处理主要是powerkey
	}


}











17G10A
{













		pass,适配平均电流apk，上报电量计的值
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

		温升问题：
		
		
		GNSPR#120797,待机界面进相机 慢动作摄影,摄影过程中双指进行缩放,操作1min后一直重复提示电池温度过高,
		查看电池温度55度》退出相机后再操作恢复 验证2台,1台出现,共出现1次
		{
			2017-10-06 15:39	电池温度55度，LCD有点偏黄，插着充电器
			
			情况如何：
			    插着充电器，开摄像，反复调整焦距，一段时间后提示温度过高
		
			但是这个是压力测试，所以需要这样的强度，之前测试温度较高，紧接着做下一个测试	    
			 
			 
			请问是在室温下测试的吗？？手机整体温度在55度附近，我们的软测、硬测、包括整机测试部，目前在室温下 还没有出现这么高的情况；
			如果可以复现，请帮忙找一个debug版本，安排使用附件脚本 抓一份温升数据，谢谢！
			操作方法：
			adb push msm_tsens_logging /data/
			adb shell
			chmod 777 /data/msm_tsens_logging
			/data/msm_tsens_logging 500 10800000 &
			参数解释： 500ms的间隔，10800000--10800s运行总时间（3个小时 能够复现了吧）;

			测完后，取出数据：
			adb pull /data/tsens_logger.csv
			发给我；   
			
			
			109601 GNSPR 【品质压力】开启录屏,待机界面进相机摄像》摄像5min后提示电池温度过高》等待15min后点击确定关闭后再操作恢复 验证2台,2台出现,共出现3次 Q20 Q21 分配 高 G_国 建秋 S2-严重 W_吴 能田 2017-09-07 BJ17G16A（大金钢2）
			109632 GNSPR 【品质压力】开启录屏》待机界面进相机摄像,摄像15min后录屏自动停止(另一台未停止),等待30min后查看录屏文件,录屏中无显示摄像界面的报错提示(实际上一直有提示电池温度过高)》重复操作恢复 验证2台,2台出现,共出现2次 Q20 Q21 申请裁决 高 G_国 建秋 S2-严重 W_吴 能田 2017-09-07 BJ17G16A（大金钢2）

			目前来看 主要是 摄像负载大 热量大，导致温升较高，8940平台没有温升限制 camera 的 策略，可以考虑让camera模块 根据温度 自己控制 调节温升；
			
		
			高通给了一套限制camera及CPU的温升方案，可否安排使用版本测试一下； 到时候 可能会有一些 无响应 黑屏 卡顿情况； 主要 是 连拍和摄像 方面；不知道 camera 模块 是否愿意呢；

			[SS-CPUS]
			#algo_type ss
			sampling 50
			sensor tsens_tz_sensor9
			device cluster1
			set_point 85000
			set_point_clr 55000
			time_constant 0

			CAMERA_CAMCORDER_MONITOR]
			algo_type monitor
			sampling 250
			sensor tsens_tz_sensor3
			thresholds 80000 85000 88000
			thresholds_clr 75000 80000 85000
			actions camera+camcorder camera+camcorder camera+camcorder
			action_info 1+1 2+2 10+10
			
			
			问题属于相机和结构问题，虽然相机没有报错，
			相机是否可以优化摄像电流和功耗，如不能优化硬件和结构进行改善，

			这个问题驱动电源是无法改善的，更该会影响系统性能，请 从相机功耗优化的出发点考虑
			
			
			
			
			
			
			
			{
				msm-thermal
				
				
			
			}
			
			
			
		
		}












	


}






/*****************************************************************************************************/	
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








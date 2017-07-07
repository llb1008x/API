





/*要处理的问题*/
{

	10.17G10A mmi测试读取的数据有问题
	{
		mmi测试读取的节点在（这几个接口有问题？）
		sys/class/power_supply/battery/

		BatteryAverageCurrent  平均电流为0？
		ChargerVoltage InstatVolt      TemperatureR batt_temp capacity     

		device power   present_smb status_smb technology uevent 

		BatterySenseVoltage         ISenseVoltage  TempBattVoltage adjust_power batt_vol  

		capacity_smb health present status      subsystem  type  

		//17G10A    要加一个POWER_SUPPLY_PROP_BatteryPresentCurrent节点
		上面那一组变量属于power_supply的特性(mtk_battery.c)
		static enum power_supply_property battery_props[] = {
			POWER_SUPPLY_PROP_STATUS,
			POWER_SUPPLY_PROP_HEALTH,
			POWER_SUPPLY_PROP_PRESENT,
			POWER_SUPPLY_PROP_TECHNOLOGY,
			POWER_SUPPLY_PROP_CAPACITY,
			/* Add for Battery Service */
			POWER_SUPPLY_PROP_batt_vol,
			POWER_SUPPLY_PROP_batt_temp,
			/* Add for EM */
			POWER_SUPPLY_PROP_TemperatureR,             // 7
			POWER_SUPPLY_PROP_TempBattVoltage,          // 8
			POWER_SUPPLY_PROP_InstatVolt,               // 9
			POWER_SUPPLY_PROP_BatteryAverageCurrent,    // 10
			POWER_SUPPLY_PROP_BatterySenseVoltage,      // 11
			POWER_SUPPLY_PROP_ISenseVoltage,            // 12
			POWER_SUPPLY_PROP_ChargerVoltage,           // 13
			/* Dual battery */
			POWER_SUPPLY_PROP_status_smb,
			POWER_SUPPLY_PROP_capacity_smb,
			POWER_SUPPLY_PROP_present_smb,
			/* ADB CMD Discharging */
			POWER_SUPPLY_PROP_adjust_power,
		};

		//这些变量后面会在sys节点下呈现
		void battery_update_psd(struct battery_data *bat_data)
		{
			bat_data->BAT_batt_vol = battery_get_bat_voltage();
			bat_data->BAT_InstatVolt = bat_data->BAT_batt_vol;
			bat_data->BAT_BatterySenseVoltage = bat_data->BAT_batt_vol;
			bat_data->BAT_batt_temp = battery_get_bat_temperature();
			bat_data->BAT_TempBattVoltage = battery_meter_get_tempV();
			bat_data->BAT_TemperatureR = battery_meter_get_tempR(bat_data->BAT_TempBattVoltage);
			bat_data->BAT_BatteryAverageCurrent = battery_get_ibus();//ibus返回0，不用这个
			bat_data->BAT_ISenseVoltage = battery_meter_get_VSense();
			bat_data->BAT_ChargerVoltage = battery_get_vbus();
		}

		打印log用的这些函数接口
		pr_err("Vbat=%d,I=%d,VChr=%d,T=%d,Soc=%d:%d,CT:%d:%d\n", battery_get_bat_voltage(),
			curr_sign ? bat_current : -1 * bat_current,
			battery_get_vbus(), battery_get_bat_temperature(),
			battery_get_bat_soc(), battery_get_bat_uisoc(),
			mt_get_charger_type(), info->chr_type);

		/*debug*/    
		{
			1.节点数据没有及时上报，导致有的数据一直显示初始化时候的值            
				battery_update_psd获取充电，电池的数据然后赋值给power_supply子系统，battery_main数据结构

				在battery_update函数里面添加 battery_update_psd，每隔10s获取的数据可以上报上去

			2.添加时时的充电电流数据这个选项
			POWER_SUPPLY_PROP_BatteryPresentCurrent
			val->intval = data->BAT_BatteryPresentCurrent

			power_supply_sysfs.c和power_supply.h里面要添加properity特性
			要把这个节点特性呈现到sysfs里面

			3.mmi测试读取的数据有问题
			{
				电池技术显示null

				电池电压BatterySenseVoltage  显示的是电池温度batt_temp

				充电电流有数据但是数据有问题

				电池剩余电量capacity 显示的是电池电压BatterySenseVoltage

				power_supply 节点路径
				充电电流：  /sys/class/power_supply/battery/BatteryPresentCurrent
				充电器电压：/sys/class/power_supply/battery/ChargerVoltage
				电量：      /sys/class/power_supply/battery/capacity
				电池电压：  /sys/class/power_supply/battery/BatterySenseVoltage
				电池技术：  /sys/class/power_supply/battery/technology
				电池温度：  /sys/class/power_supply/battery/batt_temp
			}

			/*修改*/
			{
				1.往power_supply的属性里面提供新的属性
				power_supply_property -> POWER_SUPPLY_PROP_BatteryPresentCurrent, 

				2.修改函数街口，添加新的数据
				//battery_get_ibus -> pmic_get_ibus return 0 这个函数返回值就是0，没意义
				//bat_data->BAT_BatteryAverageCurrent = battery_get_ibus();
				bat_data->BAT_BatteryAverageCurrent = battery_get_bat_avg_current();
				bat_data->BAT_BatteryPresentCurrent = battery_get_bat_current();

				3.数据没有及时上报，所以要在线程里面添加上报
				static void battery_update(struct battery_data *bat_data)
				{
					struct power_supply *bat_psy = bat_data->psy;

					bat_data->BAT_TECHNOLOGY = POWER_SUPPLY_TECHNOLOGY_LION;
					bat_data->BAT_HEALTH = POWER_SUPPLY_HEALTH_GOOD;
					bat_data->BAT_PRESENT = 1;

					//Gionee <gn_by_charging> <lilubao> <20170619> add fixed mmi begin	
					battery_update_psd(bat_data);
					//Gionee <gn_by_charging> <lilubao> <20170619> add fixed mmi end

				#if defined(CONFIG_POWER_EXT)
					bat_data->BAT_CAPACITY = 50;
				#endif
					power_supply_changed(bat_psy);
				}
			}    
		}
		
		
		mmi显示有问题
		{
			1.mmi显示充电电流数据应该少一位
				电池温度应该不用*10，虽然不需要温度的数据
				val->intval = data->BAT_batt_temp ;// remove *10
				充电电流数据应该要除10，不然好几安，太大了	
				val->intval = data->BAT_BatteryPresentCurrent/10;	
				
				
			2.分析log，mmi测试走了哪些流程
			
				
			
			
			3.对比G1605A 还有哪些是缺少的
				a.缺少enter_mmi_test,exit_mmi_test这两个节点
				已在mtk_batteruy.c 文件里面添加了两个节点
				
				b.  is_enter_mmi_test
				这个变量使用来告诉充电其他部分现在正在进行mmi测试，不用降电流等操作
				
				
				c.刚插入充电器的时候，检测到中断，充电器检测线程会在刚开始的15s内，每秒都会更新
				需要在相关的线程里面添加这个判断，并调用gn_update_BatteryPresentCurrent
				
				
				把这段代码加到哪个地方，而且是在线程里面
				//Gionee <gn_by_charging> <lilubao> <20170705> add for platform change begin
				static unsigned int gn_update_counter = 0;
				//Gionee <gn_by_charging> <lilubao> <20170705> add for platform change end
				
				
				//Gionee <gn_by_charging> <lilubao> <20170705> add for platform change begin
				if(upmu_is_chr_det() == true)
				{
					//check_battery_exist();		
					pr_err("xxxxxxxxxxxxxx\n");
					if (!(g_platform_boot_mode == KERNEL_POWER_OFF_CHARGING_BOOT || g_platform_boot_mode == LOW_POWER_OFF_CHARGING_BOOT))
					{
						if(++gn_update_counter<15){
							pr_err("gn_update_counter->%d,by lilubao\n",gn_update_counter);
							gn_update_BatteryPresentCurrent();
						}    
					}else{
						gn_update_counter = 0;
					}
				}	
				//Gionee <gn_by_charging> <lilubao> <20170705> add for platform change end
				
				
				
				
				./drivers/misc/mediatek/include/mt-plat/battery_meter.h:281:	FG_DAEMON_CMD_IS_CHARGER_EXIST,
				./drivers/misc/mediatek/include/mt-plat/mtk_battery.h:237:	FG_DAEMON_CMD_IS_CHARGER_EXIST,
				./drivers/power/mediatek/mtk_battery.c:1844:	case FG_DAEMON_CMD_IS_CHARGER_EXIST:
				./drivers/power/mediatek/mtk_battery.c:1853:			bm_debug("[fg_res] FG_DAEMON_CMD_IS_CHARGER_EXIST = %d\n", is_charger_exist);
				./drivers/power/mediatek/battery_meter_fg_20.c:3982:	case FG_DAEMON_CMD_IS_CHARGER_EXIST:

				
				./drivers/power/mediatek/charger/mtk_charger.c:1154:		pr_err("fg start timer");

				
				./drivers/misc/mediatek/include/mt-plat/mtk_battery.h:768:extern void fg_charger_in_handler(void);
				./drivers/misc/mediatek/include/mt-plat/mtk_battery.h:843:extern void fg_charger_in_handler(void);
				./drivers/power/mediatek/mtk_battery.c:3251:void fg_charger_in_handler(void)
				./drivers/power/mediatek/mtk_battery.c:3257:	bm_debug("[fg_charger_in_handler] notify daemon %d %d\n", chr_type, current_chr_type);
				./drivers/power/mediatek/charger/mtk_chg_type_det.c:183:	fg_charger_in_handler();

				
				./drivers/misc/mediatek/include/mt-plat/charger_type.h:38:extern void mtk_charger_int_handler(void);
				./drivers/power/mediatek/charger/mtk_charger.c:829:void mtk_charger_int_handler(void)
				./drivers/power/mediatek/charger/mtk_charger.c:832:	pr_err("mtk_charger_int_handler\n");
				./drivers/power/mediatek/charger/mtk_chg_type_det.c:182:	mtk_charger_int_handler();
	
		}
		
	}   


	
	有两个kthread function:hrtimer,fgtimer这是两个定时器相关的操作
	可以适当分析时钟相关的框架
	charger_kthread_hrtimer_func
	charger_kthread_fgtimer_func































	battery_id  ，研读相关代码
		读battery_id 电压，匹配电池曲线
		
		
	大概写了一下流程，请过目：
	底层电量跟上层偏差过大导致电量跳变的判断
	
	fg_result：
	0：电量触发跳变的
	2：rtc=1的情况下，把rtc记录的电量赋给电量计；软件电量超过最大电量的情况
	4：rtc记录的电量超过10，把电量计记录的初始化的值赋给电量计
	5：其他情况下也是把电量计记录的初始化的值赋给电量计
	
	if (soc_flow == HW_FG || soc_flow == SW_FG) {
        if ( (fg_plugout_status==0 || (boot_reason == BR_2SEC_REBOOT)) && (charger_exist != true)){
            if (g_rtc_fg_soc == 0) {
                fg_capacity_by_v = fg_capacity_by_v_init;
                fg_result = 0;
            } else {
                if (g_rtc_fg_soc == 1) {
                    fg_capacity_by_v = g_rtc_fg_soc;
                    fg_result = 2;

                } else if ( fg_sw_soc >= max_swocv ) {
                    fg_capacity_by_v = g_rtc_fg_soc;
                    fg_result = 2;
                } else if (g_rtc_fg_soc > 10) {
                    fg_capacity_by_v = fg_capacity_by_v_init;
                    set_rtc = 2;
                    fg_result = 4;
                } else {
                    fg_capacity_by_v = fg_capacity_by_v_init;
                    set_rtc = 1;
                    fg_result = 5;
                }
            }
        } 
        
        else {	//rtc电量-硬件电量  超过  hw_rtc的阈值 同时  软件电量-rtc电量 大于 硬件 -  软件电量
            if (((abs(g_rtc_fg_soc - fg_hw_soc)) > difference_hwocv_rtc)
                && (abs(fg_sw_soc - g_rtc_fg_soc) > abs(fg_hw_soc - fg_sw_soc))) {
                /* compare with hw_ocv & sw_ocv, check if less than or equal to 5% tolerance */
                if (abs(fg_sw_soc - fg_hw_soc) > difference_hwocv_swocv) {
                    fg_capacity_by_v = fg_capacity_by_v_init;
                    fg_result = 0;
                }
            } else {
                if (abs(fg_sw_soc - g_rtc_fg_soc) > (difference_swocv_rtc + batterypseudo1)
                    && ( abs(fg_sw_soc - g_rtc_fg_soc) > abs(fg_sw_soc - fg_vbat_soc) ) ) {
                    fg_capacity_by_v = fg_capacity_by_v_init;
                    fg_result = 0;
                } else {
                    if (g_rtc_fg_soc == 0) {
                        fg_capacity_by_v = fg_capacity_by_v_init;
                        fg_result = 0;
                    } else {
                        if (g_rtc_fg_soc == 1) {
                            fg_capacity_by_v = g_rtc_fg_soc;
                            fg_result = 2;

                        } else if ( fg_sw_soc >= max_swocv ) {
                            fg_capacity_by_v = g_rtc_fg_soc;
                            fg_result = 2;
                        } else if (g_rtc_fg_soc > 10) {
                            fg_capacity_by_v = fg_capacity_by_v_init;
                            set_rtc = 2;
                            fg_result = 4;
                        } else {
                            fg_capacity_by_v = fg_capacity_by_v_init;
                            set_rtc = 1;
                            fg_result = 5;
                        }
                    }
                }
            }
        }
    }

	// modify g_booting_vbat

    if (fg_capacity_by_v == 0 && charger_exist == true) {
        fg_capacity_by_v = 1;
        fg_result = 3;
        FGLOG_NOTICE("[FGADC] fg_capacity_by_v=%d\n", fg_capacity_by_v);
    }

    if (set_rtc == 1) {
        fg_capacity = g_rtc_fg_soc;
    } else if (set_rtc == 2){
        fg_capacity = g_rtc_fg_soc - 1;
    } else {
        fg_capacity = fg_capacity_by_v;
    }
    fg_dod0 = 100 - fg_capacity_by_v;
    fg_capacity_by_c_init = fg_capacity;
    fg_capacity_by_c = fg_capacity;
    fg_dod0_init = fg_dod0;
    fg_dod1 = fg_dod0;
    set_fg_soc(fg_capacity_by_v);
    ui_soc=fg_capacity_by_c_init; 





	int g_fg_battery_id;

	#ifdef MTK_GET_BATTERY_ID_BY_AUXADC
	void fgauge_get_profile_id(void)




















   USB  pid，vid添加到驱动中
   
   
   这几个关键字的代码逻辑
   {
   	
		pmic_throttling_dlpt

   		FGADC_D0		开机初始化的电流，电压，电量
		fg_current_avg  平均电流
		fg_current_act	消耗电量
		
		打开fg log
		adb shell setpro persist.mediatek.fg.log.enable 1
		
		dlpt_notify_thr
		
		
	   //这应该是跟低电保护策略有关
	   DLPT_FEATURE_SUPPORT  
	   {
	   		#if defined(DLPT_FEATURE_SUPPORT)

			if (g_boot_mode != META_BOOT && g_boot_mode != FACTORY_BOOT && g_boot_mode != ATE_FACTORY_BOOT) {
				/*pmic_set_register_value(PMIC_BATON_TDET_EN, 1);*/
	
				pmic_set_register_value(PMIC_RG_BATON_EN, 1);
				if (pmic_get_register_value(PMIC_RGS_BATON_UNDET) == 1) {

					dprintf(CRITICAL, "[BATTERY] No battry plug-in. Power Off.");
					mt6575_power_off();
				}
			}

			pchr_turn_on_charging(KAL_FALSE);
			/* disable SW charger power path */

			switch_charger_power_path_enable(KAL_FALSE);
			mdelay(50);

			get_dlpt_imix_r();
			/* after get imix, re-enable SW charger power path */

			switch_charger_power_path_enable(KAL_TRUE);
			mdelay(50);

			check_bat_protect_status();
			if (is_charging == 1) {
				pchr_turn_on_charging(KAL_TRUE);
				dprintf(CRITICAL, "turn on charging \n\r");
			}
			#endif //#if defined(DLPT_FEATURE_SUPPORT)
	   }   
   }
   


	7.17G10A底电流偏高问题
	{
		1.中断持锁EINT wakelock次数很多导致系统一直没有睡下去
		lk里面sc卡配置有问题，导致频繁上报中断
		
		还有一个不同阶段的gpio配置
		
		
		2.打印中断号，地址，wakeup.c
		在有中断持锁的时候知道是哪些中断
		
		
		3.系统的休眠，唤醒流程
		{
			"mem", "standby", "freeze" and "disk"
		
		}
		
		
		4.如何注掉一些驱动
		
		
		5.系统进入suspend 还有一些低功耗模式
		
		
		5.分析思路

	}




	{
		高通项目的bring up
	   
		rpm配置ld0
		配置设备树
	}





	测试按键驱动,调试按键驱动，熟悉流程和代码
	aw9523b.kl	
	
	
	hps_main是干什么的


    pmi8952,pmi8940,pmi8937,ti的bq系列电量计，以及精度对比
    {
        三个相关的文档对比
        {
            1.bq27428
            {
            	The bq27426 battery fuel gauge uses the patented Impedance TrackTM algorithm for fuel gauging
            	bq27428利用的是阻抗追踪法来进行电量计算，提供相关的电池信息


				相关参数的精确度(温度范围是-40~100)： 
				电压 -0.1% ~ -0.2%
				温度 -13%  ~ 13%    这个偏差很大
				电流 0 ~ 0.6%
            
            	电量计通过读取流过采样电阻的电压来估计充电和放电时候的电池信息
            	
            	当电池接上了fuelgauge的时候。电池的阻抗是根据流进电池的电流，电池的开路电压
            	以及有负载情况下的电池电压
            	
            	P10 I2C通信要注意的地方
            	
            	fuelgauge提供四种模式：initilization,normal,sleep,shutdown
            
            	这里面应该要注意的一个是I2C通信，还有电量计的一些内容
            	
            }
            
            
            2.linux_android_pmic_fuel_gauge_user_guide
            {
            	电量计对于电池充电接口的影响取决于
            	1.允许到充电截止的时间
            	2.当电池电量低于软件设定的阈值的时候，要关闭OTG功能
            
            	电量计读电流通过采样电阻，读电池电压通过连接电池正负极的BATT_P，BATT_N
            	放电读正极，其他时候读负极
            	
            	对于BAT_ID，通常用于检测电池是否存在，以及对于电池的识别，而这个识别的过程
            	是反复的提高基础电流知道发现匹配的为止(5->15->150uA)
            	
            	smart battery? 智能电池
            	
            	battery temperature
            	一是为了保障给电池充电可以安全的使用，二是为了更准确的计算电量
            
            	配置电量计中thermistor相关的参数(80-VT310-123).
            	qcom,thermal-coefficients这是一个位数组，通过这组数据，可以读NTC电阻的一些转换信息
            	{
            		这个有两个地方可以setting
            		一个是在kernel device tree里面
            		一个是在SBL里面，但是这里要晚1.5s因为要启动SRAM，在pm_config_target.c
            		里面设置
            			
            	}


				电池等效串联电阻ESR，这个值最好是实时的，则样才最准确
				等效电阻跟temperature有很大关系，同时也影响到电池的剩余电量
				power_supply 子系统是这个POWER_SUPPLY_PROP_RESISTANCE
				而这个数据的采集是在电量计每隔90s发一个脉冲，然后同步同一时间的电压和电流
				而这个脉冲会引起设备底电流的升高，所以当设备suspend的时候会关闭这个使能
				
				
				系统截止电流
				显示100%，系统截止电流，充电截止电流
				qcom,fg-iterm-ma = <150>; //这个是显示100%的截止电流？

				
				电量计的截止电流
				qcom,fg-chg-iterm-ma = <100>;	//这个是充电截止电流？
				
				
				系统关机电压
				这个影响0%的计算，也就是人为设定的一个0%电压应该是多少
				qcom, fg-cutoff-voltage-mv = <3000>; 
				
				
				开机的时候估计电池电压，如果超过阈值就重新检测导入对当前电量的计算
				qcom, vbat-estimate-diff-mv = <30>;

			
				恒流到恒压充电阈值
				qcom, fg-cc-cv-threshold-mv = <4340>;
				如果开启了动态电压调节充电这个值应该设定的接近vfloat voltage
				
				
				电量低于多少开启回充
				qcom,resume-soc = <95>;


				关于电池老化的检测是通过检测ESR来计算的
				一是通过ESR，
				一是通过电池电量学习算法
				{
					只是产权不开放
					跟温度，通过学习电池充电循环时间的增减，来学习充电循环
					电池开始的电量
				}


				Charge cycle count
				充电循环次数
				

				代码相关的位置
				drivers/power/qpnp-fg.c
				arch/arm/boot/dts/qcom/msm-pmi895x.dtsi
				Documentation/devicetree/bindings/power/qpnp-fg.txt

				
				debug
				{
					打开调试的log
					/*all debug types bit turned on */
					echo 0xff > /sys/module/qpnp_fg/parameters/debug_mask
					echo 8 > /proc/sys/kernel/printk
					dmesg > debug_output_filename

					还有一个dump sram信息的脚本，但是不管用					
				}

            }
            
            
            qpnp-fg.c
            {
            	为什么高通的单个结构体都有那么多成员，还有那么多工作函数？
            	
            	
				fg_probe将电量计device跟driver挂钩,建立bind
				
				log_bat_status这个打印很多电池相关的信息
				都要从get_sram_prop_now这个接口里面去根据下标去读取数据
				fg_mem_data_index
				
				
				常用函数接口：
				充电使能
						rc = set_prop_enable_charging(chip, true);
				
				
				开始的时候初始化很多wakeup source，创建了很多work
				{
					wakeup source:需要了解机制
					
					work：
						update_jeita_setting 这是跟一个温度设置相关的
						
						update_sram_data_work 这个跟fg相关参数存储在sram中相关
							然后有的数据上报都是从sram从读取数组数据，但是这些数据是在哪写的
							利用哪些接口获取的
							
						update_temp_data 上报电池温度相关信息
						
						check_empty_work 这个是上报没电 0%？如果有vbatt-low interrupt这个  中						断触发就会调用这个函数
						
						batt_profile_init 电池相关的配置文件初始化
						
						
						check_sanity_work
						

				}
            
            }
          
        }
        
        
        
        

        给高通提case
    }    










    利用17G16A项目熟悉高通代码
    {
        充电smbcharger
        {   
            dtsi配置文件
            ./arch/arm/boot/dts/qcom/msm-pmi8940.dtsi

            kernel代码
            qpnp-smbcharger.c
            pmic-voter.c    



            (qpnp-smbcharger.c) 这个是高通充电用的主要的源代码，smbchg_init初始化模块 -> spmi_driver_register注册设备驱动 -> smbchg_probe将设备驱动跟设备挂钩，
            建立bind -> 从dtsi文件中获取一些参数 ->  create_votable创建了好几个投票变量 (fcc_votable,usb_icl_votable,dc_icl_votable,usb_suspend_votable,
            dc_suspend_votable,battchg_suspend_votable,hw_aicl_rerun_disable_votable,hw_aicl_rerun_enable_indirect_votable,aicl_deglitch_short_votable,
            hvdcp_enable_votable)



            smbcharger里面涉及到的投票变量都是干什么的
            {
                1.fcc_votable:这个是设置快充充电电流的，根据投票结果决定并行充电的充电电流是多少
                   并行充电好像跟taper_irq_en这个中断有关，但是这个中断是干什么的？

                   static int set_fastchg_current_vote_cb(struct device *dev,
						int fcc_ma,
						int client,
						int last_fcc_ma,
						int last_client)
                    {
                        struct smbchg_chip *chip = dev_get_drvdata(dev);
                        int rc;

                        if (chip->parallel.current_max_ma == 0) {
                            /*设置快速充电的充电电流 fcc_ma*/
                            rc = smbchg_set_fastchg_current_raw(chip, fcc_ma);
                            if (rc < 0) {
                                pr_err("Can't set FCC fcc_ma=%d rc=%d\n", fcc_ma, rc);
                                return rc;
                            }
                        }
                        /*
                        * check if parallel charging can be enabled, and if enabled,
                        * distribute the fcc
                        */
                        /*高通的快充利用的是并行充电方案，主从charger*/
                        smbchg_parallel_usb_check_ok(chip);
                        return 0;
                    }

                
                2.usb_icl_votable：
                    设置usb充电电流的限制，可能要根据系统温升的情况决定
                    如果是快充充电器的会停止并行充电
                    static int set_usb_current_limit_vote_cb(struct device *dev,
						int icl_ma,
						int client,
						int last_icl_ma,
						int last_client)    
                    {
                        struct smbchg_chip *chip = dev_get_drvdata(dev);
                        int rc, aicl_ma, effective_id;

                        effective_id = get_effective_client_id_locked(chip->usb_icl_votable);

                        /* disable parallel charging if HVDCP is voting for 300mA */
                        if (effective_id == HVDCP_ICL_VOTER)
                            smbchg_parallel_usb_disable(chip);

                        if (chip->parallel.current_max_ma == 0) {
                            rc = smbchg_set_usb_current_max(chip, icl_ma);
                            if (rc) {
                                pr_err("Failed to set usb current max: %d\n", rc);
                                return rc;
                            }
                        }

                        /* skip the aicl rerun if hvdcp icl voter is active */
                        if (effective_id == HVDCP_ICL_VOTER)
                            return 0;

                        aicl_ma = smbchg_get_aicl_level_ma(chip);
                        if (icl_ma > aicl_ma)
                            smbchg_rerun_aicl(chip);
                        smbchg_parallel_usb_check_ok(chip);
                        return 0;
                    }


                3.dc_icl_votable
                /*
                * set the dc charge path's maximum allowed current draw
                * that may be limited by the system's thermal level
                */
                设置直流充电器最大的充电电流，遍历数组寻找最匹配的值，写进寄存器
                static int set_dc_current_limit_vote_cb(struct device *dev,
                                        int icl_ma,
                                        int client,
                                        int last_icl_ma,
                                        int last_client)
                {
                    struct smbchg_chip *chip = dev_get_drvdata(dev);

                    return smbchg_set_dc_current_max(chip, icl_ma);
                }                    

                下面这几个大部分都是操作寄存器
                4.usb_suspend_votable       将usb挂起
                5.dc_suspend_votable        dc充电挂起
                6.battchg_suspend_votable   关闭充电使能

                7.hw_aicl_rerun_disable_votable     关闭rerun高电压aicl策略
                8.smbchg_aicl_deglitch_config_cb    配置aicl去抖
                9.hvdcp_enable_votable              允许高电压充电
            }





            smbcharger里面的几个工作函数
            {
                1.smbchg_usb_update_online_work 
                  检测usb的状态然后上报给power_supply子系统
                2.smbchg_parallel_usb_en_work
                  设置并行充电使能
                3.smbchg_vfloat_adjust_work
                  根据充电能路，目标充电电压，不断步进升压
                4.smbchg_hvdcp_det_work
                  高压充电器j 
                      
            }


        }


        fuelgauge
        {


        }




    }    














    fuelgauge 3.0的问题,文档+邮件
    {


    }

}























/*17G10A当前存在的问题*/
{
	充电时序的关机充电和放电状态			
	{
		放电的1%位置还是有问题
		1%开始03：00 vbat=3.62V
		  结束03：52 vbat=3.3V

		1%可能相当于4%左右的电量，而且时间很长  
		1%电压还是很高
	}

	底层电量跟上层电量差别很大，但是这个没有开机和刚开是的充电的log		


	马达刷机第一次震动太弱，之后震动正常？	


	键盘按键有问题，mmi测试不通过，测键不管用


	mmi测试的调用节点好像还有问题
	{
		mmi测试读取的节点信息
		sys/class/power_supply/battery/

		充电可能读取的节点数据有问题
			几个电流，电压，电池电量，电池电压都有问题
			充电电流上不去
			{
				这个好像是主板温度过高，虽然显示的是电池温度50+，但是电池明显不热
				[    9.896490] <6>.(4)[258:charger_thread][name:mtk_charger&]Vbat=3775,I=-4446,VChr=9,T=49,Soc=0:0,CT:0:0

				[   12.371394] <6>.(6)[258:charger_thread][name:mtk_battery_hal&][fgauge_read_current] final current=5051 (ratio=950)
				[   12.372785] <6>.(6)[258:charger_thread][name:mtk_charger&]Vbat=3771,I=-5051,VChr=9,T=50,Soc=0:0,CT:0:0
				[   12.375494] <4>.(4)[258:charger_thread][name:pmic_auxadc&][mt6355_get_auxadc_value] ch = 2, reg_val = 0x1d3, adc_result = 410
				[   12.377141] <4>.(4)[258:charger_thread][name:mtk_battery_hal&][fgauge_read_current] final current=4735 (ratio=950)
				[   12.378491] [name:mtk_charger&][BATTERY] Battery over Temperature or NTC fail 50 50!!

				这个是电池的pin脚有问题
			}

		键盘mmi测试不能通过
			键盘的工作，以及涉及哪些目录下的文件
	}

        
	计步器，sd卡相关内容？
	{
		set_counter.c
		sd.c
	}


    从满电放电到关机的情况是否有异常


    计步器，sd卡相关内容？
    {
        set_counter.c
        sd.c
    }


	快充升压问题


	去掉OTG中断


	电池曲线的导入
}





/*G1605A  售后问题，功耗问题 healthd线程*/
{
    售后问题：   电量计不准的机器


    这个还不知道怎么看，只是觉得电量跟电池电压偏差有点大，而且底层电量跟上层显示的电量差别也很大


    apk发包次数，谁发的(主从)，就是链接到哪了？？
        那个脚本怎么用
}


































17G10A
{
	
	
	充电相关的问题问题
	{
		老化测试电流不够
			pass,GNSPR #147155	【老化测试】老化测试电流测试电流节点达到1000ma，难以通过电流测试进行老化测试
			pass,GNSPR #141724 	【产线器件老化】老化测试过程中出现充电测试未通过
			pass,GNSPR #135719	【老化测试】测试老化测试时 出现非异常重启在组合测试2中	
			pass,GNSPR #146336	【产线器件老化】产线老化12个小时 出现充电测试未通过在电流测试中 出现5台
			
			
		
			pass,GNSPR #139587	【低电量验证电流专项】进MMI硬件测试-充电测试（电量为8%），插入标配1A充电器显示充电电压为4957mv，
			充电电流一直浮动在222mA~358mA（标准值应为1000mA）》插拔充电器未恢复，充电器测试另一台测机后再次测试恢复
			pass,GNSPR #141086	电源管理：玩王者荣耀的时候插标配充电器充电玩游戏1H，电量只增加0%，查看充电记录，
		    玩游戏的时候充电电流为不足10MA
		    
		    
		pass,GNSPR #141086,电源管理：玩王者荣耀的时候插标配充电器充电玩游戏1H，电量只增加0%，查看充电记录，
		玩游戏的时候充电电流为不足10MA
		{
		
			//Gionee <GN_BY_CHG> <lilubao> <20171227> add for fixed #141086 begin
			
				1.目前分析是thermal 的bcct，根据温升降电流，导致最后充电线上的电流很小，大部分都供给系统
				首先要确定温度，充电电流的变化，thermal的策略是否合理
				
				
				2.现在thermal 的策略是
				/proc/driver/thermal/clabcct
				43000 1000 200000 5 2000 300 0 3000 0 1 5000 2000
				
				
				3.T_AP=46,T_btsmdpa=49
				电池的温度是根据auxadc采集到的电压然后查表找到对应的温度，
				这个标在一般的thermal代码里面有如，mtk_ts_btsmdpa.c BTSMDPA_TEMPERATURE BTSMDPA_Temperature_Table6[] 
				至于找哪个表是从thermal config文件中配置的，倒数第二个 数组6
				PUP_R 390000 PUP_VOLT 1800 OVER_CRITICAL_L 4251000 NTC_TABLE 6 0
				
				
				4.这段时间内 AP，跟btsmdpa温度很高
				12-07 16:48:18.501101 <7>[22198.050609]  (0)[12102:kworker/0:1][Thermal/TZ/BTSMDPA]T_btsmdpa=48000
				12-07 16:48:18.501329 <7>[22198.050837]  (0)[12102:kworker/0:1][Thermal/TZ/BTS]T_AP=45000
				
		6 100000 0 mtktsAP-sysrst 90000 0 mtk-cl-shutdown00 53000 0 mtk-cl-cam00 49000 0 abcct_lcmoff 44000 0 mtk-cl-adp-fps 41000 0 abcct 0 0 no-cooler 0 0 no-cooler 0 0 no-cooler 0 0 no-cooler 1000


				5. 16:47~16:49这段时间打游戏卡不卡，视频画质怎么样，有没有掉帧
				


				kernel_log:162074:<3>[22139.738872]  (0)[239:charger_thread]Vbat=4068,I=20496,VChr=5090,T=42,Soc=17:14,CT:4:4
				12-07 16:47:08.666912 <7>[22140.989020]  (0)[1392:system_server][Thermal/TZ/BTS]T_AP=44000
				12-07 16:47:08.667007 <7>[22140.989115]  (0)[1392:system_server][Thermal/TZ/BTSMDPA]T_btsmdpa=47000
				
				before thermal abcct
				12-07 16:47:35.154654 <3>[22160.186551]  (1)[239:charger_thread]force:0 thermal:-1 -1 setting:2300000 2100000 type:4 usb_unlimited:0 usbif:0 usbsm:0 aicl:-1
				
				12-07 16:48:08.690842 <3>[22188.240350]  (0)[239:charger_thread]force:0 thermal:-1 300000 setting:2300000 2100000 type:4 usb_unlimited:0 usbif:0 usbsm:0 aicl:-1
				
				12-07 16:48:18.693888 <3>[22198.243396]  (0)[239:charger_thread]force:0 thermal:-1 300000 setting:2300000 0 type:4 usb_unlimited:0 usbif:0 usbsm:0 aicl:-1
				12-07 16:48:20.253308 <3>[22199.802816]  (2)[239:charger_thread]force:0 thermal:-1 300000 setting:2300000 0 type:4 usb_unlimited:0 usbif:0 usbsm:0 aicl:-1
				12-07 16:48:28.258041 <3>[22207.807548]  (3)[239:charger_thread]force:0 thermal:-1 300000 setting:2300000 0 type:4 usb_unlimited:0 usbif:0 usbsm:0 aicl:-1
				12-07 16:48:28.485482 <3>[22208.034989]  (1)[239:charger_thread]force:0 thermal:-1 300000 setting:2300000 0 type:4 usb_unlimited:0 usbif:0 usbsm:0 aicl:-1
				
				
				停止充电
				12-07 16:48:18.693888 <3>[22198.243396]  (0)[239:charger_thread]force:0 thermal:-1 300000 setting:2300000 0 type:4 usb_unlimited:0 usbif:0 usbsm:0 aicl:-1
				
				12-07 16:48:18.695163 <7>[22198.244671]  (0)[239:charger_thread]rt5081_pmu 5-0034: rt5081_pmu_reg_update_bits: reg 19 data 30
				12-07 16:48:18.695176 <7>[22198.244684]  (0)[239:charger_thread]rt5081_pmu 5-0034: rt5081_pmu_reg_update_bits: mask f0
				12-07 16:48:18.695343 <7>[22198.244851]  (0)[239:charger_thread]rt5081_pmu 5-0034: rt5081_pmu_reg_read: reg 19
				12-07 16:48:18.695437 <3>[22198.244945]  (0)[239:charger_thread][charger]charging current is set 0mA, turn off charging !
				
				
				
				6.有两个电流区别和注意的地方，
				charging_current_limit
				input_current_limit
				
				rt5081会获取最低设备最低的充电电流 500mA，如果低于500mA就会设置为0
				charging current	500mA
				static int rt5081_get_min_ichg(struct charger_device *chg_dev, u32 *uA)
				{
					*uA = 500000;
					return 0;
				}
				
				input current  100mA
				static int rt5081_get_min_aicr(struct charger_device *chg_dev, u32 *uA)
				{
					*uA = 100000;
					return 0;
				}
				
				
				7.IEOC 截止电流为什么会变成250mA，设置的是150mA，为什么会变？
				这个是内部一个算法调整，主要是针对输入电流变化大的情况下，截止电流不准，
				可以调整截止电流 Workaround
				/* Workaround to make IEOC accurate */
				if (uA < 900000 && !chg_data->ieoc_wkard) { /* 900mA */
					ret = __rt5081_set_ieoc(chg_data, chg_data->ieoc + 100000);
					chg_data->ieoc_wkard = true;
				} else if (uA >= 900000 && chg_data->ieoc_wkard) {
					chg_data->ieoc_wkard = false;
					ret = __rt5081_set_ieoc(chg_data, chg_data->ieoc - 100000);
				}
				
				
				
			
				0x27=0x60   0110 0000 
				 SDP NSTD (by input pin of (sVBUSPG_syn & sCHGDETB & DCDT)=1)
				 Charger port is not detected
				 
				 
				static void swchg_turn_on_charging(struct charger_manager *info)
				{
					struct switch_charging_alg_data *swchgalg = info->algorithm_data;
					bool charging_enable = true;

					if (swchgalg->state == CHR_ERROR) {
						charging_enable = false;
						pr_err("[charger]Charger Error, turn OFF charging !\n");
					} else if ((get_boot_mode() == META_BOOT) || ((get_boot_mode() == ADVMETA_BOOT))) {
						charging_enable = false;
						pr_err("[charger]In meta or advanced meta mode, disable charging.\n");
					} else {
						mtk_pe20_start_algorithm(info);
						mtk_pe_start_algorithm(info);

						swchg_select_charging_current_limit(info);
						if (info->chg1_data.input_current_limit == 0 || info->chg1_data.charging_current_limit == 0) {
							charging_enable = false;
							pr_err("[charger]charging current is set 0mA, turn off charging !\r\n");
						} else {
							swchg_select_cv(info);
						}
					}

					charger_dev_enable(info->chg1_dev, charging_enable);
				}
				 


			}	
		}		   
		   
		   
		   
		   
		   
		   
		   
		pass,GNSPR #139587	【低电量验证电流专项】进MMI硬件测试-充电测试（电量为8%），插入标配1A充电器显示充电电压为4957mv，
		充电电流一直浮动在222mA~358mA（标准值应为1000mA）》插拔充电器未恢复，充电器测试另一台测机后再次测试恢复
 		{
 			这个是当时充电器识别成非标充导致电流设置只有500mA，亮屏进电池只有200~300mA左右
			识别成为非标充是概率性的，有时候没有插拔好充电器就会出现这种情况

			时间点：2017-5-2 05:58
			05-02 05:46:27.434155 0 0 E [ 172.031524] (0)[185:irq/773-rt5081_]: dump_charger_name: charger type: 0, Charger Unknown
			05-02 05:57:53.785801 0 0 E [ 333.714307] (0)[185:irq/773-rt5081_]: dump_charger_name: charger type: 3, Non-standard Charger
			05-02 06:00:56.656058 0 0 E [ 516.584564] (0)[185:irq/773-rt5081_]: dump_charger_name: charger type: 0, Charger Unknown
			05-02 06:01:02.302431 0 0 E [ 522.230937] (0)[185:irq/773-rt5081_]: dump_charger_name: charger type: 3, Non-standard Charger
			05-02 06:02:20.661125 0 0 E [ 600.589631] (0)[185:irq/773-rt5081_]: dump_charger_name: charger type: 0, Charger Unknown
			05-02 06:03:10.771173 0 0 E [ 650.699679] (0)[185:irq/773-rt5081_]: dump_charger_name: charger type: 4, Standard Charger
			05-02 06:03:42.047792 0 0 E [ 681.976298] (0)[185:irq/773-rt5081_]: dump_charger_name: charger type: 0, Charger Unknown

			05-02 05:58:05.086952 238 238 E [ 345.015458] (0)[238:charger_thread]: force:0 thermal:-1 -1 setting:500000 500000 type:3 usb_unlimited:0 usbif:0 usbsm:0 aicl:-1
 		
 		}
		    
		   
		   
		   
		   
		   
		pass,GNSPR #146336	【产线器件老化】产线老化12个小时 出现充电测试未通过在电流测试中 出现5台
		{
			产线的是电源5V/2A,而且充电线比较长，压差比较大，所以有时候电流可能很小
			现在标准是1000mA，所以用电源有很多机器没有通过测试，统一标准是200mA，应该可以通过
		} 
		    
		    
		    
		    

		pass，GNSPR #141724 	【产线器件老化】老化测试过程中出现充电测试未通过
		{
			<7>[ 2661.566148]  (0)[241:wdtk-0][thread:241][RT:2661566148084] 2017-05-01 04:49:39.879272 UTC;android time 2017-05-01 12:49:39.879272
			
			
			
			
			main.log
			05-01 12:49:47.199482 4699 4699 I oldtest.TAG: ChargeBroadcastReceiver=>getChargingAllValues:mFirstChargeLevel=52;mChargePlug=1;mChargeVoltage=3818;mChargeCurrent=0;mChargeLevel=46;mTemperatureAp=25
			05-01 12:49:47.199968 4699 4699 I oldtest.TAG: ChargeBroadcastReceiver=>getChargingAllValues:充电状态：
			05-01 12:49:47.199968 4699 4699 I oldtest.TAG: 老化测试开始电量:52
			05-01 12:49:47.199968 4699 4699 I oldtest.TAG: 充电方式：AC
			05-01 12:49:47.199968 4699 4699 I oldtest.TAG: 充电电压：3818mV
			05-01 12:49:47.199968 4699 4699 I oldtest.TAG: 充电电流：0mA
			05-01 12:49:47.199968 4699 4699 I oldtest.TAG: 电池电量：46
			05-01 12:49:47.199968 4699 4699 I oldtest.TAG: 电池温度：25
			05-01 12:49:47.200629 4699 4699 I oldtest.TAG: DbDatabaseManager=>oldDatabaseManager insertTP
			05-01 12:49:47.200656 4699 4699 I oldtest.TAG: DbDatabaseHelper=>oldDatabaseHelper inserttp
			05-01 12:49:47.200674 4699 4699 I oldtest.TAG: DbDatabaseHelper=>scene=电流测试
			05-01 12:49:47.209559 4699 4699 I oldtest.TAG: DbDatabaseHelper=>row=1
			
			
			同BUG#147155
		
		}    
		    
		    
		    
		    
		    
		    
		pass,GNSPR #135719	【老化测试】测试老化测试时 出现非异常重启在组合测试2中	
		{
			11-20 08:56:09.759403 3444 3581 I oldtest.TAG: DbDatabaseHelper=>query oldtest db time_=2017-11-20_08-56-09 issue_非异常重启
			11-20 08:56:09.759403 3444 3581 I oldtest.TAG: 低电关机,充电器断开时间:2017-11-17 19:51:42
			11-20 08:56:09.759403 3444 3581 I oldtest.TAG: scene=组合测试2 testtime=11-17 19:52:08
			11-20 08:56:09.759468 3444 3581 I oldtest.TAG: ReportActvity=>queryIssue:issue.getIssue()=非异常重启
			11-20 08:56:09.759468 3444 3581 I oldtest.TAG: 低电关机,充电器断开时间:2017-11-17 19:51:42
			11-20 08:56:09.802517 3444 3444 I oldtest.TAG: ReportActvity=>onPostExecute:duration=3664;isdone=true
			11-20 08:56:09.831705 3444 3444 I oldtest.TAG: ReportActvity=>getView:2017-11-20_08-56-09: 非异常重启
			11-20 08:56:09.831705 3444 3444 I oldtest.TAG: 低电关机,充电器断开时间:2017-11-17 19:51:42
			
			
			测试在测试的时候插的是usb，不是标准充电器，所以耗的电多充的电少，最后低电关机了

			150064:<3>[177297.658288] (7)[236:charger_thread]Vbat=3153,I=194,VChr=4938,T=32,Soc=-24:1,CT:1:1
			152317:<3>[177307.658305] (6)[236:charger_thread]Vbat=3150,I=-434,VChr=4920,T=32,Soc=-24:1,CT:1:1
			154591:<3>[177317.657975] (6)[236:charger_thread]Vbat=3153,I=30,VChr=4929,T=32,Soc=-24:1,CT:1:1
		
		
		}    
		    
		    
		    
		    
		    
		pass,GNSPR #147155	【老化测试】老化测试电流测试电流节点达到1000ma，难以通过电流测试进行老化测试    
		{
			01-03 18:43:43.633744 <6>[17525.419381]  (0)[242:charger_thread]rt5081_pmu_charger rt5081_pmu_charger: rt5081_dump_register: VSYS = 4420mV, VBAT = 4420mV, IBAT = 1150mA, IBUS = 1500mA, VBUS = 4625mV
			01-03 18:43:43.935567 <6>[17525.721204]  (1)[242:charger_thread]rt5081_pmu_charger rt5081_pmu_charger: rt5081_dump_register: VSYS = 4450mV, VBAT = 4435mV, IBAT = 1200mA, IBUS = 1450mA, VBUS = 4625mV



			01-03 18:43:43.365765 <3>[17525.151402]  (3)[242:charger_thread]Vbat=4251,I=-865,VChr=4976,T=30,Soc=86:87,CT:4:0
			01-03 18:43:43.640270 <3>[17525.425907]  (0)[242:charger_thread]Vbat=4431,I=11970,VChr=4740,T=29,Soc=86:87,CT:4:4
			01-03 18:43:51.144345 <3>[17532.929982]  (0)[242:charger_thread]Vbat=4238,I=1014,VChr=794,T=30,Soc=86:87,CT:0:4




			01-03 18:43:45.937121 16094 16094 V PhoneWindow: DecorView setVisiblity: visibility = 0, Parent = ViewRoot{5ed0521 com.gionee.factorytests/com.gionee.os.autooldtest.ChargeActivity,ident = 4}, this = DecorView@c03272b[ChargeActivity]



			01-03 18:43:45.937961 16094 16094 I oldtest.TAG: ChargeActivity=>mBroadcastReceiver action=android.intent.action.BATTERY_CHANGED
			01-03 18:43:45.938362 16094 16094 I oldtest.TAG: ChargeActivity=>chargeCurrent=
			01-03 18:43:45.938981 16094 16094 I oldtest.TAG: ChargeActivity=>chargeCurrent 2=49
			01-03 18:43:45.939117 16094 16094 I oldtest.TAG: ChargeActivity=>chargeVoltage = 4248;current=49;mChargeTargetValue=1000;plugged=1

			
			实际电流有1200mA，但实际只有49mA，所以要确定老化测试读的数据对不对？
			老化测试修改了，监听电流变化的sys node，原来需要10s才变化一次，所以数据可能没有即时更新
			监听 	 BatteryAverageCurrent
			读取电流 BatteryPresentCurrent
			
			集成到T210版本上
		
		}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	GNSPR#139202，去除冗余log
	{
		//Gionee <GN_BY_CHG> <lilubao> <20171130> remove redundant log begin
		1.修改log等级 5 可以减少一部分
		FG_daemon_log_level 
		Enable_BATDRV_LOG	
		
		2.mt6757.dtsi
		pwrap@1000d000 {
			compatible = "mediatek,pwrap";
			reg = <0 0x1000d000 0 0x1000>;
			//Gionee <GN_BY_CHG> <lilubao> <20171130> remove redundant log begin
			interrupts = <GIC_SPI 152 IRQ_TYPE_LEVEL_HIGH>;
			//Gionee <GN_BY_CHG> <lilubao> <20171130> remove redundant log end
			mt6351_pmic: mt6351 {
				compatible = "mediatek,mt6351-pmic";
				interrupt-controller;
			};
			mt6355_pmic: mt6355 {
				compatible = "mediatek,mt6355-pmic";
				interrupt-controller;
			};
		};
		
		
		
		3.还有这个问题，irq不匹配的问题
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

		
		
		

		
			定位开机的dod_init
			12-09 17:31:09.029360     0     0 E [   40.798592][dod_init_result]: <5> 39834 39834 6210 1 0 1 1

			5 代表第五路，用hw_ocv重新点位D0,后面三个数字c_ocv=hw_ocv=39834，v_ocv=sw_ocv=hw_ocv=39834，UI=ocv_to_soc(hw_ocv)

			标红的0 代表adc_reset没有发生，1代表vc_mode为c_mode，知道这两个就可以了

			 

			12-09 16:58:54.382134     0     0 E [  341.016964][dod_init_result]: <1> 38052 38169 3258 0 0 1 0

			1代表定位到第1路开机定位采用old_data的值，UI保持与关机前的一致。标红0 1 和上一条一致




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











18Y07A
{


}
























17G08A
{

	
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
			The CC line communication between Sink and Source will look like below :

				1. The Sink will first request Source capabilities.

				2. The Source will provide its source capabilities.

				3. The Sink will then request a suitable power profile from the Source capability list.

				4. The Source will accept the request and start to make changes to the BUS voltage. The Sink will minimize the bus loading during BUS voltage change. The Source will increase the VBUS voltage with a defined slew rate.

				5. After the BUS voltage has reached its final value, the source will wait until the BUS has stabilized and then will send a Power Supply Ready signal. The Sink will now increase the BUS loading again. 
				
				
				Bi-phase Mark Code (BMC) 这个是什么编码方式
				The BMC data can be decoded by USB PD decoders like Ellisys EX350 Analyzer
		
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






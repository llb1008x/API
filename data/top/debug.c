



















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
	
	


	

	
	
	
	
	
	
17G08A
{







	电量计的问题
	GNSPR #138519 ,连接充电器重启手机后，锁屏界面显示充电完毕，实际上手机电量是93%，截屏后恢复
	{
			
		相关的case：03238858，03249227
		{
			kba-170418012921  这是总的一些kba
			
			//Gionee <GN_BY_CHG> <lilubao> <20171206> add for debug fuel gauge begin
			dev_err(chip->dev, "in [%s] by lilubao \n",__FUNCTION__);	
			
			qcom,hold-soc-while-full;
				Definition: A boolean property that when defined holds SOC at 100% when
		    	the battery is full.

				
			qcom,fg-recharge-soc-thr = <99>; 
				Definition: Percentage of monotonic SOC upon which the charging will
				will be resumed once the charging is complete. If this
				property is not specified, then the default value will be
				95.
				
			dump.sh dump  fuel gauge寄存器   ./dump.sh > /data/kmsg.txt &
			{
				
				let count=0

				local utime
				local ktime
				local pause_time=10

				if [ -n "$1" ]; then
					pause_time=$1
				fi


				dump_peripheral () {
					local base=$1
					local size=$2
					local dump_path=$3
					echo $base > $dump_path/address
					echo $size > $dump_path/count
					cat $dump_path/data
				}

				echo "Starting dumps!"
				echo "Dump path = $dump_path, pause time = $pause_time"

				while true
				do
					utime=($(cat /proc/uptime))
					ktime=${utime[0]}
					echo "FG SRAM Dump Started at ${ktime}"
					dump_peripheral 0 500 "/sys/kernel/debug/fg/sram"
					uptime=($(cat /proc/uptime))
					ktime=${utime[0]}
					echo "FG SRAM Dump done at ${ktime}"
					let count=$count+1
					sleep $pause_time
				done
			
			}	
				
			解析dump出来的寄存器 ,但是解析工具需要到windows环境暂时不行

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



	
	17G10A，ppt测试
	{
		smart PA:TI TFA 2557  功耗
	
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


			//Gionee <GN_BY_CHG> <lilubao> <20171127> add for debug soc begin
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






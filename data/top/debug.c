


{


	ANDROID TREBLE OVERVIEW                                          80-PE644-1
	ANDROID TREBLE VNDK OVERVIEW                                     80-PE644-2
	ANDROID TREBLE HIDL OVERVIEW                                     80-PE644-3
	ANDROID TREBLE KERNEL OVERVIEW                                   80-PE644-4
	ANDROID TREBLE VTS OVERVIEW                                      80-PE644-5
	VERIFIED BOOT 2.0 INTRODUCTION                                   80-PE644-6
	SELINUX OVERVIEW AND UPDATE FOR ANDROID O        				 80-PE644-7
	
	Bus 003 Device 021: ID 058f:6387 Alcor Micro Corp. Flash Drive

	
	GNSPR#122265
	To download any document directly from this solution, first login to the CreatePoint and then click on the hyperlink listed against the relevant document below.

	80-P2485-18 : MSM8937 System Drivers PMIC Overview
	80-P2485-2 : MSM8937_Linux_Android_PMIC_SW_Drivers_Overview
	80-NV610-48 : PMIC GPIO and MPP Software Configuration

	For a complete list of PMIC Software documents and Knowledge base solutions for all technology areas please refer to the following master documents:

	80-NR097-1 : PMIC Software Master Document
	80-NR097-2 : PMIC Software KB Solution Master Document


	To download any document directly from this solution, first login to the CreatePoint and then click on the hyperlink listed against the relevant document below.

	80-P2485-18 : MSM8937 System Drivers PMIC Overview
	80-P2485-2 : MSM8937_Linux_Android_PMIC_SW_Drivers_Overview
	80-NV610-43 : System Drivers PMIC Dead Battery Charging Overview
	80-NV610-44 : MSM8937.LA Charger SW User Guide

	For a complete list of PMIC Software documents and Knowledge base solutions for all technology areas please refer to the following master documents:

	80-NR097-1 : PMIC Software Master Document
	80-NR097-2 : PMIC Software KB Solution Master Document


	80-NL708-1		dump pimc register	
	
	
	decidegc
	
	NTC温度电压转换函数，看看过程什么样的
	force_get_tbat_internal 温度转换函数


	近期W919项目为了满足生产需求，T3-3更新了一颗speaker 2557的物料，此物料不可兼容，因此区分版本维护：
	1.T3-3前面批次机器，请刷版本尾号为AB的版本
	2.T3-3以及之后批次机器，请刷版本尾号AA的版本

	目前大家手里机器都是T3-3前的，所以测试和开发请根据手中机器状态刷AB版本。

	如果刷错版本会遇到扬声器各类场景无声的问题！！！
	版本情况：
	BJ17G10A-T0148-171116AB --可刷T3-3之前机器
	BJ17G10A-T0147-171116AB --代码问题，扬声器无声
	BJ17G10A-T0146-171114AA --代码问题，扬声器无声
	BJ17G10A-T0145-171114AA --可刷T3-3及之后机器
	
	
	
	
	
	
	
	SDM660  pmic
	{
		
		
		相关的宏
		{
			CONFIG_QPNP_FG_GEN3		电量计	pm660_fg
			{
				qpnp-fg-gen3.c  fg-memif.c  fg-util.c
			}
			
			CONFIG_QPNP_SMB2		charger	pm660_charger
			{
				step-chg-jeita.c   battery.c  qpnp-smb2.c  smb-lib.c  pmic-voter.c  storm-watch.c 
			}
			
			CONFIG_WLS_CHARGER_ECH  无线充电
			{
				ech-wls-charger.c 
			}
		}
		
		
		dtsi
		{
			msm-pm660.dtsi	
			fg-gen3-batterydata-gionee-17g08a-atl-4v4-5060mAh.dtsi
			sdm660-common.dtsi
			msm-smb138x.dtsi
			sdm660-pinctrl.dtsi
			sdm660-qrd.dtsi
			
		}
		
		extcon 外设插拔检测
		
		文档号
		{
			evaluate 
				80-P7905-5A
				80-P7905-1
			design
				80-P8754-59
				80-P8754-6
				80-NN255-1
				80-P8754-30
				80-NR097-1
				80-NN139-1
			bringup
				80-P7905-2X
				80-P7905-4
				80-NM620-1
			customize
				80-P2484-42
				80-P2484-77
				80-P2484-74
				80-P8754-43
				80-NM328-53
			verify
				80-P7747-7
				80-P8754-34
			
			debug tool 这个很重要
				The following debug tools for PMIC are available:
				• How to collect a PMIC register dump?
				(Solution #00029307)
				• How to collect to read/write PMIC registers using
				ADB?
				(Solution #00029310)
				• How to enable PMIC driver logs for debugging?
				(Solution #00029311)	
				For more debug information, refer to the debug
				sections in the individual PMIC module User Guide
				documents listed in PMIC Software Master Document
				(80-NR097-1).			
						
		}

	}
	
	
	
	
	

}	
	
	
	
	
17G08A
{
	电量计的问题
	相关的case：03238858，
	{
		kba-170418012921  这是总的一些kba
		
		
	
	}
	


	GNSPR #138519 ,连接充电器重启手机后，锁屏界面显示充电完毕，实际上手机电量是93%，截屏后恢复
	{
		
	
	}


	GNSPR #138056,电量94%以上100%以下连接充电器手动重启手机，重启后充电图标不显示充电，呼吸灯显示为绿色，
	插拔充电器恢复
	{
	
	
	
	}


	GNSPR #138239,手机电量98%，关机充电，手机开机，锁屏界面与状态栏电量显示不一致
	{
	
	
	
	}
	


}	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
17G10A
{

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









	关机充电相关的几个BUG
	{
			GNSPR#134414，【内部体验】低电量关机状态下》连接充电器后》手动点亮屏幕一直在闪花屏 （开机起来显示28%电量）
			{
				现象：低电量关机充电条件，然后关机之后电量出现跳变
		
		
				分析：
				{
					1.造成插充电器低电的原因，虽然是识别了DCP，但是aicl电流一直是100mA，所以造成了充的少耗的多，
					最后虽然上层UI显示是1%，但是底层是负值，因为插着充电器才没有关机
			
			
					2.电量跳变的原因，之前电量应该是多少？
			
			
					3.花屏是跟系统起来电压有关还是本身的原因？
			
			
					4.是否跟关机充电logo有关?
			
			
					5.开机初始化的电池电压，电流等相关参数应该怎么搜？
		
				}

				//Gionee <GN_BY_CHG> <lilubao> <20171122> add for debug kpoc charging begin
				
				
				之前插入充电器的情况是什么样？充电器的识别情况？
				read_boot_battery_plug_out_status
				
				[   80.454636]  (5)[72:dlpt_notify_thr][DLPT_final] 2615,0,84,84,1,5500
				DLPT_final 低电保护 
				
				[   80.866102]  (4)[186:battery_thread]gn_boot_reason->2,gn_boot_mode->2,gn_call_state->0,gn_screenon_time->80
				之前是通过有操作过recovery 进入recovery是升级还是回复出厂设置？recovery+插了充电器的操作
				
				之前电池电压是4.07V 
				[   80.873337]  (5)[262:fuelgauged_stat][fg_res] FG_DAEMON_CMD_GET_VBAT = 40750
	
			}

	}












	去除冗余和不合理的log,代码逻辑
	{
			rt5081的 mivr中断enable跟disable不匹配
			[    2.457134]  (4)[238:charger_thread]rt5081_pmu_charger rt5081_pmu_charger: rt5081_enable_power_path: en = 1
			[    2.457141]  (4)[238:charger_thread]rt5081_pmu_charger rt5081_pmu_charger: __rt5081_set_mivr: mivr = 4400000 (0x05)


			[    2.457274]  (4)[238:charger_thread]rt5081_pmu_charger rt5081_pmu_charger: rt5081_enable_irq: (chg_mivr) en = 1
			[    2.457289] -(4)[238:charger_thread]------------[ cut here ]------------
			[    2.457293] -(4)[238:charger_thread]WARNING: CPU: 4 PID: 238 at /data/MAIN_GIT_REPO_CODE/BJ17G10A_MAIN_REPO/L31_6757_66_N_17G10A_NO.MP5_V1.53_170512_ALPS/L31_6757_66_N_17G10A_NO.MP5_V1.53_170512_ALPS/android_mtk_mp/kernel-4.4/kernel/irq/manage.c:513 enable_irq+0x88/0xcc()
			[    2.457308] -(4)[238:charger_thread]Unbalanced enable for IRQ 166
			[    2.457315] -(4)[238:charger_thread]CPU: 4 PID: 238 Comm: charger_thread Tainted: G        W       4.4.15 #1
			[    2.457321] -(4)[238:charger_thread]Hardware name: MT6757CD (DT)
			[    2.457326] -(4)[238:charger_thread]Call trace:
			[    2.457330] -(4)[238:charger_thread][<ffffffc00008a328>] dump_backtrace+0x0/0x14c
			[    2.457339] -(4)[238:charger_thread][<ffffffc00008a488>] show_stack+0x14/0x1c
			[    2.457344] -(4)[238:charger_thread][<ffffffc000337830>] dump_stack+0x8c/0xb0
			[    2.457353] -(4)[238:charger_thread][<ffffffc00009e5f8>] warn_slowpath_fmt+0xc0/0xf4
			[    2.457360] -(4)[238:charger_thread][<ffffffc0000fd32c>] enable_irq+0x88/0xcc
			[    2.457365] -(4)[238:charger_thread][<ffffffc0004d8090>] rt5081_enable_power_path+0x148/0x190
			[    2.457375] -(4)[238:charger_thread][<ffffffc0009388a0>] charger_dev_enable_powerpath+0x24/0x34
			[    2.457384] -(4)[238:charger_thread][<ffffffc00093bcf0>] charger_routine_thread+0x378/0x6a8
			[    2.457392] -(4)[238:charger_thread][<ffffffc0000be29c>] kthread+0xdc/0xf0
			[    2.457400] -(4)[238:charger_thread][<ffffffc000085cd0>] ret_from_fork+0x10/0x40
			[    2.457407] -(4)[238:charger_thread]---[ end trace 644ae300f92f6883 ]---
			
			
			log 单词错误
			<3>[ 7482.590048]  (2)[239:charger_thread]mtk_is_charger_on plug in, tyupe:4   
	
	}
	











	GNSPR#109803,关机状态，连接充电器，显示电量灭屏后，短按两次电源键1s，出现开机画面，再次操作恢复
	{
		启动之后的boot_reason 是4,wdt_by_pass_pwk,然后这个现象是在系统跑到kernel之后的
		按键的检测问题应该是在kernel出现问题的		
		{
			首先这个boot_reason 有哪些原因？
			SW,HW,kernel panic 按键检测到短按所以会重启，这个应该是系统硬件初始化的时候读取寄存器传递的值g_boot_arg
			
			kernel 部分按键的处理在kpd.c 中断回调函数 kpd_pwrkey_pmic_handler
			void kpd_pwrkey_pmic_handler(unsigned long pressed)
			{
			//Gionee <GN_BY_DRV> <wangguojun> <2017-10-17> modify for 124138 begin
				if(1==s_gn_clam)
				{
					return ;
				}
				else
				{
					kpd_print("Power Key generate, pressed=%ld\n", pressed);
					if (!kpd_input_dev) {
						kpd_print("KPD input device not ready\n");
						return;
				}
				kpd_pmic_pwrkey_hal(pressed);
			#if (defined(CONFIG_ARCH_MT8173) || defined(CONFIG_ARCH_MT8163))
				if (pressed) /* keep the lock while the button in held pushed */
					wake_lock(&pwrkey_lock);
				else /* keep the lock for extra 500ms after the button is released */
					wake_lock_timeout(&pwrkey_lock, HZ/2);
					//这一部分的处理就是说在释放powerkey之后还有500ms持锁
			#endif
				}
			}
			//Gionee <GN_BY_DRV> <wangguojun> <2017-10-17> modify for 124138 end
		}		
		
		6355的 MT6355_TOPSTATUS这个寄存器有homekey跟powerkey的检测
		preloader的keypad.c mtk_detect_key 检测按键
		
		按键相关的input子系统内定的编号
		linux-event-codes.h
		
		
		{
			相关的宏	
			BR_WDT_BY_PASS_PWK,MTK_KERNEL_POWER_OFF_CHARGING,kpoc_flag
			

			<3>[   10.779316]  (7)[185:battery_thread]in [fg_drv_update_hw_status] gn_boot_reason->4,gn_boot_mode->0,gn_call_state->0,gn_screenon_time->10
			<3>[   20.786597]  (5)[185:battery_thread]in [fg_drv_update_hw_status] gn_boot_reason->4,gn_boot_mode->0,gn_call_state->0,gn_screenon_time->20
			<3>[   30.792217]  (0)[185:battery_thread]in [fg_drv_update_hw_status] gn_boot_reason->4,gn_boot_mode->0,gn_call_state->0,gn_screenon_time->30
		}
		
		按键检测的流程大概是这样的：
		底层6355接收到按键触发的中断然后通过handler回调到(kpd.c)kpd_pwrkey_pmic_handler，这里有wake_lock持锁，按下的时候持锁，
		释放的时候还会持锁500ms，然后时（hal_kpd.c）kpd_pmic_pwrkey_hal 将按键的时间上报给input子系统的，(key_control.cpp) 之后应该是通过input
		子系统下面的fd节点，这里面是一个线程通过轮询的方式，如果有变化就会判断下一步亮屏关机充电的动画还是灭屏或者是重启之类的

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





	GNSPR#109502,后台播放音乐，进浏览器，插充电器无振动，不显示充电，也没有指示灯，拔掉充电器手机振动一下，再次插充电器恢复
	{
		//Gionee <GN_BSP_CHG> <lilubao> <20171120> add for debug begin
		增加了部分log， rt5081_pmu_charger.c ,gn_ti_drv2604l.c
	
	}



	
	
	
	GNSPR#135244,【老化测试】W919 机型 T0143 版本，充电测试未通过在电流测试中，老化测试异常
	{
		这里的问题是battery plug out 还是charger out
		bat_plug_out这个是电池拔出的log吗？
		mtk_battery.c
		battery_meter_ctrl(BATTERY_METER_CMD_GET_BOOT_BATTERY_PLUG_STATUS, &plugout_status_new);
		
		
		[fg_drv_update_hw_status] current:534 270 state:0 0 car:-220 -220 bat:4237 4243 chr:4834 4834 hwocv:1234 1234 bat_plug_out:1 1 tmp:34 34 imix 18750 rac 546
		
		[dod_init_result] NVRAM_ready 1 Embedded 1 plug_out 1 is_hwocv_unreliable 0 rtc_invalid 0 rtc_ui_soc 4300 two_sec_reboot 0 old_data.ui_soc 4374
	
	}
	
	
	
	

	



	GNSPR #128052,【品质压力】测机连接电脑USB端口，下拉状态栏显示USB已连接，传文件打开，电脑不显示便捷设备，换电脑，插拔USB端口不恢复，重启恢复
	{
		(mtk_chg_type_det.c) mt_charger_set_property,power_supply_property
		->
		dump_charger_name -> (mtk_usb.c) mt_usb_connect
	
		(mtk_charger.c) mtk_charger_int_handler
	
		wakeup_fg_algo 唤醒电量计的各种原因 然后封装fgd_msg上报给nl_send_to_user  user space
		typedef enum {
			FG_INTR_0 = 0,
			FG_INTR_TIMER_UPDATE  = 1,
			FG_INTR_BAT_CYCLE = 2,
			FG_INTR_CHARGER_OUT = 4,
			FG_INTR_CHARGER_IN = 8,
			FG_INTR_FG_TIME =		16,
			FG_INTR_BAT_INT1_HT =	32,
			FG_INTR_BAT_INT1_LT =	64,
			FG_INTR_BAT_INT2_HT =	128,
			FG_INTR_BAT_INT2_LT =	256,
			FG_INTR_BAT_TMP_HT =	512,
			FG_INTR_BAT_TMP_LT =	1024,
			FG_INTR_BAT_TIME_INT =	2048,
			FG_INTR_NAG_C_DLTV =	4096,
			FG_INTR_FG_ZCV = 8192,
			FG_INTR_SHUTDOWN = 16384,
			FG_INTR_RESET_NVRAM = 32768,
			FG_INTR_BAT_PLUGOUT = 65536,
			FG_INTR_IAVG = 0x20000,
			FG_INTR_VBAT2_L = 0x40000,
			FG_INTR_VBAT2_H = 0x80000,
			FG_INTR_CHR_FULL = 0x100000,
			FG_INTR_DLPT_SD = 0x200000,
			FG_INTR_BAT_TMP_C_HT = 0x400000,
			FG_INTR_BAT_TMP_C_LT = 0x800000,
			FG_INTR_BAT_INT1_CHECK = 0x1000000,
			FG_INTR_DUMP_INFO = 0x2000000,

		} FG_INTERRUPT_FLAG;

		FG_POWER_SUPPLY
		FG_MEM_DEBUG_READS
	}

	
	
	
	
	

	

}	
	
	
	
	
	
	

	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	









	
























/*********************************************************************************************************************************/
17G06A
{

	PPT 测试 power performance  thermal
	{
		usb-type-c-pericom
		CONFIG_USB_EXT_TYPE_C_PERICOM
		
		fuseblower qcom  的secure boot
		
		
		pericom-type-c@1d {
			compatible = "pericom,usb-type-c";
			reg = <0x1d>;
			vdd_io-supply = <&pm8917_l5>;
			interrupt-parent = <&tlmm>;
			interrupts = <97 2>;	/* TLMM 97, TRIGGER_FALLING */
			pericom,enb-gpio = <&tlmm 131 0x1>;	/* active low */
			pinctrl-names = "default";
			pinctrl-0 = <&usbc_int_default>;
			//Gionee <GN_BSP_CHG> <lilubao> <20171115> modify for remove typr-c begin
			status = "disabled";
			//Gionee <GN_BSP_CHG> <lilubao> <20171115> modify for remove typr-c end
		};
		
		06量产分支
		bj17g06a_main_dev
		
		status = "disabled";

		对于高通平台项目的签名方式要注意以下几点：
		{
			1. 压缩包名以 BJG1602A （以BJ打头具体项目名）打头；
			2. 必须 zip 格式（一定要在linux下压缩）；
			3. zip压缩包内不能包含目录；
			4. 压缩包内必须包含的文件列表：
				8976_fuseblower_USER.xml  ,8976_secimage.xml   
				这两个文件在L33_QCOM_8920_17G16A_170605_MODEM/gionee/BJ17G06/MSM8917.LA.3.0/common/sectools/config/目录下
				有高通平台的签名工具，xml里面是各种key和相关的镜像
				contents.xml这个在bp_image/ 目录下有 
				sign_img_list.txt这个不一定需要
				需要签名的镜像文件
		}

	}






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





		
		GNSPR#121275,开启手电筒后台播放音乐》进入游戏捕鱼电玩城后，按home键退出，点击app键卡顿，手电筒自动关闭，下拉状态栏手电筒图标显示开启，
		点击关闭后再次开启无异常 1台出现1次
		{
			首先要确定问题的log在哪个位置，跟哪些代码有关
			
			闪光灯
			drivers/media/platform/msm/camera_v2/sensor/flash/msm_flash.c
			
			相关的代码：
			kernel/msm-3.18/drivers/leds/leds-qpnp.c
			
			lut:Look Up Table参考表格
			LPG:light pulse generator
			VDD_TORCH，VDD_FLASH
			
			
			VPH_PWR can droop when the flash strobes and battery are low, or if other high current
			applications are running. If VPH_PWR droops too low, such as less than 2.5 V, UVLO might
			occur and cause the device to crash.
			To prevent UVLO events, the flash module monitors the VPH_PWR. If VPH_PWR is less than
			the VPH_PWR_DROOP_THRESHOLD, the hardware clamps the current to the programmed
			clamp current (software default is 200 mA). The VPH_PWR_DROOP_THRESHOLD software
			default is 3 V.
			If the current is less than the programmed clamp current when VPH_PWR hits the threshold, the
			current does not continue to rise to match the clamp current. Instead, it levels off until the flash
			strobe event ends.
			
			
			电源管理关掉了闪光灯
			frameworks/base/core/java/android/os/PowerManager.java
			10-08 11:51:35.249  1804  1804 E PowerManager: setTorchBrightness:0
			
			
			drivers/media/platform/msm/camera_v2/sensor/flash/msm_flash.c
			
			
			
			Run the following commands to test each flash or torch LED individually:
			adb shell "echo 255 > /sys/class/leds/led:flash_0/brightness"
			adb shell "echo 1 > /sys/class/leds/led:switch/brightness"
			
			Run the following commands to enable flash:
			adb shell "echo 255 > /sys/class/leds/led:flash_0/brightness"
			adb shell "echo 255 > /sys/class/leds/led:flash_1/brightness"
			adb shell "echo 1 > /sys/class/leds/led:switch/brightness"
			
			Run the following commands to disable flash
			adb shell "echo 0 > /sys/class/leds/led:flash_0/brightness"
			adb shell "echo 0 > /sys/class/leds/led:flash_1/brightness"
			adb shell "echo 0 > /sys/class/leds/led:switch/brightness"
			
			Run the following commands to turn on torch:
			adb shell "echo 100 > /sys/class/leds/led:torch_0/brightness"
			adb shell "echo 100 > /sys/class/leds/led:torch_1/brightnes"

			//Gionee <GN_BSP_CHG> <lilubao> <20171024> modify for torch begin
		}
		
		
		

		
	


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
				
				modem thermal manage
					Level 0 – No restriction, full modem performance
					Level 1 – Requests the modem to run the data throughput reduction algorithms
					Level 2 – MTPL back off / PUCCH back off
					Level 3 – Puts the modem into Limited Service mode, in which only emergency 911
					calls allowed
					
				 thermal-engine -o
				 
				 /vendor/qcom/proprietary/thermal-engine/	
					
				80-N9649-1 Thermal Tuning Procedure	

			}
			
			
			debug
			{ 
				Enable more logging for KTM
				echo 8 > /proc/sys/kernel/printk
				echo 'file msm_thermal.c +p' > /sys/kernel/debug/dynamic_debug/control
				
				
				adb	logcat –v time –s ThermalEngine
				
				
				adb shell thermal-engine –o
				
				
				adb pull /etc/thermal-engine.conf
				adb
				remount
				<edit>
				adb push thermal-engine.conf /etc/
			
			
				adb push msm_tsens_logging /data
				adb shell
				chmod 777 /data/msm_tsens_logging
				./msm_tsens_logging a b &
				
				
				stop thermal-engine
				start thermal-engine –d
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








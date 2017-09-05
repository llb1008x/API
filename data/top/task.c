

log关键字
{
	fg_drv_update_hw_status  上报一些硬件信息
	
	
	[FGADC_intr_end][FG_INTR_IAVG]
	read_fg_hw_info
	
	
	
	可以通过检索Call trace，察看内存堆栈函数的调用


	ibus，ibat可以检索相关的电流电压信息，这写都在一块

	[248:charger_thread][name:mtk_charger&]Vbat=4321,I=3597,VChr=492,T=33,Soc100,CT:0:1

	控制fuel_gauge的log等级
	FG_daemon_log_level 	

	rtc记录的电量
	BATTERY_METER_CMD_SET_RTC_UI_SOC	
	获取上层电量
	FG_status.ui_soc=battery_get_bat_uisoc


	healthd线程检测电池的状态
	healthd]: healthd: battery l=5 v=3796 t=41.0 h=2 st=4 chg=	


	FG_DAEMON_CMD_GET_VBAT
	FG_DAEMON_CMD_GET
	FG_DAEMON_CMD_SET_KERNEL_SOC
	FG_DAEMON_CMD_SET_KERNEL_UISOC
	
	
	获取lte rspr信号强弱	
	GnGetLteLevel(), rsrpIconLevel =1, standard LTE, mLteRsrp =-116,isCampOnLte =true
	
	
	performance相关的代码目录
	kernel-4.4/drivers/misc/mediatek/base/power
	
	
	/home/llb/project/PRO/source/17G10A/L31_6757_66_N_17G10A_NO.MP5_V1.53_170512_ALPS/android_mtk_mp/vendor/mediatek/proprietary/hardware/perfservice/mt6757/app_list/perfservapplist.txt


}



/*********************************************************************************************************************************


*********************************************************************************************************************************/
17G10A 
{
	给充电电流加一个上限
20170817
	1.手上的BUG													20170817
		还有两类BUG：一类是低电插上充电器不识别，关机；一类是USB不识别问题
		已经提case
		
		
		
		
	2.温升问题，温升降电流有没有起作用							20170818
		主要是分场景降低充电电流，比如打电话，拍照，听音乐，玩游戏等(具体是否还有其他还有带确认)
		温升部分主要有两个一个是上面的根据不同场景降低充电电流问题		计划今天完成
		还有一类是MTK的 thermal regulator，bcct这部分要熟悉相关文档，代码
	
	

		
	3.电量显示的不准												20170818
		这个的主要问题是充电时候电池电压跟电量显示差别比较大(差别在10%以上)，放电比较准确
		这部分跟GM3.0的客制化有关，但是相关的文档还是不熟悉，必须弄清楚,相关的软件自己首先要熟悉









	5.相关测试

	整机测试：													20170817
	{
		这几个问题都是温升相关的问题
	
		3.配合旅充充电测试       2PCS     NG  开机待机状态下充电，手机表面温度偏高，正面温度为38.6℃、背面温度为44.3℃，（标准为金属≤38℃；塑胶/橡胶 ≤43℃；玻璃≤41℃）。
		5.配合电脑USB充电功能测试      2PCS        NG  关机状态下手机连接电脑充电，电量充电至60%后充电电流由466mA将至370mA，跳变的原因最好能找到。
		{
			高温高湿测试NG										20170821
			这个测试场景是：
				1.手机先关机放在温度57度的温箱里面，一小时左右，大概环境温度稳定在57度，
				2.开机，将手机放在温箱里面待机2h，此时手机不关机
				
				
			9、高温高湿测试      5PCS     NG  5PCS手机在测试过程中会因为温度过高关机。环境温度设置为57℃，但手机的电池检测温度会达到58度以上。
软件这边 电池58度 应该不会关机，60度 关机，请知悉；



			 我只看到没有关机状态下，手机电池电压检测为58℃，没有关机，至于关机时手机的电池检测温度没法看到，所以我说的是“58℃以上”，看清楚。
     这问题跟手机待机时电池温度有关！环境设置温度为57℃，请知悉！	
			
			
		}
		
		
		
		
		
		
		
		
	
	
/**************************************************************************************/	
		
		
		
		
		//Gionee <GN_BSP_CHG> <lilubao> <20170826> modify for thermal manage begin
		
		现在问题分成两类：
		
		温升测试失败
		{
			1.一类是几种场景下充电温度偏高
			{
				这种要查找thermal是否起作用了，thermal管了那些地方，参数什么意思，怎么改？
			
				bcct:battery charging current throtting
				这个是在触发温度条件后就设定充电电流
				abcct:adaptive battery charging current throtting
				这个是在设定的目标温度的时候在最高和最低温度范围内动态调节电流
			
				根据以上两个设定chrlimt
			
			
				abcct_lcmoff 是在lcm off的时候启动的，如果灭屏的情况下温度过高设定的策略 
			
			
				 * sscanf format <klog_on> <mtk-cl-bcct00 limit> <mtk-cl-bcct01 limit> ...
				 * <klog_on> can only be 0 or 1
				 * <mtk-cl-bcct00 limit> can only be positive integer or -1 to denote no limit
		
				前面的1是打开thermal debug log的
				echo 1 1200 1000 800 > /proc/driver/thermal/clbcct
			
			
			
				config文件的相关参数
			
				/proc/driver/thermal/clabcct
				40000 1000 200000 5 2000 500 0 3000 0 1 5000 2000
			

			
				当前abcct的配置
				abcct

					abcct_cur_bat_chr_curr_limit 3000
					abcct_cur_chr_input_curr_limit -1
					abcct_pep30_cur_input_curr_limit 5000
				
					abcct_target_temp 44000
					abcct_kp 1000
					abcct_ki 200000
					abcct_kd 5
					abcct_max_bat_chr_curr_limit 3000
					abcct_min_bat_chr_curr_limit 0
					abcct_input_current_limit_on 0
					abcct_HW_thermal_solution 3000
					abcct_min_chr_input_curr_limit 0
					abcct_times_of_ts_polling_interval 1
					abcct_pep30_max_input_curr_limit 5000
					abcct_pep30_min_input_curr_limit 2000
				
				
				tzbts	这个是默认的参数，修改了几个策略  20170826 
				/proc/driver/thermal/tzbts
				6 100000 0 mtktsAP-sysrst 90000 0 mtk-cl-shutdown00 62000 0 mtk-cl-cam00 50000 0 abcct_lcmoff 44000 0 mtk-cl-adp-fps 42000 0 abcct 0 0 no-cooler 0 0 no-cooler 0 0 no-cooler 0 0 no-cooler 1000	
			
			
				alert diaglog
			
				mtk game detection service
			
			
				这个文件在devices/gionee_bj 目录下有效的，编译的时候可能要全编
			
			
				亮屏 251mA
				cam  600mA
			}
		
		
		
			2.另一类是高温高湿测试，手机关机的情况
			{
				这个在测试的时候没有关机，但是亮屏看温度的时候手机关机，电池温度达到了60度
				
				1.手机整体温度跟电池温度差多少
				2.NTC是否准确
				3.手机整体功耗偏高
				

				这个pmic读的值是准确的的，经过校准后，温度偏低了1度
				mtk_battery.c
				force_get_tbat_internal
				
			}
			
			
			
			3.低温关机
			{
				
			
			}
			
		
		}
		
		
		
		
		
		
		


	}
	



/****************************************************************************************/
	
	
	电量显示有问题
	{
		手机低电充电有问题，充电电流只有300~400mA
		
		常温 充电到5%、15%、30%、50%时，电池电量与ZCV曲线对应电量百分比差值大于5%
		
		
		
		#105124
		卡1移动4G，电量耗尽，标配旅充2A充电》第四晚待机，电量耗尽-第二天早08：30关机充电-两台机同时充电，15min后测试机电量1%，对比机电量15%-18min后测试机电量4%，对比机电量18%-开机充电-26min后测试机电量11%，对比机电量26%》再次操作恢复 对比S10无此现象 
		
		
		
		
	
	}	
		
		
		
		
		
		
		


/****************************************************************************************/	



	
	redmine
	{
	
		#103678	接听或者挂断电话时，振动的同时会有崩的一声。
		{
			降低震动强度或者震动时间
			//Gionee <GN_BSP_CHG> <lilubao> <20170829> modify for vibrator begin
			
			还有一个问题是开机振动偏弱
			
			首先要知道是调用的过程，然后是如何减小振动的强度？
		
		
			pDrv2604Platdata->GpioTrigger=0;
			pDrv2604Platdata->loop=CLOSE_LOOP;
			pDrv2604Platdata->RTPFormat=Signed;
			pDrv2604Platdata->BIDIRInput=BiDirectional;
				
		}
	
	

	
		#99363	****
		待机一晚上早上手机电量1%时，连接充2A电器不充电，连充电器30min插拔充电器不恢复，30min后待电量自动耗完再次连充电器恢复
		{
			1
			分析：
			2017-8-16 08：12左右插入充电器，不充电，然后关机，关机也不显示关机充电
	
				<6>[    5.475795]  (0)[182:irq/773-rt5081_]rt5081_pmu_charger rt5081_pmu_charger: rt5081_pmu_attachi_irq_handler
				<6>[    5.475882]  (0)[182:irq/773-rt5081_]rt5081_pmu_charger rt5081_pmu_charger: rt5081_inform_psy_changed: online = 1, type = 1
				<6>[    5.475892]  (0)[182:irq/773-rt5081_]mt_charger_set_property
				<6>[    5.475899]  (0)[182:irq/773-rt5081_]mt_charger_set_property
				<3>[    5.475906]  (0)[182:irq/773-rt5081_]dump_charger_name: charger type: 1, Standard USB Host
				<3>[    5.475922]  (0)[182:irq/773-rt5081_]mtk_charger_int_handler
				<3>[    5.475928]  (0)[182:irq/773-rt5081_]wake_up_charger
				<3>[    5.475948]  (0)[182:irq/773-rt5081_][fg_charger_in_handler] notify daemon 0 1
				<6>[    5.475965]  (0)[182:irq/773-rt5081_]rt5081_pmu_charger rt5081_pmu_charger: rt5081_enable_chgdet_flow: en = 0
				<3>[    5.476009]  (0)[245:kworker/0:1][U3D]U3D_LTSSM_INTR: 0
				<6>[    5.476122]  (6)[182:irq/773-rt5081_]rt5081_pmu_charger rt5081_pmu_charger: rt5081_set_usbsw_state: state = 1
	
			充电器可能没有插好 因为软件没有走那步流程
			MTK case ID：ALPS03468158

		}



		
		#99348
		手机连接电脑USB打开传文件和临时USB调试后，电脑不显示连接到手机，再次插拔USB不恢复，插其它手机可以正常识别，重启恢复
		{
			这个问题是连接到PC没有显示盘符，重启后恢复，08：03~08：05
		
			MTK case ID：ALPS03469265
		}
		
		
		
		pass#100910，低温0℃充电未停止充电且没有提示语，高温55℃
		{
			2PCS手机因电池温度过高关机，但环y箱里设定温度为57度，要求做高温高湿测试时手机可以有高温安全警告，但不允许执行关机机制； 
	
			低温测试，-20度，手机自动关机
			
			mt6757.dtsi			
			//Gionee <GN_BSP_CHG> <lilubao> <20170823> modify for low temp notify begin
			min_charge_temperature = <2>;
			min_charge_temperature_plus_x_degree = <4>;
			max_charge_temperature = <55>;
			max_charge_temperature_minus_x_degree = <50>;
			//Gionee <GN_BSP_CHG> <lilubao> <20170823> modify for low temp notify end

			mtk_charger.c
			//Gionee <GN_BSP_CHG> <lilubao> <20170823> modify for low temp notify begin
			#define BAT_LOW_TEMP_PROTECT_ENABLE 
			//Gionee <GN_BSP_CHG> <lilubao> <20170823> modify for low temp notify end

		}
		
		
		#功耗相关的case还没解决
		{
			 ALPS03466994 【GIONEEBJ】【待机功耗】【联通卡不开数据，不开wifi，平均电流高】

			 ALPS03467151 【GIONEEBJ】【待机功耗】【不插sim卡，开wifi，待机平均电流高】 

			 ALPS03467419 【GIONEEBJ】【待机功耗】【不插sim卡，不开wifi，飞行模式下待机平均电流高】
		
		}
	
 		关机充电logo分几档，0~10,10~35,35~70,70~99,100，如果二十几是这种应该正常
	}
	
	
	
	
		
}



































/**********************************************************************************************************************************************************


***********************************************************************************************************************************************************/
17G06A
{
	相关文件
	{
		arch
			./arch/arm64/boot/dts/qcom/msm8917-pmi8937-qrd-sku5.dtsi
			./arch/arm64/boot/dts/qcom/msm-pmi8937.dtsi

	}
	





  USB：相关的有好几个问题
  {
  	1.usb眼图失败
  	{
		眼图测试失败的case
			You can follow doc 80-PB524-1 usb_tunning_-_phy_eye_diagram_for_oems(1).pdf
			and 80-NA648-1_H_Tuning_USB_PHY_Eye_Diagram_Receiver_Sensitivity.pdf on eye diagram tunning. 
  	
  	
  	
  	 	现在是分成host/devics两种mode		kba-170123171355
  	 	  
  	 	value  addr  value addr  value addr  
	  	qcom,hsusb-otg-phy-init-seq =
		//Gionee <gn_by_CHG> <lilubao> <20170821> modify for USB eye diagram begin
			<0x74 0x80 0x3c 0x81 0x3d 0x82 0x33 0x83 0xffffffff>;
		qcom,hsusb-otg-phy-host-init-seq =
			<0x74 0x80 0x39 0x81 0x1d 0x82 0x33 0x83 0xffffffff>;
		//Gionee <gn_by_CHG> <lilubao> <20170821> modify for USB eye diagram end
  	 	  
  	 	  
		80-na648-1a
		
  	
  	}
  	
  	
  	
  	2.LD0上VDD_USB_CORE ，VDD_USBBPHY_1P8电压偏高
  	
  	3.diag口打不开

	#96132，adb端口被占用			
	2017.8.10
	{
	 问题：
		现在要解决的是如何让diag口跟adb如何共用，就是打开diag口同时可以使用adb命令
	
	 
	 分析：
		17G06A不能用，17G02A可以使用，对比两个项目的代码，初始化到打开端口的log
		
		对比代码：	

	

		涉及到的文件：
		{
			init.qcom.usb.rc
			init.gn.platform.rc
			init.msm.usb.configfs.rc
			init.qcom.factory.rc
		
		
			./alps_drv/device/qcom/common/rootdir/etc/init.qcom.usb.rc
		}
	}


	#96141，*#837504#手动开启diag口，PC端和工具找不到diag口
	2017.8.10
	{
		打开diag口之后可能是无法识别，然后USB端口无法使用 pid 9001
		是不是USB的vid，pid配置不对
		直接连接PC  pid 9039
		打开diag口  pid 9091
		
		
		getprop |grep usb
		[init.svc.qcom-usb-sh]: [stopped]
		[init.svc.usb_uicc_daemon]: [stopped]
		[init.svc.usb_uicc_enable]: [stopped]
		[persist.sys.usb.config]: [diag,serial_smd,rmnet_qti_b
		[persist.sys.usb.config.extra]: [none]
		[sys.usb.config]: [diag,serial_smd,rmnet_qti_bam,adb]
		[sys.usb.ffs.ready]: [1]
		[sys.usb.rps_mask]: [0]
		[sys.usb.state]: [diag,serial_smd,rmnet_qti_bam,adb]
	}
	
	
	
	
	
		QCAP 解析dump文件
		https://cap.qti.qualcomm.com

	
		
	
	
	
	
  	
  }
  
  
  
  
  
  
  
  
  打开飞行模式电流3~4mA，关闭飞行模式之后，其他全关闭，电流18mA以上
  {
  
  platform版本
    开飞航模式后02:39到04:52没有什么请其它交互，ril没怎么工作。
	01-01 00:02:36.061  1976  1976 D RILJ    : [3759]> RADIO_POWER off [SUB0]
	01-01 00:02:36.134  1976  1976 D RILJ    : [3760]> RADIO_POWER off [SUB1]
	01-01 00:02:39.598  1976  1976 D QtiGsmDCT: [1]screen off
	01-01 00:02:39.598  1976  1976 D QtiGsmDCT: [1]stopNetStatPoll
	01-01 00:02:39.599  1976  1976 D QtiSubscriptionController: getPhoneId, received dummy subId 2147483644
	01-01 00:02:39.603  1976  1976 D QtiSubscriptionController: getPhoneId, received dummy subId 2147483644
	01-01 00:04:52.365  1976  1976 D RILJ    : [3790]> SCREEN_STATE: true [SUB0]


	关闭飞航后从屏幕灭屏开始中间也是很长时间按ril没工作，到09:28后才有ril的处理。
	01-01 00:04:55.563  1976  1976 D RILJ    : [3792]> RADIO_POWER on [SUB0]
	01-01 00:04:55.572  1976  1976 D RILJ    : [3793]> RADIO_POWER on [SUB1]
	01-01 00:04:57.784  1976  1976 D QtiGsmDCT: [1]screen off
	01-01 00:04:57.784  1976  1976 D QtiGsmDCT: [1]stopNetStatPoll
	01-01 00:04:57.786  1976  1976 D QtiSubscriptionController: getPhoneId, received dummy subId 2147483644
	01-01 00:04:57.788  1976  1976 D QtiSubscriptionController: getPhoneId, received dummy subId 2147483644
	01-01 00:09:28.356  1976  1976 D RILJ    : [3829]> RIL_REQUEST_GET_ACTIVITY_INFO [SUB0]

	所以从log看ril屏灭后都没有其它多余的操作，wakelock的东西就开机的时候有后面搜不到。
	
	
	关闭sensor
	android_qcom_mp/gionee/project/BJ17G06/vendor/qcom/proprietary/sensors/dsps/reg_defaults/sensor_def_qcomdev.conf
	
	adb remount
	adb shell rm /etc/sensors/sensor_def_qcomdev.conf
	adb push sensor_def_qcomdev.conf  /etc/sensors
	adb shell chmod 644 /etc/sensors/sensor_def_qcomdev.conf
	adb shell rm /persist/sensors/sns.reg
	adb reboot
	
	
	
	1.
	echo 1 > /sys/module/msm_show_resume_irq/parameters/debug_mask
	echo 1 > /sys/kernel/debug/clk/debug_suspend
	echo 8 > /sys/module/mpm_of/parameters/debug_mask

	2. 
	adb shell dmesg -c
	adb shell dumpsys batterystats --enable full-wake-history
	adb shell dumpsys batterystats --reset

	3. plug out USB and perform your test

	4. adb bugreport > bugreport.txt
  
  }
  
  


  
  
  
  
  
  
  
  
  
  
  
  
  
  充电相关
  {
  		测量电池曲线	 case ID:03075361   20170807 
		{
			battery ID：15k
			NTC：9k
			电池容量：4000mAh
		}

  
  }


}













/********************************************************************************************************************************



********************************************************************************************************************************/











/*遗留的问题*/
{

G1605A  
{
	售后问题，功耗问题 healthd线程

	apk发包次数，谁发的(主从)，就是链接到哪了？？
    那个脚本怎么用
}



17G10A
{
	底电流偏高
	{
		这里主要是android系统的睡眠唤醒机制，然后为了便于分析功耗问题
		
		一下是几个功耗底电流调试相关文档
	    80-p0897-1_a   
	    80-p0955-1sc_b 
	    高通bsp技术期刊 2014/11/28
		
	
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
		
		
		5.系统进入suspend 还有一些低功耗模式，一些流程
		
		
		5.分析思路

		平均电流9mA，lcd有3mA，其他还有1mA
		1mA这个还有待查
		
		
		1.跟系统通知有关，即使飞行模式下，系统通知也会每隔一段时间会有唤醒ap检测一些信息如网络连接，想推送一些内容
			这部分波形比较高持续时间比较长 40~60s
		  现在是每隔10min唤醒一次，去除之后电流从 11mA 降到9mA，而且稳定，电流没有跳变	
		  
		2.短的异常波形跟中断有关，eint - 150，中断唤醒的持续时间较短5~10s 
		  初步分析跟串口同modem之间的通信有关
		01-01 08:06:14.378303 01-01 08:06:14.378302  1436  1436 I [  368.329961]-(0)[1436:system_server][name:irq_mtk_eic&]EINT_STA:  
		01-01 08:06:14.378303 01-01 08:06:14.378302  1436  1436 I [  368.329961]-(0)[1436:system_server][name:irq_mtk_eic&]: EINT Module - index:128,EINT_STA = 0x400000
		01-01 08:06:14.378303 01-01 08:06:14.378302  1436  1436 I [  368.329961]-(0)[1436:system_server][name:irq_mtk_eic&]: EINT 150 is pending
		01-01 08:06:14.378303 01-01 08:06:14.378302  1436  1436 I [  368.329961]-(0)[1436:system_server][name:irq_mtk_eic&]:  
		01-01 08:06:14.378303 01-01 08:06:14.378302  1436  1436 F [  368.329961]-(0)[1436:system_server][name:mtk_spm_sleep&]: [SPM] dump ID_DUMP_MD_SLEEP_MODE
		01-01 08:06:14.378303 01-01 08:06:14.378302  1436  1436 I [  368.329961]-(0)[1436:system_server]: Resume caused by IRQ 27, SPM
		01-01 08:06:14.378303 01-01 08:06:14.378302  1436  1436 D [  368.329961]-(0)[1436:system_server][name:mtk_wdt&]: mtk_wdt_mode_config  mode value=dd, tmp:220000dd,pid=1436
		01-01 08:06:14.378303 01-01 08:06:14.378302  1436  1436 D [  368.329961]-(0)[1436:system_server][name:mtk_wdt&]: [WDT] resume(1)
		01-01 08:06:14.378325 01-01 08:06:14.378324  1436  1436 E [  368.329983]-(0)[1436:system_server][name:ccci&]: [ccci1/mcd]ccci_modem_sysresume
		01-01 08:06:14.379268 01-01 08:06:14.379267  1436  1436 E [  368.330926]-(0)[1436:system_server][name:ccci&]: [ccci1/mcd]Resume no need reset cldma for md_state=1
		01-01 08:06:14.380476 01-01 08:06:14.380475  1436  1436 I [  368.332134]-(0)[1436:system_server][name:atf_log&]atf_time_sync: resume sync
		01-01 08:06:14.381082 01-01 08:06:14.381081  1436  1436 E [  368.332740]-(0)[1436:system_server][name:mtk_cpufreq_hybrid&]: [CPUHVFS] (1) cluster0: opp = 4 (0 - 11), freq = 1508000, volt = 0x56
		01-01 08:06:14.382604 01-01 08:06:14.382603  1436  1436 E [  368.334262]-(0)[1436:system_server][name:mtk_cpufreq_hybrid&]: [CPUHVFS] (1) cluster1: opp = 15 (0 - 9), freq = 494000, volt = 0x56
		01-01 08:06:14.384157 01-01 08:06:14.384156  1436  1436 E [  368.335815]-(0)[1436:system_server][name:mtk_cpufreq_hybrid&]: [CPUHVFS] (1) [00000001] cluster0 on, pause = 0x9, swctrl = 0x20f4 (0x56bb)
		01-01 08:06:14.386208 01-01 08:06:14.386207  1436  1436 I [  368.337866].(0)[1436:system_server][name:cpu&]: Enabling non-boot CPUs ...
		01-01 08:06:14.388081 01-01 08:06:14.388080     0     0 I [  368.339739]-(1)[0:swapper/1][name:cpuinfo&]: Detected VIPT I-cache on CPU1
		01-01 08:06:14.388120 01-01 08:06:14.388119     0     0 I [  368.339778]-(1)[0:swapper/1][name:irq_gic_v3&]CPU1: found redistributor 1 region 0:0x000000000c220000
		01-01 08:06:14.388249 01-01 08:06:14.388248     0     0 I [  368.339907]-(1)[0:swapper/1][name:smp&]CPU1: Booted secondary processor [410fd034]
		01-01 08:06:14.389321 01-01 08:06:14.389320  1436  1436 I [  368.340979].(1)[1436:system_server][name:cpu&]: CPU1 is up
		
		3.lcd内屏+外屏 约有 3~4mA电流，这个导致整体的底电流在11mA+
		
		
		
		1->
		EINT wakelock	63		548		0		63		0		51272		2932		296780		0
		2->
		EINT wakelock	90		584		13		89		71		65361		2932		445681		0




		01-01 08:06:14.636166 01-01 08:06:39.186808   228   228 E [  368.587824].(2)[228:kworker/u16:5][name:msdc&]: [msdc]msdc1, some of device s pin, dat1~3 are stuck in low!
		01-01 08:06:14.637566 01-01 08:06:39.188208   228   228 E [  368.589224].(2)[228:kworker/u16:5][name:msdc&]: [msdc]msdc_ops_switch_volt msdc1 set voltage to 3.3V.

		msdc_check_dat_1to3_high
		msdc_ops_switch_volt


		wakeup_reason.c   
		irq_mtk_eic.c

		max_eint_num = <160>;

		mt_eint_print_status


		01-01 08:06:14.378303 01-01 08:06:14.378302  1436  1436 I [  368.329961]-(0)[1436:system_server][name:irq_mtk_eic&]EINT_STA:  
		01-01 08:06:14.378303 01-01 08:06:14.378302  1436  1436 I [  368.329961]-(0)[1436:system_server][name:irq_mtk_eic&]: EINT Module - index:128,EINT_STA = 0x400000
		01-01 08:06:14.378303 01-01 08:06:14.378302  1436  1436 I [  368.329961]-(0)[1436:system_server][name:irq_mtk_eic&]: EINT 150 is pending

		01-01 08:06:14.378303 01-01 08:06:14.378302  1436  1436 I [  368.329961]-(0)[1436:system_server][name:irq_mtk_eic&]:  
		01-01 08:06:14.378303 01-01 08:06:14.378302  1436  1436 F [  368.329961]-(0)[1436:system_server][name:mtk_spm_sleep&]: [SPM] dump ID_DUMP_MD_SLEEP_MODE
		01-01 08:06:14.378303 01-01 08:06:14.378302  1436  1436 I [  368.329961]-(0)[1436:system_server]: Resume caused by IRQ 27, SPM
		01-01 08:06:14.378303 01-01 08:06:14.378302  1436  1436 D [  368.329961]-(0)[1436:system_server][name:mtk_wdt&]: mtk_wdt_mode_config  mode value=dd, tmp:220000dd,pid=1436
		01-01 08:06:14.378303 01-01 08:06:14.378302  1436  1436 D [  368.329961]-(0)[1436:system_server][name:mtk_wdt&]: [WDT] resume(1)
		01-01 08:06:14.378325 01-01 08:06:14.378324  1436  1436 E [  368.329983]-(0)[1436:system_server][name:ccci&]: [ccci1/mcd]ccci_modem_sysresume

		01-01 08:06:14.379268 01-01 08:06:14.379267  1436  1436 E [  368.330926]-(0)[1436:system_server][name:ccci&]: [ccci1/mcd]Resume no need reset cldma for md_state=1
		01-01 08:06:14.380476 01-01 08:06:14.380475  1436  1436 I [  368.332134]-(0)[1436:system_server][name:atf_log&]atf_time_sync: resume sync

		01-01 08:06:14.381082 01-01 08:06:14.381081  1436  1436 E [  368.332740]-(0)[1436:system_server][name:mtk_cpufreq_hybrid&]: [CPUHVFS] (1) cluster0: opp = 4 (0 - 11), freq = 1508000, volt = 0x56
		01-01 08:06:14.382604 01-01 08:06:14.382603  1436  1436 E [  368.334262]-(0)[1436:system_server][name:mtk_cpufreq_hybrid&]: [CPUHVFS] (1) cluster1: opp = 15 (0 - 9), freq = 494000, volt = 0x56
		01-01 08:06:14.384157 01-01 08:06:14.384156  1436  1436 E [  368.335815]-(0)[1436:system_server][name:mtk_cpufreq_hybrid&]: [CPUHVFS] (1) [00000001] cluster0 on, pause = 0x9, swctrl = 0x20f4 (0x56bb)
		01-01 08:06:14.386208 01-01 08:06:14.386207  1436  1436 I [  368.337866].(0)[1436:system_server][name:cpu&]: Enabling non-boot CPUs ...
		01-01 08:06:14.388081 01-01 08:06:14.388080     0     0 I [  368.339739]-(1)[0:swapper/1][name:cpuinfo&]: Detected VIPT I-cache on CPU1
		01-01 08:06:14.388120 01-01 08:06:14.388119     0     0 I [  368.339778]-(1)[0:swapper/1][name:irq_gic_v3&]CPU1: found redistributor 1 region 0:0x000000000c220000
		01-01 08:06:14.388249 01-01 08:06:14.388248     0     0 I [  368.339907]-(1)[0:swapper/1][name:smp&]CPU1: Booted secondary processor [410fd034]
		01-01 08:06:14.389321 01-01 08:06:14.389320  1436  1436 I [  368.340979].(1)[1436:system_server][name:cpu&]: CPU1 is up

		01-01 08:06:14.393848 01-01 08:06:14.393847  1436  1436 I [  368.345506].(1)[1436:system_server][name:main&]PM: noirq resume of devices complete after 3.684 msecs



		01-01 08:06:21.488047 01-01 08:06:46.038689  1436  1436 I [  375.439705].(0)[1436:system_server][name:wakeup&]PM: Wakeup pending, aborting suspend

		01-01 08:06:21.489259 01-01 08:06:46.039901  1436  1436 I [  375.440917].(0)[1436:system_server][name:wakeup&]: last active wakeup source: EINT wakelock

		01-01 08:06:21.490324 01-01 08:06:46.040966  1436  1436 W [  375.441982].(0)[1436:system_server][name:process&]:  
		01-01 08:06:21.490956 01-01 08:06:46.041598  1436  1436 E [  375.442614].(0)[1436:system_server][name:process&]: Freezing of tasks aborted after 0.011 seconds[name:process&]
		01-01 08:06:21.491954 01-01 08:06:46.042596  1436  1436 I [  375.443612].(0)[1436:system_server][name:process&]: Restarting tasks ... [name:process&]done.
		01-01 08:06:21.508172 01-01 08:06:46.058814  1436  1436 I [  375.459830].(1)[1436:system_server][name:pmic_auxadc&]: [mt6355_get_auxadc_value] ch = 5, reg_val = 0x630, adc_result = 696
		01-01 08:06:21.511222 01-01 08:06:46.061864  1436  1436 I [  375.462880].(0)[1436:system_server][name:pmic_auxadc&]: [mt6355_get_auxadc_value] ch = 2, reg_val = 0x369, adc_result = 767
		01-01 08:06:21.512974 01-01 08:06:46.063616  1436  1436 E [  375.464632].(0)[1436:system_server][name:mtk_battery_hal&]: [fgauge_read_current] final current=691 (ratio=950)
		01-01 08:06:21.514261 01-01 08:06:46.064903  1436  1436 E [  375.465919].(0)[1436:system_server][name:mtk_battery&]: [BattVoltToTemp] 768 24000 2800 1
		01-01 08:06:21.515297 01-01 08:06:46.065939  1436  1436 E [  375.466955].(0)[1436:system_server][name:mtk_battery&]: [force_get_tbat] 767,768,0,69,100,27 r:50 100
		01-01 08:06:21.516613 01-01 08:06:46.067255  1436  1436 D [  375.468271].(0)[1436:system_server]rt5081_pmu 5-0034: rt5081_pmu_reg_block_write: reg 07 size 4
		01-01 08:06:21.518169 01-01 08:06:46.068811  1436  1436 I [  375.469827].(0)[1436:system_server]rt5081_pmu_charger rt5081_pmu_charger: rt5081_enable_hidden_mode: en = 1
		01-01 08:06:21.519456 01-01 08:06:46.070098  1436  1436 D [  375.471114].(0)[1436:system_server]rt5081_pmu 5-0034: rt5081_pmu_reg_update_bits: reg 21 data c0
		01-01 08:06:21.520657 01-01 08:06:46.071299  1436  1436 D [  375.472315].(0)[1436:system_server]rt5081_pmu 5-0034: rt5081_pmu_reg_update_bits: mask f0
		01-01 08:06:21.521933 01-01 08:06:46.072575  1436  1436 D [  375.473591].(0)[1436:system_server]rt5081_pmu 5-0034: rt5081_pmu_reg_update_bits: reg 21 data 01
		01-01 08:06:21.523070 01-01 08:06:46.073712  1436  1436 D [  375.474728].(0)[1436:system_server]rt5081_pmu 5-0034: rt5081_pmu_reg_update_bits: mask 01
		01-01 08:06:21.539516 01-01 08:06:46.090158   235   235 I [  375.491174].(0)[235:hps_main]MobiCore mcd: Cpu 1 is going to die
		01-01 08:06:21.542572 01-01 08:06:46.093214   235   235 I [  375.494230].(0)[235:hps_main][name:smp&]CPU1: shutdown
		01-01 08:06:21.543753 01-01 08:06:46.094395   235   235 I [  375.495411].(0)[235:hps_main][name:psci&]psci: CPU1 killed.
		01-01 08:06:21.546277 01-01 08:06:46.096919   235   235 I [  375.497935].(0)[235:hps_main]MobiCore mcd: Cpu 1 is dead

		
		sd卡模块去除
		cust_mt6757_msdc.dtsi
		&mmc1 {
			clk_src = /bits/ 8 <MSDC30_CLKSRC_200MHZ>;
			bus-width = <4>;
			max-frequency = <200000000>;
			msdc-sys-suspend;
			cap-sd-highspeed;
			sd-uhs-sdr12;
			sd-uhs-sdr25;
			sd-uhs-sdr50;
			sd-uhs-sdr104;
			sd-uhs-ddr50;
			pinctl = <&mmc1_pins_default>;
			pinctl_sdr104 = <&mmc1_pins_sdr104>;
			pinctl_sdr50 = <&mmc1_pins_sdr50>;
			pinctl_ddr50 = <&mmc1_pins_ddr50>;
			register_setting = <&mmc1_register_setting_default>;
			host_function = /bits/ 8 <MSDC_SD>;
			cd_level = /bits/ 8 <MSDC_CD_LOW>;
			cd-gpios = <&pio 30 0>;
			status = "okay";
			vmmc-supply = <&mt_pmic_vmch_ldo_reg>;
			vqmmc-supply = <&mt_pmic_vmc_ldo_reg>;
		};


		其中的
		status = "okay";
		修改为：
		status = "disabled";
		}
		
		这个直接这样改是有问题的
	}
	
	
	
	
	
	GM3.0相关
	{
		电池曲线的导入		7.27
		{
		
			涉及到哪些文件,哪些细节
			{
				mtk_battery_table.h  ，mtk_battery_property.h
				mt6757_battery_prop_ext.dtsi,mt6757_battery_table_ext.dtsi
			
				GM3.0的客制化步骤
				{
					1.测量car_tune_value 系数
				
					2.利用GMAT_TOOL导出 
					-> battery_prop_ext.dtsi
				
					-> ZCV table，battery_table_ext.dtsi
				
					3.工厂模式下利用 ATE_tool校准参数Rfg ，meta模式下修改NVRAM中存储的car_tune_value
					这种应该是单独修正每一台机器的car_tune_value的值，因为这个值不同机器可能不一样
				}
			}
		
		

			电量计相关的debug
			{
				//Gionee <gn_by_CHG> <lilubao> <20170804> add for fuelgauge begin
				具体放充电库伦值大小
				[FGADC_intr_end][FG_INTR_IAVG]
				read_fg_hw_info
			
			
				打开电量计相关的log
				adb shell setprop persist.mediatek.fg.log.enable 1
			
				手动提高fg log等级
				adb shell
				echo 8 > /sys/bus/platform/devices/battery/FG_daemon_log_level
			
				Gauge Low power mode 关闭方法
				adb shell
			
				echo 3000 > /sys/devices/platform/mt-pmic/pmic_access
			
				cat /sys/devices/platform/mt-pmic/pmic_access
			
				看看出來的值是多少		regiser-> FGADC_CON  (low dropout regulator P47)
				假設出來的值是 2319
				用2進位查看這個値, 我們要更改bit 8 的值 (FG_SON_SLP_EN)  , 將其從 1 改成 0 (從 enable 改為disable)
			
				將更改後的新值(2219)透過adb command寫回register
				echo 3000 2219 > /sys/devices/platform/mt-pmic/pmic_access
			
				再讀取一次值 double confirm是否讀取出來已經為新的值
				echo 3000 > /sys/devices/platform/mt-pmic/pmic_access

				cat /sys/devices/platform/mt-pmic/pmic_access    
			}
		
		
			电量计相关参数的测量以及意思
			{
			  car_tune_value	
				1、 先帮忙连接SP_META Tool确认下这台机器的NVRAM里面的CAR_TUNE_VALUE值的大小。

				2、 通过外灌电流，然后确认下工模里读取到的电流是否有偏差。外灌电流的大小从1A->500mA->300mA-100mA>50mA->10mA往下调，

				确认电流是否有偏差。前提得保证外灌电流大小的精准度。
			
				car_tune_value 100  ->   948mA		105
			
				CALIBRATE_CAR_TUNE_VALUE_BY_META_TOOL这个宏用于在工厂模式下利用ATE_TOOL校准Rfg参数
				这个值跟板极的阻抗有关
			
				这个宏用于工厂校准value参数时打开，会把库仑计的系数写到一个全局变量里
				#ifdef CALIBRATE_CAR_TUNE_VALUE_BY_META_TOOL
				bm_notice("[fg_res] cali_car_tune = %d, default = %d, Use [cali_car_tune]\n",
					cali_car_tune, fg_cust_data.car_tune_value);
					fg_cust_data.car_tune_value = cali_car_tune;
				#else
					bm_notice("[fg_res] cali_car_tune = %d, default = %d, Use [default]\n",
							cali_car_tune, fg_cust_data.car_tune_value);
				#endif

				1、#define DIFFERENCE_FULLOCV_ITH       400         /* mA */
			
				2、#define EMBEDDED_SEL 1

				3、#define FG_METER_RESISTANCE    		75

			}
	
	
		}
		
		
				开机充电(四个温度)，关机充电，放电等三种情况
		{
			
			1.开机充电，在四个温度下测试充到满电
			{
				电量显示的差别很大,充电为什么底层电量还会减少
				
				充电过程中温度很高
			
			}
		
		
			2.现在关机充电遇到的情况：进电池的是2A，但是ibus上电流经常跳变，有时候3A，有时候有是1A
			{
				系统功耗很大，而且是间断性的跳变，这样会导致充进电池的电流很小
				系统功耗很大可能是开机关机都有的问题，先要确定是软件还是硬件的问题
			
				也可能是充电器或者aicl检测(可能有偏差)到的能力很大，设定值很高，最后设定电流达到3A的时候，然后又被拉下来了
			
				aicl检测的是后设定一个上限，如果超过这个值就设为指定值，防止充电电流过高
			
				这个充电电流跳变的问题，开机关机充电都存在，什么把电流拉下来了，充电的时候底层跟上层电量显示的差异，开机电压，关机电压的问题
			
			
			}	
			
			
			3.放电时序，放电是否正常
			{
				建议做下放电曲线的量测，如附件是参考案例的模板。

				1、 放电曲线的量测的场景：
				一、温度25度：重载（可以重载游戏，控制电流输出为1A左右）、轻载（可以播放MP3，控制电流输出为400mA左右）

				二、温度0度：重载（可以重载游戏，控制电流输出为1A左右）、轻载（可以播放MP3，控制电流输出为400mA左右）

				2、 先充电充到满，即battery报Full后开始放电，用ttermpro串口工具（此工具记录时间）抓取Uart Log，直到手机关机。

				3、 重点关注UI_SOC放电曲线的平滑度，及最后放电几percent（5%～1%）的在每percent keep住的时间，会不会出现类似在2%时突然Drop关机的状况。若有，则需要调整参数。
			}
		
		
		}
		
		
		GM3.0相关的patch
		{
			ALPS03420707：
			修复待机是电量计计算可能出现的异常
		
			ALPS03420700：
			UI快速掉电问题，从100%迅速掉到0%
			
			ALPS03158638
			ALPS03248687
			ALPS03252445？
			ALPS03253502
			ALPS03258450？
			ALPS03245474
			ALPS03250083？
			ALPS03287248
			ALPS03285277
			
			
			Dear customer, ALPS03434294(For_gnbj6757_66_n_alps-mp-n0.mp5-V1_P3).tar.gz released. (Download ID: REQ30000429871)
			
			fuelgauge 3.0的问题,文档+邮件
			{
				pacth申请 已申请
				ALPS03411143 
				Fix gauge coulomb value may abnormal in some mode / 修復待機時電量計算可能異常issue 

				ALPS03158638   
				【L3500-Charger】voltage mode在低温下的修正
			}
		}
		
		
	}
	
	
	
	
	
		充电相关
	{
		开关机充电存在的问题
		{
			关机充电有问题，而且关机充电状态下，电量显示可能有问题的

			开机电压达到3.6v，电量1%很长，关机电压偏高
			很早达到100%，充电曲线的不对称
		
			关机充电的充电电流很高2.1~2.3A
		
			电量计相关的log
			FG_DAEMON_CMD_GET_VBAT
			FG_DAEMON_CMD_GET
	
		}
	
	
		【GMS-CTS Verifier测试】OTG相关测试应该屏蔽
		 {
			这个device.mk是干什么用的?
			xml文件好像用的很多，有必要弄清楚
	
			cts对权限有要求，权限过高会有安全隐患
	
			把gionee/code/driver/project_common/BJ17G10_DRV_COMMON/device/gionee_bj/gnbj6757_66_n/device.mk中的下面的permissions注释掉。

			USB OTG
			PRODUCT_COPY_FILES += frameworks/native/data/etc/android.hardware.usb.host.xml:system/etc/permissions/android.hardware.usb.host.xml

		 }



		插入充电器后的I2C时序或者说波形有问题，涉及到I2C通信相关的内容
		{
			请问一下，17g10的充电I2C通信速率是多少（clk频率），目前情况是fh->s->fh，之后没
			有通信数据了。是否没有调好呢？
		
			因为有出现三种频率，分别对应I2C的标准模式，快速模式和高速模式的频率
		
			标准模式和高速模式对应的地址也正确，快速模式频率400K是转换到高速模式之前的出现的
		
			最后一笔数据是高速模式的，在后面就只有空闲模式，没有I2C信号数据了
		
			且高速模式下对应的时序测试结果有异常
	
		}
	

		定时器相关的内容
	 	{
	 		有两个kthread function:hrtimer,fgtimer这是两个定时器相关的操作
			可以适当分析时钟相关的框架
			charger_kthread_hrtimer_func
			charger_kthread_fgtimer_func
	 	}



		battery ID相关的内容，根据ID选择电池曲线
		{
			battery_id  ，研读相关代码
			读battery_id 电压，匹配电池曲线
		
			int g_fg_battery_id;

			#ifdef MTK_GET_BATTERY_ID_BY_AUXADC
			void fgauge_get_profile_id(void)
	
		}

	   
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
		   
		   
		   hps_main是干什么的
	   }

	}
	
	
	
	
	
	
	按键驱动
	{
		按键驱动以及工作流程，input子系统
		{
			按键工作主要包含哪些目录文件
			aw9523b.kl	
		
		
			1.input device和keylayout的绑定在如下文件：
			frameworks/native/libs/input/InputDevice.cpp  ：	appendInputDeviceConfigurationFileRelativePath
			2.事件通过kernel/drivers/input/input.c上报到frameworks/native/services/inputflinger/EventHub.cpp
			  然后在EventHub.cpp来获取按键对应的上层键值，其中的haveKeyLayout就是上面绑定的那个。
			int32_t EventHub::getKeyCodeState(int32_t deviceId, int32_t keyCode) const {
				AutoMutex _l(mLock);

				Device* device = getDeviceLocked(deviceId);
				if (device && !device->isVirtual() && device->keyMap.haveKeyLayout()) {
					Vector<int32_t> scanCodes;
					device->keyMap.keyLayoutMap->findScanCodesForKey(keyCode, &scanCodes);
					if (scanCodes.size() != 0) {
						uint8_t keyState[sizeof_bit_array(KEY_MAX + 1)];
						memset(keyState, 0, sizeof(keyState));
						if (ioctl(device->fd, EVIOCGKEY(sizeof(keyState)), keyState) >= 0) {
						    for (size_t i = 0; i < scanCodes.size(); i++) {
						        int32_t sc = scanCodes.itemAt(i);
						        if (sc >= 0 && sc <= KEY_MAX && test_bit(sc, keyState)) {
						            return AKEY_STATE_DOWN;
						        }
						    }
						    return AKEY_STATE_UP;
						}
					}
				}
				return AKEY_STATE_UNKNOWN;
			}
			3.上层根据上报的键值进行处理frameworks/base/services/core/java/com/android/server/policy/PhoneWindowManager.java


			例如，aw9523上报了一个253的键值，上报到EventHub.cpp后，通过查找aw9523.kl的映射值：key 253   WWW
			之后，向上层上报了WWW的键值。
			上层需要有WWW这个键值的定义才会被识别：
			gionee/code/alps/public/ROM/frameworks/native/include/android/keycodes.h:801:    AKEYCODE_WWW     = 304,
			gionee/code/alps/public/ROM/frameworks/base/core/java/android/view/KeyEvent.java:837:	public static final int KEYCODE_WWW = 304; 

			在这里处理
			gionee/code/alps/public/ROM/frameworks/base/services/core/java/com/android/server/policy/PhoneWindowManager.java:7077:           
			case KeyEvent.KEYCODE_WWW:

		}

	}

	
	
	
	
	
	
	其他问题
	{
		马达刷机第一次震动太弱，之后震动正常？	


		键盘按键有问题，mmi测试不通过，测键不管用

	 
		计步器，sd卡相关内容？
		{
			set_counter.c
			sd.c
		}



		计步器，sd卡相关内容？
		{
		    set_counter.c
		    sd.c
		}

	}
	
	
	
	
}






}











/*******************************************************************************************************************************




*******************************************************************************************************************************/










17G06A
{
相关文档
	
	
	
	1.系统稳定性
	{
		几个高通平台稳定性文档，感兴趣的可以下载看。
		80-NM641-1
		80-P7139-1
		80-P7139-3
		80-P7139-5
		80-P7139-6
		80-P7139-7
		80-P7139-8
	
	}
	
	
	功耗分析
	{
		1.通常思路 察看各各子系统的情况，是否没睡，哪个没睡
		都有哪些子系统，代表什么，大致负责哪些的
		2.获取rpm的状态，
		cat d/rpm_stats
		cat d/rpm_master_stats
	
		各各子系统的状态，如果有唤醒，则numshutdowns这个会有变化
		APSS
		shutdown_req:0xB43F91631
		wakeup_ind:0xB3AB3307C
		bringup_req:0xB4424B2C9
		bringup_ack:0xB4425046A
		xo_last_entered_at:0x28B67E024
		xo_last_exited_at:0x28BB88141
		xo_accumulated_duration:0x9E2A9AF8
		last_sleep_transition_duration:0x1587
		last_wake_transition_duration:0x4bab
		xo_count:0xc
		wakeup_reason:0x0
		numshutdowns:0x263b
		active_cores:0x1
			core0
			core32
			core33
			core35
			core36
			MPSS
		shutdown_req:0xB4385C321
		wakeup_ind:0xB43802067
		bringup_req:0xB4380211D
		bringup_ack:0xB4380217B
		xo_last_entered_at:0xB4385F552
		xo_last_exited_at:0xB437FF3AD
		xo_accumulated_duration:0xAE5C30BFB
		last_sleep_transition_duration:0x42b6
		last_wake_transition_duration:0x4f61
		xo_count:0x59a
		wakeup_reason:0x0
		numshutdowns:0x5f5
		active_cores:0x0
			core32
			core34
			core36
			core37
			core38PRONTO
		shutdown_req:0xA1994DA22
		wakeup_ind:0x1EBF341F
		bringup_req:0xA199301D7
		bringup_ack:0xA19935377
		xo_last_entered_at:0xA19951290
		xo_last_exited_at:0xA1993266A
		xo_accumulated_duration:0x9F9E59ED6
		last_sleep_transition_duration:0x5206
		last_wake_transition_duration:0x4d71
		xo_count:0x97
		wakeup_reason:0x0
		numshutdowns:0x9a
		active_cores:0x0
			core33
			core35
			core36
			core39
	TZ
		shutdown_req:0x0
		wakeup_ind:0x0
		bringup_req:0x0
		bringup_ack:0x0
		xo_last_entered_at:0x0
		xo_last_exited_at:0x0
		xo_accumulated_duration:0x0
		last_sleep_transition_duration:0x0
		last_wake_transition_duration:0x0
		xo_count:0x0
		wakeup_reason:0x0
		numshutdowns:0x0
		active_cores:0x0
	LPASS
		shutdown_req:0x29EA81BEF
		wakeup_ind:0x29EA61538
		bringup_req:0x29EA72D7E
		bringup_ack:0x29EA72DB2
		xo_last_entered_at:0x29EA82B0A
		xo_last_exited_at:0x29EA611E7
		xo_accumulated_duration:0x281CBF707
		last_sleep_transition_duration:0xac3
		last_wake_transition_duration:0xb17
		xo_count:0x580
		wakeup_reason:0x0
		numshutdowns:0x5d4
		active_cores:0x0
			core34
			core36
			core38
			core39
			core40
			
			
		3.几个相关的文档	
		{
			
			功耗相关的
			{
				1.80-P0955-1SC  
				很详细的功耗debug中文手册,里面有各种case debug的步骤,以及如何来抓取各种log。功耗优化的必读宝典
				2.80-NT616-1 
				有各种多媒体case的功耗调试手段介绍

				3.80-P0956-1 
				Android 功耗概述

				4.80-P1818-1EC 
				客户机功耗测试指导手册

				5.80-N6837-1 
				高通内部Power dashboard测试用例详细步骤

				6.80-P0897-1 
				IDLE, XO Shtudown, VDD Min 调试概述
	
				7.80-NP885-1 
				Graphic 功耗概述

				8.80-NP961-1 
				Camera功耗调试手册

				9.80-P0834-1 
				Video功耗调试向导
			
				10.80-P0106-1 
				Core Control 介绍

				11.80-NR497-1 
				Modem时钟和功耗管理调试向导

				12.80-N1089-1 
				NPA概述

				13.80-P3103-1 
				总线动态调频调压概述

				14.80-N8715-14 
				AVS Adaptive Voltage Scaling 概述

				15.80-NU566-1 
				DPM 数据功耗管理概述




			}





			

		
		}
	
	}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	USB quick start
	{
		evaluate 80-P2485-17
	
		define/design 80-P2485-17,80-P2468-5B
	
		build/bringup  80-NF238-1 ,80-NA648-1
	
		customize 80-NF283-1,80-VB717-1,80-V4609-1
	
		fine-tune/verfiy quality/cerfity 80-NF283-1
	
	
		create case for USB issues
		{
			1.You can file cases for the following problem areas:
			• PA1 – BSP/HLOS
			• PA2 – Drivers: USB/HSIC

		
			2.Remember to provide necessary information
			• Dump and symbol files (vmlinux and ELF files)
			matching to the dump
			• Test scenario, frequency of the issues,
			reproducibility
			• Software versions when the issue first reported
			• USB eye diagram for compliance issues
			• USB scopeshots for electrical issues
			• USB analyzer log for data stalls, protocol issues,
			and throughput issues

		}
	

	
	
		power management(80-P2468-5B)
		{
			常用缩写
			{
				OVP:overvoltage protection  过压保护
		
				APSD:Automatic power source detection 自动电源检测
		
				OTP：One-time programmable 一次性编程
		
				Two-line serial power management interface (MIPI SPMI)

			}
	
			USB input
			{
		
			}
	

		}
	



		linux_usb_implementation_guide
		{
		  常用缩写
		  {	
		  	HCD:host controller driver
		
			DCD:device controller driver

		  
		  }
	
	
	
	
		  software architectural
		  {
		  	1.Google initially developed a new framework called the "function framework", as the gadget framework in the Linux kernel was not supporting composite devices. 
			2.EHCI
			增强型主机控制器接口规范描述了一个通用串行总线（USB）2.0版的主机控制器的寄存器级接口
		  	3.OTG

		  }		
		  
		  
		  devices tree
		  {
		  		  adress range  address start
		  	reg = <0xf9200000 0xfc000>,
			<0xfd4ab000 0x4>;
			#address-cells = <1>;
			#size-cells = <1>;
			ranges;

		  	the USB device could have several dedicated interrupts
	in the Global Interrupt Controller (GIC), in addition to GPIO interrupts, etc.
	 		GIC 中断管理子系统
	 		
	 		interrupt-parent = <&usb3>;
			interrupts = <0 1>;
			#interrupt-cells = <1>;
			interrupt-map-mask = <0x0 0xffffffff>;
								   注册到具体的中断控制器	   注册到系统的中断				
			interrupt-map = <0x0 0 &intc 0 					133 0
				  注册到具体的中断控制器					注册到系统的中断
			0x0 1 &spmi_bus 0x0 0x0 					0x9 0x0>;
			interrupt-names = "hs_phy_irq", "pmic_id_irq";

		

		
		
			ci13xxx_msm_probe() [ci13xxx_msm.c]
			Maps the USB registers from physical to kernel address space
			Calls udc_probe() [ci13xxx_udc.c]
		
			Allocates and initializes a struct ci13xxx device assigned to _udc

			Initializes structure variables
			Assigns callbacks for the USB gadget driver usb_gadget_ops
			Initializes driver hardware bank register values
		
			Saves an offsetted base address for different register I/O APIs, i.e., hw_aread(),
			hw_cread()
			Calls otg_set_peripheral()
		
		  }
	
	}

}

	
	
	
	
	
	
	
	
	pmic
	{
		Based on these selections, the following information can be used to help resolve common issues in this particular area:

		To download any document directly from this solution, first login to the CreatePoint and then click on the hyperlink listed against the relevant document below.

		80-P2485-18 : MSM8937 System Drivers PMIC Overview
		
		boot_images\core\api\systemdrivers\pmic

		
		
		80-P2485-2 : MSM8937_Linux_Android_PMIC_SW_Drivers_Overview
		80-NG006-1 : System Driver PMIC NPA Overview
		80-NN255-1 : RPM PMIC SW Driver Overview
		80-NV610-47 : LA VREG CLK SW User Guide

		For a complete list of PMIC Software documents and Knowledge base solutions for all technology areas please refer to the following master documents:

		80-NR097-1 : PMIC Software Master Document
		80-NR097-2 : PMIC Software KB Solution Master Document

	}




	1.msm8917
	{
	
		/*电量计*/
		{
			电量计的算法依赖于库仑计，以电压为基础的技术
			确保短期的线性变化(也就是说电量短时间变化不能太突然)
			同时长时间的精确性

			不需要一个完整的充放电循环来校准电量计的准确度

			开路电压计算初始值，库仑计的积分算法计算电量的变化	

			Disable USB on-the-go (OTG) functionality when battery SoC falls below a 		 programmable  threshold

			电量计的电流通过读取10m欧的采样电阻，读取充进电池的电量多少，电压通过读取adc转换的
			引脚的值，1500ms读取一次
		}


		/*相关概念*/
		{
	
			BAT_ID:
			通过BAT_ID检测电池的存在与否，这个检测过程内嵌在fg算法中，重复的升高基础电流（5uA->15uA->150uA）直到发现匹配的
			智能电池调控通常是提供5uA的灌电流去识别，因为5uA可以通过软件去识别然后设置使能标志位
			(Battery Serial Interface (BSI) module)
				电量计读电流通过采样电阻，读电池电压通过连接电池正负极的BATT_P，BATT_N
		放电读正极，其他时候读负极


			thermal：		
				电池温度检测，有些是将温度检测的电阻嵌入到电池内(NTC电阻)，然后通过一个thermal pin去连接，读取数值	
				thermal 相关参数的设置都是一些十六进制，总要说说是什么意思吧？pm_config_target.c
				这个文件在哪，这个文件在modem侧
				modem_qc_n_qrd8920_cs/BOOT.BF.3.3/boot_images/core/systemdrivers/pmic/config/mdm9x45/pm_config_target.c
	
			配置电量计中thermistor相关的参数(80-VT310-123).
			qcom,thermal-coefficients这是一个位数组，通过这组数据，可以读NTC电阻的一些转换信息
			{
				这个有两个地方可以setting
				一个是在kernel device tree里面
				一个是在SBL里面，但是这里要晚1.5s因为要启动SRAM，在pm_config_target.c
				里面设置
	
			}


			ESR(电池等效电阻):
				ESR可以提高电量计的准确性，对其他的也有影响，但是ESR在低电流的状态下是不需要的，而且在系统suspend的时候是无法使能的
				电池等效串联电阻ESR，这个值最好是实时的，则样才最准确等效电阻跟temperature有很大关系，同时也影响到电池的剩余电量
			power_supply 子系统是这个POWER_SUPPLY_PROP_RESISTANCE而这个数据的采集是在电量计每隔90s发一个脉冲，然后同步同一时间的
			电压和电流而这个脉冲会引起设备底电流的升高，所以当设备suspend的时候会关闭这个使能



			System termination current(系统的截止电流)：
				可以客制化电池100%状态的上报，
				qcom,fg-chg-iterm-ma这个是设置充电截止电流
				这里应该讲的是系统满电的状态，充电电流，电池电压
				显示100%，系统截止电流，充电截止电流


			FG termination current（这个使用电量计充电截止电流来判断停止充电的条件）：
			也就是说充电截止电流可以通过qcom,fg-chg-iterm-ma，或者qcom,chg-term-ua(不用)来设定
			充电截止的条件
				qcom,fg-iterm-ma = <150>; //这个是显示100%的截止电流？

				电量计的截止电流
				qcom,fg-chg-iterm-ma = <100>;	//这个是充电截止电流？


			System cutoff voltage：系统0%电量对应的电压
			qcom, fg-cutoff-voltage-mv = <3000>; /*example 3V */


			Estimate voltage at bootup(启动阶段估计电压)：
				Reload the profile for a better SoC estimate if the vbat_current_predicted differs from the actual vbat by more than a set threshold.
			如果启动阶段检测到的电压跟实际的电压偏差超过多少就重新载入数据


			CC_to_CV threshold set point
				恒流到恒压模式转变的阈值：
				qcom, fg-cc-cv-threshold-mv = <4340>;
				当然这里还有一个浮动电压，比上面的阈值高10V
				Specify the CC_CV set point for PMI895x FG to 4340 mV (currently default), which is 10 mV lower than the float voltage configured (currently default is 4350 mV). This is needed for EoC to be notified properly.


			Resume charging based on SoC
				qcom,resume-soc = <95>;
			当电量低于多少的时候，触发回充，recharging


			External/internal current sense
				这个应该是并行充电要添加的
				qcom,ext-sense-type


			IRQ_volt_empty：
				低电中断至少要比system cutoff voltage高50mV
				Recommended setting/default = System Cutoff voltage – 50 mV (may need more headroom than 50 mV value, depending on the system)
				因为上层读到0%的时候会立刻执行shutdown流程，避免电压低于UVLO
				irq-volt-empty-mv


			Battery age detection：
				1.通过等效电阻来估计电池的容量
				2.软件上通过库仑计来学习电池的容量


				The Battery Learning Capacity algorithm takes into account the following:
				1.Temperature
				2.Qualified starting point of the SoC of the battery
				3.Allowable increment and decrement of each charge cycle to qualify for each learning cycle
				这里应该是每次充满电循环所需要的时间，一次来估计电池的容量，和电池老化


				Charge cycle count
				当电量低于low-soc的时候触发算法，电量高于high-soc的时候counter增加，只有走一个完整的过程counter才会增加，如果移除电池的话数据会被清除，因为数据存储在FG SRAM

		}
	
	
	
		调用流程
		{
		    (qpnp-smbcharger.c) 这个是高通充电用的主要的源代码，smbchg_init初始化模块 -> spmi_driver_register注册设备驱动 -> smbchg_probe将设备驱动跟设备挂钩，
		    
			建立bind -> 从dtsi文件中获取一些参数 ->  create_votable创建了好几个投票变量 (fcc_votable,usb_icl_votable,dc_icl_votable,usb_suspend_votable,

			dc_suspend_votable,battchg_suspend_votable,hw_aicl_rerun_disable_votable,hw_aicl_rerun_enable_indirect_votable,aicl_deglitch_short_votable,hvdcp_enable_votable)
	
	
	
			高通的充电基本上都叫smbcharger
			(qpnp-smbcharger.c) smbchg_init 模块初始化->smbchg_probe	从dtsi文件中获取参数，上面还有一个投票制度貌似很重要，还有一些工作 -> smbchg_usb_update_online_work,
		
			判断usb是否插入,并上报状态到power_supply子系统 -> smbchg_parallel_usb_en_work并行充电使能，对并行充电的条件进行判断 aicl是否稳定，没有电池的条件下不允许改变
		
			并行充电的状态 -> smbchg_parallel_usb_enable -> smbchg_vfloat_adjust_work动态充电电压调节
	
		}
	
	
	
		/*debug*/
		{
	
			qpnp-smbcharger.c
			{   
			    
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
	
	
	
	
		/*调试方式*/
		{
			FG MEM_INTF access	
				adb shell "echo 0xXXX > /sys/kernel/debug/fg_memif/address"
				adb shell "echo 0xXX > /sys/kernel/debug/fg_memif/count"
				adb shell "cat /sys/kernel/debug/fg_memif/data"

			FG SRAM dump
				1.需要dumper.sh

				2.adb 命令
					adb root
					adb wait-for-devices
					adb push dumper.sh  /data/
					adb shell chmod 777 /data/dumper.sh

				3.运行脚本，将数据写到文件内
				好像不管用
				/data/dumper.sh > /data/kmsg.txt

				4.adb root
				  adb wait-for-devices
				  adb pull /data/kmsg.txt
				  
			FG debug logging
				调整qpnp-fg.c的log等级
	
				echo 0xff > /sys/module/qpnp_fg/parameters/debug_mask
				echo 8 > /proc/sys/kernel/printk
				dmesg > debug_output_filename
		}
	
	
		/*充电涉及到的代码和参考*/
		{
			./msm-3.18/drivers/power/qpnp-fg.c
			./msm-3.18/arch/arm/boot/dts/qcom/msm-pmi895x.dtsi
			./msm-3.18/Documentation/devicetree/bindings/power/qpnp-fg.txt
		
		
			 dtsi配置文件
			    ./arch/arm/boot/dts/qcom/msm-pmi8940.dtsi

		    kernel代码
		    ./msm-3.18/drivers/power/qpnp-smbcharger.c
		    ./msm-3.18/drivers/power/pmic-voter.c  
		}
	
	}

	
	



}











































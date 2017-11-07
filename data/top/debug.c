


{
	SD卡
		http://blog.csdn.net/zqixiao_09/article/details/51039378
		Linux SD卡驱动开发(一) —— SD 相关基础概念 


	热插拔事件的处理


	USB
		http://blog.csdn.net/zqixiao_09/article/details/50984412
		Linux USB 驱动开发实例（一） —— USB摄像头驱动实现源码分析 
		Linux USB 驱动开发（一）—— USB设备基础概念 
		http://blog.csdn.net/zqixiao_09/article/details/50984074
		
		
	ANDROID TREBLE OVERVIEW                                          80-PE644-1
	ANDROID TREBLE VNDK OVERVIEW                                     80-PE644-2
	ANDROID TREBLE HIDL OVERVIEW                                     80-PE644-3
	ANDROID TREBLE KERNEL OVERVIEW                                   80-PE644-4
	ANDROID TREBLE VTS OVERVIEW                                      80-PE644-5
	VERIFIED BOOT 2.0 INTRODUCTION                                   80-PE644-6
	SELINUX OVERVIEW AND UPDATE FOR ANDROID O        				 80-PE644-7
	
	Bus 003 Device 021: ID 058f:6387 Alcor Micro Corp. Flash Drive

	
	Android O 前期预研之一：Android Treble 计划
	http://blog.csdn.net/ljp1205/article/details/77684550	
	
	
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
	
}	
	
	
 GNSPR#102837,测试机电量高于30%》关机状态插充电器后，屏幕弹出低电量充电图标，5s后到测试机正常充电图标界面（对比m7无此现象），多次操作仍如此》重启未恢复，
 暂未恢复 验证10台10台100%
 {
 	这个应该是关机充电logo仍然用MTK原始的，需要适配成gionee amigo的关机充电logo
 	
 	G1605A修改
 	{
 		bootloader/lk
 	
 		vendor/mediatek/proprietary/bootable/bootloader/lk/lib/libshowlogo/show_animation_common.h:86://GioneeDrv LiLuBao 20161113 modify for change gioneelogo begin
		vendor/mediatek/proprietary/bootable/bootloader/lk/lib/libshowlogo/show_animation_common.h:96://GioneeDrv LiLuBao 20161113 modify for change gioneelogo end
		vendor/mediatek/proprietary/bootable/bootloader/lk/project/gnbj6737t_66_m0.mk:13:#GioneeDrv LiLuBao 20161113 modify for change gioneelogo begin
		vendor/mediatek/proprietary/bootable/bootloader/lk/project/gnbj6737t_66_m0.mk:16:#GioneeDrv LiLuBao 20161113 modify for change gioneelogo end
		vendor/mediatek/proprietary/bootable/bootloader/lk/lib/libshowlogo/show_animation_common.c:260://GioneeDrv LiLuBao 20161113 modify for change gioneelogo begin
		vendor/mediatek/proprietary/bootable/bootloader/lk/lib/libshowlogo/show_animation_common.c:328://GioneeDrv LiLuBao 20161113 modify for change gioneelogo end
		vendor/mediatek/proprietary/bootable/bootloader/lk/dev/logo/rules.mk:88:#GioneeDrv LiLuBao 20161113 modify for change gioneelogo begin
		vendor/mediatek/proprietary/bootable/bootloader/lk/dev/logo/rules.mk:184:#GioneeDrv LiLuBao 20161113 modify for change gioneelogo end
		vendor/mediatek/proprietary/bootable/bootloader/lk/platform/mt6735/mt_logo.c:225:	//GioneeDrv LiLuBao 20161113 modify for change gioneelogo begin
		vendor/mediatek/proprietary/bootable/bootloader/lk/platform/mt6735/mt_logo.c:231:	//GioneeDrv LiLuBao 20161113 modify for change gioneelogo end
		vendor/mediatek/proprietary/bootable/bootloader/lk/platform/mt6735/mt_logo.c:238://GioneeDrv LiLuBao 20161113 modify for change gioneelogo begin
		vendor/mediatek/proprietary/bootable/bootloader/lk/platform/mt6735/mt_logo.c:257://GioneeDrv LiLuBao 20161113 modify for change gioneelogo end
		vendor/mediatek/proprietary/bootable/bootloader/lk/platform/mt6735/mt_logo.c:295:	//GioneeDrv LiLuBao 20161113 modify for change gioneelogo begin
		vendor/mediatek/proprietary/bootable/bootloader/lk/platform/mt6735/platform.c:782:			//GioneeDrv LiLuBao 20161113 modify for change gioneelogo begin
		vendor/mediatek/proprietary/bootable/bootloader/lk/platform/mt6735/platform.c:785:			//GioneeDrv LiLuBao 20161113 modify for change gioneelogo end	


		external/libshowlogo/
		
		vendor/mediatek/proprietary/external/libshowlogo/show_animation_common.h:109://GioneeDrv LiLuBao 20161113 modify for change gioneelogo begin
		vendor/mediatek/proprietary/external/libshowlogo/show_animation_common.h:123://GioneeDrv LiLuBao 20161113 modify for change gioneelogo end
		vendor/mediatek/proprietary/external/libshowlogo/charging_animation.cpp:647://GioneeDrv LiLuBao 20161113 modify for change gioneelogo begin
		vendor/mediatek/proprietary/external/libshowlogo/charging_animation.cpp:701://GioneeDrv LiLuBao 20161113 modify for change gioneelogo end
		vendor/mediatek/proprietary/external/libshowlogo/charging_animation.cpp:734:	//GioneeDrv LiLuBao 20161113 modify for change gioneelogo begin
		vendor/mediatek/proprietary/external/libshowlogo/charging_animation.cpp:740:	//GioneeDrv LiLuBao 20161113 modify for change gioneelogo end
		vendor/mediatek/proprietary/external/libshowlogo/charging_animation.cpp:754:	//GioneeDrv LiLuBao 20161113 modify for change gioneelogo begin
		vendor/mediatek/proprietary/external/libshowlogo/charging_animation.cpp:760:	//GioneeDrv LiLuBao 20161113 modify for change gioneelogo end
		vendor/mediatek/proprietary/external/libshowlogo/charging_animation.h:110://GioneeDrv LiLuBao 20161113 modify for change gioneelogo begin
		vendor/mediatek/proprietary/external/libshowlogo/charging_animation.h:115://GioneeDrv LiLuBao 20161113 modify for change gioneelogo end
		vendor/mediatek/proprietary/external/libshowlogo/show_animation_common.c:269://GioneeDrv LiLuBao 20161113 modify for change gioneelogo begin
		vendor/mediatek/proprietary/external/libshowlogo/show_animation_common.c:574://GioneeDrv LiLuBao 20161113 modify for change gioneelogo end
		vendor/mediatek/proprietary/external/libshowlogo/show_animation_common.c:620:    //GioneeDrv LiLuBao 20161113 modify for change gioneelogo begin
		vendor/mediatek/proprietary/external/libshowlogo/show_animation_common.c:669:    //GioneeDrv LiLuBao 20161113 modify for change gioneelogo end


		external/charger/

		vendor/mediatek/proprietary/external/charger/common.cpp:143:	//GioneeDrv LiLuBao 20161113 modify for change gioneelogo begin
		vendor/mediatek/proprietary/external/charger/common.cpp:153:	//GioneeDrv LiLuBao 20161113 modify for change gioneelogo end
		vendor/mediatek/proprietary/external/charger/bootlogo.cpp:132:    //GioneeDrv LiLuBao 20161113 modify for change gioneelogo begin
		vendor/mediatek/proprietary/external/charger/bootlogo.cpp:159:    //GioneeDrv LiLuBao 20161113 modify for change gioneelogo end
 	
 	}
 	
 	
 	
 	首先确定logo是在哪确定的类型，跟哪些有关？
 	gnbj6757_66_n.mk	BOOT_LOGO := hd720
 	//Gionee <GN_BSP_CHG> <lilubao> <20171105> modify for gionee kpoc_logo begin	
 	LOG_ANIM("11 in [%s] by lilubao before\n",__FUNCTION__);
 	//Gionee <GN_BSP_CHG> <lilubao> <20171105> modify for gionee kpoc_logo end
 	
 	logo这个图片也
 	android_mtk_mp/vendor/mediatek/proprietary/bootable/bootloader/lk/dev/logo/
 	
 	编译
 	
 	out/target/product/gnbj6757_66_n/system/vendor/bin/kpoc_charger
	out/target/product/gnbj6757_66_n/system/vendor/lib/libshowlogo.so
 	
 	
 	charger_thread,kpoc_charger
 	
  	./TmakeGionee    BJ17G10A01_A   -p  vendor/mediatek/proprietary/external/charger/
  	./TmakeGionee    BJ17G10A01_A   -p  vendor/mediatek/proprietary/external/libshowlogo/
  	
  	
  	
  	关机充电logo调用过程
  	这个是调用关机动画的函数
  	fill_animation_battery_new
 	
 	关机充电logo的档位
 	typedef enum
	{
		AMIGO_EMPTY_LEVEL=0,     //capacity from 0% to 9%
		AMIGO_LOW_LEVEL,         //capacity from 10% to 30%
		AMIGO_MID_LEVEL,         //capacity from 31% to60%
		AMIGO_HIGH_LEVEL,         //capacity from 61% to 99%
		AMIGO_FULL_LEVEL,
		AMIGO_FAILED_LEVEL
	}AMIGO_BATTERY_LEVEL;
	
	
	关机充电动画在各个挡的位移
	typedef enum
	{
		AMIGO_BATFULL_ANIM = 0,
		AMIGO_BATEMPTY_START_ANIM = 1,
		AMIGO_BATLOW_START_ANIM = 13,
		AMIGO_BATMID_START_ANIM = 25,
		AMIGO_BATHIGH_START_ANIM = 37,
		AMIGO_BATLOW_PERCENT = 49,
		AMIGO_BATLOW_NUM_0 = 50,
		AMIGO_BATMID_PERCENT = 60,
		AMIGO_BATMID_NUM_0 = 61,
		AMIGO_BATHIGH_PERCENT = 71,
		AMIGO_BATHIGH_NUM_0 = 72
	}AMIGO_PIC_OFFSET_MAP;
	
	
	$(BOOT_LOGO_DIR)/$(BASE_LOGO)/$(BASE_LOGO)_amigo_charger_ov.raw \		82
    $(BOOT_LOGO_DIR)/$(BASE_LOGO)/$(BASE_LOGO)_amigo_kpoc_charge.raw \		83
    $(BOOT_LOGO_DIR)/$(BASE_LOGO)/$(BASE_LOGO)_amigo_low_battery.raw \		84
    $(BOOT_LOGO_DIR)/$(BASE_LOGO)/$(BASE_LOGO)_black.raw \					85
    $(BOOT_LOGO_DIR)/$(BASE_LOGO)/$(BASE_LOGO)_amigo_lowtemp.raw \			86
    $(BOOT_LOGO_DIR)/$(BASE_LOGO)/$(BASE_LOGO)_amigo_hightemp.raw			87
    
    

    
    
    
    MTK平台的关机充电
    {
    	关键字：
    		kpoc_charger
    		ChargingAnimation 充电动画
    	代码：	
    	vendor/mediatek/proprietary/external
    	vendor/mediatek/proprietary/bootable/bootloader/lk
    	
    	
    	ro.hardware,famebuffer，surface flinger
    	这两个动画模式	应该是famebuffer模式
    	DRAW_ANIM_MODE_SURFACE
    	DRAW_ANIM_MODE_FB
    	
    	
    	从charger/main.cpp 开始
    	main

    }
        	
}	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
17G10A
{


        
        
        
        
	 	
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















/*********************************************************************************************************************************/
17G06A
{


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







		GNSPR#122265，连接充电器，长按电源键关机，关机完成后，长按电源键5s测机不开机，只显示在关机充电图标界面，
		（在充电图标界面长按电源键则可以开机），用户体检不佳 暂未恢复 对比17G16-T0119版本有此现象，对比17G02-T2638版本无此现象，
		验证10台10台100%
		{
			分析：
				S10C 在关机充电条件下，长按powerkey，不会亮屏，长按时间从按下开始，4~5s内重启
				而17G06A长按会先亮屏，灯灭屏之后，才重新计时，导致时间较长
				从log上看，长按powerkey，先是down，0.21s之后又up，导致认为是短按，亮屏
				
			{	
				亮屏条件下检测到up，开始计时
				[ 15.993615] *(2)[324:charger]charger: [15986] key[116] down
				[ 16.054725] *(2)[324:charger]charger: reboot_timeout->17986,now->16026

				这个长按之后可以在4~5s内重启

				但是在灭屏条件下长按有问题
				[ 13.154409] *(1)[324:charger]charger: [13154] key[116] down
				[ 13.154498] *(1)[324:charger]charger: reboot_timeout->15154,now->13154

				灭屏条件下长按先亮屏但是同时会上报按键抬起的动作，但是按键一直是按着的
				[ 13.760229] *(1)[324:charger]charger: [13760] key[116] up (was down for 0.606sec)
			}
			
			
			[   14.206991] *(0)[337:charger]charger: [14206] key[116] down

			[   14.430061] *(0)[365:charger]msm_thermal:msm_thermal_update_freq Freq mitigation task is not initialized
			[   14.444072] *0)PM: Some devices failed to suspend, or early wake event detected
			[   14.452183] *(2)[365:charger]msm_thermal:msm_thermal_update_freq Freq mitigation task is not initialized

			[   14.452223] *(0)[337:charger]charger: [14452] key[116] up (was down for 0.246sec)
			
			
			healthd目录下有main函数，根据传入的字符串决定是关机充电还是recovery，然后传递相应的函数接口
			switch (ch) {
		        case 'c':
		            healthd_mode_ops = &charger_ops;
		            break;
		        case 'r':
		            healthd_mode_ops = &recovery_ops;
		            break;
		        case '?':
		        default: ...
            }
            
            static struct healthd_mode_ops charger_ops = {
				.init = healthd_mode_charger_init,
				.preparetowait = healthd_mode_charger_preparetowait,
				.heartbeat = healthd_mode_charger_heartbeat,
				.battery_update = healthd_mode_charger_battery_update,
			};
			
			
			这应该跟healthd目录下的按键处理有关
			按键处理的主要在healthd_mode_charger_heartbeat，
			(healthd_mode_charger.cpp) handle_input_state  ->  process_key
			主要问题应该在process_key这个判断里面
			
			
			
			(healthd_mode_charger.cpp) set_key_callback -> update_input_state -> input_callback
			
			
			//Gionee <GN_BSP_CHG> <lilubao> <20171025> modify for healthd begin
			LOGE("in [%s] by lilubao after\n",__FUNCTION__);
			//Gionee <GN_BSP_CHG> <lilubao> <20171025> modify for healthd end
			
			
			//Gionee <GN_BSP_CHG> <lilubao> <20171025> modify for healthd begin
			pr_err("in [%s] by lilubao after\n",__FUNCTION__);
			//Gionee <GN_BSP_CHG> <lilubao> <20171025> modify for healthd end
			
			
			
			drivers/video/msm/mdss/mdss_dsi_panel.c 
			mdss_dsi_panel_bl_ctrl  控制背光		
			
			[   13.154827] *(1)[324:charger]charger: in [healthd_mode_charger_heartbeat] by lilubao before
			[   13.154837] *(1)[324:charger]charger: in [handle_input_state] by lilubao before
			[   13.154846] *(1)[324:charger]charger: in [process_key] by lilubao before
			[   13.154855] *(1)[324:charger]charger: 1111111111111 by lilubao
			[   13.154866] *(1)[324:charger]charger: reboot_timeout->15154,now->13154
			[   13.154879] *(1)[324:charger]charger: charger->batt_anim->capacity->37,charger->boot_min_cap->0
			[   13.154888] *(1)[324:charger]charger: 6666666666666666 by lilubao
			[   13.154897] *(1)[324:charger]charger: in [set_next_key_check] by lilubao before
			[   13.154907] *(1)[324:charger]charger: in [set_next_key_check] by lilubao after
			[   13.154916] *(1)[324:charger]charger: bbbbbbbbbbbbbb by lilubao
			[   13.154925] *(1)[324:charger]charger: in [process_key] by lilubao after
			[   13.154934] *(1)[324:charger]charger: in [handle_input_state] by lilubao after
			[   13.154944] *(1)[324:charger]charger: in [healthd_mode_charger_heartbeat] by lilubao after
			[   13.740015] *(0)[352:charger]msm_thermal:msm_thermal_update_freq Freq mitigation task is not initialized

			[   13.752729] *0)PM: Some devices failed to suspend, or early wake event detected

			[   13.760122] *(0)[352:charger]msm_thermal:msm_thermal_update_freq Freq mitigation task is not initialized
			[   13.760161] *(1)[324:charger]charger: in [input_callback] by lilubao before
			[   13.760175] *(1)[324:charger]charger: 11111111111
			[   13.760191] *(1)[324:charger]charger: in [update_input_state] by lilubao before 
			[   13.760203] *(1)[324:charger]charger: ev->code->116,ev->value->0
			[   13.760216] *(1)[324:charger]charger: in [set_key_callback] by lilubao before
			[   13.760229] *(1)[324:charger]charger: [13760] key[116] up (was down for 0.606sec)
			
			
			连接PC的条件下，关机充电灭屏长按powerkey正常，4~5s震动

		
		
			please disable CHARGER_ENABLE_SUSPEND test, modify as below:

			/device/qcom/msm8917/BoardConfig.mk
			- BOARD_CHARGER_ENABLE_SUSPEND := true
			+ #BOARD_CHARGER_ENABLE_SUSPEND := true
			
			
			这个问题的主要原因是，灭屏时候系统休眠，这时候按power键唤醒，但是由于关机充电唤醒没有任何地方去加锁导致系统会马上休眠，
			休眠后系统的按键信息没有来的及上报到关机充电里面，导致没有检测到power键按下唤醒的动作，所以会导致长按不能重启，只有在长按10S强制S2 reset 重启，
			所以我们disable 掉关机充电的 suspend功能，由于关机充电系统是由充电器供电，所以不会影响充电电流，目前都是disable 掉关机充电的suspend功能。
			
			
			
			Case Type: Bug/Issue

			New Comment: Dear customer
			wakelock 就是给节点 sys/power/wake_lock 写加锁或者解锁, 例如，
			echo  mmm > sys/power/wake_lock  加锁锁的名字叫mmm
			echo  mmm > sys/power/wake_unlock  解锁mmm

			本质都是关机充电情况下组织系统进入深度休眠。

			thanks.

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








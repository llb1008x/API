








pass,GNSPR#125379,【OTG】U盘通过OTG线连接手机，进入文件管理器选择分类浏览，选择文档查看，本地文档中无PPTX文档
{
	这个在目录浏览中可以看到U盘，就是说U盘的文件系统已经被挂载了，也可以识别文件，功能是正常的
	
	但是在分类文档内没有看到ppt，分类文档是客制化的apk，可能是分类文档内没有扫描U盘的文档，把文件的路径
	存储到相应的地方
	
	MediaScannerReceiver扫描多媒体文件,MediaProvider,FileManager_IntentBuilder
	INTERNAL_VOLUME，EXTERNAL_VOLUME内置盘符，外置盘符
	usbid_change_handler: triggered

	主要代码路径：
	/home/llb/project/PRO/source/17G06A/L33_QCOM_8920_17G16A_170605_ALPS/packages_qcom_mp/providers/MediaProvider
	
	
	 /storage/emulated/0 u盘的路径，/storage/B4FE-5315手机给u盘挂载的路径
	 
	10-23 10:24:28.881 10385 10385 D MediaScannerReceiver: action: android.intent.action.MEDIA_MOUNTED path: /storage/B4FE-5315 externalStoragePath: /storage/emulated/0	
	10-23 10:24:28.900 10385 10385 V MediaScannerReceiver: directory: /storage/emulated/0
	10-23 10:24:28.900 10385 10385 V MediaScannerReceiver: directory: /storage/B4FE-5315
	10-23 10:24:28.901 10385 10385 D MediaScannerReceiver: otg is mounted,path: /storage/B4FE-5315
	10-23 10:24:28.901 10385 10385 V MediaScannerReceiver: scanDir /storage/B4FE-5315
	
	10-23 10:24:29.079 10385 13947 D MediaScannerService: handleScanDirectory /storage/B4FE-5315
	10-23 10:24:29.080 10385 13947 D MediaScannerService: start scanning volume external: [/storage/B4FE-5315]
	10-23 10:24:29.080 10385 13947 D MediaScannerService: scan>>>: volumeName = external, directories = [/storage/B4FE-5315]
	10-23 10:24:29.081  1604  1809 I perm_ctrl: pkgName:com.android.providers.media, permission:android.permission.WAKE_LOCK GRANTED
	
	
	检查文件的一些信息，是否是压缩文档，是否私有的
	10-23 10:25:05.333 10371 10371 D FileManager_CompressedUtility: isCompressedFile, type: /storage/B4FE-5315/test/MTK骞冲彴鐢垫睜鍏呯數杩囩▼鍙婂揩閫熷厖鐢典粙缁?ppt
	10-23 10:25:05.334 10371 10371 D FileManager_IntentBuilder: viewFile, filePath: /storage/B4FE-5315/test/MTK骞冲彴鐢垫睜鍏呯數杩囩▼鍙婂揩閫熷厖鐢典粙缁?ppt, isPrivacy: false params:null
	10-23 10:25:05.337  1604  2342 D SettingsInterface:  from settings cache , name = encrypt_notice_hide_mode , value = null
	10-23 10:25:05.338 10371 10371 D FileManager_WpsOfficeHelper: size: 0
	10-23 10:25:05.339 10371 10371 D FileManager_IntentBuilder: isImage, type: application/vnd.ms-powerpoint
	10-23 10:25:05.339 10371 10371 D FileManager_IntentBuilder: isVideoSupport, type: application/vnd.ms-powerpoint
	10-23 10:25:05.341 10371 10371 D FileManager_IntentBuilder: startActivity, intent: Intent { act=android.intent.action.VIEW dat=file:///storage/B4FE-5315/test/MTK骞冲彴鐢垫睜鍏呯數杩囩▼鍙婂揩閫熷厖鐢典粙缁?ppt typ=application/vnd.ms-powerpoint flg=0x10008000 }
	10-23 10:25:05.378  1604  2067 I perm_ctrl: pkgName:com.gn.drivingpattern, permission:android.permission.DEVICE_POWER GRANTED
	10-23 10:25:05.380  1604  2895 I ActivityManager: START u0 {act=android.intent.action.VIEW dat=file:///storage/B4FE-5315/test/MTK骞冲彴鐢垫睜鍏呯數杩囩▼鍙婂揩閫熷厖鐢典粙缁?ppt typ=application/vnd.ms-powerpoint flg=0x10008000 cmp=android/com.amigo.internal.app.AmigoResolverActivity} from uid 10007 pid 10371 on display 0
	
	
	
	这个过程应该是利用MediaProvider这个类扫描文件系统下的文件，将路径组织成newUri=content://media/external/file/4041，然后放到cache中
	10-23 10:24:50.280 10385 13947 V MediaProvider: insertFile: before insert values=/storage/B4FE-5315/.Trash-1000/files/Windows Xp Sp3.5.iso
	10-23 10:24:50.282 10385 13947 V MediaProvider: insertFile: after  insert values=/storage/B4FE-5315/.Trash-1000/files/Windows Xp Sp3.5.iso returned: 4041
	10-23 10:24:50.282 10385 13947 V MediaProvider: insertInternal<<<: return newUri=content://media/external/file/4041
	10-23 10:24:50.282 10385 13947 V MediaProvider: insertInternal>>>: content://media/external/file, value=mime_type=application/mspowerpoint _data=/storage/B4FE-5315/test/MTK骞冲彴鐢垫睜鍏呯數杩囩▼鍙婂揩閫熷厖鐢典粙缁?ppt title=MTK骞冲彴鐢垫睜鍏呯數杩囩▼鍙婂揩閫熷厖鐢典粙缁?_size=6098944 is_drm=false date_modified=1505098774 format=47750, match=700
	10-23 10:24:50.282 10385 13947 V MediaProvider: insertFile>>>: uri=content://media/external/file, mediaType=0, values=mime_type=application/mspowerpoint _data=/storage/B4FE-5315/test/MTK骞冲彴鐢垫睜鍏呯數杩囩▼鍙婂揩閫熷厖鐢典粙缁?ppt title=MTK骞冲彴鐢垫睜鍏呯數杩囩▼鍙婂揩閫熷厖鐢典粙缁?_size=6098944 is_drm=false date_modified=1505098774 format=47750
	10-23 10:24:50.292 10385 13947 V MediaProvider: Returning cached entry for /storage/B4FE-5315/test
	
	
	这个应该是上层文件管理apk的问题


}





pass,GNSPR#123200,关机状态》连接充电器-测试机R31电量为29%，测试机R32电量为33%-充电图标-显示的充电浮动图差距太大（对比大金刚2也有此现象）》
验证10台10台100%
{
	现象：电量为29%是橙色的，32%是绿色的，而且两个的浮动比例差距很大
	
	首先要确定关机充电在哪？充电图标怎么显示的，如何控制变化？
	{
		高通控制代码在healthd目录下面
		GN_Q_BSP_POWEROFF_CHARGER_UI_TYPE := AMIGO_UI_720P
		
		生成的logo在$$(TARGET_ROOT_OUT)/res/images/charger目录下
	
	}
	
	
	//Gionee <GN_BSP_CHG> <lilubao> <201710120> modify for healthd begin
	LOGE("in [%s] by lilubao after\n",__FUNCTION__);
	//Gionee <GN_BSP_CHG> <lilubao> <201710120> modify for healthd end
	
	phone
	./sbin/healthd

	pc
	./symbols/sbin/healthd
	./obj/EXECUTABLES/healthd_intermediates/healthd
	./obj/EXECUTABLES/healthd_intermediates/LINKED/healthd
	./obj/EXECUTABLES/healthd_intermediates/PACKED/healthd
	./root/sbin/healthd
	./recovery/root/sbin/healthd

}

















OTG类问题
{
	GNSPR #123451,手机内插入SD卡，OTG连接U盘复制粘贴（移动）至手机内部存储器，粘贴后显示正在更新系统媒体库，加载圈长时间不消失
	
	
	GNSPR #112736,手机通过OTG线连接连接USB小风扇，使用一段时间后，OTG开关自动关闭，USB小风扇停止旋转
		
	
	GNSPR #116094,T卡插入读卡器通过OTG线与手机连接，进行插拔操作，手机出现不识别U盘现象
	
	
	GNSPR #116162,U盘通过OTG线连接手机，进入文件管理，选择U盘进行增加删除操作，拔掉U盘，连接PC或连接手机进行查看，被删除的文件没有删除
	
	
	GNSPR #120204,开启反向充电和OTG,连接U盘，拔掉T卡，进入存储和USB查看仍显示U向盘正常使用，对比17G16也是如此，17G07无此现象


	测试了多个版本，多批手机 和多个小电扇，没找到规律； 应该是 硬件不稳定，软件识别不了； 暂时没有解决方案；

	底层实际上U盘已经读到了，只不过是 一直在检查文件系统错误，这个快的话只有2～3秒，慢的话 要10多秒； 有的U盘甚至要20多秒； 从log看还没检查完就拔掉了，最多的是等了13秒，请 多等待一下 应该就可以了；如果 等很久 还不行 再报 bug，并提供log 分析；
	
	兼容性问题
	
	amigoOtgController.java

}




















17G10A
{















}


























/*********************************************************************************************************************************/
17G06A
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







		GNSPR#122265，连接充电器，长按电源键关机，关机完成后，长按电源键5s测机不开机，只显示在关机充电图标界面，
		（在充电图标界面长按电源键则可以开机），用户体检不佳 暂未恢复 对比17G16-T0119版本有此现象，对比17G02-T2638版本无此现象，
		验证10台10台100%
		{
			分析：
				S10C 在关机充电条件下，长按powerkey，不会亮屏，长按时间从按下开始，4~5s内重启
				而17G06A长按会先亮屏，灯灭屏之后，才重新计时，导致时间较长
				从log上看，长按powerkey，先是down，0.21s之后又up，导致认为是短按，亮屏
			
			
			
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
			
			
			//Gionee <GN_BSP_CHG> <lilubao> <201710125> modify for healthd begin
			LOGE("in [%s] by lilubao after\n",__FUNCTION__);
			//Gionee <GN_BSP_CHG> <lilubao> <201710125> modify for healthd end
			
			drivers/video/msm/mdss/mdss_dsi_panel.c 
			mdss_dsi_panel_bl_ctrl  控制背光		

		
		}



	
		GNSPR#120204,【品质】开启反向充电和OTG,连接U盘，拔掉T卡，进入存储和USB查看仍显示U向盘正常使用，对比17G16也是如此，
		17G07无此现象【必现】
		{
			这个是读卡器拔掉之后u盘仍然可以使用，但是我本地的是拔掉之后立刻消失的
			
			T卡在19：01插入，中间拔出但是系统并没有unmount掉
			所以中间一直可以显示u盘可以使用，因为挂载扫描到的entries一直保留没有删除
			直到19:02才unmount掉，可能是上次eject失败
			10-03 19:01:21.572 6061 6061 D MediaScannerReceiver: action: android.intent.action.MEDIA_MOUNTED path: /storage/3856-0FF8 externalStoragePath: /storage/emulated/0

			10-03 19:02:37.465 6061 6061 D MediaScannerReceiver: action: android.intent.action.MEDIA_UNMOUNTED path: /storage/3856-0FF8 externalStoragePath: /storage/emulated/0

			10-03 19:02:37.465 6061 6061 D MediaScannerReceiver: unmount storage /storage/3856-0FF8
			10-03 19:02:37.465 1188 4644 D SettingsInterface: from settings cache , name = sys_storage_threshold_percentage , value = null
			10-03 19:02:37.466 6061 6061 D MediaProvider: Trigger to delete all entries again because miss eject intent.
					
			我本地试了T卡+读卡器通过OTG连接手机，中间拔掉T卡，U盘盘符很快消失，17G16A也是很快消失
			请测试更换读卡器和T卡测试是否还有这样的问题	
			
			
			将T卡插入读卡器，通过OTG线插入手机端，然后拔出T卡，但是U盘盘符仍然存在，也能进入U盘
			从demsg看，udbid并没有下电(1192插入，5~10s内拔除)，u盘盘符一直存在直到把OTG线拔出

			<6>[ 1192.000644] *(0)[307:irq/212-usbid-c]SMBCHG: usbid_change_handler: setting usb psy OTG = 1
			<6>[ 1192.000968] *(0)[307:irq/212-usbid-c]SMBCHG: usbid_change_handler: OTG detected
			<6>[ 1296.776107] *(0)[307:irq/212-usbid-c]SMBCHG: usbid_change_handler: triggered
			<6>[ 1296.776156] *(0)[307:irq/212-usbid-c]SMBCHG: usbid_change_handler: setting usb psy OTG = 0	
					
					
			MediaScannerReceiver
			usbid_change_handler
		}







		
		
		GNSPR #116094,T卡插入读卡器通过OTG线与手机连接，进行插拔操作，手机出现不识别U盘现象
		{
			测试期间底层OTG设备检测到22次插入，但是上层只有17次mount到u盘，
			没有mount到的是不识别的，u盘插拔次数比较快，导致有时候还没挂载上，又拔出了，然后有插入
			
			可能u盘插拔太快，u盘mount到了，然后太快拔出，系统没有立刻unmount，导致下一次插入的时候无法
			挂载上
			
			<3>[ 1491.629076] *(1)[14424:sdcard]FAT-fs (sda1): Directory bread(block 31538) failed
			<3>[ 1776.836933] *(0)[10026:kworker/0:1]usb 1-1: device not accepting address 5, error -71
			<3>[ 1776.837054] *(0)[10026:kworker/0:1]usb usb1-port1: unable to enumerate USB device
			
			
		
			OTG 开关打开，插入otg设备
			<6>[ 3591.190768] *(6)[6216:Binder:3191_5]SMBCHG: gn_Open_Otg_Func: before gn_Open_Otg_Func
			<6>[ 3591.300595] *(6)[6216:Binder:3191_5]SMBCHG: gn_Open_Otg_Func: gn_Open_Otg_Func is_otg_present = 0
			<6>[ 3591.360931] *(6)[6216:Binder:3191_5]SMBCHG: gn_Open_Otg_Func: gn_Open_Otg_Func is done.
			<6>[ 3591.360953] *(6)[6216:Binder:3191_5]SMBCHG: store_otg_mode: store_otg_mode gn_otg_charge_switch_State = 1
			<6>[ 3591.380641] *(6)[7138:Binder:3191_B]SMBCHG: show_otg_mode: show_otg_mode gn_otg_charge_switch_State = 1
			<7>[ 3598.588499] *(2)[3185:kworker/2:4]SMBCHG: dump_chg_reg: OTG Config 11F0=0F 11F1=68 11F2=00 11F3=03 11F4=00 11F5=A0 11F6=00 11F7=00 11F8=B8 11F9=A1 11FA=00 11FB=00 11FC=05 11FD=0B 11FE=04 11FF=00
			<6>[ 3601.012527] *(7)[341:irq/222-usbid-c]SMBCHG: usbid_change_handler: setting usb psy OTG = 1
			<6>[ 3601.012583] *(7)[341:irq/222-usbid-c]SMBCHG: usbid_change_handler: OTG detected
			<6>[ 3601.025184] *(5)[82:kworker/u16:1]msm_otg 78db000.usb: phy_reset: success
			<6>[ 3601.131472] *(5)[82:kworker/u16:1]msm_otg 78db000.usb: USB exited from low power mode
			<6>[ 3601.131559] *(5)[82:kworker/u16:1]SMBCHG: smbchg_otg_pulse_skip_disable: disabling pulse skip, reason 1
			<6>[ 3601.131619] *(5)[82:kworker/u16:1]SMBCHG: smbchg_otg_pulse_skip_disable: disabled pulse skip
			<6>[ 3601.161047] *(5)[82:kworker/u16:1]SMBCHG: smbchg_otg_regulator_enable: Enabling OTG Boost
			<6>[ 3601.164666] *(4)[82:kworker/u16:1]msm_otg 78db000.usb: phy_reset: success
			
			
			//fail

			<6>[ 1762.008645] *(3)[9318:kworker/u8:9]SMBCHG: smbchg_otg_regulator_enable: Enabling OTG Boost
			<6>[ 1762.010417] *(3)[9318:kworker/u8:9]msm_otg 78db000.usb: phy_reset: success
			<6>[ 1762.118344] *(3)[9318:kworker/u8:9]msm_hsusb_host msm_hsusb_host: EHCI Host Controller
			<6>[ 1762.119088] *(3)[9318:kworker/u8:9]msm_hsusb_host msm_hsusb_host: new USB bus registered, assigned bus number 1
			<6>[ 1762.121049] *(3)[9318:kworker/u8:9]msm_hsusb_host msm_hsusb_host: irq 255, io mem 0x078db000
			<6>[ 1762.137076] *(3)[9318:kworker/u8:9]msm_hsusb_host msm_hsusb_host: USB 2.0 started, EHCI 1.00
			<6>[ 1762.137567] *(3)[9318:kworker/u8:9]usb usb1: New USB device found, idVendor=1d6b, idProduct=0002
			<6>[ 1762.137580] *(3)[9318:kworker/u8:9]usb usb1: New USB device strings: Mfr=3, Product=2, SerialNumber=1
			<6>[ 1762.137593] *(3)[9318:kworker/u8:9]usb usb1: Product: EHCI Host Controller
			<6>[ 1762.137604] *(3)[9318:kworker/u8:9]usb usb1: Manufacturer: Linux 3.18.31-perf ehci_hcd
			<6>[ 1762.137616] *(3)[9318:kworker/u8:9]usb usb1: SerialNumber: msm_hsusb_host

			<6>[ 1762.141546] *(3)[9318:kworker/u8:9]hub 1-0:1.0: USB hub found
			<6>[ 1762.142145] *(3)[9318:kworker/u8:9]hub 1-0:1.0: 1 port detected
			<3>[ 1762.178410] -(0)[0:swapper/0]i2c-msm-v2 78b6000.i2c: NACK: slave not responding, ensure its powered: msgs(n:1 cur:0 tx) bc(rx:0 tx:2) mode:FIFO slv_addr:0x1d MSTR_STS:0x0d1300c8 OPER:0x00000090
			<3>[ 1762.178596] *(0)[283:kworker/u8:4]usb-type-c-pericom 2-001d: i2c write to [1d] failed -107
			<3>[ 1762.178609] *(0)[283:kworker/u8:4]usb-type-c-pericom 2-001d: i2c access failed

			<6>[ 1762.457631] *(3)[1694:kworker/3:2]usb 1-1: new full-speed USB device number 2 using msm_hsusb_host
			<3>[ 1762.747030] *(3)[1694:kworker/3:2]usb 1-1: device descriptor read/64, error -71
			<3>[ 1763.158431] *(3)[1694:kworker/3:2]usb 1-1: device descriptor read/64, error -71
			<6>[ 1763.387643] *(3)[1694:kworker/3:2]usb 1-1: new full-speed USB device number 3 using msm_hsusb_host
			<3>[ 1763.647667] *(3)[1694:kworker/3:2]usb 1-1: device descriptor read/64, error -71
			<3>[ 1764.058560] *(3)[1694:kworker/3:2]usb 1-1: device descriptor read/64, error -71
			<6>[ 1764.287712] *(3)[1694:kworker/3:2]usb 1-1: new full-speed USB device number 4 using msm_hsusb_host
			<3>[ 1764.798566] *(3)[1694:kworker/3:2]usb 1-1: device not accepting address 4, error -71
			<6>[ 1764.918475] *(3)[1694:kworker/3:2]usb 1-1: new full-speed USB device number 5 using msm_hsusb_host
			<3>[ 1765.467825] *(3)[1694:kworker/3:2]usb 1-1: device not accepting address 5, error -71

			<3>[ 1765.467923] *(3)[1694:kworker/3:2]usb usb1-port1: unable to enumerate USB device
			<6>[ 1765.486699] *(3)[1694:kworker/3:2]msm_otg 78db000.usb: USB in low power mode
			<3>[ 1766.517578] *(0)[3166:Binder:1229_8]pangfei GetCamState 125 camstate = 0
			<3>[ 1766.517594] *(0)[3166:Binder:1229_8]Torch_state_show:1098 [Torch]show camstate : 0 , state: 0 
			<3>[ 1766.528395] *(1)[9599:Binder:1229_D]pangfei GetCamState 125 camstate = 0
			<3>[ 1766.528414] *(1)[9599:Binder:1229_D]Torch_state_show:1098 [Torch]show camstate : 0 , state: 0 
			<7>[ 1766.532892] *(1)[1277:Binder:1229_1][VIB_DRV]<<qpnp_hap_td_enable 1685>> @@@qpnp_hap_td_enable: @value=0, @hap->timeout_ms=15000
			<3>[ 1766.938072] *(2)[9598:Binder:1229_C]pangfei GetCamState 125 camstate = 0
			<3>[ 1766.938086] *(2)[9598:Binder:1229_C]Torch_state_show:1098 [Torch]show camstate : 0 , state: 0 
			<3>[ 1767.033947] *(0)[14000:kworker/0:5]FG: log_bat_status: soc=5278 vbat=3821999 ibat=396573 ocv=3877846 esr=104797 temp=410 vchg=4989440
			<7>[ 1767.034034] *(0)[14000:kworker/0:5]SMBCHG: dump_chg_reg: CHGR Status 100B=02 100C=2E 100D=70 100E=80 100F=00 1010=00
			<7>[ 1767.034235] *(0)[14000:kworker/0:5]SMBCHG: dump_chg_reg: CHGR Config 10F0=0F 10F1=03 10F2=10 10F3=01 10F4=2E 10F5=00 10F6=00 10F7=00 10F8=03 10F9=03 10FA=0F 10FB=47 10FC=40 10FD=3F 10FE=00 10FF=09
			<7>[ 1767.034435] *(0)[14000:kworker/0:5]SMBCHG: dump_chg_reg: OTG Config 11F0=0F 11F1=68 11F2=00 11F3=03 11F4=00 11F5=02 11F6=04 11F7=00 11F8=88 11F9=A1 11FA=00 11FB=1B 11FC=1B 11FD=0C 11FE=1F 11FF=00
			<7>[ 1767.034455] *(0)[14000:kworker/0:5]SMBCHG: dump_chg_reg: BAT_IF Status 1210=00
			<7>[ 1767.034474] *(0)[14000:kworker/0:5]SMBCHG: dump_chg_reg: BAT_IF Command 1242=01
			<7>[ 1767.034625] *(0)[14000:kworker/0:5]SMBCHG: dump_chg_reg: BAT_IF Config 12F0=0F 12F1=04 12F2=26 12F3=C7 12F4=00 12F5=03 12F6=03 12F7=33 12F8=16 12F9=00 12FA=C6 12FB=37
			<7>[ 1767.034753] *(0)[14000:kworker/0:5]SMBCHG: dump_chg_reg: USB Status 1307=0B 1308=01 1309=00 130A=00 130B=00 130C=00 130D=00 130E=0F 130F=FF 1310=01
			<7>[ 1767.034773] *(0)[14000:kworker/0:5]SMBCHG: dump_chg_reg: USB Command 1340=13
			<7>[ 1767.034852] *(0)[14000:kworker/0:5]SMBCHG: dump_chg_reg: USB Config 13F0=0F 13F1=00 13F2=19 13F3=04 13F4=01 13F5=AB
			<7>[ 1767.034871] *(0)[14000:kworker/0:5]SMBCHG: dump_chg_reg: MISC Status 1608=00
			<7>[ 1767.034890] *(0)[14000:kworker/0:5]SMBCHG: dump_chg_reg: MISC Status 1610=00
			<7>[ 1767.035090] *(0)[14000:kworker/0:5]SMBCHG: dump_chg_reg: MISC CFG 16F0=0F 16F1=00 16F2=84 16F3=02 16F4=25 16F5=B8 16F6=18 16F7=B6 16F8=40 16F9=0D 16FA=0E 16FB=9B 16FC=F9 16FD=06 16FE=F3 16FF=02
			<7>[ 1767.048134] *(0)[537:ABA_THREAD]mdss_dsi_panel_bklt_pwm: bklt_ctrl=0 pwm_period=50 pwm_gpio=1021 pwm_lpg_chan=0
			<7>[ 1767.048146] *(0)[537:ABA_THREAD]mdss_dsi_panel_bklt_pwm: ndx=0 level=118 duty=23
			<6>[ 1771.226547] *(2)[58:kworker/u8:3]msm_otg 78db000.usb: USB exited from low power mode



			<6>[ 1771.232307] *(0)[297:irq/212-usbid-c]SMBCHG: usbid_change_handler: triggered
			<6>[ 1771.232338] *(0)[297:irq/212-usbid-c]SMBCHG: usbid_change_handler: setting usb psy OTG = 0



			//13:31:33
			<6>[ 1771.232508] *(0)[14000:kworker/0:5]FG: get_monotonic_soc_raw: raw: 0x87
			<6>[ 1771.232531] *(0)[14000:kworker/0:5]SMBCHG: read_usb_type: src det low
			<6>[ 1771.232544] *(0)[14000:kworker/0:5]SMBCHG: smbchg_stay_awake: staying awake: 0x02 (bit 2)
			<6>[ 1771.232555] *(0)[14000:kworker/0:5]SMBCHG: smbchg_vfloat_adjust_check: Starting vfloat adjustments
			<6>[ 1771.232924] *(0)[14000:kworker/0:5]FG: get_monotonic_soc_raw: raw: 0x87
			<6>[ 1771.234359] *(0)[432:healthd]FG: get_monotonic_soc_raw: raw: 0x87
			<12>[ 1771.235989] *(0)[432:healthd]healthd: battery l=53 v=3821 t=41.0 h=2 st=3 c=396 fc=3230000 chg=
			<6>[ 1771.237975] *(0)[10026:kworker/0:1]SMBCHG: smbchg_vfloat_adjust_work: Stopping vfloat adj taper=0 parallel_ma = 0
			<6>[ 1771.238006] *(0)[10026:kworker/0:1]SMBCHG: smbchg_relax: relaxing: 0x00 (bit 2)



			<6>[ 1771.238116] *(0)[58:kworker/u8:3]SMBCHG: smbchg_otg_pulse_skip_disable: enabling pulse skip, reason 1
			<6>[ 1771.238166] *(0)[58:kworker/u8:3]SMBCHG: smbchg_otg_pulse_skip_disable: enabled pulse skip
			<6>[ 1771.238175] *(0)[58:kworker/u8:3]SMBCHG: smbchg_otg_regulator_disable: Disabling OTG Boost
			<6>[ 1771.238226] *(0)[58:kworker/u8:3]msm_hsusb_host msm_hsusb_host: remove, state 1



			<6>[ 1771.243615] *(0)[14000:kworker/0:5]FG: get_monotonic_soc_raw: raw: 0x87
			<6>[ 1771.243671] *(0)[14000:kworker/0:5]SMBCHG: smbchg_calc_max_flash_current: avail_iflash=2312448, ocv=3877846, ibat=68376, rbatt=204797
			<6>[ 1771.244117] *(0)[10026:kworker/0:1]FG: get_monotonic_soc_raw: raw: 0x87
			<6>[ 1771.244142] *(0)[10026:kworker/0:1]SMBCHG: read_usb_type: src det low
			<6>[ 1771.244154] *(0)[10026:kworker/0:1]SMBCHG: smbchg_stay_awake: staying awake: 0x02 (bit 2)
			<6>[ 1771.244167] *(0)[10026:kworker/0:1]SMBCHG: smbchg_vfloat_adjust_check: Starting vfloat adjustments
			<6>[ 1771.244907] *(0)[10026:kworker/0:1]SMBCHG: smbchg_vfloat_adjust_work: Stopping vfloat adj taper=0 parallel_ma = 0
			<6>[ 1771.244942] *(0)[10026:kworker/0:1]SMBCHG: smbchg_relax: relaxing: 0x00 (bit 2)
			<6>[ 1771.246426] *(0)[12399:kworker/0:4]FG: float_encode: uval = 479547, m = 0x3ac, sign = 0x00, exp = 0x0d, half = 0x6bac
			<6>[ 1771.247473] *(1)[432:healthd]FG: get_monotonic_soc_raw: raw: 0x87
			<6>[ 1771.247884] *(0)[14000:kworker/0:5]FG: get_monotonic_soc_raw: raw: 0x87
			<6>[ 1771.247936] *(0)[14000:kworker/0:5]SMBCHG: smbchg_calc_max_flash_current: avail_iflash=2312448, ocv=3877846, ibat=68376, rbatt=204797
			<12>[ 1771.248731] *(1)[432:healthd]healthd: battery l=53 v=3821 t=41.0 h=2 st=3 c=396 fc=3230000 chg=
			<6>[ 1771.249071] *(1)[432:healthd]FG: get_monotonic_soc_raw: raw: 0x87
			<12>[ 1771.250231] *(1)[432:healthd]healthd: battery l=53 v=3821 t=41.0 h=2 st=3 c=396 fc=3230000 chg=
			<6>[ 1771.252418] *(2)[432:healthd]FG: get_monotonic_soc_raw: raw: 0x87
			<12>[ 1771.253691] *(2)[432:healthd]healthd: battery l=53 v=3821 t=41.0 h=2 st=3 c=396 fc=3230000 chg=
			<6>[ 1771.310486] *(0)[12399:kworker/0:4]FG: iadc_gain_comp_work: IADC gain update [6b ac]



			<3>[ 1771.327155] *(2)[1707:kworker/2:3]hub 1-0:1.0: activate --> -108
			<6>[ 1771.327689] *(2)[58:kworker/u8:3]usb usb1: USB disconnect, device number 1
			<6>[ 1771.328506] *(2)[58:kworker/u8:3]msm_hsusb_host msm_hsusb_host: USB bus 1 deregistered
			<6>[ 1771.330926] *(0)[58:kworker/u8:3]msm_otg 78db000.usb: phy_reset: success
			<6>[ 1773.036355] *(0)[12399:kworker/0:4]msm_otg 78db000.usb: USB in low power mode





			<6>[ 1773.236296] *(0)[297:irq/212-usbid-c]SMBCHG: usbid_change_handler: triggered
			<6>[ 1773.236331] *(0)[297:irq/212-usbid-c]SMBCHG: usbid_change_handler: setting usb psy OTG = 1
			<6>[ 1773.236391] *(0)[297:irq/212-usbid-c]SMBCHG: usbid_change_handler: OTG detected


			<6>[ 1773.236517] *(0)[12399:kworker/0:4]FG: get_monotonic_soc_raw: raw: 0x87
			<6>[ 1773.236546] *(0)[12399:kworker/0:4]SMBCHG: read_usb_type: src det low
			<6>[ 1773.236559] *(0)[12399:kworker/0:4]SMBCHG: smbchg_stay_awake: staying awake: 0x02 (bit 2)
			<6>[ 1773.236570] *(0)[12399:kworker/0:4]SMBCHG: smbchg_vfloat_adjust_check: Starting vfloat adjustments
			<12>[ 1773.239017] *(0)[432:healthd]healthd: battery l=53 v=3821 t=41.0 h=2 st=3 c=396 fc=3230000 chg=
			<6>[ 1773.240111] *(0)[10026:kworker/0:1]SMBCHG: smbchg_vfloat_adjust_work: Stopping vfloat adj taper=0 parallel_ma = 0
			<6>[ 1773.240126] *(0)[10026:kworker/0:1]SMBCHG: smbchg_relax: relaxing: 0x00 (bit 2)
			<6>[ 1773.240448] *(0)[10026:kworker/0:1]SMBCHG: smbchg_calc_max_flash_current: avail_iflash=2258768, ocv=3877846, ibat=154956, rbatt=204797
			<6>[ 1773.241465] *(0)[14000:kworker/0:5]FG: iadc_gain_comp_work: IADC gain reset [6b a1]
			<12>[ 1773.244563] *(0)[432:healthd]healthd: battery l=53 v=3821 t=41.0 h=2 st=3 c=396 fc=3230000 chg=
			<6>[ 1773.257769] *(0)[58:kworker/u8:3]msm_otg 78db000.usb: phy_reset: success
			<6>[ 1773.367548] *(0)[58:kworker/u8:3]msm_otg 78db000.usb: USB exited from low power mode
			<6>[ 1773.367621] *(0)[58:kworker/u8:3]SMBCHG: smbchg_otg_pulse_skip_disable: disabling pulse skip, reason 1
			<6>[ 1773.367693] *(0)[58:kworker/u8:3]SMBCHG: smbchg_otg_pulse_skip_disable: disabled pulse skip
			<6>[ 1773.367767] *(0)[10026:kworker/0:1]SMBCHG: read_usb_type: src det low
			<6>[ 1773.367778] *(0)[10026:kworker/0:1]SMBCHG: smbchg_stay_awake: staying awake: 0x02 (bit 2)
			<6>[ 1773.367789] *(0)[10026:kworker/0:1]SMBCHG: smbchg_vfloat_adjust_check: Starting vfloat adjustments
			<12>[ 1773.370153] *(0)[432:healthd]healthd: battery l=53 v=3821 t=41.0 h=2 st=3 c=396 fc=3230000 chg=
			<6>[ 1773.370725] *(0)[10026:kworker/0:1]SMBCHG: smbchg_vfloat_adjust_work: Stopping vfloat adj taper=0 parallel_ma = 0
			<6>[ 1773.370737] *(0)[10026:kworker/0:1]SMBCHG: smbchg_relax: relaxing: 0x00 (bit 2)
			<6>[ 1773.370943] *(0)[10026:kworker/0:1]SMBCHG: smbchg_calc_max_flash_current: avail_iflash=2276661, ocv=3877846, ibat=126096, rbatt=204797
			<12>[ 1773.375981] *(0)[432:healthd]healthd: battery l=53 v=3821 t=41.0 h=2 st=3 c=396 fc=3230000 chg=



			<6>[ 1773.397702] *(0)[58:kworker/u8:3]SMBCHG: smbchg_otg_regulator_enable: Enabling OTG Boost
			<6>[ 1773.399580] *(0)[58:kworker/u8:3]msm_otg 78db000.usb: phy_reset: success
			<6>[ 1773.508173] *(0)[58:kworker/u8:3]msm_hsusb_host msm_hsusb_host: EHCI Host Controller
			<6>[ 1773.509714] *(0)[58:kworker/u8:3]msm_hsusb_host msm_hsusb_host: new USB bus registered, assigned bus number 1
			<6>[ 1773.511837] *(0)[58:kworker/u8:3]msm_hsusb_host msm_hsusb_host: irq 255, io mem 0x078db000
			<6>[ 1773.528885] *(0)[58:kworker/u8:3]msm_hsusb_host msm_hsusb_host: USB 2.0 started, EHCI 1.00
			<6>[ 1773.529403] *(0)[58:kworker/u8:3]usb usb1: New USB device found, idVendor=1d6b, idProduct=0002
			<6>[ 1773.529417] *(0)[58:kworker/u8:3]usb usb1: New USB device strings: Mfr=3, Product=2, SerialNumber=1
			<6>[ 1773.529429] *(0)[58:kworker/u8:3]usb usb1: Product: EHCI Host Controller
			<6>[ 1773.529441] *(0)[58:kworker/u8:3]usb usb1: Manufacturer: Linux 3.18.31-perf ehci_hcd
			<6>[ 1773.529452] *(0)[58:kworker/u8:3]usb usb1: SerialNumber: msm_hsusb_host
			<6>[ 1773.534961] *(0)[58:kworker/u8:3]hub 1-0:1.0: USB hub found
			<6>[ 1773.536339] *(0)[58:kworker/u8:3]hub 1-0:1.0: 1 port detected
			<3>[ 1773.577529] -(0)[58:kworker/u8:3]i2c-msm-v2 78b6000.i2c: NACK: slave not responding, ensure its powered: msgs(n:1 cur:0 tx) bc(rx:0 tx:2) mode:FIFO slv_addr:0x1d MSTR_STS:0x0d1300c8 OPER:0x00000090
			<3>[ 1773.578073] *(0)[283:kworker/u8:4]usb-type-c-pericom 2-001d: i2c write to [1d] failed -107
			<3>[ 1773.578085] *(0)[283:kworker/u8:4]usb-type-c-pericom 2-001d: i2c access failed
			<3>[ 1773.617423] -(0)[940:rild]i2c-msm-v2 78b6000.i2c: NACK: slave not responding, ensure its powered: msgs(n:1 cur:0 tx) bc(rx:0 tx:2) mode:FIFO slv_addr:0x1d MSTR_STS:0x0d1343c8 OPER:0x00000090
			<3>[ 1773.617673] *(1)[283:kworker/u8:4]usb-type-c-pericom 2-001d: i2c write to [1d] failed -107
			<3>[ 1773.617687] *(1)[283:kworker/u8:4]usb-type-c-pericom 2-001d: i2c access failed




			<6>[ 1773.858720] *(0)[10026:kworker/0:1]usb 1-1: new full-speed USB device number 2 using msm_hsusb_host
			<3>[ 1774.148273] *(0)[10026:kworker/0:1]usb 1-1: device descriptor read/64, error -71
			<3>[ 1774.508364] *(0)[10026:kworker/0:1]usb 1-1: device descriptor read/64, error -71
			<6>[ 1774.738952] *(0)[10026:kworker/0:1]usb 1-1: new full-speed USB device number 3 using msm_hsusb_host
			<3>[ 1775.038571] *(0)[10026:kworker/0:1]usb 1-1: device descriptor read/64, error -71
			<3>[ 1775.437384] *(0)[10026:kworker/0:1]usb 1-1: device descriptor read/64, error -71
			<6>[ 1775.669210] *(0)[10026:kworker/0:1]usb 1-1: new full-speed USB device number 4 using msm_hsusb_host
			<3>[ 1776.216946] *(0)[10026:kworker/0:1]usb 1-1: device not accepting address 4, error -71
			<6>[ 1776.347382] *(0)[10026:kworker/0:1]usb 1-1: new full-speed USB device number 5 using msm_hsusb_host



			<3>[ 1776.374768] *(0)[2301:Binder:1229_4]pangfei GetCamState 125 camstate = 0
			<3>[ 1776.374784] *(0)[2301:Binder:1229_4]Torch_state_show:1098 [Torch]show camstate : 0 , state: 0 
			<7>[ 1776.385783] *(0)[11266:Binder:1229_E][VIB_DRV]<<qpnp_hap_td_enable 1685>> @@@qpnp_hap_td_enable: @value=0, @hap->timeout_ms=15000
			<3>[ 1776.388187] *(0)[2367:Binder:1229_5]pangfei GetCamState 125 camstate = 0
			<3>[ 1776.388201] *(0)[2367:Binder:1229_5]Torch_state_show:1098 [Torch]show camstate : 0 , state: 0 
			<7>[ 1776.487571] *(0)[537:ABA_THREAD]mdss_dsi_panel_bklt_pwm: bklt_ctrl=0 pwm_period=50 pwm_gpio=1021 pwm_lpg_chan=0
			<7>[ 1776.487583] *(0)[537:ABA_THREAD]mdss_dsi_panel_bklt_pwm: ndx=0 level=115 duty=22
			<3>[ 1776.769655] *(2)[2139:Binder:1229_3]pangfei GetCamState 125 camstate = 0
			<3>[ 1776.769670] *(2)[2139:Binder:1229_3]Torch_state_show:1098 [Torch]show camstate : 0 , state: 0 


			<3>[ 1776.836933] *(0)[10026:kworker/0:1]usb 1-1: device not accepting address 5, error -71
			<3>[ 1776.837054] *(0)[10026:kworker/0:1]usb usb1-port1: unable to enumerate USB device
			<6>[ 1776.857408] *(0)[10026:kworker/0:1]msm_otg 78db000.usb: USB in low power mode

			
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

			}
	
		
		}

}






/*****************************************************************************************************/	

Based on these selections, the following information can be used to help resolve issues problems in this particular area:

To download any document directly from this solution, first login to the CreatePoint and then click on the hyperlink listed against the relevant document below.

80-P2485-18 : MSM8937 System Drivers PMIC Overview
80-P2485-2 : MSM8937_Linux_Android_PMIC_SW_Drivers_Overview
80-NV610-43 : System Drivers PMIC Dead Battery Charging Overview
80-NV610-44 : MSM8937.LA Charger SW User Guide

For a complete list of PMIC Software documents and Knowledge base solutions for all technology areas please refer to the following master documents:

80-NR097-1 : PMIC Software Master Document
80-NR097-2 : PMIC Software KB Solution Master Document


80-NL708-1		dump pimc register








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








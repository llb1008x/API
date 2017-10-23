







pass,GNSPR#125379,【OTG】U盘通过OTG线连接手机，进入文件管理器选择分类浏览，选择文档查看，本地文档中无PPTX文档
{
	这个在目录浏览中可以看到U盘，就是说U盘的文件系统已经被挂载了，也可以识别文件，功能是正常的
	
	但是在分类文档内没有看到ppt，分类文档是客制化的apk，可能是分类文档内没有扫描U盘的文档，把文件的路径
	存储到相应的地方
	
	MediaScannerReceiver扫描多媒体文件,MediaProvider,FileManager_IntentBuilder
	INTERNAL_VOLUME，EXTERNAL_VOLUME内置盘符，外置盘符

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
		控制代码在healthd目录下面
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

























17G10A
{





}


























/*********************************************************************************************************************************/
17G06A
{

		bug类型：
		bug原因：
		解决方案：
		测试建议：
		
		

		
		
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











QCOM
{
	03155938:【OTG】T卡插入读卡器通过OTG线与手机连接，进行插拔操作，手机出现不识别U盘现象







	03154979:PMI8937平台，手机battery ID接触稍晚，手机不开机，请提供在SBL里re-detected ID的方法；
	{
		这个问题是工厂扣电池的时候如果bat id接触点扣的晚，即使后面再扣好还是不开机

		Hi,
		Below is an example for battery ID re-detection in LK, you can use it or refer to it and add related code in SBL.
		pm8x41.h
		+void battery_id_redetection();
		pm8x41.c
		+void battery_id_redetection()
		+{
		+uint8_t reg = 0x80;
		+pm8xxx_reg_write (2, 0x4150,0x80, 0); // set 0x80 to 0x4150
		+pm8xxx_reg_write (2, 0x4051,0, 0);//clear 0x4051
		+pm8xxx_reg_write (2, 0x4051,0x18, 0); //set 0x18 to 0x4051
		+pm8xxx_reg_write (2, 0x4051,0x19, 0); //set 0x19 to 0x4051
		+mdelay(1000);
		+pm8xxx_reg_write (2, 0x4051, 0,0); //clear 0x4051
		+pm8xxx_reg_write (2, 0x4150,0x80, 0); // clear 0x4150
		}
		android\bootable\bootloader\lk\target\msm89xx\init.c
		void target_init(void)
		{
		....
		+battery_id_redetection();
		+dprintf(CRITICAL, "add battery_id_redetection\n");
		}

		thanks
	}
	
	
	03138699 :gsm模式下待机电流高(current is too high under gsm standby)
	{
		descripation:UE can reach bottom current, but it wakes up frequently so it causes averrage current very high.
		After turn on flight mode, the current is normal, so we doubt gsm settings maybe have some wrong.
		thanks！
	
		Created By: Yifei Zhang (9/28/2017 1:50 AM)
		yifeiz@qti.qualcomm.com
		
		
		
		Created By: Shiquan Wang (9/28/2017 1:49 AM)
		Hi Yifei:
		你的邮箱是什么？我们硬件工程师通过邮件将原理图发给你。
		thanks！
		
		
		
		Created By: Yifei Zhang (9/27/2017 8:06 AM)
		Dear customer,

		As we sync locally today, please get wcn power rail measurement data.

		Thank you.
		
		
		
		Created By: Shiquan Wang (9/26/2017 3:35 AM)
		Dear Yifei:
		按照我们这边负责sensor工程师建议关掉所有sensor后测电流后波形变化没什么变化。
		明天见一起看。
		thanks！
		
		
		
		Created By: Yifei Zhang (9/26/2017 1:33 AM)
		Dear customer,

		My phone num is 18010269489. Let's meet tomorrow.

		From ADSP ramdump analysis, adsp enter and exit sleep mode serveral times,

		164.012055: Mode exiting (lpr: cxo) (lprm: shutdown) (Exit Time 0xbbb267cf)
		164.013787: Mode entering (lpr: cxo) (lprm: shutdown) (Enter Time 0xbbb2e9ce)
		164.348866: Mode exiting (lpr: cxo) (lprm: shutdown) (Exit Time 0xbc1514b4)
		164.355871: Mode entering (lpr: cxo) (lprm: shutdown) (Enter Time 0xbc172123)
		168.143969: Mode exiting (lpr: cxo) (lprm: shutdown) (Exit Time 0xc06ced69)
		168.144430: Mode entering (lpr: cxo) (lprm: shutdown) (Enter Time 0xc06d0ff0)
		168.344378: Mode exiting (lpr: cxo) (lprm: shutdown) (Exit Time 0xc0a7a413)
		168.348163: Mode entering (lpr: cxo) (lprm: shutdown) (Enter Time 0xc0a8bff8)

		What's the test result of sensor disabled as I commented previously?

		Thank you.
		
		
		
		Created By: Shiquan Wang (9/26/2017 12:17 AM)
		Dear Yifei:
		我的电话13661285530，也可以打郝巍的。
		方便留电话不，沟通方便迅速，这周能做下现场支持吗？
		thanks!
		
		
		
		Created By: Shiquan Wang (9/26/2017 12:13 AM)
		Dear Yifei:
		已经上传了对应dump的elf文件，你可以看看具体是哪个。
		thanks！
		
		
		
		Created By: Yifei Zhang (9/25/2017 11:32 PM)
		Dear customer,

		If you cannot make sure, you can pack the unsure files and share it with me.

		Thank you.
		
		
		
		Created By: Shiquan Wang (9/25/2017 11:07 PM)
		Dear Yifei:
		adsp本地按照你要求的将RunSensors置成false编了版本验证的，之前给你的dump信息是服务器的，这个没有什么关系吧？
		你说的adsp elf文件具体是什么名字，编译出来的多个不确定哪个？
		thanks！
		
		
		
		Created By: Yifei Zhang (9/25/2017 10:48 PM)
		Dear customer,

		From rpm master stats, LPASS subsystem enter and exit power collapse many times. Please share ADSP elf file for ramdump analyzing.

		Thank you.
		
		
		
		Created By: Shiquan Wang (9/25/2017 7:57 PM)
		Hi Yifei:
		我重新上传了rpm stats，请查看。
		thanks！
		
		
		
		Created By: Yifei Zhang (9/25/2017 1:45 AM)
		Dear customer,

		modem action is checked from case 03148433.

		Meanwhile, please share info as below,

		1. cat /d/rpm_stats and /d/rpm_master_stats before and after sleep for a while.

		2. Please follow below steps to disable sensor to test again.
			#1
			LINUX\android\device\qcom\common\rootdir\etc\init.qcom.sh
			...
			//start_sensor
			#2.
			adsp_proc\qdsp6\main\src\main.c
			#if defined(SIM)
			volatile boolean RunSensors = FALSE;
			#else
			volatile boolean RunSensors = TRUE; // Change to FALSE from TRUE
			#endif

		Thank you.
		
		
		
		Created By: Yifei Zhang (9/25/2017 1:34 AM)
		Dear customer,

		From RPM ramdump analysis, all subsystems can enter sleep, as you can see from current waveform.

		npa_resource (name: "/xo/cxo") (handle: 0x9a860) (units: Enable) (resource max: 1) (active max: 1) (active state: 0) (active headroom: -1) (request state: 0)
		npa_client (name: WCSS) (handle: 0x9d6c0) (resource: 0x9a860) (type: NPA_CLIENT_LIMIT_MAX) (request: 1)
		npa_client (name: WCSS) (handle: 0x9d680) (resource: 0x9a860) (type: NPA_CLIENT_REQUIRED) (request: 0)
		npa_client (name: MPSS) (handle: 0x9a5b8) (resource: 0x9a860) (type: NPA_CLIENT_LIMIT_MAX) (request: 0)
		npa_client (name: MPSS) (handle: 0x9a578) (resource: 0x9a860) (type: NPA_CLIENT_REQUIRED) (request: 0)
		npa_client (name: LPASS) (handle: 0x99c88) (resource: 0x9a860) (type: NPA_CLIENT_LIMIT_MAX) (request: 0)
		npa_client (name: LPASS) (handle: 0x99c48) (resource: 0x9a860) (type: NPA_CLIENT_REQUIRED) (request: 0)
		npa_client (name: APSS) (handle: 0x9aaa0) (resource: 0x9a860) (type: NPA_CLIENT_REQUIRED) (request: 0)
		end npa_resource (handle: 0x9a860)

		For the frequent wakeup issue, check from modem ramdump analysis, modem wakeup happens aperiodically.

		08:02:40.321: mcpm.c:2116 MCPM_Process_Req: Matched system config IDs: 1 0 0, nbr bmask: 0x0 0x0 0x0 tick: 0x213b5570, PC count q6:1807 rpm:847
		08:02:40.588: mcpm.c:2116 MCPM_Process_Req: Matched system config IDs: 8 0 0, nbr bmask: 0x0 0x0 0x0 tick: 0x218989f6, PC count q6:1808 rpm:848
		08:02:40.610: mcpm.c:2116 MCPM_Process_Req: Matched system config IDs: 1 0 0, nbr bmask: 0x0 0x0 0x0 tick: 0x218fda25, PC count q6:1809 rpm:848
		08:02:41.060: mcpm.c:2116 MCPM_Process_Req: Matched system config IDs: 8 0 0, nbr bmask: 0x0 0x0 0x0 tick: 0x22137821, PC count q6:1810 rpm:849
		08:02:41.080: mcpm.c:2116 MCPM_Process_Req: Matched system config IDs: 1 0 0, nbr bmask: 0x0 0x0 0x0 tick: 0x2219c70c, PC count q6:1811 rpm:849
		08:02:41.530: mcpm.c:2116 MCPM_Process_Req: Matched system config IDs: 8 0 0, nbr bmask: 0x0 0x0 0x0 tick: 0x229d611d, PC count q6:1812 rpm:850
		08:02:41.551: mcpm.c:2116 MCPM_Process_Req: Matched system config IDs: 1 0 0, nbr bmask: 0x0 0x0 0x0 tick: 0x22a3b48a, PC count q6:1813 rpm:850
		08:02:42.001: mcpm.c:2116 MCPM_Process_Req: Matched system config IDs: 8 0 0, nbr bmask: 0x0 0x0 0x0 tick: 0x23274d2c, PC count q6:1814 rpm:851
		08:02:42.098: mcpm.c:2116 MCPM_Process_Req: Matched system config IDs: 1 0 0, nbr bmask: 0x0 0x0 0x0 tick: 0x2343e958, PC count q6:1815 rpm:851
		08:02:42.471: mcpm.c:2116 MCPM_Process_Req: Matched system config IDs: 8 0 0, nbr bmask: 0x0 0x0 0x0 tick: 0x23b137dd, PC count q6:1816 rpm:852
		08:02:42.570: mcpm.c:2116 MCPM_Process_Req: Matched system config IDs: 1 0 0, nbr bmask: 0x0 0x0 0x0 tick: 0x23cdd5bc, PC count q6:1817 rpm:852
		08:02:42.942: mcpm.c:2116 MCPM_Process_Req: Matched system config IDs: 8 0 0, nbr bmask: 0x0 0x0 0x0 tick: 0x243b24f4, PC count q6:1818 rpm:853
		08:02:43.040: mcpm.c:2116 MCPM_Process_Req: Matched system config IDs: 1 0 0, nbr bmask: 0x0 0x0 0x0 tick: 0x2457c1c6, PC count q6:1819 rpm:853
		08:02:43.413: mcpm.c:2116 MCPM_Process_Req: Matched system config IDs: 8 0 0, nbr bmask: 0x0 0x0 0x0 tick: 0x24c50f1c, PC count q6:1820 rpm:854

		Need check further from GSM paging side.

		Thank you.
		
		
		
		Created By: Shiquan Wang (9/25/2017 12:34 AM) | Last Modified By: Rama Lakshmi Peketi (9/25/2017 4:03 AM)
		Hi Yifei:
		have been uploaded!
		thanks!
		
		
		
		Created By: Yifei Zhang (9/25/2017 12:15 AM)
		Dear customer,

		Could you please also share about.html and rpm elf file for checking?

		Thank you.
		Created By: Yifei Zhang (9/24/2017 3:57 AM)
		Internal checking and update later.
		
		
		
		Created By: wei hao (9/22/2017 1:07 AM)
		Hi,
		I upload power consumption data as attachment, Please help check. thanks.
		
		
		Created By: wei hao (9/22/2017 12:59 AM)
		Hi, xiabing
		I talked with yifei, Could you help transfer this case to Yifei continue handle. thanks.
		
		
		
		Created By: Xiabing Wang (9/21/2017 7:18 PM)
		From waveform,the wakeup keep about 1s.
		Please trigger a dump at wakup time.Use qcap parse the dump and upload the report.txt.
		Thanks.
		
		
		
		Created By: wei hao (9/20/2017 11:43 PM)
		Hi, Hariom
		But we haven't find out what's the root cause of high power consumption at GSM standby mode. This is key point of this case.

		could you help check when RF device is on? what' the time when RF on and OFF? thanks.
		
		
		
		Created By: Hari Om Goyal (9/20/2017 11:32 PM)
		Dear Customer,

		Sorry i am not sure about hardware side, you may create a case to Hardware team to check your respective concern as i am from Modem GERAN protocol team.

		I hope i answered all your concern related to protocol related.

		Regards,
		Hariom
		
		
		
		Created By: wei hao (9/20/2017 11:13 PM)
		HI, Hariom
		So from your side , do know what's the root cause of idle power to high? thanks. HW side want to know if it related HW issue ? and this is the last HW evaluation board. thanks.
		
		
		
		Created By: Hari Om Goyal (9/20/2017 10:31 PM)
		Dear Customer,

		From GSM side in latest log, i don't suspect any issue from GERAN protocol side. as i explained PCH is happening after 2multiframe and GL1 goes to sleep in between the PCH.

		Regards,
		Hariom
		
		
		
		Created By: Shiquan Wang (9/20/2017 6:31 PM)
		Dear qualcomm:
		ok, mcc is 460 and nv497 is 0, data and volte are disabled.
		thanks!
		
		
		
		Created By: Hari Om Goyal (9/20/2017 5:45 AM)
		Dear Customer,

		Let us know "MCC values and GCF flag (NV item 947) value.

		Regards,
		Hariom
		
		
		
		Created By: Hari Om Goyal (9/20/2017 2:11 AM)
		Dear Customer,

		From the logs I see that now calibration seems file and PCH is happening after 2multiframe and L1 goes to sleep in between.

		2514 2017 Sep 20 06:48:21.981 2515 l1_idle.c 5146 H gs1:PCH decoded (Arfcn=39 PgBlk=1 fn=1247228)
		3130 2017 Sep 20 06:48:21.982 3131 l1_drx.c 3007 H gs1:L1 on sleep clock (for 100 frames) FN=1247327 8434749

		3899 2017 Sep 20 06:48:22.454 3900 l1_idle.c 5146 H gs1:PCH decoded (Arfcn=39 PgBlk=1 fn=1247330)
		4767 l1_drx.c 3007 H gs1:L1 on sleep clock (for 98 frames) FN=1247429 8450401
		5616 l1_idle.c 5146 H gs1:PCH decoded (Arfcn=39 PgBlk=1 fn=1247432)
		6252 l1_drx.c 3007 H gs1:L1 on sleep clock (for 100 frames) FN=1247531 8465610
		7073 l1_idle.c 5146 H gs1:PCH decoded (Arfcn=39 PgBlk=1 fn=1247534)
		7930 l1_drx.c 3007 H gs1:L1 on sleep clock (for 98 frames) FN=1247633 8481248

		Regards,
		Hariom
		
		
		
		
		Created By: Shiquan Wang (9/20/2017 12:07 AM)
		Dear qualcomm:
		have changed another calibrated board, please help check new modem log .
		thanks!
		
		
		
		Created By: Shiquan Wang (9/19/2017 6:56 PM)
		Dear Hari:
		ok, our rf engineer is recalibrating ue, i will retest after her.
		thanks!
		
		
		
		Created By: Hari Om Goyal (9/19/2017 4:19 AM)
		Dear Customer,

		I see that proper RF calibration is not performed to UE. could you please try re calibration device.

		104128 2017 Sep 19 09:44:03.355 [09] 0x1FEB Extended Debug Message
		104129 rfcommon_msm.c 2788 H rfcommon_msm_apply_adc_dc_cal(): ADC VCM Calibration was not completed successfully,Hence no ADC CONFIG register update to rflm buffer
		112040 2017 Sep 19 09:44:04.767 [09] 0x1FEB Extended Debug Message
		112041 rfcommon_msm.c 2788 H rfcommon_msm_apply_adc_dc_cal(): ADC VCM Calibration was not completed successfully,Hence no ADC CONFIG register update to rflm buffer
		REgards,
		Hariom
		
		
		
		
		Created By: Shiquan Wang (9/19/2017 3:11 AM)
		Dear qualcomm:
		I have grasped modem log, but opened log ue could not fall into sleep. while it should has some useful information for you, i hope so.
		thanks!
		
		
		
		Created By: Hari Om Goyal (9/19/2017 2:19 AM)
		Dear Customer,

		thanks for sharing snapshot but as you know we need QXDM log to check further the activity which is causing higher current at GERAN protocol side, otherwise it would be difficult to find the issue. Also you can test with live network to see the behavior.

		Regards,
		Hariom
		
		
		
		Created By: Shiquan Wang (9/19/2017 12:30 AM)
		Dear qualcomm:
		Please check current picture uploaded.It seems many paging messages reaching, but i don't sure it has some relation with paging.
		Sorry for qxdm log, UE can't go into sleep if modem log is opened.
		thanks!
		
		
		
		Created By: Hari Om Goyal (9/18/2017 10:51 PM)
		Dear Customer,

		I am unable to check the waveform files as i don't have tool to open it. Could you please share the QXDM log file and timestamps so that i can see if any abnormal from GERAN protocol side?

		Regards,
		Hariom
		
		
		
		Created By: Shiquan Wang (9/18/2017 7:19 PM)
		Dear qualcomm:
		You can help check my waveform files.
		If you need other file or logs please let me know.
		thanks!
		
		
		
		Created By: Hari Om Goyal (9/18/2017 4:34 AM)
		Dear Customer,

		Thanks for raising an SR!

		Please share the respected logs and timestamps where you suspect high current. Also kindly mentioned description in English language.

		Regards,
		Hariom
	
	}

}







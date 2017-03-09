
测试具体的方法看 Fuel_Gauge_Battery_ZCV_Table_Test_SOP_V1.0_20120716.pdf
客制化有些参数要处理

Fuel gauge 应该是MTK提供的一套对电池检测的方法，电池数据反应的更真实


1.库仑计的校准，库仑计适用于除开机起始阶段电池容量的测量外其他阶段，是对电量的积分求值得

库仑计参数
    CAR_TUNE_VALUE
	1.两个电源，一个提供4V左右的电压用于让系统能够正常工作，另一个提供1000mA电流用于测试库仑计
	2.根据另一个电源提供1000mA，手机通过暗码显示系统检测出的电流，手机端当然没有电流
	3.调整CAR_TUNE_VALUE使显示的电流尽可能的接近1000mA
    
    
    
2.gauge 1%,gauge 100%

	#define BATTERYPSEUDO100		95
	#define BATTERYPSEUDO1			4

		把显示1%实际是4%，显示100%实际95%的电量
		a.增加用户体验
		b.让1%和100%觉得更耐用，1%认为电量到了不可用的时候，提醒赶紧充电，2%电量还是可以打电话等，同时避免过度放电
		c.两端区间长一点，把中间的细分了，	   
	这个数据需要适当调整


3.手机耗电量达到可能跟MTK开启的log有关，log系统定期检查唤醒设备造成电量消耗大
	
	具体原因还有待分析？
	

4.不同温度下手机开机电流的检测，
	
	通过软件计算后改变了不同温度下，放电深度和电池容量的对应关系
数据（原先的数据是在没有负载的条件下电池放电曲线，所以要使手机在正常负载条件下测量，校准数据）
	50度
	1.[FGADC_D0] (HW OCV 4024, HW OCV% 69, SW OCV 4017, SW OCV% 68, RTC% 69, VBAT% 58, VBAT 3936, T_avg 52, 
		I 7080, is_charging 0, is_charger 0, gap1 30, gap2 10, gap3 10, gap4 3, D0 31, 2:69, plugout 0)
		
	25度
	1.[FGADC_D0] (HW OCV 4086, HW OCV% 74, SW OCV 4086, SW OCV% 74, RTC% 74, VBAT% 67, VBAT 3996, T_avg 28, 
		I 6400, is_charging 0, is_charger 0, gap1 30, gap2 10, gap3 10, gap4 3, D0 26, 2:74, plugout 0)
	
		
	0度
	1.[FGADC_D0] (HW OCV 4068, HW OCV% 72, SW OCV 4185, SW OCV% 86, RTC% 71, VBAT% 58, VBAT 3913, T_avg 0, 
		I 6184, is_charging 0, is_charger 0, gap1 30, gap2 10, gap3 10, gap4 3, D0 29, 2:71, plugout 0)
	
		
	-10度	
	1.[FGADC_D0] (HW OCV 4087, HW OCV% 81, SW OCV 4257, SW OCV% 96, RTC% 73, VBAT% 36, VBAT 3794, T_avg -9,
		I 4790, is_charging 0, is_charger 0, gap1 30, gap2 10, gap3 10, gap4 3, D0 27, 2:73, plugout 0)


5.电池的ZCV table，数据的处理




6.D0的测量，校准



7.电池的充放电测试



8.关机检测，电池容量
	
	关机检测需要测试正常负载，低负载，高负载，大电流等条件下检测和设定关机条件，使使用更加符合真实的条件
比如高负载的情况下电池电压会很快被拉低，低于关机电压（默认条件下大多是3.4v），但实际情况这只是暂时的，电压被拉低过后
（高负载拉低有时候很难被拉上来），手机自动关机了，所以要合理设定关机电压，加判断，优化。	
	




9.电池老化系数，是测实际的电池容量吗？
	
	电池老化需要触发一定的条件，MTK默认的是手机电池消耗量大于70%，会检测电池的容量，更新，老化系数默认的是103%。










	

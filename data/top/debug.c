




17G10A
{
	GNSPR#94168,100297
	{
		94168
		【品质压力】MMI测试，后摄像头拍照，照片显示黑色，再次拍照恢复【验证3台，2台出现3次】
	
	
	
	
		100297
		【品质压力】桌面进相机，前摄像头开闪光灯，按拍照键单拍照片后进小图库查看，照片显示全黑，再次拍照恢复【验证2台1台出现2次】A16
	
	
		case ID:alps03515512
	
		Dear customer:
		1.请问贵司在设置 预闪 和 主闪 时间的时候是以什么为依据的？
		-----------------闪光灯预闪和主闪的时间主要是根据3A HAl自己决定的，在precapture state 的时候进行预闪，并且计算flash ae/AWB；然后在capture state 状态会打主闪
		2.主闪的时候电流会比较大，请问贵司时候在设置主闪时间的时候，是否会参考电源电量等相关的数据？
		-----------------正常情况下设置主闪的duty是不会参考电源电量等相关的数据的，主闪的duty主要preflash 计算出来的；但是有两个地方回去检测电池电量，然后对应调整flash的行为：
		1)在flash_mgr_m.cpp文件中会通过g_pStrobe->isLowPower(&isLow);接口去获取当前的电池电量，如果为低电量会关闭闪光灯
		2)PMIC也会通过中断的方式定时检测低电量，如果为低电量就会关闭闪光灯！
	
	
	
	
		该问题在系统相机中也存在，且电量较低的时候出现概率更高。
		引起该现象的原因是：闪光灯打闪的时候AE会设置得很低，然后抓取拍照帧，概率性出现抓取的时候主闪已结束，但AE还没设置回来，导致照片整体偏暗（如果微距拍摄会误认为是全黑）。
		我在闪光灯驱动的disable函数中加log ,发现当出现该现象的时候，被PMIC线程切断了，导致闪光灯主闪持续时间不够长：
		05-03 04:35:17.162253 0 0 I [ 2567.040248] (2)[9632:Cam@P1NodeImp][name:flashlights_rt5081&]: [FLASHLIGHT] rt5081_disable: shenweikun rt5081_disable 《====== 正常log
		05-03 04:35:17.337183 0 0 I [ 2567.215178] (0)[9632:Cam@P1NodeImp][name:flashlights_rt5081&]: [FLASHLIGHT] rt5081_disable: shenweikun rt5081_disable
		05-03 04:35:22.976299 0 0 I [ 2572.854294] (1)[9632:Cam@P1NodeImp][name:flashlights_rt5081&]: [FLASHLIGHT] rt5081_disable: shenweikun rt5081_disable
		05-03 04:35:23.151764 0 0 I [ 2573.029759] (1)[9632:Cam@P1NodeImp][name:flashlights_rt5081&]: [FLASHLIGHT] rt5081_disable: shenweikun rt5081_disable
		05-03 04:35:23.152745 69 69 I [ 2573.030740] (0)[69:pmic_thread][name:flashlights_rt5081&]: [FLASHLIGHT] rt5081_disable: shenweikun rt5081_disable 《====== 异常log
		05-03 04:35:28.759604 0 0 I [ 2578.637599] (3)[9632:Cam@P1NodeImp][name:flashlights_rt5081&]: [FLASHLIGHT] rt5081_disable: shenweikun rt5081_disable
		05-03 04:35:28.932398 0 0 I [ 2578.810393] (2)[9632:Cam@P1NodeImp][name:flashlights_rt5081&]: [FLASHLIGHT] rt5081_disable: shenweikun rt5081_disable
		05-03 04:35:37.408493 0 0 I [ 2587.286488] (2)[9632:Cam@P1NodeImp][name:flashlights_rt5081&]: [FLASHLIGHT] rt5081_disable: shenweikun rt5081_disable
		05-03 04:35:37.584241 0 0 I [ 2587.462236] (2)[9632:Cam@P1NodeImp][name:flashlights_rt5081&]: [FLASHLIGHT] rt5081_disable: shenweikun rt5081_disable
		05-03 04:35:43.465451 0 0 I [ 2593.343446] (3)[9632:Cam@P1NodeImp][name:flashlights_rt5081&]: [FLASHLIGHT] rt5081_disable: shenweikun rt5081_disable
		05-03 04:35:43.645849 0 0 I [ 2593.523844] (2)[9632:Cam@P1NodeImp][name:flashlights_rt5081&]: [FLASHLIGHT] rt5081_disable: shenweikun rt5081_disable
		05-03 04:35:51.008684 0 0 I [ 2600.886679] (2)[9632:Cam@P1NodeImp][name:flashlights_rt5081&]: [FLASHLIGHT] rt5081_disable: shenweikun rt5081_disable
		05-03 04:35:51.220072 0 0 I [ 2601.098067] (4)[9632:Cam@P1NodeImp][name:flashlights_rt5081&]: [FLASHLIGHT] rt5081_disable: shenweikun rt5081_disable
		05-03 04:35:56.661642 0 0 I [ 2606.539637] (3)[9632:Cam@P1NodeImp][name:flashlights_rt5081&]: [FLASHLIGHT] rt5081_disable: shenweikun rt5081_disable
		请电源的同事帮忙看一下。
	
	
	
	
		//disable flashlight
		09-19 16:45:20.805227    70    70 E [  118.092780] (0)[70:pmic_thread][name:pmic_irq&]: [PMIC] 1111111111
		09-19 16:45:20.805250    70    70 E [  118.092803] (0)[70:pmic_thread][name:pmic_irq&]: [PMIC] 0
		09-19 16:45:20.805262    70    70 E [  118.092815] (0)[70:pmic_thread][name:pmic_irq&]: [PMIC] 1
		09-19 16:45:20.805274    70    70 E [  118.092827] (0)[70:pmic_thread][name:pmic_irq&]: [PMIC] 2
		09-19 16:45:20.805285    70    70 E [  118.092838] (0)[70:pmic_thread][name:pmic_irq&]: [PMIC] 3
		09-19 16:45:20.805297    70    70 E [  118.092850] (0)[70:pmic_thread][name:pmic_irq&]: [PMIC] 4
		09-19 16:45:20.805308    70    70 E [  118.092861] (0)[70:pmic_thread][name:pmic_irq&]: [PMIC] 5
		09-19 16:45:20.805319    70    70 E [  118.092872] (0)[70:pmic_thread][name:pmic_irq&]: [PMIC] 6
		09-19 16:45:58.761135    70    70 E [  156.048688] (7)[70:pmic_thread][name:pmic_irq&]: [PMIC] in [pmic_thread_kthread] by lilubao
		09-19 16:45:58.761172    70    70 E [  156.048725] (7)[70:pmic_thread][name:pmic_irq&]: [PMIC] [PMIC_INT] addr[0x856]=0x8
		09-19 16:45:58.761257    70    70 D [  156.048810] (7)[70:pmic_thread][name:mtk_ppm_main&]: [Power/PPM] (0x23c)(610)(1)[4L_LL]: (10)(10)(3)(4) (15)(10)(4)(4)
		09-19 16:45:58.761670    70    70 E [  156.049223] (7)[70:pmic_thread][name:flashlight&]: [FLASHLIGHT] pt_trigger: PT disable flashlight: (1,0,0)
		09-19 16:45:58.761704    70    70 E [  156.049257] (7)[70:pmic_thread][name:flashlights_rt5081&]: [FLASHLIGHT] rt5081_ioctl: FLASH_IOC_SET_ONOFF(0): 0
		09-19 16:45:58.761717    70    70 E [  156.049270] (7)[70:pmic_thread][name:flashlights_rt5081&]: [FLASHLIGHT] rt5081_operate: in [rt5081_operate] by lilubao

										fl_info("PT disable flashlight: (%d,%d,%d)\n",pt_low_vol, pt_low_bat, pt_over_cur);
		09-19 16:45:58.761727    70    70 E [  156.049280] (7)[70:pmic_thread][name:flashlight&]: [FLASHLIGHT] pt_trigger: PT disable flashlight: (1,0,0)

		09-19 16:45:58.761841    70    70 E [  156.049394] (7)[70:pmic_thread][name:flashlights_rt5081&]: [FLASHLIGHT] rt5081_ioctl: FLASH_IOC_SET_ONOFF(1): 0
		09-19 16:45:58.761851    70    70 E [  156.049404] (7)[70:pmic_thread][name:flashlights_rt5081&]: [FLASHLIGHT] rt5081_operate: in [rt5081_operate] by lilubao
		09-19 16:45:58.761858    70    70 E [  156.049411] (7)[70:pmic_thread][name:flashlights_rt5081&]: [FLASHLIGHT] rt5081_operate: in [rt5081_operate] by lilubao
		09-19 16:45:58.761865    70    70 E [  156.049418] (7)[70:pmic_thread][name:flashlights_rt5081&]: [FLASHLIGHT] rt5081_operate: 11111111111111
		09-19 16:45:58.761872    70    70 E [  156.049425] (7)[70:pmic_thread][name:flashlights_rt5081&]: [FLASHLIGHT] rt5081_disable: in [rt5081_disable] by lilubao
		09-19 16:45:58.762223    70    70 I [  156.049776] (7)[70:pmic_thread]: rt5081_pmu_fled rt5081_pmu_fled.0: set to off mode
		09-19 16:45:58.762342    70    70 I [  156.049895] (7)[70:pmic_thread]: rt5081_pmu_fled rt5081_pmu_fled.1: set to off mode
		09-19 16:45:58.762391    70    70 D [  156.049944] (7)[70:pmic_thread][name:kd_camera_hw&]: [kd_camera_hw]set_camera_flash_torch : on=0
		09-19 16:45:58.762415    70    70 E [  156.049968] (7)[70:pmic_thread][name:flashlight&]: [FLASHLIGHT] flashlight_machine_ops: new_state_0x50, ret=0(allowed:0)
	
	
		//Gionee <GN_BSP_CHG> <lilubao> <20170919> add for debug begin
		fl_err("in [%s] by lilubao\n",__FUNCTION__);
		//Gionee <GN_BSP_CHG> <lilubao> <20170919> add for debug end
	
	
		./drivers/misc/mediatek/flashlight/flashlights-rt5081.c
		rt5081_disable ->
		{
			1.rt5081_release -> rt5081_torch_store 
			-> 这个最后是读写闪光灯节点来打开或者关闭闪光灯
		
			2.rt5081_work_disable_ch1 -> INIT_WORK(&rt5081_work_ch1, rt5081_work_disable_ch1)
			-> rt5081_timer_func_ch1 -> rt5081_timer_start -> rt5081_operate 这边是对闪光灯的一些操作
		
			使能，只亮一个，前置后置闪光灯定时只亮一个，应该是拍照的自动打开闪光灯
		
			->创建了两个工作队列
		}
	
	
		./drivers/misc/mediatek/pmic/mt6355/v1/pmic_irq.c
		pmic_thread_kthread ->
		{
			[PMIC_INT] addr[0x856]=0x8
		
			struct pmic_interrupts interrupts[] = {
		PMIC_M_INTS_GEN(MT6355_INT_STATUS0, MT6355_INT_RAW_STATUS0,			//0x0854
				MT6355_INT_CON0, MT6355_INT_MASK_CON0, interrupt_status0),	//0x0856
		PMIC_M_INTS_GEN(MT6355_INT_STATUS1, MT6355_INT_RAW_STATUS1,
				MT6355_INT_CON1, MT6355_INT_MASK_CON1, interrupt_status1),
		PMIC_M_INTS_GEN(MT6355_INT_STATUS2, MT6355_INT_RAW_STATUS2,
				MT6355_INT_CON2, MT6355_INT_MASK_CON2, interrupt_status2),
		PMIC_M_INTS_GEN(MT6355_INT_STATUS3, MT6355_INT_RAW_STATUS3,
				MT6355_INT_CON3, MT6355_INT_MASK_CON3, interrupt_status3),
		PMIC_M_INTS_GEN(MT6355_INT_STATUS4, MT6355_INT_RAW_STATUS4,
				MT6355_INT_CON4, MT6355_INT_MASK_CON4, interrupt_status4),
		PMIC_M_INTS_GEN(MT6355_INT_STATUS5, MT6355_INT_RAW_STATUS5,
				MT6355_INT_CON5, MT6355_INT_MASK_CON5, interrupt_status5),
		PMIC_M_INTS_GEN(MT6355_INT_STATUS6, MT6355_INT_RAW_STATUS6,
				MT6355_INT_CON6, MT6355_INT_MASK_CON6, interrupt_status6),
		};
	
	
			09-19 16:46:07.209264    70    70 E [  165.765006] (0)[70:pmic_thread][name:pmic_irq&]: [PMIC] [PMIC_INT] addr[0x856]=0x200
		
		
		}
	

	
		低电条件下电池电压低，闪光灯瞬间的大电流会把电池电压拉到很低，触发了pmic的中断，
		跟哪些中断有关？这些中断是干什么的？设定的阈值是多少？是否能修改？如何修改的合理？
	
	
		1.RG_INT_STATUS_NAG_C_DLTV:这个中断应该是检测电池电压如果短时间内电池电压变化如果超过了阈值
		2.还有一个可能跟低电保护有关，开闪光灯电池电压可以拉低0.4V
		3.bat_temp_l,bat_h有几个相关的参数在mtk_battery_property.h文件内设置的
		最后确定的中断是bat_l_int_handler
		/* ex. 3400/5400*4096*/
		#define BAT_HV_THD   (POWER_INT0_VOLT*4096/5400)	/*ex: 3400mV */
		#define BAT_LV_1_THD (POWER_INT1_VOLT*4096/5400)	/*ex: 3250mV */
		#define BAT_LV_2_THD (POWER_INT2_VOLT*4096/5400)	/*ex: 3000mV */
	
	
	
		(flashlight-core.c)pt_trigger
		fl_info("PT disable flashlight: (%d,%d,%d)\n",
									pt_low_vol, pt_low_bat, pt_over_cur);
	
	
		09-19 16:45:58.761727    70    70 E [  156.049280] (7)[70:pmic_thread][name:flashlight&]: [FLASHLIGHT] pt_trigger: PT disable flashlight: (1,0,0)
	
		确定是low battery，pt_low_vol = LOW_BATTERY_LEVEL_0;
	
	
		pmic_throttling_dlpt 低电保护线程
	
	
	
		pt_trigger -> pt_low_vol_callback -> flashlight_pt_store -> register_low_battery_notify(&pt_low_vol_callback, LOW_BATTERY_PRIO_FLASHLIGHT);
	
		 
		-> exec_low_battery_callback(LOW_BATTERY_LEVEL_0) -> bat_l_int_handler 
	
		typedef enum LOW_BATTERY_PRIO_TAG {
			LOW_BATTERY_PRIO_CPU_B = 0,
			LOW_BATTERY_PRIO_CPU_L = 1,
			LOW_BATTERY_PRIO_GPU = 2,
			LOW_BATTERY_PRIO_MD = 3,
			LOW_BATTERY_PRIO_MD5 = 4,
			LOW_BATTERY_PRIO_FLASHLIGHT = 5,
			LOW_BATTERY_PRIO_VIDEO = 6,
			LOW_BATTERY_PRIO_WIFI = 7,
			LOW_BATTERY_PRIO_BACKLIGHT = 8
		} LOW_BATTERY_PRIO;


		闪光灯有双闪，哪双闪？电流分别是多少？开启闪光灯的时间大概是多少？怎么修改低电保护延迟的判断时间？

		而低电量开启闪光灯，电流很大，rt5081要输出很大的功率，但是低电条件下输入能力不足，导致电池电压被拉的很低，
	
	
		(pmic_throttling_dlpt.c) dlpt_check_power_off
	
	
		modify test
		{
			修改FLED_VMID是0x7A的bit[5:0]，目前寄存器默认是5V（bit[5:0]=110111）。

			请按建议修改，试试看

			FL_VMID=4.0V（bit[5:0]=001111），FL_VMID=4.1V（bit[5:0]=010011），FL_VMID=4.2V（bit[5:0]=010111），FL_VMID=3.9V（bit[5:0]=001011）.
	
			(rt5081.dtsi)
	
				rt5081_pmu_fled1 {
				compatible = "richtek,rt5081_pmu_fled1";
				interrupt-names = "fled_lvf", "fled2_short",
						  "fled1_short";
				fled_enable = <1>;
				torch_cur = <300000>;  /*25000 ~ 400000 uA */
				strobe_cur = <1200000>; /* 100000 ~ 1500000 uA */
				strobe_timeout = <2400>; /* 64 ~ 2432 ms */
			};
			rt5081_pmu_fled2 {
				compatible = "richtek,rt5081_pmu_fled2";
				fled_enable = <1>;
				torch_cur = <200000>;
				strobe_cur = <1000000>;
				strobe_timeout = <1200>;
	
	
	
	
	
			(rt5081_pmu_fled.c) rt5081_pmu_reg_update_bits这个是写寄存器的
			int rt5081_pmu_reg_update_bits(struct rt5081_pmu_chip *chip, u8 addr,u8 mask, u8 data)
		
			//Gionee <GN_BSP_CHG> <lilubao> <20170929> add for debug flashlight begin
	
			
			dev_dbg_ratelimited(chip->dev, "%s: reg %02x data %02x\n", __func__,
		addr, data);
		
		
				ret=rt5081_pmu_reg_update_bits(fled_data->chip,RT5081_PMU_REG_FLEDVMIDTRKCTRL1,
			RT5081_FLED_FIXED_MODE_MASK,0x4f);	// vmid -> 4.0v
			
			
			int rt5081_pmu_reg_read(struct rt5081_pmu_chip *chip, u8 addr)
			
			rt5081_fled_set_mode
			
				
		
			#define RT5081_PMU_REG_FLEDVMIDTRKCTRL1	(0x7A)
			#define RT5081_PMU_REG_FLEDVMIDRTM	(0x7B)
			#define RT5081_PMU_REG_FLEDVMIDTRKCTRL2	(0x7C)
	
		}
		
		
		
		
		邮件
		{
			Hi  樊工、宝玉;

			1.目前在深圳这一台手机上验证，把VMID设定为4V，打闪100次Pass@VBAT=3.7V。

			 

			因为深圳测试端只有1台，无法多试几台，请北京这边帮忙试试看，先设定VMID=4V@VBAT=3.7V，打闪是否Pass？

			a.如果Pass，那再设定VMID=4.1V or 4.2V@VBAT=3.7V，打闪是否Pass？

			b.如果Fail，那再设定VMID=3.9V@VBAT=3.7V，打闪是否Pass？

			 

			2.线路上Battery到RT5081的VBAT有一颗Logic IC（SGM4075）做硬复位使用。

			 

			请问这颗Logic IC用途为何？为Battery硬复位还是为系统硬复位？

			另外RT5081也有QonB 15s做系统Reset功能。
			

			3.北京测试反馈，电池电压会掉到3.32V，且拍照都不正常，这个电压没有低于先前提供的3.25V门限。

		

			4.一旦触发3.25V门限后，软件将会做什么动作。
			
			
			
			

			韦工

				 1、请问你们测试PASS的标准是什么？

				 2、硬复位开关是在系统出现问题时可以强制断电的，我们需要所有点都断开，类似于拔掉电池，RT5081做不到。

				 3、不管是否掉到门限电压，只要掉电压，我觉得就不是很正常，因为我们测试其他的项目是没有掉电压的，所以需要查明原因。如果是电流问题就修改电流，是配置问题就修改配置，现在原因还不清晰。

			 

			路宝

				 回答一下第四条，谢谢！
				 
			 
			 
			 
			 HI：韦工，宝玉
			  3.25V对应的  LOW_BATTERY_LEVEL_1，触发了低电保护的中断，会累加g_low_battery_level同时会判断是电池是否真的耗完了
			  LOW_BATTERY_LEVEL_1,会关掉了闪光灯和之前等级低的几个，CPU和GPU会限制performance,降频,降功率

				  typedef enum LOW_BATTERY_PRIO_TAG {
					LOW_BATTERY_PRIO_CPU_B = 0,
					LOW_BATTERY_PRIO_CPU_L = 1,
					LOW_BATTERY_PRIO_GPU = 2,
					LOW_BATTERY_PRIO_MD = 3,
					LOW_BATTERY_PRIO_MD5 = 4,
					LOW_BATTERY_PRIO_FLASHLIGHT = 5,
					LOW_BATTERY_PRIO_VIDEO = 6,
					LOW_BATTERY_PRIO_WIFI = 7,
					LOW_BATTERY_PRIO_BACKLIGHT = 8
				} LOW_BATTERY_PRIO;


			  低电保护level2的情况下，如果低电保护线程如果两次都检测到电池电压小于3.1V，就会调用关机
			  /*2nd time receive battery voltage < 3.1V, wait FG to call power off */
			 
			  这个线程的检测时间轻负载的情况下是20s，正常是10s
				 ktime = ktime_set(20, 0); /* light-loading mode */
			  else
				 ktime = ktime_set(10, 0); /* normal mode */
				 
				 
				 
				 1.请问你们测试PASS的标准是什么？

				à拍照不会黑掉，而且主闪会在100ms左右；另外，VBAT-in不会掉到3.25V以下@VBAT=3.7V & VMID设定为4V (沈工与周工也有协助帮忙)

				à波形如附件
			
			
			
			
			
			
			
			Hi  宝玉、路宝;


			1.          关于贵司之前的项目打闪不会掉电池电压：

			ð  请帮忙提供条件：电池电压/打闪电流

			ð  请帮忙量测波形：VMID/ IBAT/VBAT/ILED (如果量不到的话，就量测VLED)

			ð  相同条件下，务必两个项目一起量测才能比较

			ð  是否可以提供之前的项目layout，并与该项目比较，确认FLED电流路径的trace是否雷同？

			 

			2.       关于打闪时，VMID的Overshoot：

			ð  已提供EVB上不会有这个现象

			ð  是否每一支都会这样，还是特例?

			 

			3.        关于低电压打闪会黑的问题：

			ð  目前Layout上FLED电流路径的trace是否符合MTK建议 (之前有寄出的MTK report参考)

			(尤其是BAT connect to Reset IC to RT5081 VBAT路径)

			ð  另外请问调整VMID电压

                 i.是否调整VMID到4V都pass？

                 ii.是否可以多寄几支到深圳 ( 深圳只有一支，深圳金立其他同事也要用，FAE没办法做实验；目前深圳这支手机打闪电流不到1A，帮忙提供打闪电流达到1.5A的手机)




			

			韦工

				 我们用同一块电池对比，在17G10A上有电压被拉低的现象，在其他项目上没有被拉低的现象，如果觉得这个实验还不够说明问题，请贵司派工作人员到我们这里现场指导一下如何测试。

				 LAYOUT我们不能外发，贵司可以拍工作人员现场来看一下，我们自己对比觉得差异不大，请知悉！
				 
				 
				 
				 
				 
				 
				 Hi  宝玉、樊工;

					Q : 为何打闪会导致VBAT connector端 drop 200mV，这是否适合理现象?

					A :

					1.  这是合理现象，V=IR，由下列实验叙述验证

					2.  电流部分：打闪电流Ch1(250mA) + Ch2(450mA)，IBAT实际量测1.2A

					3.  内阻部分：电池package阻抗值(与樊工沟通后，查询了欣旺达ZCV阻抗值约160mΩ)

					4.  电流*内阻=1.2A*160mΩ=192mV

					 

					Q : 有什么改善建议吗?

					A :

					1.  因为V=I*R, 只能由I & R做手脚

					2.  FLED电流不能变动，因此建议修改FLED电压改低，以不影响打闪为前题(也就是不会压到headroom的意思)。

					3.  因为目前电流软件设定不能超过1.1A，因此修改幅度有限

					4.  因顾及贵司可能有固定使用电池，要不也可以考虑由阻抗(R)着手

					 

					Q : 除此之外，是否还有其他建议?

					A :

					1.  贵司目前把电池电流设定为最高1.1A，可能导致拍照亮度不足，请谨慎评估。

					2.  与樊工沟通，如下Layout有下列几点建议

					2-1.如下CHG_VMID到FL_VMID铜皮>1.5mm;
					
					
		}
		

	}	
	
	
	
	
	 平均电流测试自动化APK适配
	
	
	
	
	
	
	


}


























/*********************************************************************************************************************************/
17G06A
{


	GNSPR#122265,连接充电器，长按电源键关机，关机完成后，长按电源键5s测机不开机，只显示在关机充电图标界面，（在充电图标界面长按电源键则可以开机），
	用户体检不佳 暂未恢复 对比17G16-T0119版本有此现象，对比17G02-T2638版本无此现象，验证10台10台100%
	{
	
		
						关机充电亮屏时间				 	关机充电灭屏到开机			关机充电灭屏状态下到开机
		17G06A			8~9s						 	10~11s						4~5s
		
		
		
		
		首先这个问题是：插充电器进入关机充电，亮屏状态下长按powerkey，进入正常的启动流程这个时间是4~5s正常
		但是灭屏状态下，长按powerkey先是亮屏几秒，然后灭屏，过4~5s之后正常开机，总共10~11s，
		
		分析：
			1.这种情况应该可以排除硬件复位，虽然时间是10+2s左右，硬件复位battery会断电
			2.重启判断从第二次灭屏开始算的话，重启判断时间应该是相同的，
			但是17G06A 关机充电灭屏状态下长按powerkey，先亮屏然后灭屏，再过几秒正常开机
			
			所以现在问题是灭屏状态下长按powerkey走了什么流程，怎么判断是短按还是长按
			短按应该是亮屏
			{
				[   17.110191] *(1)[336:charger]charger: [17110] key[116] down

				[   17.330015] *(1)[363:charger]msm_thermal:msm_thermal_update_freq Freq mitigation task is not initialized
				[   17.344035] *1)PM: Some devices failed to suspend, or early wake event detected
				[   17.352156] *(3)[363:charger]msm_thermal:msm_thermal_update_freq Freq mitigation task is not initialized

				[   17.352197] *(0)[336:charger]charger: [17352] key[116] up (was down for 0.242sec)
				
				这几个在system 			healthd目录下
				
				
				core/healthd/healthd.cpp
				93:extern void healthd_mode_charger_init(struct healthd_config *config);
				115:    .init = healthd_mode_charger_init,

				core/healthd/healthd_mode_charger.cpp
				1771:void healthd_mode_charger_init(struct healthd_config* config)


				(healthd.cpp) process_key  这里可能有问题
				next_pwr_check		
 				/* If key is pressed when the animation is not running, kick
                 * the animation and quite suspend; If key is pressed when
                 * the animation is running, turn off the animation and request
                 * suspend.
                 */
	
			}
		
		
		
		
			
		

		KLOG_ERROR(LOG_TAG,"epoll_ctl failed; errno=%d\n", errno);
		
		
		//Gionee <GN_BSP_CHG> <lilubao> <20171012> add for debug pon begin
		pr_err("in [%s] by lilubao before\n",__FUNCTION__);
		//Gionee <GN_BSP_CHG> <lilubao> <20171012> add for debug pon end
		
		
		
		boot_images/core/systemdrivers/pmic/target/msm8953_pm8953_pmi8950/system/src/pm_sbl_boot_oem.c
		46:    err_flag |= pm_pon_reset_source_cfg(0, PM_PON_RESET_SOURCE_KPDPWR, 10256, 2000, PM_PON_RESET_CFG_WARM_RESET);
		49:    err_flag |= pm_pon_reset_source_cfg(0, PM_PON_RESET_SOURCE_RESIN_AND_KPDPWR, 10256, 2000, PM_PON_RESET_CFG_HARD_RESET);

		boot_images/core/systemdrivers/pmic/target/msm8937_pm8937_pmi8937/system/src/pm_sbl_boot_oem.c
		70:    err_flag |= pm_pon_reset_source_cfg(0, PM_PON_RESET_SOURCE_KPDPWR, 10256, 2000, PM_PON_RESET_CFG_WARM_RESET);
		72:    err_flag |= pm_pon_reset_source_cfg(0, PM_PON_RESET_SOURCE_KPDPWR, 6720, 2000, PM_PON_RESET_CFG_HARD_RESET);
		77:    err_flag |= pm_pon_reset_source_cfg(0, PM_PON_RESET_SOURCE_RESIN_AND_KPDPWR, 10256, 2000, PM_PON_RESET_CFG_HARD_RESET);

		boot_images/core/systemdrivers/pmic/drivers/pon/src/pm_pon.c
		21:06/09/15   sv      Updated pm_pon_reset_source_cfg for multi PMIC support
		531:pm_pon_reset_source_cfg(uint8 pmic_device_index, pm_pon_reset_source_type reset_source,

		boot_images/core/systemdrivers/pmic/framework/test/pm_test_driver_pon.c
		93:    err_flag = pm_pon_reset_source_cfg (pmic_index, PM_PON_RESET_SOURCE_RESIN_AND_KPDPWR, 3072, 1000, PM_PON_RESET_CFG_X_VDD_COIN_CELL_REMOVE_HARD_RESET);
		226:            err_flag = pm_pon_reset_source_cfg (pmic_index, (pm_pon_reset_source_type)i, 3072, 1000, (pm_pon_reset_cfg_type)j);

		boot_images/core/api/systemdrivers/pmic/pm_pon.h
		276:pm_pon_reset_source_cfg(uint8 pmic_device_index, 


		boot_images/core/storage/tools/deviceprogrammer_ddr/src/firehose/deviceprogrammer_initialize.c
		120:    pm_pon_reset_source_cfg(0,PM_PON_RESET_SOURCE_KPDPWR,904,500,

 		
		
	
	
		(msm-poweroff.c)  do_msm_restart  -> msm_restart_prepare  ->   (qpnp-power-on.c)  qpnp_pon_system_pwr_off 
		
		
		pm_pon.c,pm_sbl_boot_oem.c

	












		
		80-nm620-1_b_pmic_pon-reset_software_drivers_overview.pdf
		pm_pon.c,pm_pon.h，qpnp-power-on.c,msm-pm8937.dtsi,boot_dload_debug_target.c
		
		
		int qpnp_pon_system_pwr_off() – Determines the type of power off to perform:
shutdown, reset, etc.


		This code lets us check whether the PMIC went through a warm reset
		/drivers/power/qpnp-bms.c ,read_soc_params_raw(...){
		
		
		- qcom,pon-type			The type of PON/RESET source. The driver currently supports KPDPWR(0), RESIN(1) and
		CBLPWR(2) pon/reset sources.
		
		
		
		PON_XXXX_RESET_S1_TIMER
		PON_XXXX_RESET_S2_TIMER
		PON_XXXX_RESET_S2_CTL
		PON_XXXX_RESET_S2_CTL2
		
		
		重启的阶段
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
		
		
		Vdd这几个是干什么的？
		
		
		
		
		
		
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
			}
		
	
	}








	USB类
	{
		
		GNSPR#119978,连接充电器，待机界面》连接电脑传输文件后断开连接，主界面下滑通知栏仍显示有USB传输文件，点击USB传输文件闪退至桌面，
		操作多次未恢复，重新连接电脑再断开未恢复，手动重启恢复》
		

	}









	
	可能跟硬件相关的
	{
		GNSPR#119926,T2-2-228,BJ17G06A-T0050-170930AA ,吴能田
		这个可能跟NTC有关，NTC异常，因为开机后温度升到很高，
				
		请帮忙拆机测量一下NTC是否正常，电池连接器是否扣好
		然后用示波器测量一下NTC测试点的波形，如果正常应该是1.5s一次脉冲波
		没扣好会有1~2V的高电平，短路或者其他的，不会有1.5s的脉冲
		
		
		GNSPR#121951,T2-2-37,BJ17G06A-T0050-170930AA,陈艳，
		GNSPR#119947，T2-2-88,BJ17G06A-T0050-170930AA,吴 能田 
		这个可能是电池连接器没扣好，我本地试了，不扣电池，插充电器，电量显示50%，而且是关机充电不能开机
		
		
		
		GNSPR#119926 关机状态》(电量剩余：40%）手动按电源键开机后，手机自动关机，操作多次仍如此，暂未恢复（串口log）》验证2台，1台100% 
		{

			怀疑NTC有问题，电池温度在开机之后一段时间就变的很高，然后温度过高导致关机

			[ 94.207274] *(1)[96:kworker/1:1]FG: log_bat_status: soc=3885 vbat=3754250 ibat=310361 ocv=3801705 esr=109008 temp=402 vchg=246600
			[ 104.246362] *(1)[96:kworker/1:1]FG: log_bat_status: soc=3882 vbat=3749978 ibat=347288 ocv=3801705 esr=111145 temp=402 vchg=246600
			[ 114.286314] *(1)[96:kworker/1:1]FG: log_bat_status: soc=3880 vbat=3722817 ibat=597530 ocv=3801705 esr=110168 temp=405 vchg=244640
			[ 124.329015] *(1)[96:kworker/1:1]FG: log_bat_status: soc=3877 vbat=3697183 ibat=827021 ocv=3801705 esr=113525 temp=402 vchg=240740
			[ 2.299768] *(1)[1:swapper/0]BOOTPROF: 2299.762350:probe: probe=spmi_drv_probe drv=qcom,qpnp-temp-alarm(ffffffc00165ab60) 15.608489ms
			[ 3.789490] *(0)[54:kworker/0:1]FG: log_bat_status: soc=3874 vbat=3740822 ibat=432889 ocv=3801705 esr=110412 temp=-22 vchg=242720
			[ 3.809878] *(0)[271:kworker/0:2]FG: log_bat_status: soc=3874 vbat=3740822 ibat=432889 ocv=3801705 esr=110412 temp=400 vchg=242720
			[ 13.845252] *(0)[271:kworker/0:2]FG: log_bat_status: soc=3869 vbat=3685128 ibat=926813 ocv=3801705 esr=110412 temp=405 vchg=240780
			[ 23.939435] *(0)[271:kworker/0:2]FG: log_bat_status: soc=3864 vbat=3704202 ibat=737452 ocv=3801705 esr=107971 temp=405 vchg=242720
			[ 33.967626] *(0)[271:kworker/0:2]FG: log_bat_status: soc=3860 vbat=3742653 ibat=389402 ocv=3801705 esr=109069 temp=405 vchg=244680
			[ 35.763924] *(2)[1:init]BOOTPROF: [6/6].PON battery temperature : 400
			[ 44.007872] *(0)[271:kworker/0:2]FG: log_bat_status: soc=3855 vbat=3707101 ibat=695186 ocv=3801705 esr=109069 temp=405 vchg=242720
			[ 54.051098] *(0)[271:kworker/0:2]FG: log_bat_status: soc=3850 vbat=3706795 ibat=687251 ocv=3801705 esr=109802 temp=882 vchg=244680
			[ 2.230797] *(0)[1:swapper/0]BOOTPROF: 2230.791360:probe: probe=spmi_drv_probe drv=qcom,qpnp-temp-alarm(ffffffc00165ab60) 15.605937ms
			[ 3.718023] *(2)[97:kworker/2:1]FG: log_bat_status: soc=3845 vbat=3738839 ibat=425870 ocv=3801705 esr=110229 temp=-22 vchg=244640
			[ 3.737546] *(2)[292:kworker/2:2]FG: log_bat_status: soc=3845 vbat=3738839 ibat=425870 ocv=3801705 esr=110229 temp=407 vchg=244640
			[ 13.764648] *(2)[292:kworker/2:2]FG: log_bat_status: soc=3840 vbat=3682687 ibat=921167 ocv=3801705 esr=110961 temp=407 vchg=244640
			[ 23.804627] *(2)[292:kworker/2:2]FG: log_bat_status: soc=3836 vbat=3700692 ibat=752711 ocv=3799721 esr=111877 temp=925 vchg=244640
			[ 33.846244] *(2)[292:kworker/2:2]FG: log_bat_status: soc=3834 vbat=3755166 ibat=248411 ocv=3799721 esr=112243 temp=905 vchg=248560


			请帮忙拆机测量一下NTC是否正常，电池连接器是否扣好
			然后用示波器测量一下NTC测试点的波形，如果正常应该是1.5s一次脉冲波
			没扣好会有1~2V的高电平，短路或者其他的，不会有1.5s的脉冲
	
		}
		
		
	}
	
	
	
	




	充电器检测流程：
	 {
		 (qpnp-smbcharger.c) src_detect_handler 电压是上升的还是下降-> handle_usb_insertion  USB插入工作
		 
		 读取充电器类型，过滤杂波 -> smbchg_change_usb_supply_type 设置充电电流
		 
		  * src_detect_handler() - this is called on rising edge when USB charger type
		 is detected and on falling edge when USB voltage falls
		 below the coarse detect voltage(1V), use it for
		 handling USB charger insertion and removal.
		 
		 
		 充电器的类型：
		 enum power_supply_type {
			POWER_SUPPLY_TYPE_UNKNOWN = 0,
			POWER_SUPPLY_TYPE_BATTERY,
			POWER_SUPPLY_TYPE_UPS,
			POWER_SUPPLY_TYPE_MAINS,
			POWER_SUPPLY_TYPE_USB,		/* Standard Downstream Port */
			POWER_SUPPLY_TYPE_USB_DCP,	/* Dedicated Charging Port */
			POWER_SUPPLY_TYPE_USB_CDP,	/* Charging Downstream Port */
			POWER_SUPPLY_TYPE_USB_ACA,	/* Accessory Charger Adapters */
			POWER_SUPPLY_TYPE_USB_HVDCP,	/* High Voltage DCP */
			POWER_SUPPLY_TYPE_USB_HVDCP_3,	/* Efficient High Voltage DCP */
			POWER_SUPPLY_TYPE_WIRELESS,	/* Accessory Charger Adapters */
			POWER_SUPPLY_TYPE_BMS,		/* Battery Monitor System */
			POWER_SUPPLY_TYPE_USB_PARALLEL,		/* USB Parallel Path */
			POWER_SUPPLY_TYPE_WIPOWER,		/* Wipower */
			POWER_SUPPLY_TYPE_TYPEC,	/*Type-C */
			POWER_SUPPLY_TYPE_UFP,		/* Type-C UFP */
			POWER_SUPPLY_TYPE_DFP,		/* TYpe-C DFP */
		};
	
	}	
	
	
}








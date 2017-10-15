/*注意事项*/
{
	BJ17G16A & BJ17G06A 主干代码已经切换回 T0023，请各位首先fetch，然后基于T0023建立分支
	remotes/origin/old_code_main_dev_7.1.2
	remotes/origin/old_patch_main_dev_7.1.2
	BJ17G16A  的modem 的主干也已切换到  BJ17G16A_MODEM-T0013-170714AA
	hi all ：
	为了更好的理解与代码管理，特在BJ17G16A代码库中更改了拷贝顺序，主要修改点是由原来的拷贝顺序：
	copy  ./gionee/project/项目  
	copy  ./gionee/alps 
	修改为：
	copy  ./gionee/alps 
	copy  ./gionee/project/项目  
}









/*重要的概念*/
{
	电量计
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


	BAT_ID:
	{
		通过BAT_ID检测电池的存在与否，这个检测过程内嵌在fg算法中，重复的升高基础电流（5uA->15uA->150uA）直到发现匹配的
		智能电池调控通常是提供5uA的灌电流去识别，因为5uA可以通过软件去识别然后设置使能标志位
		(Battery Serial Interface (BSI) module)
			电量计读电流通过采样电阻，读电池电压通过连接电池正负极的BATT_P，BATT_N
	放电读正极，其他时候读负极
	}


	thermal：
	{
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
	}		



	ESR(电池等效电阻)
	{
		ESR可以提高电量计的准确性，对其他的也有影响，但是ESR在低电流的状态下是不需要的，而且在系统suspend的时候是无法使能的
		电池等效串联电阻ESR，这个值最好是实时的，则样才最准确等效电阻跟temperature有很大关系，同时也影响到电池的剩余电量
		power_supply 子系统是这个POWER_SUPPLY_PROP_RESISTANCE而这个数据的采集是在电量计每隔90s发一个脉冲，然后同步同一时间的
		电压和电流而这个脉冲会引起设备底电流的升高，所以当设备suspend的时候会关闭这个使能
	}
		


	System termination current(系统的截止电流)
	{
		可以客制化电池100%状态的上报，
		qcom,fg-chg-iterm-ma这个是设置充电截止电流
		这里应该讲的是系统满电的状态，充电电流，电池电压
		显示100%，系统截止电流，充电截止电流
	}


	FG termination current（这个使用电量计充电截止电流来判断停止充电的条件）
	{
		也就是说充电截止电流可以通过qcom,fg-chg-iterm-ma，或者qcom,chg-term-ua(不用)来设定
		充电截止的条件
			qcom,fg-iterm-ma = <150>; //这个是显示100%的截止电流？

		电量计的截止电流
		qcom,fg-chg-iterm-ma = <100>;	//这个是充电截止电流？
	}


	System cutoff voltage：系统0%电量对应的电压
	qcom, fg-cutoff-voltage-mv = <3000>; /*example 3V */


	Estimate voltage at bootup(启动阶段估计电压)：
		Reload the profile for a better SoC estimate if the vbat_current_predicted differs from the actual vbat by more than 
		a set threshold.
	如果启动阶段检测到的电压跟实际的电压偏差超过多少就重新载入数据


	CC_to_CV threshold set point
	{
		恒流到恒压模式转变的阈值：
		qcom, fg-cc-cv-threshold-mv = <4340>;
		当然这里还有一个浮动电压，比上面的阈值高10V
		Specify the CC_CV set point for PMI895x FG to 4340 mV (currently default), which is 10 mV lower than the float voltage
		configured (currently default is 4350 mV).This is needed for EoC to be notified properly.
	}


	Resume charging based on SoC
		qcom,resume-soc = <95>;
	当电量低于多少的时候，触发回充，recharging


	External/internal current sense
		这个应该是并行充电要添加的
		qcom,ext-sense-type


	IRQ_volt_empty
	{
		低电中断至少要比system cutoff voltage高50mV
		Recommended setting/default = System Cutoff voltage – 50 mV (may need more headroom than 50 mV value, depending on 
		the system)
		因为上层读到0%的时候会立刻执行shutdown流程，避免电压低于UVLO
		irq-volt-empty-mv
	}


	Battery age detection：
	{
		1.通过等效电阻来估计电池的容量
		2.软件上通过库仑计来学习电池的容量


		The Battery Learning Capacity algorithm takes into account the following:
		1.Temperature
		2.Qualified starting point of the SoC of the battery
		3.Allowable increment and decrement of each charge cycle to qualify for each learning cycle
		这里应该是每次充满电循环所需要的时间，一次来估计电池的容量，和电池老化


		Charge cycle count
		当电量低于low-soc的时候触发算法，电量高于high-soc的时候counter增加，只有走一个完整的过程counter才会增加，如果移除电池的话数据会被清除，
		因为数据存储在FG SRAM
	}


	
		dtsi文件中的中断，根据log中的中断在dtsi文件中找对应的中断
		{
			hwirq = GIC_PPI_START/GIC_SPI_START + interrupt number
			1 – Interrupt type
				0 – SPI
				1 – PPI
			2 – Interrupt number of interrupt type
			3 – Trigger type
				1 – Low-to-high edge triggered
				2 – High-to-low edge triggered
				4 – Active high-level sensitive
				8 – Active low-level sensitive



			msm-pmi8937.dtsi
			qcom,chgr@1000 {
				reg = <0x1000 0x100>;
								type ，number， trigger type
				interrupts =    <0x2 0x10 0x0>,
						<0x2 0x10 0x1>,
						<0x2 0x10 0x2>,
						<0x2 0x10 0x3>,
						<0x2 0x10 0x4>,
						<0x2 0x10 0x5>,
						<0x2 0x10 0x6>,
						<0x2 0x10 0x7>;

				interrupt-names =    "chg-error",
							"chg-inhibit",
							"chg-prechg-sft",
							"chg-complete-chg-sft",
							"chg-p2f-thr",
							"chg-rechg-thr",
							"chg-taper-thr",
							"chg-tcc-thr";
			};

			80-p0897-1_a_presentation__idle_xo_shutdown_vdd_minimization_overview.pdf
		}

}















/*代码分析*/
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




















/*调用流程*/
{

	1.充电	
		(qpnp-smbcharger.c) 这个是高通充电用的主要的源代码，smbchg_init初始化模块 -> spmi_driver_register注册设备驱动 -> smbchg_probe将设备驱动跟设备挂钩，
		
		建立bind -> 从dtsi文件中获取一些参数 ->  create_votable创建了好几个投票变量 (fcc_votable,usb_icl_votable,dc_icl_votable,usb_suspend_votable,

		dc_suspend_votable,battchg_suspend_votable,hw_aicl_rerun_disable_votable,hw_aicl_rerun_enable_indirect_votable,aicl_deglitch_short_votable,hvdcp_enable_votable)



		高通的充电基本上都叫smbcharger
		(qpnp-smbcharger.c) smbchg_init 模块初始化->smbchg_probe	从dtsi文件中获取参数，上面还有一个投票制度貌似很重要，还有一些工作 -> smbchg_usb_update_online_work,
		
		判断usb是否插入,并上报状态到power_supply子系统 -> smbchg_parallel_usb_en_work并行充电使能，对并行充电的条件进行判断 aicl是否稳定，没有电池的条件下不允许改变
		
		并行充电的状态 -> smbchg_parallel_usb_enable -> smbchg_vfloat_adjust_work动态充电电压调节



	debug
	{
		1.电量计
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


		2.功耗问题的大概分析思路  80-p0897-1_a_presentation__idle_xo_shutdown_vdd_minimization_overview.pdf
		{
			这个是通过波形图，分析一个时间段内的波形，异常较多，系统没睡;
			然后打开debug log 清除之前的log;住区一段时间内的log和波形图，然后检索suspen entry进入休眠
			到suspend exit 之间的log，有哪些唤醒，中断通常看较后面的那个，其实想知道这个中断或者子系统是什么
			定位具体的原因

			Step 1 – Check current waveform
			Step 2 – Enable Power Management (PM) debug kernel log
			Step 3 – Determine which IRQs prevent system sleep
			Step 4 – Determine the specific hwirq owner
			Step 5 – Determine which clocks stay on and prevent system sleep	
			
			
			echo 32 > /sys/module/msm_pm/parameters/debug_mask
			
			echo 8 > /sys/module/mpm_of/parameters/debug_mask
			
			cat  /proc/interrupt
			
				//中断看最后一个
				if (d->hwirq == 280 && enable) {
					pr_err ("!!!hwirq 280 registered, wakeset %d\n", wakeset);
					dump_stack();
				}
			
			Search “Enabled clocks” to determine which clocks prevent XO shutdown
			and VDD minimization


				1.
					echo 1 > /sys/module/msm_show_resume_irq/parameters/debug_mask
					echo 1 > /sys/kernel/debug/clk/debug_suspend
					echo 8 > /sys/module/mpm_of/parameters/debug_mask
					echo 32 > /sys/module/msm_pm/parameters/debug_mask

				2. 
					adb shell dmesg -c
					adb shell dumpsys batterystats --enable full-wake-history
					adb shell dumpsys batterystats --reset


				3. plug out USB and perform your test


				4. adb bugreport > bugreport.txt
				adb shell logcat -b main -v threadtime > android.log
				adb shell cat dmesg  > dmesg.log

			}
	}
}
		





/*相关的文件*/
{
	1.充电相关的文件代码
	{
		kernel/msm-3.18/drivers/power/qpnp-fg.c
		kernel/msm-3.18/arch/arm/boot/dts/qcom/msm-pmi895x.dtsi
		kernel/msm-3.18/Documentation/devicetree/bindings/power/qpnp-fg.txt
		
		
		dtsi配置文件
		kernel/msm-3.18/arch/arm64/boot/dts/qcom/msm8917-pmi8937-qrd-sku5.dtsi
		kernel/msm-3.18/arch/arm64/boot/dts/qcom/msm-pmi8937.dtsi

		kernel代码
		kernel/msm-3.18/drivers/power/qpnp-smbcharger.c
		kernel/msm-3.18/drivers/power/pmic-voter.c  
	}


	高通相关文档
	{
		1.系统稳定性
		{
			几个高通平台稳定性文档，感兴趣的可以下载看。
			80-NM641-1			80-P7139-1
			80-P7139-3			80-P7139-5
			80-P7139-6			80-P7139-7
			80-P7139-8
		}
		
		
		2.功耗分析
		{
				1.80-P0955-1SC  
				很详细的功耗debug中文手册,里面有各种case debug的步骤,以及如何来抓取各种log。功耗优化的必读宝典
				2.80-NT616-1 
				有各种多媒体case的功耗调试手段介绍

				3.80-P0956-1 		Android 功耗概述

				4.80-P1818-1EC 		客户机功耗测试指导手册

				5.80-N6837-1 		高通内部Power dashboard测试用例详细步骤

				6.80-P0897-1 		IDLE, XO Shtudown, VDD Min 调试概述

				7.80-NP885-1 		Graphic 功耗概述

				8.80-NP961-1 		Camera功耗调试手册

				9.80-P0834-1 		Video功耗调试向导
			
				10.80-P0106-1 		Core Control 介绍

				11.80-NR497-1 		Modem时钟和功耗管理调试向导

				12.80-N1089-1 		NPA概述

				13.80-P3103-1 		总线动态调频调压概述

				14.80-N8715-14 		AVS Adaptive Voltage Scaling 概述

				15.80-NU566-1 		DPM 数据功耗管理概述
		}
		

		3.USB quick start
		{
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


			evaluate 80-P2485-17
		
			define/design 80-P2485-17,80-P2468-5B
		
			build/bringup  80-NF238-1 ,80-NA648-1
		
			customize 80-NF283-1,80-VB717-1,80-V4609-1
		
			fine-tune/verfiy quality/cerfity 80-NF283-1
		}



		4.pmic
		{
			Based on these selections, the following information can be used to help resolve common issues in this particular area:

			To download any document directly from this solution, first login to the CreatePoint and then click on the hyperlink listed against the relevant document below.

			80-P2485-18 : MSM8937 System Drivers PMIC Overview
			
			modem目录
			boot_images/core/api/systemdrivers/pmic

			
			80-P2485-2 : MSM8937_Linux_Android_PMIC_SW_Drivers_Overview
			80-NG006-1 : System Driver PMIC NPA Overview
			80-NN255-1 : RPM PMIC SW Driver Overview
			80-NV610-47 : LA VREG CLK SW User Guide

			For a complete list of PMIC Software documents and Knowledge base solutions for all technology areas please refer to the following master documents:

			80-NR097-1 : PMIC Software Master Document
			80-NR097-2 : PMIC Software KB Solution Master Document

		}
	}
	
}












/*debug*/

/**********************************************************************************************************************************/
1.usb眼图失败
{
	眼图测试失败的case
		You can follow doc 80-PB524-1 usb_tunning_-_phy_eye_diagram_for_oems(1).pdf
		and 80-NA648-1_H_Tuning_USB_PHY_Eye_Diagram_Receiver_Sensitivity.pdf on eye diagram tunning. 


	modify code
	{

		现在是分成host/devics两种mode		kba-170123171355
		
		msm8917-pmi8937-qrd-sku5.dtsi

		value  addr  value addr  value addr  
		qcom,hsusb-otg-phy-init-seq =
		//Gionee <gn_by_CHG> <lilubao> <20170821> modify for USB eye diagram begin
			<0x74 0x80 0x3c 0x81 0x3d 0x82 0x33 0x83 0xffffffff>;
		qcom,hsusb-otg-phy-host-init-seq =
			<0x74 0x80 0x39 0x81 0x1d 0x82 0x33 0x83 0xffffffff>;
		//Gionee <gn_by_CHG> <lilubao> <20170821> modify for USB eye diagram end


		（phy-msm-usb.c） ulpi_init
		//Gionee <gn_by_CHG> <lilubao> <20170821> modify for USB eye diagram begin
        //seq = pdata->phy_init_seq;
        if (test_bit(ID, &motg->inputs)) {
            seq = pdata->phy_init_seq;
            pr_debug("use device mode init seq\n");
        }
        else{
            seq = pdata->phy_host_init_seq;
            pr_debug("use host mode init seq\n");
        }
        //Gionee <gn_by_CHG> <lilubao> <20170821> modify for USB eye diagram end


		msm_usb_phy_reset
		/* Program tuning parameters for PHY */
        //Gionee <gn_by_CHG> <lilubao> <20170821> modify for USB eye diagram begin
        //seq = motg->pdata->phy_init_seq;
        if (test_bit(ID, &motg->inputs)) {
            seq = motg->pdata->phy_init_seq;
            pr_debug("use device mode init seq\n");
        }
        else{
            seq = motg->pdata->phy_host_init_seq;
            pr_debug("use host mode init seq\n");
        }
        //Gionee <gn_by_CHG> <lilubao> <20170821> modify for USB eye diagram end


		msm_otg_dt_to_pdata
		//Gionee <gn_by_CHG> <lilubao> <20170821> modify for USB eye diagram begin
		of_get_property(node, "qcom,hsusb-otg-phy-host-init-seq", &len);
		if (len) {
			pdata->phy_host_init_seq = devm_kzalloc(&pdev->dev, len, GFP_KERNEL);
			if (!pdata->phy_host_init_seq)
				return NULL;
				
			of_property_read_u32_array(node, "qcom,hsusb-otg-phy-host-init-seq",
			pdata->phy_host_init_seq,
			len/sizeof(*pdata->phy_host_init_seq));
		}
		//Gionee <gn_by_CHG> <lilubao> <20170821> modify for USB eye diagram end

	}



	相关文档：	
		80-na648-1a
		80-PB524-1A  	phy,override
		kba-170123171355
		
}













/*********************************************************************************************************************************/
2.diag口打不开	
{

	#96132，adb端口被占用	
	{
		现在要解决的是如何让diag口跟adb如何共用，就是打开diag口同时可以使用adb命令

		分析：
		17G06A不能用，17G02A可以使用，对比两个项目的代码，初始化到打开端口的log

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


	modify code
	{
	  (init.qcom.usb.rc)	

		on property:sys.usb.config=diag,serial_smd,rmnet_ipa,adb && property:sys.usb.configfs=0
		write /sys/class/android_usb/android0/enable 0
		write /sys/class/android_usb/android0/iSerial ${ro.serialno}
	#Gionee <GN_BSP_SYS> <chuqf> <20161028> for usb_pid_vid begin
		write /sys/class/android_usb/android0/idVendor 271D
	#Gionee <GN_BSP_SYS> <chuqf> <20161028> for usb_pid_vid end
		write /sys/class/android_usb/android0/idProduct 9091
		write /sys/class/android_usb/android0/f_diag/clients diag
		write /sys/class/android_usb/android0/f_serial/transports smd
		#Gionee <GN_BSP_USB> <lilubao> <20170823> modify for diag,adb begin
		write /sys/class/android_usb/android0/f_rmnet/transports qti,bam
		//Gionee <GN_BSP_USB> <lilubao> <20170823> modify for diag,adb end
		write /sys/class/android_usb/android0/functions diag,serial,rmnet,adb
		write /sys/class/android_usb/android0/enable 1
		start adbd
		setprop sys.usb.state ${sys.usb.config}

	}


	这两个问题要有明确的分析思路：
	1.抓一份log，测试这个case，在这两个时间点之间有什么log，走了什么东西，有什么异常
	 其中这个bug抓到的log中有明显的 port invaild 但是对USB的架构和工作方式又不是很清楚
	 所以只有对一个知识有一定的体系之后才能高效的定位问题，一定要熟悉高通的文档


	2.对比测试，相同的平台有什么差异，通过对比确定其中的差异，定位问题的范围跟点
	这个bug的原因虽然几个项目平台差差不多，但是17G06A较低端，不具备DMA功能，在上层设置9091这个功能的
	时候，设置到底层因为不支持导致设置出错，因为9091有好几个，没仔细看 
	有的变量或者文件在很多地方都有，首先要确定的哪个是起作用的，谁调用的

}











/********************************************************************************************************************************/
3.打开飞行模式电流3~4mA，关闭飞行模式之后，其他全关闭，电流18mA以上
{

	platform版本 开飞航模式后02:39到04:52没有什么请其它交互，ril没怎么工作。
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
	{
		android_qcom_mp/gionee/project/BJ17G06/vendor/qcom/proprietary/sensors/dsps/reg_defaults/sensor_def_qcomdev.conf

		adb remount
		adb shell rm /etc/sensors/sensor_def_qcomdev.conf
		adb push sensor_def_qcomdev.conf  /etc/sensors
		adb shell chmod 644 /etc/sensors/sensor_def_qcomdev.conf
		adb shell rm /persist/sensors/sns.reg
		adb reboot
	}



	常用debug的方法
	{
		通常思路 察看各各子系统的情况，是否没睡，哪个没睡
		都有哪些子系统，代表什么，大致负责哪些的

		获取rpm的状态，
		cat d/rpm_stats
		cat d/rpm_master_stats
	
		各各子系统的状态，如果有唤醒，则numshutdowns这个会有变化


		1.
			echo 1 > /sys/module/msm_show_resume_irq/parameters/debug_mask
			echo 1 > /sys/kernel/debug/clk/debug_suspend
			echo 8 > /sys/module/mpm_of/parameters/debug_mask
			echo 32 > /sys/module/msm_pm/parameters/debug_mask

		2. 
			adb shell dmesg -c	(还有一个清楚main log的命令)
			adb shell dumpsys batterystats --enable full-wake-history
			adb shell dumpsys batterystats --reset


		3. plug out USB and perform your test


		4. adb bugreport > bugreport.txt
		adb shell logcat -b main -v threadtime > android.log
		adb shell cat dmesg  > dmesg.log
	}



	这个问题最后确定是modem的问题，不插卡在gsm状态，modem不停的搜网

	功耗问题，有两个case 在个高通跟，
	case 03138699 在跟进GSM 功耗高的问题，目前分析协议没有问题，但是仍在要求提供ramdump 文件。因为有几个不规律波形出来。
	case 03140253  在跟进LTE 功耗高的问题，需要CR release 出来。

	目前不插卡功耗高的问题，认为和GSM standy 功耗高相关。解决完GSM standby 功耗问题后，此问题可以复测。
	目前功耗问题，包括GSM standby 和LTE standby 两类。后续会在redmine 及时更新分析进展，谢谢。

}















/************************************************************************************************************************************/
4.电池曲线	 
{
	case ID:03075361   20170807 

	battery ID：15k
	NTC：9k
	电池容量：4000mAh


	(msm8917-pmi8937-qrd-sku5.dtsi)
	qrd_batterydata: qcom,battery-data {
        qcom,batt-id-range-pct = <15>;
        //Gionee <gn_by_CHG> <lilubao> <20170821> modify for USB eye diagram begin
        #include "batterydata-gionee-17g06a-sunwoda-4000mah-4400mv.dtsi"
        //Gionee <gn_by_CHG> <lilubao> <20170821> modify for USB eye diagram end
        //Gionee <gn_by_charging> <lilubao> <20170713> add for platform change begin
        #include "batterydata-JL-3100mah-4400mv.dtsi"
        //Gionee <gn_by_charging> <lilubao> <20170713> add for platform change end
    };


	初始化的时候是根据读到的电池的电阻去识别电池曲线的
	qcom,batt-id-kohm = <15>;

	但是这还有一个问题就是电池曲线的加载，芯片必须断电才能写入，否则仍然是以前的电池曲线

	of_batterydatat.c 这个文件是读取电池曲线相关参数的
}












/************************************************************************************************************************************/
5.	过压
{
	usbin_ov_handler 过压的回调函数
	
	
	<6>[ 3224.196260] *(0)[306:irq/223-usbin-s]SMBCHG: src_detect_handler:  chip->usb_present = 0 usb_present = 1 src_detect = 1 hvdcp_3_det_ignore_uv=0
	<6>[ 3224.196281] *(0)[306:irq/223-usbin-s]SMBCHG: handle_usb_insertion: chip->usb_present = 1 Enable USB ID pin 
	<6>[ 3224.196352] *(0)[306:irq/223-usbin-s]SMBCHG: handle_usb_insertion: triggered
	<6>[ 3224.196395] *(0)[306:irq/223-usbin-s]SMBCHG: handle_usb_insertion: inserted type = 4 (SDP)
	<6>[ 3224.196407] *(0)[306:irq/223-usbin-s]SMBCHG: smbchg_change_usb_supply_type: Type 4: setting mA = 100
	<6>[ 3224.196419] *(0)[306:irq/223-usbin-s]SMBCHG: smbchg_set_usb_current_max: USB current_ma = 100
	<6>[ 3224.196612] *(0)[306:irq/223-usbin-s]SMBCHG: smbchg_set_usb_current_max: usb type = 4 current set to 150 mA
	<6>[ 3224.196650] *(0)[306:irq/223-usbin-s]SMBCHG: get_parallel_psy: parallel charger not found
	<6>[ 3224.196823] *(1)[309:irq/227-power-o]SMBCHG: power_ok_handler: triggered: 0x01
	<6>[ 3224.196833] *(0)[306:irq/223-usbin-s]SMBCHG: handle_usb_insertion: setting usb psy present = 1


	这个问题是17G06A后来一批机器上加了ovp，ovp的触发电压是6.8V，但是线上有阻抗，所以实际要超过6.8V，number，
	此时是硬件先断开，断开之后电压肯定上不去，软件在硬件之后，所以造成了，升压的时候电流是降到0,但是没有提示，降压的时候
	有提示，这个是硬件问题，改不了

}












/***************************************************************************************************************************************/
6.关机充电存在充电电流跳变问题
{

	Qcom case:03116844
	{
		1.aicl不是旨在插入充电器的时候检测充电器的能力吗？为什么还要每隔180S检测一次

		---充电器插入的时候会启动AICL 检测，充电过程中硬件也会定时去做AICL rerun ,原因qpnp-smbcharger.txt 已经解释了，硬件AICL 默认是enable，默认rerun时间是180S。

		- qcom,force-aicl-rerun:	A boolean property which upon set will enable the
			AICL rerun by default along with the deglitch time
			configured to long interval (20 ms). Also, specifying
			this property will not adjust the AICL deglitch time
			dynamically for handling the battery over-voltage
			oscillations when the charger is headroom limited.
			
		- qcom,aicl-rerun-period-s	If force-aicl-rerun is on, this property dictates
			how often aicl is reran in seconds. Possible values
			are
			SCHG - 45, 90, 180, and 360.
			SCHG_LITE - 3 (2.8), 6 (5.6), 11 (11.3), 23 (22.5),
			45, 90, 180 and 360

		2.我们17G16A使用的msm8937平台，但是关机充电没有这个问题，这两个平台差不多啊
		---如之前电话沟通，每次的测试结果可能都不同，请多测试几次对比下波形图和数据，这两个平台是没有差异可以多测试几次对比下。
	
	
		qpnp-smbcharger.c:
		+ #define DEBUG
		- static int smbchg_debug_mask;
		+ static int smbchg_debug_mask = 0xFF;

		qpnp-fg.c:
		+ #define DEBUG
		- static int fg_debug_mask;
		+ static int fg_debug_mask = 0xFF; 
	
	
		BoardConfig.mk
		# let charger mode enter suspend
		BOARD_CHARGER_ENABLE_SUSPEND := true

		请check这个宏在你们项目里面定义了，这个是使能关机充电enable suspend的，请检查下 
	}

	
	
	
	具体情况:
	   这个现在的情况是软件设定1A 用5V/1A的充电器，跳变特别频繁，curr-bat跟curr-charger 都有几十次跳变，电流跳变到100mA有的为0mA
	   软件设定1.5A 用5V/1A充电器，跳变情况少一点，大概都是在10次左右

	分析进展：				 高通case ID:03116844 
	  1.暂时没有解决问题，可以稳定复现
	  2.之前认为可能的有电池电容问题，验证修改后不是电容的问题
	  3.高通认为仪器不够精确建议用示波器等更精密的仪器测量，但是如果是仪器问题，开机充电没有跳变，其他项目开机关机也没有这个问题，应该不是仪器精确度的问题
	  主要是关机充电的时候log太少，很难抓到跳变点的log ，charger的log全都打开了
		
	  4.充电器功率不匹配，导致频繁检测充电器能力问题，但是设定1A,用5V/1A的跳变的更多，也不是这个问题
	  这个存在一定的可能性
	  软件			充电器功率
	  关机充电
			1A			5V/1A			跳变很多，而且这个跳变似乎是有一定规律的
							5V/2A			这个恒流充电阶段没有往下跳变
			1.5A		 	5V/1A			跳变少一点，10此左右 	

	  开机充电		
			1A			5V/1A			这个也存在充电电流跳变
	  
	  这个是测出关机充电有充电电流跳变问题，开机充电是用APK测的，测试时间是3s一次，可能没有采集到数据
	  


	  所以说这个很有可能跟检测充电器的能力有关		aicl  reg :ICL_STS
	  {
		  qpnp-smbcharger.c     qpnp-smbcharger.txt这个里面是解释
		  force_aicl_rerun硬件强制默认执行aicl 时间180S
	  
		  
		  这几个关键字包含的代码要熟悉
		  {
			qpnp-smbcharger.c
				  smbchg_change_usb_supply_type 
			  
				  smbchg_default_dcp_icl_ma		这个是设置usbin的电流
				  
			msm8917-pmi8937-qrd-sku5.dtsi	
				  fastchg-current-ma			这个是设置进电池的电流
				 
			  increment_aicl_count		resetting: elp
			  aicl_done_handler: triggered, aicl:		
		  }
		  
		  /*解决办法*/
		  smbchg_default_dcp_icl_ma=1000;这样充电器的能力只有1000mA，不会有充电电流跳变
		  这个是设置USBIN的电流，也就是说如果用能力更大的充电器，也只能冲到1A
		  因为之前软件上USBIN电流2A，而充电器能力又不够，所以一直在检测，而硬件检测会断开充电
		  导致充电电流降到0
	  }
	  
 }












 
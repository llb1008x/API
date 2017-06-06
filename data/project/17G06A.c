/**********************************************************************************
                     金刚3，msm8917
**********************************************************************************/


/*相关文档*/
{
    //msm8937_la_software_user_manual
    这个是介绍软件环境的搭建


    linux_android_pmic_software_drivers_overview
    {
    	pmi8937通过spmi接口跟cpu连接
    	
    	spmi：system power management interface 
    	
    	涉及到的代码文件
    	{
    		fuel gauge 
    			./drivers/power/qpnp-fg.c
    			./drivers/bif/qpnp-bsi.c
    			
    		charger 	
				./drivers/power/qpnp-smbcharger.c
				
			gpio
				./drivers/gpio/qpnp-pin.c
				
			interrupt
				./drivers/platform/msm/spmi/qpnp-int.c

			spmi
				./drivers/spmi/spmi.c
				./drivers/platform/msm/spmi/spmi.c
				
			控制开关机
				./drivers/power/reset/msm-poweroff.c	
    	}
    	
    	中断偏移量 0x10~0x1e
    	
    	pm8937提供的对电压，电流的管理需要基于rpm的系统的支持，关于这个日常管理的驱动源码在
    	./drivers/regulator/qpnp-regulator.c
		./drivers/regulator/rpm-smd-regulator.c
		
		dts文件
		./arch/arm/boot/dts/qcom/msm-pm8937-rpm-regulator.dtsi
		arch/arm/boot/dts/qcom/

		dts相关文件的参数意思
		./Documentation/devicetree/bindings/regulator/qpnp-regulator.txt
		./Documentation/devicetree/bindings/regulator/rpm-smd-regulator.txt
		./Documentation/devicetree/bindings/gpio/qpnp-pin.txt
		
		charger
		
		battery profiles
		
		没读进去还是要看看代码
    	
    }
    
    
    
    linux_android_pmic_fuel_gauge_user_guide
    {
    	要想理解fuel gauge的工作原理需要先理解power management这个文档
    	
    	
    	电量计：
		{
			电量计的算法依赖于库仑计，以电压为基础的技术
    		确保短期的线性变化(也就是说电量短时间变化不能太突然)
    		同时长时间的精确性
    		
    		不需要一个完整的充放电循环来校准电量计的准确度
    		
    		开路电压计算初始值，库仑计的积分算法计算电量的变化	
    		
    		Disable USB on-the-go (OTG) functionality when battery SoC falls below a 		 programmable  threshold
    		
    		电量计的电流通过读取10m欧的采样电阻，读取充进电池的电量多少，电压通过读取adc转换的
    		引脚的值，1500ms读取一次
		
		BAT_ID
			通过BAT_ID检测电池的存在与否，这个检测过程内嵌在fg算法中，重复的升高基础电流（5uA->15uA->150uA）直到发现匹配的
			智能电池调控通常是提供5uA的灌电流去识别，因为5uA可以通过软件去识别然后设置使能标志位
			(Battery Serial Interface (BSI) module)
			
		
		thermal：		
			电池温度检测，有些是将温度检测的电阻嵌入到电池内(NTC电阻)，然后通过一个thermal pin去连接，读取数值	
			thermal 相关参数的设置都是一些十六进制，总要说说是什么意思吧？pm_config_target.c
			这个文件在哪，这个文件在modem侧
			modem_qc_n_qrd8920_cs/BOOT.BF.3.3/boot_images/core/systemdrivers/pmic/config/mdm9x45/pm_config_target.c
			
		ESR(电池等效电阻):
			ESR可以提高电量计的准确性，对其他的也有影响，但是ESR在低电流的状态下是不需要的，而且在系统suspend的时候是无法使能的
			
			
			
		System termination current(系统的截止电流)：
			可以客制化电池100%状态的上报，
			qcom,fg-chg-iterm-ma这个是设置充电截止电流
			这里应该讲的是系统满电的状态，充电电流，电池电压
			

			
		FG termination current（这个使用电量计充电截止电流来判断停止充电的条件）：
		
		
		也就是说充电截止电流可以通过qcom,fg-chg-iterm-ma，或者qcom,chg-term-ua(不用)来设定
		充电截止的条件
		
		
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


			电量计涉及到的代码：
			./msm-3.18/drivers/power/qpnp-fg.c
			./msm-3.18/arch/arm/boot/dts/qcom/msm-pmi895x.dtsi
			./msm-3.18/Documentation/devicetree/bindings/power/qpnp-fg.txt
			
			
			debug
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
			
		}
		
    }
    
    
    
    pmi8937_pmi8940_power_management_ic_device_specification
    {
    
    
    }
    
    
    
    
    


}

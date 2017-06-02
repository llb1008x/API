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
    	电量计：
    		电量计的算法依赖于库仑计，以电压为基础的技术
    		确保短期的线性变化
    		同时长时间的精确性
    		
    		开路电压计算初始值，库仑计的积分算法计算电量的变化	
    }
    
    


}

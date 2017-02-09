/*RT5081*/
{
集成的功能很多
	
	RT5081的几个基本功能：
	switching battery charger ;USB Type-C &&Power Delivery (PD) controller;
	dual Flash LED current sources;a RGB LED driver;a backlight WLED driver;
	a display bias driver and a general LDO for portable devices.

		
	UVP < VDDA <OVP


	MRSTB pin to manually reset the hardware or registers
	这个应该是复位功能
	
	switching charger 有三种模式：
	charge mode, boost mode (OTG-Boost),and high-impedance mode.
	high-impedance mode:高阻抗模式为了降低功耗，产生极低的电流
	
	charge mode :
	Minimum Input Voltage Regulation (MIVR):最小输入电压的控制
	为了阻止输入电压因充电器供电能力不足而下降






	   Strobe Mode ：选通模式

	
	   BMC Power Delivery protocol



	   CHG_VMID：从这个引脚提供电流
	
	   0.1A到3.25A的输入电流管理

	   OTG输出电压4.42V到5.58V，输出电流2A

	   电池电压在2.3V到3.8V低电保护，会启动升压操作

	   17V，21V，25V，29V四档过压保护

	   900mA，1200mA,1500mA,1800mA四档过流保护


代码：
	/home/llb/project/PRO/source/17G05A/L30_6757_17G05A_N0.MP5_161227_ALPS/android_mtk_6757_mp/kernel-4.4/drivers/misc/mediatek/pmic/rt5081/
	
	

	
   
}

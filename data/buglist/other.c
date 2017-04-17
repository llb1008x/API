/*************************************************************************************
							其他相关的问题
*************************************************************************************/
{


老化测试中出现的问题(老化测试方案本来就有问题)
{
--->老化测试的一些状况：大量刚出厂的机器，大批的放到一起，不断的跑一些case，然后在卖给客户...

	老化测试的一些调整措施：
		1.空间环境温度问题，大量的机器放到一起，如果产生的热量没有散去，容易积累热量导致空间整体温度升高，调整测试机器之间的距离
		需要知道当时环境的温度，以及他们直接的温度，

		2.电量较低的时候充电电流较大，产生的热量也很大，如果是满电，充电电流很小，产生不了多少热量

		3.



--->meta跟工厂写号，校准modem信号相关的
	meta写号错误是fuelgauge电量计服务没有初始化

	上层要给底层传达命令，但是fuelgauge没有初始化，就使用默认的电池参数，电量为-1，导致写号错误。

	手机写号？

	+#Gionee GuoJianqiu 20151022 modify for CR01570292 begin
	+service fuelgauged /system/bin/fuelgauged
	+    class main
	+#Gionee GuoJianqiu 20151022 modify for CR01570292 end



--->底层跟上层电量差别太大的问题
	售后问题机，bug#75833，这个问题有必要总结还有case ALPS03218062
	底层跟上层显示偏大，差不多，偏小
	1. 多次连续开关机或者recovery，导致电量跳变。
	– 这是因为电池的特性，电池恢复回原始电压需要30min.
	– 多次开关机会造成电池电压一直再下降，而电量会在头几次采用RTC的数值，当误差超过default 40%，就会产生跳变。

	2. 更换电池，手机显示电量不变。具体现象可以是：
	A电池在A手机上面显示50%，B电池在B手机上面显示70%. A电池在B手机上面显示70%.
	– 这是因为RTC记忆功能与更换新电池后容量变化不大的矛盾引起的。
	– 如果打开RTC功能，更换电池以后若电池容量在RTC偏差范围内则不会显示新电池容量，造成更换新电池D0偏差较大.
	– 只要是手机端的gauge方案，电池插拔就会有这些变化限制, 只能看电池变化的最大值去调整default的40%.
	– 一般原则，修改RTC default数值越小，越可以避免该问题.

	3. 动态测试手机, 特别大电流耗电到某一电量，此时突然关机或者扣电池，重新开机显示电量误差大。
	– 这是由于电池特性，电池恢复回原始电压需要30min. 短时间内大电流抽电，造成电池瞬间被拉低。再次开机，电量超过default的40%。
	– Software gauge(软件电量计)对于误差适应性很强，都会因为闭路回路系统而收敛误差，每次的计算就是在tracking. 短时间内的误差，会在后续使用中，慢慢校正过来。

}




GNSPR#52112 MMI充电测试失败
	1.现象：
	a.充电测试失败
	无论是交流充电还是usb，充电电压是在增加，但是电流是负，且电量越来越少。
	b.充电芯片是正常工作的，起码充进去了电，但是充的比耗的多
	

	2.可能的原因：
	a.软件：pmic未工作，或者未挂上i2c总线
	b.硬件：电源管理芯片出现问题，或内部硬件短路，造成较大的功耗。
	c.我理解的可能原因，电池对外放电（电流为负）

	原因：充电芯片没有焊好


GNSPR#55817 MMI充电测试失败

	1.现象：
		机器插上充电器或者USB充电电流为负


	2.可能的原因：
		a.跟上次类似充电芯片未正常工作
		b.根据log,fg_coulomb的数值，i2c传输数据有问题 
			c.充电芯片未正常工作，或者充电接口出现问题（未充电时电流等其他都是正常的）

	原因：又是充电芯片没焊好导致的充电芯片没有正常工作





待解决的问题
   /*a.明确打印出来的那几个物理量的意思，充放电过程什么变化情况。*/
log:
	bat_routine_thr][kernel]AvgVbat 3806,bat_vol 3749, AvgI 0, I 0, VChr 0, AvgT 29, T 30, ZCV 3864, CHR_Type 0, SOC  54: 54: 54

	bat_routine_thr][kernel]AvgVbat 3804,bat_vol 3725, AvgI 505, I -775, VChr 5137, AvgT 29, T 29, ZCV 3864, CHR_Type 1, SOC  54: 54: 54

	bat_routine_thr][kernel]AvgVbat 3800,bat_vol 3694, AvgI 465, I -655, VChr 5137, AvgT 29, T 30, ZCV 3864, CHR_Type 1, SOC  54: 54: 54

	bat_routine_thr][kernel]AvgVbat 3796,bat_vol 3718, AvgI 427, I -586, VChr 5137, AvgT 29, T 29, ZCV 3864, CHR_Type 1, SOC  54: 54: 54

	bat_routine_thr][kernel]AvgVbat 3793,bat_vol 3707, AvgI 385, I -706, VChr 5128, AvgT 29, T 29, ZCV 3864, CHR_Type 1, SOC  54: 54: 54

	bat_routine_thr][kernel]AvgVbat 3788,bat_vol 3675, AvgI 344, I -706, VChr 5128, AvgT 29, T 30, ZCV 3864, CHR_Type 1, SOC  54: 54: 54

	bat_routine_thr][kernel]AvgVbat 3784,bat_vol 3691, AvgI 306, I -586, VChr 5128, AvgT 29, T 31, ZCV 3864, CHR_Type 1, SOC  54: 54: 54

	bat_routine_thr][kernel]AvgVbat 3781,bat_vol 3726, AvgI 264, I -706, VChr 5137, AvgT 29, T 32, ZCV 3864, CHR_Type 1, SOC  54: 54: 54

	bat_routine_thr][kernel]AvgVbat 3778,bat_vol 3717, AvgI 225, I -620, VChr 5516, AvgT 29, T 32, ZCV 3864, CHR_Type 1, SOC  54: 54: 54

	bat_routine_thr][kernel]AvgVbat 3775,bat_vol 3716, AvgI 184, I -689, VChr 5128, AvgT 29, T 32, ZCV 3864, CHR_Type 1, SOC  54: 54: 54

	bat_routine_thr][kernel]AvgVbat 3772,bat_vol 3719, AvgI 142, I -689, VChr 5128, AvgT 29, T 32, ZCV 3864, CHR_Type 1, SOC  54: 54: 54

	bat_routine_thr][kernel]AvgVbat 3768,bat_vol 3702, AvgI 102, I -655, VChr 5128, AvgT 29, T 32, ZCV 3864, CHR_Type 1, SOC  54: 54: 54

	bat_routine_thr][kernel]AvgVbat 3765,bat_vol 3730, AvgI 65, I -568, VChr 5610, AvgT 29, T 32, ZCV 3864, CHR_Type 1, SOC  54: 54: 54


battery_log(BAT_LOG_CRTI,
	    "[kernel]AvgVbat %d,bat_vol %d, AvgI %d, I %d, VChr %d, AvgT %d, T %d, ZCV %d, CHR_Type %d, SOC %3d:%3d:%3d\n",
	    BMT_status.bat_vol, bat_vol, BMT_status.ICharging, ICharging,
	    BMT_status.charger_vol, BMT_status.temperature, temperature, BMT_status.ZCV,
	    BMT_status.charger_type, BMT_status.SOC, BMT_status.UI_SOC, BMT_status.UI_SOC2);


AvgVbat ：电池的平均电压 
	ret = battery_meter_ctrl(BATTERY_METER_CMD_GET_ADC_V_BAT_SENSE, &val)--->BMT_status.bat_vol =mt_battery_average_method(BATTERY_AVG_VOLT, 
		&batteryVoltageBuffer[0], ZCV, &bat_sum, batteryIndex);

bat_vol：电池的电压 ret = battery_meter_ctrl(BATTERY_METER_CMD_GET_ADC_V_BAT_SENSE, &val)

AvgI:平均充电电流（跟上面相似）

I   ：充电电流，以前是通过检测充电引脚正负极参数，做适当的补偿，算出充电的电流，现在是通过充电芯片，IC算出电流，然后读取寄存器的值

电压，电流的变化跟之前的充电过程一样，涓流->恒流->恒压->充满--->检测是否小于回充电压，是否打电话等条件，在判断是否要充电，充电还是放电主要看充电器电压，然后电池电压，
电流的变化，温度是否正常

VChr	：充电器的电压

AvgT	：电池的平均温度

T	：电池的温度

ZCV	：电池当前的开路电压，这个开路电压跟当前的温度有关，导入的温度曲线，通过线性插值法找到具体的温度对应的电压

CHR_Type：充电接口的类型

SOC	:底层的电量

UI_SOC	:上层的电量

   force_get_tbat()---》BattVoltToTemp（）上面的那几个运算，什么do_div。。。64位的除法    	


   /*b.pmic工作的过程，如何确定i2c是否正常工作*/
   安装了i2c_tools，还有什么dump register?上面有一个打印pmic寄存器的log，正常打印，i2c设备应该正常。



   /*c.充电升降压调节的控制具体怎么控制的，双充电芯片？*/
定义的输入输出电流开关的宏
CONFIG_MTK_SWITCH_INPUT_OUTPUT_CURRENT_SUPPORT

定义的开关标志位g_bcct_flag    g_bcct_input_flag，好像跟温度有关的

快充部分有相应的升压调节，快充部分的升压通过充电适配器升高电压，而快充的通信好像是循环升压，看是否变化，变化了，证明连接上了，多了次数的判断，满足条件，可以快充。
mtk_ta_retry_increase()->mtk_ta_increase()->control的那个宏，set_ta_current将电压逐渐升到12

而普通的升压跟充电模式相关，应该在各种模式中，调度算法里 select_charging_current()设置充电电流，涓流->恒流->恒压->充满--->检测是否小于回充电压，
是否打电话等条件，在判断是否要充电，充电还是放电主要看充电器电压，然后电池电压，电流的变化，温度是否正常

基于输入电压的动态电源管理 (VIN-DPM)：电源和充电器之间的电阻会阻碍充电器从电源获得最大功率，导致电源电压陡降，造成充电器欠压闭锁。
已经有了用来计算充电器所需最小电源电压的方程式，其可计算既定电源适配器的最大充电电流。此外， VIN-DPM 特性还能动态地降低充电器的输入电流限值，避免适配器电压陡降，
因而允许使用多种类型的适配器和/或电源连接

双充电芯片：
两个充电芯片同时为电池工作，可以降低充电的温度（热量分散到两个芯片上）
定义的这个宏：CONFIG_MTK_DUAL_INPUT_CHARGER_SUPPORT



   /*d.反向充电，升压，USB，otg*/

当设备检测到USB_ID信号为低时，表该设备应作为Host（主机，也称A设备）用。
当设备检测到USB_ID信号为高时，表示该设备作为Slave(外设，也称B设备）用。

switch_int_to_device   switch_int_to_host  ，根据变量的值判断进入device还是host，而这个值是是读取寄存器的值，iddig
pinctrl_select_state控制gpio引脚的状态，
反向充电应该跟USB_OTG有关，当检测到USB 的id引脚为0，otg模式，触发中断。当然逻辑好像改了，如果是OTG充电模式，上层要点击确定，使OTG中断使能，才可以反向充电。

is_enter_mmi_test 反向充电测试开关


开机充电，关机充电 ，低电量保护？

正常开机充电，一直再看。

关机充电：

       关机时充电器检测代码在lk部分，当接入充电器时，PMIC会通过ADC采样，检测Vchrg（charger/usb电源）电压，
判断Vchrg的值如果在这个区间：4.4v~6.5v，系统就会开始进入充电过程，这个过程包括show low battery logo，接着
显示充电电量logo。
相关代码：
mt6572/mediatek/platform/mt6572/lk/platform.c

查看函数void platform_init(void)：

充电器检测：
充电过程中，upmu_is_chr_det() 检测Vchrg状态，返回KAL_FALSE则power off。
#ifdef MTK_KERNEL_POWER_OFF_CHARGING
    if((g_boot_arg->boot_reason == BR_USB) && (upmu_is_chr_det() == KAL_FALSE))
    {
        printf("[%s] Unplugged Charger/Usb between Pre-loader and Uboot in Kernel Charging Mode, Power Off \n", __func__);
        mt6575_power_off();
    }
#endif

显示LCM背光：
#ifndef DISABLE_FOR_BRING_UP
    mt65xx_backlight_on(); //[TODO] workaround
#endif

boot模式选择，用于区分开机过程。
boot_mode_select();

显示low battery logo，点亮充电指示灯：

        if(kernel_charging_boot() == 1) //关机充电状态
        {
                mt_disp_power(TRUE);
                mt_disp_show_low_battery();
                mt_disp_wait_idle();
#ifndef DISABLE_FOR_BRING_UP
                mt65xx_leds_brightness_set(6, 110);
#endif
        }

kernel_charging_boot定义：
#if defined (MTK_KERNEL_POWER_OFF_CHARGING)
int kernel_charging_boot(void)
{
        if((g_boot_mode == KERNEL_POWER_OFF_CHARGING_BOOT || g_boot_mode == LOW_POWER_OFF_CHARGING_BOOT) && upmu_is_chr_det() == KAL_TRUE)
        {
                printf("[%s] Kernel Power Off Charging with Charger/Usb \n", __func__);
                return  1;
        }
        else if((g_boot_mode == KERNEL_POWER_OFF_CHARGING_BOOT || g_boot_mode == LOW_POWER_OFF_CHARGING_BOOT) && upmu_is_chr_det() == KAL_FALSE)
        {
                printf("[%s] Kernel Power Off Charging without Charger/Usb \n", __func__);
                return -1;
        }
        else
                return 0;
}
#endif


   /*e.分析那个电池曲线，出现的什么问题，自己想（虽然不是我解决的）*/
充电时间有点长

	

   /*f.ZCV电池曲线，导入，操作*/
	battery_profile_tX 几个数组

   /*g.充电类型的判断*/
	
   pmic和USB都要ready,PMU检测充电端是否正常工作


SS：高优先级--->温度的调控，处理算法，  h.thermal相关的内容还是不太清楚


待机状态下有些会定期唤醒
	1.modem不太了解，经常唤醒，耗电很大
	2.1800s定期唤醒（30min），防睡死，1%检测中断

充电状态下BAT_thread几个线程10s唤醒一次















关机和重启的两种方式
--->按powerkey弹框选择重启还是关机，搜AmigoGlobalActions
这个是从上层发送指令reboot还是shutdown广播-->ShutdownThread: reboot,这个线程负责关闭很多service
这个是上层发的一个全局性的广播,AmigoGlobalActions



AmigoGlobalActions: onReceive:reason  homekey mViewState REBOOT

[  477.443918] <0>.(0)[54:pmic_thread][name:kpd&]kpd: Power Key generate, pressed=1 



--->长按powerkey达到一定的时间然后有软件上的关机和硬件上的断电关机












}


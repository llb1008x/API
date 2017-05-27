





17G05A  软件关机重启,充电调试 (这个是重点),17G05A的mmi测试
/*debug*/
{
软件关机重启:解决
{
    pwrkey连接了pmic和rt5081，pmic关闭了使能，但是rt5081还有连接
    在long_press_reboot_function_init_pmic内

//Gionee <gn_by_charging> <lilubao> <20170406> add for change reboot begin
    //try again to make sure the reg 0x60e set to 0x00
	ret=pmic_set_register_value(KPD_PMIC_RG_PWRKEY_RST_EN, 0x00);
	kpd_info("4ret->%d\n",ret);
	ret=pmic_set_register_value(KPD_PMIC_RG_HOMEKEY_RST_EN, 0x00);
	kpd_info("5ret->%d\n",ret);

	kpd_info("[PMIC] Reg[0x%x]=0x%x after by lilubao\n",
			MT6355_TOP_RST_MISC, upmu_get_reg_value(MT6355_TOP_RST_MISC)); 
//Gionee <gn_by_charging> <lilubao> <20170406> add for change reboot end	

}


充电电流太小：标准充电器（1.9A） USB充电（500mA）：接近设定的值
{
->2017.4.14
    {
         标准充电器（1.6A） USB充电（450mA）
         修改了：
         MTK_PUMP_EXPRESS_PLUS_20_SUPPORT = yes
         mivr = <4400000>;	/* uV 4500000->4400000*/
    }

->2017.4.16
    {
        根据MTK最新的RT5081版本做了修改
        电流还是有问题，比较小
    }    

->2017.4.18
    {
        修改mivr或者根据充电器类型设置充电电流可以提高充电电流
        USB充电电流可以达到450+mA但是标准充电器还是有问题

        //Gionee <gn_by_charging> <lilubao> <20170414> add for modify charging begin
        mivr = <4400000>;	/* uV 4500000->4400000*/
        //Gionee <gn_by_charging> <lilubao> <20170414> add for modify charging end

        //Gionee <gn_by_charging> <lilubao> <20170418> add for charging change begin	
        /*chg_data->aicr_limit = aicr;
        dev_info(chg_data->dev, "%s: OK, aicr upper bound = %dmA\n", __func__,
            aicr / 1000);
        */	
        //force setting aicr_limit
        chg_data->aicr_limit=2050000;
        //Gionee <gn_by_charging> <lilubao> <20170418> add for charging change end
        这样做是强制设定aicr_limit避免将最开始尝试的最大电流设为限定值
    }

}






键盘
{
->2017.4.25
    {
        修改了keyboard的I2C的设备地址跟挂的总线
        //Gionee <gn_by_charging> <lilubao> <20170425> add for change keypad begin
        #define AW9523B_DEV_NAME         "aw9523b"
        #define AW9523B_I2C_NUM		4		// 3->4
        #define AW9523B_I2C_ADDRESS 0x58	// 0xB0 -> 0x58
        //Gionee <gn_by_charging> <lilubao> <20170425> add for change keypad end

        因为framework层还有部分未导入，所以检测到一些未知的键值，出现异常

    }
}






马达效果
{
     
    2017.4.27 	    ok
    {
     开机震动 
        //Gionee <gn_by_charging> <lilubao> <20170427> add for change vibrate begin
        mdelay(500);			// 100->500
        //Gionee <gn_by_charging> <lilubao> <20170427> add for change vibrate end
    }


    2017.5.2        
    {
        写一个测试程序调用/dev/DRV2604L这个节点，
        read，write，ioctl，写进去正确的数据，产生有效的效果
        然后写一个ioctl函数 

        ioctl函数还没有写
    }


    2017.5.3
    {
       ->插入充电器，进行充电器检测时候会有震动
        震动使能,插入充电器，震动模式   
        -> 这个是上层调用的
    }
      
    2017.5.4
    {
        通过ioctl函数可以通过测试程序，直接调用不同的work_mode产生效果
        vibrator_enable这个是最简单直接的只要调用然后写一个value就能震动
        drv2604l_change_mode这个是调用马达震动都要调用的函数

        1.vibrator提供了两套mtk自带的和第三方的
        CONFIG_GN_BSP_MTK_VIBRATOR_DRV2604L
        CONFIG_MTK_VIBRATOR

        2.lk和kernel有两套代码
        lk阶段
        知道这个函数干了什么：就是让马达震动一会然后关闭
        void gn_lk_vibrate(void)

        kernel阶段
        1.通过write写值进入不同的case，这个就要弄清写什么样的值，如何正确的写进去
        2.或者在原有函数基础上调用自己写的一个函数，ioctl
        ioctl通过写进不同的cmd，传进值，在不同的case里面调用不同的效果


        //Gionee <gn_by_charging> <lilubao> <20170504> add for change vibrate begin
        #define MOTOR_TEST
        #if defined(MOTOR_TEST)

        long dev2604_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
  
}












充电过程中，根据温度调节充电电流
{
    1.根据当前的温度简单的限定充电电流
    {
        低温（小于15）：1.5A
        正常温度（25左右）：2A
        高温（大于45）：1A

        mtk_switch_charging.c -> swchg_select_charging_current_limit

        这其中还要考虑其他情况：打电话，mmi测试等等
    }
 

    G1605A
    //Gionee GuoJianqiu 20150514 modify for GNNCR00010646 begin
    if(bat_charge_current_not_down())
    {
        g_temp_CC_value = AC_CHARGER_CURRENT;
        
        //GioneeDrv GuoJianqiu 20160429 modify for platform change begin
        if((BMT_status.temperature < 15) || ((g_call_state != CALL_IDLE) && (!is_enter_mmi_test)))
        {
            g_temp_CC_value = AC_CHARGER_CURRENT / 2;
            if(g_temp_CC_value < USB_CHARGER_CURRENT)
                g_temp_CC_value = USB_CHARGER_CURRENT; 
            battery_log(BAT_LOG_CRTI, "[BATTERY] temperature = %d, g_call_state = %d, is_enter_mmi_test = %d, g_temp_CC_value = %d\n", BMT_status.temperature, g_call_state, is_enter_mmi_test, g_temp_CC_value); 
        }
        //GioneeDrv GuoJianqiu 20160429 modify for platform change end
        
    }
    //Gionee GuoJianqiu 20150514 modify for GNNCR00010646 end


     //Gionee GuoJianqiu 20150611 modify for CR01498036 begin
     kal_bool bat_charge_current_not_down(void)
     {
        battery_log(BAT_LOG_CRTI, "[BATTERY] bat_charge_current_not_down, g_gn_screenon_time=%d, g_call_state=%d, is_enter_mmi_test=%d, g_boot_mode=%d !\n\r", g_gn_screenon_time, g_call_state, is_enter_mmi_test, get_boot_mode());    

        if (((g_gn_screenon_time <= 30) && (g_call_state == CALL_IDLE)) || (is_enter_mmi_test == KAL_TRUE) || (get_boot_mode() == KERNEL_POWER_OFF_CHARGING_BOOT || get_boot_mode() == LOW_POWER_OFF_CHARGING_BOOT))
            return true;
        else
            return false;
     }
     //Gionee GuoJianqiu 20150611 modify for CR01498036 end


     17G05A
     //Gionee <gn_by_charging> <lilubao> <20170519> add for thermal charging begin
	 pr_err("in %s info->battery_temperature->%d\n",__FUNCTION__,info->battery_temperature);

	 if(info->chr_type==STANDARD_CHARGER){

		if( info->battery_temperature <=15 ){

			pdata->charging_current_limit=1500000;
			pdata->input_current_limit=1600000;
			pr_err("in %s ,temperature is too low ,we need limit charging current->1\n",__FUNCTION__);
		}else if( (info->battery_temperature>15)&&(info->battery_temperature<=45) ){

			pr_err("in %s ,temperature in normal range,do not limit charging current->2 \n",__FUNCTION__);
		}else if( info->battery_temperature > 45 ){

			pdata->charging_current_limit=1000000;
			pdata->input_current_limit=1200000;
			pr_err("in %s ,temperature is too high ,we need limit charging current->3\n",__FUNCTION__);
		}
	 }
	 pr_err("in %s setting charging_current_limit->%d,input_current_limit->%d\n",
				__FUNCTION__,pdata->charging_current_limit,pdata->input_current_limit);
	 //Gionee <gn_by_charging> <lilubao> <20170519> add for thermal charging end	




    2.充电温度调节策略相关代码，文档  
    {
        mtk_cooler_bcct_v1.c这个文件应该充电调节温度有关


        bcct,abcct
    }  
        


    3.充电温度调节调用流程 
    {
        （mtk_thermal_monitor.c） mtkthermal_init，mtk thermal相关的初始化，创建调试调用节点/proc/driver/thermal，在这个目录下

      建立一系列的proc 节点，/proc/mtkcooler这个目录下是降温策略的设备节点 -> (mtk_cooler_shutdown.c) mtk_cooler_shutdown_init

      这个应该跟过温启动关机策略相关的，温度过高触发条件，关机策略应该是最直接，最有效的，但是影响很大，后面还有几个相关的模块的初始化，都是在proc目录下

      创建一系列节点，然后注册函数，相关的操作函数指针 -> (mtk_ts_cpu.c) tscpu_init注册驱动,这个对整个系统的温升有很大影响，tscpu是一个虚拟的thermal_zone，
      
      主要是监控cpu的状态 -> tscpu_thermal_probe   


    }   


    4.相关的文档
    {
        Thermal_Management_MT6757.pdf

        MT6757CH(CD)_Thermal_Design_Notices_V0.1.pdf
    }

}






/*
    将电池维护代码移植到17G05A上
*/
{
    mtk_switch_charging_run充电状态机的调整
    mtk_switch_charging.c

    创建节点
    mtk_battery.c
    init.mt6757.rc

    mtk_charger.c


    battery_get_bat_uisoc
    FG_status.ui_soc
    
}







modem_introduction
{

    文档ccnmi

    01-01 00:05:44.142   359   359 I ccci_mdinit(1): wait_for_property() ready for name:init.svc.gsm0710muxd, loop:18
    01-01 00:05:44.142   359   359 D ccci_mdinit(1): deamon begin to run!


    Dear Customer:

    一直报下面的AT command:
    网络状态有变化的时候上报（比如rat change）

    是否容易复现？是否一直是在同一个地方?
    如果容易复现，请帮忙打开全部的log，再捉给我

    jinhao wu
    65440
    Thanks

    1.1 Unsolicited result code: +EGMSS
    1.1.1 Description
    This URC is used to inform AP the information of system selection, according to RAT, location and attach status
    1.1.2 Format

    Unsolicited result code
    +EGMSS=<rat>,”<mcc>”,<status>
    1.1.3 Field
    <rat>: Integer
    0 3GPP2 RAT group
    1 3GPP RAT group
    2 CDMA2000 1x
    3 CDMA2000 HRPD
    4 GERAN
    5 UTRAN
    6 EUTRAN

    <mcc> String
    For example, “460” for CT.

    <status> Integer
    0 Have selected the <rat> and is going to perform PLMN search
    1 Attached to the network on the <rat>

    网络有点问题：
    05-15 05:44:15.088 938 954 D AT : AT< +EGMSS: 6,"460",0
    05-15 05:44:15.686 938 954 D AT : AT< +EGMSS: 6,"460",1
    05-15 05:47:08.126 938 954 D AT : AT< +EGMSS: 6,"460",0
    05-15 05:47:08.666 938 954 D AT : AT< +EGMSS: 6,"460",1
    05-15 05:49:21.136 938 954 D AT : AT< +EGMSS: 6,"460",0
    05-15 05:49:21.654 938 954 D AT : AT< +EGMSS: 6,"460",1
    05-15 05:50:15.086 938 954 D AT : AT< +EGMSS: 6,"460",0
    05-15 05:50:15.705 938 954 D AT : AT< +EGMSS: 6,"460",1
    05-15 06:23:32.126 938 954 D AT : AT< +EGMSS: 6,"460",0
}








关掉OTG功能
    不注册OTG这个中断





mmi测试：加入充电测试选项
{
    已加入项目中，当是其中的一些内容还有需要了解工作方式

}





电池曲线的导入    



}






G1605A  售后问题，功耗问题 ， 电池维护特性，healthd线程
/*debug*/
{
    售后问题：   电量计不准的机器
    这个还不知道怎么看，只是觉得电量跟电池电压偏差有点大，而且底层电量跟上层显示的电量差别也很大



    apk发包次数，谁发的(主从)，就是链接到哪了？？




    电池维护特性：
        机器有问题了，tp不好使怎么办，万一还不了QAQ
        正在验证

    

}


















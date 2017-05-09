





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
        {
            struct DRV2604L_data *pDrv2604ldata = (struct DRV2604L_data *)filp->private_data;
            int value=(int )arg;
            int cnt=10;
            
            printk(KERN_ERR"in %s before by lilubao\n", __FUNCTION__);
            
            switch(cmd)
            {
            // test vibrator_enable	
            case 1:
                printk(KERN_ERR"in %s case 1 vibrator_enable ,value->%d\n", __FUNCTION__,value);
                while(cnt--){

                    vibrator_enable( &(pDrv2604ldata->to_dev),value );
                    mdelay(500);
                    vibrator_enable( &(pDrv2604ldata->to_dev),0);
                    mdelay(300);
                }

                break;
                
            // test drv2604l_change_mode
            case 2:
                printk(KERN_ERR"in %s case 2 enable drv2604l_change_mode ,value->%d\n", __FUNCTION__,value);
                
                pDrv2604ldata->should_stop = YES;
                hrtimer_cancel(&pDrv2604ldata->timer);
                cancel_work_sync(&pDrv2604ldata->vibrator_work);
                
                mutex_lock(&pDrv2604ldata->lock);
                
                drv2604l_stop(pDrv2604ldata);
                
                //wake_lock(&pDrv2604ldata->wklock);
                
                drv2604l_change_mode(pDrv2604ldata, WORK_VIBRATOR, DEV_READY);
                pDrv2604ldata->vibrator_is_playing = YES;
                switch_set_state(&pDrv2604ldata->sw_dev, SW_STATE_RTP_PLAYBACK);
                
                value = (value>MAX_TIMEOUT)?MAX_TIMEOUT:value;
                hrtimer_start(&pDrv2604ldata->timer, ns_to_ktime((u64)value * NSEC_PER_MSEC), HRTIMER_MODE_REL);
                schedule_work(&pDrv2604ldata->vibrator_work);
                
                mutex_unlock(&pDrv2604ldata->lock);

                break;
                
            // test vibrator_off
            case 3:
                printk(KERN_ERR"in %s case 3 enable vibrator_off ,value->%d\n", __FUNCTION__,value);
                vibrator_off(pDrv2604ldata);
                break;

            // test play_effect
            case 4:
                printk(KERN_ERR"in %s case 4 enable play_effect ,value->%d\n", __FUNCTION__,value);
                play_effect(pDrv2604ldata);
                break;
                
            // test play_Pattern_RTP
            case 5:	
                printk(KERN_ERR"in %s case 5 enable play_Pattern_RTP ,value->%d\n", __FUNCTION__,value);
                play_Pattern_RTP(pDrv2604ldata);
                break;

            // test play_Seq_RTP
            case 6:
                printk(KERN_ERR"in %s case 6 enable play_Seq_RTP ,value->%d\n", __FUNCTION__,value);
                play_Seq_RTP(pDrv2604ldata);
                break;

            // test drv2604l_change_mode	
            case 7:
                printk(KERN_ERR"in %s case 7 enable drv2604l_change_mode ,value->%d\n", __FUNCTION__,value);
                
                pDrv2604ldata->should_stop = YES;
                hrtimer_cancel(&pDrv2604ldata->timer);
                cancel_work_sync(&pDrv2604ldata->vibrator_work);
                
                mutex_lock(&pDrv2604ldata->lock);
                
                drv2604l_stop(pDrv2604ldata);
                
                //wake_lock(&pDrv2604ldata->wklock);
                
                drv2604l_change_mode(pDrv2604ldata, WORK_VIBRATOR, DEV_READY);
                pDrv2604ldata->vibrator_is_playing = YES;
                switch_set_state(&pDrv2604ldata->sw_dev, SW_STATE_RTP_PLAYBACK);
                
                value = (value>MAX_TIMEOUT)?MAX_TIMEOUT:value;
                hrtimer_start(&pDrv2604ldata->timer, ns_to_ktime((u64)value * NSEC_PER_MSEC), HRTIMER_MODE_REL);
                schedule_work(&pDrv2604ldata->vibrator_work);
                
                mutex_unlock(&pDrv2604ldata->lock);

                break;	

            // test drv2604l_change_mode	for work_mode
            case 8:
                printk(KERN_ERR"in %s case 8 enable drv2604l_change_mode for work_mode,value->%d\n", __FUNCTION__,value);
                cnt=10;
                drv2604l_change_mode(pDrv2604ldata, value, DEV_READY);
                value = MAX_TIMEOUT;
                hrtimer_start(&pDrv2604ldata->timer, ns_to_ktime((u64)value * NSEC_PER_MSEC), HRTIMER_MODE_REL);
                schedule_work(&pDrv2604ldata->vibrator_work);
                break;

            default:
                printk(KERN_ERR"in %s default no cmd\n", __FUNCTION__);
                break;
                
            }

            mutex_unlock(&pDrv2604ldata->lock);
            printk(KERN_ERR"in %s after by lilubao\n", __FUNCTION__);

            return 0;
        }

        #endif
        //Gionee <gn_by_charging> <lilubao> <20170504> add for change vibrate end
    }  
}



调节充电时序状态
{
    2017.5.5
    {
        当前完整的重放电所需要的时间，以及电流电压的变化，以及对应三个状态
        这几个参数可能在头文件中

        但是现在的问题是充电过程中容易关机，找到关机的原因
        BOOT_REASON: 0
        BOOT_MODE: 0

    }






    //==============================================================================
    // PMIC define
    //==============================================================================
    typedef enum {
        AUXADC_LIST_BATADC,     //检测电池
        AUXADC_LIST_VCDT,       //VCDT：充电电压检测脚
        AUXADC_LIST_BATTEMP,    //电池温度
        AUXADC_LIST_BATID,      //battery ID
        AUXADC_LIST_VBIF,       //应该是跟USB相关的
        AUXADC_LIST_CHIP_TEMP,  //芯片温度
        AUXADC_LIST_DCXO,       // 数字温度补偿晶体震荡器
        AUXADC_LIST_ACCDET,     //耳机检测
        AUXADC_LISTTSX,        
        AUXADC_LIST_HP,
    } AUXADC_LIST;
    auxadc定义的采样channel


    G1605A上电池相关参数在battery_common.h mt_battery_meter.h上，但是17G05A这些参数在哪？
    mtk_charging.h，mtk_battery_meter.h


    /*****************************************************************************************************
    现在的问题是恒流充电的电流太小，0%关机电压太高，开机充电过程中会出现关机的情况，电量计系数要等电池曲线,关机充电是否有问题
    弄清楚battery_probe这个里面几个中断回调函数和创建的线程想干什么？
    马达刷机第一次震动太弱，之后震动正常
    ******************************************************************************************************/
    0%关机电压太高
    {
        这个应该需要修改电量0%参数
        mtk_battery_meter.h ，mtk_battery_property.h

        mtk_battery_property.h这个里面包含很多电池电量计相关的参数

        shutdown_event_handler做了哪些事：
        {   
        1.执行关机的回调函数，判断一些关机条件是否满足或触发然后执行kernel_power_off -> machine_power_off
        mtk_power_misc_init     power_misc      power_misc_thread


        2.会创建一个power_misc_thread线程，这个进程应该是在低电条件下创建的，低电出发了条件，高电的时候没有这个
        这个是关机的条件 
            struct shutdown_condition {
                bool is_overheat;
                bool is_soc_zero_percent;
                bool is_uisoc_one_percent;
                bool is_under_shutdown_voltage;
                bool is_dlpt_shutdown;
            };

            /* ============================================================ */
            /* power misc related */
            /* ============================================================ */
            #define BAT_VOLTAGE_LOW_BOUND 3400
            #define BAT_VOLTAGE_HIGH_BOUND 3450
            #define SHUTDOWN_TIME 40
            #define AVGVBAT_ARRAY_SIZE 30
            #define INIT_VOLTAGE 3450
            #define BATTERY_SHUTDOWN_TEMPERATURE 60

        3.shutdown_event_handler  
            这里面都是对那些关机条件进行判断，符合条件的轮训一段时间然后判断是否大于SHUTDOWN_TIME
            符合条件就  kernel_power_off

        4.main.log里面搜索ShutdownThread，上层传达的关机流程
        01-01 07:43:19.967753  2009  2460 V MediaScannerService: handleShutdownThreadpool()
        01-01 07:57:58.074049   962  1080 D ShutdownThread: !!! Request to shutdown !!!
        01-01 07:57:58.074354   962  1080 D ShutdownThread:     |----com.android.server.power.ShutdownThread.shutdown(ShutdownThread.java:201)
        01-01 07:57:58.074429   962  1080 D ShutdownThread:     |----com.android.server.power.PowerManagerService$9.run(PowerManagerService.java:2731)
        01-01 07:57:58.074473   962  1080 D ShutdownThread:     |----android.os.Handler.handleCallback(Handler.java:836)
        01-01 07:57:58.074521   962  1080 D ShutdownThread:     |----android.os.Handler.dispatchMessage(Handler.java:103)
        01-01 07:57:58.074551   962  1080 D ShutdownThread:     |----android.os.Looper.loop(Looper.java:203)
        01-01 07:57:58.074596   962  1080 D ShutdownThread:     |----android.os.HandlerThread.run(HandlerThread.java:61)
        01-01 07:57:58.074639   962  1080 D ShutdownThread:     |----com.android.server.ServiceThread.run(ServiceThread.java:46)
        01-01 07:57:58.074789   962  1080 D ShutdownThread: Notifying thread to start shutdown longPressBehavior=1
        01-01 07:57:58.079296   962  1080 D ShutdownThread: IBootAnim get screenTurnOffTime : 5000
        01-01 07:57:58.134895   962  3204 D ShutdownThread: ro.crypto.state: unencrypted
        01-01 07:57:58.137951   962  3204 D ShutdownThread: checkShutdownFlow: IPO_Support=true mReboot=false sys.ipo.disable= isSafeMode=false passEncryptionCondition=true
        01-01 07:57:58.138904   962  3204 D ShutdownThread: checkShutdownFlow: isIPOEnabled=true mShutdownFlow=0
        01-01 07:57:58.145422   962  3204 I ShutdownThread: set launcher as foreground
        01-01 07:57:58.172181   962  3204 I ShutdownThread: Sending shutdown broadcast...
        01-01 07:57:58.501727   962  3204 I ShutdownThread: Shutting down activity manager...
        01-01 07:57:58.710893   962  3204 I ShutdownThread: Shutting down package manager...
        01-01 07:57:58.720345   962  3204 I ShutdownThread: Shutting down radios...
        01-01 07:57:58.722303   962  3220 W ShutdownThread: task run
        01-01 07:57:58.725504   962  3220 W ShutdownThread: Turning off cellular radios...
        01-01 07:57:58.744406   962  3220 I ShutdownThread: Waiting for NFC, Bluetooth and Radio...
        01-01 07:57:59.247395   962  3220 I ShutdownThread: Radio turned off.
        01-01 07:57:59.248435   962  3220 I ShutdownThread: NFC, Radio and Bluetooth shutdown complete.
        01-01 07:57:59.249857   962  3204 I ShutdownThread: Shutting down MountService
        01-01 07:58:03.139331   962  1080 D ShutdownThread: setBacklightBrightness: Off
        
        }


    修改：
        1.关机的判断条件有很多，这里通常电量为0%，或者低于关机电压
        一般上报电量0%的时候，上层会下达关机的指令，然后层层调用进行关机，有时候也会是检测到关机条件后底层直接关机的
        SHUTDOWN_GAUGE0这个是不支持0%关机，只是电压低于关机电压后关机这个条件
        SHUTDOWN_GAUGE0_VOLTAGE这个定义的关机电压

        2. fg_cust_data.pseudo1_t0 = UNIT_TRANS_100 * g_FG_PSEUDO1_T0[g_fg_battery_id];
        这个根据battery_id选择底层多少的偏移量相当于上层的1% ，100%
        //Gionee <gn_by_charging> <lilubao> <20170508> add for change charging process begin
        int g_FG_PSEUDO1_T0[TOTAL_BATTERY_NUMBER] = { 2, 6, 7, 8};		// 5->2
        int g_FG_PSEUDO1_T1[TOTAL_BATTERY_NUMBER] = { 3, 10, 11, 12};	// 9->3
        int g_FG_PSEUDO1_T2[TOTAL_BATTERY_NUMBER] = { 13, 14, 15, 16};
        int g_FG_PSEUDO1_T3[TOTAL_BATTERY_NUMBER] = { 17, 18, 19, 20};
        int g_FG_PSEUDO1_T4[TOTAL_BATTERY_NUMBER] = { 21, 22, 23, 24};

        int g_FG_PSEUDO100_T0[TOTAL_BATTERY_NUMBER] = { 98, 98, 98, 97};
        int g_FG_PSEUDO100_T1[TOTAL_BATTERY_NUMBER] = { 98, 95, 94, 93};// 95->98	
        int g_FG_PSEUDO100_T2[TOTAL_BATTERY_NUMBER] = { 90, 90, 90, 89};
        int g_FG_PSEUDO100_T3[TOTAL_BATTERY_NUMBER] = { 80, 80, 86, 85};
        int g_FG_PSEUDO100_T4[TOTAL_BATTERY_NUMBER] = { 80, 80, 82, 81};
        //Gionee <gn_by_charging> <lilubao> <20170508> add for change charging process end

        //Gionee <gn_by_charging> <lilubao> <20170508> add for change charging process begin
        #define SHUTDOWN_GAUGE0 	  0 	// 1->0			
        #define SHUTDOWN_GAUGE1_XMINS 1
        #define SHUTDOWN_GAUGE0_VOLTAGE 	34000	// 35000->34000
        //Gionee <gn_by_charging> <lilubao> <20170508> add for change charging process end
    }


    恒流充电电流太小只有 1.3~1.6A左右 
    {
        之前修改的部分没有同步，试试改过之后还是否这样

        修改之后电流可以达到1.9A但是不是一直这样，然后电流逐渐下降，这不是恒流充电该有的
    }



    调用流程    
    电池相关参数的初始化，这部分跟电池参数相关的应该是之前的meter那一类
    （mtk_battery.c）battery_probe 电池参数的初始化 -> (mtk_battery_hal.c)battery_meter_ctrl = bm_ctrl_cmd,电池电量参数相关计算的一组函数接口

    -> fg_custom_init_from_header 从头文件中获得电池参数 -> battery_update_routine,这是一个线程，是负责检测硬件电池参数变化，然后上报给power_supply

    根据debug等级可以调整轮训时间 bat_get_debug_level() >= BMLOG_DEBUG_LEVEL -> (mtk_power_misc.c)shutdown_event_handler这个是关机检测执行的回调函数

    -> battery_probe


    fg这两个中断干什么？
    {
        fg_iavg_int_ht_handler      FG_IAVG_H_NO
        fg_iavg_int_lt_handler      FG_IAVG_L_NO

        应该是跟high，low相关的
    }



    控制fuel_gauge的log等级
    FG_daemon_log_level

}


















关掉OTG功能
    不注册OTG这个中断




mmi测试：加入充电测试选项



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


















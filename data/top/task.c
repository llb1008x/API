





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




    开机充电过程恒流的充电电流变化不稳定，没有达到预定要求    
    {
        ****需要定位这些参在哪里改，编译的脚本，需要的文件

        具体说就是刚开始可以达到1.9A，但是充了一段时间后充电电流下降到1.6A左右，开机电压偏高

        1.开机电压偏高 3711mV，电压都上3.7V了，开机电压偏高
        这个是都去开机时候的电压不是开机要求的临界值，插上充电器电池电压有可能偏高在3.5V左右算正常
        修改开机电压BATTERY_LOWVOL_THRESOLD
        mt_battery.c    ->   check_bat_protect_status


        2.充电电流不能稳定在1.8A左右
        充电电流大部分时间维持在1.85A左右，这里修改了电池容量

        
        3.充电显示达到100%的时候电流还是很大，从这点看出软件设置的电池容量应该比实际的容量小
        实际情况应该是充电如果达到100%肯定处于恒压阶段，充电电流很小，充电达到100%但是电流很大
        明显电量没有达到100%，电池设定的容量偏小

        之前17W05A上还有一个BUG是电池很快就截止了，90%~95%就截止，这种情况跟上面相反，电池
        设定的容量比实际的大
        mtk_battery_table.h
        修改
        {
            int g_Q_MAX_T0[TOTAL_BATTERY_NUMBER] = { 2946, 2712, 2490, 1965};   // 2946->2800->3100
            int g_Q_MAX_T1[TOTAL_BATTERY_NUMBER] = { 2796, 2851, 2468, 1984};   // 2796->2657->2936
            int g_Q_MAX_T2[TOTAL_BATTERY_NUMBER] = { 2718, 2432, 2310, 1946};   // 2718->2583->2854
            int g_Q_MAX_T3[TOTAL_BATTERY_NUMBER] = { 2535, 1991, 1858, 1873};   // 2535->2410->2662
            int g_Q_MAX_T4[TOTAL_BATTERY_NUMBER] = { 2523, 1960, 1843, 1851};   // 2523->2400->2649

            int g_Q_MAX_T0_H_CURRENT[TOTAL_BATTERY_NUMBER] = { 2646, 2412, 2190, 1665}; 
            int g_Q_MAX_T1_H_CURRENT[TOTAL_BATTERY_NUMBER] = { 2496, 2551, 2168, 1684};
            int g_Q_MAX_T2_H_CURRENT[TOTAL_BATTERY_NUMBER] = { 2418, 2132, 2010, 1646};
            int g_Q_MAX_T3_H_CURRENT[TOTAL_BATTERY_NUMBER] = { 2235, 1691, 1558, 1573};
            int g_Q_MAX_T4_H_CURRENT[TOTAL_BATTERY_NUMBER] = { 2223, 1660, 1543, 1551};
        }


        4.充电过程中，温升问题
        电池温度达到42，主板温度估计还要高很多
        需要看电池的spec，电池充电达到多少度，需要考虑充电电流大小，这根电池的密度有关



        电量计系数偏大，偏小(电量计是用来计算电池准确的电量的一个参数，是为了让显示的尽量准确)
        {   
            mt6757_battery_prop.dtsi -> CAR_TUNE_VALUE
            一起测量电量计系数的条件：
                两个电源，一路供电4V电压，维持手机正常工作，一路给电量计采样电阻两端供1A的电流，这时候看手机显示的
                电流是多少。
                大于1A，电量计系数偏大，小于1A，电量计系数偏小

            从log分析充电到100%的时候充电电流的条件：
                如果充到100%的时候还有很大的充电电流，电量计系数可能偏大。
                充到100%是电量计计算的100%，如果还有很大的电流可能是电池还没充满，认为的电池容量(偏小) * 电量计系数（偏大） = 实际的电池容量                   
        }


        电池容量，充到100%，截止
        {
            充到100%，软件显示的充满，这里面既有参数的偏差也考虑了用户的体验(底层上层映射)，充电截止则是真正的电池充满（电池容量）
            所以如果充到100%跟充电截止差距很大，充到100%跟充电截止之间时间差不能大于30min，实际情况充到100%应该处于恒压，充电电流也应该很小
            充到100%的时候充电电流应该在电池容量的10%~15%，0.1C
        }


        放电到关机，看1%电池电压，耗到关机
        {
            如果放电到1%，关机后电池电压还是很大，则用户还有很大的电量没有用到。            
            1%，两端对称，用户的电是用完的，放电关机后的电池电压不能太高
        }


        ln -f -s /home/llb/project/PRO/source/gionee_packages_apk_amigo/qcom/packages_7.0 to packages.

    }



    调用流程    
    电池相关参数的初始化，这部分跟电池参数相关的应该是之前的meter那一类
    （mtk_battery.c）battery_probe 电池参数的初始化 -> (mtk_battery_hal.c)battery_meter_ctrl = bm_ctrl_cmd,电池电量参数相关计算的一组函数接口

    -> fg_custom_init_from_header 从头文件中获得电池参数 -> battery_update_routine,这是一个线程，是负责检测硬件电池参数变化，然后上报给power_supply

    根据debug等级可以调整轮训时间 bat_get_debug_level() >= BMLOG_DEBUG_LEVEL -> 后面是一串中断回调函数，都是跟电量计相关的 -> (mtk_power_misc.c)
    
    mtk_power_misc_init ,难道是power相关的misc ，这个地方执行了很多关机条件检测 ，shutdown_event_handler这个是关机检测执行的回调函数 

     


    fg这两个中断干什么？
    {
        fg_iavg_int_ht_handler      FG_IAVG_H_NO
        fg_iavg_int_lt_handler      FG_IAVG_L_NO

        应该是跟high，low相关的
        电量增加减少变化
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
























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


    Applying: Fixed bug 67671
    No changes - did you forget to use 'git add'?
    If there is nothing left to stage, chances are that something else
    already introduced the same changes; you might want to skip this patch.

    When you have resolved this problem run "git rebase --continue".
    If you would prefer to skip this patch, instead run "git rebase --skip".
    To check out the original branch and stop rebasing run "git rebase --abort".


    It seems that there is already a rebase-apply directory, and
    I wonder if you are in the middle of another rebase.  If that is the
    case, please try
        git rebase (--continue | --abort | --skip)
    If that is not the case, please
        rm -fr /home/llb/project/PRO/source/17G05A/L30_6757_17G05A_N0.MP5_161227_ALPS/android_mtk_6757_mp/gionee/.git/rebase-apply
    and run me again.  I am stopping in case you still have something
    valuable there.




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


















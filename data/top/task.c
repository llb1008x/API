








/*要处理的问题*/
{


    pmi8952,pmi8940,pmi8937,ti的bq系列电量计，以及精度对比
    {
        三个相关的文档对比
        {
            1.bq27428
            


        }

        给高通提case
    }    










    利用17G16A项目熟悉高通代码
    {
        充电smbcharger
        {   
            dtsi配置文件
            ./arch/arm/boot/dts/qcom/msm-pmi8940.dtsi

            kernel代码
            qpnp-smbcharger.c
            pmic-voter.c    



            (qpnp-smbcharger.c) 这个是高通充电用的主要的源代码，smbchg_init初始化模块 -> spmi_driver_register注册设备驱动 -> smbchg_probe将设备驱动跟设备挂钩，
            建立bind -> 从dtsi文件中获取一些参数 ->  create_votable创建了好几个投票变量 (fcc_votable,usb_icl_votable,dc_icl_votable,usb_suspend_votable,
            dc_suspend_votable,battchg_suspend_votable,hw_aicl_rerun_disable_votable,hw_aicl_rerun_enable_indirect_votable,aicl_deglitch_short_votable,
            hvdcp_enable_votable)



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

        fuelgauge
        {


        }

    }    














    fuelgauge 3.0的问题,文档+邮件
    {


    }

}























/*17G10A当前存在的问题*/
{
	充电时序的关机充电和放电状态			
	{
		放电的1%位置还是有问题
		1%开始03：00 vbat=3.62V
		  结束03：52 vbat=3.3V

		1%可能相当于4%左右的电量，而且时间很长  
		1%电压还是很高
	}

	底层电量跟上层电量差别很大，但是这个没有开机和刚开是的充电的log		


	马达刷机第一次震动太弱，之后震动正常？	


	键盘按键有问题，mmi测试不通过，测键不管用


	mmi测试的调用节点好像还有问题
	{
		mmi测试读取的节点信息
		sys/class/power_supply/battery/

		充电可能读取的节点数据有问题
			几个电流，电压，电池电量，电池电压都有问题
			充电电流上不去
			{
				这个好像是主板温度过高，虽然显示的是电池温度50+，但是电池明显不热
				[    9.896490] <6>.(4)[258:charger_thread][name:mtk_charger&]Vbat=3775,I=-4446,VChr=9,T=49,Soc=0:0,CT:0:0

				[   12.371394] <6>.(6)[258:charger_thread][name:mtk_battery_hal&][fgauge_read_current] final current=5051 (ratio=950)
				[   12.372785] <6>.(6)[258:charger_thread][name:mtk_charger&]Vbat=3771,I=-5051,VChr=9,T=50,Soc=0:0,CT:0:0
				[   12.375494] <4>.(4)[258:charger_thread][name:pmic_auxadc&][mt6355_get_auxadc_value] ch = 2, reg_val = 0x1d3, adc_result = 410
				[   12.377141] <4>.(4)[258:charger_thread][name:mtk_battery_hal&][fgauge_read_current] final current=4735 (ratio=950)
				[   12.378491] [name:mtk_charger&][BATTERY] Battery over Temperature or NTC fail 50 50!!

				这个是电池的pin脚有问题
			}

		键盘mmi测试不能通过
			键盘的工作，以及涉及哪些目录下的文件
	}

        
	计步器，sd卡相关内容？
	{
		set_counter.c
		sd.c
	}


    从满电放电到关机的情况是否有异常


    计步器，sd卡相关内容？
    {
        set_counter.c
        sd.c
    }


	快充升压问题


	去掉OTG中断


	电池曲线的导入
}





/*G1605A  售后问题，功耗问题 healthd线程*/
{
    售后问题：   电量计不准的机器


    这个还不知道怎么看，只是觉得电量跟电池电压偏差有点大，而且底层电量跟上层显示的电量差别也很大


    apk发包次数，谁发的(主从)，就是链接到哪了？？
        那个脚本怎么用
}


















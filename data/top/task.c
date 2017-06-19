








/*要处理的问题*/
{
    G1605A底层电量跟上层电量偏差大，导致电量跳变问题，原因，怎么改？
        电量偏差累计
        1,测试电量相关的问题，要在Download软件版本后，或者 更换电池后，先充满一次电，同步一下电量，再进行测试；
        2,禁止频繁开关机重启、频繁插拔充电器操作，操作间隔时间要5分钟以上，且操作次数不要超过5次；
        3,未按照以上1、2操作，引出来的电量显示问题，开发可以直接驳回；

        eService号：


    17G10A mmi测试读取的数据有问题
    {
        mmi测试读取的节点在（这几个接口有问题？）
        sys/class/power_supply/battery/

        BatteryAverageCurrent  平均电流为0？
        ChargerVoltage InstatVolt      TemperatureR batt_temp capacity     

        device power   present_smb status_smb technology uevent 

        BatterySenseVoltage         ISenseVoltage  TempBattVoltage adjust_power batt_vol  

        capacity_smb health present status      subsystem  type  

        //17G10A    要加一个POWER_SUPPLY_PROP_BatteryPresentCurrent节点
        上面那一组变量属于power_supply的特性(mtk_battery.c)
        static enum power_supply_property battery_props[] = {
            POWER_SUPPLY_PROP_STATUS,
            POWER_SUPPLY_PROP_HEALTH,
            POWER_SUPPLY_PROP_PRESENT,
            POWER_SUPPLY_PROP_TECHNOLOGY,
            POWER_SUPPLY_PROP_CAPACITY,
            /* Add for Battery Service */
            POWER_SUPPLY_PROP_batt_vol,
            POWER_SUPPLY_PROP_batt_temp,
            /* Add for EM */
            POWER_SUPPLY_PROP_TemperatureR,             // 7
            POWER_SUPPLY_PROP_TempBattVoltage,          // 8
            POWER_SUPPLY_PROP_InstatVolt,               // 9
            POWER_SUPPLY_PROP_BatteryAverageCurrent,    // 10
            POWER_SUPPLY_PROP_BatterySenseVoltage,      // 11
            POWER_SUPPLY_PROP_ISenseVoltage,            // 12
            POWER_SUPPLY_PROP_ChargerVoltage,           // 13
            /* Dual battery */
            POWER_SUPPLY_PROP_status_smb,
            POWER_SUPPLY_PROP_capacity_smb,
            POWER_SUPPLY_PROP_present_smb,
            /* ADB CMD Discharging */
            POWER_SUPPLY_PROP_adjust_power,
        };

        //这些变量后面会在sys节点下呈现
        void battery_update_psd(struct battery_data *bat_data)
        {
            bat_data->BAT_batt_vol = battery_get_bat_voltage();
            bat_data->BAT_InstatVolt = bat_data->BAT_batt_vol;
            bat_data->BAT_BatterySenseVoltage = bat_data->BAT_batt_vol;
            bat_data->BAT_batt_temp = battery_get_bat_temperature();
            bat_data->BAT_TempBattVoltage = battery_meter_get_tempV();
            bat_data->BAT_TemperatureR = battery_meter_get_tempR(bat_data->BAT_TempBattVoltage);
            bat_data->BAT_BatteryAverageCurrent = battery_get_ibus();//ibus返回0，不用这个
            bat_data->BAT_ISenseVoltage = battery_meter_get_VSense();
            bat_data->BAT_ChargerVoltage = battery_get_vbus();
        }

        打印log用的这些函数接口
        pr_err("Vbat=%d,I=%d,VChr=%d,T=%d,Soc=%d:%d,CT:%d:%d\n", battery_get_bat_voltage(),
			curr_sign ? bat_current : -1 * bat_current,
			battery_get_vbus(), battery_get_bat_temperature(),
			battery_get_bat_soc(), battery_get_bat_uisoc(),
			mt_get_charger_type(), info->chr_type);

        /*debug*/    
        {
            1.节点数据没有及时上报，导致有的数据一直显示初始化时候的值            
                battery_update_psd获取充电，电池的数据然后赋值给power_supply子系统，battery_main数据结构

                在battery_update函数里面添加 battery_update_psd，每隔10s获取的数据可以上报上去

            2.添加时时的充电电流数据这个选项
            POWER_SUPPLY_PROP_BatteryPresentCurrent
            val->intval = data->BAT_BatteryPresentCurrent

            power_supply_sysfs.c和power_supply.h里面要添加properity特性
            要把这个节点特性呈现到sysfs里面

            3.mmi测试读取的数据有问题
            {
                电池技术显示null

                
                电池电压BatterySenseVoltage  显示的是电池温度batt_temp

                充电电流有数据但是数据有问题

                电池剩余电量capacity 显示的是电池电压BatterySenseVoltage
            }
        }


    }    




        充电器电压     POWER_SUPPLY_PROP_ChargerVoltage  
        bat_data->BAT_ChargerVoltage = battery_get_vbus();

        {


            POWER_SUPPLY_PROP_CAPACITY
            
            有几个电池相关的线程，做了哪些事

            过温保护门限设为55
        }
 

        mmi代码：
        AmigoMmiUtil.java

        amigoserver/AmigoServerManager.java
        com_android_server_AmigoServerManagerService.cpp

        BaseMMITestData.java
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


















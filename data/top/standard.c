

这里面应该保存一些用法，自己需要琢磨学习的




/**********************************************************************************************************************/
1.添加的调试代码,调试的等级需要根据情况调整
{
    //GioneeDrv LiLuBao 20161009 modify for platform change begin
    battery_log(BAT_LOG_FULL, "wait event 1\n");
    //GioneeDrv LiLuBao 20161009 modify for platform change end


    自己添加的宏
    #define  GNDRV_LLB_CHANGE
    修改模式
    //GioneeDrv LiLuBao 20161109 modify for platform change begin

    #if defined(GNDRV_LLB_CHANGE)
         KPOC_LOGI("[ChargingAnimation: %s %d],in  bootlogo_show_charging modify by llb",__FUNCTION__,__LINE__);	
    #endif


    ......

    //GioneeDrv LiLuBao 20161009 modify for platform change end


    #define BMLOG_DEBUG_LEVEL   7

    #define bm_debug(fmt, args...)   \
    do {									\
	    if (Enable_FGADC_LOG >= BMLOG_DEBUG_LEVEL) {		\
		    pr_debug(fmt, ##args); \
	    }								   \
    } while (0)

}







/***********************************************************************************************************************/
2.mtk电量计相关的函数
{
    
gm1.0 算法：    
    1.mt_battery_meter.c  
    fgauge_construct_battery_profile()
    /*
     *这段代码根据线性插值法，利用50,10,0,-10,的温度校准值构造不同温度下的ZCV
     *这边就是 MTK 为了结合实际温度 获取较为精确的电池信息 而采取的线性平均值法。
     *原理是利用预先测得的分布在-10 0 25 50 摄氏度下的 ZCV 表,结合真实温度,
     *动态重构一张当 前温度下的 ZCV 表格
     */
 
     void fgauge_construct_battery_profile_init(void)
    {
	    BATTERY_PROFILE_STRUCT_P temp_profile_p, profile_p[PROFILE_SIZE];
	    int i, j, saddles, profile_index;
	    signed int low_p = 0, high_p = 0, now_p = 0, low_vol = 0, high_vol = 0;

	    profile_p[0] = fgauge_get_profile(batt_meter_cust_data.temperature_t0);
	    profile_p[1] = fgauge_get_profile(batt_meter_cust_data.temperature_t1);
	    profile_p[2] = fgauge_get_profile(batt_meter_cust_data.temperature_t2);
	    profile_p[3] = fgauge_get_profile(batt_meter_cust_data.temperature_t3);
	    saddles = fgauge_get_saddles();
	    temp_profile_p =
	        (BATTERY_PROFILE_STRUCT_P) kmalloc(51 * sizeof(*temp_profile_p), GFP_KERNEL);
	    memset(temp_profile_p, 0, 51 * sizeof(*temp_profile_p));
	    for (i = 0; i < PROFILE_SIZE; i++) {
		    profile_index = 0;
		    for (j = 0; j * 2 <= 100; j++) {
			    while (profile_index < saddles && profile_index >= 0) {
				    if (((profile_p[i] + profile_index)->percentage) < j * 2) {
					    profile_index++;
					    continue;
				    } else if (((profile_p[i] + profile_index)->percentage) == j * 2) {
					    (temp_profile_p + j)->voltage =
					        (profile_p[i] + profile_index)->voltage;
					    (temp_profile_p + j)->percentage =
					        (profile_p[i] + profile_index)->percentage;
					    break;
				    }
				    low_p = (profile_p[i] + profile_index - 1)->percentage;
				    high_p = (profile_p[i] + profile_index)->percentage;
				    now_p = j * 2;
				    low_vol = (profile_p[i] + profile_index)->voltage;
				    high_vol = (profile_p[i] + profile_index - 1)->voltage;
				    (temp_profile_p + j)->voltage =
				        (low_vol * 1000 +
				         ((high_vol - low_vol) * 1000 * (now_p - low_p) / (high_p -
										           low_p))) /
				        1000;
				    (temp_profile_p + j)->percentage = j * 2;

				    break;
			    }
			    bm_print(BM_LOG_CRTI, "new battery_profile[%d,%d] <%d,%d>\n", i, j,
				     (temp_profile_p + j)->percentage, (temp_profile_p + j)->voltage);
		    }

		    for (j = 0; j * 2 <= 100; j++) {
			    (profile_p[i] + j)->voltage = (temp_profile_p + j)->voltage;
			    (profile_p[i] + j)->percentage = (temp_profile_p + j)->percentage;
		    }
	    }
	    kfree(temp_profile_p);
    }
 
 
 
 
    for (i = 0; i < saddles; i++) {
	    if (((high_profile_p + i)->voltage) > ((low_profile_p + i)->voltage)) {
		    temp_v_1 = (high_profile_p + i)->voltage;
		    temp_v_2 = (low_profile_p + i)->voltage;

		    (temp_profile_p + i)->voltage = temp_v_2 +
			    (((temperature - low_temperature) * (temp_v_1 - temp_v_2)
			     ) / (high_temperature - low_temperature)
			    );
	    } else {
		    temp_v_1 = (low_profile_p + i)->voltage;
		    temp_v_2 = (high_profile_p + i)->voltage;

		    (temp_profile_p + i)->voltage = temp_v_2 +
			    (((high_temperature - temperature) * (temp_v_1 - temp_v_2)
			     ) / (high_temperature - low_temperature)
			    );
	    }
    }



    2./*电池电量的计算，及一些参数的变化
    上面当电量充到100%后，如果使用GM2.0算法，就会重置电池的一些参数*/

    /*a.更新电池老化系数*/
    void fg_qmax_update_for_aging(void)
    {
    #if defined(CONFIG_POWER_EXT)
    #else
	    bool hw_charging_done = bat_is_charging_full();

    #if defined(CONFIG_MTK_JEITA_STANDARD_SUPPORT)
	    if (g_temp_status != TEMP_POS_10_TO_POS_45) {
		    bm_print(BM_LOG_CRTI, "Skip qmax update due to not 4.2V full-charging.\n");
		    return;
	    }
    #endif

	    if (hw_charging_done == true) {	/* charging full, g_HW_Charging_Done == 1 */

		    if (gFG_DOD0 > 85) {
			    if (gFG_columb < 0)
				    gFG_columb = gFG_columb - gFG_columb * 2;	/* absolute value */

			    gFG_BATT_CAPACITY_aging =
			        (((gFG_columb * 1000) + (5 * gFG_DOD0)) / gFG_DOD0) / 10;

			    /* tuning */
			    gFG_BATT_CAPACITY_aging =
			        (gFG_BATT_CAPACITY_aging * 100) / p_bat_meter_data->aging_tuning_value;

			    if (gFG_BATT_CAPACITY_aging == 0) {
				    gFG_BATT_CAPACITY_aging =
				        fgauge_get_Q_max(battery_meter_get_battery_temperature());
				    bm_print(BM_LOG_CRTI,
					     "[fg_qmax_update_for_aging] error, restore gFG_BATT_CAPACITY_aging (%d)\n",
					     gFG_BATT_CAPACITY_aging);
			    }

			    bm_print(BM_LOG_CRTI,
				     "[fg_qmax_update_for_aging] need update : gFG_columb=%d, gFG_DOD0=%d, new_qmax=%d\r\n",
				     gFG_columb, gFG_DOD0, gFG_BATT_CAPACITY_aging);
		    } else {
			    bm_print(BM_LOG_CRTI,
				     "[fg_qmax_update_for_aging] no update : gFG_columb=%d, gFG_DOD0=%d, new_qmax=%d\r\n",
				     gFG_columb, gFG_DOD0, gFG_BATT_CAPACITY_aging);
		    }
	    } else {
		    bm_print(BM_LOG_CRTI, "[fg_qmax_update_for_aging] hw_charging_done=%d\r\n",
			     hw_charging_done);
	    }
    #endif
    }

    3./*对不同数据的取平均运算*/

    static unsigned int mt_battery_average_method(BATTERY_AVG_ENUM type, unsigned int *bufferdata,

					          unsigned int data, signed int *sum,
					          unsigned char batteryIndex)
    {
	    unsigned int avgdata;

	    mt_battery_average_method_init(type, bufferdata, data, sum);

	    *sum -= bufferdata[batteryIndex];
	    *sum += data;
	    bufferdata[batteryIndex] = data;
	    avgdata = (*sum) / BATTERY_AVERAGE_SIZE;

	    battery_log(BAT_LOG_FULL, "bufferdata[%d]= (%d)\n", batteryIndex, bufferdata[batteryIndex]);
	    return avgdata;
    }

    static void mt_battery_average_method_init(BATTERY_AVG_ENUM type, unsigned int *bufferdata,
					       unsigned int data, signed int *sum)
    {
	    unsigned int i;
	    static kal_bool batteryBufferFirst = KAL_TRUE;
	    static kal_bool previous_charger_exist = KAL_FALSE;
	    static kal_bool previous_in_recharge_state = KAL_FALSE;
	    static unsigned char index;

	    /* reset charging current window while plug in/out { */
	    if (type == BATTERY_AVG_CURRENT) {
		    if (BMT_status.charger_exist == KAL_TRUE) {
			    if (previous_charger_exist == KAL_FALSE) {
				    batteryBufferFirst = KAL_TRUE;
				    previous_charger_exist = KAL_TRUE;
				    if ((BMT_status.charger_type == STANDARD_CHARGER)
    #if defined(CONFIG_MTK_DUAL_INPUT_CHARGER_SUPPORT)
				        && (DISO_data.diso_state.cur_vdc_state == DISO_ONLINE)
    #endif
				        )
					    data = batt_cust_data.ac_charger_current / 100;
				    else if (BMT_status.charger_type == CHARGING_HOST)
					    data = batt_cust_data.charging_host_charger_current / 100;
				    else if (BMT_status.charger_type == NONSTANDARD_CHARGER)
					    data = batt_cust_data.non_std_ac_charger_current / 100;	/* mA */
				    else	/* USB */
					    data = batt_cust_data.usb_charger_current / 100;	/* mA */
			    } else if ((previous_in_recharge_state == KAL_FALSE)
				       && (BMT_status.bat_in_recharging_state == KAL_TRUE)) {
				    batteryBufferFirst = KAL_TRUE;

				    if ((BMT_status.charger_type == STANDARD_CHARGER)
    #if defined(CONFIG_MTK_DUAL_INPUT_CHARGER_SUPPORT)
				        && (DISO_data.diso_state.cur_vdc_state == DISO_ONLINE)
    #endif
				        )
					    data = batt_cust_data.ac_charger_current / 100;
				    else if (BMT_status.charger_type == CHARGING_HOST)
					    data = batt_cust_data.charging_host_charger_current / 100;
				    else if (BMT_status.charger_type == NONSTANDARD_CHARGER)
					    data = batt_cust_data.non_std_ac_charger_current / 100;	/* mA */
				    else	/* USB */
					    data = batt_cust_data.usb_charger_current / 100;	/* mA */
			    }

			    previous_in_recharge_state = BMT_status.bat_in_recharging_state;
		    } else {
			    if (previous_charger_exist == KAL_TRUE) {
				    batteryBufferFirst = KAL_TRUE;
				    previous_charger_exist = KAL_FALSE;
				    data = 0;
			    }
		    }
	    }
	    /* reset charging current window while plug in/out } */

	    battery_log(BAT_LOG_FULL, "batteryBufferFirst =%d, data= (%d)\n", batteryBufferFirst, data);

	    if (batteryBufferFirst == KAL_TRUE) {
		    for (i = 0; i < BATTERY_AVERAGE_SIZE; i++)
			    bufferdata[i] = data;


		    *sum = data * BATTERY_AVERAGE_SIZE;
	    }

	    index++;
	    if (index >= BATTERY_AVERAGE_DATA_NUMBER) {
		    index = BATTERY_AVERAGE_DATA_NUMBER;
		    batteryBufferFirst = KAL_FALSE;
	    }
    }



}






/***************************************************************************************************************/
3.函数指针，封装相关接口
{
    
    /*
     *bm_func 这是一组函数指针数组，定义了一组函数指针，实现对MTK充电算法的实现
     *
     */
	
    static signed int(*bm_func[BATTERY_METER_CMD_NUMBER]) (void *data);

    signed int bm_ctrl_cmd(BATTERY_METER_CTRL_CMD cmd, void *data)
    {
	    signed int status = STATUS_UNSUPPORTED;
	    static signed int init = -1;

	    if (init == -1) {
		    init = 0;
		    bm_func[BATTERY_METER_CMD_HW_FG_INIT] = fgauge_initialization;
		    bm_func[BATTERY_METER_CMD_GET_HW_FG_CURRENT] = fgauge_read_current;
		    bm_func[BATTERY_METER_CMD_GET_HW_FG_CURRENT_SIGN] = fgauge_read_current_sign;
		    bm_func[BATTERY_METER_CMD_GET_HW_FG_CAR] = fgauge_read_columb;
		    bm_func[BATTERY_METER_CMD_HW_RESET] = fgauge_hw_reset;
		    bm_func[BATTERY_METER_CMD_GET_ADC_V_BAT_SENSE] = read_adc_v_bat_sense;
		    bm_func[BATTERY_METER_CMD_GET_ADC_V_I_SENSE] = read_adc_v_i_sense;
		    bm_func[BATTERY_METER_CMD_GET_ADC_V_BAT_TEMP] = read_adc_v_bat_temp;
		    bm_func[BATTERY_METER_CMD_GET_ADC_V_CHARGER] = read_adc_v_charger;
		    bm_func[BATTERY_METER_CMD_GET_HW_OCV] = read_hw_ocv;
		    bm_func[BATTERY_METER_CMD_DUMP_REGISTER] = dump_register_fgadc;
		    bm_func[BATTERY_METER_CMD_SET_COLUMB_INTERRUPT] = fgauge_set_columb_interrupt;
		    bm_func[BATTERY_METER_CMD_GET_BATTERY_PLUG_STATUS] = read_battery_plug_out_status;

		    bm_func[BATTERY_METER_CMD_GET_HW_FG_CAR_ACT] = fgauge_read_columb_accurate;
		    bm_func[BATTERY_METER_CMD_GET_IS_HW_OCV_READY] = read_is_hw_ocv_ready;
	    }

	    if (cmd < BATTERY_METER_CMD_NUMBER) {
		    if (bm_func[cmd] != NULL)
				    status = bm_func[cmd] (data);
		    else
			    status = STATUS_UNSUPPORTED;
	    } else
		    status = STATUS_UNSUPPORTED;

	    return status;
    }






    /*
     *battery_charging_control 函数指针的用法，一定要学会get！
     */
     
    //定义一个函数指针，参数类型和返回值类型
    typedef signed int(*CHARGING_CONTROL) (CHARGING_CTRL_CMD cmd, void *data);

    //定义函数指针变量
    CHARGING_CONTROL battery_charging_control;

    //枚举要使用的命令
    enum CHARGING_CTRL_CMD {
	    CHARGING_CMD_INIT,
	    CHARGING_CMD_DUMP_REGISTER,
	    CHARGING_CMD_ENABLE,
	    CHARGING_CMD_SET_CV_VOLTAGE,
	    CHARGING_CMD_GET_CURRENT,
	    CHARGING_CMD_SET_CURRENT,
	    CHARGING_CMD_GET_INPUT_CURRENT,
	    CHARGING_CMD_SET_INPUT_CURRENT,
	    CHARGING_CMD_GET_CHARGING_STATUS,
	    CHARGING_CMD_RESET_WATCH_DOG_TIMER,
	    CHARGING_CMD_SET_HV_THRESHOLD,
	    CHARGING_CMD_GET_HV_STATUS,
	    CHARGING_CMD_GET_BATTERY_STATUS,
	    CHARGING_CMD_GET_CHARGER_DET_STATUS,
	    CHARGING_CMD_GET_CHARGER_TYPE,
	    CHARGING_CMD_GET_IS_PCM_TIMER_TRIGGER,
	    CHARGING_CMD_SET_PLATFORM_RESET,
	    CHARGING_CMD_GET_PLATFORM_BOOT_MODE,
	    CHARGING_CMD_ENABLE_POWERPATH,
	    CHARGING_CMD_BOOST_ENABLE,
	    CHARGING_CMD_SET_TA_CURRENT_PATTERN,
	    CHARGING_CMD_NUMBER
    };


    /*定义轮训接口*/
    static void get_charging_control(void)
    {
	    battery_charging_control = chr_control_interface;
    }


    /*对具体的函数调用指针轮训，调用。这样操作是为了方便移植，可以针对不同的芯片定义不同的
    chargnig_func函数指针数组*/
    kal_int32 chr_control_interface(CHARGING_CTRL_CMD cmd, void *data)
    {
	    kal_int32 status;
	    //GioneeDrv GuoJianqiu 20151223 modify for platform change begin
	    if (cmd < CHARGING_CMD_NUMBER) {
		    if (charging_func[cmd] != NULL)
			    status = charging_func[cmd](data);
		    else {
		    battery_log(BAT_LOG_CRTI, "[chr_control_interface]cmd:%d not 
		    supported\n", cmd);
			    status = STATUS_UNSUPPORTED;
		    }
	    } else
		    status = STATUS_UNSUPPORTED;
	    //GioneeDrv GuoJianqiu 20151223 modify for platform change end
	    return status;
    }

    static kal_uint32(*const charging_func[CHARGING_CMD_NUMBER]) (void *data) = {
		    charging_hw_init, charging_dump_register,
		    charging_enable, charging_set_cv_voltage,
	        charging_get_current, charging_set_current, charging_set_input_current,
	        charging_get_charging_status, charging_reset_watch_dog_timer,
	        charging_set_hv_threshold, charging_get_hv_status,
		    charging_get_battery_status,
	        charging_get_charger_det_status, charging_get_charger_type,
	        charging_get_is_pcm_timer_trigger, charging_set_platform_reset,
	        charging_get_platfrom_boot_mode, charging_set_power_off,
	        charging_get_power_source, charging_get_csdac_full_flag,
	        charging_set_ta_current_pattern,   charging_set_error_state,charging_diso_init,
	        charging_get_diso_state, 	
		    //GioneeDrv GuoJianqiu 20151223 modify for 	 platform change begin
	        charging_set_chrind_ck_pdn, charging_sw_init
	 }; 
		    //GioneeDrv GuoJianqiu 20151223 modify for platform change end
		
		

		    /*定义这样一个函数指针，负责调用不同的函数处理不同的命令和数据*/
		    typedef  s32(*BATTERY_METER_CONTROL)  (int cmd, void *data);
		    BATTERY_METER_CONTROL  battery_meter_ctrl = NULL;

		    /*将bm_ctrl_cmd接口给那个函数指针，调用不同的函数接口*/
		    battery_meter_ctrl = bm_ctrl_cmd;
		    
		    
	4.mtk pe2.0快充电平时序
    int mtk_pe20_init(struct charger_manager *pinfo)
    {
	    int ret = 0;

	    wake_lock_init(&pinfo->pe2.suspend_lock, WAKE_LOCK_SUSPEND,
		    "PE+20 TA charger suspend wakelock");
	    mutex_init(&pinfo->pe2.access_lock);
	    mutex_init(&pinfo->pe2.pmic_sync_lock);

	    pinfo->pe2.ta_vchr_org = 5000000;
	    pinfo->pe2.idx = -1;
	    pinfo->pe2.vbus = 5000000;
	    pinfo->pe2.to_check_chr_type = true;
	    pinfo->pe2.is_enabled = true;

	    pinfo->pe2.profile[0].vbat = 3400000;
	    pinfo->pe2.profile[1].vbat = 3500000;
	    pinfo->pe2.profile[2].vbat = 3600000;
	    pinfo->pe2.profile[3].vbat = 3700000;
	    pinfo->pe2.profile[4].vbat = 3800000;
	    pinfo->pe2.profile[5].vbat = 3900000;
	    pinfo->pe2.profile[6].vbat = 4000000;
	    pinfo->pe2.profile[7].vbat = 4100000;
	    pinfo->pe2.profile[8].vbat = 4200000;
	    pinfo->pe2.profile[9].vbat = 4300000;

	    pinfo->pe2.profile[0].vchr = 8000000;
	    pinfo->pe2.profile[1].vchr = 8500000;
	    pinfo->pe2.profile[2].vchr = 8500000;
	    pinfo->pe2.profile[3].vchr = 9000000;
	    pinfo->pe2.profile[4].vchr = 9000000;

	    pinfo->pe2.profile[5].vchr = 9000000;
	    pinfo->pe2.profile[6].vchr = 9500000;
	    pinfo->pe2.profile[7].vchr = 9500000;
	    pinfo->pe2.profile[8].vchr = 10000000;
	    pinfo->pe2.profile[9].vchr = 10000000;

	    ret = charger_dev_set_pe20_efficiency_table(pinfo->chg1_dev);
	    if (ret != 0)
		    pr_err("%s: use default table, %d\n", __func__, ret);

	    return 0;
    }
    
    
    5.充电器类型的检测
    int hw_charger_type_detection(void)
	{
		CHARGER_TYPE charger_tye = CHARGER_UNKNOWN;

		/********* Step initial  ***************/
		hw_bc11_init();

		/********* Step DCD ***************/
		if(1 == hw_bc11_DCD())
		{
		     /********* Step A1 ***************/
		     if(1 == hw_bc11_stepA1())
		     {
		         charger_tye = APPLE_2_1A_CHARGER;
		         print("step A1 : Apple 2.1A CHARGER!\r\n");
		     }
		     else
		     {
		         charger_tye = NONSTANDARD_CHARGER;
		         print("step A1 : Non STANDARD CHARGER!\r\n");
		     }
		}
		else
		{
		     /********* Step A2 ***************/
		     if(1 == hw_bc11_stepA2())
		     {
		         /********* Step B2 ***************/
		         if(1 == hw_bc11_stepB2())
		         {
		             charger_tye = STANDARD_CHARGER;
		             print("step B2 : STANDARD CHARGER!\r\n");
		         }
		         else
		         {
		             charger_tye = CHARGING_HOST;
		             print("step B2 :  Charging Host!\r\n");
		         }
		     }
		     else
		     {
		         charger_tye = STANDARD_HOST;
		         print("step A2 : Standard USB Host!\r\n");
		     }
		}

		/********* Finally setting *******************************/
		hw_bc11_done();

		return charger_tye;
	}
	    

}





/*******************************************************************************************************/
4.察看中断号 cat  /proc/interrupts
{
    主要是根据log，中断号，找对应的代码

    G1605A的中断*/
               CPU0       CPU1       
     29:          0          0       GIC  29  arch_timer
     30:     250493      63863       GIC  30  arch_timer
     96:          0          0       GIC  96  mtk_cpuxgpt0
     97:          0          0       GIC  97  mtk_cpuxgpt1
     98:          0          0       GIC  98  mtk_cpuxgpt2
     99:          0          0       GIC  99  mtk_cpuxgpt3
    104:        102         65       GIC 104  musb-hdrc.0.auto
    110:          0          0       GIC 110  mtk-thermal
    111:       6423       9067       GIC 111  mtk-msdc
    112:          0          0       GIC 112  mtk-msdc
    114:        450        471       GIC 114  mtk-msdc
    116:         27         77       GIC 116  mt-i2c
    117:       1209       1228       GIC 117  mt-i2c
    118:      13383       1002       GIC 118  mt-i2c
    119:          0          0       GIC 119  mt-i2c
    122:          7         16       GIC 122  mtk btif irq
    123:        495        185       GIC 123  mtk-uart
    127:       4393       2192       GIC 127
    145:        762        638       GIC 145  mtk btif tx dma irq
    146:        357        623       GIC 146  mtk btif rx dma irq
    150:          0          0       GIC 150  1100a000.spi

    156:          0          0       GIC 156  IRTX
    157:        500        193       GIC 157  ptp
    160:          0          0       GIC 160  mt_wdt
    166:          3          1       GIC 166  mt_emi_mpu, devapc
    172:          0          0       GIC 172  CCIF0_AP
    176:          0          0       GIC 176  Afe_ISR_Handle
    177:       1438        421       GIC 177  CLDMA_AP
    178:          0          0       GIC 178  m4u
    182:          0          0       GIC 182  gcpu
    183:       3732       2384       GIC 183  mtk_cmdq
    184:          0          0       GIC 184  mt-gpt
    195:          0          0       GIC 195  pmic_wrap
    196:          0          0       GIC 196  mtk-kpd
    197:          0          0       GIC 197  SPM
    198:          0          0       GIC 198  SPM
    199:          0          0       GIC 199  SPM
    200:          0          0       GIC 200  SPM
    211:          0          0       GIC 211  Vcodec
    212:          0          0       GIC 212  Vcodec
    213:          0          0       GIC 213  jpeg_enc_driver
    215:          0          0       GIC 215  ISP
    218:       4595       4169       GIC 218  DISPSYS
    225:       2795       2629       GIC 225  DISPSYS
    226:       7778       8080       GIC 226  DISPSYS
    227:          0          0       GIC 227  DISPSYS
    228:          1          4       GIC 228  DISPSYS
    231:       1703       1448       GIC 231  DISPSYS
    235:         57         44       GIC 235  DISPSYS
    237:          0          0       GIC 237  DISPSYS
    238:          0          0       GIC 238  ISP
    239:          0          0       GIC 239  ISP
    240:          0          0       GIC 240  camera-fdvt
    241:         13         12       GIC 241  jpeg_dec_driver
    242:       4789       5013       GIC 242  13040000.mali
    243:          0          0       GIC 243  13040000.mali
    244:       2241       2366       GIC 244  13040000.mali
    253:          0          0       GIC 253  MD_WDT
    258:      14527       9034       GIC 258  AHB_SLAVE_HIF
    259:          0          1       GIC 259  BTIF_WAKEUP_IRQ
    260:          0          0       GIC 260  BTCVSD_ISR_Handle
    261:          0          0       GIC 261  C2K_CCCI
    262:       1602       1363       GIC 262  C2K_CCCI
    263:          0          0       GIC 263  CIRQ
    280:        106       2381       GIC 280  mobicore
    281:          0          0       GIC 281  ATF_irq
    288:          0          0   mt-eint   0  home_key_eint
    290:          0          1   mt-eint   2  goodix_fp_irq
    293:          0          0   mt-eint   5  11240000.msdc1 cd
    294:          0          0   mt-eint   6  accdet-eint
    298:          2          6   mt-eint  10  mtk-tpd
    334:          0          0   mt-eint  46  USB_IDDIG
    376:          0          0   mt-eint  88  ALS-eint
    494:         95          0   mt-eint 206  pmic-eint
    496:          0          0   mt-eint 208  mdm_reset_ind
    498:          0          0   mt-eint 210  cbp(c2k EE)
    499:        344         41   mt-eint 211  cp_indicate_state
    500:        328         60   mt-eint 212  cp_wake_ap
    IPI0:    458229     368679       Rescheduling interrupts
    IPI1:       306        598       Function call interrupts
    IPI2:       250       2220       Single function call interrupts
    IPI3:         0          0       CPU stop interrupts
    IPI4:         0          0       Timer broadcast interrupts
    IPI5:         0          0       IRQ work interrupts




}



/****************************************************************************************/
5.usb_otg设备的处理过程和GPIO的配置
{
    /*1.设备树相关的配置(基本的设备树的配置)*/
	usb0:usb20@11200000 {				//USB20的基地址为11200000
		compatible = "mediatek,mt6735-usb20";	//通过设备树匹配的名称mt6735-usb20
		cell-index = <0>;			//子树有几个	
		reg = <0x11200000 0x10000>,		//寄存器的地址和范围	
		      <0x11210000 0x10000>;
		interrupts = <GIC_SPI 72 IRQ_TYPE_LEVEL_LOW>;//设置中断模式：中断类型，中断号，中断触发的类型
		mode = <2>;
		multipoint = <1>;
		num_eps = <16>;				//设备树添加的一些属性，共有16个端点描述符
		clocks = <&perisys PERI_USB0>;

		clock-names = "usb0";
		vusb33-supply = <&mt_pmic_vusb33_ldo_reg>;//这个是设置的什么意思？
		iddig_gpio = <0 1>;			//iddig_gpio = <46 1>;	 //寄存器的位置改了，			
		drvvbus_gpio = <83 2>;			//drvvbus_gpio = <60 2>;
	};

	/* USB GPIO Kernal Standardization start */
	/* GioneeDrv GuoJianqiu 20160425 modify for OTG function begin */
	&pio {
		usb_default: usb_default {
		};

		gpio46_mode1_iddig: iddig_irq_init {
			pins_cmd_dat {
				pins = <PINMUX_GPIO46__FUNC_IDDIG>;
				slew-rate = <0>;
				bias-pull-up = <00>;
			};
		};

		gpio60_mode2_drvvbus: drvvbus_init {
			pins_cmd_dat {
				pins = <PINMUX_GPIO60__FUNC_GPIO60>;
				slew-rate = <1>;
				bias-pull-down = <00>;
			};
		};

		gpio60_mode2_drvvbus_low: drvvbus_low {
			pins_cmd_dat {
				pins = <PINMUX_GPIO60__FUNC_GPIO60>;
				slew-rate = <1>;
				output-low;
				bias-pull-down = <00>;
			};
		};

		gpio60_mode2_drvvbus_high: drvvbus_high {
			pins_cmd_dat {
				pins = <PINMUX_GPIO60__FUNC_GPIO60>;
				slew-rate = <1>;
				output-high;
				bias-pull-down = <00>;
			};
		};
	};

	&usb0 {
		iddig_gpio = <46 1>;
		pinctrl-names = "usb_default", "iddig_irq_init", "drvvbus_init", "drvvbus_low", "drvvbus_high";
		pinctrl-0 = <&usb_default>;
		pinctrl-1 = <&gpio46_mode1_iddig>;
		pinctrl-2 = <&gpio60_mode2_drvvbus>;
		pinctrl-3 = <&gpio60_mode2_drvvbus_low>;
		pinctrl-4 = <&gpio60_mode2_drvvbus_high>;
		status = "okay";
	};
	
	
	
	//Gionee GuoJianqiu 201601026 modify for OTG SWITCH begin
	#if defined(GN_BATTERY_OTGCHARGE_SWITCH_SUPPORT)
	ret_device_file = device_create_file(&(dev->dev), &dev_attr_Otg_Charge_Switch);
	#endif
	//Gionee GuoJianqiu 201601026 modify for OTG SWITCH end

	
	//Gionee GuoJianqiu 201601026 modify for OTG SWITCH begin
	#if defined(GN_BATTERY_OTGCHARGE_SWITCH_SUPPORT)
	kal_bool gn_otg_charge_switch_State = KAL_FALSE;
	static ssize_t show_Otg_Charge_Switch(struct device *dev,struct device_attribute *attr, char *buf)
	{
	    battery_log(BAT_LOG_CRTI, "show_otg_charge_switch_State = %d\n", gn_otg_charge_switch_State);    
	    return sprintf(buf, "%u\n", gn_otg_charge_switch_State);
	}
	static ssize_t store_Otg_Charge_Switch(struct device *dev,struct device_attribute *attr, const char *buf, size_t size)
	{
		int rv;
		kal_bool otg_charge_state = gn_otg_charge_switch_State;
	
		/*rv = sscanf(buf, "%u", &otg_charge_state);*/
		rv = kstrtouint(buf, 0, &otg_charge_state);		//字符串转无符号整形
		if (rv != 0)
			{
			battery_log(BAT_LOG_CRTI, "  bad argument, echo [enable] > Otg_Charge_Switch! gn_otg_charge_switch_State = %d\n", gn_otg_charge_switch_State);
			return -EINVAL;
			}

		if(otg_charge_state)
			gn_Open_Otg_Irq();
		else
			gn_Close_Otg_Irq();

		gn_otg_charge_switch_State = otg_charge_state;
		    battery_log(BAT_LOG_CRTI, "store_otg_charge_switch_State = %d\n", gn_otg_charge_switch_State);
		    return size;
	}
	static DEVICE_ATTR(Otg_Charge_Switch, 0664, show_Otg_Charge_Switch, store_Otg_Charge_Switch);
	#endif
	//Gionee GuoJianqiu 201601026 modify for OTG SWITCH end


}











/*******************************************************************************************************************************/
6.mtk充电相关的几个代码
{
    1.upmu_is_chr_det()
	字面意思是电源管理系统对充电的检测，至于这个宏CONFIG_MTK_DUAL_INPUT_CHARGER_SUPPORT应该是是否支持电池给手机充电或者电池给外设充电
	检测充电器是否存在
	
	2.数据结构
	//充电接口类型
	typedef enum {							
		CHARGER_UNKNOWN = 0,	/* 未充电 */
		STANDARD_HOST,		/* USB : 450mA */
		CHARGING_HOST,		/* 往外供电 */
		NONSTANDARD_CHARGER,	/* AC : 450mA~1A */
		STANDARD_CHARGER,	/* AC 充电器的电流大: ~1A */
	} CHARGER_TYPE;
	

	/*pmic中断的设置，（可能是触发了某个中断，转换成这个中断对应的状态，什么样的配置）	*/
	/*****************************************************************************
	 * interrupt Setting
	 ******************************************************************************/
	
	//设置每个引脚的名称
	#define PMIC_S_INT_GEN(_name)	\
	{	\
		.name =  #_name,	\
	}	
	
	//设置每个引脚的地址，对应的配置，设置引脚，对应的中断
	#define PMIC_M_INTS_GEN(adr, enA, setA, clearA, interrupt)	\
	{	\
		.address =  adr,	\
		.en =  enA,	\
		.set =  setA,	\
		.clear =  clearA,	\
		.interrupts = interrupt,	\
	}

	static struct pmic_interrupt_bit interrupt_status0[] = {
		PMIC_S_INT_GEN(RG_INT_STATUS_PWRKEY),
		PMIC_S_INT_GEN(RG_INT_STATUS_HOMEKEY),
		PMIC_S_INT_GEN(RG_INT_STATUS_PWRKEY_R),
		PMIC_S_INT_GEN(RG_INT_STATUS_HOMEKEY_R),
		PMIC_S_INT_GEN(RG_INT_STATUS_THR_H),
		PMIC_S_INT_GEN(RG_INT_STATUS_THR_L),
		PMIC_S_INT_GEN(RG_INT_STATUS_BAT_H),
		PMIC_S_INT_GEN(RG_INT_STATUS_BAT_L),
		PMIC_S_INT_GEN(NO_USE),
		PMIC_S_INT_GEN(RG_INT_STATUS_RTC),
		PMIC_S_INT_GEN(RG_INT_STATUS_AUDIO),
		PMIC_S_INT_GEN(NO_USE),
		PMIC_S_INT_GEN(RG_INT_STATUS_ACCDET),
		PMIC_S_INT_GEN(RG_INT_STATUS_ACCDET_EINT),
		PMIC_S_INT_GEN(RG_INT_STATUS_ACCDET_NEGV),
		PMIC_S_INT_GEN(RG_INT_STATUS_NI_LBAT_INT),
	};

	static struct pmic_interrupt_bit interrupt_status1[] = {
		PMIC_S_INT_GEN(RG_INT_STATUS_VPROC_OC),
		PMIC_S_INT_GEN(RG_INT_STATUS_VSYS_OC),
		PMIC_S_INT_GEN(RG_INT_STATUS_VLTE_OC),
		PMIC_S_INT_GEN(NO_USE),
		PMIC_S_INT_GEN(NO_USE),
		PMIC_S_INT_GEN(RG_INT_STATUS_VCORE_OC),
		PMIC_S_INT_GEN(NO_USE),
		PMIC_S_INT_GEN(NO_USE),
		PMIC_S_INT_GEN(RG_INT_STATUS_VPA_OC),
		PMIC_S_INT_GEN(RG_INT_STATUS_LDO_OC),
		PMIC_S_INT_GEN(RG_INT_STATUS_BAT2_H),
		PMIC_S_INT_GEN(RG_INT_STATUS_BAT2_L),
		PMIC_S_INT_GEN(RG_INT_STATUS_VISMPS0_H),
		PMIC_S_INT_GEN(RG_INT_STATUS_VISMPS0_L),
		PMIC_S_INT_GEN(RG_INT_STATUS_AUXADC_IMP),
		PMIC_S_INT_GEN(NO_USE),
	};

	static struct pmic_interrupt_bit interrupt_status2[] = {
		PMIC_S_INT_GEN(RG_INT_STATUS_OV),
		PMIC_S_INT_GEN(RG_INT_STATUS_BVALID_DET),
		PMIC_S_INT_GEN(RG_INT_STATUS_VBATON_HV),
		PMIC_S_INT_GEN(RG_INT_STATUS_VBATON_UNDET),
		PMIC_S_INT_GEN(RG_INT_STATUS_WATCHDOG),
		PMIC_S_INT_GEN(RG_INT_STATUS_PCHR_CM_VDEC),
		PMIC_S_INT_GEN(RG_INT_STATUS_CHRDET),
		PMIC_S_INT_GEN(RG_INT_STATUS_PCHR_CM_VINC),
		PMIC_S_INT_GEN(RG_INT_STATUS_FG_BAT_H),
		PMIC_S_INT_GEN(RG_INT_STATUS_FG_BAT_L),
		PMIC_S_INT_GEN(RG_INT_STATUS_FG_CUR_H),
		PMIC_S_INT_GEN(RG_INT_STATUS_FG_CUR_L),
		PMIC_S_INT_GEN(RG_INT_STATUS_FG_ZCV),
		PMIC_S_INT_GEN(RG_INT_STATUS_SPKL_D),
		PMIC_S_INT_GEN(RG_INT_STATUS_SPKL_AB),
		PMIC_S_INT_GEN(NO_USE),
	};

	static struct pmic_interrupts interrupts[] = {
		PMIC_M_INTS_GEN(MT6328_INT_STATUS0, MT6328_INT_CON0, MT6328_INT_CON0_SET,
				MT6328_INT_CON0_CLR, interrupt_status0),
		PMIC_M_INTS_GEN(MT6328_INT_STATUS1, MT6328_INT_CON1, MT6328_INT_CON1_SET,
				MT6328_INT_CON1_CLR, interrupt_status1),
		PMIC_M_INTS_GEN(MT6328_INT_STATUS2, MT6328_INT_CON2, MT6328_INT_CON2_SET,
				MT6328_INT_CON2_CLR, interrupt_status2),
	};

/*这整个过程可能是中断触发，执行回调函数，回调函数里面对中断的类型进行遍历，跟哪个状态对比，符合的就产生对应的配置
什么样的状态*/

}
	






/**********************************************************************************************************/
7.C语言的一些用法
{
    1.遍历双向链表，用法具有通用性，高级进阶，数据结构和算法
    
    #define list_for_each_entry(pos, head, member)				\
	    for (pos = list_entry((head)->next, typeof(*pos), member);	\
	         &pos->member != (head); 	\
	         pos = list_entry(pos->member.next, typeof(*pos), member))


    #define list_entry(ptr, type, member) \
	    container_of(ptr, type, member)


    #ifndef container_of
    #define container_of(ptr, type, member) ({			\
	    const typeof(((type *)0)->member) * __mptr = (ptr);	\
	    (type *)((char *)__mptr - offsetof(type, member)); })


    这个语法跟上面相似，还是不会，这种宏函数的用法
    /**
     * spmi_for_each_container_dev - iterate over the array of devnode resources.
     * @res: spmi_resource pointer used as the array cursor
     * @spmi_dev: spmi_device to iterate
     *
     * Only useable in spmi-dev-container configurations.
     */
    #define spmi_for_each_container_dev(res, spmi_dev)			      \
	    for (res = ((spmi_dev)->dev_node ? &(spmi_dev)->dev_node[0] : NULL);  \
	         (res - (spmi_dev)->dev_node) < (spmi_dev)->num_dev_node; res++)
	         
	         
	         
	2.条件表达式根据en使能与否使用不同的函数，参数是一样的，然后在回掉不同的函数        
    ret = (en ? rt5081_pmu_reg_set_bit : rt5081_pmu_reg_clr_bit)
	(chg_data->chip, RT5081_PMU_REG_CHGCTRL3, RT5081_MASK_ILIM_EN);	 
		
		
    static inline int rt5081_pmu_reg_set_bit(struct rt5081_pmu_chip *chip, u8 addr,
		    u8 mask)
    {
	    return rt5081_pmu_reg_update_bits(chip, addr, mask, mask);
    }	

    static inline int rt5081_pmu_reg_clr_bit(struct rt5081_pmu_chip *chip, u8 addr,
		    u8 mask)
    {
	    return rt5081_pmu_reg_update_bits(chip, addr, mask, 0x00);
    }	
    
    
    
    3.都去寄存器的值，mask，shift
    掩码是为了读多少位，位移是为了将读出的数据方便处理
    static int rt5081_get_aicr(struct charger_device *chg_dev, u32 *aicr)
    {
	    int ret = 0;
	    u8 reg_aicr = 0;
	    struct rt5081_pmu_charger_data *chg_data =
		    dev_get_drvdata(&chg_dev->dev);

	    ret = rt5081_pmu_reg_read(chg_data->chip, RT5081_PMU_REG_CHGCTRL3);
	    if (ret < 0)
		    return ret;

	    reg_aicr = (ret & RT5081_MASK_AICR) >> RT5081_SHIFT_AICR;
	    *aicr = rt5081_find_closest_real_value(RT5081_AICR_MIN, RT5081_AICR_MAX,
		    RT5081_AICR_STEP, reg_aicr);

	    return ret;
    }

    static u32 rt5081_find_closest_real_value(u32 min, u32 max, u32 step,
	    u8 reg_val)
    {
	    u32 ret_val = 0;

	    ret_val = min + reg_val * step;
	    if (ret_val > max)
		    ret_val = max;

	    return ret_val;
    }
    
    
    
    4.扫描路径下的文件
    static int scan_dir(const char *dirname)
    {
        char devname[PATH_MAX];
        char *filename;
        DIR *dir;
        struct dirent *de;
        dir = opendir(dirname);
        if(dir == NULL)
            return -1;
        strcpy(devname, dirname);
        filename = devname + strlen(devname);
        *filename++ = '/';
        while((de = readdir(dir))) {
            if(de->d_name[0] == '.' &&
               (de->d_name[1] == '\0' ||
                (de->d_name[1] == '.' && de->d_name[2] == '\0')))
                continue;
            strcpy(filename, de->d_name);
            KPOC_LOGI("%s(), open_device %s\n", __FUNCTION__, devname);
            open_device(devname);
        }
        closedir(dir);
        return 0;
    }


    5.rt5081插入充电器的时候读取中断信息
    这段对于寄存器，位的操作
     
    for (i = 0; i < 16; i++) {
	    stat_chg[i] = stat_old[i] ^ stat_new[i];
	    valid_chg[i] = (stat_new[i] & rt5081_irqr_filter[i]) |
			    (~stat_new[i] & rt5081_irqf_filter[i]);
	    data[i] |= (stat_chg[i] & valid_chg[i]);
	    data[i] &= ~mask[i];
	    if (!data[i])
		    continue;
	    for (j = 0; j < 8; j++) {
		    if (!(data[i] & (1 << j)))
			    continue;
		    ret = irq_find_mapping(chip->irq_domain, i * 8 + j);
		    if (ret)
			    handle_nested_irq(ret);
		    else
			    dev_err(chip->dev, "unmapped %d %d\n", i, j);
	    }
    }

    
    
}


     
	     

    
/**************************************************************************************/
8.kobject_uevent_env,热插拔事件处理相关的函数
{
	kobject_uevent.c
	/** 
	 * kobject_uevent_env - send an uevent with environmental data 
	 * 
	 * @action: action that is happening 
	 * @kobj: struct kobject that the action is happening to 
	 * @envp_ext: pointer to environmental data 
	 * 
	 * Returns 0 if kobject_uevent_env() is completed with success or the 
	 * corresponding error when it fails. 
	 */  
	int kobject_uevent_env(struct kobject *kobj, enum kobject_action action,  
					char *envp_ext[])  
	{  
		struct kobj_uevent_env *env;  
		const char *action_string = kobject_actions[action];  
		const char *devpath = NULL;  
		const char *subsystem;  
		struct kobject *top_kobj;  
		struct kset *kset;  
		const struct kset_uevent_ops *uevent_ops;  
		u64 seq;  
		int i = 0;  
		int retval = 0;  
	#ifdef CONFIG_NET  
		struct uevent_sock *ue_sk;  
	#endif  
		
		pr_debug("kobject: '%s' (%p): %s\n",  
				kobject_name(kobj), kobj, __func__);  
		
		/* search the kset we belong to */  
		top_kobj = kobj;  
		while (!top_kobj->kset && top_kobj->parent)  
			top_kobj = top_kobj->parent;  
		
		if (!top_kobj->kset) {  
			pr_debug("kobject: '%s' (%p): %s: attempted to send uevent "  
					"without kset!\n", kobject_name(kobj), kobj,  
					__func__);  
			return -EINVAL;  
		}  
		
		kset = top_kobj->kset;  
		uevent_ops = kset->uevent_ops;  
		
		/* skip the event, if uevent_suppress is set*/  
		if (kobj->uevent_suppress) {  
			pr_debug("kobject: '%s' (%p): %s: uevent_suppress "  
						"caused the event to drop!\n",  
						kobject_name(kobj), kobj, __func__);  
			return 0;  
		}  
		/* skip the event, if the filter returns zero. */  
		if (uevent_ops && uevent_ops->filter)  
			if (!uevent_ops->filter(kset, kobj)) {  
				pr_debug("kobject: '%s' (%p): %s: filter function "  
						"caused the event to drop!\n",  
						kobject_name(kobj), kobj, __func__);  
				return 0;  
			}  
		
		/* originating subsystem */  
		if (uevent_ops && uevent_ops->name)  
			subsystem = uevent_ops->name(kset, kobj);  
		else  
			subsystem = kobject_name(&kset->kobj);  
		if (!subsystem) {  
			pr_debug("kobject: '%s' (%p): %s: unset subsystem caused the "  
					"event to drop!\n", kobject_name(kobj), kobj,  
					__func__);  
			return 0;  
		}  
		
		/* environment buffer */  
		env = kzalloc(sizeof(struct kobj_uevent_env), GFP_KERNEL);  
		if (!env)  
			return -ENOMEM;  
		
		/* complete object path */  
		devpath = kobject_get_path(kobj, GFP_KERNEL);  
		if (!devpath) {  
			retval = -ENOENT;  
			goto exit;  
		}  
		
		/* default keys */  
		retval = add_uevent_var(env, "ACTION=%s", action_string);  
		if (retval)  
			goto exit;  
		retval = add_uevent_var(env, "DEVPATH=%s", devpath);  
		if (retval)  
			goto exit;  
		retval = add_uevent_var(env, "SUBSYSTEM=%s", subsystem);  
		if (retval)  
			goto exit;  
		
		/* keys passed in from the caller */  
		if (envp_ext) {  
			for (i = 0; envp_ext[i]; i++) {  
				retval = add_uevent_var(env, "%s", envp_ext[i]);  
				if (retval)  
					goto exit;  
			}  
		}  
		
		/* let the kset specific function add its stuff */  
		if (uevent_ops && uevent_ops->uevent) {  
			retval = uevent_ops->uevent(kset, kobj, env);  
			if (retval) {  
				pr_debug("kobject: '%s' (%p): %s: uevent() returned "  
						"%d\n", kobject_name(kobj), kobj,  
						__func__, retval);  
				goto exit;  
			}  
		}  
		
		/* 
			* Mark "add" and "remove" events in the object to ensure proper 
			* events to userspace during automatic cleanup. If the object did 
			* send an "add" event, "remove" will automatically generated by 
			* the core, if not already done by the caller. 
			*/  
		if (action == KOBJ_ADD)  
			kobj->state_add_uevent_sent = 1;  
		else if (action == KOBJ_REMOVE)  
			kobj->state_remove_uevent_sent = 1;  
		
		/* we will send an event, so request a new sequence number */  
		spin_lock(&sequence_lock);  
		seq = ++uevent_seqnum;  
		spin_unlock(&sequence_lock);  
		retval = add_uevent_var(env, "SEQNUM=%llu", (unsigned long long)seq);  
		if (retval)  
			goto exit;  
		
	#if defined(CONFIG_NET)  
		/* send netlink message */  
		mutex_lock(&uevent_sock_mutex);  
		list_for_each_entry(ue_sk, &uevent_sock_list, list) {  
			struct sock *uevent_sock = ue_sk->sk;  
			struct sk_buff *skb;  
			size_t len;  
		
			/* allocate message with the maximum possible size */  
			len = strlen(action_string) + strlen(devpath) + 2;  
			skb = alloc_skb(len + env->buflen, GFP_KERNEL);  
			if (skb) {  
				char *scratch;  
		
				/* add header */  
				scratch = skb_put(skb, len);  
				sprintf(scratch, "%s@%s", action_string, devpath); //action_string+devpath  
		
				/* copy keys to our continuous event payload buffer */  
				for (i = 0; i < env->envp_idx; i++) {  
					len = strlen(env->envp[i]) + 1;  
					scratch = skb_put(skb, len);  
					strcpy(scratch, env->envp[i]);  
				}  
		
				NETLINK_CB(skb).dst_group = 1;  
				retval = netlink_broadcast_filtered(uevent_sock, skb,  
									0, 1, GFP_KERNEL,  
									kobj_bcast_filter,  
									kobj);  
				/* ENOBUFS should be handled in userspace */  
				if (retval == -ENOBUFS)  
					retval = 0;  
			} else  
				retval = -ENOMEM;  
		}  
		mutex_unlock(&uevent_sock_mutex);  
	#endif  
		
		/* call uevent_helper, usually only enabled during early boot */  
		if (uevent_helper[0] && !kobj_usermode_filter(kobj)) {  
			char *argv [3];  
		
			argv [0] = uevent_helper;  
			argv [1] = (char *)subsystem;  
			argv [2] = NULL;  
			retval = add_uevent_var(env, "HOME=/");  
			if (retval)  
				goto exit;  
			retval = add_uevent_var(env,  
						"PATH=/sbin:/bin:/usr/sbin:/usr/bin");  
			if (retval)  
				goto exit;  
		
			retval = call_usermodehelper(argv[0], argv,  
								env->envp, UMH_WAIT_EXEC);  
		}  
		
	exit:  
		kfree(devpath);  
		kfree(env);  
		return retval;  
	}  


	
}






/********************************************************************************/
9.dump fuel gauge 
{
    dump.sh dump  fuel gauge寄存器   ./dump.sh > /data/kmsg.txt &
    {
	
	    let count=0

	    local utime
	    local ktime
	    local pause_time=10

	    if [ -n "$1" ]; then
		    pause_time=$1
	    fi


	    dump_peripheral () {
		    local base=$1
		    local size=$2
		    local dump_path=$3
		    echo $base > $dump_path/address
		    echo $size > $dump_path/count
		    cat $dump_path/data
	    }

	    echo "Starting dumps!"
	    echo "Dump path = $dump_path, pause time = $pause_time"

	    while true
	    do
		    utime=($(cat /proc/uptime))
		    ktime=${utime[0]}
		    echo "FG SRAM Dump Started at ${ktime}"
		    dump_peripheral 0 500 "/sys/kernel/debug/fg/sram"
		    uptime=($(cat /proc/uptime))
		    ktime=${utime[0]}
		    echo "FG SRAM Dump done at ${ktime}"
		    let count=$count+1
		    sleep $pause_time
	    done

    }	
	
    解析dump出来的寄存器 ,但是解析工具需要到windows环境暂时不行

}



/**************************************************************************************/
10.pmic的一个线程，这里主要是锁跟进程调度方面的用法
static void irq_thread_init(void)
{
	/* create pmic irq thread handler*/
	pmic_thread_handle = kthread_create(pmic_thread_kthread, (void *)NULL, "pmic_thread");
	if (IS_ERR(pmic_thread_handle)) {
		pmic_thread_handle = NULL;
		pr_err(PMICTAG "[pmic_thread_kthread] creation fails\n");
	} else {
		PMICLOG("[pmic_thread_kthread] kthread_create Done\n");
	}
}

int pmic_thread_kthread(void *x)
{
	unsigned int i;
	unsigned int int_status_val = 0;
#ifdef IPIMB
#else
	unsigned int pwrap_eint_status = 0;
#endif
	struct sched_param param = {.sched_priority = 98 };

	sched_setscheduler(current, SCHED_FIFO, &param);
	set_current_state(TASK_INTERRUPTIBLE);

	PMICLOG("[PMIC_INT] enter\n");

	pmic_enable_charger_detection_int(0);

	/* Run on a process content */
	while (1) {
		mutex_lock(&pmic_mutex);
#ifdef IPIMB
#else
		pwrap_eint_status = pmic_wrap_eint_status();
		PMICLOG("[PMIC_INT] pwrap_eint_status=0x%x\n", pwrap_eint_status);
#endif

		pmic_int_handler();

#ifdef IPIMB
#else
		pmic_wrap_eint_clr(0x0);
#endif

		for (i = 0; i < interrupts_size; i++) {
			int_status_val = upmu_get_reg_value(interrupts[i].address);
			PMICLOG("[PMIC_INT] after ,int_status_val[0x%x]=0x%x\n",
				interrupts[i].address, int_status_val);
		}

		/*mdelay(1); TBD*/

		mutex_unlock(&pmic_mutex);
		pmic_wake_unlock(&pmicThread_lock);

		set_current_state(TASK_INTERRUPTIBLE);
		enable_irq(g_pmic_irq);
		schedule();
	}

	return 0;
}



/********************************************************************************************/
11.sdm660获取申请注册中断

dtsi文件
	qcom,chgr@1000 {
		reg = <0x1000 0x100>;
		interrupts =
			<0x0 0x10 0x0 IRQ_TYPE_EDGE_RISING>,
			<0x0 0x10 0x1 IRQ_TYPE_EDGE_RISING>,
			<0x0 0x10 0x2 IRQ_TYPE_EDGE_RISING>,
			<0x0 0x10 0x3 IRQ_TYPE_EDGE_RISING>,
			<0x0 0x10 0x4 IRQ_TYPE_EDGE_RISING>;

		interrupt-names = "chg-error",
				  "chg-state-change",
				  "step-chg-state-change",
				  "step-chg-soc-update-fail",
				  "step-chg-soc-update-request";
	}

static struct smb_irq_info smb2_irqs[] = {
/* CHARGER IRQs */
	[CHG_ERROR_IRQ] = {
		.name		= "chg-error",
		.handler	= smblib_handle_debug,
	},
	[CHG_STATE_CHANGE_IRQ] = {
		.name		= "chg-state-change",
		.handler	= smblib_handle_chg_state_change,
		.wake		= true,
	},
	[STEP_CHG_STATE_CHANGE_IRQ] = {
		.name		= "step-chg-state-change",
		.handler	= NULL,
	},
	[STEP_CHG_SOC_UPDATE_FAIL_IRQ] = {
		.name		= "step-chg-soc-update-fail",
		.handler	= NULL,
	},
	[STEP_CHG_SOC_UPDATE_REQ_IRQ] = {
		.name		= "step-chg-soc-update-request",
		.handler	= NULL,
	},
/* OTG IRQs */
	[OTG_FAIL_IRQ] = {
		.name		= "otg-fail",
		.handler	= smblib_handle_debug,
	},
	[OTG_OVERCURRENT_IRQ] = {
		.name		= "otg-overcurrent",
		.handler	= smblib_handle_otg_overcurrent,
	},
	[OTG_OC_DIS_SW_STS_IRQ] = {
		.name		= "otg-oc-dis-sw-sts",
		.handler	= smblib_handle_debug,
	},
	[TESTMODE_CHANGE_DET_IRQ] = {
		.name		= "testmode-change-detect",
		.handler	= smblib_handle_debug,
	},
/* BATTERY IRQs */
	[BATT_TEMP_IRQ] = {
		.name		= "bat-temp",
		.handler	= smblib_handle_batt_temp_changed,
		.wake		= true,
	},
	[BATT_OCP_IRQ] = {
		.name		= "bat-ocp",
		.handler	= smblib_handle_batt_psy_changed,
	},
	[BATT_OV_IRQ] = {
		.name		= "bat-ov",
		.handler	= smblib_handle_batt_psy_changed,
	},
	[BATT_LOW_IRQ] = {
		.name		= "bat-low",
		.handler	= smblib_handle_batt_psy_changed,
	},
	[BATT_THERM_ID_MISS_IRQ] = {
		.name		= "bat-therm-or-id-missing",
		.handler	= smblib_handle_batt_psy_changed,
	},
	[BATT_TERM_MISS_IRQ] = {
		.name		= "bat-terminal-missing",
		.handler	= smblib_handle_batt_psy_changed,
	},
/* USB INPUT IRQs */
	[USBIN_COLLAPSE_IRQ] = {
		.name		= "usbin-collapse",
		.handler	= smblib_handle_debug,
	},
	[USBIN_LT_3P6V_IRQ] = {
		.name		= "usbin-lt-3p6v",
		.handler	= smblib_handle_debug,
	},
	[USBIN_UV_IRQ] = {
		.name		= "usbin-uv",
		.handler	= smblib_handle_usbin_uv,
	},
	[USBIN_OV_IRQ] = {
		.name		= "usbin-ov",
		.handler	= smblib_handle_debug,
	},
	[USBIN_PLUGIN_IRQ] = {
		.name		= "usbin-plugin",
		.handler	= smblib_handle_usb_plugin,
		.wake		= true,
	},
	[USBIN_SRC_CHANGE_IRQ] = {
		.name		= "usbin-src-change",
		.handler	= smblib_handle_usb_source_change,
		.wake		= true,
	},
	[USBIN_ICL_CHANGE_IRQ] = {
		.name		= "usbin-icl-change",
		.handler	= smblib_handle_icl_change,
		.wake		= true,
	},
	[TYPE_C_CHANGE_IRQ] = {
		.name		= "type-c-change",
		.handler	= smblib_handle_usb_typec_change,
		.wake		= true,
	},
/* DC INPUT IRQs */
	[DCIN_COLLAPSE_IRQ] = {
		.name		= "dcin-collapse",
		.handler	= smblib_handle_debug,
	},
	[DCIN_LT_3P6V_IRQ] = {
		.name		= "dcin-lt-3p6v",
		.handler	= smblib_handle_debug,
	},
	[DCIN_UV_IRQ] = {
		.name		= "dcin-uv",
		.handler	= smblib_handle_debug,
	},
	[DCIN_OV_IRQ] = {
		.name		= "dcin-ov",
		.handler	= smblib_handle_debug,
	},
	[DCIN_PLUGIN_IRQ] = {
		.name		= "dcin-plugin",
		.handler	= smblib_handle_dc_plugin,
		.wake		= true,
	},
	[DIV2_EN_DG_IRQ] = {
		.name		= "div2-en-dg",
		.handler	= smblib_handle_debug,
	},
	[DCIN_ICL_CHANGE_IRQ] = {
		.name		= "dcin-icl-change",
		.handler	= smblib_handle_debug,
	},
/* MISCELLANEOUS IRQs */
	[WDOG_SNARL_IRQ] = {
		.name		= "wdog-snarl",
		.handler	= NULL,
	},
	[WDOG_BARK_IRQ] = {
		.name		= "wdog-bark",
		.handler	= smblib_handle_wdog_bark,
		.wake		= true,
	},
	[AICL_FAIL_IRQ] = {
		.name		= "aicl-fail",
		.handler	= smblib_handle_debug,
	},
	[AICL_DONE_IRQ] = {
		.name		= "aicl-done",
		.handler	= smblib_handle_debug,
	},
	[HIGH_DUTY_CYCLE_IRQ] = {
		.name		= "high-duty-cycle",
		.handler	= smblib_handle_high_duty_cycle,
		.wake		= true,
	},
	[INPUT_CURRENT_LIMIT_IRQ] = {
		.name		= "input-current-limiting",
		.handler	= smblib_handle_debug,
	},
	[TEMPERATURE_CHANGE_IRQ] = {
		.name		= "temperature-change",
		.handler	= smblib_handle_debug,
	},
	[SWITCH_POWER_OK_IRQ] = {
		.name		= "switcher-power-ok",
		.handler	= smblib_handle_switcher_power_ok,
		.storm_data	= {true, 1000, 8},
	},
};

static int smb2_get_irq_index_byname(const char *irq_name)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(smb2_irqs); i++) {
		if (strcmp(smb2_irqs[i].name, irq_name) == 0)
			return i;
	}

	return -ENOENT;
}

static int smb2_request_interrupt(struct smb2 *chip,
				struct device_node *node, const char *irq_name)
{
	struct smb_charger *chg = &chip->chg;
	int rc, irq, irq_index;
	struct smb_irq_data *irq_data;

	irq = of_irq_get_byname(node, irq_name);
	if (irq < 0) {
		pr_err("Couldn't get irq %s byname\n", irq_name);
		return irq;
	}

	irq_index = smb2_get_irq_index_byname(irq_name);
	if (irq_index < 0) {
		pr_err("%s is not a defined irq\n", irq_name);
		return irq_index;
	}

	if (!smb2_irqs[irq_index].handler)
		return 0;

	irq_data = devm_kzalloc(chg->dev, sizeof(*irq_data), GFP_KERNEL);
	if (!irq_data)
		return -ENOMEM;

	irq_data->parent_data = chip;
	irq_data->name = irq_name;
	irq_data->storm_data = smb2_irqs[irq_index].storm_data;
	mutex_init(&irq_data->storm_data.storm_lock);

	rc = devm_request_threaded_irq(chg->dev, irq, NULL,
					smb2_irqs[irq_index].handler,
					IRQF_ONESHOT, irq_name, irq_data);
	if (rc < 0) {
		pr_err("Couldn't request irq %d\n", irq);
		return rc;
	}

	smb2_irqs[irq_index].irq = irq;
	smb2_irqs[irq_index].irq_data = irq_data;
	if (smb2_irqs[irq_index].wake)
		enable_irq_wake(irq);

	return rc;
}

static int smb2_request_interrupts(struct smb2 *chip)
{
	struct smb_charger *chg = &chip->chg;
	struct device_node *node = chg->dev->of_node;
	struct device_node *child;
	int rc = 0;
	const char *name;
	struct property *prop;

	for_each_available_child_of_node(node, child) {
		of_property_for_each_string(child, "interrupt-names",
					    prop, name) {
			rc = smb2_request_interrupt(chip, child, name);
			if (rc < 0)
				return rc;
		}
	}
	if (chg->irq_info[USBIN_ICL_CHANGE_IRQ].irq)
		chg->usb_icl_change_irq_enabled = true;

	return rc;
}



/**************************************************************************************/
12.python课程
{



}
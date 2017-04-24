





17G05A  软件关机重启,充电调试 (这个是重点),17G05A的mmi测试
/*debug*/
{
软件关机重启:解决



mmi测试：加入充电测试选项





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



按键，按键功能正常，先熟悉一下大致的工作流程
{
--->1.按键属于input子系统，所以有必要熟悉这个子系统的工作流程
cat/proc/bus/input/devices
{
    I: Bus=0019 Vendor=0000 Product=0000 Version=0000
    N: Name="ACCDET"
    P: Phys=
    S: Sysfs=/devices/virtual/input/input0
    U: Uniq=
    H: Handlers=gpufreq_ib event0 
    B: PROP=0
    B: EV=3
    B: KEY=40 0 0 0 0 0 0 1000000000 c000000000000 0

    I: Bus=0019 Vendor=2454 Product=6500 Version=0010
    N: Name="mtk-kpd"
    P: Phys=
    S: Sysfs=/devices/platform/10010000.keypad/input/input1
    U: Uniq=
    H: Handlers=gpufreq_ib event1 
    B: PROP=0
    B: EV=3
    B: KEY=1000000 0 b00000000 0 0 1c000000000000 0

    I: Bus=0000 Vendor=0000 Product=0000 Version=0000
    N: Name="gf-keys"
    P: Phys=
    S: Sysfs=/devices/virtual/input/input2
    U: Uniq=
    H: Handlers=gpufreq_ib event2 
    B: PROP=0
    B: EV=3
    B: KEY=3100000 100040000800 1c168000000000 0

    I: Bus=0019 Vendor=0000 Product=0000 Version=0000
    N: Name="aw9523b"
    P: Phys=
    S: Sysfs=/devices/virtual/input/input3
    U: Uniq=
    H: Handlers=gpufreq_ib event3 
    B: PROP=0
    B: EV=3
    B: KEY=2000019800100000 40000800 168000000000 4ffc

    I: Bus=0000 Vendor=0000 Product=0000 Version=0000
    N: Name="hwmdata"
    P: Phys=
    S: Sysfs=/devices/virtual/input/input4
    U: Uniq=
    H: Handlers=gpufreq_ib event4 
    B: PROP=0
    B: EV=5
    B: REL=6

    I: Bus=0000 Vendor=0000 Product=0000 Version=0000
    N: Name="m_alsps_input"
    P: Phys=
    S: Sysfs=/devices/virtual/input/input5
    U: Uniq=
    H: Handlers=gpufreq_ib event5 
    B: PROP=0
    B: EV=d
    B: REL=6
    B: ABS=101

    I: Bus=0000 Vendor=0000 Product=0000 Version=0000
    N: Name="m_acc_input"
    P: Phys=
    S: Sysfs=/devices/virtual/input/input6
    U: Uniq=
    H: Handlers=gpufreq_ib event6 
    B: PROP=0
    B: EV=d
    B: REL=c1
    B: ABS=107

    I: Bus=0000 Vendor=0000 Product=0000 Version=0000
    N: Name="m_mag_input"
    P: Phys=
    S: Sysfs=/devices/virtual/input/input7
    U: Uniq=
    H: Handlers=gpufreq_ib event7 
    B: PROP=0
    B: EV=d
    B: REL=3c9
    B: ABS=17f

    I: Bus=0000 Vendor=0000 Product=0000 Version=0000
    N: Name="m_step_c_input"
    P: Phys=
    S: Sysfs=/devices/virtual/input/input8
    U: Uniq=
    H: Handlers=gpufreq_ib event8 
    B: PROP=0
    B: EV=d
    B: REL=7
    B: ABS=100

    I: Bus=0000 Vendor=0000 Product=0000 Version=0000
    N: Name="mtk-tpd"
    P: Phys=
    S: Sysfs=/devices/virtual/input/input9
    U: Uniq=
    H: Handlers=gpufreq_ib event9 
    B: PROP=2
    B: EV=b
    B: KEY=10 0 0 0 400 0 0 100040000000 0 0
    B: ABS=263800001000003

    I: Bus=0019 Vendor=0001 Product=0001 Version=0100
    N: Name="mtk-tpd-kpd"
    P: Phys=
    S: Sysfs=/devices/virtual/input/input10
    U: Uniq=
    H: Handlers=gpufreq_ib event10 
    B: PROP=0
    B: EV=3
    B: KEY=10 0 0 0 0 0 0 100040000000 0 0

}


kpd.c
     内核创建的设备节点
     /devices/platform/10010000.keypad/input/input1

    struct keypad_dts_data {
        u32 kpd_key_debounce;
        u32 kpd_sw_pwrkey;
        u32 kpd_hw_pwrkey;
        u32 kpd_sw_rstkey;
        u32 kpd_hw_rstkey;
        u32 kpd_use_extend_type;
        u32 kpd_hw_map_num;
        u32 kpd_hw_init_map[72];
        u32 kpd_pwrkey_eint_gpio;
        u32 kpd_pwrkey_gpio_din;
        u32 kpd_hw_dl_key1;
        u32 kpd_hw_dl_key2;
        u32 kpd_hw_dl_key3;
        u32 kpd_hw_recovery_key;
        u32 kpd_hw_factory_key;
    }


    slide qwerty侧键
    kpd: key-debounce = 1024, sw-pwrkey = 116, hw-pwrkey = 8, hw-rstkey = 17, sw-rstkey = 115

    键盘相关的宏
    CONFIG_KEYBOARD_MTK=y
    CONFIG_ONEKEY_REBOOT_NORMAL_MODE=y
    CONFIG_ONEKEY_REBOOT_OTHER_MODE=y
    CONFIG_KPD_PWRKEY_USE_PMIC=y
    CONFIG_MTK_MRDUMP_KEY=y
    # CONFIG_KEYBOARD_ATKBD is not set

    键盘的中断回调函数
    kpd_irq_handler

    调用相关的工作函数
    kpd_keymap_handler
    
    /* bit is 1: not pressed, 0: pressed */




--->按键IC  aw9523b






  MTK统计apk发包数量脚本正常使用 
}








马达振动






关掉OTG功能








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


















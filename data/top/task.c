





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

键盘的代码执行流程：

    (platform.c) platform_init这个是很多平台相关的一个初始化 -> /*init kpd PMIC mode support*/ set_kpd_pmic_mode,是否支持外置键盘
    
    -> mtk_kpd_gpio_set设置键盘gpio的控制状态 ,这个地方应该是可以使能那些按键然后可以进入factory ，recovery模式 -> platform_boot_status
    
    系统启动的原因，powerkey，restart还是wdt引起的启动 -> (keypad.c) mtk_detect_key这个应该是检测主要的那几个物理按键powerkey还是rst之类的
    
    -> pmic_detect_homekey 检测powerkey是否按下 -> (boot_mode.c) boot_mode_select启动模式的选择 ...... -> (kpd.c)这个文件应该是键盘相
    
    关很重要的代码，kpd_mod_init这个是linux 驱动模块初始化 ->  kpd_pdrv_probe初始化kpd驱动，初始化kpd内存，哪些按键值要上报->kpd_irq_handler

    注册对应的中断回调函数，产生中断后调用注册的工作队列tasklet->kpd_keymap_handler,检测所有的按键，比较按键的状态跟之前是否有变化 -> 

    kpd_aee_handler 对VOLUMEUP，VOLUMEDOWN这两个键是按下还是松开处理并上报状态 -> (hal_kpd.c) mt_eint_register 注册一个中断在powerykey为
    
    一个中断的时候，而且这里面是一个dump操作 -> long_press_reboot_function_setting 正常模式还是其他 -> long_press_reboot_function_init_pmic
    
    这里是长按powerkey对应什么什么操作重启还是直接关机 -> gn_hall_eint_handler ,kpd_hall_eint_handler 这两个是霍尔开关，控制量实现开关的开闭

    -> aw9523b_mod_init 这是按键相关芯片的初始化 -> aw9523b_pinctrl_probe从dts文件里获取引脚的状态	pinctrl-0 = <&gpio_aw9523b_default>;
	
    pinctrl-1 = <&gpio_aw9523b_eint_as_int>; pinctrl-2 = <&gpio_aw9523b_reset_high>; pinctrl-3 = <&gpio_aw9523b_reset_low>;

    这是键盘IC控制的几个引脚的状态 -> aw9523b_i2c_probe 这是挂在I2C总线下的一个设备驱动 -> aw9523b_input_event_init 核心应该是这个，将底层事件上报

    给input子系统，注册设备驱动，可以上报哪些事件  -> set_p0_x_p1_y设置行列 -> aw9523b_init初始化 -> aw9523b_isr中断


   


kpd.c
     内核创建的设备节点
     platform总线下
     /sys/devices/platform/10010000.keypad/input/input1

     虚拟文件系统
     /sys/devices/virtual/input/

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


    static void kpd_keymap_handler(unsigned long data)
    {
        int i, j;
        bool pressed;
        u16 new_state[KPD_NUM_MEMS], change, mask;
        u16 hw_keycode, linux_keycode;

        kpd_get_keymap_state(new_state);

        wake_lock_timeout(&kpd_suspend_lock, HZ / 2);

        for (i = 0; i < KPD_NUM_MEMS; i++) {
            change = new_state[i] ^ kpd_keymap_state[i];
            if (!change)
                continue;

            for (j = 0; j < 16; j++) {
                mask = 1U << j;
                if (!(change & mask))
                    continue;

                hw_keycode = (i << 4) + j;

                if (hw_keycode >= KPD_NUM_KEYS)
                    continue;

                /* bit is 1: not pressed, 0: pressed */
                pressed = !(new_state[i] & mask);
                if (kpd_show_hw_keycode)
                    kpd_print("(%s) HW keycode = %u\n", pressed ? "pressed" : "released", hw_keycode);

                linux_keycode = kpd_keymap[hw_keycode];
                if (unlikely(linux_keycode == 0)) {
                    kpd_print("Linux keycode = 0\n");
                    continue;
                }
                kpd_aee_handler(linux_keycode, pressed);
                input_report_key(kpd_input_dev, linux_keycode, pressed);
                input_sync(kpd_input_dev);
                kpd_print("report Linux keycode = %u\n", linux_keycode);
            }
        }

        memcpy(kpd_keymap_state, new_state, sizeof(new_state));
        kpd_print("save new keymap state\n");
        enable_irq(kp_irqnr);
    }





    pmic_thread这个线程是干什么用的？

    linux驱动模块初始化在哪调用的？

    Hang_Detect是什么？


    可以通过检索Call trace，察看内存堆栈函数的调用

    accdet是什么
    这个是耳机检测用的

    MTK统计apk发包数量脚本正常使用 


硬件
--->按键IC  aw9523b  
kernel-4.4/drivers/input/keyboard/mediatek/aw9523b_gpio
  

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


















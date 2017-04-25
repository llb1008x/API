
mtk kpd 驱动

 一、kpd_pdrv_probe函数的分析：
[html] view plain copy
print?在CODE上查看代码片派生到我的代码片

      /*1. 输入设备实例  kpd_input_dev */  
    全局变量：static struct input_dev *kpd_input_dev;    
       
     static int kpd_pdrv_probe(struct platform_device *pdev)  
    {  
        int i, r;  
        u16 new_state[KPD_NUM_MEMS];  
        /* initialize and register input device (/dev/input/eventX) */  
      
      
      
    /*2. 初始化输入设备并分配内存空间*/  
        kpd_input_dev = input_allocate_device();                  
        if (!kpd_input_dev)  
            return -ENOMEM;  
      /*下面开始填充kpd_input_dev  设备驱动结构体*/  
        kpd_input_dev->name = KPD_NAME;  
        kpd_input_dev->id.bustype = BUS_HOST;  
        kpd_input_dev->id.vendor = 0x2454;  
        kpd_input_dev->id.product = 0x6575;  
        kpd_input_dev->id.version = 0x0010;  
        kpd_input_dev->open = kpd_open;  
      
      
      
      /*3. 设置某位为1，以第二个参数为起始地址，EV_KEY表示要设置的位  
      作用：告诉input子系统支持那些事件， EV_KEY 这里表示告诉input子系统支持  
      按键事件  
       
     */  
        __set_bit(EV_KEY, kpd_input_dev->evbit);               
      
      
    #if (KPD_PWRKEY_USE_EINT||KPD_PWRKEY_USE_PMIC)  
      
      
      /*4. 设置某位为1，以第二个参数为起始地址，EV_KEY表示要设置的位  
      作用：告诉input子系统支持那些按键， KPD_PWRKEY_MAP 这里表示告诉input子系统支持  
      电源按键  
      */  
        __set_bit(KPD_PWRKEY_MAP, kpd_input_dev->keybit);  
        kpd_keymap[8] = 0;  
    #endif  
        for (i = 17; i < KPD_NUM_KEYS; i += 9)   /* only [8] works for Power key */  
            kpd_keymap[i] = 0;  
      
      
        for (i = 0; i < KPD_NUM_KEYS; i++) {  
            if (kpd_keymap[i] != 0)  
                __set_bit(kpd_keymap[i], kpd_input_dev->keybit);  
        }  
        /*5. 上述几行代码表示设置电源按键 kpd_keymap 为0，其它按键 kpd_keymap 为1*/  
          
        __set_bit(250, kpd_input_dev->keybit);  
        __set_bit(251, kpd_input_dev->keybit);  
      
      
    #if KPD_AUTOTEST  
        for (i = 0; i < ARRAY_SIZE(kpd_auto_keymap); i++)  
            __set_bit(kpd_auto_keymap[i], kpd_input_dev->keybit);  
    #endif  
      
      
    #if KPD_HAS_SLIDE_QWERTY  
        __set_bit(EV_SW, kpd_input_dev->evbit);  
        __set_bit(SW_LID, kpd_input_dev->swbit);  
        __set_bit(SW_LID, kpd_input_dev->sw);    /* 1: lid shut => closed */  
    #endif  
      
      
    #ifdef KPD_PMIC_RSTKEY_MAP  
        __set_bit(KPD_PMIC_RSTKEY_MAP, kpd_input_dev->keybit);  
    #endif  
      
      
        /*6. 指定kpd_input_dev这个平台设备sysfs中的父设备节点*/  
        kpd_input_dev->dev.parent = &pdev->dev;         
        /*7. 注册input输入子系统*/  
        r = input_register_device(kpd_input_dev);   
        if (r) {  
            printk(KPD_SAY "register input device failed (%d)\n", r);  
            input_free_device(kpd_input_dev);  
            return r;  
        }  
      
      
        /* register device (/dev/mt6575-kpd) */  
        /*7. 指定kpd_dev这个平台设备sysfs中的父设备节点*/  
        kpd_dev.parent = &pdev->dev;                   
        /*8. 注册混杂设备*/  
        r = misc_register(&kpd_dev);                          
        if (r) {  
            printk(KPD_SAY "register device failed (%d)\n", r);  
            input_unregister_device(kpd_input_dev);  
            return r;  
        }  
          
        /*8. 注册按键中断*/  
        /* register IRQ and EINT */  
        /*9. 设置消抖时间*/  
        kpd_set_debounce(KPD_KEY_DEBOUNCE);           
        /*10. 设置中断触发方式*/  
        mt65xx_irq_set_sens(MT6575_KP_IRQ_ID, MT65xx_EDGE_SENSITIVE);         
        /*11 . 设置中断优先级*/  
        mt65xx_irq_set_polarity(MT6575_KP_IRQ_ID, MT65xx_POLARITY_LOW);   
        /*12. 注册中断处理函数*/  
        r = request_irq(MT6575_KP_IRQ_ID, kpd_irq_handler, 0, KPD_NAME, NULL);    
        if (r) {  
            printk(KPD_SAY "register IRQ failed (%d)\n", r);  
            misc_deregister(&kpd_dev);  
            input_unregister_device(kpd_input_dev);  
            return r;  
        }  
        /*13. 以下为电源键中断函数的注册*/  
    #if KPD_PWRKEY_USE_EINT  
        mt65xx_eint_set_sens(KPD_PWRKEY_EINT, KPD_PWRKEY_SENSITIVE);  
        mt65xx_eint_set_hw_debounce(KPD_PWRKEY_EINT, KPD_PWRKEY_DEBOUNCE);  
        mt65xx_eint_registration(KPD_PWRKEY_EINT, true, KPD_PWRKEY_POLARITY,  
                                 kpd_pwrkey_eint_handler, false);  
    #endif  
      
      
        if(kpd_enable_lprst && get_boot_mode() == NORMAL_BOOT) {  
            kpd_print("Normal Boot\n");  
    #ifdef KPD_PMIC_LPRST_TD  
            kpd_print("Enable LPRST\n");  
        /*14. 以下为设置按键唤醒的时间*/  
            upmu_testmode_pwrkey_rst_en(0x01);  
            upmu_testmode_homekey_rst_en(0x01);  
            upmu_testmode_pwrkey_rst_td(KPD_PMIC_LPRST_TD);  
    #endif  
        } else {  
            kpd_print("Disable LPRST %d\n", kpd_enable_lprst);  
        }  
        /*15. 设置一个高精度定时器，并且定义了时间到期的回调函数 aee_timer_func*/  
        hrtimer_init(&aee_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);  
        aee_timer.function = aee_timer_func;  
          
          
        /*以下为三个按键的初始化，也就是配置  
        注意，默认值gpio输出是0*/  
    #if 1 // ylliu add. dct default value does not work...  
        /* KCOL0: GPIO103: KCOL1: GPIO108, KCOL2: GPIO105, KCOL4: GPIO102 input + pull enable + pull up */  
        mt_set_gpio_mode(GPIO_KPD_KCOL0_PIN, GPIO_KPD_KCOL0_PIN_M_KP_COL);  
        mt_set_gpio_dir(GPIO_KPD_KCOL0_PIN, GPIO_DIR_IN);  
        mt_set_gpio_pull_enable(GPIO_KPD_KCOL0_PIN, GPIO_PULL_ENABLE);  
        mt_set_gpio_pull_select(GPIO_KPD_KCOL0_PIN, GPIO_PULL_UP);  
      
      
          
        mt_set_gpio_mode(GPIO_KPD_KCOL1_PIN, GPIO_KPD_KCOL1_PIN_M_KP_COL);  
        mt_set_gpio_dir(GPIO_KPD_KCOL1_PIN, GPIO_DIR_IN);  
        mt_set_gpio_pull_enable(GPIO_KPD_KCOL1_PIN, GPIO_PULL_ENABLE);  
        mt_set_gpio_pull_select(GPIO_KPD_KCOL1_PIN, GPIO_PULL_UP);  
          
        mt_set_gpio_mode(GPIO_KPD_KCOL2_PIN, GPIO_KPD_KCOL2_PIN_M_KP_COL);  
        mt_set_gpio_dir(GPIO_KPD_KCOL2_PIN, GPIO_DIR_IN);  
        mt_set_gpio_pull_enable(GPIO_KPD_KCOL2_PIN, GPIO_PULL_ENABLE);  
        mt_set_gpio_pull_select(GPIO_KPD_KCOL2_PIN, GPIO_PULL_UP);  
          
        mt_set_gpio_mode(GPIO_KPD_KCOL4_PIN, GPIO_KPD_KCOL4_PIN_M_KP_COL);  
        mt_set_gpio_dir(GPIO_KPD_KCOL4_PIN, GPIO_DIR_IN);  
        mt_set_gpio_pull_enable(GPIO_KPD_KCOL4_PIN, GPIO_PULL_ENABLE);  
        mt_set_gpio_pull_select(GPIO_KPD_KCOL4_PIN, GPIO_PULL_UP);  
          
          
        /* KROW0: GPIO98, KROW1: GPIO97: KROW2: GPIO95 output + pull disable + pull down */  
        mt_set_gpio_mode(GPIO_KPD_KROW0_PIN, GPIO_KPD_KROW0_PIN_M_KP_ROW);  
        mt_set_gpio_dir(GPIO_KPD_KROW0_PIN, GPIO_DIR_OUT);  
        mt_set_gpio_pull_enable(GPIO_KPD_KROW0_PIN, GPIO_PULL_DISABLE);   
        mt_set_gpio_pull_select(GPIO_KPD_KROW0_PIN, GPIO_PULL_DOWN);  
          
    //  mt_set_gpio_mode(97, 1);  
    //  mt_set_gpio_dir(97, 1);  
    //  mt_set_gpio_pull_enable(97, 0);   
    //  mt_set_gpio_pull_select(97, 0);  
    //    
    //  mt_set_gpio_mode(95, 1);  
    //  mt_set_gpio_dir(95, 1);  
    //  mt_set_gpio_pull_enable(95, 0);       
    //  mt_set_gpio_pull_select(95, 0);  
    #endif  
          
        // default disable backlight. reboot from recovery need this.  
        kpd_disable_backlight();  
          
        // store default state, resolve recovery bugs.  
        kpd_get_keymap_state(new_state);  
        memcpy(kpd_keymap_state, new_state, sizeof(new_state));  
          
        return 0;  
    }  




 
 
二、当执行完面probe函数进行相关初始化后，这时候，当我们按键按下了，就会触发中断，进入中断服务子程序
[html] view plain copy
print?在CODE上查看代码片派生到我的代码片

    static irqreturn_t __tcmfunc kpd_irq_handler(int irq, void *dev_id)  
    {  
        /* use _nosync to avoid deadlock */  
        disable_irq_nosync(MT6575_KP_IRQ_ID);  
        tasklet_schedule(&kpd_keymap_tasklet);  
        return IRQ_HANDLED;  
    }  




可以看到，中断服务程序里面执行了 tasklet_schedule(&kpd_keymap_tasklet);
跟踪代码可以发现，实际上是执行了这个函数kpd_keymap_handler，下面仔细分析
这个函数，详细注释如下：


[html] view plain copy
print?在CODE上查看代码片派生到我的代码片

    static void kpd_keymap_handler(unsigned long data)  
    {  
        int i, j;  
        bool pressed;  
        u16 new_state[KPD_NUM_MEMS], change, mask;  
        u16 hw_keycode, linux_keycode;  
        kpd_get_keymap_state(new_state); //首先读取键值，并且存放于new_state中  
      
      
        if (pmic_get_acc_state() == 1) {  
        for (i = 0; i < KPD_NUM_MEMS; i++) {  
            change = new_state[i] ^ kpd_keymap_state[i];//进行异或操作，就是为了取出两者不同的值  
            if (!change)  
                continue;  
      
      
            for (j = 0; j < 16; j++) {  
                mask = 1U << j;  
                if (!(change & mask))  
                    continue;  
      
      
                hw_keycode = (i << 4) + j;        //i = 0, j = 1;   //这里是得到hw_keycode的值  
                printk("hw_keycode = %d ,i = %d, j = %d \n",hw_keycode,i,j);  
                /* bit is 1: not pressed, 0: pressed */  
                pressed = !(new_state[i] & mask);   //(new_state[i] & mask) = 0  
                if (kpd_show_hw_keycode) {  
                    printk(KPD_SAY "(%s) HW keycode = %u\n",  
                           pressed ? "pressed" : "released",  
                           hw_keycode);  
                }  
                BUG_ON(hw_keycode >= KPD_NUM_KEYS);  
                linux_keycode = kpd_keymap[hw_keycode];    //这里的linux_keycode恒为零。  
                printk("linux_keycode = %d  \n",linux_keycode);  
                  
                if(unlikely(linux_keycode == 0)) {  
                    if (hw_keycode == 1 && pressed) { // special key, SOS.  
                        struct device *dev = &(kpd_input_dev->dev);  
                        char *envp[] = { "SOS_pressed", NULL };  
                        kobject_uevent_env(&dev->kobj, KOBJ_CHANGE, envp);   //建立设备文件？  
                        printk(KPD_SAY "SOS_pressed\n");  
                        // used by recovery.  
                        /*这个接口会向INPUT子系统上报按键（该按键被按下）*/  
                        input_report_key(kpd_input_dev, 251, pressed);     //如果上层检测到SOS_pressed就会做相应处理。          
                    } else if (hw_keycode == 2 && pressed) { // special key, background.  
                        struct device *dev = &(kpd_input_dev->dev);  
                        char *envp[] = { "background_pressed", NULL };  
                        kobject_uevent_env(&dev->kobj, KOBJ_CHANGE, envp);  
                        printk(KPD_SAY "background_pressed\n");  
                        // used by recovery.  
                        input_report_key(kpd_input_dev, 8, pressed);  
                    } else if (hw_keycode == 4 && pressed) { // special key, mode.  
                        struct device *dev = &(kpd_input_dev->dev);  
                        char *envp[] = { "mode_pressed", NULL };  
                        kobject_uevent_env(&dev->kobj, KOBJ_CHANGE, envp);  
                        printk(KPD_SAY "mode_pressed\n");  
                    } else if (hw_keycode == 1 || hw_keycode == 2 || hw_keycode == 4) { // add this to turn off backlight.  
                        printk(KPD_SAY "background or SOS or mode release!\n");  
                        // used by recovery.  
                        if (hw_keycode == 1)  
                            input_report_key(kpd_input_dev, 251, pressed);  
                        else if (hw_keycode == 2)  
                            input_report_key(kpd_input_dev, 8, pressed);  
                    } else {  
                        kpd_print("Linux keycode = 0\n");  
                        continue;  
                    }  
                }  
                kpd_aee_handler(linux_keycode, pressed);      
                kpd_backlight_handler(pressed, linux_keycode);  
                input_report_key(kpd_input_dev, linux_keycode, pressed);  
            }  
        }  
        } else {  
            printk(KPD_SAY "acc off, ignore and key...\n");  
        }  
          
        memcpy(kpd_keymap_state, new_state, sizeof(new_state));  
      
      
        kpd_print("save new keymap state\n");  
        enable_irq(MT6575_KP_IRQ_ID);  
    }  



三、kpd_aee_handler函数分析
[html] view plain copy
print?在CODE上查看代码片派生到我的代码片

    static void kpd_aee_handler(u32 keycode, u16 pressed) {  
        if(pressed) {  
            if(keycode == KEY_VOLUMEUP) {  
                __set_bit(0, &aee_pressed_keys);  
            } else if(keycode == KEY_VOLUMEDOWN) {  
                __set_bit(1, &aee_pressed_keys);  
            } else {  
                return;  
            }  
            kpd_update_aee_state();  
        } else {  
            if(keycode == KEY_VOLUMEUP) {  
                __clear_bit(0, &aee_pressed_keys);  
            } else if(keycode == KEY_VOLUMEDOWN) {  
                __clear_bit(1, &aee_pressed_keys);  
            } else {  
                return;  
            }  
            kpd_update_aee_state();  
        }  
    }  



详细分析： 
1.__set_bit(0, &aee_pressed_keys)，定义了一个：static u16 aee_pressed_keys;
所以__set_bit的意思是将aee_pressed_keys的bit0设置为1
2.相应的__clear_bit(0, &aee_pressed_keys);就是把aee_pressed_keys的bit0清零，
3.还有在内核的non-atomic.h文件中还有一些其它的位操作，记住__set_bit和set_bit的区别就
是前者是非原子操作，而后者是原子操作，所谓原子操作，意思是最小的执行单位，再其执行过
程中是不会被其他任务打断的。


四、背光处理函数
[html] view plain copy
print?在CODE上查看代码片派生到我的代码片

    void kpd_backlight_handler(bool pressed, u16 linux_keycode)  
    {     
        if (kpd_suspend && !test_bit(linux_keycode, kpd_wake_keybit)) {  
            kpd_print("Linux keycode %u is not WAKE key\n", linux_keycode);  
            return;  
        }  
        /* not in suspend or the key pressed is WAKE key */  
        if (pressed) {  
            atomic_inc(&kpd_key_pressed);  
            kpd_backlight_on = !!atomic_read(&kpd_key_pressed);  
            schedule_work(&kpd_backlight_work);     //点亮背光灯  
            kpd_print("switch backlight on\n");  
        } else {  
            atomic_dec(&kpd_key_pressed);  
            mod_timer(&kpd_backlight_timer,             //KPD_BACKLIGHT_TIME控制背光时间，单位为sec，如果注释掉这句，背光将不灭  
                      jiffies + KPD_BACKLIGHT_TIME * HZ);  
            kpd_print("activate backlight timer\n");  
        }  
    }  



详细分析
1.首先用到了一个位操作函数，注意这个函数是原子操作test_bit
2.全局变量static atomic_t kpd_key_pressed = ATOMIC_INIT(0);这是原子操作的初始化，kpd_key_pressed初始化为0
3.上述函数涉及到一些原子操作函数，解释如下：
atomic_inc(&kpd_key_pressed); 是对变量进行加1操作
atomic_dec(&kpd_key_pressed); 是对变量进行减1操作
!!atomic_read(&kpd_key_pressed);是读取变量的值，前面两个 !!强调该返回值不是1就是0：bool类型
4.mod_timer：该函数的作用是修改一个已经调度的定时器结构的到期时间。


五、背光控制函数


调度的是这个函数
[html] view plain copy
print?在CODE上查看代码片派生到我的代码片

    static void kpd_switch_backlight(struct work_struct *work)  
    {  
        if (kpd_backlight_on) {  
            kpd_enable_backlight();  
            kpd_print("backlight is on\n");  
        } else {  
            kpd_disable_backlight();  
            kpd_print("backlight is off\n");  
        }  
    }  



这里就能够看到使能和失能背光的函数，继续跟踪：
[html] view plain copy
print?在CODE上查看代码片派生到我的代码片

    void kpd_enable_backlight(void)  
    {  
        /*mt6326_kpled_dim_duty_Full();  
        mt6326_kpled_Enable();*/  
        upmu_kpled_dim_duty(31);      
        upmu_kpled_en(1);  
    }  
    upmu_kpled_dim_duty这是控制背光电流大小从而可以控制亮度  
    upmu_kpled_en这是控制开关。  
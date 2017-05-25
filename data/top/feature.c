/************************************************************************
                        添加新的特性
*************************************************************************/


/*可以设置充电电量的上下限*/
{
    ->笔记本电脑的一种充电方式
    {
        如何调整笔记本电池充电最低百分比?
        http://www.3lian.com/edu/2013/05-13/69533.html


        就是设置两个充电阈值，低于多少开始充电，高于多少停止充电

        主要是针对那些频繁插拔充电器的，减少充电次数，延长电池寿命       

    -->
        1、充电阈值设置，仅在自带或后期安装的电池管理软件运行条件下有效;

    　　2、充电终止百分比至少要比充电起始百分比高4%;

    　　3、由于电池老化，一般将充电阈值的设置比理想值高2%;

    　　4、当充电停止百分比小于100%时，请每三个月对电池进行一次100%充电(平时应尽量避免对电池进行完全充电和完全放电)。

    }



->所以这里的想法是如何实现设定充电的阈值
{
    在标准充电器插入的时候根据设定的阈值充电，USB充电的时候不执行这个策略

    1.数据结构里面添加设定的上限，下限

    2.初始化的时候设定一个默认的值

    3.判断充电器类型标准充电器还是非标充，就是说根据检测到的充电器的类型

    4.上层对设备节点的写0,1决定对上下限写入的权限


    扩展想法是如何创建一个内核线程，并运行控制

}




控制逻辑
{
    1.创建设备节点state,down,up;state控制开关打开关闭，down控制下限，up控制上限

    2.打开state，可以往上下限中写入值，当然这个值要经过判断
    默认值条件
    gn_switch_charging_down=60；
    gn_switch_charging_up=100;
    gn_switch_charging_down <  gn_switch_charging_up  ||  gn_switch_charging_up > 100 
    
    3.充电使能那边检测控制充电的状态，在满足条件的状态下充电，不满足不充电
    条件：满足标充，开关打开
        a.低于阈值，充电
        b.高于阈值，停止充电
        c.介于阈值之间，如果是充电之后达到这个条件的继续充电，如果手机本来电量就是介于之间则不充电


    4.上报的逻辑状态涉及哪些状态量 
    最简单的一种办法就是上报充电状态为错误
    BMT_status.bat_charging_state == CHR_ERROR;  

    5.现在的问题是充电充到上限之后可以停止充电，但是因为上报的是error状态，而这个状态在之后电量下降到下限之后
    并没有重新充电(状态为error，所以会停止充电)
        所以这里需要在检测到电量低于下限的时候将充电状态改变为CHR_PRE，使其不会保持为error，可以恢复充电，同时
    电池充到上限的时候一些计数变量应该重置


    6.电量降到80%一下的时候，没有回充，导致整晚电池耗电至关机，第二天插充电器后重启
    pchr_turn_on_charging这个函数没有走，但是插着充电线就会走那个函数
    
    将判断流程移到充电流程状态机函数中插着充电线都会走这个


    7.这是后还有一个问题就是
    如果系统深度睡眠，充电器不充电，而电池还在消耗的情况下将会变成dead battery，
    从的c这种情况是否会引起问题





}



->相关的代码
 {

/*****************************************************************************
            电池的数据结构
 ****************************************************************************/
    typedef struct {
        kal_bool bat_exist;
        kal_bool bat_full;
        signed int bat_charging_state;
        unsigned int bat_vol;
        kal_bool bat_in_recharging_state;
        unsigned int Vsense;
        kal_bool charger_exist;
        unsigned int charger_vol;
        signed int charger_protect_status;
        signed int ICharging;
        signed int IBattery;
        signed int temperature;
        signed int temperatureR;
        signed int temperatureV;
        unsigned int total_charging_time;
        unsigned int PRE_charging_time;
        unsigned int CC_charging_time;
        unsigned int TOPOFF_charging_time;
        unsigned int POSTFULL_charging_time;
        unsigned int charger_type;
        signed int SOC;
        signed int UI_SOC;
        signed int UI_SOC2;
        unsigned int nPercent_ZCV;
        unsigned int nPrecent_UI_SOC_check_point;
        unsigned int ZCV;
 } PMU_ChargerStruct;



 }

































/*type-C接口上添加OTG开关*/
{
OTG的引脚 ，怎么在设备树里面添加状态，哪个是控制OTG状态的，如何控制开关的逻辑，整个的思维过程
使用的是bq24196

    1.没有通过dtsi文件，那就是通过头文件，然后直接设置状态的

    pin脚对应的几个模式
    #define PI5USB_INT_PIN          GPIO_IO_USB_EINT_PIN
    #define PI5USB_INT_PIN_MODE     GPIO_IO_USB_EINT_PIN_M_EINT
    #define PI5USB_INT_NUM          CUST_EINT_IO_USB_NUM
    #define PI5USB_RESET_PIN        GPIO_IO_SW_EN_USB
    #define PI5USB_RESET_PIN_MODE   GPIO_IO_SW_EN_USB_M_GPIO

    对gpio的操作
    mt_set_gpio_mode(PI5USB_INT_PIN, PI5USB_INT_PIN_MODE);
    mt_set_gpio_dir(PI5USB_INT_PIN, GPIO_DIR_IN);
    mt_set_gpio_pull_enable(PI5USB_INT_PIN, GPIO_PULL_ENABLE);
    mt_set_gpio_pull_select(PI5USB_INT_PIN, GPIO_PULL_UP);

    读取寄存器上的状态
    static int current_mode = 0x4;
    先关闭中断使能，然后读取寄存器
    pi5usb_write(client, current_mode + 0x1);//Disable interrupt
	msleep(30);//Delay 20ms ~ 45ms
	pi5usb_read(client, reg_val, REG_COUNT);

    reg_val[2],0x01 or 0x00
    reg_val[3]=0x05,0x06 OTG insert


    current_mode = def_mode?
    current_mode :0x0,0x02,0x04
    def_mode=0x04




    if(1 == audio_accessory_flag || 1 == debug_accessory_flag)
    {
        msleep(100);
    }else if(repeattimes >= 3)
    {
        repeattimes = 0;
        printk("VBUS in, no CC \n");    ？？
    }
    else
    {
        repeattimes++;
        msleep(240);
        pi5usb_write(client, 0x1);
        msleep(100);
//		pi5usb_write(client, 0x4);
    }




    
    2.OTG的工作流程：
    (pi5usb30216.c)pi5usb_i2c_probe,在I2C总线上注册一个设备申请中断，创建一个内核线程pi5usb_event_handler->pi5usb_event_handler ,
    while(1),等待中断的到来，->pi5usb_process ,中断来了，就读写寄存器，根据寄存器的不同位判断处于什么状态->pi5usb_try_snk，当有设备插入的时候 
    就执行这个，按意思来说应该说是等待设备准备好状态，有延时同时读取不同的寄存器判断,是否有热插拔或者其他外设




    pi5usb type-C的操作流程
    1. INTB asserted LOW, indicating Type-C port status change.
    将电平拉低检测type-C引脚的状态
    2. Processor first masks PI5USB30216A interrupt by writing a ‘1’ to Bit 0 of Control Register(0x02). INTB returned
    Hi-Z.
    通过掩码返回中断的状态
    3. Delay 30ms.
    延时
    4. Processor then read Register(0x01), Control Register (0x02), Interrupt Register(0x03) and CC Status
    Register(0x04). Interrupt Register(0x03) indicates if an attach or detach event was detected. All interrupt flags in
    Interrupt Register will be cleared after the I2C read action. CC Status Register(0x04) is used to determine plugin
    details and charging profile. Processor can configure the power and USB channels according to information in
    CC Status Register.
    0x01:只读寄存器
    0x02:控制寄存器
    0x03:中断
    0x04:CC状态寄存器
    当读了I2C的值之后就会清零
    5. Processor unmask PI5USB30216A interrupt by writing a ‘0’ to Bit 0 of Address 0x02 before ending the interrupt
    service routine
    在执行中断工作函数的之后会操作掩码，然后结束中断工作函数

    控制type-C使能来使能OTG开关，主要思路应该是这个，就是将电平拉高关闭使能，同时I2C也不能控制
    PI5USB30216A is reset only by Power-On Reset (POR). When ENB is float or is pulled high, PI5USB30216 is
    disabled and its I2C is not accessible. Whenever users pull low ENB to enable part, PI5USB30216A should be reset
    through below I2C sequence:

    type-C的两种工作状态
    1. In DRP mode, PI5USB30216A is reset by writing Reg[0x02]=01h, then delay 30ms followed by writing
    Reg[0x02]=04h/0Ch/14h. Please see Figure 6a.
    DRP模式就是从主设备模式，下行口
    2. In Source/DFP mode, PI5USB30216A is reset by writing Reg[0x02]=01h, then delay 30ms followed by writing
    Reg[0x02]=02h/0Ah/12h. Please see Figure 6b.
    DFP双端口模式，主从设备可以切换
    3. In Sink/UFP mode, PI5USB30216A is reset by writing Reg[0x02]=05h, then delay 30ms followed by writing
    Reg[0x02]=00h/20h. Please see Figure 6c
    UFP：从设备模式，上行口




    所以这里有几个关键地方：
    1.控制iddig引脚中断使能，还是上面讲的写寄存器，让中断跟I2C都失效
    中断是哪个？原始代码应该是USB检测的中断，实际不是
    usb20.h 里面定义
    //#define IDDIG_EINT_PIN (GPIO_OTG_IDDIG_EINT_PIN & ~(0x80000000))

    OTG中断使能是iddig引脚，iddig引脚是哪个？
    中断62

    otg_cc_flag和suspend_flag功能有点重合， 但是应该是逻辑控制的关键  



    设备节点创建的有问题，而这个节点跟下面的平台也有关系
    这是挂载在I2C上的设备，可以通过i2c_board_info查找设备名称和地址
    /sys/bus/i2c/devices/



    2.一个是USB3.0，一个是USB2.0代码结构不一样
    USB2.0->usb20_host.c           USB3.0->xhci-mtk-driver.c
    所以这时后要对比两个平台的特性，和工作方式


    3.完备的代码逻辑
    这时候要考虑一些可能的情况,所以在写1，写0的时候要考虑之前的状态，还有就是反复写1，写0
    otg_cc_flag：
    suspend_flag：

    {
        还有另一点就是OTG开关是为了解决IO腐蚀，所以开关关的时候要使iddig引脚为低电平
        开关打开的时候，让中断触发方式改为高电平触发，iddig设置成低

        所以现在的问题就是关闭开关的时候iddig电平情况
        开关的状态和iddig ，CC1，CC2电平的变化

        关于type-c最重要的是控制CC1，CC2引脚
        OTG开关打开的时候插入线时CC1，CC2有方波信号，循环检测，插OTG的时候CC为0.38V电压。
        关闭开关的时候CC引脚为低电平，此时虽然iddig引脚为高电平，但是不在尾座上腐蚀不到，不会有影响
    }




    两种解决方案：直接触发中断，改变电平触发方式（模拟假插入，拔除）
    a:直接出发中断
    b:改变点评触发方式






    4.设备节点的创建和控制
    软链接
    symlink /sys/bus/i2c/devices/3-001d		/otg 
    chown system system /otg/otg_mode




    S8  通过设备树操作GPIO
    关闭iddig中断使能
    pinctrl_select_state(pinctrl_usbc, en_output1);
    打开iddig中断使能
    pinctrl_select_state(pinctrl_usbc, en_output0);
    pi5usb_reset_i2c(client);


    7505 直接控制GPIO
    关闭iddig中断使能
    mt_set_gpio_mode(PI5USB_RESET_PIN, PI5USB_RESET_PIN_MODE);
	mt_set_gpio_dir(PI5USB_RESET_PIN, GPIO_DIR_OUT);
	mt_set_gpio_out(PI5USB_RESET_PIN, 1);
    suspend_flag = 1;

    打开iddig中断使能
    mt_set_gpio_mode(PI5USB_RESET_PIN, PI5USB_RESET_PIN_MODE);
	mt_set_gpio_dir(PI5USB_RESET_PIN, GPIO_DIR_OUT);
	mt_set_gpio_out(PI5USB_RESET_PIN, 0);
	pi5usb_reset_i2c(client);
	suspend_flag = 0;

}
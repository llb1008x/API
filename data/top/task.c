





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

->IC:drv2604l


基本概念
{
    LRA (Linear Resonance Actuator) 线性制动器

    ERM (Eccentric Rotating Mass) 偏转转动惯量

    Back-EMF detection 反电动势检测

    actuator n.激励者； [电脑]执行机构； [电]（电磁铁）螺线管； [机]促动器

    braking  n. 刹车，制动，（用闸）减速；
            v. 刹（车）( brake的现在分词 )；

    calibration  n. 校准，标准化； 刻度，标度； 测量口径；变形         

    resonant adj.洪亮的； 回响的； 共振的； 能共鸣的


    The ERM_LRA bit in register 0x1A must be
    configured to select the type of actuator that the device uses.

    The smart-loop architecture makes the resonant frequency of the LRA available through I2C (see the LRA
    Resonance Period (Address: 0x22) section)

    A key feature of the DRV2604L is the “smart-loop architecture” which employs actuator feedback control for both
    ERMs and LRAs. The feedback control desensitizes the input waveform from the motor-response behavior by
    providing automatic overdrive and automatic braking.

    The FB_BRAKE_FACTOR[2:0] bits can be adjusted to set the brake factor.

    the start-time characteristic may be different for each actuator, the AUTO_CAL_TIME[1:0] bit can change the duration of the
    automatic level-calibration routine to optimize calibration performance.






[   42.657878] <0>.(0)[947:debuggerd][name:primary_display&][DISP][primary_display_trigger_nolock #4861]ERROR:primary_display_trigger_nolock, skip because primary dipslay is sleep


}



static void drv2604l_change_mode(struct DRV2604L_data *pDrv2604ldata, char work_mode, char dev_mode)
这个里面有两个mode ，work和dev，什么意思？

->work_mode
    #define MODE_REG                    0x01        //也就是说这个是控制震动效果的
    The DRV2604L device offers multiple ways to launch and control haptic effects. The MODE[2:0] bit in register
0x01 is used to select the interface mode

    #define MODE_STANDBY_MASK           0x40
    #define MODE_STANDBY                0x40
    #define MODE_RESET                  0x80
    #define DRV2604_MODE_MASK           0x07
    #define MODE_INTERNAL_TRIGGER       0
    #define MODE_EXTERNAL_TRIGGER_EDGE  1
    #define MODE_EXTERNAL_TRIGGER_LEVEL 2
    #define MODE_PWM_OR_ANALOG_INPUT    3
    #define MODE_AUDIOHAPTIC            4
    #define MODE_REAL_TIME_PLAYBACK     5
    #define MODE_DIAGNOSTICS            6
    #define AUTO_CALIBRATION            7

->dev_mode
    #define	WORK_IDLE					0x00
    #define WORK_RTP			      	0x06
    #define WORK_CALIBRATION	      	0x07
    #define WORK_VIBRATOR		      	0x08
    #define	WORK_PATTERN_RTP_ON			0x09
    #define WORK_PATTERN_RTP_OFF      	0x0a
    #define WORK_SEQ_RTP_ON		      	0x0b
    #define WORK_SEQ_RTP_OFF    	  	0x0c
    #define WORK_SEQ_PLAYBACK    	  	0x0d


    #define DEV_IDLE	                0 // default
    #define DEV_STANDBY					1
    #define DEV_READY					2

    #define DRV2604L_I2C_BUS_ID         4
    #define DRV2604L_I2C_ADDR			0x5A




代码调用的流程：


{
    充电器插入的时候会调用马达震动 + 电话震动 + 开机震动

  ->开机震动 -> OK   2017.4.27



  ->插入充电器，进行充电器检测时候会有震动
    震动使能,插入充电器，震动模式




  ->写一个测试的程序，调用设备节点操作  





1.vibrator提供了两套mtk自带的和第三方的
        CONFIG_GN_BSP_MTK_VIBRATOR_DRV2604L
        CONFIG_MTK_VIBRATOR

2.lk跟kernel有个套代码

lk阶段
知道这个函数干了什么：就是让马达震动一会然后关闭
void gn_lk_vibrate(void)
{    
	printf("%s.\n",__func__);
	
	unsigned int result_tmp;
	char reg_address;
	char reg_data;

    //set gpio，设置drv2604_en引脚输出使能
	mt_set_gpio_out(DRV2604L_GPIO_ENABLE_PIN,1);
	mdelay(2);

    //往寄存器0x01写 0 为DEV_IDLE模式，DEV_STANDBY  1， DEV_READY  2
	drv2604l_i2c_write(MODE_REG,DEV_IDLE);

	//init regs 这两个参数都要在标准化之前设定好
    //这个是设置额定电压 rated_voltage: 0x16
	drv2604l_i2c_write(RATED_VOLTAGE_REG, 0x30);

    //超速的钳位电压 ，在闭环操作中这个电压可以超过额定电压 Overdrive Clamp Voltage ：0x17
  	drv2604l_i2c_write(OVERDRIVE_CLAMP_VOLTAGE_REG,0x53);

	/*drv2604l_set_bits(FEEDBACK_CONTROL_REG,	
						FEEDBACK_CONTROL_DEVICE_TYPE_MASK|FEEDBACK_CONTROL_FB_BRAKE_MASK|FEEDBACK_CONTROL_LOOP_GAIN_MASK,
						FEEDBACK_CONTROL_MODE_LRA|FB_BRAKE_FACTOR|LOOP_GAIN);

	drv2604l_set_bits(Control3_REG,	
						Control3_REG_LOOP_MASK|Control3_REG_FORMAT_MASK,
						BIDIR_INPUT_BIDIRECTIONAL|ERM_OpenLoop_Disable|LRA_OpenLoop_Disable|RTP_FORMAT_SIGNED);
	*/

    //real time payback：0x02 给这个模式写一个值
	drv2604l_i2c_write(REAL_TIME_PLAYBACK_REG, 0x5F);
    //进入RTP模式 ， 实时的反馈
	drv2604l_i2c_write(MODE_REG,MODE_REAL_TIME_PLAYBACK);
	mdelay(100);

    //让设备进入idle模式
	drv2604l_i2c_write(MODE_REG,DEV_IDLE);

    //set gpio
    //关闭en使能引脚
	mt_set_gpio_out(DRV2604L_GPIO_ENABLE_PIN,0);
}


kernel阶段干的内容很多，
debug这几个值才会通过
#define HAPTIC_CMDID_REG_WRITE  	0x09
#define HAPTIC_CMDID_REG_READ   	0x0a
#define HAPTIC_CMDID_REG_SETBIT  	0x0b




1.编译一个测试程序，交叉编译器or在vendor下添加一个文件夹，android.mk
2.echo 应该输出的是一个字符串，但是现在要输出一个数字
3.还有open_loop ,close_loop这两个内容没有理解
4.设备节点添加一个ioctl函数可以自己修改使用





DRV2604L 器件是一款低压触觉驱动器，其闭环致动器控制系统，可为 ERM 和 LRA 提供高质量的触觉反馈。此方案有助于提升致动器在加速度稳定性、启动时间和制动时间方面的
性能，通过共用的 I2C 兼容总线或 PWM 输入信号即可触发该方案。

DRV2604L 器件集成有足够的 RAM，用户能够预装载超过 100 个定制智能环路架构波形。这些波形可通过 I2C 即时回放，或者也可选择由硬件触发引脚来触发。

此外，主机处理器可利用实时回放模式绕过存储器回放引擎并通过 I2C 从主机直接播放波形。

DRV2604L 器件内部采用智能环路架构，可轻松实现自动谐振 LRA 驱动，以及优化反馈的 ERM 驱动，从而提供自动过驱动和制动。这种智能环路架构可构建简化的输入波形接口，
并且能够提供可靠的电机控制和稳定的电机性能。此外，DRV2604L 器件还 能够 在 LRA 致动器不产生有效反电动势电压时自动切换至开环系统。当 LRA 产生有效反电动势电压时，
DRV2604L 器件会自动与 LRA 同步。 DRV2604L 还可以利用内部生成的 PWM 信号实现开环驱动。



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


















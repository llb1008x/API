/************************************************************************
                        添加新的特性
*************************************************************************/

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
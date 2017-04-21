





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
    
->1 按键属于input子系统，先熟悉一下input子系统

    mtk键盘的名字mtk_kpd  
    相关定义的dtsi文件cust_kpd_8167.dtsi
    aw9523b这是键盘使用的芯片

    kpd.c

    mtk-kpd.kl

    



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


















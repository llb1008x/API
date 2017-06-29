
高通8064 8974 8926等pm芯片都集成了电量计，估计后续芯片都会一直存在，现在许多项目UI状态栏电池都有百分比显示，所以需要深入分析BMS有助于解决电量方面的BUG。


一： SOC（荷电状态）计算方法

名词：

FCC  Full-charge capacity      
UC     Remaining capacity
CC     Coulumb counter    
UUC  Unusable capacity
RUC   Remaining usable capacity //    RUC=RC-CC-UUC
SoC   State of charge    
OCV    Open circuit voltage

 SOC=(RC-CC-UUC)/(FCC-UUC)


以下是各个变量的计算方法：
1：FCC：
   在校准的电池profile中有定义,会随温度有变化；

    static struct single_row_lut fcc_temp = {  
     .x  = {-20, 0, 25, 40, 60},  
     .y  = {3193, 3190, 3190, 3180, 3183},  
     .cols = 5  
    };  


2：RC: 开机通过开始获取的开路电压（ocv）来查表（电池校准的profile文件）计算百分比，来比对计算（电压与电荷量正比）；(ocv=vbatt+rbatt*i_ma)
内核计算方法：

    static int calculate_remaining_charge_uah(struct pm8921_bms_chip *chip,  
          struct pm8921_soc_params *raw,  
          int fcc_uah, int batt_temp,  
          int chargecycles)  
    {  
     int  ocv, pc, batt_temp_decidegc;  
      
      
     ocv = raw->last_good_ocv_uv;  
     batt_temp_decidegc = chip->last_ocv_temp_decidegc;  
     pc = calculate_pc(chip, ocv, batt_temp_decidegc, chargecycles);  
     pr_info("ocv = %d pc = %d\n", ocv, pc);  
     return (fcc_uah * pc) / 100;  
    }  


但是通常情况下开机使用计算RC的ocv是上次关机存下的百分比，反向查表算出的ocv；
现在我们做法是通过判断开机时的ocv与关机的ocv如果偏差太大，我们将采用开机ocv来计算RC，所以开机的ocv对开机的百分比影响非常大；


3:CC:pmic库伦计 ADC采样到的：
内核获取方法：
    /**  
    * calculate_cc_uah -  
    * @chip:  the bms chip pointer  
    * @cc:   the cc reading from bms h/w  
    * @val:  return value  
    * @coulumb_counter: adjusted coulumb counter for 100%  
    *  
    * RETURNS: in val pointer coulumb counter based charger in uAh  
    *        (micro Amp hour)  
    */  
    static void calculate_cc_uah(struct pm8921_bms_chip *chip, int cc, int *val)  
    {  
     int64_t cc_voltage_uv, cc_pvh, cc_uah;  
      
      
     cc_voltage_uv = cc;  
     pr_debug("cc = %d\n", cc);  
     cc_voltage_uv = cc_to_microvolt(chip, cc_voltage_uv);  
     cc_voltage_uv = pm8xxx_cc_adjust_for_gain(cc_voltage_uv);  
     pr_debug("cc_voltage_uv = %lld microvolts\n", cc_voltage_uv);  
     cc_pvh = ccmicrovolt_to_pvh(cc_voltage_uv);  
     pr_debug("cc_pvh = %lld pico_volt_hour\n", cc_pvh);  
     cc_uah = div_s64(cc_pvh, chip->r_sense_uohm);  
     *val = cc_uah;  
    }  


4:UUC:计算方法和UC一致，但是rbatt是动态变化的，会复杂点；

    static int calculate_termination_uuc(struct pm8921_bms_chip *chip,  
         int batt_temp, int chargecycles,  
        int fcc_uah, int i_ma,  
        int *ret_pc_unusable)  
    {  
     int unusable_uv, pc_unusable, uuc;  
     int i = 0;  
     int ocv_mv;  
     int batt_temp_degc = batt_temp / 10;  
     int rbatt_mohm;  
     int delta_uv;  
     int prev_delta_uv = 0;  
     int prev_rbatt_mohm = 0;  
     int prev_ocv_mv = 0;  
     int uuc_rbatt_uv;  
      
      
     for (i = 0; i <= 100; i++) {  
      ocv_mv = interpolate_ocv(chip->pc_temp_ocv_lut,  
        batt_temp_degc, i);  
      rbatt_mohm = get_rbatt(chip, i, batt_temp);  
      unusable_uv = (rbatt_mohm * i_ma) + (chip->v_cutoff * 1000);  
      delta_uv = ocv_mv * 1000 - unusable_uv;  
      
      
      pr_debug("soc = %d ocv = %d rbat = %d u_uv = %d delta_v = %d\n",  
        i, ocv_mv, rbatt_mohm, unusable_uv, delta_uv);  
      
      
      if (delta_uv > 0)  
       break;  
      
      
      prev_delta_uv = delta_uv;  
      prev_rbatt_mohm = rbatt_mohm;  
      prev_ocv_mv = ocv_mv;  
     }  
      
      
     uuc_rbatt_uv = linear_interpolate(rbatt_mohm, delta_uv,  
         prev_rbatt_mohm, prev_delta_uv,  
         0);  
      
      
     unusable_uv = (uuc_rbatt_uv * i_ma) + (chip->v_cutoff * 1000);  
      
      
     pc_unusable = calculate_pc(chip, unusable_uv, batt_temp, chargecycles);  
     uuc = (fcc_uah * pc_unusable) / 100;  
     pr_debug("For i_ma = %d, unusable_rbatt = %d unusable_uv = %d unusable_pc = %d uuc = %d\n",  
         i_ma, uuc_rbatt_uv, unusable_uv,  
         pc_unusable, uuc);  
     *ret_pc_unusable = pc_unusable;  
     return uuc;  
    }  



 高通的这套BMS算法运行起来由于ocv的校准和温度等等原因，会有一定的偏差，高通还有一套通过校准OCV来估算SOC（简称soc_est）的机制，下面就是使用这套来校准SOC；


二：校准SOC
 
 高通算法通过对soc与soc_est比较计算出ocv的差值，来改变last_ocv_uv的值，主要是改变RC，重新计算soc，将会使得soc与soc_est越来越接近，越来越准；

 ocv在以下2种情况会被改变：

1：系统睡眠唤醒期间,cov被更新，库仑计RST；
2：低电进入adjust_soc()方法调节；


    在高通8064平台由于电量计对大电流计算不准确,一直亮屏的情况（没有经历睡眠唤醒的ocv更新与CC RST）会导致关机电压到达3.74V。要想解决这个问题必须使得校准SOC可以正常工作。
    但是当满电时开机就会记录ocv的值偏高，导致快要低电时不能很好的校准soc。所以有必要在马上进入低电（15%）时做一次模拟开机一次（电量计RERST CC=0从soc找出ocv ）使得
    last_ocv_uv降下来，才可以完美发挥adjust_soc的作用，使得关机电压能一直能到3.4V左右。
 

    <6>[ 7796.038269] read_soc_params_raw: 333333333 last_good_ocv_uv= 3777000uV  
      
      
    <6>[ 7796.038360] read_soc_params_raw: last_good_ocv_raw= 0x943f, last_good_ocv_uv= 3777000uV  
      
      
    <6>[ 7796.038543] calculate_soc_params: FCC = 3190000uAh batt_temp = 300, cycles = 0  
      
      
    <6>[ 7796.038635] calculate_remaining_charge_uah: ocv = 3777000 pc = 35  
      
      
    <6>[ 7796.038665] calculate_soc_params: RC = 1116500uAh  
      
      
    <6>[ 7796.038726] calculate_soc_params: cc_uah = 394979uAh raw->cc = 5764312  
      
      
    <6>[ 7796.038818] calculate_state_of_charge: RUC(RC-CC-UUC) = 657721uAh RC = 1116500uAh CC= 394979uAh UUC= 63800uAh FCC= 3190000uAh SOC(RUC/FCC-UUC) =21  


adjust_soc方法：


    </pre><p class="pa-1" style="line-height: 18px; font-size: 14px; padding-top: 0px; padding-bottom: 0px; margin-top: 0px; margin-bottom: 10px; color: rgb(68, 68, 68); font-family: "宋体;"><pre name="code" class="html"> static int last_soc_est = -EINVAL;  
      
    static int adjust_soc(struct pm8921_bms_chip *chip, int soc,  
      int batt_temp, int chargecycles,  
      int rbatt, int fcc_uah, int uuc_uah, int cc_uah)  
    {  
      
      
      
     int ibat_ua = 0, vbat_uv = 0;  
     int ocv_est_uv = 0, soc_est = 0, pc_est = 0, pc = 0;  
     int delta_ocv_uv = 0;  
     int n = 0;  
     int rc_new_uah = 0;  
     int pc_new = 0;  
     int soc_new = 0;  
     int m = 0;  
     int rc = 0;  
     int delta_ocv_uv_limit = 0;  
     int correction_limit_uv = 0;  
      
     rc = pm8921_bms_get_simultaneous_battery_voltage_and_current(&ibat_ua, &vbat_uv);  

     if (rc < 0) {  
      pr_err("simultaneous vbat ibat failed err = %d\n", rc);  
      goto out;  
     }  

     very_low_voltage_check(chip, ibat_ua, vbat_uv);  
      
     if (chip->low_voltage_detect &&  
      
     wake_lock_active(&chip->low_voltage_wake_lock)) {  

     if (is_voltage_below_cutoff_window(chip, ibat_ua, vbat_uv)) {  

        soc = 0;  
        pr_info("Voltage below cutoff, setting soc to 0\n");  
        goto out;  
      }  
     }  
     delta_ocv_uv_limit = DIV_ROUND_CLOSEST(ibat_ua, 1000);  

     ocv_est_uv = vbat_uv + (ibat_ua * rbatt)/1000;  

     calc_current_max(chip, ocv_est_uv, rbatt);  

     pc_est = calculate_pc(chip, ocv_est_uv, batt_temp, last_chargecycles);  

     soc_est = div_s64((s64)fcc_uah * pc_est - uuc_uah*100,(s64)fcc_uah - uuc_uah);  
      
     soc_est = bound_soc(soc_est);  

     /* never adjust during bms reset mode */  
     if (bms_reset) {  
        pr_debug("bms reset mode, SOC adjustment skipped\n");  
        goto out;  
     }  

     if (ibat_ua < 0 && pm8921_is_batfet_closed()) {  

      soc = charging_adjustments(chip, soc, vbat_uv, ibat_ua,  

        batt_temp, chargecycles,  

        fcc_uah, cc_uah, uuc_uah);  
      goto out;  
    
     }  

     /*  
      
      
      
      * do not adjust  
      
      
      
      * if soc_est is same as what bms calculated  
      
      
      
      * OR if soc_est > 15  
      
      
      
      * OR if soc it is above 90 because we might pull it low  
      
      
      
      * and  cause a bad user experience  
      
      
      
      */  
      
      
      
     if (soc_est == soc  
      
      
      
      || soc_est > 15  
      
      
      
      || soc >= 90)  
      
      
      
      goto out;  
      
      
      
      
      
      
      
     if (last_soc_est == -EINVAL)  
      
      
      
      last_soc_est = soc;  
      
      
      
      
      
      
      
     n = min(200, max(1 , soc + soc_est + last_soc_est));  
      
      
      
     /* remember the last soc_est in last_soc_est */  
      
      
      
     last_soc_est = soc_est;  
      
      
      
      
      
      
      
     pc = calculate_pc(chip, chip->last_ocv_uv,  
      
      
      
       chip->last_ocv_temp_decidegc, last_chargecycles);  
      
      
      
     if (pc > 0) {  
      
      
      
      pc_new = calculate_pc(chip, chip->last_ocv_uv - (++m * 1000),  
      
      
      
         chip->last_ocv_temp_decidegc,  
      
      
      
         last_chargecycles);  
      
      
      
      while (pc_new == pc) {  
      
      
      
       /* start taking 10mV steps */  
      
      
      
       m = m + 10;  
      
      
      
       pc_new = calculate_pc(chip,  
      
      
      
          chip->last_ocv_uv - (m * 1000),  
      
      
      
          chip->last_ocv_temp_decidegc,  
      
      
      
          last_chargecycles);  
      
      
      
      }  
      
      
      
     } else {  
      
      
      
      /*  
      
      
      
       * pc is already at the lowest point,  
      
      
      
       * assume 1 millivolt translates to 1% pc  
      
      
      
       */  
      
      
      
      pc = 1;  
      
      
      
      pc_new = 0;  
      
      
      
      m = 1;  
      
      
      
     }  
      
      
      
      
      
      
      
     delta_ocv_uv = div_s64((soc - soc_est) * (s64)m * 1000,  
      
      
      
           n * (pc - pc_new));  
      
      
      
      
      
      
      
     if (abs(delta_ocv_uv) > delta_ocv_uv_limit) {  
      
      
      
      pr_debug("limiting delta ocv %d limit = %d\n", delta_ocv_uv,  
      
      
      
        delta_ocv_uv_limit);  
      
      
      
      
      
      
      
      if (delta_ocv_uv > 0)  
      
      
      
       delta_ocv_uv = delta_ocv_uv_limit;  
      
      
      
      else  
      
      
      
       delta_ocv_uv = -1 * delta_ocv_uv_limit;  
      
      
      
      pr_debug("new delta ocv = %d\n", delta_ocv_uv);  
      
      
      
     }  
      
      
      
      
      
      
      
     if (wake_lock_active(&chip->low_voltage_wake_lock)) {  
      
      
      
      pr_debug("Low Voltage, apply only ibat limited corrections\n");  
      
      
      
      goto skip_limiting_corrections;  
      
      
      
     }  
      
      
      
      
      
      
      
     if (chip->last_ocv_uv > 3800000)  
      
      
      
      correction_limit_uv = the_chip->high_ocv_correction_limit_uv;  
      
      
      
     else  
      
      
      
      correction_limit_uv = the_chip->low_ocv_correction_limit_uv;  
      
      
      
      
      
      
      
     if (abs(delta_ocv_uv) > correction_limit_uv) {  
      
      
      
      pr_debug("limiting delta ocv %d limit = %d\n", delta_ocv_uv,  
      
      
      
        correction_limit_uv);  
      
      
      
      
      
      
      
      if (delta_ocv_uv > 0)  
      
      
      
       delta_ocv_uv = correction_limit_uv;  
      
      
      
      else  
      
      
      
       delta_ocv_uv = -1 * correction_limit_uv;  
      
      
      
      pr_debug("new delta ocv = %d\n", delta_ocv_uv);  
      
      
      
     }  
      
      
      
      
      
      
      
    skip_limiting_corrections:  
      
      
      
     chip->last_ocv_uv -= delta_ocv_uv;  
      
      
      
      
      
      
      
     if (chip->last_ocv_uv >= chip->max_voltage_uv)  
      
      
      
      chip->last_ocv_uv = chip->max_voltage_uv;  
      
      
      
      
      
      
      
     /* calculate the soc based on this new ocv */  
      
      
      
     pc_new = calculate_pc(chip, chip->last_ocv_uv,  
      
      
      
       chip->last_ocv_temp_decidegc, last_chargecycles);  
      
      
      
     rc_new_uah = (fcc_uah * pc_new) / 100;  
      
      
      
     soc_new = (rc_new_uah - cc_uah - uuc_uah)*100 / (fcc_uah - uuc_uah);  
      
      
      
     soc_new = bound_soc(soc_new);  
      
      
      
      
      
      
      
     /*  
      
      
      
      * if soc_new is ZERO force it higher so that phone doesnt report soc=0  
      
      
      
      * soc = 0 should happen only when soc_est == 0  
      
      
      
      */  
      
      
      
     if (soc_new == 0 && soc_est >= the_chip->hold_soc_est)  
      
      
      
      soc_new = 1;  
      
      
      
      
      
      
      
     soc = soc_new;  
      
      
      
      
      
      
      
    out:  
      
      
      
     pr_debug("ibat_ua = %d, vbat_uv = %d, ocv_est_uv = %d, pc_est = %d, "  
      
      
      
      "soc_est = %d, n = %d, delta_ocv_uv = %d, last_ocv_uv = %d, "  
      
      
      
      "pc_new = %d, soc_new = %d, rbatt = %d, m = %d\n",  
      
      
      
      ibat_ua, vbat_uv, ocv_est_uv, pc_est,  
      
      
      
      soc_est, n, delta_ocv_uv, chip->last_ocv_uv,  
      
      
      
      pc_new, soc_new, rbatt, m);  
      
      
      
      
      
      
      
     return soc;  
      
      
      
    }  

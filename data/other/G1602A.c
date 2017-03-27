

1.充电失败
  现象：
	1.充电暂停，瞬间恢复
	2.概率性充电停止，需要重新插拔充电器
	3.打静电有时充电停止，可以瞬间恢复，有时不能恢复，需要插拔USB	（打静电，应该是测试机器在受到静电条件下，是否存在故障）

  可能的原因及处理办法：
	1.整机条件下，不好测量VBUS，如果拆开会有其他问题
	2.不恢复的测量VBUS，瞬时的用示波器


	充电电压不稳定，插上充电器后，电压升到7V左右（电流显示为负，可能表示充电），电流在2A~3A，但是没多长时间充电电压就变成4.5V左右。电流小于1A
	可能跟充电器相关，充电器有时候识别为无效的充电器，有时候识别为DCP专用充电器




FG: log_bat_status: soc=5169 vbat=3803536 ibat=679622 ocv=3 868080 esr=63720 temp=330 vchg=134700  这个状态电流为正但是电压升高电流为负
FG: log_bat_status: soc=3388 vbat=3954902 ibat=-2477097 ocv=3 809487 esr=65979 temp=305 vchg=7149840
FG: log_bat_status: soc=3629 vbat=3903480 ibat=-1447440 ocv=3815437 esr=65063 temp=305 vchg=6041160	

充电电流为负，充电器电压跳变，不稳定，如果vchg=134700-->1.34v就是充电器电压太小
电流负表示充进去了电，vchg=134700是断开充电器时的电压，4.5v时充电电流太小


CHGR Status 100B=02 100C=6E 100D=70 100E=85 100F=00 1010=10
CHGR Config 10F0=0F 10F1=03 10F2=10 10F3=03 10F4=2E 10F5=00 10F6=00 10F7=00 10F8=03 10F9=02 10FA=0F 10FB=47 10FC=40 10FD=0F 10FE=00 10FF=09


CHGR Status 100B=02 100C=2E 100D=70 100E=80 100F=00 1010=00
CHGR Config 10F0=0F 10F1=03 10F2=10 10F3=03 10F4=2E 10F5=00 10F6=00 10F7=00 10F8=03 10F9=02 10FA=0F 10FB=47 10FC=40 10FD=0F 10FE=00 10FF=09

充电器的状态改变了

static struct fg_mem_data fg_data[FG_DATA_MAX] = {
	/*       ID           Address, Offset, Length, Value*/
	DATA(BATT_TEMP,       0x550,   2,      2,     -EINVAL),
	DATA(OCV,             0x588,   3,      2,     -EINVAL),
	DATA(VOLTAGE,         0x5CC,   1,      2,     -EINVAL),
	DATA(CURRENT,         0x5CC,   3,      2,     -EINVAL),
	DATA(BATT_ESR,        0x554,   2,      2,     -EINVAL),
	DATA(BATT_ESR_COUNT,  0x558,   2,      2,     -EINVAL),
	DATA(BATT_SOC,        0x56C,   1,      3,     -EINVAL),
	DATA(CC_CHARGE,       0x570,   0,      4,     -EINVAL),
	DATA(VINT_ERR,        0x560,   0,      4,     -EINVAL),
	DATA(CPRED_VOLTAGE,   0x540,   0,      2,     -EINVAL),
	DATA(BATT_ID,         0x594,   1,      1,     -EINVAL),
	DATA(BATT_ID_INFO,    0x594,   3,      1,     -EINVAL),
};


底层根据寄存器的地址+偏移量，值的位数求出要求的量的值
static int get_sram_prop_now(struct fg_chip *chip, unsigned int type)
{
	if (fg_debug_mask & FG_POWER_SUPPLY)
		pr_info("addr 0
		x%02X, offset %d value %d\n",
			fg_data[type].address, fg_data[type].offset,
			fg_data[type].value);

	if (type == FG_DATA_BATT_ID)
		return get_batt_id(fg_data[type].value,
				fg_data[FG_DATA_BATT_ID_INFO].value);

	return fg_data[type].value;
}


void log_bat_status(struct fg_chip *chip)
{
	union power_supply_propval prop = {0,};
	struct power_supply	*batt_psy;
	int ocv,bat_voltae,bat_current,bat_esr,soc,charge_voltage,bat_temp;
	
	batt_psy = power_supply_get_by_name("battery");
	if(batt_psy != NULL)
		batt_psy->get_property(batt_psy,POWER_SUPPLY_PROP_CHARGE_VOLTAGE_NOW, &prop);
	else
		prop.intval = 5000000;
	charge_voltage = prop.intval;
	
	ocv = get_sram_prop_now(chip,FG_DATA_OCV);
	bat_voltae = get_sram_prop_now(chip,FG_DATA_VOLTAGE);
	bat_current = get_sram_prop_now(chip,FG_DATA_CURRENT);
	bat_esr = get_sram_prop_now(chip,FG_DATA_BATT_ESR);
	soc = get_sram_prop_now(chip,FG_DATA_BATT_SOC);
	bat_temp = get_sram_prop_now(chip,FG_DATA_BATT_TEMP);

	pr_err("soc=%d vbat=%d ibat=%d ocv=%d esr=%d temp=%d vchg=%d\n",
		soc,bat_voltae,bat_current,ocv,bat_esr,bat_temp,charge_voltage);
	dump_chg_regs();
}
//Gionee <GN_BSP_CHG> <liujiang> <20161025> add for CR01762188 end




2.改重启时间pmu断电

各位研发同事，领导：
    S9项目导入硬件复位后，效果很明显，目前市场故障统计中，不开机类数据比率很小；实践证明一体机导入硬件复位是非常有必要的，可惜了金钢2和M2017没有导入。

鉴于各研发院各项目实现的路径不一样，在这里我就统一下我们希望硬件复位实现的过程：
1，只要长按电源键，不需要各种组合键，各种组合键用户搞不清楚。
2，长按电源键10S左右，实现软件重启；
3，长按电源键15S左右，实现硬件重启（务必是完全断电，像拔电池一样）；

请研发同事按照以上需求设计，邮件没有抄送到的请相关同事帮忙转发，谢谢！


all:
    这个需要实际测试确认一下方案，项目最好P2 12/29 投板前确认一下,目前高通方案 无法做到10S重启/15S掉电，只能做到12S
我们只能评估一下两种，
方案一: 一键方案
        1 4S 长按，软件UI可以选择
        2 8S 长按，系统自动复位，
        3 12S 长按，掉电，
方案二：两键方案（目前方案）
       1 10S 单键重启
       2 10S 组合键掉电，
==========================
方斌会飞线，评估一下两种方式，
1 主要是高通只能做到12S,8S/12S 比较接近。方案一可能会有误触发
2 评估是否主要是给客户使用，客户使用 单键会更方便。





Dear All，
         关于多长时间强制关机或重启，这个与平台有关、不同平台可能不太一样；关于多长时间强制断电、断多久，这个是硬件电路决定的；
         这个大都是在用户死机的情况下 使用的，很难用软件弹出让用户选择；并且 如果只是单电源键的话，必须要8秒以上，不然很容易误触，G1605A的教训不能再犯了； 硬件断电必须要 大于软件时间 4、5秒以上、否则也容易误触发；
         硬件断电会导致RTC丢失、数据损坏等风险，研发这边不建议 用户经常使用，组合键是一个选择，也没有误触发的风险；但从减少售后工作量角度看，告知用户 方法、或许 单键15秒 比较合适，需要相关领导来决定；

         G1602A 高通平台 目前情况是 长按power键10秒 强制关机、需要抬起 再次按下 开机；后续如果需要 可以改成重启；
         硬件断电 目前是 power+home  组合键 7.6秒 强制断电； 如需要 改成单 power键 15秒，需要硬件更换 采取圣邦微方案，请相关人员讨论定夺；
         
        总的来说，是可以满足品质提出的要求的，需要相关人员决定；请知悉；




2.充电无过压提示
	充电升压升到14v没有过压提示
	充电的有效范围4v到9v左右，超过9v就没有提示充电







4.熟悉G1602A的代码





5.功耗问题
分析功耗问题+那个重启电量显示问题

功耗问题分析

wakeup
msm_otg     	3		3		0		0		149458		188097		149458		649781		0

USER      PID   PPID  VSIZE  RSS  PRIO  NICE  RTPRI SCHED  PCY WCHAN              PC  NAME

SCHED       cpr


msm_otg

<3>[  469.817543] msm_otg 78db000.usb: OTG PM resume



RTC_WAKEUP

 Pending alarm batches

 Top Alarms:

 Alarm Stats:

 Sync adapters for UserInfo

 DUMP OF

 DUMP OF SERVICE       打印各种service的状态

 wake_lock

 PM: suspend




























SDP ，DCP的区别：
	   充电器自动确定适配器类型，能够区分以下类型：

	　　DCP：500mA至1.5A

	　　CDP (主机或集线器)：高速充电时达到900mA (啁啾期间为580mA);低速和快速为1.5A

	　　低功率SDP (主机或集线器)：100mA

	　　大功率SDP (主机或集线器)：500mA

	　　所提供的电流支持电池充电或系统供电，或在它们之间进行分配。如果在长达10ms内未检测到总线流量，内置挂起定时器自动触发挂起。


	 SMBCHG: smbchg_vfloat_adjust_work: Stopping vfloat adj taper=0 parallel_ma = 0

	 usb plugged out, do nothing

	 msm_otg 78db000.usb: otg_power_set_property_usb: charger type = INVALID_CHARGER

	 msm_otg 78db000.usb: gadget off
     msm_hsusb msm_hsusb: CI13XXX_CONTROLLER_DISCONNECT_EVENT received

	 SMBCHG: smbchg_parallel_usb_en_work: usb plugged out, do nothing

	 SMBCHG: smbchg_vfloat_adjust_check: Starting vfloat adjustments
	 SMBCHG: smbchg_vfloat_adjust_work: Stopping vfloat adj taper=0 parallel_ma = 0

	 充电电压高之后是否是默认将USB断开？

	 i2c通信没有回应
	 i2c-msm-v2 7af8000.i2c: NACK: slave not responding, ensure its powered: msgs(n:2 cur:0 tx) bc(rx:1 tx:1) mode:FIFO slv_addr:0x45 MSTR_STS:0x0d1300c8 OPER:0x00000010




	struct smbchg_chip {
	struct device			*dev;
	struct spmi_device		*spmi;
	int				schg_version;

	/* peripheral register address bases */
	u16				chgr_base;
	u16				bat_if_base;
	u16				usb_chgpth_base;
	u16				dc_chgpth_base;
	u16				otg_base;
	u16				misc_base;

	int				fake_battery_soc;
	u8				revision[4];

	/* configuration parameters */
	int				iterm_ma;
	int				usb_max_current_ma;
	int				dc_max_current_ma;
	int				dc_target_current_ma;
	int				cfg_fastchg_current_ma;
	int				fastchg_current_ma;
	int				vfloat_mv;
	int				fastchg_current_comp;
	int				float_voltage_comp;
	int				resume_delta_mv;
	int				safety_time;
	int				prechg_safety_time;
	int				bmd_pin_src;
	int				jeita_temp_hard_limit;
	int				sw_esr_pulse_current_ma;
	int				aicl_rerun_period_s;
	bool				use_vfloat_adjustments;
	bool				iterm_disabled;
	bool				bmd_algo_disabled;
	bool				soft_vfloat_comp_disabled;
	bool				chg_enabled;
	bool				charge_unknown_battery;
	bool				chg_inhibit_en;
	bool				chg_inhibit_source_fg;
	bool				low_volt_dcin;
	bool				cfg_chg_led_support;
	bool				cfg_chg_led_sw_ctrl;
	bool				vbat_above_headroom;
	bool				force_aicl_rerun;
	bool				hvdcp3_supported;
	bool				allow_hvdcp3_detection;
	bool				restricted_charging;
	bool				cfg_override_usb_current;
	u8				original_usbin_allowance;
	struct parallel_usb_cfg		parallel;
	struct delayed_work		parallel_en_work;
	struct dentry			*debug_root;
	struct smbchg_version_tables	tables;

	/* wipower params */
	struct ilim_map			wipower_default;
	struct ilim_map			wipower_pt;
	struct ilim_map			wipower_div2;
	struct qpnp_vadc_chip		*vadc_dev;
//Gionee <GN_BSP_CHG> <liujiang> <20161025> add for CR01764517 begin
	struct qpnp_vadc_chip      *vusbin_vadc_dev;
	u32 			vusb_adc_channel;
//Gionee <GN_BSP_CHG> <liujiang> <20161025> add for CR01764517 end
	//Gionee <GN_BSP_CHG> <liujiang> <2016-11-28> add for 35113 begin
	struct votable			*thermal_level_votable;
	struct wake_lock*   backlight_change_thermal_lvl_lock;
	//Gionee <GN_BSP_CHG> <liujiang> <2016-11-28> add for 35113 end
	bool				wipower_dyn_icl_avail;
	struct ilim_entry		current_ilim;
	struct mutex			wipower_config;
	bool				wipower_configured;
	struct qpnp_adc_tm_btm_param	param;

	/* flash current prediction */
	int				rpara_uohm;
	int				rslow_uohm;
	int				vled_max_uv;

	/* vfloat adjustment */
	int				max_vbat_sample;
	int				n_vbat_samples;

	/* status variables */
	int				max_pulse_allowed;
	int				wake_reasons;
	int				previous_soc;
	int				usb_online;
	bool				dc_present;
	bool				usb_present;
	bool				batt_present;
	int				otg_retries;
	ktime_t				otg_enable_time;
	bool				aicl_deglitch_short;
	bool				safety_timer_en;
	bool				aicl_complete;
	bool				usb_ov_det;
	bool				otg_pulse_skip_dis;
	const char			*battery_type;
	enum power_supply_type		usb_supply_type;
	bool				very_weak_charger;
	bool				parallel_charger_detected;
	bool				chg_otg_enabled;
	bool				flash_triggered;
	bool				icl_disabled;
	u32				wa_flags;
	int				usb_icl_delta;

	/* jeita and temperature */
	bool				batt_hot;
	bool				batt_cold;
	bool				batt_warm;
	bool				batt_cool;
	unsigned int			thermal_levels;
	unsigned int			therm_lvl_sel;
	unsigned int			*thermal_mitigation;

	/* irqs */
	int				batt_hot_irq;
	int				batt_warm_irq;
	int				batt_cool_irq;
	int				batt_cold_irq;
	int				batt_missing_irq;
	int				vbat_low_irq;
	int				chg_hot_irq;
	int				chg_term_irq;
	int				taper_irq;
	bool				taper_irq_enabled;
	struct mutex			taper_irq_lock;
	int				recharge_irq;
	int				fastchg_irq;
	int				wdog_timeout_irq;
	int				power_ok_irq;
	int				dcin_uv_irq;
	int				usbin_uv_irq;
	int				usbin_ov_irq;
	int				src_detect_irq;
	int				otg_fail_irq;
	int				otg_oc_irq;
	int				aicl_done_irq;
	int				usbid_change_irq;
	int				chg_error_irq;
	bool				enable_aicl_wake;

	/* psy */
	struct power_supply		*usb_psy;
	struct power_supply		batt_psy;
	struct power_supply		dc_psy;
	struct power_supply		*bms_psy;
	int				dc_psy_type;
	const char			*bms_psy_name;
	const char			*battery_psy_name;
	bool				psy_registered;

	struct smbchg_regulator		otg_vreg;
	struct smbchg_regulator		ext_otg_vreg;
	struct work_struct		usb_set_online_work;
	//Gionee <GN_BSP_CHG> <liujiang> <20161024> add for CR01769307 begin
	struct work_struct		apsd_rerun_work;
	//Gionee <GN_BSP_CHG> <liujiang> <20161024> add for CR01769307 end
	struct delayed_work		vfloat_adjust_work;
	struct delayed_work		hvdcp_det_work;
	spinlock_t			sec_access_lock;
	struct mutex			therm_lvl_lock;
	struct mutex			usb_set_online_lock;
	struct mutex			pm_lock;
	/* aicl deglitch workaround */
	unsigned long			first_aicl_seconds;
	int				aicl_irq_count;
	struct mutex			usb_status_lock;
	bool				hvdcp_3_det_ignore_uv;
	struct completion		src_det_lowered;
	struct completion		src_det_raised;
	struct completion		usbin_uv_lowered;
	struct completion		usbin_uv_raised;
	struct completion		hvdcp_det_done;
	int				pulse_cnt;
	struct led_classdev		led_cdev;
	bool				skip_usb_notification;

	/* voters */
	struct votable			*fcc_votable;
	struct votable			*usb_icl_votable;
	struct votable			*dc_icl_votable;
	struct votable			*usb_suspend_votable;
	struct votable			*dc_suspend_votable;
	struct votable			*battchg_suspend_votable;

	u32				vchg_adc_channel;
	struct qpnp_vadc_chip		*vchg_vadc_dev;
}




smbcharger 的工作函数
	INIT_WORK(&chip->usb_set_online_work, smbchg_usb_update_online_work);
	//Gionee <GN_BSP_CHG> <liujiang> <20161024> add for CR01769307 begin
	INIT_WORK(&chip->apsd_rerun_work, smbchg_rerun_apsd_work);
	//Gionee <GN_BSP_CHG> <liujiang> <20161024> add for CR01769307 end
	INIT_DELAYED_WORK(&chip->parallel_en_work,
			smbchg_parallel_usb_en_work);
	INIT_DELAYED_WORK(&chip->vfloat_adjust_work, smbchg_vfloat_adjust_work);
	INIT_DELAYED_WORK(&chip->hvdcp_det_work, smbchg_hvdcp_det_work);


	//充电电压的动态调节
	static void smbchg_vfloat_adjust_work(struct work_struct *work)
	{
		struct smbchg_chip *chip = container_of(work,
					struct smbchg_chip,
					vfloat_adjust_work.work);
		int vbat_uv, vbat_mv, ibat_ua, rc, delta_vfloat_mv;
		bool taper, enable;

		smbchg_stay_awake(chip, PM_REASON_VFLOAT_ADJUST);
		taper = (get_prop_charge_type(chip)
			== POWER_SUPPLY_CHARGE_TYPE_TAPER);
		enable = taper && (chip->parallel.current_max_ma == 0);

		if (!enable) {
			pr_smb(PR_MISC,
				"Stopping vfloat adj taper=%d parallel_ma = %d\n",
				taper, chip->parallel.current_max_ma);
			goto stop;
		}

		if (get_prop_batt_health(chip) != POWER_SUPPLY_HEALTH_GOOD) {
			pr_smb(PR_STATUS, "JEITA active, skipping\n");
			goto stop;
		}

		set_property_on_fg(chip, POWER_SUPPLY_PROP_UPDATE_NOW, 1);
		rc = get_property_from_fg(chip,
				POWER_SUPPLY_PROP_VOLTAGE_NOW, &vbat_uv);
		if (rc) {
			pr_smb(PR_STATUS,
				"bms psy does not support voltage rc = %d\n", rc);
			goto stop;
		}
		vbat_mv = vbat_uv / 1000;

		if ((vbat_mv - chip->vfloat_mv) < -1 * vf_adjust_max_delta_mv) {
			pr_smb(PR_STATUS, "Skip vbat out of range: %d\n", vbat_mv);
			goto reschedule;
		}

		rc = get_property_from_fg(chip,
				POWER_SUPPLY_PROP_CURRENT_NOW, &ibat_ua);
		if (rc) {
			pr_smb(PR_STATUS,
				"bms psy does not support current_now rc = %d\n", rc);
			goto stop;
		}

		if (ibat_ua / 1000 > -chip->iterm_ma) {
			pr_smb(PR_STATUS, "Skip ibat too high: %d\n", ibat_ua);
			goto reschedule;
		}

		pr_smb(PR_STATUS, "sample number = %d vbat_mv = %d ibat_ua = %d\n",
			chip->n_vbat_samples,
			vbat_mv,
			ibat_ua);

		chip->max_vbat_sample = max(chip->max_vbat_sample, vbat_mv);
		chip->n_vbat_samples += 1;
		if (chip->n_vbat_samples < vf_adjust_n_samples) {
			pr_smb(PR_STATUS, "Skip %d samples; max = %d\n",
				chip->n_vbat_samples, chip->max_vbat_sample);
			goto reschedule;
		}
		/* if max vbat > target vfloat, delta_vfloat_mv could be negative */
		delta_vfloat_mv = chip->vfloat_mv - chip->max_vbat_sample;
		pr_smb(PR_STATUS, "delta_vfloat_mv = %d, samples = %d, mvbat = %d\n",
			delta_vfloat_mv, chip->n_vbat_samples, chip->max_vbat_sample);
		/*
		* enough valid samples has been collected, adjust trim codes
		* based on maximum of collected vbat samples if necessary
		*/
		if (delta_vfloat_mv > vf_adjust_high_threshold
				|| delta_vfloat_mv < -1 * vf_adjust_low_threshold) {
			
			//这个就是充电电压的步进升压
			rc = smbchg_adjust_vfloat_mv_trim(chip, delta_vfloat_mv);

			if (rc) {
				pr_smb(PR_STATUS,
					"Stopping vfloat adj after trim adj rc = %d\n",
					rc);
				goto stop;
			}
			chip->max_vbat_sample = 0;
			chip->n_vbat_samples = 0;
			goto reschedule;
		}

	stop:
		chip->max_vbat_sample = 0;
		chip->n_vbat_samples = 0;
		smbchg_relax(chip, PM_REASON_VFLOAT_ADJUST);
		return;

	reschedule:
		schedule_delayed_work(&chip->vfloat_adjust_work,
				msecs_to_jiffies(VFLOAT_RESAMPLE_DELAY_MS));
		return;
	}


	//对专用电姐接口的识别
	static void smbchg_hvdcp_det_work(struct work_struct *work)
	{
		struct smbchg_chip *chip = container_of(work,
					struct smbchg_chip,
					hvdcp_det_work.work);
		int rc;

		if (chip->parallel.use_parallel_aicl) {
			if (!chip->hvdcp3_supported || !is_hvdcp_present(chip)) {
				complete_all(&chip->hvdcp_det_done);
				pr_smb(PR_MISC, "hvdcp_det_done complete\n");
			}
		}

		if (is_hvdcp_present(chip)) {
			if (!chip->hvdcp3_supported &&
				(chip->wa_flags & SMBCHG_HVDCP_9V_EN_WA)) {
				/* force HVDCP 2.0 */
				rc = force_9v_hvdcp(chip);
				if (rc)
					pr_err("could not force 9V HVDCP continuing rc=%d\n",
							rc);
			}
			smbchg_change_usb_supply_type(chip,
					POWER_SUPPLY_TYPE_USB_HVDCP);
			if (chip->psy_registered)
				power_supply_changed(&chip->batt_psy);
			smbchg_aicl_deglitch_wa_check(chip);
	}





	3.开机充电电压偏高，充电一段时间后，充电电流异常降低
	电压升上去之后，充了一段时间然后就降到4.4V左右，这个以前也看到这个现象，充电电压升不上去
	有点异常的是中间系统休眠的时候，是不是把充电这一块也给冻结了？

	log_bat_status
   
7.28V
	CHGR Status 100B=02 100C=6E 100D=70 100E=05 100F=00 1010=10
	CHGR Config 10F0=0F 10F1=03 10F2=10 10F3=03 10F4=2E 10F5=00 10F6=00 10F7=00 10F8=03 10F9=02 10FA=0F 10FB=47 10FC=40 10FD=3F 10FE=00 10FF=09
4.4V
	CHGR Status 100B=02 100C=2E 100D=7F 100E=05 100F=00 1010=10
	CHGR Config 10F0=0F 10F1=03 10F2=1F 10F3=03 10F4=2E 10F5=00 10F6=00 10F7=00 10F8=03 10F9=02 10FA=0F 10FB=47 10FC=40 10FD=3F 10FE=00 10FF=09

4.8V
CHGR Status 100B=02 100C=2E 100D=7F 100E=07 100F=00 1010=50
CHGR Config 10F0=0F 10F1=03 10F2=1F 10F3=03 10F4=2E 10F5=00 10F6=00 10F7=00 10F8=03 10F9=02 10FA=0F 10FB=47 10FC=40 10FD=3F 10FE=00 10FF=09


	 SMBCHG: smbchg_run_parallel_aicl: Parallel_psy ICL status is 0mA, usb might be plugged out
	 充电芯片里面有两个工作的PMI8998，SMB13X，其中有一个降压之后不工作，parallel charge disable
	 slave charger , main charger

	 smbchg_parallel_usb_en_work: parallel charging unavailable
	 smbchg_parallel_usb_disable: disabling parallel charger

	 SMBCHG: smbchg_parallel_usb_en_work: AICL at 3000
	 SMB138X: smb138x_parallel_set_prop: USB_SUSPEND=1
	 SMB138X: smb138x_parallel_set_prop: parallel FV=4400 uV
	 SMB138X: smb138x_parallel_set_prop: parallel FCC=4500000 uA
	 SMBCHG: smbchg_set_usb_current_max: USB current_ma = 100

	 smbchg_is_parallel_usb_ok: parallel AICL return 0, disable parallel

	 充电器电压和手机电压超过一定条件后 升压
	 smbchg_vfloat_adjust_work: Skip vbat out of range: 4307


	 充电器的类型：
	 {
		POWER_SUPPLY_CHARGE_TYPE_UNKNOWN = 0,//非标准充电器
		POWER_SUPPLY_CHARGE_TYPE_NONE,		 //无充电器
		POWER_SUPPLY_CHARGE_TYPE_TRICKLE,	 //滴; 细细地流？涓流充电，但这不是充电器类型
		POWER_SUPPLY_CHARGE_TYPE_FAST,		 //这个应该是快充
		POWER_SUPPLY_CHARGE_TYPE_TAPER,		 //逐渐变细？
	 }

	 //读取寄存器一定运算过后，根据特定位的值判断类型
	 static int get_prop_charge_type(struct smbchg_chip *chip)
	{
		int rc;
		u8 reg, chg_type;

		rc = smbchg_read(chip, &reg, chip->chgr_base + CHGR_STS, 1);
		if (rc < 0) {
			dev_err(chip->dev, "Unable to read CHGR_STS rc = %d\n", rc);
			return 0;
		}

		chg_type = (reg & CHG_TYPE_MASK) >> CHG_TYPE_SHIFT;
		if (chg_type == BATT_NOT_CHG_VAL)
			return POWER_SUPPLY_CHARGE_TYPE_NONE;
		else if (chg_type == BATT_TAPER_CHG_VAL)
			return POWER_SUPPLY_CHARGE_TYPE_TAPER;
		else if (chg_type == BATT_FAST_CHG_VAL)
			return POWER_SUPPLY_CHARGE_TYPE_FAST;
		else if (chg_type == BATT_PRE_CHG_VAL)
			return POWER_SUPPLY_CHARGE_TYPE_TRICKLE;

		return POWER_SUPPLY_CHARGE_TYPE_NONE;
	}



	/*main charger,slave charger*/
Q:
	1.充电芯片是否是两路main ，slave同时工作，充电电压加载到这两个芯片上
	2.slave charger上加了100mA电流用于判断充电器连接状态
	3.后面把slave charger suspend了，充电电压降到了4.4V？升压是delta 超过了范围要升高充电电压	
	static int smbchg_run_parallel_aicl(struct smbchg_chip *chip)
	{
		int rc, aicl_ma, fcc_ma, icl_ma;
		union power_supply_propval pval = {0, };
		struct power_supply *parallel_psy = get_parallel_psy(chip);

		if (!parallel_psy) {
			pr_err("parallel charger not found\n");
			return 0;
		}

		rc = power_supply_set_present(parallel_psy, true);
		if (rc) {
			pr_err("Unable to set_present for parallel_psy rc=%d\n", rc);
			return rc;
		}
		//Gionee <GN_BSP_CHG> <liujiang> <2016-11-28> add for 35058 begin
		/* Suspend slave */
		pval.intval  = 1;
		rc = parallel_psy->set_property(parallel_psy,
				POWER_SUPPLY_PROP_INPUT_SUSPEND, &pval);
		if (rc) {
			pr_err("Unable to suspend  parallel_psy rc=%d\n", rc);
			return rc;
		}
		//Gionee <GN_BSP_CHG> <liujiang> <2016-11-28> add for 35058 end
		rc = power_supply_set_voltage_limit(parallel_psy, chip->vfloat_mv);
		if (rc) {
			pr_err("Unable to set vfloat for parallel_psy rc=%d\n",
									rc);
			return rc;
		}

		/* Enable slave AICL */
		pval.intval = 1;
		rc = parallel_psy->set_property(parallel_psy,
				POWER_SUPPLY_PROP_ENABLE_AICL, &pval);
		if (rc) {
			pr_err("Unable to enable AICL on parallel_psy rc=%d\n", rc);
			return rc;
		}
		
		//Gionee <GN_BSP_CHG> <liujiang> <2016-11-28> add for 35058 begin
		chip->parallel.parallel_aicl_run_in_progress = true;
		//Gionee <GN_BSP_CHG> <liujiang> <2016-11-28> add for 35058 end
		
		/* Set max allowable FCC to slave */
		fcc_ma = get_effective_result_locked(chip->fcc_votable);
		pval.intval = fcc_ma * 1000;
		rc = parallel_psy->set_property(parallel_psy,
				POWER_SUPPLY_PROP_CONSTANT_CHARGE_CURRENT_MAX, &pval);
		if (rc) {
			pr_err("Unable to set FCC for parallel_psy rc=%d\n",
									rc);
			goto restore_icl;
		}

		/* Get max allowable ICL */
		icl_ma = get_effective_result_locked(chip->usb_icl_votable);


		/*
		* Force main charger to draw 100mA as the minimal input current
		* but don't suspend it to keep the slave charger online.
		* Set effective ICL value to slave charger to use it detecting
		* adapter's real capability. The 100mA draw in main charger path
		* will be added on slave's ICL status.
		*/
		vote(chip->usb_icl_votable, PARALLEL_ICL_VOTER, true, CURRENT_100_MA);

		rc = power_supply_set_current_limit(parallel_psy, icl_ma * 1000);
		if (rc) {
			pr_err("Unable to set ICL for parallel_psy rc=%d\n", rc);
			goto restore_icl;
		}

		rc = parallel_psy->get_property(parallel_psy,
				POWER_SUPPLY_PROP_INPUT_CURRENT_MAX, &pval);
		if (rc) {
			pr_err("Unable to read AICL from parallel_psy rc=%d\n", rc);
			goto restore_icl;
		}

		aicl_ma = pval.intval / 1000;

		if (aicl_ma == 0) {
			//Gionee <GN_BSP_CHG> <liujiang> <2016-11-30> add for 36552 begin
			//pr_err("Parallel_psy ICL status is 0mA\n");
			pr_err("Parallel_psy ICL status is 0mA, usb might be plugged out\n");
			chip->parallel.parallel_aicl_ma = 0;
			//Gionee <GN_BSP_CHG> <liujiang> <2016-11-30> add for 36552 end
			goto restore_icl;
		}

		chip->parallel.parallel_aicl_ma = min(icl_ma, CURRENT_100_MA + aicl_ma);
		pr_smb(PR_STATUS, "parallel AICL = %d mA\n",
					chip->parallel.parallel_aicl_ma);

	restore_icl:
		//Gionee <GN_BSP_CHG> <liujiang> <2016-11-28> add for 35058 begin
		/* Suspend slave and set back last ICL value for main charger */
		pval.intval  = 1;
		rc = parallel_psy->set_property(parallel_psy,
			POWER_SUPPLY_PROP_INPUT_SUSPEND, &pval);
		if (rc) {
			pr_err("Unable to suspend-input to parallel_psy rc=%d\n", rc);
			return rc;
		}
		//Gionee <GN_BSP_CHG> <liujiang> <2016-11-28> add for 35058 end
		
		/* Disable slave AICL */
		pval.intval = 0;
		rc = parallel_psy->set_property(parallel_psy,
				POWER_SUPPLY_PROP_ENABLE_AICL, &pval);
		if (rc)
			pr_err("Unable to disable AICL on parallel_psy rc=%d\n", rc);

		/* Suspend slave and set back last ICL value for main charger */
		//Gionee <GN_BSP_CHG> <liujiang> <2016-11-28> modify for 35058 begin
		//pval.intval  = 1;
		//rc = parallel_psy->set_property(parallel_psy,
		//		POWER_SUPPLY_PROP_INPUT_SUSPEND, &pval);
		//if (rc)
		//	pr_err("Unable to suspend-input to parallel_psy rc=%d\n", rc);
		/*
		* Wait 50 msec between suspending slave charger's input and
		* restoring main charger's ICL to avoid the inrush current
		* overshotting on VBUS
		*/
		msleep(50);
		//Gionee <GN_BSP_CHG> <liujiang> <2016-11-28> modify for 35058 end
		
		vote(chip->usb_icl_votable, PARALLEL_ICL_VOTER, false, 0);
		
		//Gionee <GN_BSP_CHG> <liujiang> <2016-11-28> add for 35058 begin
		chip->parallel.parallel_aicl_run_in_progress = false;
		//Gionee <GN_BSP_CHG> <liujiang> <2016-11-28> add for 35058 end
		
		return rc;
	}


	int vote(struct votable *votable, int client_id, bool state, int val)
	{
		int effective_id, effective_result;
		int rc = 0;

		lock_votable(votable);

		if (votable->votes[client_id].state == state &&
					votable->votes[client_id].value == val) {
			pr_debug("%s: votes unchanged; skipping\n", votable->name);
			goto out;
		}

		votable->votes[client_id].state = state;
		votable->votes[client_id].value = val;

		pr_debug("%s: %d voting for %d - %s\n",
				votable->name,
				client_id, val, state ? "on" : "off");
		switch (votable->type) {
		case VOTE_MIN:
			effective_id = vote_min(votable);
			break;
		case VOTE_MAX:
			effective_id = vote_max(votable);
			break;
		case VOTE_SET_ANY:
			votable->votes[client_id].value = state;
			effective_result = vote_set_any(votable);
			if (effective_result != votable->effective_result) {
				votable->effective_client_id = client_id;
				votable->effective_result = effective_result;
				rc = votable->callback(votable->dev,
							effective_result, client_id,
							state, client_id);
			}
			goto out;
		}

		/*
		* If the votable does not have any votes it will maintain the last
		* known effective_result and effective_client_id
		*/
		if (effective_id < 0) {
			pr_debug("%s: no votes; skipping callback\n", votable->name);
			goto out;
		}

		effective_result = votable->votes[effective_id].value;

		if (effective_result != votable->effective_result) {
			votable->effective_client_id = effective_id;
			votable->effective_result = effective_result;
			pr_debug("%s: effective vote is now %d voted by %d\n",
					votable->name, effective_result, effective_id);
			rc = votable->callback(votable->dev, effective_result,
						effective_id, val, client_id);
		}

	out:
		unlock_votable(votable);
		return rc;
	}












3.开机，关机充电异常
/***************************************************************************
						UB&OTG相关的代码
***************************************************************************/

高通相关的代码：


USB的软件框架：
1.Gadget driver framework 
这个框架层用来支持外围设备

2.EHCI-compliant host controller driver
USB_OTG主控制器驱动

3.OTG模式



static int msm_otg_probe(struct platform_device *pdev)
高通平台初始化OTG功能 这个probe有点长
INIT_WORK(&motg->sm_work, msm_otg_sm_work);
INIT_DELAYED_WORK(&motg->chg_work, msm_chg_detect_work);
INIT_DELAYED_WORK(&motg->id_status_work, msm_id_status_w);
INIT_DELAYED_WORK(&motg->suspend_work, msm_otg_suspend_work);
INIT_DELAYED_WORK(&motg->perf_vote_work, msm_otg_perf_vote_work);



/*根据usb_irq_count中断相应的次数是否超过阈值，超过之后启动对应的工作
  上报更偏爱的模式
*/
static void msm_otg_perf_vote_work(struct work_struct *w)
{
	struct msm_otg *motg = container_of(w, struct msm_otg,
						perf_vote_work.work);
	unsigned curr_sample_int_count;
	bool in_perf_mode = false;

	curr_sample_int_count = motg->usb_irq_count;
	motg->usb_irq_count = 0;

	if (curr_sample_int_count >= PM_QOS_THRESHOLD)
		in_perf_mode = true;

	msm_otg_perf_vote_update(motg, in_perf_mode);
	pr_debug("%s: in_perf_mode:%u, interrupts in last sample:%u\n",
		 __func__, in_perf_mode, curr_sample_int_count);

	schedule_delayed_work(&motg->perf_vote_work,
			msecs_to_jiffies(1000 * PM_QOS_SAMPLE_SEC));
}



/*检测到USB id变化调用的中断函数，这个应该是OTG设备插入的时候响应的
  然后上报到power_supply子系统的状态
*/
/**
 * usbid_change_handler() - called when the usb RID changes.
 * This is used mostly for detecting OTG
 */
static irqreturn_t usbid_change_handler(int irq, void *_chip)
{
	struct smbchg_chip *chip = _chip;
	bool otg_present;

	pr_smb(PR_INTERRUPT, "triggered\n");

	otg_present = is_otg_present(chip);
	if (chip->usb_psy) {
		pr_smb(PR_MISC, "setting usb psy OTG = %d\n",
				otg_present ? 1 : 0);
		power_supply_set_usb_otg(chip->usb_psy, otg_present ? 1 : 0);
	}
	if (otg_present)
		pr_smb(PR_STATUS, "OTG detected\n");

	/* update FG */
	set_property_on_fg(chip, POWER_SUPPLY_PROP_STATUS,
			get_prop_batt_status(chip));

	return IRQ_HANDLED;
}



/*获取iddig引脚的状态*/
static void msm_id_status_w(struct work_struct *w)
{
	struct msm_otg *motg = container_of(w, struct msm_otg,
						id_status_work.work);
	int work = 0;

	dev_dbg(motg->phy.dev, "ID status_w\n");

	if (motg->pdata->pmic_id_irq)
		motg->id_state = msm_otg_read_pmic_id_state(motg);
	else if (motg->ext_id_irq)
		motg->id_state = gpio_get_value(motg->pdata->usb_id_gpio);
	else if (motg->phy_irq)
		motg->id_state = msm_otg_read_phy_id_state(motg);

	if (motg->err_event_seen)
		return;

	if (motg->id_state) {
		if (gpio_is_valid(motg->pdata->switch_sel_gpio))
			gpio_direction_input(motg->pdata->switch_sel_gpio);
		if (!test_and_set_bit(ID, &motg->inputs)) {
			pr_debug("ID set\n");
			msm_otg_dbg_log_event(&motg->phy, "ID SET",
					motg->inputs, motg->phy.state);
			work = 1;
		}
	} else {
		if (gpio_is_valid(motg->pdata->switch_sel_gpio))
			gpio_direction_output(motg->pdata->switch_sel_gpio, 1);
		if (test_and_clear_bit(ID, &motg->inputs)) {
			pr_debug("ID clear\n");
			msm_otg_dbg_log_event(&motg->phy, "ID CLEAR",
					motg->inputs, motg->phy.state);
			set_bit(A_BUS_REQ, &motg->inputs);
			work = 1;
		}
	}

	if (work && (motg->phy.state != OTG_STATE_UNDEFINED)) {
		msm_otg_dbg_log_event(&motg->phy,
				"CHECK ID EVENT DURING SUSPEND",
				atomic_read(&motg->pm_suspended),
				motg->sm_work_pending);
		if (atomic_read(&motg->pm_suspended)) {
			motg->sm_work_pending = true;
		} else if (!motg->sm_work_pending) {
			/* process event only if previous one is not pending */
			queue_work(motg->otg_wq, &motg->sm_work);
		}
	}

}



/**************************************************************************************
								充电相关的代码
**************************************************************************************/



->/*mtk*/
{
	底层对硬件操作最好能和芯片手册对应这看

-->1.hw_charger_type_detection底层检测充电器类型
	这些bc11就是指BC1.1协议
	检测充电器类型根据BC1.1协议来的，给D+一个电压，检测D-上的电压，如果是低电平非标准充电；如果电压相同，标准充电；
	有一定的分压，专用的或其他类型的充电器。
	
	int hw_charger_type_detection(void)
	{
		CHARGER_TYPE charger_tye = CHARGER_UNKNOWN;

		/********* Step initial  ***************/
		hw_bc11_init();

		/********* Step DCD ***************/   
		//DCD：  数据载波检测
		if(1 == hw_bc11_DCD())
		{
			/********* Step A1 ***************/
			if(1 == hw_bc11_stepA1())
			{
				charger_tye = APPLE_2_1A_CHARGER;
				print("step A1 : Apple 2.1A CHARGER!\r\n");
			}
			else
			{
				charger_tye = NONSTANDARD_CHARGER;
				print("step A1 : Non STANDARD CHARGER!\r\n");
			}
		}
		else
		{
			/********* Step A2 ***************/
			if(1 == hw_bc11_stepA2())
			{
				/********* Step B2 ***************/
				if(1 == hw_bc11_stepB2())
				{
					charger_tye = STANDARD_CHARGER;
					print("step B2 : STANDARD CHARGER!\r\n");
				}
				else
				{
					charger_tye = CHARGING_HOST;
					print("step B2 :  Charging Host!\r\n");
				}
			}
			else
			{
				charger_tye = STANDARD_HOST;
				print("step A2 : Standard USB Host!\r\n");
			}
		}

		/********* Finally setting *******************************/
		hw_bc11_done();

		return charger_tye;
	}



-->检测充电器
	void do_chrdet_int_task(void)
	{
		if (g_bat_init_flag == KAL_TRUE) {
	#if !defined(CONFIG_MTK_DUAL_INPUT_CHARGER_SUPPORT)
			if (upmu_is_chr_det() == KAL_TRUE) {
	#else
			battery_charging_control(CHARGING_CMD_GET_DISO_STATE, &DISO_data);
			if ((DISO_data.diso_state.cur_vusb_state == DISO_ONLINE) ||
				(DISO_data.diso_state.cur_vdc_state == DISO_ONLINE)) {
	#endif
				battery_log(BAT_LOG_CRTI, "[do_chrdet_int_task] charger exist!\n");
				BMT_status.charger_exist = KAL_TRUE;

				wake_lock(&battery_suspend_lock);

	#if defined(CONFIG_POWER_EXT)
				mt_usb_connect();
				battery_log(BAT_LOG_CRTI,
						"[do_chrdet_int_task] call mt_usb_connect() in EVB\n");
	#elif defined(CONFIG_MTK_POWER_EXT_DETECT)
				if (KAL_TRUE == bat_is_ext_power()) {
					mt_usb_connect();
					battery_log(BAT_LOG_CRTI,
							"[do_chrdet_int_task] call mt_usb_connect() in EVB\n");
					return;
				}
	#endif
			} else {
				battery_log(BAT_LOG_CRTI, "[do_chrdet_int_task] charger NOT exist!\n");
				BMT_status.charger_exist = KAL_FALSE;

	#if defined(CONFIG_MTK_DUAL_INPUT_CHARGER_SUPPORT)
				battery_log(BAT_LOG_CRTI,
						"turn off charging for no available charging source\n");
				battery_charging_control(CHARGING_CMD_ENABLE, &BMT_status.charger_exist);
	#endif

	#ifdef CONFIG_MTK_KERNEL_POWER_OFF_CHARGING
				if (g_platform_boot_mode == KERNEL_POWER_OFF_CHARGING_BOOT
					|| g_platform_boot_mode == LOW_POWER_OFF_CHARGING_BOOT) {
					battery_log(BAT_LOG_CRTI,
							"[pmic_thread_kthread] Unplug Charger/USB In Kernel Power Off Charging Mode!  Shutdown OS!\r\n");
					battery_charging_control(CHARGING_CMD_SET_POWER_OFF, NULL);
					/* mt_power_off(); */
				}
	#endif

				wake_unlock(&battery_suspend_lock);

	#if defined(CONFIG_POWER_EXT)
				mt_usb_disconnect();
				battery_log(BAT_LOG_CRTI,
						"[do_chrdet_int_task] call mt_usb_disconnect() in EVB\n");
	#elif defined(CONFIG_MTK_POWER_EXT_DETECT)
				if (KAL_TRUE == bat_is_ext_power()) {
					mt_usb_disconnect();
					battery_log(BAT_LOG_CRTI,
							"[do_chrdet_int_task] call mt_usb_disconnect() in EVB\n");
					return;
				}
	#endif
	#if defined(CONFIG_MTK_PUMP_EXPRESS_SUPPORT) || defined(CONFIG_MTK_PUMP_EXPRESS_PLUS_SUPPORT)
				is_ta_connect = KAL_FALSE;
				ta_check_chr_type = KAL_TRUE;
				ta_cable_out_occur = KAL_TRUE;
	#endif

			}

			/* reset_parameter_dod_charger_plug_event(); */
			wakeup_fg_algo(FG_CHARGER);
			/* Place charger detection and battery update here is used to speed up charging icon display. */

			mt_battery_charger_detect_check();
			if (BMT_status.UI_SOC2 == 100 && BMT_status.charger_exist == KAL_TRUE) {
				BMT_status.bat_charging_state = CHR_BATFULL;
				BMT_status.bat_full = KAL_TRUE;
				g_charging_full_reset_bat_meter = KAL_TRUE;
			}

			if (g_battery_soc_ready == KAL_FALSE) {
				if (BMT_status.nPercent_ZCV == 0)
					battery_meter_initial();

				BMT_status.SOC = battery_meter_get_battery_percentage();
			}

			if (BMT_status.bat_vol > 0)
				mt_battery_update_status();

	#if defined(CONFIG_MTK_DUAL_INPUT_CHARGER_SUPPORT)
			DISO_data.chr_get_diso_state = KAL_TRUE;
	#endif

			wake_up_bat();
		} else {
	#if defined(CONFIG_MTK_DUAL_INPUT_CHARGER_SUPPORT)
			g_vcdt_irq_delay_flag = KAL_TRUE;
	#endif
			battery_log(BAT_LOG_CRTI,
					"[do_chrdet_int_task] battery thread not ready, will do after bettery init.\n");
		}

	}





	-->healthd进程负责监听底层上报的事件，uevent，periodic_chores负责将相应的事件上报给batterymonitor
		主循环
		static void healthd_mainloop(void) {
			while (1) {
				struct epoll_event events[eventct];
				int nevents;
				int timeout = awake_poll_interval;
				int mode_timeout;

				mode_timeout = healthd_mode_ops->preparetowait();
				if (timeout < 0 || (mode_timeout > 0 && mode_timeout < timeout))
					timeout = mode_timeout;
				nevents = epoll_wait(epollfd, events, eventct, timeout);

				if (nevents == -1) {
					if (errno == EINTR)
						continue;
					KLOG_ERROR(LOG_TAG, "healthd_mainloop: epoll_wait failed\n");
					break;
				}

				for (int n = 0; n < nevents; ++n) {
					if (events[n].data.ptr)
						//这段代码是强制类型转换，转换成相应类型的指针
						(*(void (*)(int))events[n].data.ptr)(events[n].events);//这个语句想干啥？
				}

				if (!nevents)
					periodic_chores();

				healthd_mode_ops->heartbeat();
			}

			return;
		}




	-->MTK不同充电算法的切换：
		void mt_battery_charging_algorithm(void)
		{
			battery_charging_control(CHARGING_CMD_RESET_WATCH_DOG_TIMER, NULL);

		#if defined(CONFIG_MTK_PUMP_EXPRESS_PLUS_SUPPORT)
		#if defined(PUMPEX_PLUS_RECHG)
			if (BMT_status.bat_in_recharging_state == KAL_TRUE && pep_det_rechg == KAL_TRUE)
				ta_check_chr_type = KAL_TRUE;
		#endif
			battery_pump_express_charger_check();
		#endif
			switch (BMT_status.bat_charging_state) {
			case CHR_PRE:
				BAT_PreChargeModeAction();
				break;

			case CHR_CC:
				BAT_ConstantCurrentModeAction();
				break;

			case CHR_BATFULL:
				BAT_BatteryFullAction();
				break;

			case CHR_HOLD:
				BAT_BatteryHoldAction();
				break;

			case CHR_ERROR:
				BAT_BatteryStatusFailAction();
				break;
			}

			battery_charging_control(CHARGING_CMD_DUMP_REGISTER, NULL);
		}


		static void battery_pump_express_charger_check(void)
		{
			if (KAL_TRUE == ta_check_chr_type &&
				STANDARD_CHARGER == BMT_status.charger_type &&
				BMT_status.SOC >= batt_cust_data.ta_start_battery_soc &&
				BMT_status.SOC < batt_cust_data.ta_stop_battery_soc) {
				battery_log(BAT_LOG_CRTI, "[PE+]Starting PE Adaptor detection\n");

				mutex_lock(&ta_mutex);
				wake_lock(&TA_charger_suspend_lock);

				mtk_ta_reset_vchr();

				mtk_ta_init();
				mtk_ta_detector();

				/* need to re-check if the charger plug out during ta detector */
				if (KAL_TRUE == ta_cable_out_occur)
					ta_check_chr_type = KAL_TRUE;
				else
					ta_check_chr_type = KAL_FALSE;
		#if defined(PUMPEX_PLUS_RECHG)
				/*PE detection disable in the event of recharge after 1st PE detection is finished */
				pep_det_rechg = KAL_FALSE;
		#endif
				wake_unlock(&TA_charger_suspend_lock);
				mutex_unlock(&ta_mutex);
			} else {
				battery_log(BAT_LOG_CRTI,
						"[PE+]Stop battery_pump_express_charger_check, SOC=%d, ta_check_chr_type = %d, charger_type = %d\n",
						BMT_status.SOC, ta_check_chr_type, BMT_status.charger_type);
			}
		}

		升压代码，current pattern ，通过VBUS上的一定规律的电流变化进行握手识别，通信
		static u32 charging_set_ta_current_pattern(void *data)
		{
			u32 increase = *(u32 *) (data);
			u32 charging_status = false;

		#if defined(HIGH_BATTERY_VOLTAGE_SUPPORT)
			u32 cv_voltage = BATTERY_VOLT_04_340000_V;
		#else
			u32 cv_voltage = BATTERY_VOLT_04_200000_V;
		#endif

			charging_get_charging_status(&charging_status);
			if (false == charging_status) {
				charging_set_cv_voltage(&cv_voltage);	/* Set CV */
				bq24196_set_ichg(0x0);	/* Set charging current 500ma */
				bq24196_set_chg_config(0x1);	/* Enable Charging */
			}

			if (increase == true) {
				bq24196_set_iinlim(0x0);	/* 100mA */
				msleep(85);

				bq24196_set_iinlim(0x2);	/* 500mA */
				pr_debug("mtk_ta_increase() on 1");
				msleep(85);

				bq24196_set_iinlim(0x0);	/* 100mA */
				pr_debug("mtk_ta_increase() off 1");
				msleep(85);

				bq24196_set_iinlim(0x2);	/* 500mA */
				pr_debug("mtk_ta_increase() on 2");
				msleep(85);

				bq24196_set_iinlim(0x0);	/* 100mA */
				pr_debug("mtk_ta_increase() off 2");
				msleep(85);

				bq24196_set_iinlim(0x2);	/* 500mA */
				pr_debug("mtk_ta_increase() on 3");
				msleep(281);

				bq24196_set_iinlim(0x0);	/* 100mA */
				pr_debug("mtk_ta_increase() off 3");
				msleep(85);

				bq24196_set_iinlim(0x2);	/* 500mA */
				pr_debug("mtk_ta_increase() on 4");
				msleep(281);

				bq24196_set_iinlim(0x0);	/* 100mA */
				pr_debug("mtk_ta_increase() off 4");
				msleep(85);

				bq24196_set_iinlim(0x2);	/* 500mA */
				pr_debug("mtk_ta_increase() on 5");
				msleep(281);

				bq24196_set_iinlim(0x0);	/* 100mA */
				pr_debug("mtk_ta_increase() off 5");
				msleep(85);

				bq24196_set_iinlim(0x2);	/* 500mA */
				pr_debug("mtk_ta_increase() on 6");
				msleep(485);

				bq24196_set_iinlim(0x0);	/* 100mA */
				pr_debug("mtk_ta_increase() off 6");
				msleep(50);

				pr_debug("mtk_ta_increase() end\n");

				bq24196_set_iinlim(0x2);	/* 500mA */
				msleep(200);
			} else {
				bq24196_set_iinlim(0x0);	/* 100mA */
				msleep(85);

				bq24196_set_iinlim(0x2);	/* 500mA */
				pr_debug("mtk_ta_decrease() on 1");
				msleep(281);

				bq24196_set_iinlim(0x0);	/* 100mA */
				pr_debug("mtk_ta_decrease() off 1");
				msleep(85);

				bq24196_set_iinlim(0x2);	/* 500mA */
				pr_debug("mtk_ta_decrease() on 2");
				msleep(281);

				bq24196_set_iinlim(0x0);	/* 100mA */
				pr_debug("mtk_ta_decrease() off 2");
				msleep(85);

				bq24196_set_iinlim(0x2);	/* 500mA */
				pr_debug("mtk_ta_decrease() on 3");
				msleep(281);

				bq24196_set_iinlim(0x0);	/* 100mA */
				pr_debug("mtk_ta_decrease() off 3");
				msleep(85);

				bq24196_set_iinlim(0x2);	/* 500mA */
				pr_debug("mtk_ta_decrease() on 4");
				msleep(85);

				bq24196_set_iinlim(0x0);	/* 100mA */
				pr_debug("mtk_ta_decrease() off 4");
				msleep(85);

				bq24196_set_iinlim(0x2);	/* 500mA */
				pr_debug("mtk_ta_decrease() on 5");
				msleep(85);

				bq24196_set_iinlim(0x0);	/* 100mA */
				pr_debug("mtk_ta_decrease() off 5");
				msleep(85);

				bq24196_set_iinlim(0x2);	/* 500mA */
				pr_debug("mtk_ta_decrease() on 6");
				msleep(485);

				bq24196_set_iinlim(0x0);	/* 100mA */
				pr_debug("mtk_ta_decrease() off 6");
				msleep(50);

				pr_debug("mtk_ta_decrease() end\n");

				bq24196_set_iinlim(0x2);	/* 500mA */
			}

			return STATUS_OK;
		}



		//控制充电使能
		static void pchr_turn_on_charging(void)
		{
		#if !defined(CONFIG_MTK_JEITA_STANDARD_SUPPORT)
			BATTERY_VOLTAGE_ENUM cv_voltage;
		#endif
			unsigned int charging_enable = KAL_TRUE;

		#if defined(CONFIG_MTK_DUAL_INPUT_CHARGER_SUPPORT)
			if (KAL_TRUE == BMT_status.charger_exist)
				charging_enable = KAL_TRUE;
			else
				charging_enable = KAL_FALSE;
		#endif

			if (BMT_status.bat_charging_state == CHR_ERROR) {
				battery_log(BAT_LOG_CRTI, "[BATTERY] Charger Error, turn OFF charging !\n");

				charging_enable = KAL_FALSE;

			} else if ((g_platform_boot_mode == META_BOOT) || (g_platform_boot_mode == ADVMETA_BOOT)) {
				battery_log(BAT_LOG_CRTI,
						"[BATTERY] In meta or advanced meta mode, disable charging.\n");
				charging_enable = KAL_FALSE;
			} else {
				/*HW initialization */
				battery_charging_control(CHARGING_CMD_INIT, NULL);

				battery_log(BAT_LOG_FULL, "charging_hw_init\n");

		#if defined(CONFIG_MTK_PUMP_EXPRESS_PLUS_SUPPORT)
				battery_pump_express_algorithm_start();
		#endif

				/* Set Charging Current */
				if (get_usb_current_unlimited()) {
					if (batt_cust_data.ac_charger_input_current != 0)
						g_temp_input_CC_value = batt_cust_data.ac_charger_input_current;
					else
						g_temp_input_CC_value = batt_cust_data.ac_charger_current;

					g_temp_CC_value = batt_cust_data.ac_charger_current;
					battery_log(BAT_LOG_FULL,
							"USB_CURRENT_UNLIMITED, use batt_cust_data.ac_charger_current\n");
				//GioneeDrv GuoJianqiu 20151223 modify for platform change begin
				}
		#ifndef CONFIG_MTK_SWITCH_INPUT_OUTPUT_CURRENT_SUPPORT
				else if (g_bcct_flag == 1) {
					select_charging_current_bcct();

					battery_log(BAT_LOG_CRTI, "[BATTERY] select_charging_current_bcct !\n");
				} else {
					select_charging_current();

					battery_log(BAT_LOG_CRTI, "[BATTERY] select_charging_current !\n");
				}
		#else
				else if (g_bcct_flag == 1 || g_bcct_input_flag == 1) {
				//GioneeDrv GuoJianqiu 20151223 modify for platform change end
					select_charging_current();
					select_charging_current_bcct();
					battery_log(BAT_LOG_FULL, "[BATTERY] select_charging_curret_bcct !\n");
				} else {
					select_charging_current();
					battery_log(BAT_LOG_FULL, "[BATTERY] select_charging_curret !\n");
				}
		#endif
				battery_log(BAT_LOG_CRTI,
						"[BATTERY] Default CC mode charging : %d, input current = %d\r\n",
						g_temp_CC_value, g_temp_input_CC_value);
				if (g_temp_CC_value == CHARGE_CURRENT_0_00_MA
					|| g_temp_input_CC_value == CHARGE_CURRENT_0_00_MA) {

					charging_enable = KAL_FALSE;

					battery_log(BAT_LOG_CRTI,
							"[BATTERY] charging current is set 0mA, turn off charging !\r\n");
				} else {
					battery_charging_control(CHARGING_CMD_SET_INPUT_CURRENT,
								&g_temp_input_CC_value);
					battery_charging_control(CHARGING_CMD_SET_CURRENT, &g_temp_CC_value);

					/*Set CV Voltage */
		#if !defined(CONFIG_MTK_JEITA_STANDARD_SUPPORT)
					//GioneeDrv GuoJianqiu 20160429 modify for platform change begin
					if (batt_cust_data.high_battery_voltage_support)
						cv_voltage = BATTERY_VOLT_04_400000_V;
					else
						cv_voltage = BATTERY_VOLT_04_200000_V;

					#ifdef CONFIG_MTK_DYNAMIC_BAT_CV_SUPPORT
					//GioneeDrv GuoJianqiu 20160429 modify for platform change end
					cv_voltage = get_constant_voltage() * 1000;
					battery_log(BAT_LOG_CRTI, "[BATTERY][BIF] Setting CV to %d\n", cv_voltage / 1000);
					#endif
					battery_charging_control(CHARGING_CMD_SET_CV_VOLTAGE, &cv_voltage);

					#if defined(CONFIG_MTK_HAFG_20)
					g_cv_voltage = cv_voltage;
					#endif
		#endif
				}
			}

			/* enable/disable charging */
			battery_charging_control(CHARGING_CMD_ENABLE, &charging_enable);

			battery_log(BAT_LOG_CRTI, "[BATTERY] pchr_turn_on_charging(), enable =%d !\r\n",	//Gionee GuoJianqiu 20150318 modify for fix bugs
					charging_enable);
		}













}










-->/*qc*/

{
	   	pmi8950_charger: qcom,qpnp-smbcharger {
		spmi-dev-container;
		compatible = "qcom,qpnp-smbcharger";
		#address-cells = <1>;
		#size-cells = <1>;

		qcom,iterm-ma = <100>;		//充电截止时的电流
		//Gionee <GN_BSP_CHG> <liujiang> <20161025> modify for CR01748202 begin
		qcom,float-voltage-mv = <4400>;
		//Gionee <GN_BSP_CHG> <liujiang> <20161025> modify for CR01748202 end
		qcom,resume-delta-mv = <200>;
		qcom,chg-inhibit-fg;
		qcom,rparasitic-uohm = <100000>;
		qcom,bms-psy-name = "bms";
		//Gionee <GN_BSP_CHG> <liujiang> <2016-11-28> add for 35113 begin
		qcom,thermal-mitigation = <3000 2500 2000 1500 1000 0>;
		//Gionee <GN_BSP_CHG> <liujiang> <2016-11-28> add for 35113 end
		//Gionee <GN_BSP_CHG> <liujiang> <20161025> modify for CR01748202 begin
		qcom,parallel-usb-min-current-ma = <2000>; //<1400>;
		//Gionee <GN_BSP_CHG> <liujiang> <20161025> modify for CR01748202 end
		qcom,parallel-usb-9v-min-current-ma = <900>;
		qcom,parallel-allowed-lowering-ma = <500>;
		qcom,autoadjust-vfloat;
		qcom,pmic-revid = <&pmi8950_revid>;
		qcom,charge-unknown-battery;
		qcom,force-aicl-rerun;
		qcom,aicl-rerun-period-s = <180>;
		//Gionee <GN_BSP_CHG> <liujiang> <20161025> add for CR01764517 begin
		qcom,usbin-vadc = <&pmi8950_vadc>;
		qcom,usbin-adc-channel-id = <0>;
		//Gionee <GN_BSP_CHG> <liujiang> <20161025> add for CR01764517 end
		//Gionee BSP wuxh modify for chg led control by triple ic in kernel on 20161009 begin
		qcom,chg-led-support;
		qcom,chg-led-sw-controls;
		//Gionee BSP wuxh modify for chg led control by triple ic in kernel on 20161009 end
		qcom,chgr@1000 {
			reg = <0x1000 0x100>;
			interrupts =	<0x2 0x10 0x0>,
					<0x2 0x10 0x1>,
					<0x2 0x10 0x2>,
					<0x2 0x10 0x3>,
					<0x2 0x10 0x4>,
					<0x2 0x10 0x5>,
					<0x2 0x10 0x6>,
					<0x2 0x10 0x7>;

			interrupt-names =	"chg-error",
						"chg-inhibit",
						"chg-prechg-sft",
						"chg-complete-chg-sft",
						"chg-p2f-thr",
						"chg-rechg-thr",
						"chg-taper-thr",
						"chg-tcc-thr";
		};

		qcom,otg@1100 {
			reg = <0x1100 0x100>;
			interrupts =	<0x2 0x11 0x0>,
					<0x2 0x11 0x1>,
					<0x2 0x11 0x3>;
			interrupt-names =	"otg-fail",
						"otg-oc",
					"usbid-change";
		};

		qcom,bat-if@1200 {
			reg = <0x1200 0x100>;
			interrupts =	<0x2 0x12 0x0>,
					<0x2 0x12 0x1>,
					<0x2 0x12 0x2>,
					<0x2 0x12 0x3>,
				<0x2 0x12 0x4>,
					<0x2 0x12 0x5>,
					<0x2 0x12 0x6>,
					<0x2 0x12 0x7>;

			interrupt-names =	"batt-hot",
						"batt-warm",
						"batt-cold",
						"batt-cool",
					"batt-ov",
						"batt-low",
						"batt-missing",
						"batt-term-missing";
		};

		qcom,usb-chgpth@1300 {
			reg = <0x1300 0x100>;
			interrupts =	<0x2 0x13 0x0>,
					<0x2 0x13 0x1>,
				<0x2 0x13 0x2>,
					<0x2 0x13 0x5>;

			interrupt-names =	"usbin-uv",
					"usbin-ov",
						"usbin-src-det",
						"aicl-done";
		};

		qcom,dc-chgpth@1400 {
			reg = <0x1400 0x100>;
			interrupts =	<0x2 0x14 0x0>,
					<0x2 0x14 0x1>;
			interrupt-names =	"dcin-uv",
						"dcin-ov";
		};

		qcom,chgr-misc@1600 {
			reg = <0x1600 0x100>;
			interrupts =	<0x2 0x16 0x0>,
					<0x2 0x16 0x1>,
					<0x2 0x16 0x2>,
				<0x2 0x16 0x3>,
					<0x2 0x16 0x4>,
					<0x2 0x16 0x5>;

			interrupt-names =	"power-ok",
						"temp-shutdown",
						"wdog-timeout",
						"flash-fail",
						"otst2",
						"otst3";
		};
	};








	voters相关这个相关的函数，变量的作用

	struct client_vote {
		int	state;
		int	value;
	};


	struct votable {
		struct client_vote	votes[NUM_MAX_CLIENTS];
		struct device		*dev;
		const char		*name;
		int			num_clients;
		int			type;
		int			effective_client_id;
		int			effective_result;
		int			default_result;
		struct mutex		vote_lock;
		int			(*callback)(struct device *dev,
							int effective_result,
							int effective_client,
							int last_result,
							int last_client);
	};


	/* voters */
	struct votable			*fcc_votable;
	struct votable			*usb_icl_votable;
	struct votable			*dc_icl_votable;
	struct votable			*usb_suspend_votable;
	struct votable			*dc_suspend_votable;
	struct votable			*battchg_suspend_votable;






	struct votable *create_votable(struct device *dev, const char *name,
					int votable_type,
					int num_clients,
					int default_result,
					int (*callback)(struct device *dev,
							int effective_result,
							int effective_client,
							int last_result,
							int last_client)
					)
	{
		int i;
		struct votable *votable = devm_kzalloc(dev, sizeof(struct votable),
								GFP_KERNEL);

		if (!votable)
			return ERR_PTR(-ENOMEM);

		if (!callback) {
			dev_err(dev, "Invalid callback specified for voter\n");
			return ERR_PTR(-EINVAL);
		}

		if (votable_type >= NUM_VOTABLE_TYPES) {
			dev_err(dev, "Invalid votable_type specified for voter\n");
			return ERR_PTR(-EINVAL);
		}

		if (num_clients > NUM_MAX_CLIENTS) {
			dev_err(dev, "Invalid num_clients specified for voter\n");
			return ERR_PTR(-EINVAL);
		}

		votable->dev = dev;
		votable->name = name;
		votable->num_clients = num_clients;
		votable->callback = callback;
		votable->type = votable_type;
		votable->default_result = default_result;
		mutex_init(&votable->vote_lock);

		/*
		* Because effective_result and client states are invalid
		* before the first vote, initialize them to -EINVAL
		*/
		votable->effective_result = -EINVAL;
		votable->effective_client_id = -EINVAL;

		for (i = 0; i < votable->num_clients; i++)
			votable->votes[i].state = -EINVAL;

		return votable;



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

}






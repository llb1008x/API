
/*
 *如果要修改代码：参照下面的方式
 */

1.添加的调试代码,调试的等级需要根据情况调整

//GioneeDrv LiLuBao 20161009 modify for platform change begin
battery_log(BAT_LOG_FULL, "wait event 1\n");
//GioneeDrv LiLuBao 20161009 modify for platform change end


自己添加的宏
#define  GNDRV_LLB_CHANGE
修改模式
//GioneeDrv LiLuBao 20161109 modify for platform change begin

#if defined(GNDRV_LLB_CHANGE)
     KPOC_LOGI("[ChargingAnimation: %s %d],in  bootlogo_show_charging modify by llb",__FUNCTION__,__LINE__);	
#endif


......

//GioneeDrv LiLuBao 20161009 modify for platform change end




#define BMLOG_DEBUG_LEVEL   7

#define bm_debug(fmt, args...)   \
do {									\
	if (Enable_FGADC_LOG >= BMLOG_DEBUG_LEVEL) {		\
		pr_debug(fmt, ##args); \
	}								   \
} while (0)



2.调试代码原型

/*
 * These can be used to print at the various log levels.
 * All of these will print unconditionally, although note that pr_debug()
 * and other debug macros are compiled out unless either DEBUG is defined
 * or CONFIG_DYNAMIC_DEBUG is set.
 */
#if 0
#define pr_emerg(fmt, ...) \
	printk(KERN_EMERG "[name:"KBUILD_MODNAME"&]"pr_fmt(fmt), ##__VA_ARGS__)
#define pr_alert(fmt, ...) \
	printk(KERN_ALERT "[name:"KBUILD_MODNAME"&]"pr_fmt(fmt), ##__VA_ARGS__)
#define pr_crit(fmt, ...) \
	printk(KERN_CRIT "[name:"KBUILD_MODNAME"&]"pr_fmt(fmt), ##__VA_ARGS__)
#define pr_err(fmt, ...) \
	printk(KERN_ERR "[name:"KBUILD_MODNAME"&]"pr_fmt(fmt), ##__VA_ARGS__)
#define pr_warning(fmt, ...) \
	printk(KERN_WARNING "[name:"KBUILD_MODNAME"&]"pr_fmt(fmt), ##__VA_ARGS__)
#define pr_warn pr_warning
#define pr_notice(fmt, ...) \
	printk(KERN_NOTICE "[name:"KBUILD_MODNAME"&]"pr_fmt(fmt), ##__VA_ARGS__)
#define pr_info(fmt, ...) \
	printk(KERN_INFO "[name:"KBUILD_MODNAME"&]"pr_fmt(fmt), ##__VA_ARGS__)
#define pr_cont(fmt, ...) \
	printk(KERN_CONT "[name:"KBUILD_MODNAME"&]"fmt, ##__VA_ARGS__)

#endif

/* pr_devel() should produce zero code unless DEBUG is defined */
#ifdef DEBUG
#define pr_devel(fmt, ...) \
	printk(KERN_DEBUG "[name:"KBUILD_MODNAME"&]"pr_fmt(fmt), ##__VA_ARGS__)
#else
#define pr_devel(fmt, ...) \
	no_printk(KERN_DEBUG "[name:"KBUILD_MODNAME"&]"pr_fmt(fmt), ##__VA_ARGS__)
#endif

#include <linux/dynamic_debug.h>

/* If you are writing a driver, please use dev_dbg instead */
#if defined(CONFIG_DYNAMIC_DEBUG)
/* dynamic_pr_debug() uses pr_fmt() internally so we don't need it here */
#define pr_debug(fmt, ...) \
	dynamic_pr_debug("[name:"KBUILD_MODNAME"&]"fmt, ##__VA_ARGS__)
#elif defined(DEBUG)
#define pr_debug(fmt, ...) \
	printk(KERN_DEBUG "[name:"KBUILD_MODNAME"&]"pr_fmt(fmt), ##__VA_ARGS__)
#else
#define pr_debug(fmt, ...) \
	no_printk(KERN_DEBUG "[name:"KBUILD_MODNAME"&]"pr_fmt(fmt), ##__VA_ARGS__)
#endif


/* -------printk too much patch------ */
#if defined CONFIG_MT_ENG_BUILD && defined CONFIG_LOG_TOO_MUCH_WARNING

#define pr_emerg(fmt, ...) \
({											\
	static bool __print_once __read_mostly;	\
	if (!__print_once) {					\
		DEFINE_DYNAMIC_DEBUG_METADATA(descriptor, "[name:"KBUILD_MODNAME"&]"fmt); \
		if (unlikely(descriptor.flags & _DPRINTK_FLAGS_PRINT)) \
			barrier();   \
		__print_once = true;				\
		printk(KERN_EMERG "[name:"KBUILD_MODNAME"&]"pr_fmt(fmt), ##__VA_ARGS__); \
	}	else								\
		printk(KERN_EMERG "[name:"KBUILD_MODNAME"&]"pr_fmt(fmt), ##__VA_ARGS__); \
})


#define pr_alert(fmt, ...) \
({											\
	static bool __print_once __read_mostly;	\
	if (!__print_once) {					\
		DEFINE_DYNAMIC_DEBUG_METADATA(descriptor, "[name:"KBUILD_MODNAME"&]"fmt); \
		if (unlikely(descriptor.flags & _DPRINTK_FLAGS_PRINT)) \
			barrier();   \
		__print_once = true;				\
		printk(KERN_ALERT "[name:"KBUILD_MODNAME"&]"pr_fmt(fmt), ##__VA_ARGS__); \
	}	else								\
		printk(KERN_ALERT "[name:"KBUILD_MODNAME"&]"pr_fmt(fmt), ##__VA_ARGS__); \
})

#define pr_crit(fmt, ...) \
({											\
	static bool __print_once __read_mostly;	\
	if (!__print_once) {					\
		DEFINE_DYNAMIC_DEBUG_METADATA(descriptor, "[name:"KBUILD_MODNAME"&]"fmt); \
		if (unlikely(descriptor.flags & _DPRINTK_FLAGS_PRINT)) \
			barrier();   \
		__print_once = true;				\
		printk(KERN_CRIT "[name:"KBUILD_MODNAME"&]"pr_fmt(fmt), ##__VA_ARGS__); \
	}	else								\
		printk(KERN_CRIT "[name:"KBUILD_MODNAME"&]"pr_fmt(fmt), ##__VA_ARGS__); \
})


#define pr_err(fmt, ...) \
({											\
	static bool __print_once __read_mostly;	\
	if (!__print_once) {					\
		DEFINE_DYNAMIC_DEBUG_METADATA(descriptor, "[name:"KBUILD_MODNAME"&]"fmt); \
		if (unlikely(descriptor.flags & _DPRINTK_FLAGS_PRINT)) \
			barrier();   \
		__print_once = true;				\
		printk(KERN_ERR "[name:"KBUILD_MODNAME"&]"pr_fmt(fmt), ##__VA_ARGS__); \
	}	else								\
		printk(KERN_ERR "[name:"KBUILD_MODNAME"&]"pr_fmt(fmt), ##__VA_ARGS__); \
})

#define pr_warning(fmt, ...) \
({											\
	static bool __print_once __read_mostly;	\
	if (!__print_once) {					\
		DEFINE_DYNAMIC_DEBUG_METADATA(descriptor, "[name:"KBUILD_MODNAME"&]"fmt); \
		if (unlikely(descriptor.flags & _DPRINTK_FLAGS_PRINT)) \
			barrier();   \
		__print_once = true;				\
		printk(KERN_WARNING "[name:"KBUILD_MODNAME"&]"pr_fmt(fmt), ##__VA_ARGS__); \
	}	else								\
		printk(KERN_WARNING "[name:"KBUILD_MODNAME"&]"pr_fmt(fmt), ##__VA_ARGS__); \
})

#define pr_warn pr_warning
#define pr_notice(fmt, ...) \
({											\
	static bool __print_once __read_mostly;	\
	if (!__print_once) {					\
		DEFINE_DYNAMIC_DEBUG_METADATA(descriptor, "[name:"KBUILD_MODNAME"&]"fmt); \
		if (unlikely(descriptor.flags & _DPRINTK_FLAGS_PRINT)) \
			barrier();   \
		__print_once = true;				\
		printk(KERN_NOTICE "[name:"KBUILD_MODNAME"&]"pr_fmt(fmt), ##__VA_ARGS__); \
	}	else								\
		printk(KERN_NOTICE "[name:"KBUILD_MODNAME"&]"pr_fmt(fmt), ##__VA_ARGS__); \
})

#define pr_info(fmt, ...)          \
({											\
	static bool __print_once __read_mostly;	\
	if (!__print_once) {					\
		DEFINE_DYNAMIC_DEBUG_METADATA(descriptor, "[name:"KBUILD_MODNAME"&]"fmt); \
		if (unlikely(descriptor.flags & _DPRINTK_FLAGS_PRINT)) \
			barrier();   \
		__print_once = true;				\
		printk(KERN_INFO "[name:"KBUILD_MODNAME"&]"pr_fmt(fmt), ##__VA_ARGS__); \
	}	else								\
		printk(KERN_INFO "[name:"KBUILD_MODNAME"&]"pr_fmt(fmt), ##__VA_ARGS__); \
})

#define pr_cont(fmt, ...) \
({											\
	static bool __print_once __read_mostly;	\
	if (!__print_once) {					\
		DEFINE_DYNAMIC_DEBUG_METADATA(descriptor, "[name:"KBUILD_MODNAME"&]"fmt); \
		if (unlikely(descriptor.flags & _DPRINTK_FLAGS_PRINT)) \
			barrier();   \
		__print_once = true;				\
		printk(KERN_CONT "[name:"KBUILD_MODNAME"&]"pr_fmt(fmt), ##__VA_ARGS__); \
	}	else								\
		printk(KERN_CONT "[name:"KBUILD_MODNAME"&]"pr_fmt(fmt), ##__VA_ARGS__); \
})

#else
#define pr_emerg(fmt, ...) \
	printk(KERN_EMERG pr_fmt(fmt), ##__VA_ARGS__)
#define pr_alert(fmt, ...) \
	printk(KERN_ALERT pr_fmt(fmt), ##__VA_ARGS__)
#define pr_crit(fmt, ...) \
	printk(KERN_CRIT pr_fmt(fmt), ##__VA_ARGS__)
#define pr_err(fmt, ...) \
	printk(KERN_ERR pr_fmt(fmt), ##__VA_ARGS__)
#define pr_warning(fmt, ...) \
	printk(KERN_WARNING pr_fmt(fmt), ##__VA_ARGS__)
#define pr_warn pr_warning
#define pr_notice(fmt, ...) \
	printk(KERN_NOTICE pr_fmt(fmt), ##__VA_ARGS__)
#define pr_info(fmt, ...) \
	printk(KERN_INFO pr_fmt(fmt), ##__VA_ARGS__)
#define pr_cont(fmt, ...) \
	printk(KERN_CONT fmt, ##__VA_ARGS__)

#endif

/*
 * Print a one-time message (analogous to WARN_ONCE() et al):
 */

#ifdef CONFIG_PRINTK
#define printk_once(fmt, ...)					\
({								\
	static bool __print_once __read_mostly;			\
								\
	if (!__print_once) {					\
		__print_once = true;				\
		printk(fmt, ##__VA_ARGS__);			\
	}							\
})
#define printk_deferred_once(fmt, ...)				\
({								\
	static bool __print_once __read_mostly;			\
								\
	if (!__print_once) {					\
		__print_once = true;				\
		printk_deferred(fmt, ##__VA_ARGS__);		\
	}							\
})
#else
#define printk_once(fmt, ...)					\
	no_printk(fmt, ##__VA_ARGS__)
#define printk_deferred_once(fmt, ...)				\
	no_printk(fmt, ##__VA_ARGS__)
#endif

#define pr_emerg_once(fmt, ...)					\
	printk_once(KERN_EMERG "[name:"KBUILD_MODNAME"&]"pr_fmt(fmt), ##__VA_ARGS__)
#define pr_alert_once(fmt, ...)					\
	printk_once(KERN_ALERT "[name:"KBUILD_MODNAME"&]"pr_fmt(fmt), ##__VA_ARGS__)
#define pr_crit_once(fmt, ...)					\
	printk_once(KERN_CRIT "[name:"KBUILD_MODNAME"&]"pr_fmt(fmt), ##__VA_ARGS__)
#define pr_err_once(fmt, ...)					\
	printk_once(KERN_ERR "[name:"KBUILD_MODNAME"&]"pr_fmt(fmt), ##__VA_ARGS__)
#define pr_warn_once(fmt, ...)					\
	printk_once(KERN_WARNING "[name:"KBUILD_MODNAME"&]"pr_fmt(fmt), ##__VA_ARGS__)
#define pr_notice_once(fmt, ...)				\
	printk_once(KERN_NOTICE "[name:"KBUILD_MODNAME"&]"pr_fmt(fmt), ##__VA_ARGS__)
#define pr_info_once(fmt, ...)					\
	printk_once(KERN_INFO "[name:"KBUILD_MODNAME"&]"pr_fmt(fmt), ##__VA_ARGS__)
#define pr_cont_once(fmt, ...)					\
	printk_once(KERN_CONT "[name:"KBUILD_MODNAME"&]"pr_fmt(fmt), ##__VA_ARGS__)

#if defined(DEBUG)
#define pr_devel_once(fmt, ...)					\
	printk_once(KERN_DEBUG "[name:"KBUILD_MODNAME"&]"pr_fmt(fmt), ##__VA_ARGS__)
#else
#define pr_devel_once(fmt, ...)					\
	no_printk(KERN_DEBUG "[name:"KBUILD_MODNAME"&]"pr_fmt(fmt), ##__VA_ARGS__)
#endif

/* If you are writing a driver, please use dev_dbg instead */
#if defined(DEBUG)
#define pr_debug_once(fmt, ...)					\
	printk_once(KERN_DEBUG "[name:"KBUILD_MODNAME"&]"pr_fmt(fmt), ##__VA_ARGS__)
#else
#define pr_debug_once(fmt, ...)					\
	no_printk(KERN_DEBUG "[name:"KBUILD_MODNAME"&]"pr_fmt(fmt), ##__VA_ARGS__)
#endif



3.Mt_battery_meter.c  
 fgauge_construct_battery_profile()
/*
 *这段代码根据线性插值法，利用50,10,0,-10,的温度校准值构造不同温度下的ZCV
 *这边就是 MTK 为了结合实际温度 获取较为精确的电池信息 而采取的线性平均值法。
 *原理是利用预先测得的分布在-10 0 25 50 摄氏度下的 ZCV 表,结合真实温度,
 *动态重构一张当 前温度下的 ZCV 表格
 */
 
 void fgauge_construct_battery_profile_init(void)
{
	BATTERY_PROFILE_STRUCT_P temp_profile_p, profile_p[PROFILE_SIZE];
	int i, j, saddles, profile_index;
	signed int low_p = 0, high_p = 0, now_p = 0, low_vol = 0, high_vol = 0;

	profile_p[0] = fgauge_get_profile(batt_meter_cust_data.temperature_t0);
	profile_p[1] = fgauge_get_profile(batt_meter_cust_data.temperature_t1);
	profile_p[2] = fgauge_get_profile(batt_meter_cust_data.temperature_t2);
	profile_p[3] = fgauge_get_profile(batt_meter_cust_data.temperature_t3);
	saddles = fgauge_get_saddles();
	temp_profile_p =
	    (BATTERY_PROFILE_STRUCT_P) kmalloc(51 * sizeof(*temp_profile_p), GFP_KERNEL);
	memset(temp_profile_p, 0, 51 * sizeof(*temp_profile_p));
	for (i = 0; i < PROFILE_SIZE; i++) {
		profile_index = 0;
		for (j = 0; j * 2 <= 100; j++) {
			while (profile_index < saddles && profile_index >= 0) {
				if (((profile_p[i] + profile_index)->percentage) < j * 2) {
					profile_index++;
					continue;
				} else if (((profile_p[i] + profile_index)->percentage) == j * 2) {
					(temp_profile_p + j)->voltage =
					    (profile_p[i] + profile_index)->voltage;
					(temp_profile_p + j)->percentage =
					    (profile_p[i] + profile_index)->percentage;
					break;
				}
				low_p = (profile_p[i] + profile_index - 1)->percentage;
				high_p = (profile_p[i] + profile_index)->percentage;
				now_p = j * 2;
				low_vol = (profile_p[i] + profile_index)->voltage;
				high_vol = (profile_p[i] + profile_index - 1)->voltage;
				(temp_profile_p + j)->voltage =
				    (low_vol * 1000 +
				     ((high_vol - low_vol) * 1000 * (now_p - low_p) / (high_p -
										       low_p))) /
				    1000;
				(temp_profile_p + j)->percentage = j * 2;

				break;
			}
			bm_print(BM_LOG_CRTI, "new battery_profile[%d,%d] <%d,%d>\n", i, j,
				 (temp_profile_p + j)->percentage, (temp_profile_p + j)->voltage);
		}

		for (j = 0; j * 2 <= 100; j++) {
			(profile_p[i] + j)->voltage = (temp_profile_p + j)->voltage;
			(profile_p[i] + j)->percentage = (temp_profile_p + j)->percentage;
		}
	}
	kfree(temp_profile_p);
}
 
 
for (i = 0; i < saddles; i++) {
	if (((high_profile_p + i)->voltage) > ((low_profile_p + i)->voltage)) {
		temp_v_1 = (high_profile_p + i)->voltage;
		temp_v_2 = (low_profile_p + i)->voltage;

		(temp_profile_p + i)->voltage = temp_v_2 +
			(((temperature - low_temperature) * (temp_v_1 - temp_v_2)
			 ) / (high_temperature - low_temperature)
			);
	} else {
		temp_v_1 = (low_profile_p + i)->voltage;
		temp_v_2 = (high_profile_p + i)->voltage;

		(temp_profile_p + i)->voltage = temp_v_2 +
			(((high_temperature - temperature) * (temp_v_1 - temp_v_2)
			 ) / (high_temperature - low_temperature)
			);
	}



4.
/*
 *bm_func 这是一组函数指针数组，定义了一组函数指针，实现对MTK充电算法的实现
 *
 */
	
static signed int(*bm_func[BATTERY_METER_CMD_NUMBER]) (void *data);

signed int bm_ctrl_cmd(BATTERY_METER_CTRL_CMD cmd, void *data)
{
	signed int status = STATUS_UNSUPPORTED;
	static signed int init = -1;

	if (init == -1) {
		init = 0;
		bm_func[BATTERY_METER_CMD_HW_FG_INIT] = fgauge_initialization;
		bm_func[BATTERY_METER_CMD_GET_HW_FG_CURRENT] = fgauge_read_current;
		bm_func[BATTERY_METER_CMD_GET_HW_FG_CURRENT_SIGN] = fgauge_read_current_sign;
		bm_func[BATTERY_METER_CMD_GET_HW_FG_CAR] = fgauge_read_columb;
		bm_func[BATTERY_METER_CMD_HW_RESET] = fgauge_hw_reset;
		bm_func[BATTERY_METER_CMD_GET_ADC_V_BAT_SENSE] = read_adc_v_bat_sense;
		bm_func[BATTERY_METER_CMD_GET_ADC_V_I_SENSE] = read_adc_v_i_sense;
		bm_func[BATTERY_METER_CMD_GET_ADC_V_BAT_TEMP] = read_adc_v_bat_temp;
		bm_func[BATTERY_METER_CMD_GET_ADC_V_CHARGER] = read_adc_v_charger;
		bm_func[BATTERY_METER_CMD_GET_HW_OCV] = read_hw_ocv;
		bm_func[BATTERY_METER_CMD_DUMP_REGISTER] = dump_register_fgadc;
		bm_func[BATTERY_METER_CMD_SET_COLUMB_INTERRUPT] = fgauge_set_columb_interrupt;
		bm_func[BATTERY_METER_CMD_GET_BATTERY_PLUG_STATUS] = read_battery_plug_out_status;
		bm_func[BATTERY_METER_CMD_GET_HW_FG_CAR_ACT] = fgauge_read_columb_accurate;
		bm_func[BATTERY_METER_CMD_GET_IS_HW_OCV_READY] = read_is_hw_ocv_ready;
	}

	if (cmd < BATTERY_METER_CMD_NUMBER) {
		if (bm_func[cmd] != NULL)
				status = bm_func[cmd] (data);
		else
			status = STATUS_UNSUPPORTED;
	} else
		status = STATUS_UNSUPPORTED;

	return status;
}


5.
/*
 *battery_charging_control 函数指针的用法，一定要学会get！
 */
 
//定义一个函数指针，参数类型和返回值类型
typedef signed int(*CHARGING_CONTROL) (CHARGING_CTRL_CMD cmd, void *data);

//定义函数指针变量
CHARGING_CONTROL battery_charging_control;

//枚举要使用的命令
enum CHARGING_CTRL_CMD {
	CHARGING_CMD_INIT,
	CHARGING_CMD_DUMP_REGISTER,
	CHARGING_CMD_ENABLE,
	CHARGING_CMD_SET_CV_VOLTAGE,
	CHARGING_CMD_GET_CURRENT,
	CHARGING_CMD_SET_CURRENT,
	CHARGING_CMD_GET_INPUT_CURRENT,
	CHARGING_CMD_SET_INPUT_CURRENT,
	CHARGING_CMD_GET_CHARGING_STATUS,
	CHARGING_CMD_RESET_WATCH_DOG_TIMER,
	CHARGING_CMD_SET_HV_THRESHOLD,
	CHARGING_CMD_GET_HV_STATUS,
	CHARGING_CMD_GET_BATTERY_STATUS,
	CHARGING_CMD_GET_CHARGER_DET_STATUS,
	CHARGING_CMD_GET_CHARGER_TYPE,
	CHARGING_CMD_GET_IS_PCM_TIMER_TRIGGER,
	CHARGING_CMD_SET_PLATFORM_RESET,
	CHARGING_CMD_GET_PLATFORM_BOOT_MODE,
	CHARGING_CMD_ENABLE_POWERPATH,
	CHARGING_CMD_BOOST_ENABLE,
	CHARGING_CMD_SET_TA_CURRENT_PATTERN,
	CHARGING_CMD_NUMBER
};


/*定义轮训接口*/
static void get_charging_control(void)
{
	battery_charging_control = chr_control_interface;
}


/*对具体的函数调用指针轮训，调用。这样操作是为了方便移植，可以针对不同的芯片定义不同的
chargnig_func函数指针数组*/
kal_int32 chr_control_interface(CHARGING_CTRL_CMD cmd, void *data)
{
	kal_int32 status;
	//GioneeDrv GuoJianqiu 20151223 modify for platform change begin
	if (cmd < CHARGING_CMD_NUMBER) {
		if (charging_func[cmd] != NULL)
			status = charging_func[cmd](data);
		else {
		battery_log(BAT_LOG_CRTI, "[chr_control_interface]cmd:%d not 
		supported\n", cmd);
			status = STATUS_UNSUPPORTED;
		}
	} else
		status = STATUS_UNSUPPORTED;
	//GioneeDrv GuoJianqiu 20151223 modify for platform change end
	return status;
}

static kal_uint32(*const charging_func[CHARGING_CMD_NUMBER]) (void *data) = {
		charging_hw_init, charging_dump_register,
		charging_enable, charging_set_cv_voltage,
	    charging_get_current, charging_set_current, charging_set_input_current,
	    charging_get_charging_status, charging_reset_watch_dog_timer,
	    charging_set_hv_threshold, charging_get_hv_status,
		charging_get_battery_status,
	    charging_get_charger_det_status, charging_get_charger_type,
	    charging_get_is_pcm_timer_trigger, charging_set_platform_reset,
	    charging_get_platfrom_boot_mode, charging_set_power_off,
	    charging_get_power_source, charging_get_csdac_full_flag,
	    charging_set_ta_current_pattern,   charging_set_error_state,charging_diso_init,
	    charging_get_diso_state, 	
		//GioneeDrv GuoJianqiu 20151223 modify for 	 platform change begin
	    charging_set_chrind_ck_pdn, charging_sw_init}; 
		//GioneeDrv GuoJianqiu 20151223 modify for platform change end
		
		

		/*定义这样一个函数指针，负责调用不同的函数处理不同的命令和数据*/
		typedef  s32(*BATTERY_METER_CONTROL)  (int cmd, void *data);
		BATTERY_METER_CONTROL  battery_meter_ctrl = NULL;

		/*将bm_ctrl_cmd接口给那个函数指针，调用不同的函数接口*/
		battery_meter_ctrl = bm_ctrl_cmd;

 
6./*代码相关的文件（注意不同的项目不一样）
	芯片相关的代码：/home/llb/project/PRO/source/BBL7337A/L27_6750_66_BBL7337A_L1.MP10.V1_160427_ALPS/android_mtk_6750_mp/kernel-3.18/drivers/misc/mediatek/power/mt6755
	充电相关的代码：/home/llb/project/PRO/source/BBL7337A/L27_6750_66_BBL7337A_L1.MP10.V1_160427_ALPS/android_mtk_6750_mp/kernel-3.18/drivers/power/mediatek
			修改的gionee目录下：/home/llb/project/PRO/source/BBL7337A/L27_6750_66_BBL7337A_L1.MP10.V1_160427_ALPS/android_mtk_6750_mp/gionee/code/driver/project_common/BBL7337_DRV_COMMON/	kernel-3.18/drivers/power/mediatek

	设备资源的设备树目录：/home/llb/project/PRO/source/BBL7337A/L27_6750_66_BBL7337A_L1.MP10.V1_160427_ALPS/android_mtk_6750_mp/kernel-3.18/arch/arm/boot/dts/mt6755.dtsi

	一些宏和编译的配置在（根据不同的版本）：/home/llb/project/PRO/source/BBL7337A/L27_6750_66_BBL7337A_L1.MP10.V1_160427_ALPS/android_mtk_6750_mp/gionee/config	

	USB相关的目录

	上层选择模式传到底层的对应参数修改
	/home/llb/project/PRO/source/G1605A/L28_6737M_65_G1605A_M0.MP1_V2.140.3_160908_ALPS/android_mtk_6737m_65_mp/gionee/code/driver/project_common/G1605A_DRV_COMMON/device/mediatek/mt6735


	编译相关的目录
	/home/llb/project/PRO/source/BBL7337A/L27_6750_66_BBL7337A_L1.MP10.V1_160427_ALPS/android_mtk_6750_mp/gionee/config

	/.local/share/Trash/files 垃圾箱目录？
*/




8./*这个宏的定义是什么意思 
相比以前的#if CONFIG_POWER_EXIT 用法要灵活
当然条件编译的用法要学*/
#if defined(CONFIG_POWER_EXT)  /*条件编译 定义了这个宏就为真*/



9./*定时器*/
	if (time_after(last_timer, timeout)) {
		if (!timer_pending(&musb_idle_timer))
			last_timer = timeout;
		else {
			DBG(2, "Longer idle timer already pending, ignoring\n");
			return;
		}
	}
	last_timer = timeout;


10./*察看中断号 cat  /proc/interrupts

G1605A的中断*/
           CPU0       CPU1       
 29:          0          0       GIC  29  arch_timer
 30:     250493      63863       GIC  30  arch_timer
 96:          0          0       GIC  96  mtk_cpuxgpt0
 97:          0          0       GIC  97  mtk_cpuxgpt1
 98:          0          0       GIC  98  mtk_cpuxgpt2
 99:          0          0       GIC  99  mtk_cpuxgpt3
104:        102         65       GIC 104  musb-hdrc.0.auto
110:          0          0       GIC 110  mtk-thermal
111:       6423       9067       GIC 111  mtk-msdc
112:          0          0       GIC 112  mtk-msdc
114:        450        471       GIC 114  mtk-msdc
116:         27         77       GIC 116  mt-i2c
117:       1209       1228       GIC 117  mt-i2c
118:      13383       1002       GIC 118  mt-i2c
119:          0          0       GIC 119  mt-i2c
122:          7         16       GIC 122  mtk btif irq
123:        495        185       GIC 123  mtk-uart
127:       4393       2192       GIC 127
145:        762        638       GIC 145  mtk btif tx dma irq
146:        357        623       GIC 146  mtk btif rx dma irq
150:          0          0       GIC 150  1100a000.spi
156:          0          0       GIC 156  IRTX
157:        500        193       GIC 157  ptp
160:          0          0       GIC 160  mt_wdt
166:          3          1       GIC 166  mt_emi_mpu, devapc
172:          0          0       GIC 172  CCIF0_AP
176:          0          0       GIC 176  Afe_ISR_Handle
177:       1438        421       GIC 177  CLDMA_AP
178:          0          0       GIC 178  m4u
182:          0          0       GIC 182  gcpu
183:       3732       2384       GIC 183  mtk_cmdq
184:          0          0       GIC 184  mt-gpt
195:          0          0       GIC 195  pmic_wrap
196:          0          0       GIC 196  mtk-kpd
197:          0          0       GIC 197  SPM
198:          0          0       GIC 198  SPM
199:          0          0       GIC 199  SPM
200:          0          0       GIC 200  SPM
211:          0          0       GIC 211  Vcodec
212:          0          0       GIC 212  Vcodec
213:          0          0       GIC 213  jpeg_enc_driver
215:          0          0       GIC 215  ISP
218:       4595       4169       GIC 218  DISPSYS
225:       2795       2629       GIC 225  DISPSYS
226:       7778       8080       GIC 226  DISPSYS
227:          0          0       GIC 227  DISPSYS
228:          1          4       GIC 228  DISPSYS
231:       1703       1448       GIC 231  DISPSYS
235:         57         44       GIC 235  DISPSYS
237:          0          0       GIC 237  DISPSYS
238:          0          0       GIC 238  ISP
239:          0          0       GIC 239  ISP
240:          0          0       GIC 240  camera-fdvt
241:         13         12       GIC 241  jpeg_dec_driver
242:       4789       5013       GIC 242  13040000.mali
243:          0          0       GIC 243  13040000.mali
244:       2241       2366       GIC 244  13040000.mali
253:          0          0       GIC 253  MD_WDT
258:      14527       9034       GIC 258  AHB_SLAVE_HIF
259:          0          1       GIC 259  BTIF_WAKEUP_IRQ
260:          0          0       GIC 260  BTCVSD_ISR_Handle
261:          0          0       GIC 261  C2K_CCCI
262:       1602       1363       GIC 262  C2K_CCCI
263:          0          0       GIC 263  CIRQ
280:        106       2381       GIC 280  mobicore
281:          0          0       GIC 281  ATF_irq
288:          0          0   mt-eint   0  home_key_eint
290:          0          1   mt-eint   2  goodix_fp_irq
293:          0          0   mt-eint   5  11240000.msdc1 cd
294:          0          0   mt-eint   6  accdet-eint
298:          2          6   mt-eint  10  mtk-tpd
334:          0          0   mt-eint  46  USB_IDDIG
376:          0          0   mt-eint  88  ALS-eint
494:         95          0   mt-eint 206  pmic-eint
496:          0          0   mt-eint 208  mdm_reset_ind
498:          0          0   mt-eint 210  cbp(c2k EE)
499:        344         41   mt-eint 211  cp_indicate_state
500:        328         60   mt-eint 212  cp_wake_ap
IPI0:    458229     368679       Rescheduling interrupts
IPI1:       306        598       Function call interrupts
IPI2:       250       2220       Single function call interrupts
IPI3:         0          0       CPU stop interrupts
IPI4:         0          0       Timer broadcast interrupts
IPI5:         0          0       IRQ work interrupts



11./*usb_otg设备的处理过程和GPIO的配置*/

	/*1.设备树相关的配置(基本的设备树的配置)*/

	usb0:usb20@11200000 {				//USB20的基地址为11200000
		compatible = "mediatek,mt6735-usb20";	//通过设备树匹配的名称mt6735-usb20
		cell-index = <0>;			//子树有几个	
		reg = <0x11200000 0x10000>,		//寄存器的地址和范围	
		      <0x11210000 0x10000>;
		interrupts = <GIC_SPI 72 IRQ_TYPE_LEVEL_LOW>;//设置中断模式：中断类型，中断号，中断触发的类型
		mode = <2>;
		multipoint = <1>;
		num_eps = <16>;				//设备树添加的一些属性，共有16个端点描述符
		clocks = <&perisys PERI_USB0>;
		clock-names = "usb0";
		vusb33-supply = <&mt_pmic_vusb33_ldo_reg>;//这个是设置的什么意思？
		iddig_gpio = <0 1>;			//iddig_gpio = <46 1>;	 //寄存器的位置改了，			
		drvvbus_gpio = <83 2>;			//drvvbus_gpio = <60 2>;
	};

	/* USB GPIO Kernal Standardization start */
	/* GioneeDrv GuoJianqiu 20160425 modify for OTG function begin */
	&pio {
		usb_default: usb_default {
		};

		gpio46_mode1_iddig: iddig_irq_init {
			pins_cmd_dat {
				pins = <PINMUX_GPIO46__FUNC_IDDIG>;
				slew-rate = <0>;
				bias-pull-up = <00>;
			};
		};

		gpio60_mode2_drvvbus: drvvbus_init {
			pins_cmd_dat {
				pins = <PINMUX_GPIO60__FUNC_GPIO60>;
				slew-rate = <1>;
				bias-pull-down = <00>;
			};
		};

		gpio60_mode2_drvvbus_low: drvvbus_low {
			pins_cmd_dat {
				pins = <PINMUX_GPIO60__FUNC_GPIO60>;
				slew-rate = <1>;
				output-low;
				bias-pull-down = <00>;
			};
		};

		gpio60_mode2_drvvbus_high: drvvbus_high {
			pins_cmd_dat {
				pins = <PINMUX_GPIO60__FUNC_GPIO60>;
				slew-rate = <1>;
				output-high;
				bias-pull-down = <00>;
			};
		};
	};

	&usb0 {
		iddig_gpio = <46 1>;
		pinctrl-names = "usb_default", "iddig_irq_init", "drvvbus_init", "drvvbus_low", "drvvbus_high";
		pinctrl-0 = <&usb_default>;
		pinctrl-1 = <&gpio46_mode1_iddig>;
		pinctrl-2 = <&gpio60_mode2_drvvbus>;
		pinctrl-3 = <&gpio60_mode2_drvvbus_low>;
		pinctrl-4 = <&gpio60_mode2_drvvbus_high>;
		status = "okay";
	};

12./*枚举的用法enum*/







13./*充电相关的几个代码*/
	
	1.upmu_is_chr_det()
	字面意思是电源管理系统对充电的检测，至于这个宏CONFIG_MTK_DUAL_INPUT_CHARGER_SUPPORT应该是是否支持电池给手机充电或者电池给外设充电
	检测充电器是否存在
	
	2.数据结构
	//充电接口类型
	typedef enum {							
		CHARGER_UNKNOWN = 0,	/* 未充电 */
		STANDARD_HOST,		/* USB : 450mA */
		CHARGING_HOST,		/* 往外供电 */
		NONSTANDARD_CHARGER,	/* AC : 450mA~1A */
		STANDARD_CHARGER,	/* AC 充电器的电流大: ~1A */
	} CHARGER_TYPE;
	

	/*pmic中断的设置，（可能是触发了某个中断，转换成这个中断对应的状态，什么样的配置）	*/
	/*****************************************************************************
	 * interrupt Setting
	 ******************************************************************************/
	
	//设置每个引脚的名称
	#define PMIC_S_INT_GEN(_name)	\
	{	\
		.name =  #_name,	\
	}	
	
	//设置每个引脚的地址，对应的配置，设置引脚，对应的中断
	#define PMIC_M_INTS_GEN(adr, enA, setA, clearA, interrupt)	\
	{	\
		.address =  adr,	\
		.en =  enA,	\
		.set =  setA,	\
		.clear =  clearA,	\
		.interrupts = interrupt,	\
	}

	static struct pmic_interrupt_bit interrupt_status0[] = {
		PMIC_S_INT_GEN(RG_INT_STATUS_PWRKEY),
		PMIC_S_INT_GEN(RG_INT_STATUS_HOMEKEY),
		PMIC_S_INT_GEN(RG_INT_STATUS_PWRKEY_R),
		PMIC_S_INT_GEN(RG_INT_STATUS_HOMEKEY_R),
		PMIC_S_INT_GEN(RG_INT_STATUS_THR_H),
		PMIC_S_INT_GEN(RG_INT_STATUS_THR_L),
		PMIC_S_INT_GEN(RG_INT_STATUS_BAT_H),
		PMIC_S_INT_GEN(RG_INT_STATUS_BAT_L),
		PMIC_S_INT_GEN(NO_USE),
		PMIC_S_INT_GEN(RG_INT_STATUS_RTC),
		PMIC_S_INT_GEN(RG_INT_STATUS_AUDIO),
		PMIC_S_INT_GEN(NO_USE),
		PMIC_S_INT_GEN(RG_INT_STATUS_ACCDET),
		PMIC_S_INT_GEN(RG_INT_STATUS_ACCDET_EINT),
		PMIC_S_INT_GEN(RG_INT_STATUS_ACCDET_NEGV),
		PMIC_S_INT_GEN(RG_INT_STATUS_NI_LBAT_INT),
	};

	static struct pmic_interrupt_bit interrupt_status1[] = {
		PMIC_S_INT_GEN(RG_INT_STATUS_VPROC_OC),
		PMIC_S_INT_GEN(RG_INT_STATUS_VSYS_OC),
		PMIC_S_INT_GEN(RG_INT_STATUS_VLTE_OC),
		PMIC_S_INT_GEN(NO_USE),
		PMIC_S_INT_GEN(NO_USE),
		PMIC_S_INT_GEN(RG_INT_STATUS_VCORE_OC),
		PMIC_S_INT_GEN(NO_USE),
		PMIC_S_INT_GEN(NO_USE),
		PMIC_S_INT_GEN(RG_INT_STATUS_VPA_OC),
		PMIC_S_INT_GEN(RG_INT_STATUS_LDO_OC),
		PMIC_S_INT_GEN(RG_INT_STATUS_BAT2_H),
		PMIC_S_INT_GEN(RG_INT_STATUS_BAT2_L),
		PMIC_S_INT_GEN(RG_INT_STATUS_VISMPS0_H),
		PMIC_S_INT_GEN(RG_INT_STATUS_VISMPS0_L),
		PMIC_S_INT_GEN(RG_INT_STATUS_AUXADC_IMP),
		PMIC_S_INT_GEN(NO_USE),
	};

	static struct pmic_interrupt_bit interrupt_status2[] = {
		PMIC_S_INT_GEN(RG_INT_STATUS_OV),
		PMIC_S_INT_GEN(RG_INT_STATUS_BVALID_DET),
		PMIC_S_INT_GEN(RG_INT_STATUS_VBATON_HV),
		PMIC_S_INT_GEN(RG_INT_STATUS_VBATON_UNDET),
		PMIC_S_INT_GEN(RG_INT_STATUS_WATCHDOG),
		PMIC_S_INT_GEN(RG_INT_STATUS_PCHR_CM_VDEC),
		PMIC_S_INT_GEN(RG_INT_STATUS_CHRDET),
		PMIC_S_INT_GEN(RG_INT_STATUS_PCHR_CM_VINC),
		PMIC_S_INT_GEN(RG_INT_STATUS_FG_BAT_H),
		PMIC_S_INT_GEN(RG_INT_STATUS_FG_BAT_L),
		PMIC_S_INT_GEN(RG_INT_STATUS_FG_CUR_H),
		PMIC_S_INT_GEN(RG_INT_STATUS_FG_CUR_L),
		PMIC_S_INT_GEN(RG_INT_STATUS_FG_ZCV),
		PMIC_S_INT_GEN(RG_INT_STATUS_SPKL_D),
		PMIC_S_INT_GEN(RG_INT_STATUS_SPKL_AB),
		PMIC_S_INT_GEN(NO_USE),
	};

	static struct pmic_interrupts interrupts[] = {
		PMIC_M_INTS_GEN(MT6328_INT_STATUS0, MT6328_INT_CON0, MT6328_INT_CON0_SET,
				MT6328_INT_CON0_CLR, interrupt_status0),
		PMIC_M_INTS_GEN(MT6328_INT_STATUS1, MT6328_INT_CON1, MT6328_INT_CON1_SET,
				MT6328_INT_CON1_CLR, interrupt_status1),
		PMIC_M_INTS_GEN(MT6328_INT_STATUS2, MT6328_INT_CON2, MT6328_INT_CON2_SET,
				MT6328_INT_CON2_CLR, interrupt_status2),
	};

/*这整个过程可能是中断触发，执行回调函数，回调函数里面对中断的类型进行遍历，跟哪个状态对比，符合的就产生对应的配置
什么样的状态*/


14./*创建设备节点，上层向设备节点写值，是否打开开关，底层读写寄存器		*/

	//Gionee GuoJianqiu 201601026 modify for OTG SWITCH begin
	#if defined(GN_BATTERY_OTGCHARGE_SWITCH_SUPPORT)
	ret_device_file = device_create_file(&(dev->dev), &dev_attr_Otg_Charge_Switch);
	#endif
	//Gionee GuoJianqiu 201601026 modify for OTG SWITCH end

	
	//Gionee GuoJianqiu 201601026 modify for OTG SWITCH begin
	#if defined(GN_BATTERY_OTGCHARGE_SWITCH_SUPPORT)
	kal_bool gn_otg_charge_switch_State = KAL_FALSE;
	static ssize_t show_Otg_Charge_Switch(struct device *dev,struct device_attribute *attr, char *buf)
	{
	    battery_log(BAT_LOG_CRTI, "show_otg_charge_switch_State = %d\n", gn_otg_charge_switch_State);    
	    return sprintf(buf, "%u\n", gn_otg_charge_switch_State);
	}
	static ssize_t store_Otg_Charge_Switch(struct device *dev,struct device_attribute *attr, const char *buf, size_t size)
	{
		int rv;
		kal_bool otg_charge_state = gn_otg_charge_switch_State;
	
		/*rv = sscanf(buf, "%u", &otg_charge_state);*/
		rv = kstrtouint(buf, 0, &otg_charge_state);		//字符串转无符号整形
		if (rv != 0)
			{
			battery_log(BAT_LOG_CRTI, "  bad argument, echo [enable] > Otg_Charge_Switch! gn_otg_charge_switch_State = %d\n", gn_otg_charge_switch_State);
			return -EINVAL;
			}

		if(otg_charge_state)
			gn_Open_Otg_Irq();
		else
			gn_Close_Otg_Irq();

		gn_otg_charge_switch_State = otg_charge_state;
		    battery_log(BAT_LOG_CRTI, "store_otg_charge_switch_State = %d\n", gn_otg_charge_switch_State);
		    return size;
	}
	static DEVICE_ATTR(Otg_Charge_Switch, 0664, show_Otg_Charge_Switch, store_Otg_Charge_Switch);
	#endif
	//Gionee GuoJianqiu 201601026 modify for OTG SWITCH end

15./*电池电量的计算，及一些参数的变化

上面当电量充到100%后，如果使用GM2.0算法，就会重置电池的一些参数*/

/*a.更新电池老化系数*/
void fg_qmax_update_for_aging(void)
{
#if defined(CONFIG_POWER_EXT)
#else
	bool hw_charging_done = bat_is_charging_full();

#if defined(CONFIG_MTK_JEITA_STANDARD_SUPPORT)
	if (g_temp_status != TEMP_POS_10_TO_POS_45) {
		bm_print(BM_LOG_CRTI, "Skip qmax update due to not 4.2V full-charging.\n");
		return;
	}
#endif

	if (hw_charging_done == true) {	/* charging full, g_HW_Charging_Done == 1 */

		if (gFG_DOD0 > 85) {
			if (gFG_columb < 0)
				gFG_columb = gFG_columb - gFG_columb * 2;	/* absolute value */

			gFG_BATT_CAPACITY_aging =
			    (((gFG_columb * 1000) + (5 * gFG_DOD0)) / gFG_DOD0) / 10;

			/* tuning */
			gFG_BATT_CAPACITY_aging =
			    (gFG_BATT_CAPACITY_aging * 100) / p_bat_meter_data->aging_tuning_value;

			if (gFG_BATT_CAPACITY_aging == 0) {
				gFG_BATT_CAPACITY_aging =
				    fgauge_get_Q_max(battery_meter_get_battery_temperature());
				bm_print(BM_LOG_CRTI,
					 "[fg_qmax_update_for_aging] error, restore gFG_BATT_CAPACITY_aging (%d)\n",
					 gFG_BATT_CAPACITY_aging);
			}

			bm_print(BM_LOG_CRTI,
				 "[fg_qmax_update_for_aging] need update : gFG_columb=%d, gFG_DOD0=%d, new_qmax=%d\r\n",
				 gFG_columb, gFG_DOD0, gFG_BATT_CAPACITY_aging);
		} else {
			bm_print(BM_LOG_CRTI,
				 "[fg_qmax_update_for_aging] no update : gFG_columb=%d, gFG_DOD0=%d, new_qmax=%d\r\n",
				 gFG_columb, gFG_DOD0, gFG_BATT_CAPACITY_aging);
		}
	} else {
		bm_print(BM_LOG_CRTI, "[fg_qmax_update_for_aging] hw_charging_done=%d\r\n",
			 hw_charging_done);
	}
#endif
}

16./*对不同数据的取平均运算*/

static unsigned int mt_battery_average_method(BATTERY_AVG_ENUM type, unsigned int *bufferdata,
					      unsigned int data, signed int *sum,
					      unsigned char batteryIndex)
{
	unsigned int avgdata;

	mt_battery_average_method_init(type, bufferdata, data, sum);

	*sum -= bufferdata[batteryIndex];
	*sum += data;
	bufferdata[batteryIndex] = data;
	avgdata = (*sum) / BATTERY_AVERAGE_SIZE;

	battery_log(BAT_LOG_FULL, "bufferdata[%d]= (%d)\n", batteryIndex, bufferdata[batteryIndex]);
	return avgdata;
}

static void mt_battery_average_method_init(BATTERY_AVG_ENUM type, unsigned int *bufferdata,
					   unsigned int data, signed int *sum)
{
	unsigned int i;
	static kal_bool batteryBufferFirst = KAL_TRUE;
	static kal_bool previous_charger_exist = KAL_FALSE;
	static kal_bool previous_in_recharge_state = KAL_FALSE;
	static unsigned char index;

	/* reset charging current window while plug in/out { */
	if (type == BATTERY_AVG_CURRENT) {
		if (BMT_status.charger_exist == KAL_TRUE) {
			if (previous_charger_exist == KAL_FALSE) {
				batteryBufferFirst = KAL_TRUE;
				previous_charger_exist = KAL_TRUE;
				if ((BMT_status.charger_type == STANDARD_CHARGER)
#if defined(CONFIG_MTK_DUAL_INPUT_CHARGER_SUPPORT)
				    && (DISO_data.diso_state.cur_vdc_state == DISO_ONLINE)
#endif
				    )
					data = batt_cust_data.ac_charger_current / 100;
				else if (BMT_status.charger_type == CHARGING_HOST)
					data = batt_cust_data.charging_host_charger_current / 100;
				else if (BMT_status.charger_type == NONSTANDARD_CHARGER)
					data = batt_cust_data.non_std_ac_charger_current / 100;	/* mA */
				else	/* USB */
					data = batt_cust_data.usb_charger_current / 100;	/* mA */
			} else if ((previous_in_recharge_state == KAL_FALSE)
				   && (BMT_status.bat_in_recharging_state == KAL_TRUE)) {
				batteryBufferFirst = KAL_TRUE;

				if ((BMT_status.charger_type == STANDARD_CHARGER)
#if defined(CONFIG_MTK_DUAL_INPUT_CHARGER_SUPPORT)
				    && (DISO_data.diso_state.cur_vdc_state == DISO_ONLINE)
#endif
				    )
					data = batt_cust_data.ac_charger_current / 100;
				else if (BMT_status.charger_type == CHARGING_HOST)
					data = batt_cust_data.charging_host_charger_current / 100;
				else if (BMT_status.charger_type == NONSTANDARD_CHARGER)
					data = batt_cust_data.non_std_ac_charger_current / 100;	/* mA */
				else	/* USB */
					data = batt_cust_data.usb_charger_current / 100;	/* mA */
			}

			previous_in_recharge_state = BMT_status.bat_in_recharging_state;
		} else {
			if (previous_charger_exist == KAL_TRUE) {
				batteryBufferFirst = KAL_TRUE;
				previous_charger_exist = KAL_FALSE;
				data = 0;
			}
		}
	}
	/* reset charging current window while plug in/out } */

	battery_log(BAT_LOG_FULL, "batteryBufferFirst =%d, data= (%d)\n", batteryBufferFirst, data);

	if (batteryBufferFirst == KAL_TRUE) {
		for (i = 0; i < BATTERY_AVERAGE_SIZE; i++)
			bufferdata[i] = data;


		*sum = data * BATTERY_AVERAGE_SIZE;
	}

	index++;
	if (index >= BATTERY_AVERAGE_DATA_NUMBER) {
		index = BATTERY_AVERAGE_DATA_NUMBER;
		batteryBufferFirst = KAL_FALSE;
	}
}

17./*遍历双向链表，用法具有通用性，高级进阶，数据结构和算法*/
#define list_for_each_entry(pos, head, member)				\
	for (pos = list_entry((head)->next, typeof(*pos), member);	\
	     &pos->member != (head); 	\
	     pos = list_entry(pos->member.next, typeof(*pos), member))


#define list_entry(ptr, type, member) \
	container_of(ptr, type, member)


#ifndef container_of
#define container_of(ptr, type, member) ({			\
	const typeof(((type *)0)->member) * __mptr = (ptr);	\
	(type *)((char *)__mptr - offsetof(type, member)); })


这个语法跟上面相似，还是不会，这种宏函数的用法
/**
 * spmi_for_each_container_dev - iterate over the array of devnode resources.
 * @res: spmi_resource pointer used as the array cursor
 * @spmi_dev: spmi_device to iterate
 *
 * Only useable in spmi-dev-container configurations.
 */
#define spmi_for_each_container_dev(res, spmi_dev)			      \
	for (res = ((spmi_dev)->dev_node ? &(spmi_dev)->dev_node[0] : NULL);  \
	     (res - (spmi_dev)->dev_node) < (spmi_dev)->num_dev_node; res++)
	     
	     
18.条件表达式根据en使能与否使用不同的函数，参数是一样的，然后在回掉不同的函数
ret = (en ? rt5081_pmu_reg_set_bit : rt5081_pmu_reg_clr_bit)
		(chg_data->chip, RT5081_PMU_REG_CHGCTRL3, RT5081_MASK_ILIM_EN);	 
		
		
static inline int rt5081_pmu_reg_set_bit(struct rt5081_pmu_chip *chip, u8 addr,
		u8 mask)
{
	return rt5081_pmu_reg_update_bits(chip, addr, mask, mask);
}	

static inline int rt5081_pmu_reg_clr_bit(struct rt5081_pmu_chip *chip, u8 addr,
		u8 mask)
{
	return rt5081_pmu_reg_update_bits(chip, addr, mask, 0x00);
}	    


19.都去寄存器的值，mask，shift
掩码是为了读多少位，位移是为了将读出的数据方便处理
static int rt5081_get_aicr(struct charger_device *chg_dev, u32 *aicr)
{
	int ret = 0;
	u8 reg_aicr = 0;
	struct rt5081_pmu_charger_data *chg_data =
		dev_get_drvdata(&chg_dev->dev);

	ret = rt5081_pmu_reg_read(chg_data->chip, RT5081_PMU_REG_CHGCTRL3);
	if (ret < 0)
		return ret;

	reg_aicr = (ret & RT5081_MASK_AICR) >> RT5081_SHIFT_AICR;
	*aicr = rt5081_find_closest_real_value(RT5081_AICR_MIN, RT5081_AICR_MAX,
		RT5081_AICR_STEP, reg_aicr);

	return ret;
}

static u32 rt5081_find_closest_real_value(u32 min, u32 max, u32 step,
	u8 reg_val)
{
	u32 ret_val = 0;

	ret_val = min + reg_val * step;
	if (ret_val > max)
		ret_val = max;

	return ret_val;
}


20.在linux或者android下创建一个system 节点(扩展device节点)


21.快充算法
int mtk_pe20_init(struct charger_manager *pinfo)
{
	int ret = 0;

	wake_lock_init(&pinfo->pe2.suspend_lock, WAKE_LOCK_SUSPEND,
		"PE+20 TA charger suspend wakelock");
	mutex_init(&pinfo->pe2.access_lock);
	mutex_init(&pinfo->pe2.pmic_sync_lock);

	pinfo->pe2.ta_vchr_org = 5000000;
	pinfo->pe2.idx = -1;
	pinfo->pe2.vbus = 5000000;
	pinfo->pe2.to_check_chr_type = true;
	pinfo->pe2.is_enabled = true;

	pinfo->pe2.profile[0].vbat = 3400000;
	pinfo->pe2.profile[1].vbat = 3500000;
	pinfo->pe2.profile[2].vbat = 3600000;
	pinfo->pe2.profile[3].vbat = 3700000;
	pinfo->pe2.profile[4].vbat = 3800000;
	pinfo->pe2.profile[5].vbat = 3900000;
	pinfo->pe2.profile[6].vbat = 4000000;
	pinfo->pe2.profile[7].vbat = 4100000;
	pinfo->pe2.profile[8].vbat = 4200000;
	pinfo->pe2.profile[9].vbat = 4300000;

	pinfo->pe2.profile[0].vchr = 8000000;
	pinfo->pe2.profile[1].vchr = 8500000;
	pinfo->pe2.profile[2].vchr = 8500000;
	pinfo->pe2.profile[3].vchr = 9000000;
	pinfo->pe2.profile[4].vchr = 9000000;
	pinfo->pe2.profile[5].vchr = 9000000;
	pinfo->pe2.profile[6].vchr = 9500000;
	pinfo->pe2.profile[7].vchr = 9500000;
	pinfo->pe2.profile[8].vchr = 10000000;
	pinfo->pe2.profile[9].vchr = 10000000;

	ret = charger_dev_set_pe20_efficiency_table(pinfo->chg1_dev);
	if (ret != 0)
		pr_err("%s: use default table, %d\n", __func__, ret);

	return 0;
}




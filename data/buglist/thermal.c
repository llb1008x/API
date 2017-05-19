/*********************************************************************************
								thermal相关的
**********************************************************************************/

{

	首先thermal.txt文件内的相关参数要知道什么意思，怎么改更合理，有效。

	相关的几个case(12时00分12秒)ALPS02710500 ，ALPS02796764这个是温升降低充电电流的 


--->温升CPU降核，降频问题
	在如下时间点系统发生cpu占用率高，导致系统弹出cpu占有率高的对话框，请驱动同事做进一步分析！

	11-24 17:13:38.570567 <7>[25342.267555] (2)[509:AALLightSensor][name:aal_control&][ALS/AAL]Get als dat :40
	11-24 17:13:38.718625 <3>[25342.415613] (0)[11158:kworker/0:2]<STEP_COUNTER> step_c data48
	11-24 17:13:38.728131 <4>[25342.425119] (0)[11236:kworker/0:3][Power/CPU_Thermal]set_adaptive_cpu_power_limit 1621 T=63200,1734 T=62600,1884 T=61500,2010 T=61900,0 T=58700
	11-24 17:13:38.728154 <4>[25342.425142] (0)[11236:kworker/0:3][Power/cpufreq] mt_cpufreq_thermal_protect(): limited_power = 1384
	11-24 17:13:38.728170 <7>[25342.425158] (0)[11236:kworker/0:3][name:mt_cpufreq&][Power/cpufreq] @mt_cpufreq_thermal_protect: found = 1, limited_power_idx = 1, limited_max_freq = 1235000, limited_max_ncpu = 4
	11-24 17:13:38.728319 <7>[25342.425307] (0)[11236:kworker/0:3][name:mt_cpufreq&][Power/cpufreq] @_mt_cpufreq_set_locked(): Vproc = 1112mv, freq = 1235000 KHz
	11-24 17:13:38.728345 <7>[25342.425333] (0)[11236:kworker/0:3][name:mtk_cooler_atm&][Power/CPU_Thermal]set_adaptive_gpu_power_limit 600
	11-24 17:13:38.770063 <7>[25342.467051] (0)[11236:kworker/0:3][name:mtk_ts_bts&][Power/BTS_Thermal] T_AP=51000
	11-24 17:13:43.660714 <7>[25347.357702] (0)[507:AALServiceMain][name:ddp_pwm&][PWM] (latest= 5): 145( 823, 575) 144( 823, 592) 143( 823, 609) 141( 823, 643) 140( 823, 660)
	11-24 17:13:43.728026 <4>[25347.425014] (0)[11158:kworker/0:2][Power/cpufreq] mt_cpufreq_thermal_protect(): limited_power = 1420
	11-24 17:13:43.728049 <7>[25347.425037] (0)[11158:kworker/0:2][name:mt_cpufreq&][Power/cpufreq] @mt_cpufreq_thermal_protect: found = 1, limited_power_idx = 1, limited_max_freq = 1235000, 


	温度升高导致系统启动温度预警，限制CPU频率，核的数量，最终导致CPU负载过大，系统卡顿

	set_adaptive_cpu_power_limit（）

	adaptive_limit：
	1621 T=63200,
	1734 T=62600,
	1884 T=61500,
	2010 T=61900,
	0    T=58700

	mt_cpufreq_thermal_protect()
	limited_power = 1384

	mt_cpufreq_thermal_protect（）
	found = 1,
	limited_power_idx = 1,
	limited_max_freq = 1235000,

	_mt_cpufreq_set_locked()
	Vproc = 1112mv, 
	freq = 1235000 KHz

	set_adaptive_gpu_power_limit（）
	600


	T_AP=51000

	[PWM] (latest= 5): 145( 823, 575) 144( 823, 592) 143( 823, 609) 141( 823, 643) 140( 823, 660)

	mt_cpufreq_thermal_protect()
	limited_power = 1420

	mt_cpufreq_thermal_protect（）
	found = 1, 
	limited_power_idx = 1,
	limited_max_freq = 1235000, 


	#if THERMAL_HEADROOM
	static int p_Tpcb_correlation;
	static int Tpcb_trip_point;
	static int thp_max_cpu_power;
	static int thp_p_tj_correlation;
	static int thp_threshold_tj;
	#endif


	static struct mt_cpu_dvfs cpu_dvfs[] = {
		[MT_CPU_DVFS_LITTLE] = {
			.name = __stringify(MT_CPU_DVFS_LITTLE),
			.cpu_id = MT_CPU_DVFS_LITTLE,	/* TODO: FIXME */
			.cpu_level = CPU_LEVEL_1,	/* FY segment */
			.ops = &dvfs_ops,
	#ifdef CONFIG_CPU_DVFS_TURBO_MODE
			.turbo_mode = 0,
	#endif
	#ifdef CONFIG_CPU_DVFS_POWER_THROTTLING
			.idx_opp_tbl_for_pwr_thro = -1,
			.idx_pwr_thro_max_opp = 0,
	#endif
		},
	};


	#elif defined(CONFIG_ARCH_MT6735M)
	/* CPU LEVEL 0, 1.2GHz segment */
	static struct mt_cpu_freq_info opp_tbl_e1_0[] = {
		OP(CPU_DVFS_FREQ0, 125000),
		OP(CPU_DVFS_FREQ1, 121875),
		OP(CPU_DVFS_FREQ4, 118750),
		OP(CPU_DVFS_FREQ6, 115000),
		OP(CPU_DVFS_FREQ7, 110000),
		OP(CPU_DVFS_FREQ8, 105000),
		OP(CPU_DVFS_FREQ9, 100000),
		OP(CPU_DVFS_FREQ10, 95000),
	};

	/* CPU LEVEL 1, 1GHz segment */
	static struct mt_cpu_freq_info opp_tbl_e1_1[] = {
		OP(CPU_DVFS_FREQ2, 125000),
		OP(CPU_DVFS_FREQ3, 121875),
		OP(CPU_DVFS_FREQ5, 118750),
		OP(CPU_DVFS_FREQ6, 115000),
		OP(CPU_DVFS_FREQ7, 110000),
		OP(CPU_DVFS_FREQ8, 105000),
		OP(CPU_DVFS_FREQ9, 100000),
		OP(CPU_DVFS_FREQ10, 95000),
	};

	/* CPU LEVEL 2, 1.25GHz segment */
	static struct mt_cpu_freq_info opp_tbl_e1_2[] = {
		OP(CPU_DVFS_FREQ0_1,125000),
		OP(CPU_DVFS_FREQ1,  121875),
		OP(CPU_DVFS_FREQ5,  118750),
		OP(CPU_DVFS_FREQ6,  115000),
		OP(CPU_DVFS_FREQ7,  110000),
		OP(CPU_DVFS_FREQ8,  105000),
		OP(CPU_DVFS_FREQ9,  100000),
		OP(CPU_DVFS_FREQ10, 95000),
	};

	/* CPU LEVEL 3, 1.1GHz segment */
	static struct mt_cpu_freq_info opp_tbl_e1_3[] = {
		OP(CPU_DVFS_FREQ1_1,125000),
		OP(CPU_DVFS_FREQ2,  121875),
		OP(CPU_DVFS_FREQ5,  118750),
		OP(CPU_DVFS_FREQ6,  115000),
		OP(CPU_DVFS_FREQ7,  110000),
		OP(CPU_DVFS_FREQ8,  105000),
		OP(CPU_DVFS_FREQ9,  100000),
		OP(CPU_DVFS_FREQ10, 95000),
	};

	static struct opp_tbl_info opp_tbls[] = {
		[CPU_LEVEL_0] = {opp_tbl_e1_0, ARRAY_SIZE(opp_tbl_e1_0)},
		[CPU_LEVEL_1] = {opp_tbl_e1_1, ARRAY_SIZE(opp_tbl_e1_1)},
		[CPU_LEVEL_2] = {opp_tbl_e1_2, ARRAY_SIZE(opp_tbl_e1_2)},
		[CPU_LEVEL_3] = {opp_tbl_e1_3, ARRAY_SIZE(opp_tbl_e1_3)},
	};




	mtktscpu  (IC junction temperature)

	ATM (adaptive thermal management)

	overshooting 过辐射

	opp ：operating performance point 操作性能点

	

	收敛

	/proc/driver/thermal/clatm_setting
	0  2010 15 30 1 720 2010 600 1050 

	0  	id
	2010    firststep
	15	theta(ja)fall
	30	theta(js)rise	 （fall/rise 比值越小变化越快）	
	1	minbudgetchange （每次最少要变化的大小）
	720	minCPUpower(mW)
	2010	maxCPUpower(mW)
	600	minGPUpower(mW)
	1050	maxGPUpower(mW)


	/proc/driver/thermal/clctm
	1 85000 66000 38999 41999 75000 60000 331980 6333 269995 5000   

	1 	ctm on
	85000	Target Tj0
	66000	Target Tj2
	38999	Tpcb1
	41999	Tpcb2
	75000	exit Tj0
	60000	exit Tj2
	331980	enter_a
	6333	enter_b
	269995	exit_a
	5000	exit_b


	thermal温度高想降频，但降频会导致系统卡顿，无响应，温升和降频是一对互斥的

	温度容易升高，同时系统容易卡顿......

	整理一下逻辑：温度升高，系统启动mt_cpufreq_thermal_protect，降压，降频，限制CPU核，导致系统无响应，降频过快还是跨度过大，还是CPU核减的太少


	Cooler Name    ctm    ENABLE
	Extra    ctm on    1
	Extra    Target Tj 0    85000
	Extra    Target Tj 2    66000
	Extra    Tpcb 1    38999
	Extra    Tpcb 2    41999
	Extra    Exit Tj 0    75000
	Extra    Exit Tj 2    60000
	Extra    Enter_a    331980
	Extra    Enter_b    6333
	Extra    Exit_a    269995
	Extra    Exit_b    5000

	算法：
	Tj = min(85, max(66, (Enter_a – Enter_b*bts_cur_temp/1000)/1000))

	Exit tj = min(75, max(60, (Exit_a – Exit_b*bts_cur_temp/1000)/1000))


	//通过这个函数获取CPU动态调节的策略
	p = id_to_cpu_dvfs(_get_cpu_dvfs_id(policy->cpu));

	[MT_CPU_DVFS_LITTLE/0]
	[0] = { .cpufreq_khz = 1300000,	.cpufreq_ncpu = 4,	.cpufreq_power = 1379, },
	[1] = { .cpufreq_khz = 1235000,	.cpufreq_ncpu = 4,	.cpufreq_power = 1271, },
	[2] = { .cpufreq_khz = 1170000,	.cpufreq_ncpu = 4,	.cpufreq_power = 1157, },
	[3] = { .cpufreq_khz = 1300000,	.cpufreq_ncpu = 3,	.cpufreq_power = 1034, },
	[4] = { .cpufreq_khz = 1235000,	.cpufreq_ncpu = 3,	.cpufreq_power = 953, },
	[5] = { .cpufreq_khz = 1040000,	.cpufreq_ncpu = 4,	.cpufreq_power = 935, },
	[6] = { .cpufreq_khz = 1170000,	.cpufreq_ncpu = 3,	.cpufreq_power = 867, },
	[7] = { .cpufreq_khz = 1040000,	.cpufreq_ncpu = 3,	.cpufreq_power = 701, },
	[8] = { .cpufreq_khz = 1300000,	.cpufreq_ncpu = 2,	.cpufreq_power = 689, },
	[9] = { .cpufreq_khz = 819000,	.cpufreq_ncpu = 4,	.cpufreq_power = 685, },
	[10] = { .cpufreq_khz = 1235000,	.cpufreq_ncpu = 2,	.cpufreq_power = 635, },
	[11] = { .cpufreq_khz = 1170000,	.cpufreq_ncpu = 2,	.cpufreq_power = 578, },
	[12] = { .cpufreq_khz = 819000,	.cpufreq_ncpu = 3,	.cpufreq_power = 513, },
	[13] = { .cpufreq_khz = 598000,	.cpufreq_ncpu = 4,	.cpufreq_power = 468, },
	[14] = { .cpufreq_khz = 1040000,	.cpufreq_ncpu = 2,	.cpufreq_power = 467, },
	[15] = { .cpufreq_khz = 598000,	.cpufreq_ncpu = 3,	.cpufreq_power = 351, },
	[16] = { .cpufreq_khz = 1300000,	.cpufreq_ncpu = 1,	.cpufreq_power = 344, },
	[17] = { .cpufreq_khz = 819000,	.cpufreq_ncpu = 2,	.cpufreq_power = 342, },
	[18] = { .cpufreq_khz = 442000,	.cpufreq_ncpu = 4,	.cpufreq_power = 324, },
	[19] = { .cpufreq_khz = 1235000,	.cpufreq_ncpu = 1,	.cpufreq_power = 317, },
	[20] = { .cpufreq_khz = 1170000,	.cpufreq_ncpu = 1,	.cpufreq_power = 289, },
	[21] = { .cpufreq_khz = 442000,	.cpufreq_ncpu = 3,	.cpufreq_power = 243, },
	[22] = { .cpufreq_khz = 598000,	.cpufreq_ncpu = 2,	.cpufreq_power = 234, },
	[23] = { .cpufreq_khz = 1040000,	.cpufreq_ncpu = 1,	.cpufreq_power = 233, },
	[24] = { .cpufreq_khz = 299000,	.cpufreq_ncpu = 4,	.cpufreq_power = 209, },
	[25] = { .cpufreq_khz = 819000,	.cpufreq_ncpu = 1,	.cpufreq_power = 171, },
	[26] = { .cpufreq_khz = 442000,	.cpufreq_ncpu = 2,	.cpufreq_power = 162, },
	[27] = { .cpufreq_khz = 299000,	.cpufreq_ncpu = 3,	.cpufreq_power = 156, },
	[28] = { .cpufreq_khz = 598000,	.cpufreq_ncpu = 1,	.cpufreq_power = 117, },
	[29] = { .cpufreq_khz = 299000,	.cpufreq_ncpu = 2,	.cpufreq_power = 104, },
	[30] = { .cpufreq_khz = 442000,	.cpufreq_ncpu = 1,	.cpufreq_power = 81, },
	[31] = { .cpufreq_khz = 299000,	.cpufreq_ncpu = 1,	.cpufreq_power = 52, },


	mt_gpufreqs_power[0].gpufreq_khz = 448500
	mt_gpufreqs_power[0].gpufreq_volt = 115000
	mt_gpufreqs_power[0].gpufreq_power = 753

	mt_gpufreqs_power[1].gpufreq_khz = 279500
	mt_gpufreqs_power[1].gpufreq_volt = 115000
	mt_gpufreqs_power[1].gpufreq_power = 487


	mt_gpufreqs_power[2].gpufreq_khz = 279500
	mt_gpufreqs_power[2].gpufreq_volt = 105000
	mt_gpufreqs_power[2].gpufreq_power = 396



--->温升导致的充电电流降低
	Dear MTK，
	之前在6753平台 ，mtk_cooler_bcct.c 会根据 thermal.conf 里 配置 调用 mtk_cl_bcct_set_bcct_limit来限制充电电流；现在6755平台这个功能 没有起作用，请问是不
	在支持了吗？？ 还是某个开关控制没有打开呢？
	我现在 thermal.conf 里设置如下：
	/proc/driver/thermal/clbcct
	0 1000 900 750
	/proc/driver/thermal/tzbts
	6 100000 0 mtktspa-sysrst 90000 0 mtk-cl-shutdown00 57000 0 mtk-cl-cam00 50000 0 mtk-cl-bcct02 45000 0 mtk-cl-bcct01 40000 0 mtk-cl-bcct00
	实际温度已经56度了，T_AP=56000，仍旧没有调用 mtk_cl_bcct_set_bcct_limit；
	请给予解答，ThankS！

	我现在 thermal.conf 里设置如下：
	/proc/driver/thermal/clbcct
	0 1000 900 750
	/proc/driver/thermal/tzbts
	6 100000 0 mtktspa-sysrst 90000 0 mtk-cl-shutdown00 57000 0 mtk-cl-cam00 50000 0 mtk-cl-bcct02 45000 0 mtk-cl-bcct01 40000 0 mtk-cl-bcct00


	这个应该可以生效，可以在mtk_cl_bcct_set_bcct_limit 加点log看看是否可以触发？
	内部机器配置基本是这样：
	/proc/driver/thermal/clbcct
	0 2000 1000 500
	/proc/driver/thermal/tzbts
	6 100000 0 mtktspa-sysrst 90000 0 mtk-cl-shutdown00 57000 0 mtk-cl-cam00 50000 0 mtk-cl-bcct02 49000 0 mtk-cl-bcct01 48000 0 mtk-cl-bcct00 0 0 no-cooler 0 0 no-cooler 0 0 no-cooler 0 0 no-cooler 1000

	mtk_cl_bcct_set_bcct_limit里默认就有log啊，没有出来；并且肯定没有调到set_bat_charging_current_limit，这个我里面是加了log的；我们关心的就是为何没有调到
	set_bat_charging_current_limit，调整电流；
	thermal.conf    4.18 KB     和 thermal.eng.conf    4.18 KB
	APLog_2015_0101_081542.rar    62.39 MB     抓取的mobile log，请参考分析；

	为了减小内核的log量，
	thermal management default将kernel部分的debug log都关闭了。
	只留下了一些，error和warning log。

	而有些问题的分析和thermal policy的tuning是需要完整的thermal log的。
	特别是各个thermal zone device的温度值。

	为此可以使用Elephant Stress工具来抓取完整的thermal log。
	(user load也可以使用)
	Elephant stress tool可以在敝司DDC网站上下载。

	或者修改一下mtk_cl_bcct_set_bcct_limit

	/kernel-3.10/drivers/misc/mediatek/thermal/mt6755/common/coolers/mtk_cooler_bcct.c
	下的log是否有开
	9#define mtk_cooler_bcct_dprintk(fmt, args...) \
	20do { \
	21    if (1 == cl_bcct_klog_on) \
	22        pr_debug("[thermal/cooler/bcct]" fmt, ##args); \
	23} while (0)

	初始化或者其它code流程 都没有bcct 部分的log


	dear customer
	echo 1 1000 900 750 > /proc/driver/thermal/clbcct

	也可以打开log


	Dear MTK，
	root@GiONEE_BFL7512B:/ # echo 1 1000 900 750 > /proc/driver/thermal/clbcct
	root@GiONEE_BFL7512B:/proc/driver/thermal # cat clbcct
	klog 1
	curr_limit 65535
	mtk-cl-bcct00 1000 mA, state 0
	mtk-cl-bcct01 900 mA, state 0
	mtk-cl-bcct02 750 mA, state 0
		
	mobilelog-7512-gaowenlog.rar    73.05 MB     新抓取的 mobile log，请参考分析；


	<7>[ 345.529336].(0)[3790:sh][name:mtk_cooler_bcct&][thermal/cooler/bcct]_cl_bcct_read invoked.
	<7>[ 362.634991].(0)[3790:sh][name:mtk_cooler_bcct&][thermal/cooler/bcct]_cl_bcct_read invoked.
	这两句log 可以看出，log是打开了的；
	并且6755平台其他项目 也有反馈mtk_cl_bcct_set_bcct_limit 没有其作用；


	dear customer
	有开启MTK_PUMP_EXPRESS_PLUS_SUPPORT 吗?

	在检查device\mediatek\<.your projec name>\ProjectConfig.mk

	在device\mediatek\mt6755\device.mk 有区分



	修改一下：
	device\mediatek\mt6755\device.mk

	ifeq ($(strip $(MTK_PUMP_EXPRESS_PLUS_SUPPORT)),yes)
	ifeq ($(TARGET_BUILD_VARIANT),eng)
	PRODUCT_COPY_FILES += $(LOCAL_PATH)/thermal.eng.conf:system/etc/.tp/thermal.conf
	else
	PRODUCT_COPY_FILES += $(LOCAL_PATH)/thermal.conf:system/etc/.tp/thermal.conf
	endif
	PRODUCT_COPY_FILES += $(LOCAL_PATH)/thermal.wfd.6755.conf:system/etc/.tp/.thermal_policy_00
	PRODUCT_COPY_FILES += $(LOCAL_PATH)/thermal.pip.6755.conf:system/etc/.tp/.thermal_policy_01
	PRODUCT_COPY_FILES += $(LOCAL_PATH)/thermal.fdvrgis.6755.conf:system/etc/.tp/.thermal_policy_02
	PRODUCT_COPY_FILES += $(LOCAL_PATH)/thermal.meta.6755.conf:system/etc/.tp/.thermal_meta.conf
	else
	ifeq ($(TARGET_BUILD_VARIANT),eng)
	PRODUCT_COPY_FILES += $(LOCAL_PATH)/thermal.eng_noBCCT.conf:system/etc/.tp/thermal.conf
	else
	PRODUCT_COPY_FILES += $(LOCAL_PATH)/thermal_noBCCT.conf:system/etc/.tp/thermal.conf
	endif
	PRODUCT_COPY_FILES += $(LOCAL_PATH)/thermal.wfd.6755_noBCCT.conf:system/etc/.tp/.thermal_policy_00
	PRODUCT_COPY_FILES += $(LOCAL_PATH)/thermal.pip.6755_noBCCT.conf:system/etc/.tp/.thermal_policy_01
	PRODUCT_COPY_FILES += $(LOCAL_PATH)/thermal.fdvrgis.6755_noBCCT.conf:system/etc/.tp/.thermal_policy_02
	PRODUCT_COPY_FILES += $(LOCAL_PATH)/thermal.meta.6755_noBCCT.conf:system/etc/.tp/.thermal_meta.conf
	endif

	或者定义MTK 的快充MTK_PUMP_EXPRESS_PLUS_SUPPORT
	建议是修改device.mk


	


}

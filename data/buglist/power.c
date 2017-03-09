/***********************************************************************************

								功耗相关的问题
***********************************************************************************/



？？待解决的问题
{
   底电流

   AP有没有睡下去

   各模块耗电隔离分析

   gpio配置，未使用时的suspend
   
   deepidle是什么
}









对待这类问题要有个完整的判断逻辑：
{
	正常待机问题（分场景），待机电流问题（底电流，开数据的电流3G，4G），电池续航问题(一系列场景下手机使用时间)
	
	
	处理待机问题，需要提供mobile log and netlog(如果有打开数据连接或者WIFI), MTK 处理一般是使用PowerMonitor.
	测试前
	adb shell dumpsys batterystats --reset
	adb shell dumpsys batterystats --enable full-wake-history
	adb shell cat /sys/kernel/debug/wakeup_sources > wakeup_sources_1.log
	测试后：
	adb bugreport > bugreport.txt
	adb shell cat /sys/kernel/debug/wakeup_sources > wakeup_sources_2.log
	adb shell ps -t > ps.txt

}




基本的概念：	
{
	1.Cell standby：手机搜索信号的稳定


	2.volte：
	VoLTE是基于IMS的语音业务。IMS由于支持多种接入和丰富的多媒体业务，成为全IP时代的核心网标准架构。
	经历了过去几年的发展成熟后，如今IMS已经跨越裂谷，成为固定话音领域VoBB、PSTN网改的主流选择，而且也被3GPP、GSMA确定为移动语音的标准架构
	。VoLTE即Voice over LTE，它是一种IP数据传输技术，无需2G/3G网，全部业务承载于4G网络上，可实现数据与语音业务在同一网络下的统一。
	换言之，4G网络下不仅仅提供高速率的数据业务，同时还提供高质量的音视频通话，后者便需要VoLTE技术来实现。


	3.tty：
	在linux中，tty设备用来抽象串口类型的设备，它位于字符驱动之下，抽象了串口设备需要的特性、功能，抽象后的一个tty设备即可表示一个串行输入、
	输出接口（比如控制台口，串口、pty设备接口）。

    tty core：它以统一一致的方式来处理流向某个tty设备的数据以及来自某个tty设备的数据，并向用户空间提供了统一一致的用户接口，向底层即真实的
	设备驱动提供了统一一致的编程接口。利用这些特性，可以很容易为一个新的串口设备编写驱动程序以及用户程序。
    line discipline：它是线路规程的意思。正如它的名字一样，它表示的是这条终端”线程”的输入与输出规范设置。主要用来进行输入/输出数据的预处理，
	写入设备的数据要先经过它的处理才会被发送给真实的设备驱动，从设备接收的数据也会先经过它的处理才会进入到tty core的处理逻辑。


	4.modem 调制解调器
	
	BTS：	基站收发台
	
	CLDMA_WDT (20)是代表有数据连接发送到手机端.

	CONN2AP是wifi的唤醒.

	EINT212就是跟C2K的唤醒

}	





功耗要注意的几个地方
{
	a.首先明确是否有modem log，c2kmdlog文件，modem log 特别耗电，而且占空间然后察看modem log的时间变化。
	c2kmdlog文件应该是记录传输的数据，recycle_config，modem log会经常唤醒系统，导致系统没有睡下去，所以耗电量肯定高，
	开始那几个谁开的modem log耗电四五十的。

	modem常用的几个信道，哪些跟耗电密切相关的？
	{
		channel 10是AT交互，AT command需要结合radio.log,一般都是网络变化
		
		channel 14是读写nvram,需要结合modem.log
		
		channel 20代表网络连接需要结合main.log
		
		channel 32modem读取AP属性如sim卡gpio的状态
		
		channel 34是与WCN通信，告知wifi当前modem所使用频段。
		
		channel 6&42是modem的专用通道，打开modem.log就会有此类唤醒
		
		
		
		
		55是跟volte相关
		
		基本上，上述3个channel的跟modem的网络唤醒有关。

		wakeup source:(2/42)
		42是modem log开关
		20是流量

	}
	
	常见的几个唤醒源：pwrkey?
	EINT：这部分包括pmic的唤醒(pwrkey/charger/RTC,alarm/wifi EINT/C2K EINT)
	conn2Ap:,wcn chip的唤醒，包括BT/wifi/GPS等
	CLMDA_MD/CCIFO_MD,modem同AP的通道
	
	搜索PM: suspend查找唤醒时间
	




	b.抓取battersystats.log和wakeup_sources.log，以及导出mobile log；
	这两个文件对于功耗的分析很重要，系统个部分耗电多少，持有锁，唤醒...
	抓取方法：
	adb shell dumpsys batterystats > battersystats.log
	adb shell cat /sys/kernel/debug/wakeup_sources > wakeup_sources.log




	c.比较关键的两个log文件batterystats.log和kernel.log
		batterystats.log记录系统耗电的过程
		
		1.signal level 当前环境的信号质量
		信号质量差，modem一直在找信号，耗电量肯定大，所以网络环境变化在一定程度上也会造成耗电。

		2.wake lock 应用是否长时间占用锁
		wake up alarm, wake up by 
		以上两个都有一个限度，什么样才属于过度，什么样在合理范围内?

		3.系统是否被经常被应用唤醒
		在sys.log目录下，搜索alarm 
		应用推送业务，频繁唤醒。

		kernel部分的统计思路是：
		1. 搜索"wake up by"关键字，只看kworker或是system_server进程打印的
		2. 如果是EINT event的话，要接着搜索"is pending"关键字，找到是哪个EINT
		EINT是外部中断，GPT是什么？
		3. 如果CLDMA event的话，要接着搜索"wakeup source"关键字，找到是哪个channel			
		4. 计算时间的话，从wakeup event后面的第一个PM: suspend exit到后面第一个PM: suspend entry之间的时间差

		sys log部分的统计思路是：
		1. 搜索关键字"wakeup alarm"
		2. 只查找type为0和type为2的alarm
		3. 过滤package name做统计

	
	
	
	d.常见的待机电流:
	极致省电模式电流：5mA以上
	飞行模式待机电流：3——4mA
	未开数据待机电流：5——6mA
	开数据:10mA< x < 20mA
	开wifi待机电流：<20mA
	正常的待机电流小于20mA，所以超过20mA有异常
	这些只是一部分的待机电流

	待机功耗，待机电流就你们问题多





	e.功耗测试的标准：（但是感觉有些有问题）
	1.移动运营商的电流为9ma，电信运营商的电流为9mA，联通运营商的电流为16mA
	2.计算公式：电流（ma）×时间（H）=Mah
	  如联通卡待机12小时耗电量为：（16×12）÷6020=3%
	3.如果手机插双卡，就在单卡的基础上电流加2ma；
	  如果手机开wifi，就在待机基础上加5ma
	4.如果双卡中插了联通卡，就优先联通算法，在单卡的基础上加2mA
	5.如果手机开启飞行模式进行待机，直接以5ma的电流进行计算
	  如待机12小时的耗电量为：（5×12）÷6020≈1%
	6.耗电量算法根据实际项目要求为主，这里仅供参考。

	测试前注意事项：
	1.高通平台的手机，测试充电，耗电相关项目时，LOG只开前两项，一定不能开启QXDM LOG；
	2.MTK平台的手机，不能开启MODEM LOG,否则很耗电，不方便数据的统计，只开启MobileLog中的 Android Log、Kernel Log和第三项NetworkLog，开log后需要重启手机。
	3.次日早上来正常开启所有的LOG,包括QXDM LOG也要开启，然后重启手机，若是MTK平台的手机则将MODEM LOG重启开启，否则通话类的问题LOG无效；
	4.每次重启手机都要进入开发选项中确认USB调试是否开启；
	5.测试过程中要注意自己的LOG开启是否正确，高通平台的logs文件夹是5个,MTK平台的LOG开启时要注意当前LOG主页面的三项是否都是正常开启；
	6.耗电测试时，测试前若是清除过LOG，需要重启手机；
	7.测试耗电相关时，如果中途有换电池和升级新版本，需要充满一次电后再验证；
	8.验证第二三四六七晚的电量问题，都需截图，且第二天早来不能急着清理log，需得到组长确认后再清理log；

	
	待机电流问题：
	1.飞行模式下的底电流是否正常。
	2.屏幕熄灭，系统不会立刻睡下去，要等待一定的时间系统才能睡下去，同时测试的时间也不能太短
	3.通过log察看唤醒源是谁，谁唤醒的，同时要关闭应用推送业务，避免推送经常唤醒系统。
	4.modem搜信号会唤醒系统
	环境变化影响，一般的MODEM 待机测试，需要关闭应用推送业务，蓝牙／WIFI／GPS，最好关闭数据链接，
    因为应用推送业务瞬间一段时间会比较大，等待10-15 分钟稳定，测试20分钟左右（这个时间太长了，不太准确）




	f:查找唤醒的软件
	1.wake up byEINT,是什么pending的
	2.然后在sys.log上搜索最近的alarm点，察看唤醒源是什么
	
	APK后台发送数据的Posix_connect Debug
	CLMDA_MD是modem跟AP沟通桥梁
	
	
	
	



}










出现的相关BUG
{
	GNSPR#55512：
	现象：卡1插卡电信，开启数据业务连接WIFI-AP，后台运行微信、QQ、浏览器; 待机15小时50分耗电11%

	经常被唤醒，未能睡眠下去wakeup source CLDMA
	  Start clock time: 2016-11-15-17-56-42
	  Screen on: 8m 3s 745ms (0.8%) 8x, Interactive: 8m 2s 529ms (0.8%)
	  Screen brightnesses:
	    dark 56s 557ms (11.7%)
	    bright 7m 7s 188ms (88.3%)
	  Connectivity changes: 3
	  Total full wakelock time: 1m 47s 277ms
	  Total partial wakelock time: 13m 13s 304ms
	  Mobile total received: 0B, sent: 0B (packets received 0, sent 0)
	  Phone signal levels:
	    none 18s 432ms (0.0%) 12x
	    great 33m 38s 7ms (3.5%) 27x
	  Signal scanning time: 18s 242ms
	  Radio types:
	    none 16h 9m 47s 328ms (100.0%) 1x
	  Mobile radio active time: 0ms (0.0%) 0x
	  Wi-Fi total received: 111.50MB, sent: 3.86MB (packets received 82222, sent 61884)
	  Wifi on: 5h 48m 8s 330ms (35.9%), Wifi running: 16h 9m 50s 595ms (100.0%)
	  Wifi states: (no activity)
	  Wifi supplicant states:
	    disconn 5ms (0.0%) 1x
	    scanning 4s 447ms (0.0%) 2x
	    associating 43ms (0.0%) 2x
	    associated 9ms (0.0%) 2x
	    4-way-handshake 320ms (0.0%) 2x
	    group-handshake 56ms (0.0%) 2x
	    completed 16h 9m 43s 681ms (100.0%) 2x
	  Wifi signal levels:
	    level(3) 12s 133ms (0.0%) 2x
	    level(4) 16h 9m 33s 984ms (100.0%) 3x
	  WiFi Idle time: 0ms (--%)
	  WiFi Rx time:   0ms (--%)
	  WiFi Tx time:   0ms (--%)
	  WiFi Power drain: 0mAh
	  Bluetooth Idle time: 0ms (--%)
	  Bluetooth Rx time:   0ms (--%)
	  Bluetooth Tx time:   0ms (--%)
	  Bluetooth Power drain: 0mAh


GNSPR#55283

	长时间占用锁
	  All kernel wake locks:
	  Kernel Wake lock ttyC0       : 1h 48m 0s 478ms (3279 times) realtime
	  Kernel Wake lock ccci_imsc   : 1h 3m 45s 858ms (3823 times) realtime
	  Kernel Wake lock PowerManagerService.WakeLocks: 57m 53s 239ms (11366 times) realtime
	  Kernel Wake lock ccci_fs     : 41m 52s 851ms (1342 times) realtime
	  Kernel Wake lock asc_rx_RxHdSdio: 30m 51s 64ms (1996 times) realtime
	  Kernel Wake lock cbp_rst     : 27m 22s 385ms (231 times) realtime
	  Kernel Wake lock ccci_monitor: 24m 0s 303ms (1041 times) realtime
	  Kernel Wake lock EINT wakelock: 21m 44s 679ms (1698 times) realtime
	  Kernel Wake lock asc_tx_TxHdCbp: 15m 47s 623ms (1661 times) realtime
	  Kernel Wake lock ccci_rpc_k  : 14m 11s 483ms (463 times) realtime


	/******/
	/* Wakeup Source */
	enum SPM_WAKE_SRC_LIST	{
		WAKE_SRC_SPM_MERGE = (1U << 0),  /* PCM timer, TWAM or CPU */
		WAKE_SRC_LTE_PTP = (1U << 1),
		WAKE_SRC_KP = (1U << 2),
		WAKE_SRC_WDT = (1U << 3),
		WAKE_SRC_GPT = (1U << 4),
		WAKE_SRC_EINT = (1U << 5),
		WAKE_SRC_CONN_WDT = (1U << 6),
		WAKE_SRC_CCIF0_MD = (1U << 7),
		WAKE_SRC_CCIF1_MD = (1U << 8),
		WAKE_SRC_LOW_BAT = (1U << 9),
		WAKE_SRC_CONN2AP = (1U << 10),
		WAKE_SRC_F26M_WAKE = (1U << 11),
		WAKE_SRC_F26M_SLEEP = (1U << 12),
		WAKE_SRC_PCM_WDT = (1U << 13),
		WAKE_SRC_USB_CD = (1U << 14),
		WAKE_SRC_USB_PDN = (1U << 15),
		WAKE_SRC_LTE_WAKE = (1U << 16),
		WAKE_SRC_LTE_SLEEP = (1U << 17),
		WAKE_SRC_SEJ = (1U << 18),
		WAKE_SRC_UART0 = (1U << 19),
		WAKE_SRC_AFE = (1U << 20),
		WAKE_SRC_THERM = (1U << 21),
		WAKE_SRC_CIRQ = (1U << 22),
		WAKE_SRC_MD1_VRF18_WAKE = (1U << 23),
		WAKE_SRC_SYSPWREQ = (1U << 24),
		WAKE_SRC_MD_WDT = (1U << 25),
		WAKE_SRC_C2K_WDT = (1U << 26),
		WAKE_SRC_CLDMA_MD = (1U << 27),
		WAKE_SRC_MD1_VRF18_SLEEP = (1U << 28),
		WAKE_SRC_CPU_IRQ = (1U << 29),
		WAKE_SRC_APSRC_WAKE = (1U << 30),
		WAKE_SRC_APSRC_SLEEP = (1U << 31)




eservice：
从我这边分析的结果看：没看到网路异常. 就是QQ/WEchat 定时起来跟网路做.

从alarm这部分的唤醒看，都是QQ 跟微信 5min起来一次跟服务器联网.
11-24 20:00:25.009802 1532 1746 D AlarmManager: wakeup alarm = Alarm{3bb42d2 type 0 when 1479988824257 com.tencent.mobileqq}; package = com.tencent.mobileqqneedGrouping = false
11-24 20:00:25.012924 1532 1746 D AlarmManager: wakeup alarm = Alarm{5839fa3 type 2 when 22724930 com.tencent.mm}; package = com.tencent.mmneedGrouping = false
11-24 20:00:25.016047 1532 1746 D AlarmManager: wakeup alarm = Alarm{69e59 type 0 when 1479988824940 com.gionee.cloud.gpe}; package = com.gionee.cloud.gpeneedGrouping = false
11-24 20:09:10.015841 1532 1746 D AlarmManager: wakeup alarm = Alarm{869acfc type 2 when 23056399 com.android.phone}; package = com.android.phoneneedGrouping = true
11-24 20:10:24.025028 1532 1746 D AlarmManager: wakeup alarm = Alarm{2de3d39 type 0 when 1479989424024 com.tencent.mobileqq}; package = com.tencent.mobileqqneedGrouping = true
11-24 20:15:10.028418 1532 1746 D AlarmManager: wakeup alarm = Alarm{b1307fb type 2 when 23324539 com.tencent.mm}; package = com.tencent.mmneedGrouping = true
11-24 20:15:10.030567 1532 1746 D AlarmManager: wakeup alarm = Alarm{aab1018 type 0 when 1479989424178 com.gionee.cloud.gpe}; package = com.gionee.cloud.gpeneedGrouping = true
11-24 20:15:10.035478 1532 1746 D AlarmManager: wakeup alarm = Alarm{ae42056 type 2 when 23325469 com.tencent.mm}; package = com.tencent.mmneedGrouping = true
11-24 20:15:10.039940 1532 1746 D AlarmManager: wakeup alarm = Alarm{94d15c4 type 2 when 23325472 com.tencent.mm}; package = com.tencent.mmneedGrouping = true
11-24 20:15:10.044497 1532 1746 D AlarmManager: wakeup alarm = Alarm{7347b73 type 2 when 23610512 com.android.phone}; package = com.android.phoneneedGrouping = true
11-24 20:20:24.093182 1532 1746 D AlarmManager: wakeup alarm = Alarm{da6d6bf type 2 when 23630547 com.tencent.mm}; package = com.tencent.mmneedGrouping = true
11-24 20:20:24.095583 1532 1746 D AlarmManager: wakeup alarm = Alarm{c31d18c type 0 when 1479990024066 com.gionee.cloud.gpe}; package = com.gionee.cloud.gpeneedGrouping = true
11-24 20:20:24.099179 1532 1746 D AlarmManager: wakeup alarm = Alarm{d5710d5 type 2 when 23924577 com.tencent.mm}; package = com.tencent.mmneedGrouping = true
11-24 20:21:48.014131 1532 1746 D AlarmManager: wakeup alarm = Alarm{194ce6d type 2 when 23925104 com.tencent.mm}; package = com.tencent.mmneedGrouping = true


Alarm 起来的时候，会伴随着连接网路的动作，CLDMA-10主要是反馈网路RRC的连接状态，用于分组处理.
11-24 20:00:25------->11-24 20:00:27, sleep_time = 26.9 , wake_up_time = 2.1 , wake_up_reason = alarm
11-24 20:05:55------->11-24 20:05:57, sleep_time = 328.7 , wake_up_time = 2.7 , wake_up_reason = alarm
11-24 20:06:16------->11-24 20:06:17, sleep_time = 17.6 , wake_up_time = 1.2 , wake_up_reason = CLDMA_MD-10
11-24 20:09:10------->11-24 20:09:11, sleep_time = 172.7 , wake_up_time = 1.2 , wake_up_reason = alarm
11-24 20:10:24------->11-24 20:10:25, sleep_time = 72.8 , wake_up_time = 1.7 , wake_up_reason = alarm
11-24 20:14:09------->11-24 20:14:10, sleep_time = 223.3 , wake_up_time = 1.3 , wake_up_reason = alarm
11-24 20:14:29------->11-24 20:14:31, sleep_time = 19.0 , wake_up_time = 2.2 , wake_up_reason = CLDMA_MD-10
11-24 20:15:10------->11-24 20:15:14, sleep_time = 38.5 , wake_up_time = 5.0 , wake_up_reason = alarm
11-24 20:20:24------->11-24 20:20:29, sleep_time = 309.0 , wake_up_time = 5.7 , wake_up_reason = alarm
11-24 20:20:49------->11-24 20:20:50, sleep_time = 19.7 , wake_up_time = 1.2 , wake_up_reason = CLDMA_MD-10
11-24 20:21:48------->11-24 20:21:49, sleep_time = 57.4 , wake_up_time = 1.7 , wake_up_reason = alarm
11-24 20:30:24------->11-24 20:30:26, sleep_time = 514.3 , wake_up_time = 2.1 , wake_up_reason = alarm
11-24 20:30:44------->11-24 20:30:45, sleep_time = 18.5 , wake_up_time = 1.2 , wake_up_reason = CLDMA_MD-10
11-24 20:31:48------->11-24 20:31:49, sleep_time = 62.2 , wake_up_time = 1.7 , wake_up_reason = alarm
11-24 20:37:48------->11-24 20:37:50, sleep_time = 358.3 , wake_up_time = 2.7 , wake_up_reason = alarm
11-24 20:38:09------->11-24 20:38:10, sleep_time = 19.1 , wake_up_time = 1.2 , wake_up_reason = CLDMA_MD-10
11-24 20:40:25------->11-24 20:40:26, sleep_time = 134.1 , wake_up_time = 1.2 , wake_up_reason = alarm
11-24 20:41:48------->11-24 20:42:17, sleep_time = 81.8 , wake_up_time = 29.7 , wake_up_reason = alarm
11-24 20:42:21------->11-24 20:42:22, sleep_time = 3.3 , wake_up_time = 1.2 , wake_up_reason = alarm

从main log看，都是QQ 建立socket 连接.
Line 3118: 11-24 23:36:13.029622 5041 5100 D Posix : [Posix_connect Debug]Process com.tencent.mobileqq:MSF :80 
Line 3153: 11-24 23:37:18.034159 5041 5100 D Posix : [Posix_connect Debug]Process com.tencent.mobileqq:MSF :80 
Line 3188: 11-24 23:38:23.024540 5041 5100 D Posix : [Posix_connect Debug]Process com.tencent.mobileqq:MSF :80 
Line 3221: 11-24 23:39:27.023168 5041 5100 D Posix : [Posix_connect Debug]Process com.tencent.mobileqq:MSF :443 
Line 3256: 11-24 23:40:33.029502 5041 5100 D Posix : [Posix_connect Debug]Process com.tencent.mobileqq:MSF :443 
Line 3289: 11-24 23:41:38.028188 5041 5100 D Posix : [Posix_connect Debug]Process com.tencent.mobileqq:MSF :14000 
Line 3332: 11-24 23:42:43.028241 5041 5100 D Posix : [Posix_connect Debug]Process com.tencent.mobileqq:MSF :14000 
Line 3357: 11-24 23:42:45.721971 1734 2651 D Posix : [Posix_connect Debug]Process com.gionee.cloud.gpe :5222 
Line 3376: 11-24 23:43:47.020887 5041 5100 D Posix : [Posix_connect Debug]Process com.tencent.mobileqq:MSF :80 
Line 3415: 11-24 23:44:51.033229 5041 5100 D Posix : [Posix_connect Debug]Process com.tencent.mobileqq:MSF :80 
Line 3448: 11-24 23:45:55.019767 5041 5100 D Posix : [Posix_connect Debug]Process com.tencent.mobileqq:MSF :80 
Line 3479: 11-24 23:45:56.217867 5012 13486 D Posix : [Posix_connect Debug]Process com.tencent.mobileqq :14000 
Line 3487: 11-24 23:45:56.223832 5012 13487 D Posix : [Posix_connect Debug]Process com.tencent.mobileqq :14000 
Line 3509: 11-24 23:45:56.302290 5012 8514 D Posix : [Posix_connect Debug]Process com.tencent.mobileqq :443


GNSPR#54702:
	现象：单卡（移动3g 卡1），开启数据业务，未涉及项保持默认待机12小时，耗电26%
	
	终于回复了......

	{白卡没有正常睡眠下去，AP端一直在发送AT+ESIMAPP，导致手机无法休眠}?

	已经申请patch，patch里修改了哪些
	patch下面都是动态库文件，没有代码


	jinhao wu
	65440
	Thanks

	当前版本：alps-mp-m0.mp1-V2.140.3_gnbj6737t.66.m0_p23？
	怎么知道版本察看，这个在ProjectConfig.mk脚本里面有更新




GNSPR#54241	恢出厂清三方，开数据关闭所有快捷开关，清后台待机11H，耗电16%、35%

这个跟modem相关

耗电比较大的局域是在上半夜，21点到0点这段时间，其中主要是集中在EINT212的唤醒，EINT212主要是C2K的ccci.
目前不是很确定是哪些channel出了问题，需要贵司进一步打开下面的log来做debug.

你先测试一下，保证有正常的log输出.

	/kernel-3.18/drivers/misc/mediatek/c2k_ccci/modem_sdio.c

	1.修改一.
	static void sdio_pio_intr_handler(struct sdio_func *func)
	..........
	..........
	if ((modem->msg->head.chanInfo == SDIO_AT_CHANNEL_NUM) ||
	(modem->msg->head.chanInfo == SDIO_AT2_CHANNEL_NUM) ||
	(modem->msg->head.chanInfo == EXCP_MSG_CH_ID) ||
	(modem->msg->head.chanInfo == EXCP_CTRL_CH_ID) ||
	(modem->msg->head.chanInfo == AGPS_CH_ID) ||
	(modem->msg->head.chanInfo == SDIO_AT3_CHANNEL_NUM)) {
	sdio_tx_rx_printk(modem->msg, 0);
	}
	.........
	.........
	修改成：
	sdio_tx_rx_printk(modem->msg, 0);
	也就是把if的条件去掉.

	2.修改二，默认把所有channel的信息打出来.
	同时修改：sdio_tx_rx_printk把LOGPRT(LOG_INFO, 修改成printk(
	/**
	*sdio_tx_rx_printk - print sdio tx and rx data, when log level is LOG_NOTICE or larger
	*@buf: the point of data buffer
	*@type: print type, 0:rx 1:tx
	*
	*no return
	*/
	static void sdio_tx_rx_printk(const void *buf, unsigned char type)
	{
	unsigned int count;
	const unsigned char *print_buf = (const unsigned char *)buf;
	const struct sdio_msg_head *head = NULL;
	/*int i; */
	/*return; */
	if (buf)
	head = (struct sdio_msg_head *)buf;
	count = calc_payload_len(head, NULL);
	if (type == 1)
	LOGPRT(LOG_INFO, "write %d to channel%d/[%d]>>",
	count, head->chanInfo, sdio_tx_cnt);
	else
	LOGPRT(LOG_INFO, "read %d from channel%d/[%d]<<",
	count, head->chanInfo, sdio_rx_cnt);
	/*if(count > RECORD_DUMP_MAX) */
	/*count = RECORD_DUMP_MAX; */
	LOGPRT
	(LOG_INFO, "%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x\n",
	*print_buf, *(print_buf + 1), *(print_buf + 2), *(print_buf + 3),
	*(print_buf + 4), *(print_buf + 5), *(print_buf + 6),
	*(print_buf + 7), *(print_buf + 8), *(print_buf + 9),
	*(print_buf + 10), *(print_buf + 11), *(print_buf + 12),
	*(print_buf + 13), *(print_buf + 14), *(print_buf + 15),
	*(print_buf + 16), *(print_buf + 17), *(print_buf + 18),
	*(print_buf + 19), *(print_buf + 20), *(print_buf + 21),
	*(print_buf + 22), *(print_buf + 23));
	/*
	for(i = 0; i < count + sizeof(struct sdio_msg_head); i++)
	{
	printk(KERN_CONT "%x-", *(print_buf+i));
	}
	printk(KERN_CONT "\n");
	*/
	}
	修改成：
	static void sdio_tx_rx_printk(const void *buf, unsigned char type)
	{
	unsigned int count;
	const unsigned char *print_buf = (const unsigned char *)buf;
	const struct sdio_msg_head *head = NULL;
	/*int i; */
	/*return; */
	if (buf)
	head = (struct sdio_msg_head *)buf;
	count = calc_payload_len(head, NULL);
	if (type == 1)
	printk("write %d to channel%d/[%d]>>",
	count, head->chanInfo, sdio_tx_cnt);
	else
	printk("read %d from channel%d/[%d]<<",
	count, head->chanInfo, sdio_rx_cnt);
	/*if(count > RECORD_DUMP_MAX) */
	/*count = RECORD_DUMP_MAX; */
	printk("%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x\n",
	*print_buf, *(print_buf + 1), *(print_buf + 2), *(print_buf + 3),
	*(print_buf + 4), *(print_buf + 5), *(print_buf + 6),
	*(print_buf + 7), *(print_buf + 8), *(print_buf + 9),
	*(print_buf + 10), *(print_buf + 11), *(print_buf + 12),
	*(print_buf + 13), *(print_buf + 14), *(print_buf + 15),
	*(print_buf + 16), *(print_buf + 17), *(print_buf + 18),
	*(print_buf + 19), *(print_buf + 20), *(print_buf + 21),
	*(print_buf + 22), *(print_buf + 23));
	/*
	for(i = 0; i < count + sizeof(struct sdio_msg_head); i++)
	{
	printk(KERN_CONT "%x-", *(print_buf+i));
	}
	printk(KERN_CONT "\n");
	*/
	}
	先试一下把log是否可以打出来.



GNSPR#58813
	现象：送测待机电流偏高



两份log看起来都一样，com.gionee.cloud.gpe定时起来，同时引起了网络跟com.android.contacts and com.gionee.cloud.gpe的连接
gionee.cloud平均6m唤醒一次，导致20s的异常波形

聯通3G:
    main log:
    Line 392: 12-02 11:51:50.708433 6156 6227 D Posix : [Posix_connect Debug]Process com.android.contacts :80
    Line 535: 12-02 11:52:05.186478 2563 3904 D Posix : [Posix_connect Debug]Process com.gionee.cloud.gpe :5222
    Line 676: 12-02 12:05:33.468285 6156 6253 D Posix : [Posix_connect Debug]Process com.android.contacts :80
    
    main log:
    12-02 11:47:53.007978 1576 1729 D AlarmManager: wakeup alarm = Alarm{2db3665 type 0 when 1480650448619 com.gionee.cloud.gpe}; package = com.gionee.cloud.gpeneedGrouping = false
    12-02 11:47:53.015030 1576 1729 D AlarmManager: wakeup alarm = Alarm{46827eb type 2 when 538857 com.gionee.softmanager}; package = com.gionee.softmanagerneedGrouping = false
    12-02 11:52:20.006299 1576 1729 D AlarmManager: wakeup alarm = Alarm{21df2fd type 2 when 806203 com.android.phone}; package = com.android.phoneneedGrouping = false
    12-02 11:58:09.012702 1576 1729 D AlarmManager: wakeup alarm = Alarm{46af243 type 0 when 1480650966655 com.gionee.cloud.gpe}; package = com.gionee.cloud.gpeneedGrouping = false
    12-02 11:58:09.015941 1576 1729 D AlarmManager: wakeup alarm = Alarm{a2bc0c0 type 2 when 1155094 com.android.phone}; package = com.android.phoneneedGrouping = false
    12-02 12:04:09.038977 1576 1729 D AlarmManager: wakeup alarm = Alarm{1b6956d type 0 when 1480651330256 com.gionee.cloud.gpe}; package = com.gionee.cloud.gpeneedGrouping = true
    12-02 12:04:09.040910 1576 1729 D AlarmManager: wakeup alarm = Alarm{b9305a2 type 2 when 1515265 com.android.phone}; package = com.android.phoneneedGrouping = true
    
    kernel log:
    12-02 11:44:14------->12-02 11:44:15, sleep_time = 31.0 , wake_up_time = 1.2 , wake_up_reason = CLDMA_MD-10
    12-02 11:44:51------->12-02 11:44:52, sleep_time = 35.1 , wake_up_time = 1.2 , wake_up_reason = alarm
    12-02 11:45:48------->12-02 11:45:49, sleep_time = 20.7 , wake_up_time = 1.8 , wake_up_reason = CLDMA_MD-10-10
    12-02 11:47:53------->12-02 11:47:54, sleep_time = 122.5 , wake_up_time = 1.2 , wake_up_reason = alarm
    12-02 11:49:36------->12-02 11:49:38, sleep_time = 102.1 , wake_up_time = 2.6 , wake_up_reason = EINT_212
    12-02 11:50:10------->12-02 11:50:11, sleep_time = 31.8 , wake_up_time = 1.2 , wake_up_reason = CLDMA_MD-10
    12-02 11:52:20------->12-02 11:52:21, sleep_time = 8.9 , wake_up_time = 1.3 , wake_up_reason = alarm
    12-02 11:58:09------->12-02 11:58:10, sleep_time = 347.7 , wake_up_time = 1.2 , wake_up_reason = alarm
    12-02 11:58:10------->12-02 11:58:13, sleep_time = 0.2 , wake_up_time = 3.1 , wake_up_reason = CLDMA_MD-20
    12-02 12:04:09------->12-02 12:04:10, sleep_time = 355.5 , wake_up_time = 1.2 , wake_up_reason = alarm
    12-02 12:04:10------->12-02 12:04:11, sleep_time = 0.2 , wake_up_time = 1.2 , wake_up_reason = CLDMA_MD-20
    
移動3G:
    main log:
    Line 447: 12-02 11:27:42.119386 3272 6423 D Posix : [Posix_connect Debug]Process com.android.contacts :80
    Line 647: 12-02 11:27:44.843551 2590 4086 D Posix : [Posix_connect Debug]Process com.gionee.cloud.gpe :5222
    Line 1174: 12-02 11:29:01.855725 2590 4086 D Posix : [Posix_connect Debug]Process com.gionee.cloud.gpe :5222
    Line 1390: 12-02 11:36:50.941793 2590 4086 D Posix : [Posix_connect Debug]Process com.gionee.cloud.gpe :5222
    Line 1460: 12-02 11:36:51.529011 3272 6612 D Posix : [Posix_connect Debug]Process com.android.contacts :80
    Line 1496: 12-02 11:36:52.649083 3272 6612 D Posix : [Posix_connect Debug]Process com.android.contacts :80
    Line 1521: 12-02 11:36:54.725319 3272 6612 D Posix : [Posix_connect Debug]Process com.android.contacts :80
    12-02 11:35:12.010899 1552 1747 D AlarmManager: wakeup alarm = Alarm{920b011 type 0 when 1480649582841 com.gionee.cloud.gpe}; package = com.gionee.cloud.gpeneedGrouping = false
    12-02 11:35:12.016786 1552 1747 D AlarmManager: wakeup alarm = Alarm{8f08777 type 2 when 1355796 com.android.phone}; package = com.android.phoneneedGrouping = false
    
    kernel log:
    12-02 11:17:02------->12-02 11:17:04, sleep_time = 5.0 , wake_up_time = 2.1 , wake_up_reason = CLDMA_MD-10
    12-02 11:17:31------->12-02 11:17:40, sleep_time = 26.5 , wake_up_time = 9.8 , wake_up_reason = CLDMA_MD-10-10-10
    12-02 11:17:44------->12-02 11:17:45, sleep_time = 3.2 , wake_up_time = 1.5 , wake_up_reason = alarm
    12-02 11:18:30------->12-02 11:18:31, sleep_time = 44.3 , wake_up_time = 1.5 , wake_up_reason = alarm
    12-02 11:18:39------->12-02 11:18:40, sleep_time = 7.6 , wake_up_time = 1.5 , wake_up_reason = CLDMA_MD-10
    12-02 11:18:44------->12-02 11:18:45, sleep_time = 3.6 , wake_up_time = 1.2 , wake_up_reason = CLDMA_MD-10
    12-02 11:21:17------->12-02 11:21:18, sleep_time = 152.4 , wake_up_time = 1.2 , wake_up_reason = CLDMA_MD-10
    12-02 11:21:22------->12-02 11:21:23, sleep_time = 3.8 , wake_up_time = 1.3 , wake_up_reason = CLDMA_MD-10
    12-02 11:22:15------->12-02 11:22:16, sleep_time = 1.4 , wake_up_time = 1.2 , wake_up_reason = CLDMA_MD-10
    12-02 11:23:15------->12-02 11:23:16, sleep_time = 59.0 , wake_up_time = 1.4 , wake_up_reason = EINT_212
    12-02 11:27:39------->12-02 11:27:51, sleep_time = 262.6 , wake_up_time = 12.6 , wake_up_reason = CLDMA_MD-10-4-4-4
    12-02 11:29:09------->12-02 11:29:15, sleep_time = 1.5 , wake_up_time = 6.5 , wake_up_reason = alarm
    12-02 11:30:05------->12-02 11:30:06, sleep_time = 50.0 , wake_up_time = 1.2 , wake_up_reason = CLDMA_MD-20
    12-02 11:35:12------->12-02 11:35:14, sleep_time = 305.0 , wake_up_time = 2.1 , wake_up_reason = alarm


待机过程中还有很多峰值较小的波形
从你提供的log看，是有网络连接，网络连接就可能会有20S，这个主要看网络的配置.
峰值较小的波形------>这种一般是APK起来.没有做网络的连接.

    kernel log:
    12-02 11:44:14------->12-02 11:44:15, sleep_time = 31.0 , wake_up_time = 1.2 , wake_up_reason = CLDMA_MD-10
    12-02 11:44:51------->12-02 11:44:52, sleep_time = 35.1 , wake_up_time = 1.2 , wake_up_reason = alarm
    12-02 11:45:48------->12-02 11:45:49, sleep_time = 20.7 , wake_up_time = 1.8 , wake_up_reason = CLDMA_MD-10-10
    12-02 11:47:53------->12-02 11:47:54, sleep_time = 122.5 , wake_up_time = 1.2 , wake_up_reason = alarm
    12-02 11:49:36------->12-02 11:49:38, sleep_time = 102.1 , wake_up_time = 2.6 , wake_up_reason = EINT_212
    12-02 11:50:10------->12-02 11:50:11, sleep_time = 31.8 , wake_up_time = 1.2 , wake_up_reason = CLDMA_MD-10
    12-02 11:52:20------->12-02 11:52:21, sleep_time = 8.9 , wake_up_time = 1.3 , wake_up_reason = alarm
    12-02 11:58:09------->12-02 11:58:10, sleep_time = 347.7 , wake_up_time = 1.2 , wake_up_reason = alarm
    12-02 11:58:10------->12-02 11:58:13, sleep_time = 0.2 , wake_up_time = 3.1 , wake_up_reason = CLDMA_MD-20
    12-02 12:04:09------->12-02 12:04:10, sleep_time = 355.5 , wake_up_time = 1.2 , wake_up_reason = alarm----->这种是APK的唤醒，可能就是小波峰。
    12-02 12:04:10------->12-02 12:04:11, sleep_time = 0.2 , wake_up_time = 1.2 , wake_up_reason = CLDMA_MD-20-----> 这种是网络的唤醒，可能会有20S


网络连接有两种情况，一种是alarm起来，然后发起网络连接，另外一种是网络返回数据，唤醒系统，这两种唤醒，都会造成modem 端连接电流很长的case.


GNSPR #56384
	现象：【品质压力】恢复出厂设置》清除所有安装的三方应用,插入SIM卡-开关飞行模式,
	关闭后台所有后台运行/设置开关,电量为99%待机11H后电量为94%》耗电5% 2/40 C45、Q23、Q22


}











	

	
	







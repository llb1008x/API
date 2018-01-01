rt5081集成了和多功能:
	charger,flash led, display bias,RGB,LDO,power delivery
	
/******************************************************************************************/
主要功能：
	
OTP： over temperature protection	过温保护
	超过160度会关闭PMU(但是这个温度是不是有点高)
	可以写0x0f,bit[7:4]读写I2C选择相应的channel关闭

	
	
VDDA：VDD表示正，A表示Analog，即模拟的意思，也就是模拟输入正极，相应的还有VSSA模拟输入负极。
	也就是说VDDA是给模拟输入供电的，模拟输入供电分为上限(UVP)和下限(OVP)
	下限	可以通过0x0e,bit[7:4]读写I2C选择关闭	
	上限可以通过0x0e,bit[0:3]读写I2C选择关闭
	准确的说是UVP和OVP shutdown function
	

MRSTB：手动复位硬件或者寄存器	
	0x01[4]使能复位功能
	0x01[3:1]设置延时去抖时间
	0x01[0]选择复位的方法（硬件或者寄存器）
	
	
IRQ watchdog:中断看门狗
	可以通过0x0d, [7:6]使能和设置interrupt watchdog timer
	
	
I2C safe timer:就是在系统休眠的时候同时充电器停止充电，而此时电池一直在消耗
	此时就称为dead battery（电池电量耗尽，电池没电），最终结果就是直接关机了
	这种机制跟看门狗很相似，应该是轮训检测避免充电器不充电，同时电池还在耗电
	可以通过0x01  bit[7]设置使能
	0x01 bit[6:5]设置timer
		
	
/******************************************************************************************/
充电部分：(从最基本的充电时序开始)
	1.trickle charge 涓流充电
	当电池电压很低，就会自动用100mA的电流，这个电流电压应该是有硬件设计的，无法通过软件改变
	
	
	2.pre charge 预充电  Vpre=3V
	这个地方系统默认的启动电压是3.6V，当然是可以通过I2C控制的
	0x1b[4:2] 
	
	
	3.constant charge 恒流充电
	电池电压大于Vpre=3V的时候，进行恒流充电，Vpre控制进入恒流的
	充电电流的主要控制因素
	
	
	4.constant voltage	恒压充电
	电池电压大于Voreg=4.2V，充电电流会下降，这个Voreg应该是控制进入恒压的电压
	主要控制因素：
	
	
	5.termination充电截止
	当充电电流低于Ieoc=250mA(log上好像是100mA)的时候过一段时间rt5081将会发送Ieoc中断，通知系统
	Ieoc应该是截止电流
	
	
	6.recharge 回充
	如果电池电压低于Voreg(两者的差值在多少)系统会进行回充，这个好像是硬件上的，但是可以控制差值
	
	
	
/*******************************************************************************************/
一些基本概念：
IR Drop compensation 这个应该是一个补偿电阻
	补偿电阻是电池充电路径上的对充电过程中因电阻造成的能量损失和电量计算的准确性
	补偿电阻可以加速从的充电时间
	当系统触发了OVP功能的时候会停止往电池充电但是还是可以通过充电器给系统供电
	
	
三个跟充电使能相关的寄存器位
	CFO_EN：这个可以使能充电器的升压和buck电路
	CHG_EN：这个应该是关闭给电池充电，让充电器的输入直接给系统，如果不足在由电池增加
	HZ	  ：这个是关闭充电内部电路
	
	
power path
	充电可以选择进电池，或是直接给系统供电，还可以按比例部分充电进电池，前提是根据充电电流的
	大小设定	
	
	
MIVR：最低输入充电电压的控制
	MIVR是通过代码直接设置的，当Vbus电压小于此值就会自动减少充电电流,用来维持充电电压在阈值之上，
	如果电压低于阈值(UVLO)可能就不能充电了 

	mivr这个值最低在4.3~4.4V之间，电池电压在4.3V左右，而电池的充电电流是根据充电器电压和电池电压之间的电压差决定的，
	MIVR是根据充电器的供电能力，设置最低的充电电压，如果电压小于这个值就降电流不降电压，保持充电电压的稳定，而mivr<4.3V
	的话电池永远充不满
	0x16[7:1]设定MIVR的最低阈值
	0x16[0]MIVR回路功能的使能
	
	
AICR：average input current regulation 平均输入电流控制
	充电电流不能大于此值,此功能是检测充电器的input current limit，也就是输出电流的最大值，是由rt5081来做；
	  但是只是在插入充电器的时候去做检测，直到拔出充电器再次插入才会再一次检测，同一个充电器不会多次检测输出能力，
	  因为同一个充电器此值肯定是相同的，没必要再检测
	  When IAICR set to large current and the VBUS drop to VMIVR level, AICL measurement will decrease IAICR level step by step automatically until VBUS voltage is large than AICL threshold
voltage
	  
	  0x13[7:2]设置AICR电流的大小
	  0x13[1:0]控制使能
	  
	  
AICL：average input current level 平均输入电流等级 
	When IAICR set to large current and the VBUS drop to VMIVR level, AICL measurement will decrease IAICR level step by step automatically until VBUS voltage is large than AICL threshold
voltage
	具体就是要满足充电能力如果不满足Vbus上的电压低于充电要求的电压，就会不断降低充电电流用来满足
充电电压的要求	



input current limit setting:  
输入电流限制的设定		0x12[3:2]
		00：AICR=3.25A
		01：根据充电器类型
		10：根据IAICR的结果
		11：取决与上面三个的最低值
		
	
	
charger watchdog:充电看门狗，0x1d
	跟一般的看门狗类似
	
	
thermal regulation setting
	四个挡位60,80,100,120 	
	当出发条件的时候会降低充电电流，同时发送TREG IRQ
	
	
	
shipping mode：	这个还是有疑问？



	
charger status：充电器的状态
	可以通过CHG_STAT引脚输出情况检测充电器的状态
	0x4A[7:6]准备好00，正在充电01，充电完成10，有问题11
	
	
	
OTG mode:OTG模式(这个在W919上要关闭)
	OTG mode 和UUG_ON都使能，升压输出将会作用到Vbus引脚上
	OTG mode 使能，UUG_ON关闭升压输出将作用到VMID引脚上，可以直接作用在PD协议支持的设备上
	
	
CHG_DSEL:开路输出可以检测Ｄ+/Ｄ-的状态，可以检测充电器的类型



charger OCP充电器的过流保护
	一旦触发了OCP,系统将会降低电压立刻锁住电感的峰值电流
	

OTG_LBP 低电保护
	可以通过0x1a[7:4]设置低电保护的电压阈值
	0x1a[3]低电保护使能
	
	
OTG_OC 过流保护
	可以	阻止Ｖbus引脚上短时的大负载的电流
	0x1a[2:0]设置保护电流的阈值
	
/*************************************************************************************/	

LDO:	(这个功能应该要掌握)
	LDO输出电压可以通过I2C接口调节[1.6,4]之间

		
	
		

 		
	
	  
	  
	  
	  
			
		
	
	

	

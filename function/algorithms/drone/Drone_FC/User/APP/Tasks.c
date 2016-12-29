#include "IMU.h"
#include "Tasks.h"
#include "Maths.h"
#include "Filter.h"
#include "Control.h"
#include "struct_all.h"

uint8_t Bsp_Int_Ok = 0;
uint16_t RUN_fre,Debug;
/******************************************************************************
函数原型：	void Nvic_Init(void)
功    能：	NVIC初始化
*******************************************************************************/ 
void Nvic_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	//NVIC_PriorityGroup 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//优先级分组
	//Timer3
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//先占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;//从占优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	//Nrf2401中断
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	//串口1
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	//串口3
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/******************************************************************************
函数原型：	void BSP_Int(void)
功    能：	硬件驱动初始化
*******************************************************************************/ 
void BSP_Int(void)
{
	LED_Init();//LED初始化
	LED_ON_OFF();//LED闪烁
	
	Uart1_Init(115200);	//串口1初始化（未使用到，方便添加模块）
	Uart3_Init(9600); //串口3初始化(超声波模块使用)
	Timer3_Init(1000);	//Timer3中断1KHZ
	Nvic_Init();		//中断优先级初始化	
	
	PrintString("\r\n RagingFire_Fly V3.5 \r\n");	//版本号
	Motor_Init();//PWM初始化
	ADC1_Init();//ADC及DMA初始化

	SPI1_Init();//SPI1初始化
	NRF24L01_Init(35,RX);//2401选择40通道，接收模式
 	NRF24L01_Check(); //检测2401是否正常

	I2C2_Int();//I2C初始化
	while( InitMPU6050()!=1 );//若MPU6050初始化不成功，则程序不向下运行
	Calculate_FilteringCoefficient(0.001f,10.f);//计算IIR滤波器参数

	FLASH_Unlock();//Flash解锁
	EEPROM_INIT();//初始化模拟的EEPROM，读取参数
	
	PID_Reset();
		
	Bsp_Int_Ok = 1;
}

/******************************************************************************
函数原型：	void Task_1000HZ(void)
功    能：	主循环中运行频率为1000HZ任务
*******************************************************************************/ 
void Task_1000HZ(void)
{
	if( MPU6050_SequenceRead()==0 )//若连续读取6050数据寄存器失败
	{
		I2C_Erro++;//统计I2C出错次数
		MPU6050_SingleRead();//分次读MPU6050数据寄存器
	}
	MPU6050_Compose();//6050数据合成
	ACC_IIR_Filter(&acc,&filter_acc);//对acc做IIR滤波
	Gyro_Filter(&gyro,&filter_gyro);//对gyro做窗口滤波
	Get_Radian(&filter_gyro,&SI_gyro);//角速度数据转为弧度
	IMUupdate(SI_gyro.x,SI_gyro.y,SI_gyro.z,filter_acc.x,filter_acc.y,filter_acc.z);//姿态解算
	Nrf_Connect();//NRF24L01连接函数
}

/******************************************************************************
函数原型：	void Task_500HZ(void)
功    能：	主循环中运行频率为500HZ任务
*******************************************************************************/ 
void Task_500HZ(void)
{
	Control_Gyro(&SI_gyro,&Rc,Rc_Lock);//内环控制
}

/******************************************************************************
函数原型：	void Task_250HZ(void)
功    能：	主循环中运行频率为250HZ任务
*******************************************************************************/ 
void Task_250HZ(void)
{
	Get_Eulerian_Angle(&out_angle);//四元数转欧拉角
	Control_Angle(&out_angle,&Rc);//外环控制
}
/******************************************************************************
函数原型：	void Task_50HZ(void)
功    能：	主循环中运行频率为50HZ任务
*******************************************************************************/ 
void Task_50HZ(void)
{
	if(high_mode)	get_high();
}

/******************************************************************************
函数原型：	void Task_1HZ(void)
功    能：	主循环中运行频率为1HZ任务
*******************************************************************************/ 
void Task_1HZ(void)
{
	get_Battery_Fly();//计算航模电池电压
	
	RUN_fre = Debug;
	Debug = 0;
}

void Task_check(void)
{
		Count_1ms++;
		Count_2ms++;
		Count_4ms++;
		Count_20ms++;
		Count_1s++;
		if(Task_check_flag == 1)
		{
			Task_err_flag ++;//每累加一次，证明代码在预定周期内没有跑完。
		}
		else
		{
			Task_check_flag = 1;//该标志位在循环的最后被清零
		}
}
void Task(void)//最短任务周期为1ms，总的代码执行时间需要小于1ms。
{
	if(Task_check_flag == 1)
	{
		if(Count_1ms>=1)
		{	
			Count_1ms = 0;
			Task_1000HZ();
		}
		if(Count_2ms>=2)
		{
			Count_2ms = 0;
			Task_500HZ();
		}
		if(Count_4ms>=4)
		{
			Count_4ms = 0;
			Task_250HZ();
		}
		if(Count_20ms>=20)
		{
			Count_20ms = 0;
			Task_50HZ();
		}
		if(Count_1s>=1000)
		{
			Count_1s = 0;
			Task_1HZ();
		}
		Task_check_flag = 0;
	}
}
			

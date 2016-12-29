#include "Tasks.h"
#include "Filter.h"

/******************************************************************************
函数原型:	static void Nvic_Init(void)
功　　能:	NVIC初始化
*******************************************************************************/ 
static void Nvic_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	//NVIC_PriorityGroup 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//优先级分组
	//Timer3
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//先占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;//从优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	//Nrf2401中断
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	//串口
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	//MODE按键
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	//FUN按键
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
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
	Uart1_Init(115200);	//串口初始化
	LED_Init();//LED初始化
	KEY_Init();//按键初始化
	Timer3_Init(500);//Timer3中断500HZ
	Nvic_Init();//中断优先级初始化	
	ADC1_Init();//ADC及DMA初始化
	SPI2_Init();//SPI2初始化
	NRF24L01_Init(35,TX);//2401选择40通道，发送模式
 	NRF24L01_Check();//检测2401是否正常
	LED_ON_OFF();//LED闪烁
	
	PrintString("\r\n RagingFire_RC V2.2.2 \r\n");//版本号
	Bsp_Int_Ok = 1;
}

/******************************************************************************
函数原型：	void Task_500HZ(void)
功    能：	主循环中运行频率为500HZ任务
*******************************************************************************/ 
void Task_500HZ(void)
{
	Nrf_Connect();//NRF24L01连接函数
	Rc_Filter(&Rc,&Filter_Rc);//计算遥控指令+滤波
}

/******************************************************************************
函数原型：	void Task_100HZ(void)
功    能：	主循环中运行频率为100HZ任务
*******************************************************************************/ 
void Task_100HZ(void)
{
	ANO_DT_Send_RCData(	Filter_Rc.THROTTLE,
											Filter_Rc.YAW,
											Filter_Rc.ROLL,
											Filter_Rc.PITCH,
											Filter_Rc.AUX1,
											Filter_Rc.AUX2,
											Filter_Rc.AUX3,
											Filter_Rc.AUX4,
											Battery_Rc,0);
	LED_Show();//LED指示灯闪烁
}

/******************************************************************************
函数原型：	void Task_25HZ(void)
功    能：	主循环中运行频率为25HZ任务
*******************************************************************************/ 
void Task_25HZ(void)
{

}

/******************************************************************************
函数原型：	void Task_4HZ(void)
功    能：	主循环中运行频率为4HZ任务
*******************************************************************************/ 
void Task_4HZ(void)
{
	Battery_Rc = (uint16_t)(2.0f*ADC_Value[3]/ADC_Value[8]*1.24f*100);//遥控电压值的100倍
}

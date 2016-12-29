#include "struct_all.h"

uint8_t offset=1;//开机校正摇杆及旋钮中点
int16_t offset0,offset1,offset2,offset3;
int16_t offset4,offset5,offset6;
uint8_t Mode=1,Fun=0;//默认锁尾模式、非夜间模式

/******************************************************************************
函数原型:	void KEY_Init(void)
功　　能:	按键初始化
*******************************************************************************/ 
void KEY_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;	//MODE按键
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;	//FUN按键
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
 
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource1);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource9);
	EXTI_InitStructure.EXTI_Line=EXTI_Line1|EXTI_Line9;
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;//外部中断
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;//下降沿触发
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	PrintString("\rKEY_Init_succeed...\n");
}

/******************************************************************************
函数原型:	void Do_Offset(void)
功　　能:	摇杆校准
*******************************************************************************/ 
void Do_Offset(void)
{
	if(offset)
	{		
		static uint8_t count=0;
		static int32_t count0,count1,count2,count3;
		static int32_t count4,count5,count6;
		if(count==0)
		{
			offset0 = 0;offset1 = 0;offset2 = 0;offset3 = 0;
			offset4 = 0;offset5 = 0;offset6 = 0;
			count0  = 0;count1  = 0;count2  = 0;count3  = 0;
			count4  = 0;count5  = 0;count6  = 0;
			count = 1;
			return;
		}
		else
		{
			count++;
			count0 += Rc.THROTTLE;
			count1 += Rc.YAW;
			count2 += Rc.PITCH;
			count3 += Rc.ROLL;
			count4 += Rc.AUX1;
			count5 += Rc.AUX2;
			count6 += Rc.AUX3;
		}
		if(count==51)
		{
			count--;
			offset0 = count0 / count - 1500;
			offset1 = count1 / count - 1500;
			offset2 = count2 / count - 1500;
			offset3 = count3 / count - 1500;
			offset4 = count4 / count - 1500;
//			offset5 = count5 / count - 1500;
			offset6 = count6 / count - 1500;
			
//			offset4 = 0x18;//不校准微调旋钮
			offset5 = 0x1A;
//			offset6 = 0x17;
			
			count = 0;
			offset = 0;
			Bee_ON;
		}
		
	}
}

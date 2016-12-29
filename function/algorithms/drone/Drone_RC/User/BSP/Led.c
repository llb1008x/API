#include "struct_all.h"

#include "math.h"
#define LED TIM2->CCR4
#define PI 3.141592653f
float light = 1.5*PI;//从灭开始
/******************************************************************************
函数原型:	static void Delay_led(uint16_t n)
功　　能:	延时
*******************************************************************************/ 
static void Delay_led(uint16_t n)
{	
	uint16_t i,j;
	for(i=0;i<n;i++)
		for(j=0;j<8888;j++);
} 

/******************************************************************************
函数原型：	static void Tim2_init(void)
功    能：	呼吸灯PWM初始化
*******************************************************************************/ 
static void Tim2_init(void)
{
	TIM_TimeBaseInitTypeDef		TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  			TIM_OCInitStructure;
	
 	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);//使能TIM2时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);//使能复用功能时钟
	
	GPIO_PinRemapConfig(GPIO_PartialRemap2_TIM2,ENABLE);
	
	//PWM频率 = 72000000 / 72 / 2000 = 500hz
	TIM_TimeBaseStructure.TIM_Period = 2000 - 1; //PWM计数上限	 
	TIM_TimeBaseStructure.TIM_Prescaler = 72 - 1; //设置用来作为TIM4时钟频率除数的预分频值，4分频
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIMx向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据TIM_TimeBaseStructure中指定的参数初始化外设TIM4
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_Pulse = 0; //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	
	TIM_OC4Init(TIM2, &TIM_OCInitStructure); 
	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable); //使能TIM2在CCR1上的预装载寄存器
	
	TIM_ARRPreloadConfig(TIM2, ENABLE); //使能TIM2在ARR上的预装载寄存器
	TIM_Cmd(TIM2, ENABLE);  //使能TIM2外设
	
	LED = 0;
}


/******************************************************************************
函数原型:	void LED_Init(void)
功　　能:	LED初始化
*******************************************************************************/ 
void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_Structure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_Structure.GPIO_Pin =  Bee_Pin;	//状态指示灯
	GPIO_Structure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Structure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(Bee_GPIO, &GPIO_Structure);
	
	GPIO_Structure.GPIO_Pin =  LEDRed_Pin;	//呼吸灯
	GPIO_Structure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Structure.GPIO_Mode = GPIO_Mode_AF_PP; 
	GPIO_Init(LEDRed_GPIO, &GPIO_Structure);
	
	GPIO_Structure.GPIO_Pin =  LEDGreen_Pin;//通信指示灯
	GPIO_Structure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Structure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(LEDGreen_GPIO, &GPIO_Structure);
	
	Tim2_init();
	
	PrintString("\rLED_Init_succeed...\n");
}

/******************************************************************************
函数原型:	void LED_ON_OFF(void)
功　　能:	LED开机闪烁
*******************************************************************************/ 
void LED_ON_OFF(void)
{
	uint8_t i ;
	for(i=0;i<3;i++)
	{
		LEDRed_ON;	LEDGreen_OFF;Bee_ON;
		Delay_led(200);
		LEDGreen_ON;	LEDRed_OFF;Bee_OFF;
		Delay_led(200);	
	}	
	LEDRed_OFF;
	LEDGreen_OFF;
	Delay_led(100);
}

/******************************************************************************
函数原型：	void Breath_LED(float speed)
参    数：	speed 呼吸速度
功    能：	呼吸灯
*******************************************************************************/ 
void Breath_LED(float speed)
{
	light += speed;
	if(light >=2*PI)	light = 0.0f;
	LED = 1999*( (sin(light)+1) / 2);
}
/******************************************************************************
函数原型：	void LED_Show(void)
功    能：	LED指示灯闪烁
*******************************************************************************/ 
void LED_Show(void)
{	
	static uint8_t i;
	
	if( Battery_Rc>300 && Battery_Rc<370 )//低压报警
	{
		Bee_ON;
		Breath_LED(0.4);
		if(Battery_Rc<350) //电压低于3.5停止工作
		{
			i++;
			if(i>=200)
			{
				TIM2->CCR4 = 1500 ;//微亮
				LEDGreen_OFF;
				while(1);
			}
		}
		else i=0;
	}
	else if(Mode)//锁尾模式，正常呼吸
	{
		Bee_OFF;
		Breath_LED(0.02);//红色LED4HZ闪烁
	}
	else//非锁尾模式，加速呼吸，（绷紧神经）
	{
		Bee_ON;
		Breath_LED(0.2);//红色LED1HZ闪烁
	}
}

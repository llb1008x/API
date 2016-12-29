#include "struct_all.h"
#define LED TIM4->CCR1
float light = 1.5*PI;//从灭开始
/******************************************************************************
函数原型：	static void Delay_led(u16 n)
功    能：	延时
*******************************************************************************/ 
void Delay_led(u16 n)
{	
	uint16_t i,j;
	for(i=0;i<n;i++)
		for(j=0;j<8500;j++);
}

/******************************************************************************
函数原型：	void LED_Init(void)
功    能：	LED初始化
*******************************************************************************/ 
static void Tim4_init(void)
{
	TIM_TimeBaseInitTypeDef		TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  			TIM_OCInitStructure;
	
 	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	
	//PWM频率 = 72000000 / 72 / 2000 = 500hz
	TIM_TimeBaseStructure.TIM_Period = 2000 - 1; //PWM计数上限	 
	TIM_TimeBaseStructure.TIM_Prescaler = 72 - 1; //设置用来作为TIM4时钟频率除数的预分频值，4分频
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIMx向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据TIM_TimeBaseStructure中指定的参数初始化外设TIM4
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_Pulse = 0; //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	
	TIM_OC1Init(TIM4, &TIM_OCInitStructure); 
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable); //使能TIM2在CCR1上的预装载寄存器
	
	TIM_ARRPreloadConfig(TIM4, ENABLE); //使能TIM2在ARR上的预装载寄存器
	TIM_Cmd(TIM4, ENABLE);  //使能TIM2外设
	
	LED = 0;
}

void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //使能呼吸灯用的时钟
	
	GPIO_InitStructure.GPIO_Pin =  LED2_Pin;	//LED2：电源旁的指示灯
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(LED2_GPIO, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 ; 				//设置呼吸灯使用到得管脚
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	Tim4_init();
	
	PrintString("\r\n 呼吸灯初始化完成！");
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
函数原型：	void LED_ON_OFF(void)
功    能：	LED开机闪烁
*******************************************************************************/ 
void LED_ON_OFF(void)
{
	uint8_t i ;
	for(i=0;i<3;i++)
	{
		LED2_ON;LED3_OFF;
		Delay_led(200);
		LED3_ON;LED2_OFF;
		Delay_led(200);	
	}
	for(i=0;i<3;i++)
	{
		LED2_ON;LED3_OFF;
		Delay_led(200);
		LED3_ON;LED2_OFF;
		Delay_led(200);	
	}	
	LED2_OFF;
	LED3_OFF;
	Delay_led(200);
	
	light = 1.5*PI;
}
void LED_FLASH(void)
{
		LED3_ON;
		Delay_led(100);
		LED3_OFF;
		Delay_led(100);
		LED3_ON;
		Delay_led(100);
		LED3_OFF;
		Delay_led(100);
		light = 1.5*PI;
}


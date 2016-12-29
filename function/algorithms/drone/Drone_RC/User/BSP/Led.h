#ifndef _LED_H_
#define _LED_H_
#include "stm32f10x.h"

/******************************************************************************
							宏定义
*******************************************************************************/ 
#define Bee_GPIO		GPIOB
#define Bee_Pin			GPIO_Pin_0//状态指示灯
#define LEDRed_GPIO		GPIOB
#define LEDRed_Pin		GPIO_Pin_11//呼吸灯
#define LEDGreen_GPIO	GPIOB
#define LEDGreen_Pin	GPIO_Pin_12//通信指示灯

#define Bee_ON 			Bee_GPIO->BRR    = Bee_Pin //状态指示灯
#define Bee_OFF  		Bee_GPIO->BSRR   	= Bee_Pin
#define LEDRed_ON 		TIM2->CCR4 = 0 	//呼吸灯
#define LEDRed_OFF  	TIM2->CCR4 = 1999
#define LEDGreen_ON  	LEDGreen_GPIO->BRR  = LEDGreen_Pin 	//通信指示灯
#define LEDGreen_OFF 	LEDGreen_GPIO->BSRR = LEDGreen_Pin 	

/******************************************************************************
							全局函数声明
*******************************************************************************/ 
void LED_Init(void);
void LED_ON_OFF(void);
void LED3_ON_Frequency(uint8_t Light_Frequency);
void LED_Show(void);
	
#endif

#ifndef _US_100_H_
#define _US_100_H_
#include "stm32f10x.h"

/******************************************************************************
							全局函数声明
*******************************************************************************/ 
void receive_higt(uint8_t com_data);
void get_high(void);
void US_100_Offset(void);
void Do_US_Offset(void);

#endif

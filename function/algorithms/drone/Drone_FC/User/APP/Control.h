#ifndef _CONTROL_H_
#define _CONTROL_H_
#include "stm32f10x.h"
#include "struct_all.h"

/******************************************************************************
							全局函数声明
*******************************************************************************/ 
void RC_LOCK(void);
void RC_Limit(struct _Rc *rc);
void Control_Angle(struct _out_angle *angle,struct _Rc *rc);
void Control_Gyro(struct _SI_float *gyro,struct _Rc *rc,uint8_t Lock);
void MY_CONTROL(struct _out_angle *angle,struct _SI_float *gyro,struct _Rc *rc,uint8_t Lock);
void Control_roll(uint8_t direction,uint8_t *control_roll_mode);
uint16_t Control_high(uint16_t THROTTLE);
#endif

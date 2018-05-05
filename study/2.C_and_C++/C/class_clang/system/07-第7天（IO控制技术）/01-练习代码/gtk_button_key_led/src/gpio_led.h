#ifndef _GPIO_LED_H_
#define _GPIO_LED_H_

//led == 1：控制第一个LED
//led == 2：控制第二个LED
//led == 3：控制第三个LED
//flag == 0：灯灭
//flag == 1：灯亮
void led_control(int led, int flag);

#endif
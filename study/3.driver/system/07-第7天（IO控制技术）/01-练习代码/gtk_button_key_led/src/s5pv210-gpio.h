#ifndef __GPIODRIVER_H__
#define __GPIODRIVER_H__

//io口名字
//gpA0 gpA1 gpB gpC0 gpC1 gpD0 gpD1 gpE0 
//gpF0 gpF1 gpF2 gpF3
//gpG0 gpG1 gpG2 gpG3 gpH0 gpH1 gpH2 gpH3 
//gpI gpJ0 gpJ1 gpJ2 gpJ3 gpJ4

#define GPIO_SET_PIN_OUT			0x01	//配置指定管脚为输出
#define GPIO_SET_PIN_IN				0x00	//配置指定管脚为输入

#define GPIO_CLR_PIN				0x10	//配置指定管脚为低电平
#define GPIO_SET_PIN				0x11	//配置指定管脚为高电平

//type: 0 - disabled, 1 - pull down, 2 - pull up
#define GPIO_SET_PULL_DIS			0x20	//禁止指定管脚上拉、下拉
#define GPIO_SET_PULL_DOWN			0x21	//配置指定管脚下拉使能
#define GPIO_SET_PULL_UP		 	0x22	//配置指定管脚上拉使能

#define GPIO_SET_PIN_DRV0			0x30	//配置io口驱动能力为1级
#define GPIO_SET_PIN_DRV1			0x31	//配置io口驱动能力为2级
#define GPIO_SET_PIN_DRV2			0x32	//配置io口驱动能力为3级

#define BIT(x) (0x01)<<(x)

#endif//__GPIODRIVER_H__

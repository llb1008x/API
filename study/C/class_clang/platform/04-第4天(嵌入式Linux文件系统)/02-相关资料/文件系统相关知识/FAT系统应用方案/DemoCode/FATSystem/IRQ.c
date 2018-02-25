//======================================================
// 文件名称：	IRQ.c
// 功能描述：	中断程序
// 维护记录：	2006-8-11	v1.0
//======================================================

#include "SPCE061A.h"
#include "Key\Key.h"

void IRQ4()__attribute__((ISR));
void IRQ4()
{
	*P_INT_Clear = C_IRQ4_1KHz;
	KeyScanServiceLoop();		//	后台按键扫描
}

void IRQ5(void)__attribute__((ISR));
void IRQ5(void)
{
	*P_INT_Clear = C_IRQ5_2Hz;
	*P_Watchdog_Clear = 0x0001;
}

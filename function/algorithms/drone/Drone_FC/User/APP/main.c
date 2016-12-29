#include "stm32f10x.h"
//#include "stdio.h"//为了正常使用printf而添加！不建议使用。

#include "IMU.h"
#include "Tasks.h"
#include "Maths.h"
#include "Filter.h"
#include "Control.h"
#include "struct_all.h"

int main(void)
{
	BSP_Int();	//底层驱动初始化
	while(1)
	{
		Task();
	}
}

//int fputc(int ch, FILE *f)	//为了正常使用printf而添加！不建议使用。
//{
//	USART_SendData(USART1, (uint8_t) ch);
//	while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
//	return ch;
//}

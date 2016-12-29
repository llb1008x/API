#include "struct_all.h"

uint8_t USART3_TxCount=0;
uint8_t USART3_Count=0;
static uint8_t USART3_TxBuff[256];	//串口发送缓冲区

/******************************************************************************
函数原型：	void Uart1_Init(uint32_t baud)
功    能：	串口初始化
参    数：  baud，波特率
*******************************************************************************/ 
void Uart3_Init(uint32_t baud)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure
	;
	/* 打开GPIO和USART部件的时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	
	/* 将USART Tx的GPIO配置为推挽复用模式 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/* 将USART Rx的GPIO配置为浮空输入模式 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/* 配置USART参数 */
	USART_InitStructure.USART_BaudRate = baud;//波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//八位数据位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一位停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//不进行奇偶校验
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//硬件流控制失能
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//接收+发送模式
	USART_Init(USART3, &USART_InitStructure);
	
	/* 使能串口接收中断 */
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
		
	/* 第5步：使能 USART， 配置完毕 */
	USART_Cmd(USART3, ENABLE);
	/* CPU的小缺陷：串口配置好，如果直接Send，则第1个字节发送不出去
		如下语句解决第1个字节无法正确发送出去的问题 */
	USART_ClearFlag(USART3, USART_FLAG_TC);//清发送完成标志位
}

/******************************************************************************
函数原型：	void USART3_IRQHandler(void)
功    能：	串口3中断
*******************************************************************************/ 
void USART3_IRQHandler(void)
{
	if(USART_GetFlagStatus(USART3, USART_FLAG_ORE) != RESET)//ORE中断
	{
		USART_ReceiveData(USART3);
	}
	
	if(USART3->SR & USART_SR_TC && (USART3->CR1 & USART_CR1_TXEIE) )//发送中断
	{
		USART3->DR = USART3_TxBuff[USART3_TxCount++];//写DR清除中断标志          
		if(USART3_TxCount == USART3_Count)
		{
			USART3->CR1 &= ~USART_CR1_TXEIE;//关闭TXE中断
		}
	}
	
	if(USART3->SR & USART_SR_RXNE)//接收中断
	{
		uint8_t com_data = USART3->DR;
		receive_higt(com_data);
	}
}

//以HEX的形式输出U8型数据
void USART3_PrintHexU8(uint8_t data)
{
	USART3_TxBuff[USART3_Count++] = data;  
	if(!(USART3->CR1 & USART_CR1_TXEIE))
	USART_ITConfig(USART3, USART_IT_TXE, ENABLE); //打开TXE中断
}

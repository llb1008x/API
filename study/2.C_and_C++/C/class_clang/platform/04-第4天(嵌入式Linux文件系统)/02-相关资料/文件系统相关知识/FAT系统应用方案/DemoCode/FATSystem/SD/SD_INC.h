#ifndef	__SD_INC_h__
#define	__SD_INC_h__
//	write your header here

#include "spce061a.h"
#include "SD_Hardware.h"
#include "SD_CRC.h"
#include "SD_Command.h"
#include "SD_Driver.h"


#define SD_CRC_EN		   		0			//	设置数据传输时是否使用CRC
#define SPI_CLOCK				185654		//	SPI总线的SCLK的频率
											//	49.152MHz下执行SPI接收/发送字节函数大概需要2118cycles
											//		发送字节需要8clk，大概折合185654Hz

#define SD_BLOCKSIZE 			512			//	SD卡块的长度
#define SD_BLOCKSIZE_NBITS		9  


#define SD_EraseBlock_EN		1			//	是否使能擦卡函数

#endif

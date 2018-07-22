#ifndef	__SD_INC_h__
#define	__SD_INC_h__
//	write your header here

#include "spce061a.h"
#include "SD_Hardware.h"
#include "SD_CRC.h"
#include "SD_Command.h"
#include "SD_Driver.h"


#define SD_CRC_EN		   		0			//	�������ݴ���ʱ�Ƿ�ʹ��CRC
#define SPI_CLOCK				185654		//	SPI���ߵ�SCLK��Ƶ��
											//	49.152MHz��ִ��SPI����/�����ֽں��������Ҫ2118cycles
											//		�����ֽ���Ҫ8clk������ۺ�185654Hz

#define SD_BLOCKSIZE 			512			//	SD����ĳ���
#define SD_BLOCKSIZE_NBITS		9  


#define SD_EraseBlock_EN		1			//	�Ƿ�ʹ�ܲ�������

#endif

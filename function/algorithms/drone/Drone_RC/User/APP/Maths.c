#include "struct_all.h"

/******************************************************************************
函数原型:	uint8_t Get_Checksum(uint8_t mydata[])
功　　能:	MWC上位机---计算校验码
*******************************************************************************/ 
uint8_t Get_Checksum(uint8_t mydata[])
{	
	uint8_t i;
    uint8_t checksum=0;
	uint8_t length = mydata[3];

    for(i=0;i<length+2;i++)
    {
        checksum ^= (mydata[3+i]&0xFF);
    }

    return checksum;
}



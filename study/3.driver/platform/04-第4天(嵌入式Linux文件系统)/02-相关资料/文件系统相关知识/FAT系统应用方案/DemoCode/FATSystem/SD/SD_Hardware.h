//======================================================
// �ļ����ƣ�	SD_Hardware.c
// ����������	SD�ײ�����ͷ�ļ�
// ά����¼��	2006-8-03	v1.0
//======================================================
#ifndef	__SD_HARDWARE_H__
#define	__SD_HARDWARE_H__

extern unsigned char SD_ChkCard(void);


extern void SD_IOInit(void);
extern unsigned char SD_ChkCardWP(void);

extern void SPI_SendByte(unsigned char ucByte);

extern unsigned char SPI_RecByte(void);

extern void SPI_CS_Assert(void);
extern void SPI_CS_Deassert(void);
extern void SD_SPIDelay(unsigned char ucTim);


#endif
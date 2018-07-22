//======================================================
// 文件名称：	SD_CRC.c
// 功能描述：	CRC计算头文件
// 维护记录：	2006-8-03	v1.0
//======================================================
#ifndef	__SD_CRC_H__
#define	__SD_CRC_H__

extern unsigned int SD_GetCRC16(unsigned char *ucArray, unsigned int uiLen);

extern unsigned char SD_GetCmdByte6(unsigned char cmdIndex, unsigned char *Param);

extern unsigned char SD_GetCRC7(unsigned char *ucArray, unsigned int uiLen);

#endif
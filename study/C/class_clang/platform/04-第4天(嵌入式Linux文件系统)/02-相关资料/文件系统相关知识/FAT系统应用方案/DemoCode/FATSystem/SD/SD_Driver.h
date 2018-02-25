//======================================================
// 文件名称：	SD_Driver.c
// 功能描述：	SD卡组件层头文件
// 维护记录：	2006-8-03	v1.0
//======================================================
#ifndef __SDDRIVER_H__
#define __SDDRIVER_H__

typedef struct _SDINFO
{	
	unsigned long int BlockNum;			//	卡中Block的数量
	unsigned long int BlockLen;			//	Block大小
	unsigned long int EraseNum;			//	一次可擦除的Block个数

	unsigned long int rTimOut;			//	读Block超时时间
	unsigned long int wTimOut;			//	写块超时时间
	unsigned long int eTimOut;			//	擦块超时时间
}SDINFO;

extern SDINFO SDInfo;						//	SD卡信息

extern unsigned char SD_Initialize(void);

extern unsigned char SD_ReadBlock(unsigned long int BlockAddr, unsigned char *BlockBuf);

extern unsigned char SD_WriteBlock(unsigned long int BlockAddr, unsigned char *BlockBuf);

extern unsigned char SD_EraseBlock(unsigned long int BlockStartAddr, unsigned long int BlockNum);

extern unsigned char SD_GetCardInfo();

extern void SD_CalTimeout(unsigned char *CSDBuf);

extern unsigned char SD_ActiveInit(void);

//	错误码
#define   SD_NO_ERR			     	0x00			//	函数执行成功
#define   SD_ERR_NO_CARD		 	0x01			//	SD卡没有完全插入到卡座中
#define   SD_ERR_USER_PARAM      	0x02			//	用户使用API函数时，入口参数有错误
#define   SD_ERR_CARD_PARAM		 	0x03			//	卡中参数有错误（与本模块不兼容）
#define	  SD_ERR_VOL_NOTSUSP        0x04			//	卡不支持3.3V供电
#define   SD_ERR_OVER_CARDRANGE		0x05			//	操作超出卡容量范围

//	Command可能返回的错误码
#define   SD_ERR_CMD_RESPTYPE	 	0x10			//	命令类型错误
#define   SD_ERR_CMD_TIMEOUT     	0x11			//	SD命令响应超时
#define   SD_ERR_CMD_RESP		 	0x12			//	SD命令响应错误
			
//	Data错误码
#define   SD_ERR_DATA_CRC16      	0x20			//	数据流CRC16校验不通过
#define   SD_ERR_DATA_START_TOK		0x21			//	读单块或多块时，数据开始令牌不正确
#define	  SD_ERR_DATA_RESP		 	0x22			//	写单块或多块时，SD卡数据响应令牌不正确

//	TimeOut错误码
#define   SD_ERR_TIMEOUT_WAIT    	0x30			//	写或擦操作时，发生超时错误
#define   SD_ERR_TIMEOUT_READ    	0x31			//	读操作超时错误
#define	  SD_ERR_TIMEOUT_WRITE	 	0x32			//	写操作超时错误
#define   SD_ERR_TIMEOUT_ERASE   	0x33			//	擦除操作超时错误
#define	  SD_ERR_TIMEOUT_WAITIDLE 	0x34			//	初始化SD卡时，等待SD卡进入空闲状态超时错误

//	Write Operate可能返回的错误码
#define	  SD_ERR_WRITE_BLK			0x40			//	写块数据错误
#define	  SD_ERR_WRITE_BLKNUMS      0x41			//	写多块时，想要写入的块与正确写入的块数不一致
#define   SD_ERR_WRITE_PROTECT		0x42			//	卡外壳的写保护开关打在写保护位置

#define SD_INIT_TIMEOUT			1000
#define SD_CMD_TIMEOUT			100
#define SD_READ_TIMEOUT			100 * SPI_CLOCK / 1000 / 8
#define SD_WRITE_TIMEOUT		250 * SPI_CLOCK / 1000 / 8    
#endif
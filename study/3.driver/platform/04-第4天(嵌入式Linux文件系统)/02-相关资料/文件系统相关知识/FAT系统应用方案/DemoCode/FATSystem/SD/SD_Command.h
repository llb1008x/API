//======================================================
// 文件名称：	SD_Command.h
// 功能描述：	SD卡命令相关头文件
// 维护记录：	2006-8-03	v1.0
//======================================================
#ifndef	__SD_COMMAND_H__
#define	__SD_COMMAND_H__

// 命令响应长度
#define R1 1
#define R1B 1
#define R2 2
#define R3 5

//	错误码（R1和R2高字节）
#define MSK_IDLE          		  0x01
#define MSK_ERASE_RST     		  0x02
#define MSK_ILL_CMD       		  0x04
#define MSK_CRC_ERR       		  0x08
#define MSK_ERASE_SEQ_ERR 		  0x10
#define MSK_ADDR_ERR      		  0x20
#define MSK_PARAM_ERR     		  0x40

//	错误码（R2低字节）
#define MSK_TOK_ERROR             0x01
#define MSK_TOK_CC_ERROR          0x02
#define MSK_TOK_ECC_FAILED        0x04
#define MSK_TOK_CC_OUTOFRANGE     0x08
#define MSK_TOK_CC_LOCKED         0x10

//	数据令牌
#define SD_TOK_READ_STARTBLOCK    0xFE
#define SD_TOK_WRITE_STARTBLOCK   0xFE
#define SD_TOK_READ_STARTBLOCK_M  0xFE
#define SD_TOK_WRITE_STARTBLOCK_M 0xFC
#define SD_TOK_STOP_MULTI         0xFD

//	数据响应令牌
#define SD_RESP_DATA_MSK		  0x0F		//	数据响应掩码
#define SD_RESP_DATA_ACCETPTED	  0x05		//	数据被接受
#define SD_RESP_DATA_REJECT_CRC	  0x0B      //	由于CRC错误而被拒绝
#define SD_RESP_DATA_REJECT_WRITE 0x0D		//	由于写错误而被拒绝

//	等待类型
#define SD_WAIT_READ			  0x00		//	读等待
#define SD_WAIT_WRITE			  0x01		//	写等待
#define SD_WAIT_ERASE		 	  0x02		//	擦除等待

#define SD_READREG_TIMEOUT		  8			//	读寄存器超时次数

#define CMD0 0								//	复位
#define CMD0_R R1

#define CMD1 1								//	读OCR寄存器
#define CMD1_R R1

#define CMD9 9								//	读CSD寄存器
#define CMD9_R R1

#define CMD13 13							//	读 Card_Status 寄存器
#define CMD13_R R2

#define CMD16 16							//	设置块的长度
#define CMD16_R R1

#define CMD17 17							//	读单块
#define CMD17_R R1

#define CMD24 24							//	写单块
#define CMD24_R R1

#define CMD32 32							//	设置擦除块的起始地址
#define CMD32_R R1

#define CMD33 33							//	设置擦除块的终止地址
#define CMD33_R R1

#define CMD38 38							//	擦除所选择的块
#define CMD38_R R1B

#define CMD58 58							//	读OCR寄存器
#define CMD58_R R3

#define CMD59 59							//	使能或禁止 CRC
#define CMD59_R R1

extern unsigned char SD_SendCmd_NoCSOP(unsigned char cmdIndex, unsigned char *Param, unsigned char resLen, unsigned char *resp);
extern unsigned char SD_SendCmd(unsigned char cmdIndex, unsigned char *Param, unsigned char resLen, unsigned char *resp);
extern void SD_DWord2Bytes(unsigned char *Param, unsigned long int ulValue);
extern unsigned char SD_BlockCommand(unsigned char cmdIndex, unsigned char resLen, unsigned long int ulParam);

extern unsigned char SD_ResetSD(void);

extern unsigned char SD_ReadCSD(unsigned char CSDLen, unsigned char *CSDBuf);

extern unsigned char SD_ReadCard_Status(unsigned char *ucBuffer);
extern unsigned char SD_SetBlockLen(unsigned long int ulLen);

extern unsigned char SD_ReadSingleBlock(unsigned long int ulBlockAddr);
extern unsigned char SD_ReadMultipleBlock(unsigned long int ulBlockAddr);

extern unsigned char SD_WriteSingleBlock(unsigned long int ulBlockAddr);
extern unsigned char SD_WriteMultipleBlock(unsigned long int ulBlockAddr);

extern unsigned char SD_ProgramCSD(unsigned char CSDLen, unsigned char *CSDBuf);

extern unsigned char SD_EraseStartBlock(unsigned long int ulStartBlockAddr);
extern unsigned char SD_EraseEndBlock(unsigned long int ulEndBlockAddr);
extern unsigned char SD_EraseSelectedBlock(void);

extern unsigned char SD_ReadOCR(unsigned char *OCRBuf);
extern unsigned char SD_EnableCRC(unsigned char Enable);

extern unsigned char SD_ReadSD_Status(unsigned char SDSLen, unsigned char *SDSBuf);
extern unsigned char SD_ReadSCR(unsigned char SCRLen, unsigned char *SCRBuf);
extern unsigned char SD_GetNumWRBlcoks(unsigned long int *ulBlockNum);

extern unsigned char SD_ReadRegister(unsigned long int ulLen, unsigned char *ucRegBuf);
extern unsigned char SD_ReadBlockData(unsigned long int BlockLen, unsigned char *BlockBuf);
extern unsigned char SD_WriteBlockData(unsigned char Multi, unsigned long int BlockLen, unsigned char *BlockBuf);

extern void SD_StopMultiToken(void);
extern unsigned char SD_WaitBusy(unsigned char Type);

#endif

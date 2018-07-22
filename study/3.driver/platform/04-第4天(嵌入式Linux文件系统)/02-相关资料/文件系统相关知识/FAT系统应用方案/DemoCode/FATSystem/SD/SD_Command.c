//======================================================
// 文件名称：	SD_Command.c
// 功能描述：	SD卡命令相关函数
// 维护记录：	2006-8-03	v1.0
//======================================================
#include "SD_INC.h"

//=============================================================
// 语法格式：	unsigned char SD_SendCmd_NoCSOP(unsigned char cmdIndex, unsigned char *Param, unsigned char resLen, unsigned char *resp)
// 实现功能：	发送命令(不对CS操作)
// 参数：		cmdIndex	-	命令序号
//				Param		-	参数首地址
//				resLen		-	响应长度
//				resp		-	响应存储首地址
// 返回值：		错误类型
//=============================================================
unsigned char SD_SendCmd_NoCSOP(unsigned char cmdIndex, unsigned char *Param, unsigned char resLen, unsigned char *resp)
{
	int i;
	unsigned char ucTemp;
	SPI_SendByte((cmdIndex & 0x3F) | 0x40);			//	发送命令
	for (i = 3; i >= 0; i--)
		SPI_SendByte(Param[i]);						//	发送参数

	ucTemp = SD_GetCmdByte6((cmdIndex & 0x3F) | 0x40, Param);
	SPI_SendByte(ucTemp);

	i = 0;				
	do
	{												//	等待SD卡返回响应
		ucTemp = SPI_RecByte();
		i++;
	}while (((ucTemp & 0x80) != 0) && (i < SD_CMD_TIMEOUT));
    if (i >= SD_CMD_TIMEOUT)
    {
        return SD_ERR_CMD_TIMEOUT;					//	返回命令超时错误
    }
    for (i = resLen - 1; i >= 0; i--)
    {
        resp[i] = ucTemp;
        ucTemp = SPI_RecByte();						//	存储响应
    }
    return SD_NO_ERR;								//	返回成功
}

//=============================================================
// 语法格式：	unsigned char SD_SendCmd(unsigned char cmdIndex, unsigned char *param, unsigned char resptype, unsigned char *resp)
// 实现功能：	发送命令
// 参数：		cmdIndex	-	命令序号
//				Param		-	参数首地址
//				resLen		-	响应长度
//				resp		-	响应存储首地址
// 返回值：		错误类型
//=============================================================
unsigned char SD_SendCmd(unsigned char cmdIndex, unsigned char *Param, unsigned char resLen, unsigned char *resp)
{
	unsigned char ucRet;

	SPI_CS_Assert();
	SPI_SendByte(0xff);
	SPI_SendByte(0xff);
	ucRet = SD_SendCmd_NoCSOP(cmdIndex, Param, resLen, resp);
	SPI_CS_Deassert();
	return (ucRet);
}

//=============================================================
// 语法格式：	void SD_DWord2Bytes(unsigned char *Param, unsigned long int ulValue)
// 实现功能：	双字转换为字节
// 参数：		Param		-	参数首地址
//				ulValue		-	双字参数
// 返回值：		无
//=============================================================
void SD_DWord2Bytes(unsigned char *Param, unsigned long int ulValue)
{
	Param[0] = (unsigned char)(ulValue);
	Param[1] = (unsigned char)(ulValue >> 8);
	Param[2] = (unsigned char)(ulValue >> 16);
	Param[3] = (unsigned char)(ulValue >> 24);
}

//=============================================================
// 语法格式：	unsigned char SD_BlockCommand(unsigned char cmdIndex, unsigned char resLen, unsigned long int ulParam)
// 实现功能：	发送Block相关命令
// 参数：		cmdIndex	-	命令序号
//				resLen		-	响应长度
//				ulParam		-	参数(Block号)
// 返回值：		错误类型
//=============================================================
unsigned char SD_BlockCommand(unsigned char cmdIndex, unsigned char resLen, unsigned long int ulParam)
{
	unsigned char ucParam[4],ucResp,ucRet;
	ulParam <<= SD_BLOCKSIZE_NBITS;					//	Block号转换为地址
	SD_DWord2Bytes(ucParam, ulParam);
	ucRet = SD_SendCmd(cmdIndex, ucParam, resLen, &ucResp);
	if(ucRet != SD_NO_ERR)
		return ucRet;								//	发送命令失败
	if (ucResp != 0)
		return SD_ERR_CMD_RESP;						//	响应不正确
	return SD_NO_ERR;
}

//=============================================================
// 语法格式：	unsigned char SD_ResetSD(void)
// 实现功能：	发送SD卡软件复位命令
// 参数：		无
// 返回值：		错误类型
//=============================================================
unsigned char SD_ResetSD(void)
{
	unsigned char ucParam[4] = {0,0,0,0},ucResp;
    return(SD_SendCmd(CMD0, ucParam, CMD0_R, &ucResp));
}

//=============================================================
// 语法格式：	unsigned char SD_ReadCSD(unsigned char CSDLen, unsigned char *CSDBuf)
// 实现功能：	发送读CSD寄存器命令
// 参数：		CSDLen	-	CSD寄存器长度
//				CSDBuf	-	CSD寄存器内容存储缓冲区首址
// 返回值：		错误类型
//=============================================================
unsigned char SD_ReadCSD(unsigned char CSDLen, unsigned char *CSDBuf)
{
	unsigned char ucParam[4] = {0,0,0,0};
	unsigned char ucResp, ucRet;
	ucRet = SD_SendCmd(CMD9, ucParam, CMD9_R, &ucResp);
	if (ucRet != SD_NO_ERR) 									
		return ucRet;									
	if (ucResp != 0)
		return SD_ERR_CMD_RESP;
	return (SD_ReadRegister(CSDLen, CSDBuf));
}

//=============================================================
// 语法格式：	unsigned char SD_ReadCard_Status(unsigned char *ucBuffer)
// 实现功能：	发送读Crad Status寄存器命令
// 参数：		无
// 返回值：		错误类型
//=============================================================
unsigned char SD_ReadCard_Status(unsigned char *ucBuffer)
{
	unsigned char ucParam[4] = {0, 0, 0, 0};
	return (SD_SendCmd(CMD13, ucParam, CMD13_R, ucBuffer));
}

//=============================================================
// 语法格式：	unsigned char SD_ReadCard_Status(unsigned char *ucBuffer)
// 实现功能：	发送设置Block大小命令
// 参数：		无
// 返回值：		错误类型
//=============================================================
unsigned char SD_SetBlockLen(unsigned long int ulLen)
{
	unsigned char ucParam[4];
	unsigned char ucResp, ucRet;
	SD_DWord2Bytes(ucParam, ulLen);
	ucRet = SD_SendCmd(CMD16, ucParam, CMD16_R, &ucResp);
	if (ucRet != SD_NO_ERR)
		return ucRet;
	if (ucResp != 0)
		return SD_ERR_CMD_RESP;
	return SD_NO_ERR;
}

//=============================================================
// 语法格式：	unsigned char SD_ReadSingleBlock(unsigned long int ulBlockAddr)
// 实现功能：	发送读Block函数命令
// 参数：		Block地址
// 返回值：		错误类型
//=============================================================
unsigned char SD_ReadSingleBlock(unsigned long int ulBlockAddr)
{
	return(SD_BlockCommand(CMD17, CMD17_R, ulBlockAddr));
}

//=============================================================
// 语法格式：	unsigned char SD_WriteSingleBlock(unsigned long int ulBlockAddr)
// 实现功能：	发送写Block函数命令
// 参数：		ulBlockAddr	-	Block地址
// 返回值：		错误类型
//=============================================================
unsigned char SD_WriteSingleBlock(unsigned long int ulBlockAddr)
{
	return (SD_BlockCommand(CMD24, CMD24_R, ulBlockAddr));
}

#if SD_EraseBlock_EN
//=============================================================
// 语法格式：	unsigned char SD_EraseStartBlock(unsigned long int ulStartBlockAddr)
// 实现功能：	发送设置Block擦除起始地址命令
// 参数：		ulStartBlockAddr	-	Block地址
// 返回值：		错误类型
//=============================================================
unsigned char SD_EraseStartBlock(unsigned long int ulStartBlockAddr)
{
	return (SD_BlockCommand(CMD32, CMD32_R, ulStartBlockAddr));
}

//=============================================================
// 语法格式：	unsigned char SD_EraseEndBlock(unsigned long int ulStartBlockAddr)
// 实现功能：	发送设置Block擦除结束地址命令
// 参数：		ulEndBlockAddr	-	Block地址
// 返回值：		错误类型
//=============================================================
unsigned char SD_EraseEndBlock(unsigned long int ulEndBlockAddr)
{
	return (SD_BlockCommand(CMD33, CMD33_R, ulEndBlockAddr));
}

//=============================================================
// 语法格式：	unsigned char SD_EraseSelectedBlock(void)
// 实现功能：	发送擦除选中的Block命令
// 参数：		无
// 返回值：		错误类型
//=============================================================
unsigned char SD_EraseSelectedBlock(void)
{
	unsigned char ucParam[4] = {0};
	unsigned char ucResp, ucRet;
	ucRet = SD_SendCmd(CMD38, ucParam, CMD38_R, &ucResp);
	if (ucRet != SD_NO_ERR)
		return ucRet;
	if (SD_WaitBusy(SD_WAIT_ERASE) != SD_NO_ERR)	//	等待擦除完成
		return SD_ERR_TIMEOUT_ERASE;
	else
		return SD_NO_ERR;									
}	
#endif

//=============================================================
// 语法格式：	unsigned char SD_ReadOCR(unsigned char *OCRBuf)
// 实现功能：	发送读操作条件寄存器OCR命令
// 参数：		无
// 返回值：		错误类型
//=============================================================
unsigned char SD_ReadOCR(unsigned char *OCRBuf)
{
	unsigned char ucParam[4] = {0,0,0,0};
	unsigned char ucResp[5];
	unsigned char ucRet;
	ucRet = SD_SendCmd(CMD58, ucParam, CMD58_R, ucResp);//	读OCR寄存器
	if (ucRet != SD_NO_ERR)
		return ucRet;		 										
	if (ucResp[0] != 0)
		return SD_ERR_CMD_RESP;
    for (ucRet = 0; ucRet < 4; ucRet++)
    	OCRBuf[ucRet] = ucResp[ucRet + 1];				//	复制OCR寄存器内容到接收缓冲区
	return SD_NO_ERR;
}

//=============================================================
// 语法格式：	unsigned char SD_EnableCRC(unsigned char Enable)
// 实现功能：	发送使能CRC校验命令
// 参数：		Enable	-	使能CRC
// 返回值：		错误类型
//=============================================================
unsigned char SD_EnableCRC(unsigned char Enable)
{
	unsigned char ucParam[4] = {0};
	unsigned char ucResp, ucRet;
	
	if (Enable == 1)
		ucParam[0] = 1;								//	使能CRC
	else
		ucParam[0] = 0;								//	禁止CRC

	ucRet = SD_SendCmd(CMD59, ucParam, CMD59_R, &ucResp);
	if (ucRet != SD_NO_ERR)
		return ucRet;

	if (ucResp != 0)
		return SD_ERR_CMD_RESP;

	return SD_NO_ERR;
}

//=============================================================
// 语法格式：	unsigned char SD_ReadRegister(unsigned long int ulLen, unsigned char *ucRegBuf)
// 实现功能：	读取寄存器数据
// 参数：		ulLen	-	寄存器长度
//				ucRegBuf-	寄存器存储缓冲区首址
// 返回值：		错误类型
//=============================================================
unsigned char SD_ReadRegister(unsigned long int ulLen, unsigned char *ucRegBuf)
{	
	unsigned long int i = 0;
	unsigned char ucResp;

	SPI_CS_Assert();
	do{												//	等待数据起始令牌
		ucResp = SPI_RecByte();
		i++;
	}while((ucResp == 0xFF) && (i < SD_READREG_TIMEOUT));    

	if (i >= SD_READREG_TIMEOUT)
	{
		SPI_CS_Deassert();
		return SD_ERR_TIMEOUT_READ;					//	超时
	}

	if (ucResp != SD_TOK_READ_STARTBLOCK)				
	{												//	收到的不是数据起始令牌
		ucRegBuf[0] = ucResp;							
		i = 1;										//	还有ulLen - 1个字节要接收
	}
	else
		i = 0;										//	收到数据起始令牌，还有ulLen个字节要接收

	for (; i < ulLen; i++)
		ucRegBuf[i] = SPI_RecByte();				//	接收数据

#if SD_CRC_EN 
	i = SPI_RecByte();								
	i = (i << 8) + SPI_RecByte();					//	读取16bit CRC

	if (i != SD_GetCRC16(ucRegBuf, ulLen))
	{												//	CRC 错误
		SPI_CS_Deassert();
		SPI_SendByte(0xFF);
		return SD_ERR_DATA_CRC16;
	}
#endif

	SPI_SendByte(0xFF);
	SPI_CS_Deassert();

	return SD_NO_ERR;
}

//=============================================================
// 语法格式：	unsigned char SD_ReadBlockData(unsigned long int BlockLen, unsigned char *BlockBuf)
// 实现功能：	读数据块
// 参数：		BlockLen	-	数据块长度
//				BlockBuf	-	数据块存储缓冲区首址
// 返回值：		错误类型
//=============================================================
unsigned char SD_ReadBlockData(unsigned long int BlockLen, unsigned char *BlockBuf)
{
	unsigned char ucTemp;
	unsigned long int i = 0;

	SPI_CS_Assert();    
	do
	{												//	等待数据起始令牌
		ucTemp = SPI_RecByte();
		i++;
	}while((ucTemp == 0xFF) && (i < SDInfo.rTimOut));

	if (i >= SDInfo.rTimOut)
	{
		SPI_CS_Deassert();
		return SD_ERR_TIMEOUT_READ;					//	超时
	}

	if (ucTemp != SD_TOK_READ_STARTBLOCK)			//	令牌错误
	{
		SPI_SendByte(0xFF);
		SPI_CS_Deassert();
		return SD_ERR_DATA_START_TOK;
	}

	for (i = 0; i < BlockLen; i++)
		BlockBuf[i] = SPI_RecByte();				//	接收数据

#if SD_CRC_EN
	i = SPI_RecByte();
	i = (i << 8) + SPI_RecByte();					//	读取16bit CRC

	if (i != SD_GetCRC16(BlockBuf, BlockLen))
	{												//	CRC校验错误
		SPI_SendByte(0xFF); 
		SPI_CS_Deassert();  						
		return SD_ERR_DATA_CRC16;
	}
#endif

	SPI_SendByte(0xFF); 
	SPI_CS_Deassert();

	return SD_NO_ERR;
}

//=============================================================
// 语法格式：	unsigned char SD_WriteBlockData(unsigned char Multi, unsigned long int BlockLen, unsigned char *BlockBuf)
// 实现功能：	读数据块
// 参数：		Multi		-	是否为多块操作
//				BlockLen	-	长度
//				BlockBuf	-	Block存储缓冲区首址
// 返回值：		错误类型
//=============================================================
unsigned char SD_WriteBlockData(unsigned char Multi, unsigned long int BlockLen, unsigned char *BlockBuf)
{
	unsigned int i;
	unsigned char ucTemp;

	SPI_CS_Assert();

	if (Multi == 1)
		SPI_SendByte(SD_TOK_WRITE_STARTBLOCK_M);	//	写多块起始令牌
	else
		SPI_SendByte(SD_TOK_WRITE_STARTBLOCK);		//	写单块起始令牌

	for (i = 0; i < BlockLen; i++)
		SPI_SendByte(BlockBuf[i]);					//	发送数据

#if SD_CRC_EN	
	i = SD_GetCRC16(BlockBuf, BlockLen);

	SPI_SendByte((i >> 8) & 0xFF);
	SPI_SendByte(i & 0xFF);							//	发送CRC
#else
	SD_SPIDelay(2);
#endif

	ucTemp = SPI_RecByte();
	if ((ucTemp & SD_RESP_DATA_MSK) != SD_RESP_DATA_ACCETPTED)	
	{
		SPI_CS_Deassert();
		SPI_SendByte(0xFF);
		return SD_ERR_DATA_RESP;
	}

	if (SD_WaitBusy(SD_WAIT_WRITE) != SD_NO_ERR)			
		return SD_ERR_TIMEOUT_WRITE;				//	超时
	else
		return SD_NO_ERR;
}

//=============================================================
// 语法格式：	void SD_StopMultiToken(void)
// 实现功能：	发送多块写停止令牌
// 参数：		无
// 返回值：		无
//=============================================================
void SD_StopMultiToken(void)
{
	SPI_CS_Assert();
	SPI_SendByte(0xFF);
	SPI_SendByte(SD_TOK_STOP_MULTI);				//	发送停止数据传输令牌
	SPI_RecByte();
	SPI_CS_Deassert();
}

//=============================================================
// 语法格式：	unsigned char SD_WaitBusy(unsigned char Type)
// 实现功能：	查询SD卡是否处于忙状态
// 参数：		Type	-	查询类型
// 返回值：		无
//=============================================================
unsigned char SD_WaitBusy(unsigned char Type)
{
	unsigned long int ulTimOut, i = 0;
	unsigned char ucTemp;

	if (Type == SD_WAIT_WRITE)
		ulTimOut = SDInfo.wTimOut;
	else
		ulTimOut = SDInfo.eTimOut;

	SPI_CS_Assert();
	do
	{												//	等待空闲
		ucTemp = SPI_RecByte();
		i++;
	}while ((ucTemp != 0xFF) && (i < ulTimOut));	//	忙则收到0xff
	SPI_CS_Deassert();

	if(i < ulTimOut) 
		return SD_NO_ERR;
	else 
		return SD_ERR_TIMEOUT_WAIT;
}

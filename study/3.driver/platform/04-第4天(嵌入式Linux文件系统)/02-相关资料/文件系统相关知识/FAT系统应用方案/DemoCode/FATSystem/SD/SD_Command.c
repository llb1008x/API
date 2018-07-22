//======================================================
// �ļ����ƣ�	SD_Command.c
// ����������	SD��������غ���
// ά����¼��	2006-8-03	v1.0
//======================================================
#include "SD_INC.h"

//=============================================================
// �﷨��ʽ��	unsigned char SD_SendCmd_NoCSOP(unsigned char cmdIndex, unsigned char *Param, unsigned char resLen, unsigned char *resp)
// ʵ�ֹ��ܣ�	��������(����CS����)
// ������		cmdIndex	-	�������
//				Param		-	�����׵�ַ
//				resLen		-	��Ӧ����
//				resp		-	��Ӧ�洢�׵�ַ
// ����ֵ��		��������
//=============================================================
unsigned char SD_SendCmd_NoCSOP(unsigned char cmdIndex, unsigned char *Param, unsigned char resLen, unsigned char *resp)
{
	int i;
	unsigned char ucTemp;
	SPI_SendByte((cmdIndex & 0x3F) | 0x40);			//	��������
	for (i = 3; i >= 0; i--)
		SPI_SendByte(Param[i]);						//	���Ͳ���

	ucTemp = SD_GetCmdByte6((cmdIndex & 0x3F) | 0x40, Param);
	SPI_SendByte(ucTemp);

	i = 0;				
	do
	{												//	�ȴ�SD��������Ӧ
		ucTemp = SPI_RecByte();
		i++;
	}while (((ucTemp & 0x80) != 0) && (i < SD_CMD_TIMEOUT));
    if (i >= SD_CMD_TIMEOUT)
    {
        return SD_ERR_CMD_TIMEOUT;					//	�������ʱ����
    }
    for (i = resLen - 1; i >= 0; i--)
    {
        resp[i] = ucTemp;
        ucTemp = SPI_RecByte();						//	�洢��Ӧ
    }
    return SD_NO_ERR;								//	���سɹ�
}

//=============================================================
// �﷨��ʽ��	unsigned char SD_SendCmd(unsigned char cmdIndex, unsigned char *param, unsigned char resptype, unsigned char *resp)
// ʵ�ֹ��ܣ�	��������
// ������		cmdIndex	-	�������
//				Param		-	�����׵�ַ
//				resLen		-	��Ӧ����
//				resp		-	��Ӧ�洢�׵�ַ
// ����ֵ��		��������
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
// �﷨��ʽ��	void SD_DWord2Bytes(unsigned char *Param, unsigned long int ulValue)
// ʵ�ֹ��ܣ�	˫��ת��Ϊ�ֽ�
// ������		Param		-	�����׵�ַ
//				ulValue		-	˫�ֲ���
// ����ֵ��		��
//=============================================================
void SD_DWord2Bytes(unsigned char *Param, unsigned long int ulValue)
{
	Param[0] = (unsigned char)(ulValue);
	Param[1] = (unsigned char)(ulValue >> 8);
	Param[2] = (unsigned char)(ulValue >> 16);
	Param[3] = (unsigned char)(ulValue >> 24);
}

//=============================================================
// �﷨��ʽ��	unsigned char SD_BlockCommand(unsigned char cmdIndex, unsigned char resLen, unsigned long int ulParam)
// ʵ�ֹ��ܣ�	����Block�������
// ������		cmdIndex	-	�������
//				resLen		-	��Ӧ����
//				ulParam		-	����(Block��)
// ����ֵ��		��������
//=============================================================
unsigned char SD_BlockCommand(unsigned char cmdIndex, unsigned char resLen, unsigned long int ulParam)
{
	unsigned char ucParam[4],ucResp,ucRet;
	ulParam <<= SD_BLOCKSIZE_NBITS;					//	Block��ת��Ϊ��ַ
	SD_DWord2Bytes(ucParam, ulParam);
	ucRet = SD_SendCmd(cmdIndex, ucParam, resLen, &ucResp);
	if(ucRet != SD_NO_ERR)
		return ucRet;								//	��������ʧ��
	if (ucResp != 0)
		return SD_ERR_CMD_RESP;						//	��Ӧ����ȷ
	return SD_NO_ERR;
}

//=============================================================
// �﷨��ʽ��	unsigned char SD_ResetSD(void)
// ʵ�ֹ��ܣ�	����SD�������λ����
// ������		��
// ����ֵ��		��������
//=============================================================
unsigned char SD_ResetSD(void)
{
	unsigned char ucParam[4] = {0,0,0,0},ucResp;
    return(SD_SendCmd(CMD0, ucParam, CMD0_R, &ucResp));
}

//=============================================================
// �﷨��ʽ��	unsigned char SD_ReadCSD(unsigned char CSDLen, unsigned char *CSDBuf)
// ʵ�ֹ��ܣ�	���Ͷ�CSD�Ĵ�������
// ������		CSDLen	-	CSD�Ĵ�������
//				CSDBuf	-	CSD�Ĵ������ݴ洢��������ַ
// ����ֵ��		��������
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
// �﷨��ʽ��	unsigned char SD_ReadCard_Status(unsigned char *ucBuffer)
// ʵ�ֹ��ܣ�	���Ͷ�Crad Status�Ĵ�������
// ������		��
// ����ֵ��		��������
//=============================================================
unsigned char SD_ReadCard_Status(unsigned char *ucBuffer)
{
	unsigned char ucParam[4] = {0, 0, 0, 0};
	return (SD_SendCmd(CMD13, ucParam, CMD13_R, ucBuffer));
}

//=============================================================
// �﷨��ʽ��	unsigned char SD_ReadCard_Status(unsigned char *ucBuffer)
// ʵ�ֹ��ܣ�	��������Block��С����
// ������		��
// ����ֵ��		��������
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
// �﷨��ʽ��	unsigned char SD_ReadSingleBlock(unsigned long int ulBlockAddr)
// ʵ�ֹ��ܣ�	���Ͷ�Block��������
// ������		Block��ַ
// ����ֵ��		��������
//=============================================================
unsigned char SD_ReadSingleBlock(unsigned long int ulBlockAddr)
{
	return(SD_BlockCommand(CMD17, CMD17_R, ulBlockAddr));
}

//=============================================================
// �﷨��ʽ��	unsigned char SD_WriteSingleBlock(unsigned long int ulBlockAddr)
// ʵ�ֹ��ܣ�	����дBlock��������
// ������		ulBlockAddr	-	Block��ַ
// ����ֵ��		��������
//=============================================================
unsigned char SD_WriteSingleBlock(unsigned long int ulBlockAddr)
{
	return (SD_BlockCommand(CMD24, CMD24_R, ulBlockAddr));
}

#if SD_EraseBlock_EN
//=============================================================
// �﷨��ʽ��	unsigned char SD_EraseStartBlock(unsigned long int ulStartBlockAddr)
// ʵ�ֹ��ܣ�	��������Block������ʼ��ַ����
// ������		ulStartBlockAddr	-	Block��ַ
// ����ֵ��		��������
//=============================================================
unsigned char SD_EraseStartBlock(unsigned long int ulStartBlockAddr)
{
	return (SD_BlockCommand(CMD32, CMD32_R, ulStartBlockAddr));
}

//=============================================================
// �﷨��ʽ��	unsigned char SD_EraseEndBlock(unsigned long int ulStartBlockAddr)
// ʵ�ֹ��ܣ�	��������Block����������ַ����
// ������		ulEndBlockAddr	-	Block��ַ
// ����ֵ��		��������
//=============================================================
unsigned char SD_EraseEndBlock(unsigned long int ulEndBlockAddr)
{
	return (SD_BlockCommand(CMD33, CMD33_R, ulEndBlockAddr));
}

//=============================================================
// �﷨��ʽ��	unsigned char SD_EraseSelectedBlock(void)
// ʵ�ֹ��ܣ�	���Ͳ���ѡ�е�Block����
// ������		��
// ����ֵ��		��������
//=============================================================
unsigned char SD_EraseSelectedBlock(void)
{
	unsigned char ucParam[4] = {0};
	unsigned char ucResp, ucRet;
	ucRet = SD_SendCmd(CMD38, ucParam, CMD38_R, &ucResp);
	if (ucRet != SD_NO_ERR)
		return ucRet;
	if (SD_WaitBusy(SD_WAIT_ERASE) != SD_NO_ERR)	//	�ȴ��������
		return SD_ERR_TIMEOUT_ERASE;
	else
		return SD_NO_ERR;									
}	
#endif

//=============================================================
// �﷨��ʽ��	unsigned char SD_ReadOCR(unsigned char *OCRBuf)
// ʵ�ֹ��ܣ�	���Ͷ����������Ĵ���OCR����
// ������		��
// ����ֵ��		��������
//=============================================================
unsigned char SD_ReadOCR(unsigned char *OCRBuf)
{
	unsigned char ucParam[4] = {0,0,0,0};
	unsigned char ucResp[5];
	unsigned char ucRet;
	ucRet = SD_SendCmd(CMD58, ucParam, CMD58_R, ucResp);//	��OCR�Ĵ���
	if (ucRet != SD_NO_ERR)
		return ucRet;		 										
	if (ucResp[0] != 0)
		return SD_ERR_CMD_RESP;
    for (ucRet = 0; ucRet < 4; ucRet++)
    	OCRBuf[ucRet] = ucResp[ucRet + 1];				//	����OCR�Ĵ������ݵ����ջ�����
	return SD_NO_ERR;
}

//=============================================================
// �﷨��ʽ��	unsigned char SD_EnableCRC(unsigned char Enable)
// ʵ�ֹ��ܣ�	����ʹ��CRCУ������
// ������		Enable	-	ʹ��CRC
// ����ֵ��		��������
//=============================================================
unsigned char SD_EnableCRC(unsigned char Enable)
{
	unsigned char ucParam[4] = {0};
	unsigned char ucResp, ucRet;
	
	if (Enable == 1)
		ucParam[0] = 1;								//	ʹ��CRC
	else
		ucParam[0] = 0;								//	��ֹCRC

	ucRet = SD_SendCmd(CMD59, ucParam, CMD59_R, &ucResp);
	if (ucRet != SD_NO_ERR)
		return ucRet;

	if (ucResp != 0)
		return SD_ERR_CMD_RESP;

	return SD_NO_ERR;
}

//=============================================================
// �﷨��ʽ��	unsigned char SD_ReadRegister(unsigned long int ulLen, unsigned char *ucRegBuf)
// ʵ�ֹ��ܣ�	��ȡ�Ĵ�������
// ������		ulLen	-	�Ĵ�������
//				ucRegBuf-	�Ĵ����洢��������ַ
// ����ֵ��		��������
//=============================================================
unsigned char SD_ReadRegister(unsigned long int ulLen, unsigned char *ucRegBuf)
{	
	unsigned long int i = 0;
	unsigned char ucResp;

	SPI_CS_Assert();
	do{												//	�ȴ�������ʼ����
		ucResp = SPI_RecByte();
		i++;
	}while((ucResp == 0xFF) && (i < SD_READREG_TIMEOUT));    

	if (i >= SD_READREG_TIMEOUT)
	{
		SPI_CS_Deassert();
		return SD_ERR_TIMEOUT_READ;					//	��ʱ
	}

	if (ucResp != SD_TOK_READ_STARTBLOCK)				
	{												//	�յ��Ĳ���������ʼ����
		ucRegBuf[0] = ucResp;							
		i = 1;										//	����ulLen - 1���ֽ�Ҫ����
	}
	else
		i = 0;										//	�յ�������ʼ���ƣ�����ulLen���ֽ�Ҫ����

	for (; i < ulLen; i++)
		ucRegBuf[i] = SPI_RecByte();				//	��������

#if SD_CRC_EN 
	i = SPI_RecByte();								
	i = (i << 8) + SPI_RecByte();					//	��ȡ16bit CRC

	if (i != SD_GetCRC16(ucRegBuf, ulLen))
	{												//	CRC ����
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
// �﷨��ʽ��	unsigned char SD_ReadBlockData(unsigned long int BlockLen, unsigned char *BlockBuf)
// ʵ�ֹ��ܣ�	�����ݿ�
// ������		BlockLen	-	���ݿ鳤��
//				BlockBuf	-	���ݿ�洢��������ַ
// ����ֵ��		��������
//=============================================================
unsigned char SD_ReadBlockData(unsigned long int BlockLen, unsigned char *BlockBuf)
{
	unsigned char ucTemp;
	unsigned long int i = 0;

	SPI_CS_Assert();    
	do
	{												//	�ȴ�������ʼ����
		ucTemp = SPI_RecByte();
		i++;
	}while((ucTemp == 0xFF) && (i < SDInfo.rTimOut));

	if (i >= SDInfo.rTimOut)
	{
		SPI_CS_Deassert();
		return SD_ERR_TIMEOUT_READ;					//	��ʱ
	}

	if (ucTemp != SD_TOK_READ_STARTBLOCK)			//	���ƴ���
	{
		SPI_SendByte(0xFF);
		SPI_CS_Deassert();
		return SD_ERR_DATA_START_TOK;
	}

	for (i = 0; i < BlockLen; i++)
		BlockBuf[i] = SPI_RecByte();				//	��������

#if SD_CRC_EN
	i = SPI_RecByte();
	i = (i << 8) + SPI_RecByte();					//	��ȡ16bit CRC

	if (i != SD_GetCRC16(BlockBuf, BlockLen))
	{												//	CRCУ�����
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
// �﷨��ʽ��	unsigned char SD_WriteBlockData(unsigned char Multi, unsigned long int BlockLen, unsigned char *BlockBuf)
// ʵ�ֹ��ܣ�	�����ݿ�
// ������		Multi		-	�Ƿ�Ϊ������
//				BlockLen	-	����
//				BlockBuf	-	Block�洢��������ַ
// ����ֵ��		��������
//=============================================================
unsigned char SD_WriteBlockData(unsigned char Multi, unsigned long int BlockLen, unsigned char *BlockBuf)
{
	unsigned int i;
	unsigned char ucTemp;

	SPI_CS_Assert();

	if (Multi == 1)
		SPI_SendByte(SD_TOK_WRITE_STARTBLOCK_M);	//	д�����ʼ����
	else
		SPI_SendByte(SD_TOK_WRITE_STARTBLOCK);		//	д������ʼ����

	for (i = 0; i < BlockLen; i++)
		SPI_SendByte(BlockBuf[i]);					//	��������

#if SD_CRC_EN	
	i = SD_GetCRC16(BlockBuf, BlockLen);

	SPI_SendByte((i >> 8) & 0xFF);
	SPI_SendByte(i & 0xFF);							//	����CRC
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
		return SD_ERR_TIMEOUT_WRITE;				//	��ʱ
	else
		return SD_NO_ERR;
}

//=============================================================
// �﷨��ʽ��	void SD_StopMultiToken(void)
// ʵ�ֹ��ܣ�	���Ͷ��дֹͣ����
// ������		��
// ����ֵ��		��
//=============================================================
void SD_StopMultiToken(void)
{
	SPI_CS_Assert();
	SPI_SendByte(0xFF);
	SPI_SendByte(SD_TOK_STOP_MULTI);				//	����ֹͣ���ݴ�������
	SPI_RecByte();
	SPI_CS_Deassert();
}

//=============================================================
// �﷨��ʽ��	unsigned char SD_WaitBusy(unsigned char Type)
// ʵ�ֹ��ܣ�	��ѯSD���Ƿ���æ״̬
// ������		Type	-	��ѯ����
// ����ֵ��		��
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
	{												//	�ȴ�����
		ucTemp = SPI_RecByte();
		i++;
	}while ((ucTemp != 0xFF) && (i < ulTimOut));	//	æ���յ�0xff
	SPI_CS_Deassert();

	if(i < ulTimOut) 
		return SD_NO_ERR;
	else 
		return SD_ERR_TIMEOUT_WAIT;
}

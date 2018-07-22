//======================================================
// �ļ����ƣ�	SD_Driver.c
// ����������	SD��������ļ�
// ά����¼��	2006-8-03	v1.0
//======================================================

#include "SD_INC.h"

SDINFO SDInfo;

const unsigned long int ulTimUnit[8] = {1000000000,100000000,10000000,	//	��ʱʱ�䵥λ��(0.000000001ns)
										1000000,100000,10000,1000,100};

const unsigned char ucTimValue[16] = {0,10,12,13,15,20,25,30,			//	��ʱʱ���
										35,40,45,50,55,60,70,80};

const unsigned char ucTimOutFator[6] = {1,2,4,8,16,32};					//	��ʱʱ��������

//=============================================================
// �﷨��ʽ��	unsigned char SD_Initialize(void)
// ʵ�ֹ��ܣ�	��ʼ��SD��
// ������		��
// ����ֵ��		��������
//=============================================================
unsigned char SD_Initialize(void)
{
	unsigned char ucRecBuf[4],ucRet;

	SD_IOInit();

	SPI_CS_Deassert();

	SD_SPIDelay(12);								//	���ٷ���74��SCLK
	ucRet = SD_ResetSD();							//	�����λSD��
	if (ucRet != SD_NO_ERR)
		return ucRet;
	ucRet = SD_ActiveInit();						//	SD���ڲ���ʼ��
	if (ucRet != SD_NO_ERR)
		return ucRet;
	ucRet = SD_ReadOCR(ucRecBuf);					//	��OCR�Ĵ�������ѯ֧�ֵĵ�ѹֵ
	if (ucRet != SD_NO_ERR)
		return ucRet;
    
	if ((ucRecBuf[1] & 0xc0) != 0xc0)
	{
		return SD_ERR_VOL_NOTSUSP;
    }
#if SD_CRC_EN        
	ucRet = SD_EnableCRC(1);						//	ʹ��CRCУ��
#else
	ucRet = SD_EnableCRC(0);						//	�ر�CRCУ��
#endif
	if (ucRet != SD_NO_ERR)
	{
		return ucRet;
	}
	ucRet = SD_SetBlockLen(SD_BLOCKSIZE);			//	����Block��С
	if (ucRet != SD_NO_ERR)
	{
		return ucRet;
	}
	return(SD_GetCardInfo());						//	��CSD�Ĵ�������ѯSD����Ϣ
}

//=============================================================
// �﷨��ʽ��	unsigned char SD_ReadBlock(unsigned long int BlockAddr, unsigned char *BlockBuf)
// ʵ�ֹ��ܣ�	��Block
// ������		BlockAddr	-	Block��ַ
//				BlockBuf	-	�洢Block���ݵĻ�������ַ
// ����ֵ��		��������
//=============================================================
unsigned char SD_ReadBlock(unsigned long int BlockAddr, unsigned char *BlockBuf)
{
	unsigned char ucRet;
	if (BlockAddr > SDInfo.BlockNum)
	{
		return SD_ERR_OVER_CARDRANGE;				//	��ַ����SD������
	}
	ucRet = SD_ReadSingleBlock(BlockAddr);			//	���Ͷ�Block����
	if (ucRet != SD_NO_ERR)
	{
		return ucRet;
	}
	return (SD_ReadBlockData(SD_BLOCKSIZE, BlockBuf));//	��������
}

//=============================================================
// �﷨��ʽ��	unsigned char SD_WriteBlock(unsigned long int BlockAddr, unsigned char *BlockBuf)
// ʵ�ֹ��ܣ�	дBlock
// ������		BlockAddr	-	Block��ַ
//				BlockBuf	-	�洢Block���ݵĻ�������ַ
// ����ֵ��		��������
//=============================================================
unsigned char SD_WriteBlock(unsigned long int BlockAddr, unsigned char *BlockBuf)
{
	unsigned char ucRet;
	unsigned char ucResp[2];


	if (BlockAddr > SDInfo.BlockNum)
	{
		return SD_ERR_OVER_CARDRANGE;				//	��ַ����SD������
	}
	
	if (SD_ChkCardWP() == 1)
	{
		return SD_ERR_WRITE_PROTECT;				//	����д����
	}
	
	ucRet = SD_WriteSingleBlock(BlockAddr);			//	дBlock����
	if (ucRet != SD_NO_ERR)
	{
		return ucRet;
	}
	
	ucRet = SD_WriteBlockData(0, SD_BLOCKSIZE, BlockBuf);//	д������
	if (ucRet == SD_NO_ERR)
	{
		ucRet = SD_ReadCard_Status(ucResp);			//	��Card Status�Ĵ���, ���д���Ƿ�ɹ�
		if (ucRet != SD_NO_ERR)
		{
			return ucRet;
		}

		if((ucResp[0] != 0) || (ucResp[1] != 0))
		{
			ucRet = SD_ERR_WRITE_BLK;
		}
	}
	return ucRet;
}

//=============================================================
// �﷨��ʽ��	unsigned char SD_EraseBlock(unsigned long int BlockStartAddr, unsigned long int BlockNum)
// ʵ�ֹ��ܣ�	����SD���еĿ�
// ������		BlockStartAddr	-	Block��ʼ��ַ
//				BlockNum		-	Block����
// ����ֵ��		��������
//=============================================================
#if SD_EraseBlock_EN
unsigned char SD_EraseBlock(unsigned long int BlockStartAddr, unsigned long int BlockNum)
{
	long int ulTemp;
	unsigned char ucRet;

	if ((BlockStartAddr + BlockNum) > SDInfo.BlockNum)	
	{
		return SD_ERR_OVER_CARDRANGE;				//	��ַ����SD������
	}
		
	if (SD_ChkCardWP() == 1)
	{
		return SD_ERR_WRITE_PROTECT;
	}

	ulTemp = BlockNum - SDInfo.EraseNum;	
	while(ulTemp >= 0)
	{
		ucRet = SD_EraseStartBlock(BlockStartAddr);//	ѡ����ʼBlock��ַ
		if (ucRet != SD_NO_ERR)
		{
			return ucRet;
		}

		ucRet = SD_EraseEndBlock(BlockStartAddr + SDInfo.EraseNum - 1);//	ѡ����ֹBlock��ַ
		if (ucRet != SD_NO_ERR)
		{
			return ucRet;
		}

		ucRet = SD_EraseSelectedBlock();			//	����ѡ�е�Block
		if (ucRet != SD_NO_ERR)
		{
			return ucRet;
		}

		BlockStartAddr += SDInfo.EraseNum;			//	��ʼ��ַ����
		BlockNum  -= SDInfo.EraseNum;
		ulTemp = BlockNum - SDInfo.EraseNum;
	}
	
	if (BlockNum > 0)								//	��������������С��Block
	{
		ucRet = SD_EraseStartBlock(BlockStartAddr);
		if (ucRet != SD_NO_ERR)
		{
			return ucRet;
		}

		ucRet = SD_EraseEndBlock(BlockStartAddr + BlockNum - 1);
		if (ucRet != SD_NO_ERR)
		{
			return ucRet;
		}

		ucRet = SD_EraseSelectedBlock();
		if (ucRet != SD_NO_ERR)
		{
			return ucRet;
		}
	}
	return SD_NO_ERR;
}
#endif

//=============================================================
// �﷨��ʽ��	unsigned char SD_GetCardInfo()
// ʵ�ֹ��ܣ�	���SD������Ϣ
// ������		��
// ����ֵ��		��������
//=============================================================
unsigned char SD_GetCardInfo()
{
	unsigned long int ulTemp;
	unsigned char ucCSDBuf[16],ucRet;

	ucRet = SD_ReadCSD(16,ucCSDBuf);				//	��CSD�Ĵ���
	if (ucRet != SD_NO_ERR)	
		return ucRet;	

	SD_CalTimeout(ucCSDBuf);						//	���㳬ʱʱ��ֵ

	SDInfo.BlockLen = 1 << (ucCSDBuf[5] & 0x0f);	//	��������󳤶�

	SDInfo.BlockNum = ((ucCSDBuf[6] & 0x03) << 10) +
					(ucCSDBuf[7] << 2) +
					((ucCSDBuf[8] & 0xc0) >> 6) + 1;//	���㿨�п�ĸ���

	ulTemp = ((ucCSDBuf[9] & 0x03) << 1) +
			((ucCSDBuf[10] & 0x80) >> 7) + 2;

	SDInfo.BlockNum = SDInfo.BlockNum * (1 << ulTemp);	//	��ÿ��п������

	SDInfo.EraseNum = ((ucCSDBuf[10] & 0x3f) << 1) +
					((ucCSDBuf[11] & 0x80) >> 7) + 1;//	����������С
	return SD_NO_ERR;
}

//=============================================================
// �﷨��ʽ��	void SD_CalTimeout(unsigned char *CSDBuf)
// ʵ�ֹ��ܣ�	�����/д/����ʱʱ��
// ������		CSDBuf	-	CSD�Ĵ�������
// ����ֵ��		��
//=============================================================
void SD_CalTimeout(unsigned char *CSDBuf)
{
	unsigned long int ulTemp;
	unsigned char ucTu, ucTv, ucF;

	SDInfo.rTimOut = SD_READ_TIMEOUT;			//	Ĭ�϶���ʱΪ100ms
	SDInfo.wTimOut = SD_WRITE_TIMEOUT;		//	Ĭ��д��ʱΪ250ms
	SDInfo.eTimOut = SD_WRITE_TIMEOUT;

	ucTu = (CSDBuf[1] & 0x07);				//	����ʱʱ�䵥λ
	ucTv = (CSDBuf[1] & 0x78) >> 3;			//	����ʱʱ��ֵ
	ucF = (CSDBuf[12] & 0x1c) >> 2;		//	д��ʱʱ������

	if(ucTv == 0) return;
	if(ucF >= 6) return;

	ulTemp = SPI_CLOCK * ucTimValue[ucTv] / 10 / ulTimUnit[ucTu];
	ulTemp = ulTemp + CSDBuf[2] * 100;

	//	����õ��ĳ�ʱֵ
	SDInfo.rTimOut = ulTemp;
	SDInfo.wTimOut = ulTemp * ucTimOutFator[ucF];

	SDInfo.rTimOut  = SDInfo.rTimOut * 100 / 8;	//	ʵ��ֵΪ����ֵ��100��
	SDInfo.wTimOut = SDInfo.wTimOut * 100 / 8;

	if (SDInfo.rTimOut > SD_READ_TIMEOUT)		//	ȡ����ֵ��Ĭ��ֵ�е���Сֵ
		SDInfo.rTimOut = SD_READ_TIMEOUT;

	if (SDInfo.wTimOut > SD_WRITE_TIMEOUT)
		SDInfo.wTimOut = SD_WRITE_TIMEOUT;

	SDInfo.eTimOut = SDInfo.wTimOut;
}

//=============================================================
// �﷨��ʽ��	unsigned char SD_ActiveInit(void)
// ʵ�ֹ��ܣ�	��ʼ����
// ������		��
// ����ֵ��		��������
//=============================================================
unsigned char SD_ActiveInit(void)
{
	unsigned char ucParam[4] = {0,0,0,0};
	unsigned char ucResp[5], ucRet;
	unsigned long int i = 0;

	do 
	{
		ucRet = SD_SendCmd(CMD1, ucParam, CMD1_R, ucResp);//	����CMD1��ѯ��״̬
		if (ucRet != SD_NO_ERR)
			return ucRet;
		i++;
	}while (((ucResp[0] & MSK_IDLE) == MSK_IDLE) && (i < SD_INIT_TIMEOUT));//	�ȴ���ʼ�����

	if (i >= SD_INIT_TIMEOUT)
		return SD_ERR_TIMEOUT_WAITIDLE;				//	��ʱ

	return SD_NO_ERR;
}


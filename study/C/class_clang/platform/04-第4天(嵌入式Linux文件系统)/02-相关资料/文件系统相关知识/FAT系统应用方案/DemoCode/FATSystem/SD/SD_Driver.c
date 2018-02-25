//======================================================
// 文件名称：	SD_Driver.c
// 功能描述：	SD卡组件层文件
// 维护记录：	2006-8-03	v1.0
//======================================================

#include "SD_INC.h"

SDINFO SDInfo;

const unsigned long int ulTimUnit[8] = {1000000000,100000000,10000000,	//	超时时间单位表(0.000000001ns)
										1000000,100000,10000,1000,100};

const unsigned char ucTimValue[16] = {0,10,12,13,15,20,25,30,			//	超时时间表
										35,40,45,50,55,60,70,80};

const unsigned char ucTimOutFator[6] = {1,2,4,8,16,32};					//	超时时间因数表

//=============================================================
// 语法格式：	unsigned char SD_Initialize(void)
// 实现功能：	初始化SD卡
// 参数：		无
// 返回值：		错误类型
//=============================================================
unsigned char SD_Initialize(void)
{
	unsigned char ucRecBuf[4],ucRet;

	SD_IOInit();

	SPI_CS_Deassert();

	SD_SPIDelay(12);								//	至少发送74个SCLK
	ucRet = SD_ResetSD();							//	软件复位SD卡
	if (ucRet != SD_NO_ERR)
		return ucRet;
	ucRet = SD_ActiveInit();						//	SD卡内部初始化
	if (ucRet != SD_NO_ERR)
		return ucRet;
	ucRet = SD_ReadOCR(ucRecBuf);					//	读OCR寄存器，查询支持的电压值
	if (ucRet != SD_NO_ERR)
		return ucRet;
    
	if ((ucRecBuf[1] & 0xc0) != 0xc0)
	{
		return SD_ERR_VOL_NOTSUSP;
    }
#if SD_CRC_EN        
	ucRet = SD_EnableCRC(1);						//	使能CRC校验
#else
	ucRet = SD_EnableCRC(0);						//	关闭CRC校验
#endif
	if (ucRet != SD_NO_ERR)
	{
		return ucRet;
	}
	ucRet = SD_SetBlockLen(SD_BLOCKSIZE);			//	设置Block大小
	if (ucRet != SD_NO_ERR)
	{
		return ucRet;
	}
	return(SD_GetCardInfo());						//	读CSD寄存器，查询SD卡信息
}

//=============================================================
// 语法格式：	unsigned char SD_ReadBlock(unsigned long int BlockAddr, unsigned char *BlockBuf)
// 实现功能：	读Block
// 参数：		BlockAddr	-	Block地址
//				BlockBuf	-	存储Block数据的缓冲区首址
// 返回值：		错误类型
//=============================================================
unsigned char SD_ReadBlock(unsigned long int BlockAddr, unsigned char *BlockBuf)
{
	unsigned char ucRet;
	if (BlockAddr > SDInfo.BlockNum)
	{
		return SD_ERR_OVER_CARDRANGE;				//	地址超出SD卡容量
	}
	ucRet = SD_ReadSingleBlock(BlockAddr);			//	发送读Block命令
	if (ucRet != SD_NO_ERR)
	{
		return ucRet;
	}
	return (SD_ReadBlockData(SD_BLOCKSIZE, BlockBuf));//	读出数据
}

//=============================================================
// 语法格式：	unsigned char SD_WriteBlock(unsigned long int BlockAddr, unsigned char *BlockBuf)
// 实现功能：	写Block
// 参数：		BlockAddr	-	Block地址
//				BlockBuf	-	存储Block数据的缓冲区首址
// 返回值：		错误类型
//=============================================================
unsigned char SD_WriteBlock(unsigned long int BlockAddr, unsigned char *BlockBuf)
{
	unsigned char ucRet;
	unsigned char ucResp[2];


	if (BlockAddr > SDInfo.BlockNum)
	{
		return SD_ERR_OVER_CARDRANGE;				//	地址超出SD卡容量
	}
	
	if (SD_ChkCardWP() == 1)
	{
		return SD_ERR_WRITE_PROTECT;				//	卡有写保护
	}
	
	ucRet = SD_WriteSingleBlock(BlockAddr);			//	写Block命令
	if (ucRet != SD_NO_ERR)
	{
		return ucRet;
	}
	
	ucRet = SD_WriteBlockData(0, SD_BLOCKSIZE, BlockBuf);//	写入数据
	if (ucRet == SD_NO_ERR)
	{
		ucRet = SD_ReadCard_Status(ucResp);			//	读Card Status寄存器, 检查写入是否成功
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
// 语法格式：	unsigned char SD_EraseBlock(unsigned long int BlockStartAddr, unsigned long int BlockNum)
// 实现功能：	擦除SD卡中的块
// 参数：		BlockStartAddr	-	Block起始地址
//				BlockNum		-	Block数量
// 返回值：		错误类型
//=============================================================
#if SD_EraseBlock_EN
unsigned char SD_EraseBlock(unsigned long int BlockStartAddr, unsigned long int BlockNum)
{
	long int ulTemp;
	unsigned char ucRet;

	if ((BlockStartAddr + BlockNum) > SDInfo.BlockNum)	
	{
		return SD_ERR_OVER_CARDRANGE;				//	地址超出SD卡容量
	}
		
	if (SD_ChkCardWP() == 1)
	{
		return SD_ERR_WRITE_PROTECT;
	}

	ulTemp = BlockNum - SDInfo.EraseNum;	
	while(ulTemp >= 0)
	{
		ucRet = SD_EraseStartBlock(BlockStartAddr);//	选择起始Block地址
		if (ucRet != SD_NO_ERR)
		{
			return ucRet;
		}

		ucRet = SD_EraseEndBlock(BlockStartAddr + SDInfo.EraseNum - 1);//	选择终止Block地址
		if (ucRet != SD_NO_ERR)
		{
			return ucRet;
		}

		ucRet = SD_EraseSelectedBlock();			//	擦除选中的Block
		if (ucRet != SD_NO_ERR)
		{
			return ucRet;
		}

		BlockStartAddr += SDInfo.EraseNum;			//	起始地址递增
		BlockNum  -= SDInfo.EraseNum;
		ulTemp = BlockNum - SDInfo.EraseNum;
	}
	
	if (BlockNum > 0)								//	擦除不够扇区大小的Block
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
// 语法格式：	unsigned char SD_GetCardInfo()
// 实现功能：	获得SD卡的信息
// 参数：		无
// 返回值：		错误类型
//=============================================================
unsigned char SD_GetCardInfo()
{
	unsigned long int ulTemp;
	unsigned char ucCSDBuf[16],ucRet;

	ucRet = SD_ReadCSD(16,ucCSDBuf);				//	读CSD寄存器
	if (ucRet != SD_NO_ERR)	
		return ucRet;	

	SD_CalTimeout(ucCSDBuf);						//	计算超时时间值

	SDInfo.BlockLen = 1 << (ucCSDBuf[5] & 0x0f);	//	计算块的最大长度

	SDInfo.BlockNum = ((ucCSDBuf[6] & 0x03) << 10) +
					(ucCSDBuf[7] << 2) +
					((ucCSDBuf[8] & 0xc0) >> 6) + 1;//	计算卡中块的个数

	ulTemp = ((ucCSDBuf[9] & 0x03) << 1) +
			((ucCSDBuf[10] & 0x80) >> 7) + 2;

	SDInfo.BlockNum = SDInfo.BlockNum * (1 << ulTemp);	//	获得卡中块的数量

	SDInfo.EraseNum = ((ucCSDBuf[10] & 0x3f) << 1) +
					((ucCSDBuf[11] & 0x80) >> 7) + 1;//	计算扇区大小
	return SD_NO_ERR;
}

//=============================================================
// 语法格式：	void SD_CalTimeout(unsigned char *CSDBuf)
// 实现功能：	计算读/写/擦超时时间
// 参数：		CSDBuf	-	CSD寄存器内容
// 返回值：		无
//=============================================================
void SD_CalTimeout(unsigned char *CSDBuf)
{
	unsigned long int ulTemp;
	unsigned char ucTu, ucTv, ucF;

	SDInfo.rTimOut = SD_READ_TIMEOUT;			//	默认读超时为100ms
	SDInfo.wTimOut = SD_WRITE_TIMEOUT;		//	默认写超时为250ms
	SDInfo.eTimOut = SD_WRITE_TIMEOUT;

	ucTu = (CSDBuf[1] & 0x07);				//	读超时时间单位
	ucTv = (CSDBuf[1] & 0x78) >> 3;			//	读超时时间值
	ucF = (CSDBuf[12] & 0x1c) >> 2;		//	写超时时间因数

	if(ucTv == 0) return;
	if(ucF >= 6) return;

	ulTemp = SPI_CLOCK * ucTimValue[ucTv] / 10 / ulTimUnit[ucTu];
	ulTemp = ulTemp + CSDBuf[2] * 100;

	//	计算得到的超时值
	SDInfo.rTimOut = ulTemp;
	SDInfo.wTimOut = ulTemp * ucTimOutFator[ucF];

	SDInfo.rTimOut  = SDInfo.rTimOut * 100 / 8;	//	实际值为计算值的100倍
	SDInfo.wTimOut = SDInfo.wTimOut * 100 / 8;

	if (SDInfo.rTimOut > SD_READ_TIMEOUT)		//	取计算值与默认值中的最小值
		SDInfo.rTimOut = SD_READ_TIMEOUT;

	if (SDInfo.wTimOut > SD_WRITE_TIMEOUT)
		SDInfo.wTimOut = SD_WRITE_TIMEOUT;

	SDInfo.eTimOut = SDInfo.wTimOut;
}

//=============================================================
// 语法格式：	unsigned char SD_ActiveInit(void)
// 实现功能：	初始化卡
// 参数：		无
// 返回值：		错误类型
//=============================================================
unsigned char SD_ActiveInit(void)
{
	unsigned char ucParam[4] = {0,0,0,0};
	unsigned char ucResp[5], ucRet;
	unsigned long int i = 0;

	do 
	{
		ucRet = SD_SendCmd(CMD1, ucParam, CMD1_R, ucResp);//	发送CMD1查询卡状态
		if (ucRet != SD_NO_ERR)
			return ucRet;
		i++;
	}while (((ucResp[0] & MSK_IDLE) == MSK_IDLE) && (i < SD_INIT_TIMEOUT));//	等待初始化完成

	if (i >= SD_INIT_TIMEOUT)
		return SD_ERR_TIMEOUT_WAITIDLE;				//	超时

	return SD_NO_ERR;
}


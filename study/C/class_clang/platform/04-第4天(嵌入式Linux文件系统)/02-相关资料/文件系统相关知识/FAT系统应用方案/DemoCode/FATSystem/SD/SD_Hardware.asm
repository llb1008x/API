//======================================================
// 文件名称：	SD_Hardware.c
// 功能描述：	SD底层驱动文件
// 维护记录：	2006-8-03	v1.0
//======================================================

.include SD_CFG.inc
.code
//=============================================================
// 语法格式：	void SPI_SCK_SET(void)
// 实现功能：	拉高SCLK
// 参数：		无
// 返回值：		无
//=============================================================
SPI_SCK_SET:	.macro
	r4 = [SD_BUS_Buffer]
	r4 |= SD_BUS_SCK
	[SD_BUS_Buffer] = r4
	.endm

//=============================================================
// 语法格式：	void SPI_SCK_CLR(void)
// 实现功能：	拉低SCLK
// 参数：		无
// 返回值：		无
//=============================================================
SPI_SCK_CLR:	.macro
	r4 = [SD_BUS_Buffer]
	r4 &= ~SD_BUS_SCK
	[SD_BUS_Buffer] = r4
	.endm

//=============================================================
// 语法格式：	void SPI_DOUT_SET(void)
// 实现功能：	拉高Dout
// 参数：		无
// 返回值：		无
//=============================================================
SPI_DOUT_SET:	.macro
	r4 = [SD_BUS_Buffer]
	r4 |= SD_BUS_Dout
	[SD_BUS_Buffer] = r4
	.endm

//=============================================================
// 语法格式：	void SPI_DOUT_CLR(void)
// 实现功能：	拉低 Dout
// 参数：		无
// 返回值：		无
//=============================================================
SPI_DOUT_CLR:	.macro
	r4 = [SD_BUS_Buffer]
	r4 &= ~SD_BUS_Dout
	[SD_BUS_Buffer] = r4
	.endm

//=============================================================
// 语法格式：	void SD_IOInit(void)
// 实现功能：	SD端口初始化
// 参数：		无
// 返回值：		无
//=============================================================
.public _SD_IOInit
_SD_IOInit:	.proc
	push r1 to [sp]
	r1 = [SD_BUS_Dir]
	r1 |= SD_BUS_Dout + SD_BUS_SCK + SD_BUS_CS
	r1 &= ~(SD_BUS_CardDetect + SD_BUS_WriteProtect + SD_BUS_Din)
	[SD_BUS_Dir] = r1
	r1 = [SD_BUS_Attrib]
	r1 |= SD_BUS_Dout + SD_BUS_SCK + SD_BUS_CS + SD_BUS_Din
	r1 &= ~(SD_BUS_CardDetect + SD_BUS_WriteProtect)
	[SD_BUS_Attrib] = r1
	r1 = [SD_BUS_Buffer]
	r1 |= SD_BUS_Dout + SD_BUS_SCK + SD_BUS_CS + SD_BUS_CardDetect + SD_BUS_WriteProtect + SD_BUS_Din
	[SD_BUS_Buffer] = r1
	pop r1 from [sp]
	retf
	.endp

//=============================================================
// 语法格式：	void SPI_SendByte(unsigned char ucByte)
// 实现功能：	通过SPI总线发送一个字节
// 参数：		ucByte	-	要发送的数据
// 返回值：		无
//=============================================================
.public _SPI_SendByte
_SPI_SendByte:	.proc
	r1 = sp + 3
	r1 = [r1]
	r2 = 8
?L_SPI_SendByte_Lop:
	test r1, 0x0080
	jz ?L_SPI_SendByte_Bit8_0
	SPI_DOUT_SET
	jmp ?L_SPI_SendByte_Bit8_End
?L_SPI_SendByte_Bit8_0:
	SPI_DOUT_CLR
?L_SPI_SendByte_Bit8_End:
	SPI_SCK_CLR
	SPI_SCK_SET
	r1 = r1 lsl 1
	r2 -= 1
	jnz ?L_SPI_SendByte_Lop
	retf
	.endp

//=============================================================
// 语法格式：	unsigned char SPI_RecByte(void)
// 实现功能：	通过SPI总线接收一个字节
// 参数：		无
// 返回值：		接收到的数据
//=============================================================
.public _SPI_RecByte
_SPI_RecByte:	.proc
	r1 = 0
	r2 = 8
?L_SPI_RecByte_Loop:
	SPI_SCK_CLR
	SPI_SCK_SET
	r1 = r1 lsl 1
	r3 = [SD_BUS_Data]
	test r3, SD_BUS_Din
	jz ?L_SPI_RecByte_NoAdd
	r1 += 1
?L_SPI_RecByte_NoAdd:
//	SPI_SCK_CLR
	r2 -= 1
	jnz ?L_SPI_RecByte_Loop
	retf
	.endp

//=============================================================
// 语法格式：	unsigned char SD_ChkCard(void)
// 实现功能：	检测卡是否完全插入
// 参数：		无
// 返回值：		1:	插入；	0:	未插入
//=============================================================
.public _SD_ChkCard
_SD_ChkCard:	.proc
	r1 = 1
	r2 = [SD_BUS_Data]
	test r2, SD_BUS_CardDetect
	jz ?L_SD_ChkCard_Exit
	r1 = 0
?L_SD_ChkCard_Exit:
	retf
	.endp

//=============================================================
// 语法格式：	unsigned char SD_ChkCardWP(void)
// 实现功能：	检测卡是否有写保护
// 参数：		无
// 返回值：		1:	有写保护；	0:	没有写保护
//=============================================================
.public _SD_ChkCardWP
_SD_ChkCardWP:	.proc
	r1 = 0
	r2 = [SD_BUS_Data]
	test r2, SD_BUS_WriteProtect
	jz ?L_SD_ChkCardWP_Exit
	r1 = 1
?L_SD_ChkCardWP_Exit:
	retf
	.endp

//=============================================================
// 语法格式：	void SPI_CS_Assert(void)
// 实现功能：	拉低CS
// 参数：		无
// 返回值：		无
//=============================================================
.public _SPI_CS_Assert
_SPI_CS_Assert:	.proc
	r4 = [SD_BUS_Buffer]
	r4 &= ~SD_BUS_CS
	[SD_BUS_Buffer] = r4
	retf
	.endp

//=============================================================
// 语法格式：	void SPI_CS_Deassert(void)
// 实现功能：	拉高CS
// 参数：		无
// 返回值：		无
//=============================================================
.public _SPI_CS_Deassert
_SPI_CS_Deassert:	.proc
	r4 = [SD_BUS_Buffer]
	r4 |= SD_BUS_CS
	[SD_BUS_Buffer] = r4
	retf
	.endp

//=============================================================
// 语法格式：	void SD_SPIDelay(unsigned char ucTim)
// 实现功能：	SPI总线延时
// 参数：		ucTim	-	延时时长
// 返回值：		无
//=============================================================
.public _SD_SPIDelay
_SD_SPIDelay:	.proc
	r1 = sp + 3
	r1 = [r1]
	jz ?L_SD_SPIDelay_Exit
	SPI_DOUT_SET
?L_SD_SPIDelay_Loop_o:
	r2 = 8
?L_SD_SPIDelay_Loop_i:
	SPI_SCK_CLR
	SPI_SCK_SET
	r2 -= 1
	jnz ?L_SD_SPIDelay_Loop_i
	r1 -= 1
	jnz ?L_SD_SPIDelay_Loop_o
?L_SD_SPIDelay_Exit:
	retf
	.endp

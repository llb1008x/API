//======================================================
// �ļ����ƣ�	SD_Hardware.c
// ����������	SD�ײ������ļ�
// ά����¼��	2006-8-03	v1.0
//======================================================

.include SD_CFG.inc
.code
//=============================================================
// �﷨��ʽ��	void SPI_SCK_SET(void)
// ʵ�ֹ��ܣ�	����SCLK
// ������		��
// ����ֵ��		��
//=============================================================
SPI_SCK_SET:	.macro
	r4 = [SD_BUS_Buffer]
	r4 |= SD_BUS_SCK
	[SD_BUS_Buffer] = r4
	.endm

//=============================================================
// �﷨��ʽ��	void SPI_SCK_CLR(void)
// ʵ�ֹ��ܣ�	����SCLK
// ������		��
// ����ֵ��		��
//=============================================================
SPI_SCK_CLR:	.macro
	r4 = [SD_BUS_Buffer]
	r4 &= ~SD_BUS_SCK
	[SD_BUS_Buffer] = r4
	.endm

//=============================================================
// �﷨��ʽ��	void SPI_DOUT_SET(void)
// ʵ�ֹ��ܣ�	����Dout
// ������		��
// ����ֵ��		��
//=============================================================
SPI_DOUT_SET:	.macro
	r4 = [SD_BUS_Buffer]
	r4 |= SD_BUS_Dout
	[SD_BUS_Buffer] = r4
	.endm

//=============================================================
// �﷨��ʽ��	void SPI_DOUT_CLR(void)
// ʵ�ֹ��ܣ�	���� Dout
// ������		��
// ����ֵ��		��
//=============================================================
SPI_DOUT_CLR:	.macro
	r4 = [SD_BUS_Buffer]
	r4 &= ~SD_BUS_Dout
	[SD_BUS_Buffer] = r4
	.endm

//=============================================================
// �﷨��ʽ��	void SD_IOInit(void)
// ʵ�ֹ��ܣ�	SD�˿ڳ�ʼ��
// ������		��
// ����ֵ��		��
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
// �﷨��ʽ��	void SPI_SendByte(unsigned char ucByte)
// ʵ�ֹ��ܣ�	ͨ��SPI���߷���һ���ֽ�
// ������		ucByte	-	Ҫ���͵�����
// ����ֵ��		��
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
// �﷨��ʽ��	unsigned char SPI_RecByte(void)
// ʵ�ֹ��ܣ�	ͨ��SPI���߽���һ���ֽ�
// ������		��
// ����ֵ��		���յ�������
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
// �﷨��ʽ��	unsigned char SD_ChkCard(void)
// ʵ�ֹ��ܣ�	��⿨�Ƿ���ȫ����
// ������		��
// ����ֵ��		1:	���룻	0:	δ����
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
// �﷨��ʽ��	unsigned char SD_ChkCardWP(void)
// ʵ�ֹ��ܣ�	��⿨�Ƿ���д����
// ������		��
// ����ֵ��		1:	��д������	0:	û��д����
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
// �﷨��ʽ��	void SPI_CS_Assert(void)
// ʵ�ֹ��ܣ�	����CS
// ������		��
// ����ֵ��		��
//=============================================================
.public _SPI_CS_Assert
_SPI_CS_Assert:	.proc
	r4 = [SD_BUS_Buffer]
	r4 &= ~SD_BUS_CS
	[SD_BUS_Buffer] = r4
	retf
	.endp

//=============================================================
// �﷨��ʽ��	void SPI_CS_Deassert(void)
// ʵ�ֹ��ܣ�	����CS
// ������		��
// ����ֵ��		��
//=============================================================
.public _SPI_CS_Deassert
_SPI_CS_Deassert:	.proc
	r4 = [SD_BUS_Buffer]
	r4 |= SD_BUS_CS
	[SD_BUS_Buffer] = r4
	retf
	.endp

//=============================================================
// �﷨��ʽ��	void SD_SPIDelay(unsigned char ucTim)
// ʵ�ֹ��ܣ�	SPI������ʱ
// ������		ucTim	-	��ʱʱ��
// ����ֵ��		��
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

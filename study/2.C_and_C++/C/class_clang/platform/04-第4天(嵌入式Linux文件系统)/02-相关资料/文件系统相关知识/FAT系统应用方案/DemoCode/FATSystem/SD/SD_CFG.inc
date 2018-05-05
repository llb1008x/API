//======================================================
// 文件名称：	SD_Driver.c
// 功能描述：	SD卡配置文件
// 维护记录：	2006-8-03	v1.0
//======================================================
.ifndef __SD_CFG_H__
.define __SD_CFG_H__

.include spce061a.inc

// 下面为SD卡使用的硬件条件配置,用户可根据实际需要修改
.define PortA	1
.define PortB	0

//	使用IOB请修改为
//.define C_SPI_Port		PortB
.define C_SPI_Port		PortB	//PortB

//定义各控制线
//.define SD_BUS_CardDetect	C_Bit7
//.define SD_BUS_WriteProtect	C_Bit6
//.define SD_BUS_Dout			C_Bit10
//.define SD_BUS_Din			C_Bit11
//.define SD_BUS_SCK			C_Bit9
//.define SD_BUS_CS			C_Bit8

.define SD_BUS_CardDetect	C_Bit4
.define SD_BUS_WriteProtect	C_Bit5
.define SD_BUS_Dout			C_Bit1
.define SD_BUS_Din			C_Bit3
.define SD_BUS_SCK			C_Bit2
.define SD_BUS_CS			C_Bit0

.if C_SPI_Port
	.define SD_BUS_Dir P_IOA_Dir
	.define SD_BUS_Attrib P_IOA_Attrib
	.define SD_BUS_Buffer P_IOA_Buffer
	.define SD_BUS_Data P_IOA_Data
.else
	.define SD_BUS_Dir P_IOB_Dir
	.define SD_BUS_Attrib P_IOB_Attrib
	.define SD_BUS_Buffer P_IOB_Buffer
	.define SD_BUS_Data P_IOB_Data
.endif

.endif
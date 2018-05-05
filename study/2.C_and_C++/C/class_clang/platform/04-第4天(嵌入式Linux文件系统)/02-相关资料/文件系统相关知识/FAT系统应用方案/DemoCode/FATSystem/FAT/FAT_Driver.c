//=================================================================
// 文件名称：	FAT_Driver.c
// 功能描述：	FAT文件系统与SD接口程序
// 维护记录：	2006-8-11	v1.0
//==================================================================

#define FAT_Dir
#include "FAT.h"
#include "SD\SD_INC.h"

//===============================================================================
// 语法格式:	unsigned long int GetVolumeFirstSect(void)
// 实现功能:	获取首扇区
// 参数:		无
// 返回值:		卷首扇区地址
//===============================================================================
unsigned long int GetVolumeFirstSect(void)
{
	unsigned char buffer[512];
	unsigned long int RelaStaSect;
	SD_ReadBlock(0, buffer);						// 读SD卡的0块 */
	if((buffer[510]==0x55)&&(buffer[511]==0xAA))
	{
		if(((buffer[0]==0xEB)&&(buffer[2]==0x90))||(buffer[0] == 0xE9))
		{
			RelaStaSect = 0;
		}
		else
		{
			RelaStaSect = buffer[454]+ buffer[455]*0x100 + buffer[456]*0x10000 + buffer[457]*0x1000000;
		}
	}
	return RelaStaSect;	
}

//=======================================================================================
// 语法格式:	unsigned int SDCammand(unsigned char Cammand, void *Parameter)
// 实现功能:	底层驱动程序与上层的接口程序
// 参数:		DISK_INIT	-	驱动程序初始化
//				DISK_CLOSE	-	关闭驱动器（移除驱动程序）                 
//				DISK_CREATE_BOOT_SECTOR	-	重建引导扇区
//				DISK_READ_SECTOR	-	读扇区
//				DISK_WRITE_SECTOR	-	写扇区
//				Parameter	-	剩余参数
// 返回值:		DISK_READ_OK	-	读扇区完成
//				DISK_READ_NOT_OK	-	读扇区失败
//				DISK_WRITE_OK	-	写扇区完成
//				DISK_WRITE_NOT_OK	-	写扇区失败
//				DISK_INIT_OK	-	初始化完成
//				DISK_INIT_NOT_OK	-	初始化失败
//				BAD_DISK_COMMAND	-	无效的命令
//========================================================================================
unsigned int SDCammand(unsigned char Cammand, void *Parameter)
{
	unsigned char ret;
	unsigned int rt;
	Disk_RW_Parameter *Dp;
	Disk_Info *DiskInfo;
	Dp = (Disk_RW_Parameter *)Parameter;
	switch (Cammand)                                        
	{
		case DISK_INIT:			// 设备初始化
			rt = DISK_INIT_NOT_OK;
			ret = SD_Initialize();
			if (ret == SD_NO_ERR)
			{
				DiskInfo = GetEmptyDiskInfoAddr();
				if (DiskInfo != NULL)
				{
					DiskInfo->DiakCommand = SDCammand;
					DiskInfo->RsvdForLow = GetVolumeFirstSect();// 获取卷首扇区
					rt = DISK_INIT_OK;
				}
			}
			break;
            
		case DISK_CLOSE:		
			rt = RETURN_OK;
			break;
            
		case DISK_READ_SECTOR:	// 读扇区
			rt = DISK_READ_NOT_OK;
			if(SD_ReadBlock(Dp->SectorIndex + Dp->RsvdForLow,Dp->Buf) == SD_NO_ERR)
				rt = DISK_READ_OK;
			break;
            
		case DISK_WRITE_SECTOR:	// 写扇区
			rt = DISK_WRITE_NOT_OK;
			if(SD_WriteBlock(Dp->SectorIndex + Dp->RsvdForLow,Dp->Buf) == SD_NO_ERR)
				rt = DISK_WRITE_OK;
			break;
		default:
			rt = BAD_DISK_COMMAND;
			break;
	}
	return rt;
}

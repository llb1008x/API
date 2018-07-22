//======================================================
// 文件名称：	FAT_Disk.c
// 功能描述：	FAT文件系统关于逻辑盘管理的部分
// 维护记录：	2006-8-11	v1.0
//======================================================

#define FAT_Dir
#define FAT_Disk
#include "FAT.h"
#include "SD\SD_INC.h"

Disk_Info DiskInfo[MAX_DRIVES];     // 逻辑盘信息

//============================================
// 语法格式:	void DiskInit(void)
// 实现功能:	初始化逻辑盘管理模块
// 参数:		无
// 返回值:		无
//============================================
void DiskInit(void)
{
	unsigned char i;
    
	for (i = 0; i < MAX_DRIVES; i++)
	{
		DiskInfo[i].Drive = EMPTY_DRIVE;    // 没有分配
		DiskInfo[i].FATType = 0xff;         // 无效的文件系统
		DiskInfo[i].SecPerClus = 0;         // 每簇扇区数
		DiskInfo[i].NumFATs = 0;            // FAT表数
		DiskInfo[i].SecPerDisk = 0xffffffff;// 逻辑驱动器包含扇区数
		DiskInfo[i].BytsPerSec = 0;         // 每扇区字节数
		DiskInfo[i].RootDirTable = 0;       // 根目录开始扇区号（FAT32为开始簇号）
		DiskInfo[i].RootSecCnt = 0;         // 根目录占用扇区数
		DiskInfo[i].FATStartSec = 0;        // FAT表开始扇区号
		DiskInfo[i].FATSecCnt = 0;          // 每个FAT占用扇区数
		DiskInfo[i].DataStartSec = 0;       // 数据区开始扇区号
		DiskInfo[i].PathClusIndex = 0;      // 当前目录
		DiskInfo[i].DiakCommand = NULL;     // 驱动程序
	}
	CacheInit();                			// 初始化磁盘cache
}

//===============================================================
// 语法格式:	Disk_Info *GetDiskInfo(unsigned char Drive)
// 实现功能:	获取指定逻辑盘信息
// 参数:		逻辑盘号
// 返回值:		指向逻辑盘信息的结构体指针
//================================================================        
Disk_Info *GetDiskInfo(unsigned char Drive)
{
	Disk_Info *Rt;
	Rt = NULL;
	if (Drive < MAX_DRIVES)
	{
		if (DiskInfo[Drive].Drive != EMPTY_DRIVE)     
		{
			Rt = DiskInfo + Drive;   
		}
	}
    return Rt;
}

//==========================================================
// 语法格式:	Disk_Info *GetEmptyDiskInfoAddr(void)
// 实现功能:	获取空闲逻辑盘
// 参数:		无
// 返回值:		指向逻辑盘信息的结构体指针
//===========================================================
Disk_Info *GetEmptyDiskInfoAddr(void)
{
	unsigned char i;
	Disk_Info *Rt;
    
	Rt = NULL;
	for (i = 0; i < MAX_DRIVES; i++)
	{
		if (DiskInfo[i].Drive == EMPTY_DRIVE)     // 逻辑盘号空闲
		{
			Rt = DiskInfo + i;
			DiskInfo[i].Drive = i;
			break;
		}
	}
	return Rt;
}

//===========================================================================================================
// 语法格式:	void AddFileDriver(unsigned int  (* DiakCommand)(unsigned char Cammand, void *Parameter))
// 实现功能:	增加一个底层驱动程序
// 参数:		DiakCommand	-	驱动程序接口函数
// 返回值:		无
//============================================================================================================
void AddFileDriver(unsigned int  (* DiakCommand)(unsigned char Cammand, void *Parameter))
{
	unsigned char i;
	unsigned char *Buf;
	unsigned int temp;
	unsigned long int temp1;
    
	if (DiakCommand == NULL)        // 参数无效退出
	{
		return;
	}

	if (DiakCommand(DISK_INIT, NULL) == DISK_INIT_NOT_OK)   // DISK_INIT=0,底层驱动初始化失败退出
	{
		return;
	}
    
    // 重新获取逻辑盘信息
	for (i = 0; i < MAX_DRIVES; i++)
	{
		if (DiskInfo[i].Drive != EMPTY_DRIVE)       // 逻辑盘已分配
		{
			Buf = OpenSec(DiskInfo[i].Drive, 0);    // 打开扇区0（保留扇区）,返回指向指定扇区数据的指针
			ReadSec(DiskInfo[i].Drive, 0);
			if (Buf != NULL)
			{
				// 每扇区字节数
				DiskInfo[i].BytsPerSec = Buf[11] | (Buf[12] << 8);
				// 每簇扇区数
				DiskInfo[i].SecPerClus = Buf[13];
				// FAT开始扇区号
				DiskInfo[i].FATStartSec = Buf[14] | (Buf[15] << 8);
				// FAT表个数
				DiskInfo[i].NumFATs = Buf[16];
				// 根目录占扇区数 
				DiskInfo[i].RootSecCnt = ((Buf[17] | (Buf[18] << 8)) * 32 + DiskInfo[i].BytsPerSec - 1) / DiskInfo[i].BytsPerSec;
				// 逻辑盘（卷）占扇区数
				temp = Buf[19] | (Buf[20] << 8);
				if (temp != 0)
				{
					DiskInfo[i].SecPerDisk = temp;
				}
				else
				{
					DiskInfo[i].SecPerDisk = Buf[32] | (Buf[33] << 8);
					DiskInfo[i].SecPerDisk |= (((unsigned long int)Buf[34] << 16) | ((unsigned long int)Buf[35] << 24));
				}
				// FAT表占用扇区数
				temp = Buf[22] | (Buf[23] << 8);
				if (temp != 0)
				{
					DiskInfo[i].FATSecCnt = temp;
				}
				else
				{
					DiskInfo[i].FATSecCnt = Buf[36] | (Buf[37] << 8);
					DiskInfo[i].FATSecCnt |= (((unsigned long int)Buf[38] << 16) | ((unsigned long int)Buf[39] << 24));
				}
                
				// 当前目录为根目录
				DiskInfo[i].PathClusIndex = 0;
			}
			// 根目录开始扇区号
			DiskInfo[i].RootDirTable = DiskInfo[i].NumFATs * DiskInfo[i].FATSecCnt + DiskInfo[i].FATStartSec;
			// 数据区开始扇区号
			DiskInfo[i].DataStartSec = DiskInfo[i].RootDirTable + DiskInfo[i].RootSecCnt;
			temp1 = DiskInfo[i].SecPerDisk - DiskInfo[i].DataStartSec;
			temp1 = temp1 / DiskInfo[i].SecPerClus;
			DiskInfo[i].ClusPerData = temp1;
                    
			// 判断是FAT12、FAT16还是FAT32
			if (temp1 < 4085)
			{
				DiskInfo[i].FATType = FAT12;
            }
			else if (temp1 < 65525)
			{
				DiskInfo[i].FATType = FAT16;
			}
			else
			{
				DiskInfo[i].FATType = FAT32;
                // FAT32 RootDirTable为开始簇号
				DiskInfo[i].RootDirTable = Buf[44] | (Buf[45] << 8);
				DiskInfo[i].RootDirTable |= (((unsigned long int)Buf[46] << 16) | ((unsigned long int)Buf[47] << 24));
				DiskInfo[i].PathClusIndex = DiskInfo[i].RootDirTable;
			}
			CloseSec(DiskInfo[i].Drive, 0);     // 关闭扇区
		}
	}
}

//=============================================================
// 语法格式: 	void RemoveFileDriver(unsigned char Drive)
// 实现功能:	删除一个底层驱动程序
// 参数:		Drive:逻辑驱动器号
// 返回值:		无
//=============================================================
void RemoveFileDriver(unsigned char Drive)
{
	Disk_RW_Parameter Pa;
    
	if (Drive < MAX_DRIVES)
	{
		if (DiskInfo[Drive].Drive != EMPTY_DRIVE)
		{
			AllCacheWriteBack();
			Pa.Drive = Drive;
 			Pa.RsvdForLow = DiskInfo[Drive].RsvdForLow;
			DiskInfo[Drive].DiakCommand(DISK_CLOSE, &Pa);

			DiskInfo[Drive].Drive = EMPTY_DRIVE;    // 没有分配
			DiskInfo[Drive].FATType = 0xff;         // 无效的文件系统
			DiskInfo[Drive].SecPerClus = 0;         // 每簇扇区数
			DiskInfo[Drive].NumFATs = 0;            // FAT表数
			DiskInfo[Drive].SecPerDisk = 0xffffffff;// 逻辑驱动器包含扇区数
			DiskInfo[Drive].BytsPerSec = 0;         // 每扇区字节数
			DiskInfo[Drive].RootDirTable = 0;       // 根目录开始扇区号（FAT32为开始簇号）
			DiskInfo[Drive].RootSecCnt = 0;         // 根目录占用扇区数
			DiskInfo[Drive].FATStartSec = 0;        // FAT表开始扇区号
			DiskInfo[Drive].FATSecCnt = 0;          // 每个FAT占用扇区数
			DiskInfo[Drive].DataStartSec = 0;       // 数据区开始扇区号
			DiskInfo[Drive].PathClusIndex = 0;      // 当前目录
			DiskInfo[Drive].DiakCommand = NULL;     // 驱动程序        
		}
	}
}


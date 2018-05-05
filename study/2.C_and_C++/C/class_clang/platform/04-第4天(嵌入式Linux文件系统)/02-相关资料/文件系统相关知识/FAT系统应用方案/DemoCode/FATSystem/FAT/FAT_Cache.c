//======================================================
// 文件名称：	FAT_Cache.c
// 功能描述：	FAT文件系统关于cache管理的部分
// 维护记录：	2006-8-11	v1.0
//======================================================

#define FAT_Dir
#define FAT_Cache
#define FAT_File
#include "FAT.h"
#include "SD\SD_INC.h"

Disk_cache DiskCache[MAX_DISK_CACHES];     // 磁盘cache

//=======================================================
// 语法格式:	void CacheInit(void)
// 实现功能:	初始化磁盘cache
// 参数:		无
// 返回值:		无
//=======================================================
void CacheInit(void)
{
    unsigned int i;
    for (i = 0; i < MAX_DISK_CACHES; i++)
    {
        DiskCache[i].Drive = EMPTY_DRIVE;   // cache空闲
        DiskCache[i].Flag = 0;              // 扇区无需回写
        DiskCache[i].RW_ID = 0;             // 访问纪录
        DiskCache[i].RW_ID--;               // 访问纪录
        DiskCache[i].SecIndex = 0;          // 缓冲的扇区号
    }
}

//==================================================================================
// 语法格式:	void CloseSec(unsigned char Drive, unsigned long int Index)
// 实现功能:	解锁指定cache
// 参数:		Drive	-	逻辑驱动器号
//				Index	-	缓冲的扇区号
// 返回值:		无
//==================================================================================
void CloseSec(unsigned char Drive, unsigned long int Index)
{
    Drive = Drive;
    Index = Index;
}

//====================================================================
// 语法格式:	void CacheWriteBack2(unsigned int Index)
// 实现功能:	把指定扇区写回逻辑盘
// 参数:		Index	-	cache索引
// 返回值:		无
//====================================================================
void CacheWriteBack2(unsigned int Index)
{
	Disk_RW_Parameter Pa;
	Disk_Info *Disk;
	Disk = GetDiskInfo(DiskCache[Index].Drive);     // 获取逻辑盘信息
	if (Disk != NULL)
	if (Disk->DiakCommand != NULL)
    {
		DiskCache[Index].Flag &= ~CACHE_WRITED;     // cache不再需要回写
		// 传递参数
		Pa.Drive = DiskCache[Index].Drive;               
		Pa.SectorIndex = DiskCache[Index].SecIndex;
		Pa.RsvdForLow = Disk->RsvdForLow;
		Pa.Buf = DiskCache[Index].buf;
		Disk->DiakCommand(DISK_WRITE_SECTOR, &Pa);  // 调用底层驱动写扇区 
    }
}

//=======================================================================================
// 语法格式:	void CacheWriteBack(unsigned char Drive, unsigned long int Index)
// 实现功能:	把指定cache写回逻辑盘
// 参数:		Drive	-	逻辑驱动器号
//				Index	-	缓冲的扇区号
// 返回值:		无
//=======================================================================================       
void CacheWriteBack(unsigned char Drive, unsigned long int Index)
{
	unsigned int i;
	for (i = 0; i < MAX_DISK_CACHES; i++)
	{
		if (DiskCache[i].Drive == Drive)
		if (DiskCache[i].SecIndex == Index)
		{
			if ((DiskCache[i].Flag & CACHE_WRITED) != 0)   // #define CACHE_WRITED   0x01
			{
				CacheWriteBack2(i);     // 循环知道找到指定i值,写回逻辑盘
			}
			break;
		}
	}
}

//=================================================================
// 语法格式:	void AllCacheWriteBack(void)
// 实现功能:	把所有已改变的扇区写回逻辑盘
// 参数:		无
// 返回值:		无
//=================================================================
void AllCacheWriteBack(void)
{
	unsigned int i;

	for (i = 0; i < MAX_DISK_CACHES; i++)
	{
		if ((DiskCache[i].Flag & CACHE_WRITED) != 0)// #define CACHE_WRITED    0x01
		{
			CacheWriteBack2(i);
		}
	}
}

//==========================================================
// 语法格式:	unsigned int GetCache(void)
// 实现功能:	获取一个cache
// 参数:		无
// 返回值:		cache索引
//===========================================================
unsigned int GetCache(void)
{
	unsigned int Max;
	unsigned int i,j;
	Max = 0;
	j = 0;
	j--;
	for (i = 0; i < MAX_DISK_CACHES; i++)
	{
		if (Max <= DiskCache[i].RW_ID)
		{
			Max = DiskCache[i].RW_ID;
			j = i;
		}
		if (Max == (unsigned int)(-1))
		{
			break;
		}
	}
	if (j < MAX_DISK_CACHES)	// j为Cache中DiskCache[i].RW_ID次数最多的
	{
		if (DiskCache[j].Drive != EMPTY_DRIVE)			// 如果该Cache已被使用
		if ((DiskCache[j].Flag & CACHE_WRITED) != 0)	// 同时,进行过写操作
		{
			CacheWriteBack2(j);							// 则应写回到设备扇区
		}
	}
	return j;
}

//======================================================================================
// 语法格式:	unsigned char *OpenSec(unsigned char Drive, unsigned long int Index)
// 实现功能:	为逻辑盘上的一个扇区打开一个cache并锁定
//参数:			Drive	-	磁盘号
//				Index	-	簇号
// 返回值:		指向指定扇区数据的指针
//=======================================================================================
unsigned char *OpenSec(unsigned char Drive, unsigned long int Index)
{
	unsigned int i;
	unsigned char *Rt;
	Disk_Info *Disk;
	// 增加访问间隔
	for (i = 0; i < MAX_DISK_CACHES; i++)
	{
		if (DiskCache[i].Drive != EMPTY_DRIVE)        
		{
			if (DiskCache[i].RW_ID < (unsigned int)(-1))   
			{
				DiskCache[i].RW_ID++;                
			}
		}
	}
	// 看扇区是否已经缓存
	Rt = NULL;
	for (i = 0; i < MAX_DISK_CACHES; i++)
	{
		if (DiskCache[i].Drive == Drive )             
		if (DiskCache[i].SecIndex == Index)                 
		{
			Rt = DiskCache[i].buf;           
			DiskCache[i].RW_ID = 0;            
			break;
		}
	}
	// Rt == NULL则未缓存
	if (Rt == NULL)
	{
		Disk = GetDiskInfo(Drive);
		if (Disk != NULL)
		if (Disk->SecPerDisk > Index)
		{
			i = GetCache();                     // 获取cache
			if (i < MAX_DISK_CACHES)
			{
				// 初始化cache
				DiskCache[i].Drive = Drive;     
				DiskCache[i].RW_ID = 0;
				DiskCache[i].Flag = 0;
				DiskCache[i].SecIndex = Index;
				Rt = DiskCache[i].buf;
				for (i = 0; i < Disk->BytsPerSec; i++)
				{
					Rt[i] = 0;
				}
			}
		}
	}
	return Rt;
}

//==========================================================================================
// 语法格式:	unsigned char ReadSec(unsigned char Drive, unsigned long int Index)
// 实现功能:	从逻辑盘读扇区
// 参数:		Drive	-	磁盘号
//				Index	-	扇区号
// 返回值:		RETURN_OK	-	成功
//===========================================================================================
unsigned char ReadSec(unsigned char Drive, unsigned long int Index)
{
	unsigned int i;
	Disk_RW_Parameter Pa;
	Disk_Info *Disk;
	unsigned char Rt;
	for (i = 0; i < MAX_DISK_CACHES; i++)
	{
		if (DiskCache[i].Drive == Drive)
		if (DiskCache[i].SecIndex == Index)
		{
			if ((DiskCache[i].Flag & CACHE_READED) != 0)
			{
				return RETURN_OK;
			}
			break;
		}
	}
	Rt = SECTOR_NOT_IN_CACHE;
	if (i < MAX_DISK_CACHES)
	{
		Disk = GetDiskInfo(Drive);    
		DiskCache[i].Flag |= CACHE_READED;
		// 从磁盘读取扇区数据
		Pa.Drive = Drive;
		Pa.SectorIndex = Index;
		Pa.RsvdForLow = Disk->RsvdForLow;
		Pa.Buf = DiskCache[i].buf;
		Rt = NOT_FIND_DISK;
		if (Disk->DiakCommand != NULL)
		{
			Rt = SECTOR_READ_ERR;
			if (Disk->DiakCommand(DISK_READ_SECTOR, &Pa) == DISK_READ_OK)
			{
				return RETURN_OK;
			}
		}
	}
	return Rt;
}

//================================================================================
// 语法格式:	void WriteSec(unsigned char Drive, unsigned long int Index)
// 实现功能:	说明指定逻辑盘的指定一个扇区被改写
// 参数:		Disk	-	逻辑盘信息
//				Index	-	扇区号
// 返回值:		无
//================================================================================
void WriteSec(unsigned char Drive, unsigned long int Index)
{
	unsigned int i;
	for (i = 0; i < MAX_DISK_CACHES; i++)
	{
		if (DiskCache[i].Drive == Drive)
		if (DiskCache[i].SecIndex == Index)
		{
			DiskCache[i].Flag |= (CACHE_WRITED | CACHE_READED);
			break;
		}
	}
}

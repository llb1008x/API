//======================================================
// �ļ����ƣ�	FAT_Cache.c
// ����������	FAT�ļ�ϵͳ����cache����Ĳ���
// ά����¼��	2006-8-11	v1.0
//======================================================

#define FAT_Dir
#define FAT_Cache
#define FAT_File
#include "FAT.h"
#include "SD\SD_INC.h"

Disk_cache DiskCache[MAX_DISK_CACHES];     // ����cache

//=======================================================
// �﷨��ʽ:	void CacheInit(void)
// ʵ�ֹ���:	��ʼ������cache
// ����:		��
// ����ֵ:		��
//=======================================================
void CacheInit(void)
{
    unsigned int i;
    for (i = 0; i < MAX_DISK_CACHES; i++)
    {
        DiskCache[i].Drive = EMPTY_DRIVE;   // cache����
        DiskCache[i].Flag = 0;              // ���������д
        DiskCache[i].RW_ID = 0;             // ���ʼ�¼
        DiskCache[i].RW_ID--;               // ���ʼ�¼
        DiskCache[i].SecIndex = 0;          // �����������
    }
}

//==================================================================================
// �﷨��ʽ:	void CloseSec(unsigned char Drive, unsigned long int Index)
// ʵ�ֹ���:	����ָ��cache
// ����:		Drive	-	�߼���������
//				Index	-	�����������
// ����ֵ:		��
//==================================================================================
void CloseSec(unsigned char Drive, unsigned long int Index)
{
    Drive = Drive;
    Index = Index;
}

//====================================================================
// �﷨��ʽ:	void CacheWriteBack2(unsigned int Index)
// ʵ�ֹ���:	��ָ������д���߼���
// ����:		Index	-	cache����
// ����ֵ:		��
//====================================================================
void CacheWriteBack2(unsigned int Index)
{
	Disk_RW_Parameter Pa;
	Disk_Info *Disk;
	Disk = GetDiskInfo(DiskCache[Index].Drive);     // ��ȡ�߼�����Ϣ
	if (Disk != NULL)
	if (Disk->DiakCommand != NULL)
    {
		DiskCache[Index].Flag &= ~CACHE_WRITED;     // cache������Ҫ��д
		// ���ݲ���
		Pa.Drive = DiskCache[Index].Drive;               
		Pa.SectorIndex = DiskCache[Index].SecIndex;
		Pa.RsvdForLow = Disk->RsvdForLow;
		Pa.Buf = DiskCache[Index].buf;
		Disk->DiakCommand(DISK_WRITE_SECTOR, &Pa);  // ���õײ�����д���� 
    }
}

//=======================================================================================
// �﷨��ʽ:	void CacheWriteBack(unsigned char Drive, unsigned long int Index)
// ʵ�ֹ���:	��ָ��cacheд���߼���
// ����:		Drive	-	�߼���������
//				Index	-	�����������
// ����ֵ:		��
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
				CacheWriteBack2(i);     // ѭ��֪���ҵ�ָ��iֵ,д���߼���
			}
			break;
		}
	}
}

//=================================================================
// �﷨��ʽ:	void AllCacheWriteBack(void)
// ʵ�ֹ���:	�������Ѹı������д���߼���
// ����:		��
// ����ֵ:		��
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
// �﷨��ʽ:	unsigned int GetCache(void)
// ʵ�ֹ���:	��ȡһ��cache
// ����:		��
// ����ֵ:		cache����
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
	if (j < MAX_DISK_CACHES)	// jΪCache��DiskCache[i].RW_ID��������
	{
		if (DiskCache[j].Drive != EMPTY_DRIVE)			// �����Cache�ѱ�ʹ��
		if ((DiskCache[j].Flag & CACHE_WRITED) != 0)	// ͬʱ,���й�д����
		{
			CacheWriteBack2(j);							// ��Ӧд�ص��豸����
		}
	}
	return j;
}

//======================================================================================
// �﷨��ʽ:	unsigned char *OpenSec(unsigned char Drive, unsigned long int Index)
// ʵ�ֹ���:	Ϊ�߼����ϵ�һ��������һ��cache������
//����:			Drive	-	���̺�
//				Index	-	�غ�
// ����ֵ:		ָ��ָ���������ݵ�ָ��
//=======================================================================================
unsigned char *OpenSec(unsigned char Drive, unsigned long int Index)
{
	unsigned int i;
	unsigned char *Rt;
	Disk_Info *Disk;
	// ���ӷ��ʼ��
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
	// �������Ƿ��Ѿ�����
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
	// Rt == NULL��δ����
	if (Rt == NULL)
	{
		Disk = GetDiskInfo(Drive);
		if (Disk != NULL)
		if (Disk->SecPerDisk > Index)
		{
			i = GetCache();                     // ��ȡcache
			if (i < MAX_DISK_CACHES)
			{
				// ��ʼ��cache
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
// �﷨��ʽ:	unsigned char ReadSec(unsigned char Drive, unsigned long int Index)
// ʵ�ֹ���:	���߼��̶�����
// ����:		Drive	-	���̺�
//				Index	-	������
// ����ֵ:		RETURN_OK	-	�ɹ�
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
		// �Ӵ��̶�ȡ��������
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
// �﷨��ʽ:	void WriteSec(unsigned char Drive, unsigned long int Index)
// ʵ�ֹ���:	˵��ָ���߼��̵�ָ��һ����������д
// ����:		Disk	-	�߼�����Ϣ
//				Index	-	������
// ����ֵ:		��
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

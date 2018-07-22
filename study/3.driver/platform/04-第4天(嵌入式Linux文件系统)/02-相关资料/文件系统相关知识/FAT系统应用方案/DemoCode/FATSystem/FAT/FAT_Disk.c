//======================================================
// �ļ����ƣ�	FAT_Disk.c
// ����������	FAT�ļ�ϵͳ�����߼��̹���Ĳ���
// ά����¼��	2006-8-11	v1.0
//======================================================

#define FAT_Dir
#define FAT_Disk
#include "FAT.h"
#include "SD\SD_INC.h"

Disk_Info DiskInfo[MAX_DRIVES];     // �߼�����Ϣ

//============================================
// �﷨��ʽ:	void DiskInit(void)
// ʵ�ֹ���:	��ʼ���߼��̹���ģ��
// ����:		��
// ����ֵ:		��
//============================================
void DiskInit(void)
{
	unsigned char i;
    
	for (i = 0; i < MAX_DRIVES; i++)
	{
		DiskInfo[i].Drive = EMPTY_DRIVE;    // û�з���
		DiskInfo[i].FATType = 0xff;         // ��Ч���ļ�ϵͳ
		DiskInfo[i].SecPerClus = 0;         // ÿ��������
		DiskInfo[i].NumFATs = 0;            // FAT����
		DiskInfo[i].SecPerDisk = 0xffffffff;// �߼�����������������
		DiskInfo[i].BytsPerSec = 0;         // ÿ�����ֽ���
		DiskInfo[i].RootDirTable = 0;       // ��Ŀ¼��ʼ�����ţ�FAT32Ϊ��ʼ�غţ�
		DiskInfo[i].RootSecCnt = 0;         // ��Ŀ¼ռ��������
		DiskInfo[i].FATStartSec = 0;        // FAT��ʼ������
		DiskInfo[i].FATSecCnt = 0;          // ÿ��FATռ��������
		DiskInfo[i].DataStartSec = 0;       // ��������ʼ������
		DiskInfo[i].PathClusIndex = 0;      // ��ǰĿ¼
		DiskInfo[i].DiakCommand = NULL;     // ��������
	}
	CacheInit();                			// ��ʼ������cache
}

//===============================================================
// �﷨��ʽ:	Disk_Info *GetDiskInfo(unsigned char Drive)
// ʵ�ֹ���:	��ȡָ���߼�����Ϣ
// ����:		�߼��̺�
// ����ֵ:		ָ���߼�����Ϣ�Ľṹ��ָ��
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
// �﷨��ʽ:	Disk_Info *GetEmptyDiskInfoAddr(void)
// ʵ�ֹ���:	��ȡ�����߼���
// ����:		��
// ����ֵ:		ָ���߼�����Ϣ�Ľṹ��ָ��
//===========================================================
Disk_Info *GetEmptyDiskInfoAddr(void)
{
	unsigned char i;
	Disk_Info *Rt;
    
	Rt = NULL;
	for (i = 0; i < MAX_DRIVES; i++)
	{
		if (DiskInfo[i].Drive == EMPTY_DRIVE)     // �߼��̺ſ���
		{
			Rt = DiskInfo + i;
			DiskInfo[i].Drive = i;
			break;
		}
	}
	return Rt;
}

//===========================================================================================================
// �﷨��ʽ:	void AddFileDriver(unsigned int  (* DiakCommand)(unsigned char Cammand, void *Parameter))
// ʵ�ֹ���:	����һ���ײ���������
// ����:		DiakCommand	-	��������ӿں���
// ����ֵ:		��
//============================================================================================================
void AddFileDriver(unsigned int  (* DiakCommand)(unsigned char Cammand, void *Parameter))
{
	unsigned char i;
	unsigned char *Buf;
	unsigned int temp;
	unsigned long int temp1;
    
	if (DiakCommand == NULL)        // ������Ч�˳�
	{
		return;
	}

	if (DiakCommand(DISK_INIT, NULL) == DISK_INIT_NOT_OK)   // DISK_INIT=0,�ײ�������ʼ��ʧ���˳�
	{
		return;
	}
    
    // ���»�ȡ�߼�����Ϣ
	for (i = 0; i < MAX_DRIVES; i++)
	{
		if (DiskInfo[i].Drive != EMPTY_DRIVE)       // �߼����ѷ���
		{
			Buf = OpenSec(DiskInfo[i].Drive, 0);    // ������0������������,����ָ��ָ���������ݵ�ָ��
			ReadSec(DiskInfo[i].Drive, 0);
			if (Buf != NULL)
			{
				// ÿ�����ֽ���
				DiskInfo[i].BytsPerSec = Buf[11] | (Buf[12] << 8);
				// ÿ��������
				DiskInfo[i].SecPerClus = Buf[13];
				// FAT��ʼ������
				DiskInfo[i].FATStartSec = Buf[14] | (Buf[15] << 8);
				// FAT�����
				DiskInfo[i].NumFATs = Buf[16];
				// ��Ŀ¼ռ������ 
				DiskInfo[i].RootSecCnt = ((Buf[17] | (Buf[18] << 8)) * 32 + DiskInfo[i].BytsPerSec - 1) / DiskInfo[i].BytsPerSec;
				// �߼��̣���ռ������
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
				// FAT��ռ��������
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
                
				// ��ǰĿ¼Ϊ��Ŀ¼
				DiskInfo[i].PathClusIndex = 0;
			}
			// ��Ŀ¼��ʼ������
			DiskInfo[i].RootDirTable = DiskInfo[i].NumFATs * DiskInfo[i].FATSecCnt + DiskInfo[i].FATStartSec;
			// ��������ʼ������
			DiskInfo[i].DataStartSec = DiskInfo[i].RootDirTable + DiskInfo[i].RootSecCnt;
			temp1 = DiskInfo[i].SecPerDisk - DiskInfo[i].DataStartSec;
			temp1 = temp1 / DiskInfo[i].SecPerClus;
			DiskInfo[i].ClusPerData = temp1;
                    
			// �ж���FAT12��FAT16����FAT32
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
                // FAT32 RootDirTableΪ��ʼ�غ�
				DiskInfo[i].RootDirTable = Buf[44] | (Buf[45] << 8);
				DiskInfo[i].RootDirTable |= (((unsigned long int)Buf[46] << 16) | ((unsigned long int)Buf[47] << 24));
				DiskInfo[i].PathClusIndex = DiskInfo[i].RootDirTable;
			}
			CloseSec(DiskInfo[i].Drive, 0);     // �ر�����
		}
	}
}

//=============================================================
// �﷨��ʽ: 	void RemoveFileDriver(unsigned char Drive)
// ʵ�ֹ���:	ɾ��һ���ײ���������
// ����:		Drive:�߼���������
// ����ֵ:		��
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

			DiskInfo[Drive].Drive = EMPTY_DRIVE;    // û�з���
			DiskInfo[Drive].FATType = 0xff;         // ��Ч���ļ�ϵͳ
			DiskInfo[Drive].SecPerClus = 0;         // ÿ��������
			DiskInfo[Drive].NumFATs = 0;            // FAT����
			DiskInfo[Drive].SecPerDisk = 0xffffffff;// �߼�����������������
			DiskInfo[Drive].BytsPerSec = 0;         // ÿ�����ֽ���
			DiskInfo[Drive].RootDirTable = 0;       // ��Ŀ¼��ʼ�����ţ�FAT32Ϊ��ʼ�غţ�
			DiskInfo[Drive].RootSecCnt = 0;         // ��Ŀ¼ռ��������
			DiskInfo[Drive].FATStartSec = 0;        // FAT��ʼ������
			DiskInfo[Drive].FATSecCnt = 0;          // ÿ��FATռ��������
			DiskInfo[Drive].DataStartSec = 0;       // ��������ʼ������
			DiskInfo[Drive].PathClusIndex = 0;      // ��ǰĿ¼
			DiskInfo[Drive].DiakCommand = NULL;     // ��������        
		}
	}
}


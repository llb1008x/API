//======================================================
// �ļ����ƣ�	FAT_FAT.c
// ����������	FAT�ļ�ϵͳ���ļ�����������������
// ά����¼��	2006-8-11	v1.0
//======================================================

#define FAT_Dir
#define FAT_Clus
#include "FAT.h"
#include "SD\SD_INC.h"

//================================================================================================
// �﷨��ʽ:	unsigned long int FATGetNextClus(unsigned char Drive, unsigned long int Index)
// ʵ�ֹ���:	����FAT��ָ���ص���һ���غ�
// ����:		Drive	-	��������
//       		Index	-	�غ�
// ����ֵ:		��һ���غ�
//=================================================================================================
unsigned long int FATGetNextClus(unsigned char Drive, unsigned long int Index)
{
	unsigned int temp, ByteIndex;
	unsigned long int SecIndex;
	unsigned char *Buf;
	Disk_Info *Disk;
	unsigned long int Rt;
    
	Disk = GetDiskInfo(Drive);       // ��ȡָ���߼�����Ϣ
	if (Disk == NULL)
	{
		return BAD_CLUS;
	}

	if (Index >= (Disk->ClusPerData))
	{
		return BAD_CLUS;
	}
    
	// ���������ź��ֽ�����
	switch (Disk->FATType)
	{
		case FAT12:
			SecIndex = Index * 3 / (2 * Disk->BytsPerSec);
			ByteIndex = ((Index * 3) / 2) - (SecIndex * Disk->BytsPerSec);
			SecIndex += Disk->FATStartSec;
			break;
		case FAT16:
			SecIndex = Index * 2 / Disk->BytsPerSec + Disk->FATStartSec;
			ByteIndex = (Index * 2) & (Disk->BytsPerSec - 1);//ƫ����
			break;
		case FAT32:
			SecIndex = Index * 4 / Disk->BytsPerSec + Disk->FATStartSec;
			ByteIndex = (Index * 4) & (Disk->BytsPerSec - 1);
			break;
		default:
			return BAD_CLUS;
	}

	Buf = OpenSec(Drive, SecIndex);
	if (Buf == NULL)
	{
		return BAD_CLUS;
	}
	ReadSec(Drive, SecIndex);
    
	// ��ȡFAT������
	switch (Disk->FATType)
	{
		case FAT12:
			temp = Buf[ByteIndex];
			ByteIndex++;
			if (ByteIndex >= Disk->BytsPerSec)          // ��һ���ֽ��Ƿ�����һ������
			{
				Buf = OpenSec(Drive, SecIndex + 1);
				if (Buf == NULL)
				{
					return BAD_CLUS;
				}
				ReadSec(Drive, SecIndex + 1);
				temp = temp | (Buf[0] << 8);
				CloseSec(Drive, SecIndex + 1);
			}
			else
			{
				temp = temp | (Buf[ByteIndex] << 8);
			}
			if ((Index & 0x01) != 0)                // �ж���12λ��Ч
			{
				temp = temp / 16;
			}
			else
			{
				temp = temp & 0x0fff;
			}
			Rt = temp;
			if (temp >= (BAD_CLUS & 0x0fff))        // �Ƿ�����������
			{
				Rt = ((unsigned long int)0x0fffL << 16) | (temp | 0xf000);
			}
			break;
		case FAT16:
			temp = Buf[ByteIndex] | (Buf[ByteIndex + 1] << 8);
			Rt = temp;
			if (temp >= (BAD_CLUS & 0xffff))        // �Ƿ�����������
			{
				Rt = ((unsigned long int)0x0fffL << 16) | temp;
			}
			break;
		case FAT32:
			Rt = Buf[ByteIndex] | (Buf[ByteIndex + 1] << 8);
			Rt |= ((unsigned long int)Buf[ByteIndex + 2] << 16) | ((unsigned long int)Buf[ByteIndex + 3] << 24);
			Rt = Rt & 0x0fffffff;
			break;
		default:
			Rt = BAD_CLUS;
			break;
	}
	CloseSec(Drive, SecIndex);
	return Rt;
}

//===========================================================================================================
// �﷨��ʽ:	void FATSetNextClus(unsigned char Drive, unsigned long int Index, unsigned long int Next)
// ʵ�ֹ���:	������һ����
// ����:		Drive	-	��������
//	      		Index	-	�غ�
//	        	Next	-	��һ���غ�
// ����ֵ:		��
//============================================================================================================      
void FATSetNextClus(unsigned char Drive, unsigned long int Index, unsigned long int Next)
{
	unsigned int temp;
	unsigned int SecIndex, ByteIndex;
	unsigned char *Buf;
	Disk_Info *Disk;
    
	Disk = GetDiskInfo(Drive);
	if (Disk == NULL)
	{
		return;
	}
	if (Index <= EMPTY_CLUS_1)
	{
		return;
	}

	if (Index >= Disk->ClusPerData)
	{
		return;
	}
    
	// ���������ź��ֽ�����
	switch (Disk->FATType)
	{
		case FAT12:
			SecIndex = Index * 3 / (2 * Disk->BytsPerSec);
			ByteIndex = ((Index * 3) / 2) - (SecIndex * Disk->BytsPerSec);
			SecIndex += Disk->FATStartSec;
			break;
		case FAT16:
			SecIndex = Index * 2 / Disk->BytsPerSec + Disk->FATStartSec;
			ByteIndex = (Index * 2) & (Disk->BytsPerSec - 1);
			break;
		case FAT32:
			SecIndex = Index * 4 / Disk->BytsPerSec + Disk->FATStartSec;
			ByteIndex = (Index * 4) & (Disk->BytsPerSec - 1);
			break;
		default:
			return;
	}

	Buf = OpenSec(Drive, SecIndex);
	if (Buf == NULL)
	{
		return;
	}
	ReadSec(Drive, SecIndex);

	switch (Disk->FATType)
	{
		case FAT12:
			temp = Next & 0x0fff;
			if ((Index & 0x01) != 0)                // �ж���12λ��Ч
			{
				temp = temp * 16;
				temp |= (Buf[ByteIndex] & 0x0f);
				Buf[ByteIndex] = temp;
			}
			else
			{
				Buf[ByteIndex] = temp;
			}
			ByteIndex++;
			temp = temp >> 8;
			if (ByteIndex >= Disk->BytsPerSec)         // ��һ���ֽ��Ƿ�����һ������
			{
				Buf = OpenSec(Drive, SecIndex + 1);
				if (Buf == NULL)
				{
					break;
				}
				ReadSec(Drive, SecIndex + 1);
				if ((Index & 0x01) != 0)                // �ж���12λ��Ч
				{
					Buf[0] = temp;
				}
				else
				{
					Buf[0] = (Buf[0] & 0xf0) | temp;
				}
				WriteSec(Drive, SecIndex + 1);
				CloseSec(Drive, SecIndex + 1);
			}
			else
			{
				if ((Index & 0x01) != 0)                // �ж���12λ��Ч
				{
					Buf[ByteIndex] = temp;
				}
				else
				{
					Buf[ByteIndex] = (Buf[ByteIndex] & 0xf0) | temp;
				}
			}
			break;
		case FAT16:
			Buf[ByteIndex] = Next;
			Buf[ByteIndex + 1] = Next >> 8;
			break;
		case FAT32:
			Buf[ByteIndex] = Next;
			Buf[ByteIndex + 1] = Next >> 8;
			Buf[ByteIndex + 2] = Next >> 16;
			Buf[ByteIndex + 3] = (Buf[ByteIndex + 3] & 0xf0) | ((Next >> 24) & 0x0f);
			break;
		default:
			break;
	}
	WriteSec(Drive, SecIndex);
	CloseSec(Drive, SecIndex);
	return ;
}

//========================================================================================
// �﷨��ʽ:	unsigned long int FATAddClus(unsigned char Drive, unsigned long int Index)
// ʵ�ֹ���:	Ϊָ����������һ����
// ����:		Drive	-	��������
//				Index	-	����������һ���غţ����Ϊ0����Ϊһ����������һ����
// ����ֵ:		���ӵĴغ�
//==========================================================================================
unsigned long int FATAddClus(unsigned char Drive, unsigned long int Index)
{
	unsigned long int NextClus,ThisClus,MaxClus;
	Disk_Info *Disk;

	Disk = GetDiskInfo(Drive);
	if (Disk == NULL)
	{
		return BAD_CLUS;
	}
    
	if (Index >= BAD_CLUS)
	{
		return BAD_CLUS;
	}
	MaxClus = Disk->ClusPerData;

	// �������һ����
	ThisClus = Index;
	if (ThisClus != EMPTY_CLUS && ThisClus != EMPTY_CLUS_1)
	{
		while (1)
		{
			NextClus = FATGetNextClus(Drive, ThisClus);
			if (NextClus >= EOF_CLUS_1)
			{
				break;
			}
			if (NextClus <= EMPTY_CLUS_1)
			{
				break;
			}
			if (NextClus == BAD_CLUS)
			{
				return BAD_CLUS;
			}
			ThisClus = NextClus;
		}
	}
	else
	{
		ThisClus = EMPTY_CLUS_1;
	}
    
	for (NextClus = ThisClus + 1; NextClus < MaxClus; NextClus++)
	{
		if (FATGetNextClus(Drive, NextClus) == EMPTY_CLUS)
		{
			break;
		}
	}
	if (NextClus >= MaxClus)
	{
		for (NextClus = EMPTY_CLUS_1 + 1; NextClus < ThisClus; NextClus++)
		{
			if (FATGetNextClus(Drive, NextClus) == EMPTY_CLUS)
			{
 				break;
			}
		}
	}
	if (FATGetNextClus(Drive, NextClus) == EMPTY_CLUS)
	{
		if (ThisClus > EMPTY_CLUS_1)
		{
			FATSetNextClus(Drive, ThisClus, NextClus);
		}
		FATSetNextClus(Drive, NextClus, EOF_CLUS_END);
		return NextClus;
	}
 	else
	{
		return BAD_CLUS;
	}
}

//================================================================================
// �﷨��ʽ:	void FATDelClusChain(unsigned char Drive, unsigned long int Index)
// ʵ�ֹ���:	ɾ��ָ������
// ����:		Drive	-	��������
//				Index	-	�������״غ�
// ����ֵ:		��
//================================================================================     
void FATDelClusChain(unsigned char Drive, unsigned long int Index)
{
	unsigned long int NextClus, ThisClus;
    
	if (Index <= EMPTY_CLUS_1)
	{
		return;
	}
	if (Index >= BAD_CLUS)
 	{
		return;
	}
	ThisClus = Index;
	while (1)
	{
		NextClus = FATGetNextClus(Drive, ThisClus);
		FATSetNextClus(Drive, ThisClus, EMPTY_CLUS);
		if (NextClus >= BAD_CLUS)
		{
			break;
		}
		if (NextClus <= EMPTY_CLUS_1)
		{
			break;
		}
		ThisClus = NextClus;
	}
}


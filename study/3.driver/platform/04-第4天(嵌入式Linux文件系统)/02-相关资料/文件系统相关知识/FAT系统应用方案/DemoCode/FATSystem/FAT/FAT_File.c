//======================================================
// �ļ����ƣ�	FAT_File.c
// ����������	�ļ�ϵͳ���ļ�������������
// ά����¼��	2006-8-11	v1.0
//======================================================

#define FAT_Dir
#define FAT_File
#include "FAT.h"
#include "SD\SD_INC.h"

extern char *strupr(char *Str);
extern unsigned char GetDrive(char *Path);
extern unsigned long int _GetFDTInfo(char *FDTName, char *DirFileName);
static MY_FILE FileInfo[MAX_OPEN_FILES];	// ���ļ���Ϣ�� 

//====================================================
// �﷨��ʽ:	void FileInit(void)
// ʵ�ֹ���:	��ʼ���ļ�ָ��ϵͳ
// ����:		��
// ����ֵ:		��
//=====================================================
void FileInit(void)
{
	unsigned char i;
	for (i = 0; i < MAX_OPEN_FILES; i++)
	{
		FileInfo[i].Flags = 0;
	}
	DiskInit();
}

//===============================================================================================================
// �﷨��ʽ:	HANDLE FindOpenFile(unsigned char Drive, unsigned long int DirClus, char FileName[11])
// ʵ�ֹ���:	�����Ѵ򿪵�ָ���ļ����ļ�������ڲ�ʹ��
// ����:		Drive:�ļ����ڵ��߼��̺�
//				DirClus:�ļ�����Ŀ¼�Ŀ�ʼ�غ�
//				FileName:�ڲ��ļ���
// ����ֵ:		�ļ������Not_Open_FILEΪû�д�
//===============================================================================================================
HANDLE FindOpenFile(unsigned char Drive, unsigned long int DirClus, char FileName[11])
{
	HANDLE Rt;
	MY_FILE *fp;
	fp = FileInfo;
	for (Rt = 0; Rt < MAX_OPEN_FILES; Rt++)
	{
		if (fp->Flags != 0)
		if (fp->Drive == Drive)
		if (fp->DirClus == DirClus)
		if (fp->Name[0] == FileName[0])
		if (fp->Name[1] == FileName[1])
		if (fp->Name[2] == FileName[2])
		if (fp->Name[3] == FileName[3])
		if (fp->Name[4] == FileName[4])
		if (fp->Name[5] == FileName[5])
		if (fp->Name[6] == FileName[6])
		if (fp->Name[7] == FileName[7])
		if (fp->Name[8] == FileName[8])
		if (fp->Name[9] == FileName[9])
		if (fp->Name[10] == FileName[10])
		{
			break;
		}
		fp++;
	}
	if (Rt >= MAX_OPEN_FILES)
	{
		Rt = Not_Open_FILE;
	}
	return Rt;
}

//==================================================================
// �﷨��ʽ:	unsigned char RemoveFile(char *DirFileName)
// ʵ�ֹ���:	ɾ���ļ�
// ����:		DirFileName	-	�û�ʹ�õ��ļ���
// ����ֵ:		RETURN_OK	-	�ɹ�
//==================================================================
unsigned char RemoveFile(char *DirFileName)
{
	unsigned long int ClusIndex, ClusIndex1;
	unsigned char Drive, Rt;
	char DirName[12];
	FDT temp;
	strupr(DirFileName);                       // ��Ϊ��д
	ClusIndex = _GetFDTInfo(DirName, DirFileName);
	Rt = PATH_NOT_FIND;
	if (ClusIndex != BAD_CLUS)
	{
		Drive = GetDrive(DirFileName);     // ��ȡ�ļ������߼��̺�
		Rt = FindFDTInfo(&temp, Drive, ClusIndex, DirName);
		if (Rt == RETURN_OK)
		{
			Rt = NOT_FIND_FILE;
			if ((temp.Attr & ATTR_DIRECTORY) == 0)  // ���ļ���ɾ��
			{
				Rt = FILE_LOCK;
				if (FindOpenFile(Drive, ClusIndex, DirName) < MAX_OPEN_FILES)
				{
					// �ļ�û�д򿪲�ɾ��
					ClusIndex1 = temp.FstClusLO + ((unsigned long int)(temp.FstClusHI) << 16);
					FATDelClusChain(Drive, ClusIndex1);
					Rt = DelFDT(Drive, ClusIndex, DirName);
                    AllCacheWriteBack();
				}
			}
		}
	}
	return Rt;
}

//========================================================================================================
// �﷨��ʽ:	HANDLE FindOpenFileW(unsigned char Drive, unsigned long int DirClus, char FileName[11])
// ʵ�ֹ���:	������д��ʽ�򿪵�ָ���ļ����ļ�������ڲ�ʹ��
// ����:		Drive	-	�ļ����ڵ��߼��̺�
//				DirClus	-	�ļ�����Ŀ¼�Ŀ�ʼ�غ�
//				FileName	-	�ڲ��ļ���
// ����ֵ:		�ļ����	-	Not_Open_FILEΪû�д�
//========================================================================================================
HANDLE FindOpenFileW(unsigned char Drive, unsigned long int DirClus, char FileName[11])
{
	HANDLE Rt;
	MY_FILE *fp;
	fp = FileInfo;
	for (Rt = 0; Rt < MAX_OPEN_FILES; Rt++)
	{
		if ((fp->Flags & FILE_FLAGS_WRITE) != 0)
		if (fp->Drive == Drive)
		if (fp->DirClus == DirClus)
		if (fp->Name[0] == FileName[0])
		if (fp->Name[1] == FileName[1])
		if (fp->Name[2] == FileName[2])
		if (fp->Name[3] == FileName[3])
		if (fp->Name[4] == FileName[4])
		if (fp->Name[5] == FileName[5])
		if (fp->Name[6] == FileName[6])
		if (fp->Name[7] == FileName[7])
		if (fp->Name[8] == FileName[8])
		if (fp->Name[9] == FileName[9])
		if (fp->Name[10] == FileName[10])
		{
			break;
		}
		fp++;
	}
	if (Rt >= MAX_OPEN_FILES)
	{
		Rt = Not_Open_FILE;
	}
  
	return Rt;
}

//======================================================================
// �﷨��ʽ:	HANDLE _FileOpenR(char *DirFileName)
// ʵ�ֹ���:	ֻ����ʽ���ļ����ڲ�ʹ��
// ����:		DirFileName:�û�ʹ�õ��ļ���
// ����ֵ:		�ļ����	-	Not_Open_FILEΪ���ܴ�
//======================================================================
HANDLE _FileOpenR(char *DirFileName)
{
	MY_FILE *fp;
	HANDLE Rt;
	FDT FileFDT;
	// ���ҿ����ļ��Ǽ���
	for (Rt = 0; Rt < MAX_OPEN_FILES; Rt++)
	{
		if (FileInfo[Rt].Flags == 0)
		{
			break;
		}
	}
	if (Rt < MAX_OPEN_FILES)
	{
		fp = FileInfo + Rt;
		// ��ȡĿ¼��ʼ�غź��ļ���
		fp->DirClus = _GetFDTInfo(fp->Name, DirFileName);
		if (fp->DirClus < BAD_CLUS)
		{
			fp->Drive = GetDrive(DirFileName);     // ��ȡ�ļ������߼��̺�
			// ��ȡ�ļ���Ϣ
			if (FindFDTInfo(&FileFDT, fp->Drive, fp->DirClus, fp->Name) == RETURN_OK)
			{
				if ((FileFDT.Attr & ATTR_DIRECTORY) == 0)
				{
					fp->Flags = FILE_FLAGS_READ;
					fp->FileSize = FileFDT.FileSize;
					fp->FstClus = FileFDT.FstClusLO | (unsigned long int)FileFDT.FstClusHI << 16;
					fp->Clus = fp->FstClus;
					fp->Offset = 0;
					return Rt;
				}
			}
		}
	}
	return Not_Open_FILE; 
}

//===============================================================
// �﷨��ʽ:	HANDLE _FileOpenW(char *DirFileName)
// ʵ�ֹ���:	ֻд��ʽ���ļ����ڲ�ʹ��
// ����:		DirFileName	-	�û�ʹ�õ��ļ���
// ����ֵ:		�ļ����	-	Not_Open_FILEΪ���ܴ�
//================================================================
HANDLE _FileOpenW(char *DirFileName)
{
	MY_FILE *fp;
	FDT temp;
	HANDLE Rt;
	unsigned char i;
	for (Rt = 0; Rt < MAX_OPEN_FILES; Rt++)
	{
		if (FileInfo[Rt].Flags == 0)
		{
			break;
		}
	}
	if (Rt < MAX_OPEN_FILES)
	{
		fp = FileInfo + Rt;
        
		// ��ȡĿ¼��ʼ�غź��ļ���
		fp->DirClus = _GetFDTInfo(fp->Name, DirFileName);
		if (fp->DirClus < BAD_CLUS)
		{
			fp->Drive = GetDrive(DirFileName);     // ��ȡ�ļ������߼��̺�
			// �ļ��Ѿ��Զ�д��ʽ�򿪣������ٴ��Զ�д��ʽ��
			if (FindOpenFileW(fp->Drive, fp->DirClus, fp->Name) == Not_Open_FILE)
			{
				if (FindFDTInfo(&temp, fp->Drive, fp->DirClus, fp->Name) == RETURN_OK)
				{
					if ((temp.Attr & ATTR_DIRECTORY) != 0)
					{
						return Not_Open_FILE;
					}
					if (RemoveFile(DirFileName) != RETURN_OK)   // ɾ���ļ�
					{
						return Not_Open_FILE;
					}
				}
				// �����ļ�
				for (i = 0; i < 11; i++)
				{
					temp.Name[i] = fp->Name[i];
				}
				temp.Attr = 0;
				temp.FileSize = 0;
				temp.NTRes = 0;
				temp.CrtTimeTenth = 0;
				temp.CrtTime = 0;
				temp.CrtDate = 0;
				temp.LstAccDate = 0;
				temp.WrtTime = 0;
				temp.WrtDate = 0;
				temp.FstClusLO = 0;
				temp.FstClusHI = 0;
				if (AddFDT(fp->Drive, fp->DirClus, &temp) == RETURN_OK)       // �����ļ�
				{
					// �����ļ���Ϣ
					fp->Flags = FILE_FLAGS_READ | FILE_FLAGS_WRITE;
					fp->FileSize = 0;
					fp->FstClus = 0;
					fp->Clus = 0;
					fp->Offset = 0;
					return Rt;
				}
			}
		}
	}
	return Not_Open_FILE;
}

//==============================================================
// �﷨��ʽ:	HANDLE _FileOpenRW(char *DirFileName)
// ʵ�ֹ���:	ֻ��д��ʽ���ļ����ڲ�ʹ��
// ����: 		DirFileName	-	�û�ʹ�õ��ļ���
// ����ֵ: 		�ļ����	-	Not_Open_FILEΪ���ܴ�
//==============================================================
HANDLE _FileOpenRW(char *DirFileName)
{
	HANDLE Rt;
	Rt = _FileOpenR(DirFileName);
	if (Rt == Not_Open_FILE)
	{
		Rt = _FileOpenW(DirFileName);
	}
	else
	{
		if (FindOpenFileW(FileInfo[Rt].Drive, FileInfo[Rt].DirClus, FileInfo[Rt].Name) == Not_Open_FILE)
		{
			FileInfo[Rt].Flags |=  (FILE_FLAGS_WRITE | FILE_FLAGS_READ);
		}
		else
		{
			FileInfo[Rt].Flags = 0;
			Rt = Not_Open_FILE;
		}
	}
	return Rt;
}

//===================================================================
// �﷨��ʽ:	HANDLE FileOpen(char *DirFileName, char *Type)
// ʵ�ֹ���:	��ָ����ʽ���ļ�
// ����			DirFileName	-	�û�ʹ�õ��ļ���
//				Type	-	�򿪷�ʽ
// ����ֵ:		�ļ����	-	Not_Open_FILEΪ���ܴ�
//====================================================================
HANDLE FileOpen(char *DirFileName, char *Type)
{
	HANDLE Rt;
	Rt = Not_Open_FILE;    
	strupr(Type);
	strupr(DirFileName);
	if (Type[0] == 'R')
	{
		if (Type[1] == 'W')
		{
			Rt = _FileOpenRW(DirFileName);
		}
		else
		{
			Rt = _FileOpenR(DirFileName);
		}
		return Rt;
	}
	if (Type[0] == 'W')
	{
		if (Type[1] == 'R')
		{
			Rt = _FileOpenRW(DirFileName);
		}
		else
		{
			Rt = _FileOpenW(DirFileName);
		}
	}
	return Rt;
}

//================================================================================
// �﷨��ʽ:	unsigned char FileClose(HANDLE Handle)
// ʵ�ֹ���:	�ر�ָ���ļ�
// ����:		Handle	-	�ļ����
// ����ֵ:		RETURN_OK	-	�ɹ�
//=================================================================================
unsigned char FileClose(HANDLE Handle)
{
	unsigned char Rt;
	FDT FileFDT;
	MY_FILE *fp;
	Rt = PARAMETER_ERR;
	if (Handle >= 0 && Handle < MAX_OPEN_FILES)
	{
		Rt = RETURN_OK;
		fp = FileInfo + Handle;
		if ((fp->Flags & FILE_FLAGS_WRITE) == FILE_FLAGS_WRITE)
		{
			Rt = FindFDTInfo(&FileFDT, fp->Drive, fp->DirClus, fp->Name);
			if (Rt == RETURN_OK)
			if (FileFDT.FileSize < fp->FileSize)
			{
				FileFDT.FileSize = fp->FileSize;
				if (FileFDT.FstClusLO == 0)
				if (FileFDT.FstClusHI == 0)
				{
					FileFDT.FstClusLO = fp->FstClus & 0xffff;
					FileFDT.FstClusHI = (fp->FstClus >> 16) & 0xffff;
				}
				ChangeFDT(fp->Drive, fp->DirClus, &FileFDT);
			}
		}
		fp->Flags = 0;
	}
	AllCacheWriteBack();
	return Rt;
}

//===============================================================================
// �﷨��ʽ:	unsigned char FileGetCh(unsigned char *Ch, HANDLE Handle)
// ʵ�ֹ���:	���ļ���һ���ֽ�
// ����:		Ch	-	���ض���������
//				Handle	-	�ļ����
// ����ֵ:		RETURN_OK	-	�ɹ�
//===============================================================================
unsigned char FileGetCh(unsigned char *Ch, HANDLE Handle)
{
	unsigned long int i, j, SecIndex;
	MY_FILE *fp;
	Disk_Info *DiskInfo;
	unsigned char Rt;
	unsigned char *Buf;
	Rt = PARAMETER_ERR;	// �Ƿ�����							
	fp = FileInfo + Handle;
	if (Handle >= 0 && Handle < MAX_OPEN_FILES)    // Handle�Ƿ���Ч
	if (fp->Flags != 0)                            // ��Ӧ�Ĵ��ļ���Ϣ���Ƿ���ʹ��
	{
		Rt = FILE_EOF;
		if (fp->Offset >= fp->FileSize)
		{
			// ��������ݳ����ļ��ĳ��ȣ���һ��˭���б�ĳ�������д����ļ�
			// ����У�������ļ��ĳ����б仯������֮
			Handle = FindOpenFileW(fp->Drive, fp->DirClus, fp->Name);
			if (Handle < MAX_OPEN_FILES)
			{
				fp->FileSize = FileInfo[Handle].FileSize;
				if (fp->Offset == 0)
				{
					fp->FstClus = FileInfo[Handle].FstClus;
					fp->Clus = FileInfo[Handle].Clus;
				}
			}
		}
		Rt = FILE_EOF;
		if (fp->Offset < fp->FileSize)              // �Ƿ��ļ�����
		{
			Rt = NOT_FIND_DISK;                        
			// ��ȡ�ļ������߼�����Ϣ
			DiskInfo = GetDiskInfo(fp->Drive);
			if (DiskInfo != NULL)
			{
				// ����������������
				j = fp->Offset % (DiskInfo->SecPerClus * DiskInfo->BytsPerSec);
				i = j / DiskInfo->BytsPerSec;
				j = j % DiskInfo->BytsPerSec;
				SecIndex = (fp->Clus - 2) * DiskInfo->SecPerClus + DiskInfo->DataStartSec + i;
				Rt = SECTOR_READ_ERR;
				Buf = OpenSec(fp->Drive, SecIndex);
				// ������
				if (Buf != NULL)
				{
					// ��ȡ��������
					if (ReadSec(fp->Drive, SecIndex) == RETURN_OK)
					{
						// ��ȡ����
						*Ch = Buf[j];
						// �����ļ�ָ��
						fp->Offset++;
						j++;
						if (j >= DiskInfo->BytsPerSec)
						{
							i++;
							if (i >= DiskInfo->SecPerClus)
							{
								fp->Clus = FATGetNextClus(fp->Drive, fp->Clus);
							}
						}
						Rt = RETURN_OK;
					}
					// �ر�����
					CloseSec(fp->Drive, SecIndex);
				}
			}
		}
	}
	return Rt;
}

//=================================================================================================
// �﷨��ʽ:	unsigned long int FileRead(void *Buf, unsigned long int Size, HANDLE Handle)
// ʵ�ֹ���:	��ȡ�ļ�
// ����:		Buf	-	������ص�����
//				Size	-	Ҫ�����ֽ���
//				Handle	-	�ļ����
// ����ֵ:		ʵ�ʶ������ֽ���
//=================================================================================================
unsigned long int FileRead(void *Buf, unsigned long int Size, HANDLE Handle)
{
	unsigned char *temp;
	unsigned long int i;
	temp = (unsigned char *)Buf;
	for (i = 0; i < Size; i++)
	{
		if (FileGetCh(temp, Handle) != RETURN_OK)
		{
			break;
		}
		temp++;
	}
	return i;
}

//====================================================================================
// �﷨��ʽ:	unsigned char FilePutCh(unsigned char Ch, HANDLE Handle)
// ʵ�ֹ���:	дһ���ֽڵ��ļ�
// ����:		Ch	-	Ҫд������
//				Handle	-	�ļ����
// ����ֵ:		RETURN_OK	-	�ɹ�
//=====================================================================================
unsigned char FilePutCh(unsigned char Ch, HANDLE Handle)
{
	unsigned long int i, j, SecIndex;
	MY_FILE *fp;
	Disk_Info * DiskInfo;
	unsigned char Rt;
	unsigned char *Buf;
	Rt = PARAMETER_ERR;
	fp = FileInfo + Handle;
	if (Handle >= 0 && Handle < MAX_OPEN_FILES)      // Handle�Ƿ���Ч
	if (((fp->Flags) & FILE_FLAGS_WRITE) != 0)       // ��Ӧ�Ĵ��ļ���Ϣ���Ƿ���ʹ��
	{
		Rt = FILE_EOF;
		if (fp->Offset <= fp->FileSize)
		{
			Rt = NOT_FIND_DISK;
                // ��ȡ�ļ������߼�����Ϣ
			DiskInfo = GetDiskInfo(fp->Drive);
			if (DiskInfo != NULL)
			{
				// �����ļ�ָ��
				j = (fp->Offset) % (DiskInfo->SecPerClus * DiskInfo->BytsPerSec); 
                
				if (j == 0)
				{
					if (fp->Offset < fp->FileSize)
					{
						fp->Clus = FATGetNextClus(fp->Drive, fp->Clus);
					}
					else
					{
						i = FATAddClus(fp->Drive, fp->Clus);
						if (i >= BAD_CLUS)
						{
							return DISK_FULL;
						}
						fp->Clus = i;
						if (fp->FstClus == EMPTY_CLUS)
						{
							fp->FstClus = i;
						}
					}
				}
                
				// ����������������
				i = j / DiskInfo->BytsPerSec;
				j = j % DiskInfo->BytsPerSec;
				SecIndex = (fp->Clus - 2) * DiskInfo->SecPerClus + DiskInfo->DataStartSec + i;
				Rt = SECTOR_READ_ERR;
				// �������ļ���Ϣ��
				fp->Offset++;
				if (fp->Offset > fp->FileSize)
				{
					fp->FileSize = fp->Offset;
				}
				// ������
				Buf = OpenSec(fp->Drive, SecIndex);
				if (Buf != NULL)
				{
					// д��������
					if (ReadSec(fp->Drive, SecIndex) == RETURN_OK)
					{
						// �洢����
						Buf[j] = Ch;
						WriteSec(fp->Drive, SecIndex);
						Rt = RETURN_OK;
					}
					CloseSec(fp->Drive, SecIndex);
				}
			}
		}
	}
	return Rt;
}

//====================================================================================================
// �﷨��ʽ:	unsigned long int FileWrite(void *Buf, unsigned long int Size, HANDLE Handle)
// ʵ�ֹ���:	д�ļ�
// ����:		Buf		-	Ҫд������
//				Size	-	Ҫд���ֽ���
//				Handle	-	�ļ����
// ����ֵ:		ʵ��д���ֽ���
//====================================================================================================
unsigned long int FileWrite(void *Buf, unsigned long int Size, HANDLE Handle)
{
	unsigned char *temp;
	unsigned long int i;
	temp = (unsigned char *)Buf;
	for (i = 0; i < Size; i++)
	{
		if (FilePutCh(*temp, Handle) != RETURN_OK)
		{
			break;
		}
		temp++;
	}
	return i;
}

//====================================================
// �﷨��ʽ:	void FileCloseAll(void)
// ʵ�ֹ���:	�ر����д򿪵��ļ�
// ����:		��
// ����ֵ:		��
//====================================================
void FileCloseAll(void)
{
	unsigned long int i;
	for (i = 0; i < MAX_OPEN_FILES; i++)
	{
		FileClose(i);
	}
	AllCacheWriteBack();
}

//=====================================================================
// �﷨��ʽ:	unsigned char FileEof(HANDLE Handle)
// ��������:	�ж��ļ��Ƿ񵽶�\д���ļ�β
// ����:		Handle:�ļ����
// ����ֵ:		0:��
//				1:�� 
//=====================================================================
unsigned char FileEof(HANDLE Handle)
{
	if (Handle < MAX_OPEN_FILES)
	if (FileInfo[Handle].Offset < FileInfo[Handle].FileSize)
	{
		return 0;
	}
	return 1;
}

//=========================================================================================================
// �﷨��ʽ:	unsigned char FileSeek(HANDLE Handle, long int offset, unsigned char Whence)
// ʵ�ֹ���:	�ƶ��ļ���\дλ��
// ����:		Handle	-	�ļ����
//				offset	-	�ƶ�ƫ����
//				Whence	-	�ƶ�ģʽ
//							SEEK_SET	-	���ļ�ͷ����
//							SEEK_CUR	-	�ӵ�ǰλ�ü���
//							SEEK_END	-	���ļ�β����
// ����ֵ:		��
//==========================================================================================================
unsigned char FileSeek(HANDLE Handle, long int offset, unsigned char Whence)
{
	unsigned char Rt;
	unsigned long int i, Clus;
	MY_FILE *fp;
	Disk_Info * DiskInfo;
	Rt = PARAMETER_ERR;
	fp = FileInfo + Handle;
	if (Handle >= 0 && Handle < MAX_OPEN_FILES)     // Handle�Ƿ���Ч
	if (fp->Flags  != 0)                            // ��Ӧ�Ĵ��ļ���Ϣ���Ƿ���ʹ��
	{
		Rt = RETURN_OK;
		switch (Whence)
		{
			case SEEK_END:             // ���ļ�β����
				fp->Offset = fp->FileSize - offset;
				offset = -offset;
				break;
			case SEEK_SET:
				fp->Offset = offset;
				break;
			case SEEK_CUR:             // �ӵ�ǰλ�ü��� ���ﲻ��break����ȷ��
				i = fp->Offset + offset;
				break;
			default:
				Rt = PARAMETER_ERR;
				break;
		}
		// �ı䵱ǰ�غ�
		if (Rt == RETURN_OK)
		{
			if (fp->Offset > fp->FileSize)
			{
				if (offset > 0)
				{
					fp->Offset = fp->FileSize;
				}
				else
				{
					fp->Offset = 0;
				}
			}
			Rt = NOT_FIND_DISK;
			DiskInfo = GetDiskInfo(fp->Drive);
			if (DiskInfo != NULL)
			{
				Rt = RETURN_OK;
				i = fp->Offset / (DiskInfo->BytsPerSec * DiskInfo->SecPerClus);
				Clus = fp->FstClus;
				for (; i != 0; i--)
				{
					Clus = FATGetNextClus(fp->Drive, Clus);
					if (Clus >= BAD_CLUS)
					{
						Rt = FAT_ERR;
						break;
					}
				}
				fp->Clus = Clus;
			}
		}
	}
	return Rt;
}

//======================================================
// 文件名称：	FAT_File.c
// 功能描述：	文件系统的文件基本操作程序
// 维护记录：	2006-8-11	v1.0
//======================================================

#define FAT_Dir
#define FAT_File
#include "FAT.h"
#include "SD\SD_INC.h"

extern char *strupr(char *Str);
extern unsigned char GetDrive(char *Path);
extern unsigned long int _GetFDTInfo(char *FDTName, char *DirFileName);
static MY_FILE FileInfo[MAX_OPEN_FILES];	// 打开文件信息表 

//====================================================
// 语法格式:	void FileInit(void)
// 实现功能:	初始化文件指针系统
// 参数:		无
// 返回值:		无
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
// 语法格式:	HANDLE FindOpenFile(unsigned char Drive, unsigned long int DirClus, char FileName[11])
// 实现功能:	查找已打开的指定文件的文件句柄，内部使用
// 参数:		Drive:文件所在的逻辑盘号
//				DirClus:文件所在目录的开始簇号
//				FileName:内部文件名
// 返回值:		文件句柄，Not_Open_FILE为没有打开
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
// 语法格式:	unsigned char RemoveFile(char *DirFileName)
// 实现功能:	删除文件
// 参数:		DirFileName	-	用户使用的文件名
// 返回值:		RETURN_OK	-	成功
//==================================================================
unsigned char RemoveFile(char *DirFileName)
{
	unsigned long int ClusIndex, ClusIndex1;
	unsigned char Drive, Rt;
	char DirName[12];
	FDT temp;
	strupr(DirFileName);                       // 变为大写
	ClusIndex = _GetFDTInfo(DirName, DirFileName);
	Rt = PATH_NOT_FIND;
	if (ClusIndex != BAD_CLUS)
	{
		Drive = GetDrive(DirFileName);     // 获取文件所在逻辑盘号
		Rt = FindFDTInfo(&temp, Drive, ClusIndex, DirName);
		if (Rt == RETURN_OK)
		{
			Rt = NOT_FIND_FILE;
			if ((temp.Attr & ATTR_DIRECTORY) == 0)  // 是文件才删除
			{
				Rt = FILE_LOCK;
				if (FindOpenFile(Drive, ClusIndex, DirName) < MAX_OPEN_FILES)
				{
					// 文件没有打开才删除
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
// 语法格式:	HANDLE FindOpenFileW(unsigned char Drive, unsigned long int DirClus, char FileName[11])
// 实现功能:	查找以写方式打开的指定文件的文件句柄，内部使用
// 参数:		Drive	-	文件所在的逻辑盘号
//				DirClus	-	文件所在目录的开始簇号
//				FileName	-	内部文件名
// 返回值:		文件句柄	-	Not_Open_FILE为没有打开
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
// 语法格式:	HANDLE _FileOpenR(char *DirFileName)
// 实现功能:	只读方式打开文件，内部使用
// 参数:		DirFileName:用户使用的文件名
// 返回值:		文件句柄	-	Not_Open_FILE为不能打开
//======================================================================
HANDLE _FileOpenR(char *DirFileName)
{
	MY_FILE *fp;
	HANDLE Rt;
	FDT FileFDT;
	// 查找空闲文件登记项
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
		// 获取目录开始簇号和文件名
		fp->DirClus = _GetFDTInfo(fp->Name, DirFileName);
		if (fp->DirClus < BAD_CLUS)
		{
			fp->Drive = GetDrive(DirFileName);     // 获取文件所在逻辑盘号
			// 获取文件信息
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
// 语法格式:	HANDLE _FileOpenW(char *DirFileName)
// 实现功能:	只写方式打开文件，内部使用
// 参数:		DirFileName	-	用户使用的文件名
// 返回值:		文件句柄	-	Not_Open_FILE为不能打开
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
        
		// 获取目录开始簇号和文件名
		fp->DirClus = _GetFDTInfo(fp->Name, DirFileName);
		if (fp->DirClus < BAD_CLUS)
		{
			fp->Drive = GetDrive(DirFileName);     // 获取文件所在逻辑盘号
			// 文件已经以读写方式打开，不能再次以读写方式打开
			if (FindOpenFileW(fp->Drive, fp->DirClus, fp->Name) == Not_Open_FILE)
			{
				if (FindFDTInfo(&temp, fp->Drive, fp->DirClus, fp->Name) == RETURN_OK)
				{
					if ((temp.Attr & ATTR_DIRECTORY) != 0)
					{
						return Not_Open_FILE;
					}
					if (RemoveFile(DirFileName) != RETURN_OK)   // 删除文件
					{
						return Not_Open_FILE;
					}
				}
				// 创建文件
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
				if (AddFDT(fp->Drive, fp->DirClus, &temp) == RETURN_OK)       // 增加文件
				{
					// 设置文件信息
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
// 语法格式:	HANDLE _FileOpenRW(char *DirFileName)
// 实现功能:	只读写方式打开文件，内部使用
// 参数: 		DirFileName	-	用户使用的文件名
// 返回值: 		文件句柄	-	Not_Open_FILE为不能打开
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
// 语法格式:	HANDLE FileOpen(char *DirFileName, char *Type)
// 实现功能:	以指定方式打开文件
// 参数			DirFileName	-	用户使用的文件名
//				Type	-	打开方式
// 返回值:		文件句柄	-	Not_Open_FILE为不能打开
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
// 语法格式:	unsigned char FileClose(HANDLE Handle)
// 实现功能:	关闭指定文件
// 参数:		Handle	-	文件句柄
// 返回值:		RETURN_OK	-	成功
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
// 语法格式:	unsigned char FileGetCh(unsigned char *Ch, HANDLE Handle)
// 实现功能:	从文件读一个字节
// 参数:		Ch	-	返回读到的数据
//				Handle	-	文件句柄
// 返回值:		RETURN_OK	-	成功
//===============================================================================
unsigned char FileGetCh(unsigned char *Ch, HANDLE Handle)
{
	unsigned long int i, j, SecIndex;
	MY_FILE *fp;
	Disk_Info *DiskInfo;
	unsigned char Rt;
	unsigned char *Buf;
	Rt = PARAMETER_ERR;	// 非法参数							
	fp = FileInfo + Handle;
	if (Handle >= 0 && Handle < MAX_OPEN_FILES)    // Handle是否有效
	if (fp->Flags != 0)                            // 对应的打开文件信息表是否已使用
	{
		Rt = FILE_EOF;
		if (fp->Offset >= fp->FileSize)
		{
			// 如果读数据超出文件的长度，看一看谁否有别的程序正在写这个文件
			// 如果有，则可能文件的长度有变化，更正之
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
		if (fp->Offset < fp->FileSize)              // 是否到文件结束
		{
			Rt = NOT_FIND_DISK;                        
			// 获取文件所在逻辑盘信息
			DiskInfo = GetDiskInfo(fp->Drive);
			if (DiskInfo != NULL)
			{
				// 计算数据所在扇区
				j = fp->Offset % (DiskInfo->SecPerClus * DiskInfo->BytsPerSec);
				i = j / DiskInfo->BytsPerSec;
				j = j % DiskInfo->BytsPerSec;
				SecIndex = (fp->Clus - 2) * DiskInfo->SecPerClus + DiskInfo->DataStartSec + i;
				Rt = SECTOR_READ_ERR;
				Buf = OpenSec(fp->Drive, SecIndex);
				// 打开扇区
				if (Buf != NULL)
				{
					// 读取扇区数据
					if (ReadSec(fp->Drive, SecIndex) == RETURN_OK)
					{
						// 读取数据
						*Ch = Buf[j];
						// 调整文件指针
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
					// 关闭扇区
					CloseSec(fp->Drive, SecIndex);
				}
			}
		}
	}
	return Rt;
}

//=================================================================================================
// 语法格式:	unsigned long int FileRead(void *Buf, unsigned long int Size, HANDLE Handle)
// 实现功能:	读取文件
// 参数:		Buf	-	保存读回的数据
//				Size	-	要读的字节数
//				Handle	-	文件句柄
// 返回值:		实际读到的字节数
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
// 语法格式:	unsigned char FilePutCh(unsigned char Ch, HANDLE Handle)
// 实现功能:	写一个字节到文件
// 参数:		Ch	-	要写的数据
//				Handle	-	文件句柄
// 返回值:		RETURN_OK	-	成功
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
	if (Handle >= 0 && Handle < MAX_OPEN_FILES)      // Handle是否有效
	if (((fp->Flags) & FILE_FLAGS_WRITE) != 0)       // 对应的打开文件信息表是否已使用
	{
		Rt = FILE_EOF;
		if (fp->Offset <= fp->FileSize)
		{
			Rt = NOT_FIND_DISK;
                // 获取文件所在逻辑盘信息
			DiskInfo = GetDiskInfo(fp->Drive);
			if (DiskInfo != NULL)
			{
				// 调整文件指针
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
                
				// 计算数据所在扇区
				i = j / DiskInfo->BytsPerSec;
				j = j % DiskInfo->BytsPerSec;
				SecIndex = (fp->Clus - 2) * DiskInfo->SecPerClus + DiskInfo->DataStartSec + i;
				Rt = SECTOR_READ_ERR;
				// 调整打开文件信息表
				fp->Offset++;
				if (fp->Offset > fp->FileSize)
				{
					fp->FileSize = fp->Offset;
				}
				// 打开扇区
				Buf = OpenSec(fp->Drive, SecIndex);
				if (Buf != NULL)
				{
					// 写扇区数据
					if (ReadSec(fp->Drive, SecIndex) == RETURN_OK)
					{
						// 存储数据
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
// 语法格式:	unsigned long int FileWrite(void *Buf, unsigned long int Size, HANDLE Handle)
// 实现功能:	写文件
// 参数:		Buf		-	要写的数据
//				Size	-	要写的字节数
//				Handle	-	文件句柄
// 返回值:		实际写的字节数
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
// 语法格式:	void FileCloseAll(void)
// 实现功能:	关闭所有打开的文件
// 参数:		无
// 返回值:		无
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
// 语法格式:	unsigned char FileEof(HANDLE Handle)
// 功能描述:	判断文件是否到读\写到文件尾
// 参数:		Handle:文件句柄
// 返回值:		0:否
//				1:是 
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
// 语法格式:	unsigned char FileSeek(HANDLE Handle, long int offset, unsigned char Whence)
// 实现功能:	移动文件读\写位置
// 参数:		Handle	-	文件句柄
//				offset	-	移动偏移量
//				Whence	-	移动模式
//							SEEK_SET	-	从文件头计算
//							SEEK_CUR	-	从当前位置计算
//							SEEK_END	-	从文件尾计算
// 返回值:		无
//==========================================================================================================
unsigned char FileSeek(HANDLE Handle, long int offset, unsigned char Whence)
{
	unsigned char Rt;
	unsigned long int i, Clus;
	MY_FILE *fp;
	Disk_Info * DiskInfo;
	Rt = PARAMETER_ERR;
	fp = FileInfo + Handle;
	if (Handle >= 0 && Handle < MAX_OPEN_FILES)     // Handle是否有效
	if (fp->Flags  != 0)                            // 对应的打开文件信息表是否已使用
	{
		Rt = RETURN_OK;
		switch (Whence)
		{
			case SEEK_END:             // 从文件尾计算
				fp->Offset = fp->FileSize - offset;
				offset = -offset;
				break;
			case SEEK_SET:
				fp->Offset = offset;
				break;
			case SEEK_CUR:             // 从当前位置计算 这里不用break是正确的
				i = fp->Offset + offset;
				break;
			default:
				Rt = PARAMETER_ERR;
				break;
		}
		// 改变当前簇号
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

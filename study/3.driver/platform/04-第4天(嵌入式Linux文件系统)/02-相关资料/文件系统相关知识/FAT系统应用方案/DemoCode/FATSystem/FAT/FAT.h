//==========================================================
// 文件名称：	FAT.h
// 功能描述：	FAT文件系统FAT表数据结构，及相关信息
// 维护记录：	2006-8-11	v1.0
//==========================================================
#define  NULL    (void *)0

#ifndef MAX_OPEN_FILES
#define MAX_OPEN_FILES              2        	//可以同时打开的文件数目
#endif

#ifndef DISK_CACHE_SIZE
#define DISK_CACHE_SIZE             512         //文件系统Cache大小(字节),应当是最大的扇区所占字节数
#endif

#ifndef MAX_DISK_CACHES
#define MAX_DISK_CACHES             1         	//文件系统Cache数目
#endif

#ifndef MAX_DRIVES
#define MAX_DRIVES                  1        	//可以同时加载的逻辑驱动器数目
#endif

#if MAX_OPEN_FILES < 0x8000
	typedef int HANDLE;
#else
	typedef long int HANDLE;
#endif


#define Not_Open_FILE               -1

// 底层驱动命令 
#define DISK_INIT                   0
#define DISK_CLOSE                  1
#define DISK_READ_SECTOR            2
#define DISK_WRITE_SECTOR           3

// 底层驱动返回值 
#define DISK_READ_OK                0x01
#define DISK_READ_NOT_OK            0x80
#define DISK_WRITE_OK               0x02
#define DISK_WRITE_NOT_OK           0x82
#define DISK_INIT_OK                0x03
#define DISK_INIT_NOT_OK            0x83
#define BAD_DISK_COMMAND            0xffff
#define SD_NO_ERR                   0x00

// 函数返回值 
#define RETURN_OK                   0x00		//操作成功
#define NOT_FIND_DISK               0x01		//逻辑盘不存在
#define DISK_FULL                   0x02		//逻辑盘满
#define SECTOR_NOT_IN_CACHE         0x03		//扇区没有被cache
#define NOT_EMPTY_CACHE             0x04		//没有空闲cache
#define SECTOR_READ_ERR             0x05		//读扇区错误
#define CLUSTER_NOT_IN_DISK         0x06		//逻辑盘中没有此簇
#define NOT_FIND_FDT                0x07		//没有发现文件(目录)
#define NOT_FAT_DISK                0x08		//非FAT文件系统
#define FDT_OVER                    0x09		//FDT索引超出范围
#define FDT_EXISTS                  0x0a		//文件(目录)已经存在
#define ROOT_FDT_FULL               0x0b		//根目录满
#define DIR_EMPTY                   0x0C		//目录空
#define DIR_NOT_EMPTY               0x0d		//目录不空
#define PATH_NOT_FIND               0x0e		//路径未找到
#define FAT_ERR                     0x0f		//FAT表错误
#define FILE_NAME_ERR               0x10		//文件(目录)名错误
#define FILE_EOF                    0x11		//文件结束
#define FILE_LOCK                   0x12		//文件被锁定
#define NOT_FIND_FILE               0x13		//没有发现指定文件
#define NOT_FIND_DIR                0x14		//没有发现指定目录
#define NOT_RUN                     0xfd		//命令未执行
#define BAD_COMMAND                 0xfe		//错误命令
#define PARAMETER_ERR               0xff		//非法参数

// 未使用的逻辑盘 
#define EMPTY_DRIVE                 0xff

// FAT类型 
#define FAT12                       0
#define FAT16                       1
#define FAT32                       2

// FDT文件属性
#define ATTR_READ_ONLY              0x01
#define ATTR_HIDDEN                 0x02
#define ATTR_SYSTEM                 0x04
#define ATTR_VOLUME_ID              0x08
#define ATTR_DIRECTORY              0x10
#define ATTR_ARCHIVE                0x20

// 簇类型
#define EMPTY_CLUS                  0
#define EMPTY_CLUS_1                1
#define BAD_CLUS                    0x0ffffff7L
#define EOF_CLUS_1                  0x0ffffff8L
#define EOF_CLUS_END                0x0fffffffL

// FDT类型 
#define EMPTY_FDT                   0
#define DEL_FDT                     ((char)(0xe5))
#define ESC_FDT                     0x05

// Cache状态
#define CACHE_READED                0x02
#define CACHE_WRITED                0x01

// 文件指针状态
#define FILE_FLAGS_READ             1 << 0			// 可读
#define FILE_FLAGS_WRITE            1 << 1          // 可写

// 文件指针调整方式
#define SEEK_SET    0				// 从文件开始处移动文件指针
#define SEEK_CUR    1               // 从文件当前位置移动文件指针
#define SEEK_END    2               // 从文件尾移动文件指针

// 给底层驱动读写命令的参数结构体
typedef struct _Disk_RW_Parameter
{
    unsigned char Drive;          	// 驱动器号
    unsigned long int SectorIndex;	// 操作的扇区
    unsigned long int RsvdForLow;	// 保留给底层驱动程序，由_Disk_Info中拷贝过来 
    unsigned char *Buf;           	// 数据存储位置
}Disk_RW_Parameter;

// 文件系统cache数据结构
typedef struct _Disk_Cache
{
    unsigned char Drive;          	// 驱动器号,EMPTY_DRIVE为空闲
    unsigned char Flag;           	// 状态
    unsigned int RW_ID;          	// 读写ID,用于释放Cache
    unsigned long int SecIndex;		// 缓冲的扇区索引
    unsigned char buf[DISK_CACHE_SIZE];
									// 缓冲区
}Disk_cache;

// 逻辑盘信息的数据结构
typedef struct _Disk_Info
{
    unsigned char Drive;			// 逻辑驱动器号，EMPTY_DRIVE为还未分配
    unsigned char FATType;			// 类型：FAT12、FAT16和FAT32
    unsigned char SecPerClus;		// 每簇扇区数,BytsPerSec*SecPerClus<=1024*32
    unsigned char NumFATs;			// FAT表数目,建议为2
    unsigned long int SecPerDisk;	// 逻辑驱动器包含扇区数
    unsigned long int BytsPerSec;	// 每扇区字节数为512
    unsigned long int RootDirTable;	// 根目录开始扇区号（FAT32为开始簇号）
    unsigned long int RootSecCnt;  	// 根目录占用扇区数
    unsigned long int FATStartSec; 	// FAT表开始扇区号
    unsigned long int FATSecCnt;  	// 每个FAT占用扇区数
    unsigned long int DataStartSec;	// 数据区开始扇区号
    unsigned long int ClusPerData;	// 数据区包含簇数
    unsigned long int PathClusIndex;// 当前路径的FDT表开始簇号, 0为根目录 
    unsigned long int RsvdForLow;	// 保留给底层驱动程序 
    unsigned int (*DiakCommand)(unsigned char Cammand, void *Parameter);
									// 对应的驱动程序
}Disk_Info;

// 目录表数据结构
typedef struct _FDT
{
    char Name[11];					//短文件名主文件名
    unsigned char Attr;				//文件属性
    unsigned char NTRes;			//保留给NT
    unsigned char CrtTimeTenth;		//建立时间（fat16保留）
    unsigned int CrtTime;			//建立时间（fat16保留）
    unsigned int CrtDate;			//建立日期（fat16保留）
    unsigned int LstAccDate;		//最后访问日期（fat16保留）
    unsigned int FstClusHI;			//起始簇号高两个字节（fat16保留）
    unsigned int WrtTime;			//最后写时间
    unsigned int WrtDate;			//最后写日期
    unsigned int FstClusLO;			//起始簇(cluster)号低两个字节
    unsigned long int FileSize;		//文件大小
} FDT;

// 文件信息结构体
typedef struct _FILE
{
    unsigned char Flags;			// 一些标志
    char Name[11];					// 文件名
    unsigned char Drive;			// 文件所在磁盘
    unsigned long int DirClus;		// 所在目录开始簇号
    unsigned long int FileSize;		// 文件大小
    unsigned long int FstClus;		// 起始簇号
    unsigned long int Clus;			// 当前簇号
    unsigned long int Offset;		// 文件指针偏移量
} MY_FILE;

#ifndef FAT_File
//	文件初始化
extern void FileInit(void);
//	删除文件
extern unsigned char RemoveFile(char *DirFileName);
//	以指定方式打开文件
extern HANDLE FileOpen(char *DirFileName, char *Type);
//关闭指定文件
extern unsigned char FileClose(HANDLE Handle);
//	从文件读一个字节
extern unsigned char FileGetCh(unsigned char *Ch, HANDLE Handle);
//	读取文件
extern unsigned long int FileRead(void *Buf, unsigned long int Size, HANDLE Handle);
//	写一个字节到文件
extern unsigned char FilePutCh(unsigned char Ch, HANDLE Handle);
//	写文件
extern unsigned long int FileWrite(void *Buf, unsigned long int Size, HANDLE Handle);
//关闭所有打开的文件
extern void FileCloseAll(void);
//	判断文件是否到读\写到文件尾
extern unsigned char FileEof(HANDLE Handle);
//移动文件读\写位置
extern unsigned char FileSeek(HANDLE Handle, long int offset, unsigned char Whence);
#endif

#ifndef FAT_Dir
//	变为大写
extern char *strupr(char *Str);
//获取指定目录的逻辑盘号
extern unsigned char GetDrive(char *Path);
//	改变当前逻辑盘
extern unsigned char ChangeDrive(char *Drive);
//	获取指定目录开始簇号
extern unsigned long int GetDirClusIndex(char *Path);
//	建立目录
extern unsigned char MakeDir(char *Path);
//	删除目录
extern unsigned char RemoveDir(char *Path);
//	改变当前目录
extern unsigned char ChangeDir(char *Path);
#endif

#ifndef FAT_FDT
////	用户文件\目录名转换为系统名
//unsigned char StrToFDTName(char *FDTName, char *Str)
//	将指定簇所有数据清零
extern unsigned char ClearClus(unsigned char Drive, unsigned long int Index);
//	改变指定目录指定文件（目录）的属性
extern unsigned char ChangeFDT(unsigned char Drive, unsigned long int ClusIndex, FDT *FDTData);
//	读取FDT信息
extern unsigned char  ReadFDTInfo(FDT *Rt, unsigned char Drive, unsigned long int SecIndex, unsigned int ByteIndex);
//	写FDT信息
extern unsigned char  WriteFDTInfo(FDT *FDTData, unsigned char Drive, unsigned long int SecIndex, unsigned int ByteIndex);
//	获取指定目录指定文件（目录）信息
extern unsigned char GetFDTInfo(FDT *Rt,unsigned char Drive, unsigned long int ClusIndex, unsigned long int Index);
//	设置指定目录指定文件（目录）信息
extern unsigned char SetFDTInfo(unsigned char Drive, unsigned long int ClusIndex, unsigned long int Index, FDT *FDTData);
//	在指定目录查找指定文件（目录）
extern unsigned char FindFDTInfo(FDT *Rt,unsigned char Drive, unsigned long int ClusIndex, char FileName[]);
//	在指定目录查增加指定文件（目录）
extern unsigned char AddFDT(unsigned char Drive, unsigned long int ClusIndex, FDT *FDTData);
//	在指定目录删除指定文件（目录
extern unsigned char DelFDT(unsigned char Drive, unsigned long int ClusIndex, char FileName[]);
//	目录是否为空
extern unsigned char DirIsEmpty(unsigned char Drive, unsigned long int ClusIndex);
//	在指定目录查看指定文件（目录）是否存在
extern unsigned char FDTIsLie(unsigned char Drive, unsigned long int ClusIndex, char FileName[]);
#endif

#ifndef FAT_Disk
//	初始化逻辑盘管理模块
extern void DiskInit(void);
//	获取指定逻辑盘信息
extern Disk_Info *GetDiskInfo(unsigned char Drive);
//	获取空闲逻辑盘
extern Disk_Info  *GetEmptyDiskInfoAddr(void);
//	增加一个底层驱动程序
extern void AddFileDriver(unsigned int  (* DiakCommand)(unsigned char Cammand, void *Parameter));
//	删除一个底层驱动程序
extern void RemoveFileDriver(unsigned char Drive);
#endif

#ifndef FAT_Cache
//	初始化磁盘cache
extern void CacheInit(void);
//	解锁指定cache（允许切换出内存）
extern void CloseSec(unsigned char Drive, unsigned long int Index);
//	把指定扇区写回逻辑盘
extern void CacheWriteBack2(unsigned int Index);
//	把指定cache写回逻辑盘
extern void CacheWriteBack(unsigned char Drive, unsigned long int Index);
//	把所有已改变的扇区写回逻辑盘
extern void AllCacheWriteBack(void);
//	获取一个cache
extern unsigned int GetCache(void);
//	为逻辑盘上的一个扇区打开一个cache并锁定
extern unsigned char *OpenSec(unsigned char Drive, unsigned long int Index);
//	从逻辑盘读扇区
extern unsigned char ReadSec(unsigned char Drive, unsigned long int Index);
//	说明指定逻辑盘的指定一个扇区被改写
extern void WriteSec(unsigned char Drive, unsigned long int Index);
#endif

#ifndef FAT_Clus
//	返回FAT表指定簇的下一个簇号
extern unsigned long int FATGetNextClus(unsigned char Drive, unsigned long int Index);
//	删除指定簇链
void FATSetNextClus(unsigned char Drive, unsigned long int Index, unsigned long int Next);
//	为指定簇链增加一个簇
extern unsigned long int FATAddClus(unsigned char Drive, unsigned long int Index);
//	删除指定簇链
extern void FATDelClusChain(unsigned char Drive, unsigned long int Index);
#endif

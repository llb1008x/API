#ifndef FILESYS_H
#define FILESYS_H

#define DEVNAME "/dev/fd0H1440"		/*要打开的设备文件名,表示1.44MB软盘*/

#define DIR_ENTRY_SIZE 32		/*目录项大小*/
#define SECTOR_SIZE 512			/*每个扇区大小*/
#define FAT_ONE_OFFSET 0X200		/*第一个FAT表起始地址*/
#define FAT_TWO_OFFSET 0X1400		/*第二个FAT表起始地址*/
#define ROOTDIR_OFFSET 0X2600		/*根目录区起始地址*/
#define DATA_OFFSET 0x4200		/*数据区起始地址*/

/* 属性位掩码 */
#define ATTR_READONLY 0x01	// 00000001
#define ATTR_HIDDEN 0x02	// 00000010
#define ATTR_SYSTEM 0x04	// 00000100
#define ATTR_VLABEL 0x08	// 00001000
#define ATTR_SUBDIR 0x10	// 00010000
#define ATTR_ARCHIVE 0x20	// 00100000

/* 时间掩码5:6:5 */
#define MASK_HOUR 0xf800	// 1111100000000000
#define MASK_MIN 0x07e0		// 11111100000
#define MASK_SEC 0x001f		// 11111

/* 日期掩码7:4:5 */
#define MASK_YEAR 0xfe00	// 1111111000000000
#define MASK_MONTH 0x01e0	// 111100000
#define MASK_DAY 0x001f		// 11111

/* 这个结构体定义了启动扇区的内容 */
struct BootDescriptor_t{
	unsigned char Oem_name[9];	/*0x03 - 0x0a*/
	int BytesPerSector;		/*0x0b - 0x0c*/
	int SectorsPerCluster;		/*0x0d       */
	int ReservedSectors;		/*0x0e - 0x0f*/
	int FATs;			/*0x10 	     */
	int RootDirEntries;		/*0x11 - 0x12*/
	int LogicSectors;		/*0x13 - 0x14*/
	int MediaType;			/*0x15 	     */
	int SectorsPerFAT;		/*0x16 - 0x17*/
	int SectorsPerTrack;		/*0x18 - 0x19*/
	int Heads;			/*0x1a - 0x1b*/
	int HiddenSectors;		/*0x1c - 0x1d*/
};

/* 这个结构体定义了根目录区目录项的内容 */
struct Entry{
	unsigned char short_name[12];	/*字节0-10,11个字节长度的名字*/
	unsigned char long_name[27];	/*未使用,26个字节长度        */
	unsigned short hour,min,sec;	/*22 - 23字节*/
	unsigned short year,month,day;	/*24 - 25字节*/
	unsigned short FirstCluster;	/*26 - 27字节*/
	unsigned int size;		/*28 - 31字节*/
	//属性位
	// 7 6 5 4 3 2 1 0
	// N N A D V S H R	N:未使用
	unsigned char readonly:1;	/* R(只读)     */
	unsigned char hidden:1;		/* H(隐藏)     */
	unsigned char system:1;		/* S(系统文件) */
	unsigned char vlabel:1;		/* V(卷标)     */
	unsigned char subdir:1;		/* D(子目录)   */
	unsigned char archive:1;	/* A(存档)     */
};

/* 下面是函数声明 */
//int fd_load();
int fd_ls();
int fd_cd(char *dir);
int fd_rm(char *file_name);

void findDate(unsigned short *year,
	unsigned short *month,
	unsigned short *day,
	unsigned char info[2]);

void findTime(unsigned short *hour,
	unsigned short *min,
	unsigned short *sec,
	unsigned char info[2]);

int ReadFat();
int WriteFat();
void ScanBootSector();
void ScanRootEntry();
int ScanEntry(char *entryname, struct Entry *pentry, int mode);
int GetEntry(struct Entry *entry);
void FileNameFormat(unsigned char* name);
unsigned short GetFatCluster(unsigned short prev);
void ClearFatCluster(unsigned short cluster);
#endif

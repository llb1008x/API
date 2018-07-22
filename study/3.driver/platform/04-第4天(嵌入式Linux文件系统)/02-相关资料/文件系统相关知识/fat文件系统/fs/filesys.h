#ifndef FILESYS_H
#define FILESYS_H

#define DEVNAME "/dev/fd0H1440"		/*Ҫ�򿪵��豸�ļ���,��ʾ1.44MB����*/

#define DIR_ENTRY_SIZE 32		/*Ŀ¼���С*/
#define SECTOR_SIZE 512			/*ÿ��������С*/
#define FAT_ONE_OFFSET 0X200		/*��һ��FAT����ʼ��ַ*/
#define FAT_TWO_OFFSET 0X1400		/*�ڶ���FAT����ʼ��ַ*/
#define ROOTDIR_OFFSET 0X2600		/*��Ŀ¼����ʼ��ַ*/
#define DATA_OFFSET 0x4200		/*��������ʼ��ַ*/

/* ����λ���� */
#define ATTR_READONLY 0x01	// 00000001
#define ATTR_HIDDEN 0x02	// 00000010
#define ATTR_SYSTEM 0x04	// 00000100
#define ATTR_VLABEL 0x08	// 00001000
#define ATTR_SUBDIR 0x10	// 00010000
#define ATTR_ARCHIVE 0x20	// 00100000

/* ʱ������5:6:5 */
#define MASK_HOUR 0xf800	// 1111100000000000
#define MASK_MIN 0x07e0		// 11111100000
#define MASK_SEC 0x001f		// 11111

/* ��������7:4:5 */
#define MASK_YEAR 0xfe00	// 1111111000000000
#define MASK_MONTH 0x01e0	// 111100000
#define MASK_DAY 0x001f		// 11111

/* ����ṹ�嶨������������������ */
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

/* ����ṹ�嶨���˸�Ŀ¼��Ŀ¼������� */
struct Entry{
	unsigned char short_name[12];	/*�ֽ�0-10,11���ֽڳ��ȵ�����*/
	unsigned char long_name[27];	/*δʹ��,26���ֽڳ���        */
	unsigned short hour,min,sec;	/*22 - 23�ֽ�*/
	unsigned short year,month,day;	/*24 - 25�ֽ�*/
	unsigned short FirstCluster;	/*26 - 27�ֽ�*/
	unsigned int size;		/*28 - 31�ֽ�*/
	//����λ
	// 7 6 5 4 3 2 1 0
	// N N A D V S H R	N:δʹ��
	unsigned char readonly:1;	/* R(ֻ��)     */
	unsigned char hidden:1;		/* H(����)     */
	unsigned char system:1;		/* S(ϵͳ�ļ�) */
	unsigned char vlabel:1;		/* V(���)     */
	unsigned char subdir:1;		/* D(��Ŀ¼)   */
	unsigned char archive:1;	/* A(�浵)     */
};

/* �����Ǻ������� */
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

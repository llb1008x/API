//==========================================================
// �ļ����ƣ�	FAT.h
// ����������	FAT�ļ�ϵͳFAT�����ݽṹ���������Ϣ
// ά����¼��	2006-8-11	v1.0
//==========================================================
#define  NULL    (void *)0

#ifndef MAX_OPEN_FILES
#define MAX_OPEN_FILES              2        	//����ͬʱ�򿪵��ļ���Ŀ
#endif

#ifndef DISK_CACHE_SIZE
#define DISK_CACHE_SIZE             512         //�ļ�ϵͳCache��С(�ֽ�),Ӧ��������������ռ�ֽ���
#endif

#ifndef MAX_DISK_CACHES
#define MAX_DISK_CACHES             1         	//�ļ�ϵͳCache��Ŀ
#endif

#ifndef MAX_DRIVES
#define MAX_DRIVES                  1        	//����ͬʱ���ص��߼���������Ŀ
#endif

#if MAX_OPEN_FILES < 0x8000
	typedef int HANDLE;
#else
	typedef long int HANDLE;
#endif


#define Not_Open_FILE               -1

// �ײ��������� 
#define DISK_INIT                   0
#define DISK_CLOSE                  1
#define DISK_READ_SECTOR            2
#define DISK_WRITE_SECTOR           3

// �ײ���������ֵ 
#define DISK_READ_OK                0x01
#define DISK_READ_NOT_OK            0x80
#define DISK_WRITE_OK               0x02
#define DISK_WRITE_NOT_OK           0x82
#define DISK_INIT_OK                0x03
#define DISK_INIT_NOT_OK            0x83
#define BAD_DISK_COMMAND            0xffff
#define SD_NO_ERR                   0x00

// ��������ֵ 
#define RETURN_OK                   0x00		//�����ɹ�
#define NOT_FIND_DISK               0x01		//�߼��̲�����
#define DISK_FULL                   0x02		//�߼�����
#define SECTOR_NOT_IN_CACHE         0x03		//����û�б�cache
#define NOT_EMPTY_CACHE             0x04		//û�п���cache
#define SECTOR_READ_ERR             0x05		//����������
#define CLUSTER_NOT_IN_DISK         0x06		//�߼�����û�д˴�
#define NOT_FIND_FDT                0x07		//û�з����ļ�(Ŀ¼)
#define NOT_FAT_DISK                0x08		//��FAT�ļ�ϵͳ
#define FDT_OVER                    0x09		//FDT����������Χ
#define FDT_EXISTS                  0x0a		//�ļ�(Ŀ¼)�Ѿ�����
#define ROOT_FDT_FULL               0x0b		//��Ŀ¼��
#define DIR_EMPTY                   0x0C		//Ŀ¼��
#define DIR_NOT_EMPTY               0x0d		//Ŀ¼����
#define PATH_NOT_FIND               0x0e		//·��δ�ҵ�
#define FAT_ERR                     0x0f		//FAT�����
#define FILE_NAME_ERR               0x10		//�ļ�(Ŀ¼)������
#define FILE_EOF                    0x11		//�ļ�����
#define FILE_LOCK                   0x12		//�ļ�������
#define NOT_FIND_FILE               0x13		//û�з���ָ���ļ�
#define NOT_FIND_DIR                0x14		//û�з���ָ��Ŀ¼
#define NOT_RUN                     0xfd		//����δִ��
#define BAD_COMMAND                 0xfe		//��������
#define PARAMETER_ERR               0xff		//�Ƿ�����

// δʹ�õ��߼��� 
#define EMPTY_DRIVE                 0xff

// FAT���� 
#define FAT12                       0
#define FAT16                       1
#define FAT32                       2

// FDT�ļ�����
#define ATTR_READ_ONLY              0x01
#define ATTR_HIDDEN                 0x02
#define ATTR_SYSTEM                 0x04
#define ATTR_VOLUME_ID              0x08
#define ATTR_DIRECTORY              0x10
#define ATTR_ARCHIVE                0x20

// ������
#define EMPTY_CLUS                  0
#define EMPTY_CLUS_1                1
#define BAD_CLUS                    0x0ffffff7L
#define EOF_CLUS_1                  0x0ffffff8L
#define EOF_CLUS_END                0x0fffffffL

// FDT���� 
#define EMPTY_FDT                   0
#define DEL_FDT                     ((char)(0xe5))
#define ESC_FDT                     0x05

// Cache״̬
#define CACHE_READED                0x02
#define CACHE_WRITED                0x01

// �ļ�ָ��״̬
#define FILE_FLAGS_READ             1 << 0			// �ɶ�
#define FILE_FLAGS_WRITE            1 << 1          // ��д

// �ļ�ָ�������ʽ
#define SEEK_SET    0				// ���ļ���ʼ���ƶ��ļ�ָ��
#define SEEK_CUR    1               // ���ļ���ǰλ���ƶ��ļ�ָ��
#define SEEK_END    2               // ���ļ�β�ƶ��ļ�ָ��

// ���ײ�������д����Ĳ����ṹ��
typedef struct _Disk_RW_Parameter
{
    unsigned char Drive;          	// ��������
    unsigned long int SectorIndex;	// ����������
    unsigned long int RsvdForLow;	// �������ײ�����������_Disk_Info�п������� 
    unsigned char *Buf;           	// ���ݴ洢λ��
}Disk_RW_Parameter;

// �ļ�ϵͳcache���ݽṹ
typedef struct _Disk_Cache
{
    unsigned char Drive;          	// ��������,EMPTY_DRIVEΪ����
    unsigned char Flag;           	// ״̬
    unsigned int RW_ID;          	// ��дID,�����ͷ�Cache
    unsigned long int SecIndex;		// �������������
    unsigned char buf[DISK_CACHE_SIZE];
									// ������
}Disk_cache;

// �߼�����Ϣ�����ݽṹ
typedef struct _Disk_Info
{
    unsigned char Drive;			// �߼��������ţ�EMPTY_DRIVEΪ��δ����
    unsigned char FATType;			// ���ͣ�FAT12��FAT16��FAT32
    unsigned char SecPerClus;		// ÿ��������,BytsPerSec*SecPerClus<=1024*32
    unsigned char NumFATs;			// FAT����Ŀ,����Ϊ2
    unsigned long int SecPerDisk;	// �߼�����������������
    unsigned long int BytsPerSec;	// ÿ�����ֽ���Ϊ512
    unsigned long int RootDirTable;	// ��Ŀ¼��ʼ�����ţ�FAT32Ϊ��ʼ�غţ�
    unsigned long int RootSecCnt;  	// ��Ŀ¼ռ��������
    unsigned long int FATStartSec; 	// FAT��ʼ������
    unsigned long int FATSecCnt;  	// ÿ��FATռ��������
    unsigned long int DataStartSec;	// ��������ʼ������
    unsigned long int ClusPerData;	// ��������������
    unsigned long int PathClusIndex;// ��ǰ·����FDT��ʼ�غ�, 0Ϊ��Ŀ¼ 
    unsigned long int RsvdForLow;	// �������ײ��������� 
    unsigned int (*DiakCommand)(unsigned char Cammand, void *Parameter);
									// ��Ӧ����������
}Disk_Info;

// Ŀ¼�����ݽṹ
typedef struct _FDT
{
    char Name[11];					//���ļ������ļ���
    unsigned char Attr;				//�ļ�����
    unsigned char NTRes;			//������NT
    unsigned char CrtTimeTenth;		//����ʱ�䣨fat16������
    unsigned int CrtTime;			//����ʱ�䣨fat16������
    unsigned int CrtDate;			//�������ڣ�fat16������
    unsigned int LstAccDate;		//���������ڣ�fat16������
    unsigned int FstClusHI;			//��ʼ�غŸ������ֽڣ�fat16������
    unsigned int WrtTime;			//���дʱ��
    unsigned int WrtDate;			//���д����
    unsigned int FstClusLO;			//��ʼ��(cluster)�ŵ������ֽ�
    unsigned long int FileSize;		//�ļ���С
} FDT;

// �ļ���Ϣ�ṹ��
typedef struct _FILE
{
    unsigned char Flags;			// һЩ��־
    char Name[11];					// �ļ���
    unsigned char Drive;			// �ļ����ڴ���
    unsigned long int DirClus;		// ����Ŀ¼��ʼ�غ�
    unsigned long int FileSize;		// �ļ���С
    unsigned long int FstClus;		// ��ʼ�غ�
    unsigned long int Clus;			// ��ǰ�غ�
    unsigned long int Offset;		// �ļ�ָ��ƫ����
} MY_FILE;

#ifndef FAT_File
//	�ļ���ʼ��
extern void FileInit(void);
//	ɾ���ļ�
extern unsigned char RemoveFile(char *DirFileName);
//	��ָ����ʽ���ļ�
extern HANDLE FileOpen(char *DirFileName, char *Type);
//�ر�ָ���ļ�
extern unsigned char FileClose(HANDLE Handle);
//	���ļ���һ���ֽ�
extern unsigned char FileGetCh(unsigned char *Ch, HANDLE Handle);
//	��ȡ�ļ�
extern unsigned long int FileRead(void *Buf, unsigned long int Size, HANDLE Handle);
//	дһ���ֽڵ��ļ�
extern unsigned char FilePutCh(unsigned char Ch, HANDLE Handle);
//	д�ļ�
extern unsigned long int FileWrite(void *Buf, unsigned long int Size, HANDLE Handle);
//�ر����д򿪵��ļ�
extern void FileCloseAll(void);
//	�ж��ļ��Ƿ񵽶�\д���ļ�β
extern unsigned char FileEof(HANDLE Handle);
//�ƶ��ļ���\дλ��
extern unsigned char FileSeek(HANDLE Handle, long int offset, unsigned char Whence);
#endif

#ifndef FAT_Dir
//	��Ϊ��д
extern char *strupr(char *Str);
//��ȡָ��Ŀ¼���߼��̺�
extern unsigned char GetDrive(char *Path);
//	�ı䵱ǰ�߼���
extern unsigned char ChangeDrive(char *Drive);
//	��ȡָ��Ŀ¼��ʼ�غ�
extern unsigned long int GetDirClusIndex(char *Path);
//	����Ŀ¼
extern unsigned char MakeDir(char *Path);
//	ɾ��Ŀ¼
extern unsigned char RemoveDir(char *Path);
//	�ı䵱ǰĿ¼
extern unsigned char ChangeDir(char *Path);
#endif

#ifndef FAT_FDT
////	�û��ļ�\Ŀ¼��ת��Ϊϵͳ��
//unsigned char StrToFDTName(char *FDTName, char *Str)
//	��ָ����������������
extern unsigned char ClearClus(unsigned char Drive, unsigned long int Index);
//	�ı�ָ��Ŀ¼ָ���ļ���Ŀ¼��������
extern unsigned char ChangeFDT(unsigned char Drive, unsigned long int ClusIndex, FDT *FDTData);
//	��ȡFDT��Ϣ
extern unsigned char  ReadFDTInfo(FDT *Rt, unsigned char Drive, unsigned long int SecIndex, unsigned int ByteIndex);
//	дFDT��Ϣ
extern unsigned char  WriteFDTInfo(FDT *FDTData, unsigned char Drive, unsigned long int SecIndex, unsigned int ByteIndex);
//	��ȡָ��Ŀ¼ָ���ļ���Ŀ¼����Ϣ
extern unsigned char GetFDTInfo(FDT *Rt,unsigned char Drive, unsigned long int ClusIndex, unsigned long int Index);
//	����ָ��Ŀ¼ָ���ļ���Ŀ¼����Ϣ
extern unsigned char SetFDTInfo(unsigned char Drive, unsigned long int ClusIndex, unsigned long int Index, FDT *FDTData);
//	��ָ��Ŀ¼����ָ���ļ���Ŀ¼��
extern unsigned char FindFDTInfo(FDT *Rt,unsigned char Drive, unsigned long int ClusIndex, char FileName[]);
//	��ָ��Ŀ¼������ָ���ļ���Ŀ¼��
extern unsigned char AddFDT(unsigned char Drive, unsigned long int ClusIndex, FDT *FDTData);
//	��ָ��Ŀ¼ɾ��ָ���ļ���Ŀ¼
extern unsigned char DelFDT(unsigned char Drive, unsigned long int ClusIndex, char FileName[]);
//	Ŀ¼�Ƿ�Ϊ��
extern unsigned char DirIsEmpty(unsigned char Drive, unsigned long int ClusIndex);
//	��ָ��Ŀ¼�鿴ָ���ļ���Ŀ¼���Ƿ����
extern unsigned char FDTIsLie(unsigned char Drive, unsigned long int ClusIndex, char FileName[]);
#endif

#ifndef FAT_Disk
//	��ʼ���߼��̹���ģ��
extern void DiskInit(void);
//	��ȡָ���߼�����Ϣ
extern Disk_Info *GetDiskInfo(unsigned char Drive);
//	��ȡ�����߼���
extern Disk_Info  *GetEmptyDiskInfoAddr(void);
//	����һ���ײ���������
extern void AddFileDriver(unsigned int  (* DiakCommand)(unsigned char Cammand, void *Parameter));
//	ɾ��һ���ײ���������
extern void RemoveFileDriver(unsigned char Drive);
#endif

#ifndef FAT_Cache
//	��ʼ������cache
extern void CacheInit(void);
//	����ָ��cache�������л����ڴ棩
extern void CloseSec(unsigned char Drive, unsigned long int Index);
//	��ָ������д���߼���
extern void CacheWriteBack2(unsigned int Index);
//	��ָ��cacheд���߼���
extern void CacheWriteBack(unsigned char Drive, unsigned long int Index);
//	�������Ѹı������д���߼���
extern void AllCacheWriteBack(void);
//	��ȡһ��cache
extern unsigned int GetCache(void);
//	Ϊ�߼����ϵ�һ��������һ��cache������
extern unsigned char *OpenSec(unsigned char Drive, unsigned long int Index);
//	���߼��̶�����
extern unsigned char ReadSec(unsigned char Drive, unsigned long int Index);
//	˵��ָ���߼��̵�ָ��һ����������д
extern void WriteSec(unsigned char Drive, unsigned long int Index);
#endif

#ifndef FAT_Clus
//	����FAT��ָ���ص���һ���غ�
extern unsigned long int FATGetNextClus(unsigned char Drive, unsigned long int Index);
//	ɾ��ָ������
void FATSetNextClus(unsigned char Drive, unsigned long int Index, unsigned long int Next);
//	Ϊָ����������һ����
extern unsigned long int FATAddClus(unsigned char Drive, unsigned long int Index);
//	ɾ��ָ������
extern void FATDelClusChain(unsigned char Drive, unsigned long int Index);
#endif

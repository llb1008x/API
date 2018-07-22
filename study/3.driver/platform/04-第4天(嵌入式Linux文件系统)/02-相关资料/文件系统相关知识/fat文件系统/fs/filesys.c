#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>
#include "filesys.h"

#define DEBUG
//���ʲô�Ǵ��С��
#define RevByte(low,high) ((high) << 8 | (low))		//RevByte()���������ת���ֽ�
#define RevWord(lowest, lower, higher, highest) ((highest) << 24 | (higher) << 16 | (lower) << 8 | lowest)

struct BootDescriptor_t bdptor;	//��������
struct Entry *curdir = NULL;	//��¼��ǰĿ¼��Ϣ,����Ǹ�Ŀ¼,��curdirΪNULL
int fd;		//��ʾ��ǰ�򿪵����̵�������
unsigned char fatbuf[0x1200];

//����:��ȡ������������Ϣ,������bdptor�ṹ����,����ʱ��ӡ�����Ϣ.
void ScanBootSector()
{
	unsigned char buf[SECTOR_SIZE];
	int ret, i;

	if((ret = read(fd, buf, SECTOR_SIZE)) < 0)
		perror("read boot sector failed");

	for(i=0; i<=10; i++)	//����ط�������,iӦ��С�ڵ���7,��Ϊ�汾��Ϣ����Ϊ8�ֽ�,����Oem_name[]���鶨���СΪ9.
		bdptor.Oem_name[i] = buf[i+0x03];
	bdptor.Oem_name[i] = '\0';

	//RevByte()���������ת���ֽ�
	bdptor.BytesPerSector       = RevByte(buf[0x0b],buf[0x0c]);
	bdptor.SectorsPerCluster    = buf[0x0d];
	bdptor.ReservedSectors      = RevByte(buf[0x0e], buf[0x0f]);
	bdptor.FATs		    = buf[0x10];
	bdptor.RootDirEntries	    = RevByte(buf[0x11], buf[0x12]);
	bdptor.LogicSectors         = RevByte(buf[0x13], buf[0x14]);
	bdptor.MediaType            = buf[0x15];
	bdptor.SectorsPerFAT        = RevByte(buf[0x16], buf[0x17]);
	bdptor.SectorsPerTrack      = RevByte(buf[0x18], buf[0x19]);
	bdptor.Heads                = RevByte(buf[0x1a], buf[0x1b]);
	bdptor.HiddenSectors        = RevByte(buf[0x1c], buf[0x1d]);	//����Ƿ�Ӧ����RevWord()?
/*     	#ifdef DEBUG
      	printf("Oem_name \t\t%s\n"
      		"BytesPerSector \t\t%d\n"
      		"SectorsPerCluster \t\t%d\n"
      		"ReservedSectors \t\t%d\n"
      		"FATs \t\t\t%d\n"
      		"RootDirEntries \t\t%d\n"
      		"LogicSectors \t\t%d\n"
      		"MediaType \t\t%x\n"
      		"SectorsPerFAT \t\t%d\n"
      		"SectorsPerTrack \t%d\n"
      		"Heads \t\t\t%d\n"
      		"HiddenSectors \t\t%d\n",
      		bdptor.Oem_name,
      		bdptor.BytesPerSector,
      		bdptor.SectorsPerCluster,
      		bdptor.ReservedSectors,
      		bdptor.FATs,
      		bdptor.RootDirEntries,
      		bdptor.LogicSectors,
      		bdptor.MediaType,
      		bdptor.SectorsPerFAT,
      		bdptor.SectorsPerTrack,
      		bdptor.Heads,
      		bdptor.HiddenSectors);
      	#endif
*/
}

//�������
void findDate(unsigned short *year,
	unsigned short *month,
	unsigned short *day,
	unsigned char info[2])
{
	int date;
	date = RevByte(info[0], info[1]);

	*year = ((date & MASK_YEAR) >> 9) + 1980;	//��Ҫ����1980
	*month = (date & MASK_MONTH) >> 5;
	*day = (date & MASK_DAY);
}

//���ʱ��
void findTime(unsigned short *hour,
	unsigned short *min,
	unsigned short *sec,
	unsigned char info[2])
{
	int time;
	time = RevByte(info[0], info[1]);
	*hour = (time & MASK_HOUR) >> 11;
	*min = (time & MASK_MIN) >> 5;
	*sec = (time & MASK_SEC) * 2;	//���������2
}

void FileNameFormat_old(unsigned char *name)
{
	unsigned char *q, *k, filename[12];

	q = name;
	k = name;

	while(*q!= ' ')		//ΪʲôҪ�жϿո�?
		q++;
	*q='\0';
	strcpy(filename, k);
	*q= ' ';

	strcat(filename, ".");

	while(*q == ' ')
		q++;
	k=q;
	strcat(filename,k);

	strcpy(name,filename);
}

void FileNameFormat(unsigned char *name)
{
	unsigned char *p;

	p=name+strlen(name) - 1;
	while(*p == ' ')
		p--;
	p++;
	*p = '\0';
}

//������Ŀ¼����
void ScanRootEntry()
{
	struct Entry entry;
	int ret, offset;
	
	if((ret =lseek(fd,ROOTDIR_OFFSET,SEEK_SET)) < 0)	//�ƶ��ļ���дλ�õ�ROOTDIR_OFFSET��,ROOTDIR_OFFSET�Ǹ�Ŀ¼������ʼ��ַ
		perror("lseek ROOTDIR_OFFSET failed");

	offset = ROOTDIR_OFFSET;

	while(offset < DATA_OFFSET)
	{
		ret = GetEntry(&entry);		//GetEntry( )�������Ƕ���Ŀ¼�����Ϣ,���ڲ���entry��,����ֵ��ƫ����
		offset += abs(ret);
		if(ret > 0)
		{
			printf("%12s\t"
				"%d:%d:%d\t "
				"%d:%d:%d\t "
				"%d\t "
				"%d\t\t "
				"%s\n",
				entry.short_name,
				entry.year, entry.month, entry.day,
				entry.hour, entry.min, entry.sec,
				entry.size,
				entry.FirstCluster,
				(entry.subdir) ? "dir" : "file");
		}
	}
}

//����:�Ӹ�Ŀ¼���ļ����ж���Ŀ¼�����ݷ��ڲ���pentry��.
//����ֵ: �ɹ�,�򷵻�ƫ��ֵ;ʧ��,�򷵻ظ�ֵ.
int GetEntry(struct Entry *pentry)
{
	int ret, i;
	int count = 0;
	unsigned char buf[DIR_ENTRY_SIZE], info[2];
	
	if((ret = read(fd, buf, DIR_ENTRY_SIZE)) < 0)
		perror("read entry failed");
	count += ret;
	
	if(buf[0] == 0xe5 || buf[0] ==0x00)	//δʹ�û���ɾ���ı���
		return -1*count;
	else{
		//���ļ�������,����
		while (buf[11] == 0x0f)		//���ļ���Ŀ¼��������ֽ���0x0f��ʶ,���ļ����������ֽ�Ϊ0x20.������Ŀ¼��ĵ�12λ.
		{
			if((ret = read(fd, buf, DIR_ENTRY_SIZE)) < 0)
				perror("read root dir failed");
			count += ret;
		}
		
		for(i=0;i<=10;i++)	//ȡ�ļ�������չ��
			pentry->short_name[i] = buf[i];
		pentry->short_name[i] = '\0';

		FileNameFormat(pentry->short_name);	//�������������ʲô?

		//ȡʱ��
		info[0] = buf[22];
		info[1] = buf[23];
		findTime(&(pentry->hour), &(pentry->min), &(pentry->sec), info);
		
		//ȡ����
		info[0] = buf[24];
		info[1] = buf[25];
		findDate(&(pentry->year), &(pentry->month), &(pentry->day), info);
		
		//�ļ�����ʼ�غ�
		pentry->FirstCluster = RevByte(buf[26], buf[27]);
		
		//�ļ���С
		pentry->size = RevWord(buf[28], buf[29], buf[30], buf[31]);
	
		//�ļ�����
		pentry->readonly = (buf[11] & ATTR_READONLY) ? 1:0;
		pentry->hidden   = (buf[11] & ATTR_HIDDEN)   ? 1:0;
		pentry->system   = (buf[11] & ATTR_SYSTEM)   ? 1:0;
		pentry->vlabel   = (buf[11] & ATTR_VLABEL)   ? 1:0;
		pentry->subdir   = (buf[11] & ATTR_SUBDIR)   ? 1:0;
		pentry->archive  = (buf[11] & ATTR_ARCHIVE)  ? 1:0;
		return count;
	}
}		

//����:��ʾ��ǰĿ¼����,����1��ʾ�ɹ�,����-1��ʾʧ��				
int fd_ls()
{
	int ret, offset, cluster_addr;
	struct Entry entry;

	//��ӡ��Ϣͷ��
	printf("\tname\tdate\t\t time\t\tsector\tsize\t\tattr\n");

	if(curdir == NULL)	//�ڳ���ʼ��,ȫ�ֱ���curdir����ʼ��ΪNULL
	{
		ScanRootEntry(fd);	//��ӡ��Ŀ¼,���ﴫ�Ĳ���fdû����,��ΪScanRootEntry()����ԭ����û�в���.
	}
	else
	{
		//��Ŀ¼��ʼ��ַ,�ڴ�ֻ��һ������,����֮����ֻ��һ������������,����Ϊ��һ��������һ������,
		//��Ҫ��������һ������������,��Ҫ����FAT��,Ϊ�򻯳���,ֻ��һ������������.
		cluster_addr = DATA_OFFSET + (curdir->FirstCluster - 2) * SECTOR_SIZE;

		if((ret = lseek(fd, cluster_addr, SEEK_SET)) < 0)
			perror("lseek cluster_addr failed");

		offset = cluster_addr;

		while(offset < cluster_addr +SECTOR_SIZE)
		{
			ret = GetEntry(&entry);	//����ע��,�����Ŀ¼�µ��ļ����Ǵ�д��,ret����32,����ļ�����Сд��,retֵΪ64
			offset += abs(ret);
			if(ret > 0)
			{
				printf("%12s\t"
					"%d:%d:%d\t "
					"%d:%d:%d\t "
					"%d\t "
					"%d\t\t "
					"%s\n",
					entry.short_name,
					entry.year, entry.month, entry.day,
					entry.hour, entry.min, entry.sec,
					entry.size,
					entry.FirstCluster,
					(entry.subdir) ? "dir" : "file");
			}
//			printf("%d\n",offset);		
		}
	}
	return 0;
}

//����:������ǰĿ¼,�����ļ���Ŀ¼����
//����:entryname, ����: char *
//     pentry,����: struct Entry *
//     mode,����: int, mode=1,ΪĿ¼����; mode=0,Ϊ�ļ�
//����ֵ:ƫ��ֵ����0,��ɹ�; -1,��ʧ��
int ScanEntry(char *entryname, struct Entry *pentry, int mode)
{
	int ret, offset, i;
	int cluster_addr;
	char uppername[80];

	for(i=0;i<strlen(entryname);i++)
		uppername[i] = toupper(entryname[i]);
	uppername[i]= '\0';

	if(curdir == NULL)
	{
		//ɨ���Ŀ¼
		if((ret = lseek(fd, ROOTDIR_OFFSET, SEEK_SET)) < 0)
			perror("lseek ROOTDIR_OFFSET failed");
		offset = ROOTDIR_OFFSET;

		while(offset < DATA_OFFSET)		
		{
			ret = GetEntry(pentry);
			offset += abs(ret);
			if(pentry->subdir == mode && !strcmp(pentry->short_name, uppername))
				return offset;
		}
		//û���ҵ���ڱ���
		return -1;
	}
	else
	{
		//ɨ��Ǹ�Ŀ¼
		cluster_addr = DATA_OFFSET + (curdir->FirstCluster-2) *SECTOR_SIZE;

		if((ret = lseek(fd, cluster_addr, SEEK_SET)) < 0)
			perror("lseek cluster_addr failed");

		offset = cluster_addr;

		while(offset < cluster_addr + SECTOR_SIZE)
		{
			ret = GetEntry(pentry);
			offset += abs(ret);
			if(pentry->subdir == mode && !strcmp(pentry->short_name, uppername))
				return offset;
		}
		//û���ҵ���ڱ���
		return -1;
	}
}

//����: �ı�Ŀ¼����Ŀ¼����Ŀ¼
//����: "cd .."��"cd subdir"
int fd_cd(char *dir)
{
	struct Entry *pentry;
	int ret;

	if(!strcmp(dir, "."))	//��Ҫ�л�����ǰĿ¼,ֱ�ӷ���
		return 1;

	if(!strcmp(dir, "..") && curdir == NULL)	//��Ҫ�л����ϼ�Ŀ¼��ǰĿ¼�Ǹ�Ŀ¼,ֱ�ӷ���
		return 1;
	
	pentry = (struct Entry *)malloc(sizeof(struct Entry));

	ret = ScanEntry(dir, pentry, 1);	//����Ŀ¼,����ƫ��ֵ

	if(ret < 0)
	{
		printf("no such dir\n");
		free(pentry);
		return -1;
	}
	
	free(curdir);
	curdir = pentry;
	
/*	#ifdef DEBUG	//���Գ���ʱʹ��
	printf("%12s\t"
		"%d:%d:%d\t "
		"%d:%d:%d\t "
		"%d\t "
		"%d\t\t "
		"%s\n",
		entry.short_name,
		entry.year, entry.month, entry.day,
		entry.hour, entry.min, entry.sec,
		entry.size,
		entry.FirstCluster,
		(entry.subdir) ? "dir" : "file");
	#endif
*/	
	return 1;
}

//����:��FAT���л����һ���ص�λ��
//����:prev, ����: unsigned short
//����ֵ: ��һ����
unsigned short GetFatCluster(unsigned short prev)
{
	unsigned short next, fatentry;
	int index;

	index = prev * 1.5;
	fatentry = RevByte(fatbuf[index], fatbuf[index+1]);

	if(prev % 2 == 1)
		next = fatentry >> 4;
	else
		next = fatentry & 0x0fff;

	return next;
}

//���FAT���еĴ���Ϣ
//����:cluster, ����: unsigned short, cluster number
//����ֵ: void
void ClearFatCluster(unsigned short cluster)
{
	int index;

	index = cluster * 1.5;
	
	if(cluster % 2 == 1)
	{
		fatbuf[index] &= 0x0f;
		fatbuf[index+1] &= 0x00;
	}
	else
	{
		fatbuf[index] &= 0x00;
		fatbuf[index+1] &= 0xf0;
	}
}

//����:ɾ����ǰĿ¼�µ�һ���ļ�,����1��ʾ�ɹ�,����-1��ʾʧ��.
int fd_rm(char *filename)
{
	struct Entry *pentry;
	int ret;
	unsigned char c;
	unsigned short seed, next;

	pentry = (struct Entry *)malloc(sizeof(struct Entry));

	//�ڵ�ǰĿ¼�����в����ļ���
	ret = ScanEntry(filename, pentry, 0);
	if(ret < 0)
	{
		printf("no such file\n");
		free(pentry);
		return -1;
	}
	
	//���FAT��
	seed = pentry->FirstCluster;
	while((next = GetFatCluster(seed)) != 0x0fff)  //0xFFF��ʾΪ���һ����
	{
		seed = next;
		ClearFatCluster(next);
	}
	ClearFatCluster(pentry->FirstCluster);

	//���Ŀ¼����,0xe5��ʾ��Ŀ¼����ǰʹ�ù�,�����Ѿ��ͷŵ���
	c = 0xe5;
	if(lseek(fd, ret-0x40, SEEK_SET) < 0)
		perror("lseek fd_rm failed");
	if(write(fd, &c , 1) < 0)
		perror("write failed");

	
	if(lseek(fd, ret-0x20, SEEK_SET) < 0)
		perror("lseek fd_rm failed");
	if(write(fd, &c , 1) < 0)
		perror("write failed");

	free(pentry);
	
	//д�ش���
	if(WriteFat() < 0)	//�������������,��ΪWriteFat()�����ǰ��ڴ��е�fatbufд������,�������������Ĺ�����������?
		exit(1);
	
	return 1;
}

int WriteFat()
{
	if(lseek(fd, FAT_ONE_OFFSET, SEEK_SET) < 0) 
	{
		perror("lseek failed");
		return -1;
	}
	
	if(write(fd, fatbuf, 0x1200) < 0)
	{
		perror("read failed");
		return -1;
	}

	if(lseek(fd, FAT_TWO_OFFSET, SEEK_SET) < 0)
	{
		perror("LSEEK failed");
		return -1;
	}
	
	if(write(fd, fatbuf, 0x1200) < 0)
	{
		perror("read failed");
		return -1;
	}
	
	return 1;
}

int ReadFat()
{
	if(lseek(fd, FAT_ONE_OFFSET, SEEK_SET) < 0)	//��λ��һ��FAT���λ��
	{
		perror("lseek failed");
		return -1;
	}

	//��FAT������ݶ���fatbuf��
	if(read(fd, fatbuf, 0x1200) < 0)	//FAT��Ĵ�С��0x1200
	{
		perror("read failed");
		return -1;
	}
	
	return 1;
}

int main()
{
//	char p[]="dir";
//	char p1[]="tmp";
	if((fd = open(DEVNAME, O_RDWR)) < 0)
		perror("open failed");

	if(ReadFat() < 0)	//����һ��FAT������ݶ���fatbuf��
		exit(1);

//	ScanBootSector();
//	ScanRootEntry();

//	fd_cd(p);	
	fd_ls();	

//	fd_rm(p1);
	
	close(fd);
	free(curdir);
	return 0;
}

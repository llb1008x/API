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
//理解什么是大端小端
#define RevByte(low,high) ((high) << 8 | (low))		//RevByte()宏的作用是转换字节
#define RevWord(lowest, lower, higher, highest) ((highest) << 24 | (higher) << 16 | (lower) << 8 | lowest)

struct BootDescriptor_t bdptor;	//启动扇区
struct Entry *curdir = NULL;	//记录当前目录信息,如果是根目录,则curdir为NULL
int fd;		//表示当前打开的软盘的描述符
unsigned char fatbuf[0x1200];

//功能:获取启动扇区的信息,保存在bdptor结构体中,调试时打印相关信息.
void ScanBootSector()
{
	unsigned char buf[SECTOR_SIZE];
	int ret, i;

	if((ret = read(fd, buf, SECTOR_SIZE)) < 0)
		perror("read boot sector failed");

	for(i=0; i<=10; i++)	//这个地方有问题,i应该小于等于7,因为版本信息长度为8字节,而且Oem_name[]数组定义大小为9.
		bdptor.Oem_name[i] = buf[i+0x03];
	bdptor.Oem_name[i] = '\0';

	//RevByte()宏的作用是转换字节
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
	bdptor.HiddenSectors        = RevByte(buf[0x1c], buf[0x1d]);	//这个是否应该用RevWord()?
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

//获得日期
void findDate(unsigned short *year,
	unsigned short *month,
	unsigned short *day,
	unsigned char info[2])
{
	int date;
	date = RevByte(info[0], info[1]);

	*year = ((date & MASK_YEAR) >> 9) + 1980;	//年要加上1980
	*month = (date & MASK_MONTH) >> 5;
	*day = (date & MASK_DAY);
}

//获得时间
void findTime(unsigned short *hour,
	unsigned short *min,
	unsigned short *sec,
	unsigned char info[2])
{
	int time;
	time = RevByte(info[0], info[1]);
	*hour = (time & MASK_HOUR) >> 11;
	*min = (time & MASK_MIN) >> 5;
	*sec = (time & MASK_SEC) * 2;	//秒数必须乘2
}

void FileNameFormat_old(unsigned char *name)
{
	unsigned char *q, *k, filename[12];

	q = name;
	k = name;

	while(*q!= ' ')		//为什么要判断空格?
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

//搜索根目录表项
void ScanRootEntry()
{
	struct Entry entry;
	int ret, offset;
	
	if((ret =lseek(fd,ROOTDIR_OFFSET,SEEK_SET)) < 0)	//移动文件读写位置到ROOTDIR_OFFSET处,ROOTDIR_OFFSET是根目录区的起始地址
		perror("lseek ROOTDIR_OFFSET failed");

	offset = ROOTDIR_OFFSET;

	while(offset < DATA_OFFSET)
	{
		ret = GetEntry(&entry);		//GetEntry( )的作用是读出目录项的信息,放在参数entry中,返回值是偏移量
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

//功能:从根目录或文件簇中读出目录项内容放在参数pentry中.
//返回值: 成功,则返回偏移值;失败,则返回负值.
int GetEntry(struct Entry *pentry)
{
	int ret, i;
	int count = 0;
	unsigned char buf[DIR_ENTRY_SIZE], info[2];
	
	if((ret = read(fd, buf, DIR_ENTRY_SIZE)) < 0)
		perror("read entry failed");
	count += ret;
	
	if(buf[0] == 0xe5 || buf[0] ==0x00)	//未使用或已删掉的表项
		return -1*count;
	else{
		//长文件名表项,忽略
		while (buf[11] == 0x0f)		//长文件名目录项的属性字节以0x0f标识,短文件名的属性字节为0x20.属性在目录项的第12位.
		{
			if((ret = read(fd, buf, DIR_ENTRY_SIZE)) < 0)
				perror("read root dir failed");
			count += ret;
		}
		
		for(i=0;i<=10;i++)	//取文件名和扩展名
			pentry->short_name[i] = buf[i];
		pentry->short_name[i] = '\0';

		FileNameFormat(pentry->short_name);	//这个函数作用是什么?

		//取时间
		info[0] = buf[22];
		info[1] = buf[23];
		findTime(&(pentry->hour), &(pentry->min), &(pentry->sec), info);
		
		//取日期
		info[0] = buf[24];
		info[1] = buf[25];
		findDate(&(pentry->year), &(pentry->month), &(pentry->day), info);
		
		//文件的起始簇号
		pentry->FirstCluster = RevByte(buf[26], buf[27]);
		
		//文件大小
		pentry->size = RevWord(buf[28], buf[29], buf[30], buf[31]);
	
		//文件属性
		pentry->readonly = (buf[11] & ATTR_READONLY) ? 1:0;
		pentry->hidden   = (buf[11] & ATTR_HIDDEN)   ? 1:0;
		pentry->system   = (buf[11] & ATTR_SYSTEM)   ? 1:0;
		pentry->vlabel   = (buf[11] & ATTR_VLABEL)   ? 1:0;
		pentry->subdir   = (buf[11] & ATTR_SUBDIR)   ? 1:0;
		pentry->archive  = (buf[11] & ATTR_ARCHIVE)  ? 1:0;
		return count;
	}
}		

//功能:显示当前目录内容,返回1表示成功,返回-1表示失败				
int fd_ls()
{
	int ret, offset, cluster_addr;
	struct Entry entry;

	//打印信息头部
	printf("\tname\tdate\t\t time\t\tsector\tsize\t\tattr\n");

	if(curdir == NULL)	//在程序开始处,全局变量curdir被初始化为NULL
	{
		ScanRootEntry(fd);	//打印根目录,这里传的参数fd没有用,因为ScanRootEntry()函数原型中没有参数.
	}
	else
	{
		//子目录起始地址,在此只读一个扇区,这里之所以只读一个扇区的内容,是因为下一个扇区不一定连续,
		//如要继续读下一个扇区的内容,还要查找FAT表,为简化程序,只读一个扇区的内容.
		cluster_addr = DATA_OFFSET + (curdir->FirstCluster - 2) * SECTOR_SIZE;

		if((ret = lseek(fd, cluster_addr, SEEK_SET)) < 0)
			perror("lseek cluster_addr failed");

		offset = cluster_addr;

		while(offset < cluster_addr +SECTOR_SIZE)
		{
			ret = GetEntry(&entry);	//这里注意,如果子目录下的文件名是大写的,ret返回32,如果文件名是小写的,ret值为64
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

//功能:搜索当前目录,查找文件或目录表项
//参数:entryname, 类型: char *
//     pentry,类型: struct Entry *
//     mode,类型: int, mode=1,为目录表项; mode=0,为文件
//返回值:偏移值大于0,则成功; -1,则失败
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
		//扫描根目录
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
		//没有找到入口表项
		return -1;
	}
	else
	{
		//扫描非根目录
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
		//没有找到入口表项
		return -1;
	}
}

//功能: 改变目录到父目录或子目录
//例如: "cd .."或"cd subdir"
int fd_cd(char *dir)
{
	struct Entry *pentry;
	int ret;

	if(!strcmp(dir, "."))	//若要切换到当前目录,直接返回
		return 1;

	if(!strcmp(dir, "..") && curdir == NULL)	//若要切换到上级目录或当前目录是根目录,直接返回
		return 1;
	
	pentry = (struct Entry *)malloc(sizeof(struct Entry));

	ret = ScanEntry(dir, pentry, 1);	//查找目录,返回偏移值

	if(ret < 0)
	{
		printf("no such dir\n");
		free(pentry);
		return -1;
	}
	
	free(curdir);
	curdir = pentry;
	
/*	#ifdef DEBUG	//调试程序时使用
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

//功能:在FAT表中获得下一个簇的位置
//参数:prev, 类型: unsigned short
//返回值: 下一个簇
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

//清除FAT表中的簇信息
//参数:cluster, 类型: unsigned short, cluster number
//返回值: void
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

//功能:删除当前目录下的一个文件,返回1表示成功,返回-1表示失败.
int fd_rm(char *filename)
{
	struct Entry *pentry;
	int ret;
	unsigned char c;
	unsigned short seed, next;

	pentry = (struct Entry *)malloc(sizeof(struct Entry));

	//在当前目录表项中查找文件名
	ret = ScanEntry(filename, pentry, 0);
	if(ret < 0)
	{
		printf("no such file\n");
		free(pentry);
		return -1;
	}
	
	//清除FAT表
	seed = pentry->FirstCluster;
	while((next = GetFatCluster(seed)) != 0x0fff)  //0xFFF表示为最后一个簇
	{
		seed = next;
		ClearFatCluster(next);
	}
	ClearFatCluster(pentry->FirstCluster);

	//清除目录表项,0xe5表示该目录项以前使用过,但是已经释放掉了
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
	
	//写回磁盘
	if(WriteFat() < 0)	//这里可能有问题,因为WriteFat()函数是把内存中的fatbuf写到磁盘,这样把上面做的工作覆盖了吗?
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
	if(lseek(fd, FAT_ONE_OFFSET, SEEK_SET) < 0)	//定位第一个FAT表的位置
	{
		perror("lseek failed");
		return -1;
	}

	//将FAT表的内容读到fatbuf中
	if(read(fd, fatbuf, 0x1200) < 0)	//FAT表的大小是0x1200
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

	if(ReadFat() < 0)	//将第一个FAT表的内容读到fatbuf中
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

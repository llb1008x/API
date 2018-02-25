
实例:

    #include<stdio.h>
    #include<stdlib.h>
    #include<dirent.h>

	char *song_list[100]="";
    int main(void)
    {
        DIR *dir;
        struct dirent *dirp;
		int i = 0;

        if((dir=opendir("tmpdir"))==NULL)
		{
            printf("Open dir tmpdir fail\n");
            exit(1);
		}

        while((dirp=readdir(dir))!=NULL)
        {
			printf("Name:%s\n",dirp->d_name);
			song_list[i]=(char*)malloc(sizeof(dirp->d_name)+1);
			strcpy(song_list[i],dirp->d_name);
		}
        closedir(dir);

        return(0);
    }
-----------------------------------------------------------------------
opendir

名称：打开一个目录

总揽:
    #include<dirent.h>
    #include<sys/types.h>

    DIR *opendir(const char *name)

描述：

    opendir()函数用来打开一个指定的目录name,返回一个目录流DIR指针。

目录流的位置为第一个目录项上。

返回值:

    opendir()返回执行成功返回一个目录流指针，失败返回NULL并设置errno.

错误：

    EACCESS: 权限不允许

    EMFILE:  进程使用太多文件描述符了

    ENFILE:  当前系统中打开太多文件了

    ENOENT:  要打开的目录不存在或name是一个空字符串

    ENOMEM:  没有足够的内存完成操作

    ENOTDIR: 指定的name不是一个目录


DIR结构体成员如下：
　　struct __dirstream
　　{
	　　void *__fd; /* `struct hurd_fd' pointer for descriptor. */
	　　char *__data; /* Directory block. */
	　　int __entry_data; /* Entry number `__data' corresponds to. */
	　　char *__ptr; /* Current pointer into the block. */
	　　int __entry_ptr; /* Entry number `__ptr' corresponds to. */
	　　size_t __allocation; /* Space allocated for the block. */
	　　size_t __size; /* Total valid data in the block. */
	　　__libc_lock_define (, __lock) /* Mutex lock for this structure. */
　　};
　　typedef struct __dirstream DIR;

语法: struct dirent* readdir(DIR* dir_handle);

　　返回值: dirent结构

　　函数种类: 文件存取

　　内容说明 本函数用来读取目录。返回是dirent结构体指针，dirent结构体成员如下，

　　struct dirent
　　{
　　	long d_ino; /* inode number 索引节点号 */
　　	off_t d_off; /* offset to this dirent 在目录文件中的偏移 */
　　	unsigned short d_reclen; /* length of this d_name 文件名长 */
　　	unsigned char d_type; /* the type of d_name 文件类型 */
　　	char d_name [NAME_MAX+1]; /* file name (null-terminated) 文件名，最长255字符 */
　　}

　　readdir相当于文件操作函数read，DIR结构体相当于文件操作里面的FILE结构体。

closedir()

　　#include<sys/types.h>
　　#include<dirent.h>

函数原型

　　int closedir(DIR *dir);

函数说明
　　closedir()关闭参数dir所指的目录流。关闭成功则返回0，失败返回-1，错误原因存于errno 中。EBADF 参数dir为无效的目录流。
　　注意：目录文件作为一种文件，在打开必须关闭，否则会由于文件的进程打开文件过多而不能打开新的文件。因此opendir函数和closedir函数同样是配对出现的。




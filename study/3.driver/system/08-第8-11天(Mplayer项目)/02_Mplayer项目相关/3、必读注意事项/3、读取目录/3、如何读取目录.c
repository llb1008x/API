
ʵ��:

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

���ƣ���һ��Ŀ¼

����:
    #include<dirent.h>
    #include<sys/types.h>

    DIR *opendir(const char *name)

������

    opendir()����������һ��ָ����Ŀ¼name,����һ��Ŀ¼��DIRָ�롣

Ŀ¼����λ��Ϊ��һ��Ŀ¼���ϡ�

����ֵ:

    opendir()����ִ�гɹ�����һ��Ŀ¼��ָ�룬ʧ�ܷ���NULL������errno.

����

    EACCESS: Ȩ�޲�����

    EMFILE:  ����ʹ��̫���ļ���������

    ENFILE:  ��ǰϵͳ�д�̫���ļ���

    ENOENT:  Ҫ�򿪵�Ŀ¼�����ڻ�name��һ�����ַ���

    ENOMEM:  û���㹻���ڴ���ɲ���

    ENOTDIR: ָ����name����һ��Ŀ¼


DIR�ṹ���Ա���£�
����struct __dirstream
����{
	����void *__fd; /* `struct hurd_fd' pointer for descriptor. */
	����char *__data; /* Directory block. */
	����int __entry_data; /* Entry number `__data' corresponds to. */
	����char *__ptr; /* Current pointer into the block. */
	����int __entry_ptr; /* Entry number `__ptr' corresponds to. */
	����size_t __allocation; /* Space allocated for the block. */
	����size_t __size; /* Total valid data in the block. */
	����__libc_lock_define (, __lock) /* Mutex lock for this structure. */
����};
����typedef struct __dirstream DIR;

�﷨: struct dirent* readdir(DIR* dir_handle);

��������ֵ: dirent�ṹ

������������: �ļ���ȡ

��������˵�� ������������ȡĿ¼��������dirent�ṹ��ָ�룬dirent�ṹ���Ա���£�

����struct dirent
����{
����	long d_ino; /* inode number �����ڵ�� */
����	off_t d_off; /* offset to this dirent ��Ŀ¼�ļ��е�ƫ�� */
����	unsigned short d_reclen; /* length of this d_name �ļ����� */
����	unsigned char d_type; /* the type of d_name �ļ����� */
����	char d_name [NAME_MAX+1]; /* file name (null-terminated) �ļ������255�ַ� */
����}

����readdir�൱���ļ���������read��DIR�ṹ���൱���ļ����������FILE�ṹ�塣

closedir()

����#include<sys/types.h>
����#include<dirent.h>

����ԭ��

����int closedir(DIR *dir);

����˵��
����closedir()�رղ���dir��ָ��Ŀ¼�����رճɹ��򷵻�0��ʧ�ܷ���-1������ԭ�����errno �С�EBADF ����dirΪ��Ч��Ŀ¼����
����ע�⣺Ŀ¼�ļ���Ϊһ���ļ����ڴ򿪱���رգ�����������ļ��Ľ��̴��ļ���������ܴ��µ��ļ������opendir������closedir����ͬ������Գ��ֵġ�




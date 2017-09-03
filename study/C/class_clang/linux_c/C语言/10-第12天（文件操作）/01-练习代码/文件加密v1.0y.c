#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define FILE_NAME_LEN	30

void get_file_name(char *dest_file_name,char *src_file_name)
{
	printf("请输入源文件名称[30字符以内]:\n");
	gets(src_file_name);
	printf("请输入目标文件名称[30字符以内]:\n");
	gets(dest_file_name);
}
char *read_src_file(unsigned long int *file_len,char *src_file_name)
{
	char *src;//源文件
	FILE *fp;//打开文件
	
	fp = fopen(src_file_name,"rb");//以只读的方式打开源文件
	if(fp == NULL)                //判断打开
{                                     //方式是否成功
		perror("fopen");      //如果失败，返回NULL
		return NULL;          //注：这是一个整体，叫做打开文件，以只读方式
	}
	fseek(fp,0,SEEK_END);//将fp文件移动到开头
	*file_len = ftell(fp);//获取文件起始位置到分seek中选取的位置之间的字节数，在此时是测文件的大小
	src = malloc(*file_len);//将文件的大小动态分配一个内存（这个东西是指针）并赋值给源文件 即将源文件动态分配一个空间
	rewind(fp);//重新将fseek的位置指针 移动到文件开头，
	if(fread(src,*file_len,1,fp) == 0)//读取该文件的字符串送到源文件中，数据块的大小为fp的长度，数据块个数为一个，
{                                         //内容为零时，关闭fp，返回值就是读到的内容为空；这个(fread(src,*file_len,1,fp)
		fclose(fp);               //函数本身就是读取内容，所以!=0时就会读取内容
		return NULL;
	}                                 //从if到此，采薇以个完整的读取内容的程序方法
	fclose(fp);                        //关闭文件
	return src;                       //将src返回，重新变成源文件
}
char *file_text_encrypt(char *src_file_text,unsigned long int lengh,unsigned char password)给//src加密。源文件加密
{
	char *ps = src_file_text;//将源文件初始化定义，被ps表示
	char *dst = src_file_text;
	unsigned long int i;//定义变量i的类型为unsigned long int，是为了和 lengh的类型相同
	
	for(i=0;i<lengh;i++)
{
		*src_file_text++ = (*dst++)+password;//给加密的数组中的每个元素加上password
	}
	return ps;//将ps返回
}
char *file_text_decrypt(char *src_file_text,unsigned long int lengh,unsigned char password)//给源地址解密
{
	char *ps = src_file_text;//将源文件初始化定义，被ps表示
	char *dst = src_file_text;
	unsigned long int i;//定义变量i的类型为unsigned long int，是为了和 lengh的类型相同
	
	for(i=0;i<lengh;i++){
		*src_file_text++ = (*dst++)-password;
	}
	return ps;
}
void save_file(char *text,unsigned long int len,char *name)//将字符串保存到目的文件，并重命名
{
	FILE *fp;//打开一个文件的起始形式
	fp = fopen(name,"wb");//以只写的方式打开，判断打开是否成功，
	if(fp == NULL)        //如果失败，返回NULL
		perror("fopen");
	if(fwrite(text,len,1,fp) == 0)//写入该文件的字符串送到源文件中，数据块的大小为fp的长度，数据块个数为一个，
{
		fclose(fp);         //内容为零时，关闭fp，返回值就是写入的内容为空；这个(fwrite(text,len,1,fp)
		return;             //函数本身就是内容，所以!=0时就会写入内容
	}
	fclose(fp);
}
int main()
{
	char src_file_name[FILE_NAME_LEN];//
	char dest_file_name[FILE_NAME_LEN];//
	unsigned long int    file_len,i;//上面的函数用到过得变量
	char *src,select;//上面的函数用到过得变量或main下面即将用到的变量
	unsigned char password;//上面的函数用到过得变量
	
	get_file_name(dest_file_name,src_file_name);//上面用到的函数，这是在？
	src = read_src_file(&file_len,src_file_name);//上面用到的函数。将读到的函数赋值给源文件
	// for(i=0;i<file_len;i++)//
		// printf("%c",*src++);//
	printf("请输入密码:\n");
	scanf("%d",&password);//从键盘上输入密码
	scanf("%d",&select);//select选择的意思，即从键盘上输入...
	if(select == 1)//如果输入的数为1
		src = file_text_encrypt(src,file_len,password);//
	else if(select == 2)//
		src = file_text_decrypt(src,file_len,password);//
	save_file(src,file_len,dest_file_name);//
	return 0;//
}
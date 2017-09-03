#include <stdio.h>
#include <stdlib.h>
void get_file_name(char * dest_file_name,char * src_file_name);

char * read_src_file(unsigned long int *file_length,char *src_file_name);

char * file_text_encrypt(char * src_file_text,unsigned long int length,unsigned int password);

char * file_text_decrypt(char * src_file_text,unsigned long int length,unsigned int password);

void save_file(char* text,unsigned long int length,char * file_name);

void print_help();


int main()
{
	char src_file_name[30];
	char dest_file_name[30];
	char *src_file_text=NULL;
	unsigned long int file_length;
	unsigned int password;
	int key;
	while(1)
	{
		print_help();
		scanf("%d",&key);
		getchar();
		switch(key)
		{
	    	case  1 :
				{
					get_file_name(dest_file_name,src_file_name);// 获取原文件和目的文件名
					src_file_text=read_src_file(&file_length,src_file_name);// 从源文件中读取数据
					printf("please input your unsigned int passworld:");
					scanf("%d",&password);// 输入密码 必须是无符号整型的
					getchar();

					src_file_text=file_text_encrypt(src_file_text,file_length,password);// 给字符串加密返回加密后的字符串的首地址
					save_file(src_file_text,file_length,dest_file_name);// 将字符串保存到目的文件中
					if(src_file_text!=NULL)
						free(src_file_text);//回收资源
					break;
				}
			case 2:
				{
					get_file_name(dest_file_name,src_file_name);// 获取原文件和目的文件名
					src_file_text=read_src_file(&file_length,src_file_name);// 从源文件中读取数据
					printf("please input your passworld:");
					
					scanf("%d",&password);// 获取密码
					getchar();

					src_file_text=file_text_decrypt(src_file_text,file_length,password);// 解密字符串，返回解密后的字符串的首地址
					save_file(src_file_text,file_length,dest_file_name);// 将解密后的字符串保存到目的文件中
					if(src_file_text!=NULL)
						free(src_file_text);//回收资源
					break;
				}
			case 3:
				{
					
					return 0;// 退出 程序
					break;
				}
			default :
				{
					printf("请输入一个正确的选项 \n");
				}
		}
	}
	return 0;
}


/**************************************************************************/
//函数功能:获取 目的文件和源文件的名字
//参数： src_file_name:源文件名字。 
// dest_file_name:目的文件的名字
/**************************************************************************/
void get_file_name(char * dest_file_name,char * src_file_name)
{
	printf("请输入你的原文件名称:");
	gets(src_file_name);
	
	printf("请输入你的目的文件名称:");
	gets(dest_file_name);
}



/**************************************************************************/
//函数功能:读出文件内容
//参数：file_length:整型指针，此地址中保存文件字节数。 
// src_file_name:文件名字，从此文件中读取内容。 
// 返回值:读出字符串的首地址
/**************************************************************************/
char * read_src_file(unsigned long int *file_length,char *src_file_name)
{
	unsigned long int FILE_SIZE;
	char *src_file_text;
	FILE  *fp;

	fp=fopen(src_file_name,"rb");
	if(fp==NULL)
	{
		printf("open %s faild\n",src_file_name);
		return 0;
	}

	fseek(fp,0,SEEK_END);
	FILE_SIZE=ftell(fp);
	*file_length=FILE_SIZE;

	rewind(fp);
	src_file_text=malloc(FILE_SIZE);
	if(fread(src_file_text,FILE_SIZE,1,fp)==0)
	{
		printf("read from src_file faild\n");
		return NULL;
	}
	
	fclose(fp);// 关闭文件
		return src_file_text ;
}




/**************************************************************************/
//函数功能:加密字符串
//参数：src_file_text:要加密的字符串。 length:字符串的长度  
//   password: 加密密码
// 返回值: 加密后的字符串的首地址
/**************************************************************************/
char * file_text_encrypt(char * src_file_text,unsigned long int length,unsigned int password)
{
	char * file_text;
	char  *result_text;
	unsigned int i;

	result_text=file_text=src_file_text;
	
	for(i=0;i<length;i++)
	{
		*file_text++ = (*src_file_text++)+password;
	}
	return result_text;

}


/**************************************************************************/
//函数功能:解密字符串
//参数：src_file_text:要解密的字符串。 length:字符串的长度  
//   password: 解密密码
// 返回值: 解密后的字符串的首地址
/**************************************************************************/
char * file_text_decrypt(char * src_file_text,unsigned long int length,unsigned int password)
{
	char * file_text;
	char  *result_text;

	unsigned long int i;
	
	result_text=file_text=src_file_text;
	
	
	for(i=0;i<length;i++)
	{
		*file_text++ = (*src_file_text++)-password;
	}
	return result_text;
	
}


/**************************************************************************/
//
//	函数功能:将字符串保存到目的文件中
//参数：text:要保存的字符串首地址 。   file_name :目的文件的名字
//    length:字符串的长度
/**************************************************************************/
void save_file(char* text,unsigned long int length,char * file_name)
{
	FILE  *dest_fp;
	dest_fp=fopen(file_name,"wb+");
	if(dest_fp==NULL)
	{
		printf("open %s faild\n",file_name);
	}

	if(fwrite(text,length,1,dest_fp)<=0)
	{
		printf("write %s error\n",file_name);
	}
	else
	{
		printf("save sucess\n")
	}
	fclose(dest_fp);
}

/**************************************************************************/
//
//	函数功能:打印帮助信息
//
//
/**************************************************************************/
void print_help()
{
	printf("********1:加密文件***********\n");
	printf("********2:解密文件***********\n");
	printf("********3:退出程序***********\n");
	
}
/*************************************************************************
	> File Name: file_encrypt.c
	> Author: llb
	> Mail: llb1008x@126.com
	> Created Time: 2017年09月26日 星期二 23时35分09秒
	
对称文件加密系统
 ************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_SIZE       10

void print_help()
{
    printf("*****1:加密文件*****\n");
    printf("*****2.解密文件*****\n");
    printf("*****3.退出程序*****\n");

}


/*
    获取源文件和目的文件的名字
*/
void get_file_name(char *dest_file_name,char *src_file_name)
{
    printf("请输入要加密文件名称:");
    scanf("%s",dest_file_name);
    printf("请输入加密后文件的名称:");
    scanf("%s",src_file_name);
}


/*
    获取文件的大小
*/
int get_file_size(FILE *fp)
{
    int len;
    
    fseek(fp,0,SEEK_END);
    len=ftell(fp);
    rewind(fp);          //文件指针重新指向文件开始
    
    return len;
}


/*
    从文件中读出内容
*/
char *read_src_file(FILE *fp,int len)
{
    char *buf;
    
    buf=(char *)malloc(len);    //动态申请一块内存，用来存储文件内容 
    fread(buf,len,1,fp);
    printf("sub buf->%s\n",buf);

    return buf;
}


int main()
{
    print_help();

//1.获取相关文件的名称
    char dest_file_name[MAX_SIZE]="";
    char src_file_name[MAX_SIZE]="";
    FILE *fp_en,*fp_de;
    int len;
    char *buf;
    
    get_file_name(dest_file_name,src_file_name);
    printf("dest->%s,src->%s\n",dest_file_name,src_file_name);
    
//2.打开要加密的文件
    fp_en=fopen(src_file_name,"ab");
    if(!fp_en){
    
        printf("open faild!\n");
        return -1;
    }    
    
    
//3.获取文件的大小
    len=get_file_size(fp_en);
    printf("len->%d\n",len);
    
    
//4.读取文件内容
    buf=read_src_file(fp_en,len);
    printf("main buf->%s\n",buf);
    
    
    free(buf);
    //关闭文件指针
    fclose(fp_en);         


    return 0;
}

/*************************************************************************
	> File Name: file_copy.c
	> Author: llb
	> Mail: llb1008x@126.com
	> Created Time: 2017年09月26日 星期二 22时42分56秒
	
实现文件拷贝	
 ************************************************************************/

#include <stdio.h>
#include <string.h>

#define MAX_SIZE    20

int main()
{
//1.获取两个文件的名称
    FILE *fp,*fq;
    char file_name1[20]="";
    char file_name2[20]="";
    char str[MAX_SIZE]="";
    
    printf("请输入要复制的文件的名称:\n");
    scanf("%s",file_name1);
    printf("请输入要输出文件的名称:\n");
    scanf("%s",file_name2);
    
//2.打开两个文件
    fp=fopen(file_name1,"r+");
    if(!fp){
    
        printf("open faild!\n");
        return -1;
    }        
    fq=fopen(file_name2,"a+");
    if(!fq){
        
        printf("open faild!\n");
        return -1;
    }
    
//3.读一个文件的内容，同时写到另一个文件中    
    while( fgets(str,sizeof(str),fp)!=NULL){
    
        //printf("%s\n",str);
        fputs(str,fq);
        memset(str,0,MAX_SIZE);
    }

//4.关闭文件指针
    fclose(fp);
    fclose(fq);    

    return 0;
}

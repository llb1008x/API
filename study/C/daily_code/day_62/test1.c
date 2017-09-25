/*************************************************************************
	> File Name: test.c
	> Author: llb
	> Mail: llb1008x@126.com
	> Created Time: 2017年09月25日 星期一 21时54分56秒
	
	
 ************************************************************************/

#include <stdio.h>


#define FILE_NAME   "./work.txt"
#define NEW_FILE    "./new_file.txt"


int main()
{
    //1.打开文件
    FILE *fp=NULL,*fq=NULL;
    char ch;
    
    fp=fopen(FILE_NAME,"a+");
    if(!fp){
    
        printf("open faild!\n");
        return -1;
    }

    fq=fopen(NEW_FILE,"a+");
    if(!fq){
    
        printf("open faild!\n");
        return -1;
    }
    
    
    //读写文件
    while( (ch=fgetc(fp))!=EOF){
    
        fputc(ch,fq);
        printf("111111111111\n");
    }
    
    
    //关闭文件
    fclose(fp);
    fclose(fq);
    

    return 0;
}

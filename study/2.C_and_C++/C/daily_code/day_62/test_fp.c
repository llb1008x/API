/*************************************************************************
	> File Name: test.c
	> Author: llb
	> Mail: llb1008x@126.com
	> Created Time: 2017年09月25日 星期一 21时54分56秒
	
	
 ************************************************************************/

#include <stdio.h>


#define FILE_NAME   "./work.txt"
#define MAX_SIZE    15

int main()
{
    FILE *fp=NULL;
    char str[MAX_SIZE]="";
    
 //1.打开文件   
    fp=fopen(FILE_NAME,"a+");
    if(!fp){
    
        printf("open error,not exist!\n");
        return -1;
    }
    
//读写文件    EOF是文件结尾
    fgets(str,MAX_SIZE-1,fp);
    
    printf("str->%s\n",str);


//2.关闭文件    
    fclose(fp);
    

    return 0;
}

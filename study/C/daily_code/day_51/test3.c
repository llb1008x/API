/*************************************************************************
	> File Name: test3.c
	> Author: llb
	> Mail: llb1008x@126.com
	> Created Time: 2017年08月19日 星期六 23时09分38秒
	
	
题目：3

使用sscanf两个#号之间的字符串 abc#def#ghi
   使用sscanf读取"ti:简单爱]" ":"号与"]"之间的内容
   使用sscanf读取"[02:06.85]"  02（代表分钟）06（代表秒）到整形变量 minute、second中	
	
 ************************************************************************/

#include <stdio.h>

int main()
{
    char *buff="ti:简单爱]";
    char src[20]="";
    
        
    sscanf(src,"%*[^:]:%[^@]",buff);
    
    printf("src->%s\n",src);     


    return 0;
}

/*************************************************************************
	> File Name: test.c
	> Author: llb
	> Mail: llb1008x@126.com
	> Created Time: 2017年08月09日 星期三 23时55分29秒
	
	1. 编写程序，将一个字符串反向存放(编写函数实现)。
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "my_string.h"

//将一个字符串反向存储
char *my_strreverse(char *dst,char *src,int len)
{
    int i=0,j=0;

    for(i=len-1,j=0;i>=0;i--,j++){
    
        dst[j]=src[i];
    }
    
    
    return dst;
}

int main()
{
    char *str="abcefghi";    
    int len;
    
    len=mystrlen(str);
    char *dst=(char *)malloc(len);
    my_strreverse(dst,str,len);
    
    printf("src->%s,dst->%s\n",str,dst);
    free(dst);
    

    return 0;
}

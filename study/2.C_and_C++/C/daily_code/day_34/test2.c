/*************************************************************************
	> File Name: test2.c
	> Author: llb
	> Mail: llb1008x@126.com
	> Created Time: 2017年06月26日 星期一 23时31分54秒
 ************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main()
{
    char str1[10000]="abba";
    char str2[10000]="ababbba abbabbabbabbaacc";
    int len1,len2,len3;
    char ch;

    // 1.获取字符串
    //gets(str1);
    //gets(str2);
    len1=strlen(str1);
    len2=strlen(str2);

    printf("len1->%d,len2->%d\n",len1,len2);
    // 2.判断两个字符串是否满足条件
    if((len1>len2)||(len1<=0)||(len2>10000)){

        printf("error!\n");
        return -1;
    }

    puts(str1);
    puts(str2);

    // 3.利用字符串匹配函数寻找可能的位置
    char *p=strstr(str2,str1);
    ch=*p;
    *p='\0';

    len3=strlen(str2);

    puts(str2);
    printf("len3->%d\n",len3);


    return 0;
}

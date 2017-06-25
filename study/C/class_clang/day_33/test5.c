/*************************************************************************
	> File Name: test5.c
	> Author: llb
	> Mail: llb1008x@126.com
	> Created Time: 2017年06月25日 星期日 23时44分24秒

    慕课网C语言进阶
    作业一
    题目说起来很简单，你会读到两个字符串，每个字符串占据一行，每个字符串的长度均小于10000字符，
    而且第一个字符串的长度小于第二个字符串的。你的程序要找出第一个字符串在第二个字符串中出现的位置，
    输出这些位置，如果找不到，则输出-1。
 ************************************************************************/

#include <stdio.h>
#include <string.h>

int mystrstr(char *str1,char *str2)
{
    int ret=0;    

    return ret;
}

int main()
{
    char *str1;
    char *str2;
    
    gets(str1);
    gets(str2);
    printf("sizeof(str1)->%d,sizeof(str2)->%d\n",sizeof(str1),sizeof(str2));
    
    if( sizeof(str1)>sizeof(str2) ){

        printf("error\n");
        return -1;
    }


    puts(str1);
    puts(str2);

    return 0;
}

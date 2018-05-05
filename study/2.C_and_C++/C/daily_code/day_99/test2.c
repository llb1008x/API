/*************************************************************************
	> File Name: test2.c
	> Author: 
	> Mail: 
	> Created Time: 2018年03月05日 星期一 15时39分27秒
 ************************************************************************/

#include <stdio.h>
#include <string.h>

int main()
{
    char string[11];
    char *str="0123456789";
    strcpy(string,str);

    printf("string ->%s\n",string);

    return 0;
}

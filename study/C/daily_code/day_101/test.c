/*************************************************************************
	> File Name: test.c
	> Author: 
	> Mail: 
	> Created Time: 2018年03月07日 星期三 13时56分09秒
 ************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main()
{
    char *str="hello";
    int len=strlen(str);

    char *dest=(char *)malloc(len+1);
    char *d=dest;
    char *s=&str[len-1];

    while(len-- != 0){

        *d++ = *s--;
    }

    *d=0;
    printf("s->%s\n",dest);
    free(dest);

    return 0;
}

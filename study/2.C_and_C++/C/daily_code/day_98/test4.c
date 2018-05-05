/*************************************************************************
	> File Name: test4.c
	> Author: 
	> Mail: 
	> Created Time: 2018年02月28日 星期三 16时37分11秒
 ************************************************************************/

#include <stdio.h>

char *mystrcpy(char *dest,char *str)
{
    char *res=dest;

    if((dest==NULL)||(str==NULL)){
        printf("error!\n");
        return NULL;
    }

    while((*dest++ = *str++)!='\0');

    return res;
}

int main()
{
    char str1[20]="";
    char str2[10]="hello";
    printf("str->%s\n",mystrcpy(str1,str2));

    return 0;
}

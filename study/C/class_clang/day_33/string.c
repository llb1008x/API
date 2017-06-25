/*************************************************************************
	> File Name: string.c
	> Author: llb
	> Mail: llb1008x@126.com
	> Created Time: 2017年06月25日 星期日 14时14分24秒

    常用的字符串操作函数
    strlen,strcmp,strcpy,strcat,strchar,strstr
 ************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int mystrlen(const char *str)
{
    int cnt=0;

    while( *str!= '\0' ){

        str++;
        cnt++;
    }

    return cnt;
}

int mystrcmp(const char *str1,const char *str2)
{
    while((*str1==*str2)&&(*str1!='\0')){

        str1++;
        str2++;
    }

    return *str1-*str2;
}

char *mystrcpy(char *dst,const char *src)
{
    /*数组版本*/
/*  int index;
    while(src[index]!='\0'){

        dst[index]=src[index];
        index++;
    }
    dst[index]='\0';
*/
    /*指针版本*/
    char *tmp=dst;
    while( *dst++ = *src++ )
            ;
    *dst='\0';

    return tmp;
}

char *mystrcat(char *dst,const char *src)
{
    int index=0;
    
    while(dst[index]!='\0'){
        printf("index->%d\n",index);
        index++;
    }

    printf("****************\n");
    //index--;
    while(*src!='\0'){
        printf("index->%d\n",index);
        dst[index++]=*src++;
    }

    return dst;
}

int main()
{
    char dst[]="hello";
    char src[]="world";
    char *ret=mystrcat(dst,src);
    printf("ret->%s\n",ret);
   //free(ret);
/*
 * 字符串中找字符
 */
/*    
    char s[]="hello";
    char *p=strchr(s,'l');
    char c=*p;
    
    *p='\0';
    char *t=(char *)malloc(mystrlen(s)+1);
    strcpy(t,s);
    printf("t->%s\n",t);

    free(t);
*/    

/*
    char str[]="hello";
    char str2[]="hello";
    
    int len;
    char a[]="hello ";
    printf("sizeof->%d\n",sizeof(a));

    len=mystrlen(str);
    printf("len->%d\n",len);

    int ret;
    ret=mystrcmp(str,str2);
    printf("ret->%d\n",ret);
*/

/*
    char *dst;
    char src[]="hello world";

    dst=(char *)malloc(sizeof(src)+1);
    dst=mystrcpy(dst,src);
    printf("dst->%s\n",dst);

    free(dst);
*/
    return 0;
}

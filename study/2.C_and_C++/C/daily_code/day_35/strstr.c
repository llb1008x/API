/*************************************************************************
	> File Name: strstr.c
	> Author: llb
	> Mail: llb1008x@126.com 
	> Created Time: 2017年06月27日 星期二 09时22分13秒
	
	strstr字符串匹配函数的实现，来自百科

 ************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define VERSION_GCC
#if defined(VERSION_90)
//1.Copyright 1990 Software Development Systems, Inc.
char *strstr(const char *s1,const char *s2)
{
  int len;
  //此种情况下s2不能指向空，否则strlen无法测出长度，这条语句有问题
  if(!(len=strlen(s2)))
  		return (char *)s1;
  		
  for(;*s1;++s1){
  
  	if((*s1==*s2)&&(strncmp(s1,s2,len))==0)
  		return (char *)s1;
  }		

	return NULL;	
}

#elif defined(VERSION_99)
//2.Copyright 1986 - 1999 IAR Systems. All rights reserved
char *strstr(const char*s1,const char*s2)
{
    int n;
    if(*s2)
    {
        while(*s1)
        {
            for(n=0;*(s1+n)==*(s2+n);n++)
            {
                if(!*(s2+n+1))
                    return(char*)s1;
            }
            s1++;
        }
        return NULL;
    }
    else
        return (char*)s1;
}

#elif defined(VERSION_GCC)
// gcc-4.8.0
char *mystrstr(const char *s1,const char *s2)
{
    const char *p=s1;
    const size_tlen=strlen(s2);
    for(;(p=strchr(p,*s2))!=0;p++)
    {
        if(strncmp(p,s2,size_tlen)==0)
            return (char*)p;
    }
    return(0);
}



#endif


int main()
{

//	把目标字符串中匹配的字符串都切割出来，然后放到一个字符串数组内
/*
	char src[]="ababaababababaabb";
	char *ch="ab";
	char *dst[10];
	int i=0;
	
	while( (dst+i)=mystrstr(src,ch) ){
	
			printf("i->%d\n",i);
			i++;
	}

	
	for(i=0;i<10;i++){
		
		printf("dst[%d]->%s\n",i,dst+i);
	}
*/
	return 0;
}



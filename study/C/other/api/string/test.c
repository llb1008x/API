/*************************************************************************
	> File Name: test.c
	> Author: llb
	> Mail: llb1008x@126.com 
	> Created Time: 2017年08月07日 星期一 20时52分10秒

 ************************************************************************/
#include <stdio.h>
#include <string.h>

int main()
{

#if     1
   
   char buf[20]=""; 
   sscanf("abcdeADCfghl","%[^A-B]",buf);
   printf("buf->%s\n",buf);


#else
    char buf[20]="";
    int a,b,c;
    
    sprintf(buf,"%d:%d:%d",2017,8,9);
    printf("buf=%s\n",buf);
    
    sscanf("2017:8:9","%d:%d:%d",&a,&b,&c);
    printf("a=%d,b=%d,c=%d\n",a,b,c);
#endif

    return 0;
}






/*************************************************************************
	> File Name: test.c
	> Author: 
	> Mail: 
	> Created Time: 2018年03月06日 星期二 14时57分21秒
 ************************************************************************/

#include <stdio.h>
#include <arpa/inet.h>

int main(int argc,char *argv[])
{

#if  1
	
	unsigned char ip[]={172,20,223,75};
	char ip_str[16]="NULL";
	
	inet_ntop(AF_INET,(unsigned int *)ip,ip_str,16);
	printf("ip_str=%s\n",ip_str);


#else
	char ip_str[]="172.20.226.11";
	unsigned int ip_uint=0;
	unsigned char *ip_p=NULL;
	
	inet_pton(AF_INET,ip_str,&ip_uint);
	printf("ip_uint = %d\n",ip_uint);
	
	ip_p=(unsigned char *)&ip_uint;
	printf("ip_uint=%d.%d.%d.%d\n",*ip_p,*(ip_p+1),*(ip_p+2),*(ip_p+3));
#endif 
	
    return 0;
}

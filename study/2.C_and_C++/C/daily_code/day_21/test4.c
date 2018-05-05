/*************************************************************************
	> File Name: test4.c
	> Author: llb
	> Mail: llb1008x@126.com
	> Created Time: 2017年06月04日 星期日 15时56分50秒
 ************************************************************************/

#include <stdio.h>


int main()
{
/*    
    int x=0xfedcba98;
    printf("1 x->0x%x\n",x);

    x = x<<36;
    printf("2 x->0x%x\n",x);
*/  
    short int v=-1234;
    unsigned short uv=(unsigned short)v;

    printf("v=%d,uv=%u\n",v,uv);

    return 0;
}


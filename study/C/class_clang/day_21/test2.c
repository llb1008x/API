/*************************************************************************
	> File Name: test2.c
	> Author: llb
	> Mail: llb1008x@126.com
	> Created Time: 2017年06月04日 星期日 11时09分11秒
 ************************************************************************/

#include <stdio.h>

int bits(int x,int m)
{
    int result = x | m;
    return result;
}

int bitc(int x,int m)
{
    int result = x & (~m);
    return result;
}


int main()
{
    int x=0x1234,m=0x2468;
    int a=0,b=0;
    printf("1->0x%x\n",x);
    
    a=bits(x,m);
    printf("2->0x%x\n",a);
   
    b=bitc(x,m);
    printf("3->0x%x\n",b);

    return 0;
}

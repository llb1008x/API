/*************************************************************************
	> File Name: test.c
	> Author: llb
	> Mail: llb1008x@126.com
	> Created Time: 2017年05月17日 星期三 23时21分03秒
 ************************************************************************/

#include <stdio.h>

typedef  unsigned short int U16;

int main()
{
    U16 a=125;
    float b=3.6f;

    a=b;
    a=(int )b;

    printf("a->%d,b->%f\n",a,b);


    return 0;
}

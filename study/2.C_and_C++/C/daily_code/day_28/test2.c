/*************************************************************************
	> File Name: test2.c
	> Author: llb
	> Mail: llb1008x@126.com
	> Created Time: 2017年06月18日 星期日 22时36分48秒
 ************************************************************************/

#include <stdio.h>

typedef unsigned char *byte_pointer;

void show_bytes(byte_pointer start,size_t len)
{
    size_t i;

    for(i=0;i<len;i++){
        printf("%.2x",start[i]);
    }

    printf("\n");
}

int main()
{
    short sx=-12345;
    unsigned uy=sx;

    printf("uy=%u:\t",uy);
    show_bytes((byte_pointer)&uy,sizeof(unsigned));
    return 0;
}

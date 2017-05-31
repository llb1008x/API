/*************************************************************************
	> File Name: test.c
	> Author: llb
	> Mail: llb1008x@126.com
	> Created Time: 2017年05月30日 星期二 16时05分15秒
 验证字节序问题

 ************************************************************************/


#include <stdio.h>
#include <string.h>

typedef unsigned char *byte_pointer;

void show_bytes(byte_pointer start,size_t len)
{
    size_t i;

    for(i=0;i<len;i++){

        printf("%.2x",start[i]);
    }

    printf("\n");
}

void show_int(int x)
{
    show_bytes((byte_pointer)&x,sizeof(int));

}

void show_float(float x)
{
    show_bytes((byte_pointer)&x,sizeof(float));

}

void show_pointer(void *x)
{
    show_bytes((byte_pointer)&x,sizeof(void *));

}


int main()
{
    printf("hello world\n");

    int val=0x87654321;
    byte_pointer valp=(byte_pointer)&val;

    const char *s="abcdef";
    show_bytes((byte_pointer) s,strlen(s));

/*    
    show_bytes(valp,1);
    show_bytes(valp,2);
    show_bytes(valp,3);
    //int ret=12345.0;
    //show_int(val);
    //show_float(ret);
*/ 
}

/*************************************************************************
	> File Name: test3.c
	> Author: llb
	> Mail: llb1008x@126.com
	> Created Time: 2017年06月04日 星期日 11时26分32秒
 ************************************************************************/

#include <stdio.h>

int fun(int x,int y)
{
    int ret;

    if( x == (x|y)){
        ret=1;
    }else{
        ret=0;
    }

    return ret;
}

int func(int x,int y)
{
    int ret;
    if( x&&y ){
        ret=0; 
    }else{
        ret=1;
    }

    return ret;
}


int main()
{
    int x=0x1234,y=0x1235;
    int ret;

    ret=func(x,y);

    printf("ret->%d\n",ret);
    return 0;
}


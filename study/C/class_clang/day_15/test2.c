/*************************************************************************
	> File Name: test2.c
	> Author: llb
	> Mail: llb1008x@126.com
	> Created Time: 2017年05月17日 星期三 23时46分41秒
 ************************************************************************/

#include <stdio.h>
#define  NUM     100

int main()
{
    int i=0;
    for(i=1;i<NUM;i++){

        if( i%7 == 0 ){

            printf("i->%d\n",i);
        }
    }

    return 0;
}

/*************************************************************************
	> File Name: test3.c
	> Author: llb
	> Mail: llb1008x@126.com 
	> Created Time: 2017年05月02日 星期二 13时50分01秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>

int main()
{
    char *p;
	int i=0;

    while (1) {
        p = malloc(1);
        *p = 0;
		i++;
		printf("i->%d\n",i);
    }

	return 0;
}

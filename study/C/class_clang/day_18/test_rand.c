/*************************************************************************
	> File Name: test_rand.c
	> Author: llb
	> Mail: llb1008x@126.com
	> Created Time: 2017年05月20日 星期六 23时02分28秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int main()
{
    srand((unsigned )time(NULL));
    
    int i=0;
    for(i=0;i<10;i++){

        printf("i->%d,rand()->%d\n",i,rand()%100);
    }

    return 0;
}

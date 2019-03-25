/*************************************************************************
	> File Name: main.c
	> Author:llb 
	> Mail: llb1008x@126.com
	> Created Time: 2019年02月12日 星期二 09时59分23秒
 ************************************************************************/

#include<stdio.h>
#include "vector.h"

int x[2]={1,2};
int y[2]={3,4};
int z[2];

int main()
{
    addvec(x,y,z,2);
    printf("z=[%d %d]\n",z[0],z[1]);
    
    return 0;
}

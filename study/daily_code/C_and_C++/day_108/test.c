/*************************************************************************
	> File Name: test.c
	> Author:llb 
	> Mail: llb1008x@126.com
	> Created Time: 2019年01月19日 星期六 20时12分25秒

数组申明的内存排列    
 ************************************************************************/

#include<stdio.h>

int main()
{
    int a=3;
    int b=2;
    int array[3];
    
    array[0]=1;
    array[1]=10;
    array[2]=100;
    
    int *p=&a;
    int i;

    for(i=0;i<6;i++){
    
        printf("*p=%d\n",*p);
        p++;
    }

    printf("---------------------------\n");

    p=&a;
    for(i=0;i<6;i++){

        printf("p[%d]=%d\n",i,p[i]);
    }



    return 0;
}

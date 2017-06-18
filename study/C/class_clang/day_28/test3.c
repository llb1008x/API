/*************************************************************************
	> File Name: test3.c
	> Author: llb
	> Mail: llb1008x@126.com
	> Created Time: 2017年06月18日 星期日 23时17分20秒
 ************************************************************************/

#include <stdio.h>

float sum_elements(float a[],unsigned len)
{

    int i;
    float result=0;

    for(i=0;i<=len-1;i++){

        result += a[i];
    }

    return result;
}

int main()
{

    float a[]={0,1.0,2.0,3.4,4.5};
    float ret;
    int len=sizeof(a)/sizeof(float);

    len=0;
    ret=sum_elements(a,len);
    printf("ret->%f\n",ret);


    return 0;
}

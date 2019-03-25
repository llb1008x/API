#include <stdio.h>

int max(int a,int b)
{

    return (a>b)?a:b;
}


int min(int a,int b)
{

    return (a>b)?b:a;
}


int main()
{

    int ret=0;
    int a=5,b=10,k=3;

    ret=( (k>6)?max:min)(a,b);


    printf("ret=%d\n",ret);


    return 0;
}

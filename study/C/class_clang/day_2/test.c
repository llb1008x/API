/****************************************************
    C数据
****************************************************/

#include <stdio.h>

int main()
{
    printf("hello world\n");

    int x=2;
    printf("%ld,%ld\n",sizeof(int),sizeof(long));
    printf("1->%d,2->%d\n",x);

    int t1=12,t2=34,t3=45;

    printf("%o,%x,%#x\n",t1,t2,t3);

    char t='abc';
    int a='abc';
    printf("t->%c,%d,a->%c,%d\n",t,t,a,a);

    return 0;
}


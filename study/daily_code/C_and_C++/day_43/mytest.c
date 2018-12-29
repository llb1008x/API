#include <stdio.h>
#include "mylib.h"

int main()
{
    int a=10,b=20,max_num,min_num;
    
    max_num=max(a,b);
    min_num=min(a,b);
    printf("max->%d\n",max_num);
    printf("min->%d\n",min_num);
}


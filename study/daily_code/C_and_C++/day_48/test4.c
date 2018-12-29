/*************************************************************************
	> File Name: test4.c
	> Author: llb
	> Mail: llb1008x@126.com
	> Created Time: 2017年08月15日 星期二 00时01分37秒
 ************************************************************************/

#include <stdio.h>

union stu{
    
    long int x;
    char y;
    int z;

}a={10};

int main()
{
    a.x=5;
    a.y=6;
    a.z=15;
    
    printf("%d\n",a.x+a.y);

    return 0;
}

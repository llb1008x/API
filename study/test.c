#include <stdio.h>

/*
 * #define的用法：
 *定义一个跟行号相关的变量
 */

//为什么不能直接定义为DD(A)  BB(A,__LINE__)

#define BB(B,C) B##C
#define AA(B,C) BB(B,C)
#define DD(A)   AA(A,__LINE__)


int main()
{
    printf("line--->%d\n",DD(1));


    return 0;
}

#include <stdio.h>

#define TEST_DEFINE
//#define DATA_CONVERT1
//#define DATA_CONVERT2

/*
 * #define的用法：
 *定义一个跟行号相关的变量
 */

//为什么不能直接定义为DD(A)  BB(A,__LINE__)
#if defined(TEST_DEFINE)

#define BB(B,C) B##C
#define AA(B,C) BB(B,C)
#define DD(A)   AA(A,__LINE__)

#endif



int main()
{
#if defined(TEST_DEFINE)
    printf("line--->%d\n",DD(1));
#endif

#if defined(DATA_CONVERT1)
    int num=5;
    float f = (float)num/2;
    
    printf("f--->%f\n",f);
    unsigned int *p = &f;
	
    printf("s1=%x\n",*p);
#endif


#if defined(DATA_CONVERT2)
	unsigned int i = x;
	unsigned int *p = &x;
	printf("x=%f,i=0x%x,p_x=0x%x\n",x,i,*p);
#endif

    return 0;
}

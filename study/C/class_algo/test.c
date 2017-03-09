#include <stdio.h>

int Max( int A, int B, int C )
{ /* 返回3个整数中的最大值 */
    return A > B ? A > C ? A : C : B > C ? B : C;
}



int main()
{
	//printf("hello world\n");

	printf("MAX->%d\n",Max(1,3,5));


	return 0;
}

#include <stdio.h>
int main()
{
	float x=0;
	int i = 0;
	x=3.6f;
	i = x;			//x为实型,i为整型，直接赋值会有警告
	i = (int)x;		//使用强制类型转换
	printf("x=%f,i=%d\n",x,i);
	return 0;
}
 
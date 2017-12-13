#include <stdio.h>
//数组指针
int main()
{
	int a[5]={1,2,3,4,5};
    int (*p)[5] = &a;		//定义一个数组指针指向一个一维数组
    
	printf("%d\n",	*( (int *)(p+1) -1 )	);
	return 0;
}

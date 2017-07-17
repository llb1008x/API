#include <stdio.h>
//指针作函数的参数
void exchang1(int p1,int p2)
{
	int p;
	p = p1;
	p1 = p2;
	p2 = p;	
}
void exchang2(int *p1,int*p2)
{
	int p;
	p = *p1;
	*p1 = *p2;
	*p2 = p;
}
int	main(void)
{
	int a,b;
	scanf("%d %d",&a,&b);
	if(a>b)
	{
	//	exchang1(a,b);
		exchang2(&a,&b);
	}
	printf("a=%d,b=%d\n",a,b);
	return 0;
}
#include <stdio.h>
//采用指针法引用数组元素
int main()
{
	int a[5],i,*pa,*pb;
	pa = a;				   				//数组名代表首地址
	pb = &a[0];							//数组的第一个元素的地址也代表首地址
	
	for(i=0;i<5;i++)
	{
		a[i]=i;
		printf("a[%d]=%d\n",i,a[i]);
		printf("a[%d]=%d\n",i,*(a+i));
		printf("a[%d]=%d\n",i,*(pa+i));	
		printf("a[%d]=%d\n",i,*pb++);	
		printf("a[%d]=%d\n",i,pb[i]);	
		
	//	printf("a[%d]=%d\n",i, a++);		//a为常量，不允许执行++操作
	}
	printf("\n");
}
 
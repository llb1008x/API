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
//void max(int *p)
//void max(int (*a)[4])//将a当着指针来用
//void max(int a[][4])//将a当着指针来用
void max(int *a)//将a当着指针来用
{
	int i,j;
	printf("size-2 = %d\n",sizeof(a));
	for(i=0;i<3*4;i++)
		a[i] = a[i]+50;
		//*(a+i) = *(a+i)+50;

}
int	main(void)
{
	int a[3][4]={1,2,3,4,5,6,7,8,9,10,11,12};
	int i,j;

	printf("size-1 = %d\n",sizeof(a));
	max(a);
	for(i=0;i<3;i++){
		for(j=0;j<4;j++)
			printf("%3d ",a[i][j]);
		printf("\n");
	}
	return 0;
}
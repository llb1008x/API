#include <stdio.h>
//����ָ��
int main()
{
	int a[5]={1,2,3,4,5};
    int (*p)[5] = &a;		//����һ������ָ��ָ��һ��һά����
    
	printf("%d\n",	*( (int *)(p+1) -1 )	);
	return 0;
}

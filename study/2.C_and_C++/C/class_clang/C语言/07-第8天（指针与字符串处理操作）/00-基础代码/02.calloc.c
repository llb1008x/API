#include <stdlib.h> 
#include <stdio.h>
int main()
{ 
	int count,*array,n; 
	printf("������Ҫ��������ĸ���:\n");
	scanf("%d",&n);
	array=(int *) calloc ( n , sizeof (int) );
	if(array==NULL)
	{
		printf("����ռ�ʧ��!\n");
		return -1;
	}
	for (count = 0;count < n;count++) /*�����鸳ֵ*/ 
		array[count]=count; 

	for(count = 0;count < n;count++) /*��ӡ����Ԫ��*/ 
		printf("%2d",array[count]); 
	free(array);
	return 0;
} 


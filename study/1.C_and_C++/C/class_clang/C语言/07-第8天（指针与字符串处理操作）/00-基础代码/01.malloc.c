#include <stdlib.h> 
#include <stdio.h>
#include <string.h>
int main()
{ 
	int count,*array,n; 
	printf("������Ҫ��������ĸ���:\n");
	scanf("%d",&n);
	array=(int *) malloc ( n * sizeof (int) );
	if(array==NULL)
	{
		printf("����ռ�ʧ��!\n");
		return -1;
	}
	//�����뵽�ռ���0
	memset(array,0,sizeof(int)*n);
	
	for (count = 0;count < n;count++) /*�����鸳ֵ*/ 
		array[count]=count; 

	for(count = 0;count < n;count++) /*��ӡ����Ԫ��*/ 
		printf("%2d",array[count]); 
	free(array);
	return 0;
} 

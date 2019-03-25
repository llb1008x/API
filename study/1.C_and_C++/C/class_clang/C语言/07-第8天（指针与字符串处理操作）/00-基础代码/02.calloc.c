#include <stdlib.h> 
#include <stdio.h>
int main()
{ 
	int count,*array,n; 
	printf("请输入要申请数组的个数:\n");
	scanf("%d",&n);
	array=(int *) calloc ( n , sizeof (int) );
	if(array==NULL)
	{
		printf("申请空间失败!\n");
		return -1;
	}
	for (count = 0;count < n;count++) /*给数组赋值*/ 
		array[count]=count; 

	for(count = 0;count < n;count++) /*打印数组元素*/ 
		printf("%2d",array[count]); 
	free(array);
	return 0;
} 


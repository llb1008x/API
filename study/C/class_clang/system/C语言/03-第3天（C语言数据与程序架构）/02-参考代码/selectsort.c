#include "stdio.h"

void swap(int *a,int *b)
{                            
	int tmp;
	tmp = *a;
	*a = *b;
	*b = tmp;
}

void selectsort(int k[],int n)    /*选择排序*/
{
	int i,j,max;
	int temp;

	for(i=0;i<n;i++)// “ i ” 就是起始值
	{
		for(j=i+1;j<n;j++)          
		{
			if(k[j] < k[i])
				swap(&k[j],&k[i]);
		}
	}
}

int main()
{
    int i,a[10] = {2,5,6,3,7,8,0,9,12,3};        
    printf("The data array is:\n") ;
    for(i=0;i<10;i++)                        /*显示原序列之中的元素*/
        printf("%d ",a[i]);

    selectsort(a,10);                         /*执行选择排序*/
	printf("\nThe result of selection sorting for the array is:\n");
    for(i=0;i<10;i++)
        printf("%d ",a[i]);                        /*输出排序后的结果*/
	printf("\n");
	
	return 0;
}


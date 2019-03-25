#include "stdio.h"

void swap(int *a,int *b)
{                            
	int tmp;
	tmp = *a;
	*a = *b;
	*b = tmp;
}

void bubblesort(int k[],int n)
{                    /*冒泡排序*/
	int i,j,tmp;
	int flag = 1,cont = 0;

	for(i=0;i<n && flag == 1;i++)	//控制每趟往前推一个，即少比较一次
	{             
		//flag = 0;	//加上flag
		for(j=0;j<n-i-1;j++) //从第一个开始，不断与相邻的值比较，并交换最值，一直推到最后，形如冒泡
		{	
			cont++;
			if(k[j]<k[j+1])
			{         
				swap(&k[j+1],&k[j]);
				flag = 1;
			}
		}
	}

	printf("\n循环比较次数：%d\n",cont);
}

int main()
{
    int i,a[10] = {2,5,6,3,7,8,12,9,1,0};   
	
    printf("The data array is\n") ;
    for(i=0;i<10;i++)                            /*显示原序列之中的元素*/
        printf("%d ",a[i]);
	
	bubblesort(a,10);                            /*执行冒泡排序*/
    printf("\nThe result of bubble sorting for the array is\n");
    for(i=0;i<10;i++)
        printf("%d ",a[i]);                            /*输出排序后的结果*/
	printf("\n");
	
	return 0;
}

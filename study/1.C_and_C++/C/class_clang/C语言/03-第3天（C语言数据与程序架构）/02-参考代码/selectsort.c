#include "stdio.h"

void swap(int *a,int *b)
{                            
	int tmp;
	tmp = *a;
	*a = *b;
	*b = tmp;
}

void selectsort(int k[],int n)    /*ѡ������*/
{
	int i,j,max;
	int temp;

	for(i=0;i<n;i++)// �� i �� ������ʼֵ
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
    for(i=0;i<10;i++)                        /*��ʾԭ����֮�е�Ԫ��*/
        printf("%d ",a[i]);

    selectsort(a,10);                         /*ִ��ѡ������*/
	printf("\nThe result of selection sorting for the array is:\n");
    for(i=0;i<10;i++)
        printf("%d ",a[i]);                        /*��������Ľ��*/
	printf("\n");
	
	return 0;
}


/*************************************************************************
	> File Name: test.c
	> Author: llb
	> Mail: llb1008x@126.com 
	> Created Time: 2017年03月16日 星期四 22时15分33秒
	从一组数中确定第K个最大值
 ************************************************************************/

#include<stdio.h>

#define NUM  5

/*
	第一步是给这组数排序
	第二步是从这组数中排出指定的位数
*/
void sort(int a[])
{
	int i,j;
	int temp;

	for(i=0;i<NUM;i++){

		for(j=0;j<NUM-1;j++){

			if(a[j]>a[j+1]){

				temp=a[j];
				a[j]=a[j+1];
				a[j+1]=temp;
			}
		}
	}
/*
	for(i=0;i<NUM;i++){

		printf("%d\n",a[i]);
	}
*/

}



int main()
{
	int N[]={1,3,2,6,8,6,9,6,7,4};
	int i;

	sort(N);
	for(i=0;i<NUM;i++){

		printf("%d\n",N[i]);
	}

	return 0;
}

/*************************************************************************
	> File Name: task2.c
	> Author: llb
	> Mail: llb1008x@126.com 
	> Created Time: 2017年03月03日 星期五 09时58分54秒

功能：计算给定N个整数的序列{A1,A2,A3...An},求子序列的最大和
 ************************************************************************/

#include<stdio.h>

//#define		NUM			8

/***************************************
 *方案一
 * ************************************/
int MaxSum(int A[],int N)
{
	int This_Sum,Max_Sum;
	int i,j,k;

	//i序列的左端
	for(i=0;i<N;i++){
		//j序列的右端
		for(j=i;j<N;j++){			
			This_Sum=0;
			//计算i到j的子序列的和
			for(k=i;k<j;k++){
			
				This_Sum += A[k];
				printf("This_iSum->%d\n",This_Sum);
			
			}
		
			if(This_Sum>Max_Sum){	
				Max_Sum = This_Sum;
			}
		}
	
	}
	
	return Max_Sum;
}



/***************************************
 *方案二
 * ************************************/

int MaxSum2(int A[],int N)
{
	int This_Sum,Max_Sum;
	int i,j;

	for(i=0;i<N;i++){
	
		This_Sum=0;
		for(j=i;j<N;j++){
		
			This_Sum  += A[j];
			printf("This_Sum->%d\n",This_Sum);

			if(This_Sum>Max_Sum){
				Max_Sum=This_Sum;
			}
		}
		
	}
	return Max_Sum;
}

/****************************************
 *方案三：分而治之，快速排序法
 * *************************************/








/*****************************************
 *方案四：在线处理
 ****************************************/
int MaxSum4(int A[],int N)
{
	int This_Sum,Max_Sum;
	int i;

	This_Sum=Max_Sum=0;
	for(i=0;i<N;i++){
	
		This_Sum += A[i];

		if(This_Sum>Max_Sum){
					
			Max_Sum=This_Sum;
		}
		if(This_Sum<0)
			This_Sum=0;
	}

	return Max_Sum;
}





int main()
{
	printf("hello world\n");

	int A[]={4.-3,5,-2,-1,2,6,-2,4,-3};
	int t;
	
	t=MaxSum4(A,sizeof(A)/sizeof(int));

	printf("MaxSum4-->%d\n",t);


	return 0;
}

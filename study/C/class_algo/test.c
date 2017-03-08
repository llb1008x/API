#include <stdio.h>
#include <stdlib.h>


int MaxSum(int A[],int N)
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
	//printf("hello world\n");

	int num,i,t;
	scanf("%d",&num);
	
	int *p=malloc(sizeof(int)*num);
	
	for(i=0;i<num;i++){
	
		scanf("%d",p+i);
	}


	t=MaxSum(p,num);

	printf("%d\n",t);


	return 0;
}

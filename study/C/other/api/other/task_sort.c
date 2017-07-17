#include <stdio.h>

#define   N   10


int main()
{
	int score[N]={67,98,75,63,82,79,81,91,66,84};
	int temp=0;	
	
	int i=0,j=0;

	for(i=0;i<N;i++){

		printf("%d ",score[i]);
	}
	printf("\n");

	for(i=0;i<N;i++){


		for(j=0;j<N-1;j++){


			if(score[j]<score[j+1]){

				temp=score[j];
				score[j]=score[j+1];
				score[j+1]=temp;
			}
		}
	}

	for(i=0;i<N;i++){

		printf("%d ",score[i]);
	}
	printf("\n");	


	return 0;
}

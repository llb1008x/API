#include <stdio.h>

#define     N   10


/*求考试总分*/
int sum_score(int *score)
{
    int num=0;
    int i=0;
    
    for(i=0;i<N;i++){
        
        
        num += *(score+i);
    }
    
	return num;
}

/*求考试最大分*/
int max_score(int *score)
{
    int i=0,max=0;
    
    for(i=0;i<N;i++){
        
        if( *(score+i)>max){
            
            max= *(score+i);
        }
        
    }
    
    return max;
    
}


/*求考试最低分*/
int min_score(int *score)
{
    int i=0,min=*score;
    
    
    for(i=0;i<N;i++){
        
        
        if( *(score+i)<min){
            
            
            min= *(score+i);
        }
    }
    
    
    return min;
}


/*求平均分*/
double average_score(int *score)
{
    int i=0;
	double sum=0;
    
    for(i=0;i<N;i++){
        
        
        sum  += *(score+i);
    }
    
    return sum/N;
}

/*考试成绩降序*/
void sort_score(int *score)
{
    int i=0,j=0;
	int temp=0;

	for(i=0;i<N;i++){

		for(j=0;j<N-1;j++){

			if(score[j]<score[j+1]){
				
				temp=score[j];
				score[j]=score[j+1];
				score[j+1]=temp;		
			}
		}
	}
}

int main()
{
    int score[N]={67,98,75,63,82,79,81,91,66,84};
    int i=0;

    printf("sum_score=%d,max_score=%d,min_score=%d,average_score=%lf\n",
		sum_score(score),max_score(score),min_score(score),average_score(score));
    
	sort_score(score);
	for(i=0;i<N;i++){

		printf("%d ",score[i]);
	}

	printf("\n");
    
    return 0;
}

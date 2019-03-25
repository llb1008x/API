/*************************************************************************
	> File Name: main.c
	> Author: 
	> Mail: 
	> Created Time: 2019年01月10日 星期四 17时30分19秒
在一个长度为10的整型数组里面，保存了班级10个学生的考试成绩。要求编写5个函数，
分别实现计算考试的总分，最高分，最低分，平均分和考试成绩降序排序。
 ************************************************************************/

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
float averge_score(int *score)
{
    int i=0;
    float sum=0;
    
    for(i=0;i<N;i++){
        
        
        sum  += *(score+i);
    }
    
    return 1.0*sum/N;
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

    printf("sum_score=%d,max_score=%d,min_score=%d,average_score=%f\n",sum_score(score),max_score(score),min_score(score),averge_score(score));
    
	sort_score(score);
	for(i=0;i<N;i++){

		printf("%d ",score[i]);
	}

	printf("\n");
    
    
    return 0;
}

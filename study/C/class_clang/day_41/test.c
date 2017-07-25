/*************************************************************************
	> File Name: test.c
	> Author: llb
	> Mail: llb1008x@126.com 
	> Created Time: 2017年07月25日 星期二 19时51分39秒
 ************************************************************************/

#include<stdio.h>


int main()
{
	float a[5][3]={{80,75,56},
					{59,63,70},
					{85,45,90},
					{76,77,45}};
	char lesson[][10]={"语文","数学","外语"};
					
	int i,j,person_low[3]={0};
	float score=0,lesson_aver[3]={0};
	
	for(i=0;i<3;i++){
	
		for(j=0;j<5;j++){
		
			score += a[j][i];
			if(a[j][i]<60){
			
				person_low[i]++;
			}
		}
		
		lesson_aver[i]=score/5;
		score=0;
	}			
	
	//printf("各科的平均成绩：\n");
	for(i=0;i<3;i++){
	
		printf("%s的平均成绩%.2f\n",lesson[i],lesson_aver[i]);
	}		
	
	//printf("各科不及格的人数：\n");
	for(i=0;i<3;i++){
	
		printf("%s不及格的人数%d\n",lesson[i],person_low[i]);
	}
	
	return 0;
}

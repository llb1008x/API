#include <stdio.h>
int main()
{
	int i;
	float chinese_score[3]={85.5,78.5,90.5};
	float english_score[]={90,88,50};
	
	float math_score[5]={65,80,78};
	float computer_score[5];
	
	for(i=0;i<5;i++)
		printf("math_score[%d]=%f\n",i,math_score[i]);

	for(i=0;i<5;i++)
		printf("computer_score[%d]=%f\n",i,computer_score[i]);
	
	printf("请输入计算机的成绩(5个)");
	for(i=0;i<5;i++)
		scanf("%f",&computer_score[i]);
	
	for(i=0;i<5;i++)
		printf("%f\n",computer_score[i]);
	return 0;
}

#include <stdio.h>
int main()
{ 
	float a[5][3]={{80,75,56},{59,65,71},{59,63,70},{85,45,90},{76,77,45}};
	int i,j,person_low[3]={0};
	float s=0,lesson_aver[3]={0};
	for(i=0;i<3;i++)
	{
		for(j=0;j<5;j++)
		{
			s=s+a[j][i];
			if(a[j][i]<60)
				person_low[i]++;
		}
		lesson_aver[i]=s/5;	
		s=0;
	}
	printf("���Ƶ�ƽ���ɼ�:\n");
	for(i=0;i<3;i++)
		printf("%.2f\n",lesson_aver[i]);
	printf("���Ʋ����������:\n");
	for(i=0;i<3;i++)
		printf("%d\n",person_low[i]);
	return 0;
}

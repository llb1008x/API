#include <stdio.h>
//=========================================================================
//	�ļ�����	main.c
//  ����������	����ѧ����ƽ���ɼ��Ͳ������������
//	ά����¼:	2008-4-17 v1.0		by xdw
//=========================================================================
struct stu
{
  	int num;
	char *name;
	char sex;
	float score;
}temp,boy[5]={
			  {101,"Li ping",'M',45},			//����һ������5��Ԫ�صĽṹ�����鲢�����ʼ��
			  {102,"Zhang ping",'M',62.5},
			  {103,"He fang",'F',92.5},
			  {104,"Cheng ling",'F',87},
			  {105,"Wang ming",'M',58},
			};
int main(void)
{
	int i,j,c=0;
	float ave,s=0;
	for(i=0;i<5;i++)
	{
		s+=boy[i].score;			//�����ܷ�
		if(boy[i].score<60) 
		  c+=1;						//ͳ�Ʋ������˵ķ���
	}
	//printf("s=%f\n",s);				//��ӡ�ܷ���
	ave=s/5;						//����ƽ������

	for( i = 0 ;i<5;i++)
	{
		for( j = i+1; j<5 ; j++)
		{
			if (boy[j].score < boy[i].score )
			{
				 temp = boy[i];
				 boy[i] = boy[j];
				 boy[j] = temp;
			}
		}
	}

	for( i = 0 ;i<5;i++)
	{
		printf("name=%s sour=%f\n",boy[i].name,boy[i].score);
	}
	printf("average=%f\ncount=%d\n",ave,c); //��ӡƽ�����벻��������
	return 0;
}














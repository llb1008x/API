#include<stdio.h>
#include"lyrics_process.h"

int main(int argc, char *argv[])
{
	LRC_PTR lrc;
	LRC *head = NULL;
	
	head = dispose_lrc("./easy_love.lrc", &lrc);
	if(head == NULL)//�������ʧ��
	{
		printf("No this lyrics\n");
	}
	else
	{
		//print_lrc_link(lrc);//��ӡ���������ĸ�����Ӧ��ʱ��
		int i;
		for(i=0;i<lrc.lrc_arry_size;i++)//ʱ���Ǻ���ms
		{
			printf("time=%d, lrc==%s\n", lrc.lrc_arry[i]->time, lrc.lrc_arry[i]->lrc);
		}
	}
	
	free_lrc_arry(&lrc);//�ͷŸ�ʽ���(dispose_lrc)�����Դ
	
	return 0;
}
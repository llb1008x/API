#include<stdio.h>
#include"lyrics_process.h"

int main(int argc, char *argv[])
{
	LRC_PTR lrc;
	LRC *head = NULL;
	
	head = dispose_lrc("./easy_love.lrc", &lrc);
	if(head == NULL)//解析歌词失败
	{
		printf("No this lyrics\n");
	}
	else
	{
		//print_lrc_link(lrc);//打印解析出来的歌词与对应的时间
		int i;
		for(i=0;i<lrc.lrc_arry_size;i++)//时间是毫秒ms
		{
			printf("time=%d, lrc==%s\n", lrc.lrc_arry[i]->time, lrc.lrc_arry[i]->lrc);
		}
	}
	
	free_lrc_arry(&lrc);//释放歌词解析(dispose_lrc)后的资源
	
	return 0;
}
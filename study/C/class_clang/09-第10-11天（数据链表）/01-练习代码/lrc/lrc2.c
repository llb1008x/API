#include <stdio.h>
#include <string.h>
typedef  struct lrc{
	int m_time;
	char lrc_buf[200];
	struct lrc *front;
	struct lrc *next;
}LRC;


int main()
{
	char *str="[03:39.68][02:39.34][01:10.71]爱能不能够永远单纯没有悲哀";
	int time_buf[5];
	int i=0,j;
	int min,sec;
	LRC *head=NULL,*p_new;
	i=0;
	while(*str=='[')
	{
		
	}
	for(j=0;j<i;j++)
	{
		//申请节点
		//节点的时间成员赋值
		//节点的歌词成员赋值
		//插入链表
	}
	//遍历链表
	return 0;
}
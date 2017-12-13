#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define ROWS  5

typedef  struct lrc{
	int time;
	char lrc_buf[200];
	struct lrc *next;
}LRC;
void link_print(LRC *head)
{
	LRC *pb = head;
	
	while(pb != NULL){
		printf("%d\t%s\n",pb->time,pb->lrc_buf);
		pb = pb->next;
	}
}
LRC *link_insert(LRC *head,LRC *p_new)
{
	if(head == NULL){
		head = p_new;
		head->next = NULL;
	}else{
		p_new->next = head;
		head = p_new;	
	}
	return head;
}
LRC *link_search_by_time(LRC *head,int time)
{
	LRC *pb = head;
	
	while(pb != NULL){
		if(pb->time == time){
			return pb;
		}
		pb = pb->next;
	}	
	return NULL;
}
int msg_deal(char *msg_src,char *msg_done[],char *str)
{
	int i=0;
	msg_done[i]=strtok(msg_src,str);
	while(msg_done[i]!=NULL)
	{
		i++;
		msg_done[i]=strtok(NULL,str);
	}
	return i;
}
void show(LRC *p)
{
	static LRC *buffer[ROWS] = {NULL};
	static int count = 0;
	int i,current;
	if(p == NULL)
		return;
	system("clear");
	buffer[count++] = p;
	count %= ROWS;
	current = count;
	for(i=0;i<ROWS;i++){
		if(buffer[current] != NULL)
			printf("%s\n",buffer[current]->lrc_buf);	
		current++;
		current %= ROWS;
	}
}

int main()
{
	
	int time_buf[5];
	int i=0,j,k,song_time=0;
	char *str;
	int min,sec;
	LRC *head=NULL,*p_new,*pb;
	char *q[100];
	int len=0;
	char *lrc_buf;
	FILE *fp;
	fp = fopen("love.lrc","rb");
	fseek(fp,0,SEEK_END);
	len = ftell(fp);
	lrc_buf = malloc(len+1);
	rewind(fp);
	fread(lrc_buf,len,1,fp);
	//puts(lrc_buf);
	len=msg_deal(lrc_buf,q,"\r\n");
	// for(i=0;i<len;i++)
		// printf("q[%d]=%s\n",i,q[i]);
		
	for(k=4;k<len;k++)//处理第5行到最后一行
	{
		str=q[k];
		i=0;
		while(*str=='['){
			time_buf[i++] = atoi(str+1)*60+atoi(str+4);
			str = str+10;
		}
		for(j=0;j<i;j++)
		{
			p_new = malloc(sizeof(LRC));
			p_new->time = time_buf[j];
			strcpy(p_new->lrc_buf,str);
			head = link_insert(head,p_new);
		}
	}
//	link_print(head);
	while(1)
	{
		usleep(1000*100);
		song_time++;
		// pb = link_search_by_time(head,song_time);
		// if(pb != NULL){
			// system("clear");
			// printf("%d:%s\n",pb->time,pb->lrc_buf);
		// }
		show(link_search_by_time(head,song_time));
	}
	
	//link_print(head);
	
	return 0;
}
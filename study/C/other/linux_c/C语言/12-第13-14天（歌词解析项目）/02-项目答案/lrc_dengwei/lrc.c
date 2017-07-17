#include "lrc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 
#include "./pos/console.h"

void insert_link(LRC**p_head,LRC*p_new)
{
	LRC *p_mov=*p_head,*p_front;
	int flag=0;
	if(*p_head==NULL)
	{
		*p_head=p_new;
		p_new->next=NULL;
	}
	else
	{
		while(p_mov!=NULL)
		{
			if(p_new->time<p_mov->time)
			{
				if(p_mov==*p_head)
				{
					p_new->next=*p_head;
					*p_head=p_new;
				}
				else
				{
					p_front->next=p_new;
					p_new->next=p_mov;
				}
				flag=1;
				break;
			}
			p_front=p_mov;
			p_mov=p_mov->next;
		}
		if(flag==0)
		{
			p_front->next=p_new;
			p_new->next=NULL;
		}
	
	}

}


void lrc_read_file(char **p_lrc_buf,char *lrc_name)
{
	FILE *fp;
	int len;
	fp=fopen(lrc_name,"r");
	if(fp==NULL)
	{
		printf("文件打开失败\n");
		return ;
	}
	
	fseek(fp,0,SEEK_END);
	len=ftell(fp);
	rewind(fp);
	*p_lrc_buf=(char *)malloc(len+1);
	
	fread(*p_lrc_buf,1,len,fp);
	
	(*p_lrc_buf)[len]='\0';
	
	fclose(fp);
	
	
}

int lrc_deal(char *lrc_buf,char *str,char **lrc_done)
{
	int num=0;
	lrc_done[num]=strtok(lrc_buf,str);
	while(lrc_done[num]!=NULL)
	{
		num++;
		lrc_done[num]=strtok(NULL,str);
	}
	return num;

}


void lrc_title_deal(char *lrc_done[],char (*lrc_title)[100])
{
	int i;
	for(i=0;i<4;i++)
	{
		sscanf(lrc_done[i],"%*[^:]:%[^]]",lrc_title[i]);
	}
}

void lrc_title_display(char (*lrc_title)[100])
{
	int i;
	char *lrc_title_name[]={"歌名:","歌手:","专辑:","制作:"};
	for(i=0;i<4;i++)
	{
		printf("\t\t%s\t%s\n",lrc_title_name[i],lrc_title[i]);
	}

}

void print_link(LRC*p_mov)
{
	while(p_mov!=NULL)
	{
		printf("%d %s\n",p_mov->time,p_mov->lrc);
		p_mov=p_mov->next;
	}
}

void lrc_link_deal(LRC** head,char **lrc_done,int line_num)
{
	char *line_done[50];
	int row_num,minute,second,j,i;
	LRC *p_new=NULL;
	for(i=4;i<line_num;i++)
	{
		row_num=lrc_deal(lrc_done[i],"]",line_done);
		
		for(j=0;j<row_num-1;j++)
		{
			sscanf(line_done[j],"[%2d:%2d",&minute,&second);
			
			p_new=(LRC*)malloc(sizeof(LRC));
			p_new->time=minute*60+second;
			strcpy(p_new->lrc,line_done[row_num-1]);
			
			//printf("%d %s\n",p_new->time,p_new->lrc);
			
			insert_link(head,p_new);
			
			//printf("%d %d %s\n",minute,second,line_done[row_num-1]);
		}
		
		
	}

}


/*
mplayer_play(song_name);
*/


void lrc_display(LRC*head,char (*lrc_title)[100])
{
	int second=0;
	LRC	*p_mov=head;
	LRC *p_mov2;
	LRC *p_mov_front=head,*p_mov_front2;
	int i,flag=0;
	

	while(1)
	{
		system("clear");	
	
		
		lrc_title_display(lrc_title);
		
		printf("\t\t\t%02d:%02d\n",second/60,second%60);
		
		if(flag<4)
			p_mov_front=head;
		
		if(second==p_mov->time)
		{
			p_mov=p_mov->next;
			flag++;
			if(flag>4)
				p_mov_front=p_mov_front->next;
			
		}
		p_mov2 = p_mov;
		p_mov_front2=p_mov_front;
		while(p_mov_front2!=p_mov)
		{
			printf("\t%s\n",p_mov_front2->lrc);
			p_mov_front2=p_mov_front2->next;
		}
		
		
		for(i=0;i<5;i++)
		{
			if(i==0)
				set_fg_color(COLOR_RED);
			else
				set_fg_color(COLOR_CYAN);
				
			if(p_mov2!=NULL)
			{
				printf("\t%s\n",p_mov2->lrc);
				p_mov2=p_mov2->next;
			}
		}
		if(p_mov==NULL)
				break;
		
		usleep(10*2000);
		second++;
	}

}

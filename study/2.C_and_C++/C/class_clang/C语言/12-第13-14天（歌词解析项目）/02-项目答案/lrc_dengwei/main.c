#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "lrc.h"
#include "./pos/console.h"

#include "start_mplayer.h"


int main(int argc,char *argv[])
{
	char *lrc_name="./lrc/love.lrc";
	char *song_name="./song/love.mp3";
	char *lrc_buf;
	char *str="\r\n";
	char *lrc_done[100];
	int line_num;
	char lrc_title[4][100];
	//int i;
	LRC *head=NULL;
	lrc_read_file(&lrc_buf,lrc_name);
	//printf("lrc_buf=%s\n",lrc_buf);
	
	
	line_num=lrc_deal(lrc_buf,str,lrc_done);
	
	//for(i=0;i<line_num;i++)
	//	printf("lrc_done[%d]=%s\n",i,lrc_done[i]);
	
	
	lrc_title_deal(lrc_done,lrc_title);
	
	//lrc_title_display(lrc_title);
	

	lrc_link_deal(&head,lrc_done,line_num);
	
	print_link(head);
	
	
		
	mplayer_play(song_name);
	
	lrc_display(head,lrc_title);

	return 0;
}
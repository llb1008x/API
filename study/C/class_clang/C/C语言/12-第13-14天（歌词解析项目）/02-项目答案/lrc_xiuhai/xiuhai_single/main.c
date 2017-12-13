#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "lrc.h"
#include "start_mplayer.h"
#include "./pos/console.h"

int main(int argc,char *argv[])
{
	char song_msg[HEAD_NUM][HEAD_SIZE];//保存前四行歌曲信息首地址
	TIME m_time={0,0};
	char * lrc_file;
	unsigned int file_size;
	int   s_time=0;
	LRC_INFO *head=NULL,*cur_node = NULL;
	
	lrc_file=read_src_file("./lrc/简单爱.lrc",&file_size);//把lrc文件中的歌词内容读出来
	if(lrc_file == NULL)
		exit(-1);
	
	lrc_head_resolve(lrc_file,song_msg);//处理lrc文件的前四行
	head=lrc_resolve(lrc_file,file_size);//所有时间点都对应一个节点存放在链表中
	free(lrc_file);//回收之前申请的内存
	
	
	system("clear");
	print_song_message(song_msg);//打印歌曲信息	
	cur_node = head;
	mplayer_play("./song/简单爱.mp3");
	while(cur_node != NULL)//最后一个节点了退出循环
	{
		time_printf(m_time);//打印时间
		usleep(100*1000);
		s_time=s_time+1;
		++m_time.second;
		if(m_time.second==60)
		{
			m_time.second=0;
			++m_time.minute;
			if(m_time.minute==60)
				m_time.minute=0;
		}
		cur_node=play_song(s_time,head,song_msg);
	}
	link_free(head);
	cusor_show();
	return 0;
}

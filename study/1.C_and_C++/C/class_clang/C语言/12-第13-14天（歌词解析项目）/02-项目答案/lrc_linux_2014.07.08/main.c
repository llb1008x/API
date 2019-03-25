#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "lrc.h"
#include "start_mplayer.h"
#include "./pos/console.h"
TIME m_time={0,0};
char * song_msg[4];//保存前四行歌曲信息首地址
int main(int argc,char *argv[])
{
	char * lrc_str;
	char * lrc_text[200]={NULL};//指针数组保存每一行的地址
	
	int  line_num,s_time=0;
	LRC *head=NULL,*pb=NULL;
													//存储模拟时间
	lrc_str=read_src_file("./lrc/简单爱.lrc");//把lrc文件中的歌词内容读出来
	line_num=strtok_lrc_buf(lrc_text,lrc_str);//切割lrc_str ，每一行的地址保存在lrc_text中
	lrc_head_resolve(song_msg,lrc_text);//处理lrc文件的前四行
	head=lrc_resolve(line_num,lrc_text);//所有时间点都对应一个节点存放在链表中
	free(lrc_str);//回收之前申请的内存
	
	
	print_song_message(song_msg);//打印歌曲信息
	
	time_printf(m_time);//打印时间
	lrc_printf(head,NULL);
	
	while(1)//最后一个节点了退出循环
	{
		time_printf(m_time);//打印时间
		pb=link_search(head,s_time);
		if(pb!=NULL)//找到节点显示
		{
			lrc_show(head,pb);
			if(pb->lrc_cur_num==max_lrc_pointer_num)
				break;
		}
		usleep(1000*1000);
		s_time=s_time+1;
		++m_time.second;
		if(m_time.second==60)
		{
			m_time.second=0;
			++m_time.minute;
			if(m_time.minute==60)
				m_time.minute=0;
		}
	}
	link_free(head);
	cusor_show();
	return 0;
}
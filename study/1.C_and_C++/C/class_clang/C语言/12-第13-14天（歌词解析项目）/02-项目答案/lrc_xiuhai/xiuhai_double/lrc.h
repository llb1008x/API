#ifndef __LRC_INFO_H__
#define __LRC_INFO_H__

#define  HEAD_NUM		4
#define  HEAD_SIZE		50
#define  SPACE_SIZE		4

typedef struct lrc{
	signed int  time;
	char lrc_buf[200];
	int lrc_cur_num;
	struct lrc *next;
	struct lrc *prior;
}LRC_INFO;

typedef struct
{     												 		//表示日期时间的数据结构
	char minute;
	char second;
}TIME;

extern char * read_src_file(char *src_file_name,unsigned int *file_len);
extern void lrc_head_resolve(char *lrc_buffer,char song_msg[HEAD_NUM][HEAD_SIZE]);
extern LRC_INFO* lrc_resolve(char *lrc_file,unsigned int file_size);
extern void print_song_message(char song_msg[HEAD_NUM][HEAD_SIZE]);
extern LRC_INFO *play_song(unsigned int time,LRC_INFO *head,char song_msg[HEAD_NUM][HEAD_SIZE]);
extern void  time_printf(TIME m_time);
extern LRC_INFO *insert_list( LRC_INFO * head, LRC_INFO* pi);
extern void print_link( LRC_INFO * head);
extern void link_free(LRC_INFO *head);
extern void color_pos_printf(char *str,int x,int y,int color);

#endif

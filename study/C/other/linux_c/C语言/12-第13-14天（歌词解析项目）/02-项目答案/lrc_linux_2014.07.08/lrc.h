#ifndef __LRC_H__
#define __LRC_H__
typedef struct lrc{
	int  time;
	char lrc_buf[200];
	int lrc_cur_num;
	struct lrc *next;
}LRC;
typedef struct
{     												 		//表示日期时间的数据结构
	char minute;
	char second;
}TIME;
extern int max_lrc_pointer_num;

extern char * read_src_file(char *src_file_name);
extern int strtok_lrc_buf(char *lrc_text[],char *file_buf);
extern void lrc_head_resolve(char *song_msg[],char * lrc_text[]);
extern void print_song_message(char * song_msg[]);
extern LRC* link_search(LRC *head,int m_time);
extern LRC* lrc_resolve(int lrc_line,char * lrc_text[]);
extern LRC *InsertList(LRC* head,LRC *lrc_link);
extern void link_free(LRC *head);
extern void time_delay(int sec);
extern void lrc_show(LRC *head,LRC *expect_play_pointer);
extern void  time_printf(TIME m_time);
extern void color_pos_printf(char *str,int x,int y,int color);
extern int get_show_num(int find_num);
extern void lrc_printf(LRC *pb , LRC *expect_play_pointer);

#endif
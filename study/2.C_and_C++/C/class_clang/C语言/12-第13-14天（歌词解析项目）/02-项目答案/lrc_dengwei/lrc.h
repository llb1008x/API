#ifndef __LRC_H__
#define __LRC_H__

typedef struct lrc
{
	int time;
	char lrc[50];
	struct lrc *next;
}LRC;

void insert_link(LRC**p_head,LRC*p_new);
void lrc_read_file(char **p_lrc_buf,char *lrc_name);
int lrc_deal(char *lrc_buf,char *str,char **lrc_done);
void lrc_title_deal(char *lrc_done[],char (*lrc_title)[100]);
void lrc_title_display(char (*lrc_title)[100]);
void print_link(LRC*p_mov);
void lrc_link_deal(LRC** head,char **lrc_done,int line_num);
void lrc_display(LRC*head,char (*lrc_title)[100]);


#endif
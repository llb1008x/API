#ifndef _PLAYER_PROCESS_H_
#define _PLAYER_PROCESS_H_

#define SONG_PATH "../song/"
#define SQLITE_PATH "../sqlite/"
#define FIFO_PATH "../execute/"
#define LRC_PATH "../lyrics/"
#define PIC_PATH "../picture/"
#define TOTAL_SONG_NUM 200

/******���ĳ�ʼ��******/
extern void lock_init(PLAYER *player);

/********·����ʼ��*********/
extern void path_init(PLAYER *player);

/********�����ܵ���ʼ��********/
extern void fifo_fd_init(PLAYER *player);


extern void song_status_init(PLAYER *player);

extern int player_start(PLAYER *player);

extern void player_lrc_init(PLAYER *player);

#endif


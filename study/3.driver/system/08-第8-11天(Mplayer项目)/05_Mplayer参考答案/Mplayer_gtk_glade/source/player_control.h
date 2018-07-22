#ifndef _PLAYER_CONTROL_H_
#define _PLAYER_CONTROL_H_

#include"types.h"

/******mplayer���������******/
enum player_cmd_num
{
	CMD_BACK,//��һ��0
	CMD_FRONT,//��һ��1
	CMD_BCKWORD,//����2
	CMD_FORWORD,//���3
	CMD_PAUSE,//��ͣ4
	CMD_VOLUME,//��������5
	CMD_MUTE1,//����6
    CMD_PLAY_MODEL,//����ģʽ7
	CMD_CHANGESONG//�л�����8
};


/*******************************************************
*���ܣ�		���ո���ļ���·����ת��
*������		dest��ת��Ŀ���ַ
			length��ת��Ŀ���ַ�ռ��С
			src���ļ���·������ַ
*����ֵ��	�ɹ���ת��Ŀ���ַ��dest��ʧ�ܣ�NULL
********************************************************/
extern char *pathname_space_dispose(char *dest, int length, char *src);

/*****************************************************
��������:���������������
��������:�����ܵ���״̬
��������:
******************************************************/
extern int write_player_volume(FIFO_FD *fifo, SONG_STATUS *status);

/*****************************************************
��������:������ͣ״̬��ԭԭ������,����ֻ���ڷ�����ͣ�����ʹ��
��������:cmd_num=��ͣ����ţ�fifo=�����ܵ���status=״̬
��������:
******************************************************/
extern int deal_pause_and_volume(int cmd_num, PLAYER *player);

/***************************************
��������:д�����mplayer
��������:д�������(������һ������)
��������:
****************************************/
extern int write_player_cmd(FIFO_FD *fifo, SONG_STATUS *status, const char *cmd);

/***************************************
��������:ͨ�������ʵ�ַ��ͷ�����Ӧ���������mplayer
��������:PLAYER�ṹ�壬����ţ�������һ��ö��
��������:
****************************************/
extern int send_cmd_to_player(PLAYER *player, enum player_cmd_num cmd_num, const void *msg);

/******ѭ�����������ȡ������Ϣ�߳�********/
extern void *pthread_send_player_cmd(void *arg);

/**********��ȡ������Ϣ�߳�*****************/
extern void *pthread_rcv_player_msg(void *arg);

/**********�����ʾ�߳�**********************/
extern void *pthread_show_lrc_msg(void *arg);

extern void *pthread_key_control(void *arg);

#endif


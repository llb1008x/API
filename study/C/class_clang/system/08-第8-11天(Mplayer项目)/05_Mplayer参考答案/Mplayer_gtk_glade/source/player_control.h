#ifndef _PLAYER_CONTROL_H_
#define _PLAYER_CONTROL_H_

#include"types.h"

/******mplayer发送命令号******/
enum player_cmd_num
{
	CMD_BACK,//上一曲0
	CMD_FRONT,//下一曲1
	CMD_BCKWORD,//快退2
	CMD_FORWORD,//快进3
	CMD_PAUSE,//暂停4
	CMD_VOLUME,//控制音量5
	CMD_MUTE1,//静音6
    CMD_PLAY_MODEL,//播放模式7
	CMD_CHANGESONG//切换歌曲8
};


/*******************************************************
*功能：		带空格的文件及路径的转换
*参数：		dest：转换目标地址
			length：转换目标地址空间大小
			src：文件或路径名地址
*返回值：	成功：转换目标地址：dest；失败：NULL
********************************************************/
extern char *pathname_space_dispose(char *dest, int length, char *src);

/*****************************************************
函数功能:发送命令，控制音量
参数类型:命名管道，状态
返回类型:
******************************************************/
extern int write_player_volume(FIFO_FD *fifo, SONG_STATUS *status);

/*****************************************************
函数功能:根据暂停状态还原原来音量,并且只有在发送暂停命令后使用
参数类型:cmd_num=暂停命令号，fifo=命名管道，status=状态
返回类型:
******************************************************/
extern int deal_pause_and_volume(int cmd_num, PLAYER *player);

/***************************************
函数功能:写命令给mplayer
参数类型:写入的命名(完整的一条命令)
返回描述:
****************************************/
extern int write_player_cmd(FIFO_FD *fifo, SONG_STATUS *status, const char *cmd);

/***************************************
函数功能:通过命令号实现发送发送相应的命令控制mplayer
参数类型:PLAYER结构体，命令号，设置了一个枚举
返回描述:
****************************************/
extern int send_cmd_to_player(PLAYER *player, enum player_cmd_num cmd_num, const void *msg);

/******循环发送命令获取歌曲信息线程********/
extern void *pthread_send_player_cmd(void *arg);

/**********读取歌曲信息线程*****************/
extern void *pthread_rcv_player_msg(void *arg);

/**********歌词显示线程**********************/
extern void *pthread_show_lrc_msg(void *arg);

extern void *pthread_key_control(void *arg);

#endif


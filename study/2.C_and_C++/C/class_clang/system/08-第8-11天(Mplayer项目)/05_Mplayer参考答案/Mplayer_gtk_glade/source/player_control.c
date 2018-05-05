#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include"player_control.h"
#include"player_display.h"
#include"player_process.h"
#include"./LRC/lyrics_process.h"
#include"./encoding/gb2312_ucs2.h"

static void send_init_cmd_to_mplayer(PLAYER *player);
static void send_time_percent_cmd_to_mplayer(PLAYER *player);


/*******************************************************
*功能：		带空格的文件及路径的转换
*参数：		dest：转换目标地址
			length：转换目标地址空间大小
			src：文件或路径名地址
*返回值：	成功：转换目标地址：dest；失败：NULL
********************************************************/
char *pathname_space_dispose(char *dest, int length, char *src)
{
	char *tmp;
	int src_length = 0;
	tmp = dest;
	
//	printf("src=[%s]\n", src);
	memset(dest, 0, length);
	while(*src != 0)
	{
		src_length++;
		if(src_length > (length-1)){
			return NULL;
		}
		if(*src == ' '){
			*dest = '\\';
			dest++;
		}
		*dest = *src;
		dest++;
		src++;
	}
	//printf("dest=[%s]\n", tmp);
	//printf("length=%d\n", dest-tmp);
	return tmp;
}


/*****************************************************
函数功能:发送命令，控制音量
参数类型:命名管道，状态
返回类型:
******************************************************/
int write_player_volume(FIFO_FD *fifo, SONG_STATUS *status)
{
	char volume_cmd[32] = "";
	int wr_len;
	snprintf(volume_cmd, sizeof(volume_cmd), "volume %d 1\n",
		status->volume);
	wr_len = write(fifo->cmd_fifo_fd, volume_cmd, strlen(volume_cmd));
	return wr_len;
}

/*****************************************************
函数功能:根据暂停状态还原原来音量,并且只有在发送暂停命令后使用
参数类型:cmd_num=暂停命令号，fifo=命名管道，status=状态
返回类型:
******************************************************/
int deal_pause_and_volume(int cmd_num, PLAYER *player)
{
	if(cmd_num == CMD_PAUSE)
	{
		player->status.ispause = (player->status.ispause==1 ? 0 : 1);
		if(player->status.ispause==0){
			pthread_mutex_lock(&player->fifo.write_cmd_mutex);
			write_player_volume(&player->fifo, &player->status);
			pthread_mutex_unlock(&player->fifo.write_cmd_mutex);
		}
	}
	return player->status.ispause;
}

/***************************************
函数功能:写命令给mplayer
参数类型:写入的命名(完整的一条命令)
返回描述:
****************************************/
int write_player_cmd(FIFO_FD *fifo, SONG_STATUS *status, const char *cmd)
{
	pthread_mutex_lock(&fifo->write_cmd_mutex);
	int wr_len = write(fifo->cmd_fifo_fd,cmd,strlen(cmd));
	/*暂停处理*/
	if(status->ispause==1 && strcmp(cmd, "pause\n")!=0){
		write(fifo->cmd_fifo_fd, "pause\n", strlen("pause\n"));
	}/*音量处理*/
	else if(status->ispause==0 && strcmp(cmd, "pause\n")!=0){
		write_player_volume(fifo, status);
	}
	
	pthread_mutex_unlock(&fifo->write_cmd_mutex);
	return wr_len;
}

static int player_change_song_deal(PLAYER *player, char *cmd, int size)
{
	char song_cur[128] = "";
	player->status.play_change_flag = 1;
	player->status.ispause = 0;
	
	memset(player->status.song_cur, 0, sizeof(player->status.song_cur));
	strcpy(player->status.song_cur, player->list.song_list[player->status.song_cur_num]);
	//printf("song_cur------------%s-\n", player->status.song_cur);
	pathname_space_dispose(song_cur, sizeof(song_cur), player->status.song_cur);
	snprintf(cmd, size, "loadfile %s%s\n",
			player->path.song_path, song_cur);
	return 0;
}

/***************************************
函数功能:通过命令号实现发送发送相应的命令控制mplayer
参数类型:PLAYER结构体，命令号，设置了一个枚举
返回描述:
****************************************/
int send_cmd_to_player(PLAYER *player, enum player_cmd_num cmd_num, const void *data)
{
	char cmd[256] = "";
	
//	printf("=====%d========\n", cmd_num);
	switch(cmd_num)
	{
		case CMD_BACK:/*上一曲*/
			player->status.song_cur_num--;
			if(player->status.song_cur_num < 0){
				player->status.song_cur_num = player->list.song_num-1;
			}
			player_change_song_deal(player, cmd, sizeof(cmd));
			//printf("cmd========%s=====\n", cmd);
			break;

		case CMD_FRONT:/*下一曲*/
			player->status.song_cur_num++;
			if(player->status.song_cur_num >= player->list.song_num){
				player->status.song_cur_num = 0;
			}
			player_change_song_deal(player, cmd, sizeof(cmd));
			//printf("cmd========%s=====\n", cmd);
			break;
		
		case CMD_BCKWORD:	/*2 快退*/
		case CMD_FORWORD:	/*3 快进*/
			if((player->status.time_position + (*(int *)data))>= (player->status.song_length-10))
			{
				printf("---------%d------\n", *(int *)data);
				return -1;
			}
			snprintf(cmd, sizeof(cmd), "seek %d\n", *(int *)data);
			break;

		case CMD_PAUSE:		/*4 暂停*/
			strncpy(cmd, "pause\n", sizeof(cmd));
			break;

		case CMD_VOLUME:	/*5 音量控制*/
			if(data!=NULL){	
				player->status.volume = *(int *)data;
				player->status.temp_volume = player->status.volume;
				snprintf(cmd, sizeof(cmd), "volume %d 1\n",
							player->status.volume);
			}
			break;

		case CMD_MUTE1:		/*6 静音*/
			if(player->status.ismute==0){
				player->status.ismute = 1;
				player->status.temp_volume = player->status.volume;
				player->status.volume = 0;
				strcpy(cmd, "mute 1\n");
			}else{
				player->status.ismute = 0;
				player->status.volume = player->status.temp_volume;
				strcpy(cmd, "mute 0\n");
			}
			break;

		case CMD_CHANGESONG:/*7 切换歌曲*/
			if(data != NULL){
				player->status.song_cur_num = *(int *)data;
			}
			player_change_song_deal(player, cmd, sizeof(cmd));
			break;

		default:
				printf("cmd not found!!\n");
				return -1;
			break;
		
	}
	int wr_len;
	wr_len = write_player_cmd(&player->fifo, &player->status, cmd);
	if(wr_len>0){
		if(cmd_num==CMD_BACK || cmd_num==CMD_FRONT|| cmd_num==CMD_CHANGESONG)
			send_init_cmd_to_mplayer(player);
		else if(cmd_num == CMD_BACK || cmd_num == CMD_FORWORD)
			send_time_percent_cmd_to_mplayer(player);

		deal_pause_and_volume(cmd_num, player);/*暂停和音量处理*/
	}
	
	return wr_len;
}


static void send_init_cmd_to_mplayer(PLAYER *player)
{
	char *detail_cmd = "get_time_length\nget_meta_artist\n";
	int i;
	for(i=0;i<2;i++)
	{
		write_player_cmd(&player->fifo, &player->status, detail_cmd);
	}
}

static void send_time_percent_cmd_to_mplayer(PLAYER *player)
{
	char *detail_cmd ="get_time_pos\nget_percent_pos\n";
	write_player_cmd(&player->fifo, &player->status, detail_cmd);
}

void *pthread_send_player_cmd(void *arg)
{
	PLAYER *player = (PLAYER *)arg;
	send_init_cmd_to_mplayer(player);
	while(1)
	{
		if(player->status.ispause != 1){	/*暂停之后不发送命令*/
			char *detail_cmd ="get_time_pos\nget_percent_pos\n";
			write_player_cmd(&player->fifo, &player->status, detail_cmd);
		}
		usleep(100*1000);
	}
}

static int rcv_player_msg(PLAYER *player)
{
	int i = 0;
	char buf[100] = "";
	int ss_flag = 0;
	char artist[128] = "";
	static int time_postion = -1;
	static int percent = -1;
	char *position = NULL;
	char *msg_cmp[] = {"ANS_TIME_POSITION","ANS_PERCENT_POSITION",
						"ANS_LENGTH",
						"ANS_META_ARTIST"};
	read(player->fifo.msg_fifo_fd, buf, sizeof(buf));
	//printf("buf=====------%s-----------------\n",buf);
	for(i=0;i<sizeof(msg_cmp)/sizeof(msg_cmp[0]);++i)
	{
		position = strstr(buf, msg_cmp[i]);
		if(position != NULL)
		{
			switch(i)
			{
				case 0:
					sscanf(position, "%*[^=]=%d", &player->status.time_position);
					if(time_postion != player->status.time_position){
						time_postion = player->status.time_position;
						player_current_time_display(player, player->status.time_position);
					}
					break;
				case 1:	
					sscanf(position, "%*[^=]=%d", &player->status.percent);
					if(percent != player->status.percent){
						percent = player->status.percent;
						player_percent_display(player, player->status.percent);
					}
					break;
				case 2:
					sscanf(position, "%*[^=]=%d", &player->status.song_length);
					player_song_length_display(player, player->status.song_length);
					break;
				case 3:
					memset(artist, 0, sizeof(artist));
					sscanf(position, "%*[^']'%[^']", artist);
					player_artist_display(player, artist);
					break;

				default:

					break;
			}
		}
	}

	return ss_flag;
}

/***************************************
函数功能:接收mplayer传输的信息
参数类型:PLAYER结构体
返回描述:
****************************************/
void *pthread_rcv_player_msg(void *arg)
{
	PLAYER *player = (PLAYER *)arg;
	int restart_flag = 0;
	while(1)
	{
		rcv_player_msg(player);
		if(player->status.percent>=99 && restart_flag==0){
			restart_flag = 1;
			player_auto_play(player);/*下一曲*/
		}
		else if(player->status.percent<98 && restart_flag==1){
			restart_flag = 0;
		}
	}
}


static char *get_play_current_lrc(PLAYER *player, char *lrc_pathname)
{
	char name[100] = "";
	char *p = NULL;
	p = strstr(player->list.song_list[player->status.song_cur_num],".mp3");
	
	memcpy(name,player->list.song_list[player->status.song_cur_num],
		(int)(p - player->list.song_list[player->status.song_cur_num]));
	sprintf(lrc_pathname,"%s%s.lrc",
		player->path.lrc_path, name);
	
	return lrc_pathname;
}

static void display_current_lrc(PLAYER *player, const char *text, const int i)
{
	gdk_threads_enter();
	gtk_label_set_text(GTK_LABEL(player->win.lrc_light), text);
	gdk_threads_leave();
}

void *pthread_show_lrc_msg(void *arg)
{
	LRC_PTR lrc;
	PLAYER *player = (PLAYER *)arg;
	char lrc_pathname[128] = "";
	LRC *open_lrc_head = NULL;
	int find_lrc_flag = 0;
	memset(&lrc, 0, sizeof(LRC_PTR));
	while(1)
	{
		usleep(600*1000);
		if(player->status.ispause == 1)
			continue;
		
		if(player->status.play_change_flag==1){
			player->status.play_change_flag = 0;
			memset(lrc_pathname, 0, sizeof(lrc_pathname));
			get_play_current_lrc(player,lrc_pathname);
			
			if(find_lrc_flag==1){
				free_lrc_arry(&lrc);
			}
			
			open_lrc_head = dispose_lrc(lrc_pathname, &lrc);
			if(open_lrc_head==NULL){
				find_lrc_flag = 0;
				display_current_lrc(player, "No lyrics", 0);
			}
			else{
				char lrc_temp[200] = "";
				gb2312_to_utf8((unsigned char *)lrc.lrc_arry[0]->lrc, (unsigned char *)lrc_temp);	
				display_current_lrc(player, lrc_temp, 0);
				find_lrc_flag = 1;
			}
		}
		if(player->status.play_change_flag==0 && NULL != open_lrc_head){
			int i;
			for(i=0;i<lrc.lrc_arry_size;++i)
			{
				if((lrc.lrc_arry[i]->time/1000)==player->status.time_position)
				{
					char lrc_temp[200] = "";
					gb2312_to_utf8((unsigned char *)lrc.lrc_arry[i]->lrc, (unsigned char *)lrc_temp);	
					display_current_lrc(player, lrc_temp, 0);
					//printf("time--%d--%s\n",lrc.lrc_arry[i]->time/1000, lrc_temp);
				}
			}
		}
	}
}




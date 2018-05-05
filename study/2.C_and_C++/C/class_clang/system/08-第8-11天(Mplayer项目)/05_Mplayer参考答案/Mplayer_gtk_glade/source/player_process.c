#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include"types.h"
#include"player_process.h"
#include"player_control.h"
#include"./LRC/lyrics_process.h"
#include"./encoding/gb2312_ucs2.h"


/******锁的初始化******/
void lock_init(PLAYER *player)
{
	sem_init(&player->fifo.sem_wr, 0, 1);
	sem_init(&player->fifo.sem_rd, 0, 0);

	pthread_mutex_init(&player->fifo.write_cmd_mutex, NULL);//创建写mplayer命令的互斥锁
	pthread_mutex_init(&player->fifo.rate_cmd_mutex, NULL);//进度

	pthread_rwlock_init(&player->path.path_rwlock, NULL);//初始化一个读写锁
	pthread_rwlock_init(&player->list.list_rwlock, NULL);
	pthread_rwlock_init(&player->status.status_rwlock, NULL);
}



/********路径初始化*********/
void path_init(PLAYER *player)
{
	strncpy(player->path.song_path, SONG_PATH, sizeof(player->path.song_path));
	strncpy(player->path.fifo_path, FIFO_PATH, sizeof(player->path.fifo_path));
	strncpy(player->path.lrc_path, LRC_PATH, sizeof(player->path.lrc_path));
	strncpy(player->path.pic_path, PIC_PATH, sizeof(player->path.pic_path));
}

/***************************************
函数功能:创建mplayer接收命令和传输数据的命名管道
参数类型:命名管道，路径
返回描述:
****************************************/
int create_cmd_msg_fifo(FIFO_FD *fifo, PATH *path)
{
	char cmd_pathname[128] = "";
	char msg_pathname[128] = "";
	strcpy(cmd_pathname, path->fifo_path);
	strcat(cmd_pathname, "player_cmd_fifo");

	strcpy(msg_pathname, path->fifo_path);
	strcat(msg_pathname, "player_msg_fifo");
	
	unlink(cmd_pathname);
	unlink(msg_pathname);

	int cmd_fd = mkfifo(cmd_pathname,0666);
	int msg_fd = mkfifo(msg_pathname,0666);
	if((cmd_fd<0) || (msg_fd)<0)
	{
		perror("mkfifo error");
		_exit(1);
	}
	fifo->cmd_fifo_fd = open(cmd_pathname,O_RDWR);
	fifo->msg_fifo_fd = open(msg_pathname,O_RDWR);
	if((fifo->cmd_fifo_fd<0)||(fifo->msg_fifo_fd<0))
	{
		perror("open fifo error");
		_exit(2);
	}

	return 1;
}


/********命名管道初始化********/
void fifo_fd_init(PLAYER *player)
{
	create_cmd_msg_fifo(&player->fifo, &player->path);
}


/***********歌曲状态初始化**************/
void song_status_init(PLAYER *player)
{
	if(player->list.song_list != NULL){
		strcpy(player->status.song_cur, player->list.song_list[0]);
	}
	player->status.ispause = 0;
	player->status.ismute = 0;
	player->status.play_change_flag = 1;
	player->status.volume = 30;
	player->status.play_model = 0;
	player->status.song_cur_num = 0;
	player->status.percent = 0;
	player->status.time_position = 0;
}



/***************************************
函数功能:创建进程，启动mplayer
参数类型:PLAYER结构体
返回描述:
****************************************/
int player_start(PLAYER *player)
{
	pid_t pid = 0;

	if(player->list.song_list==NULL)
	{
		printf("------No song can player,song_list is NULL-----\n");
		return -1;
	}
	player->process.pid = vfork();
	if(player->process.pid<0){
		perror("player start fork error");
		return -1;
	}
	else if(player->process.pid==0)
	{
		char pathname[200] = "";
	//	char song_cur[64] = "";
	//	pathname_space_dispose(song_cur, sizeof(song_cur), player->status.song_cur);
		snprintf(pathname,sizeof(pathname),"%s%s",
			player->path.song_path, player->list.song_list[0]);

		printf("===============%s==========\n", pathname);
		
		dup2(player->fifo.msg_fifo_fd, STDOUT_FILENO);/*重定向标准输出*/
		close(STDIN_FILENO);
		execlp("../execute/mplayer_arm",
				"mplayer", "-ac", "mad",
				"-slave","-idle", "-quiet",
				"-input", "file=../execute/player_cmd_fifo",
				pathname, NULL);
		
		perror("execl error");
		_exit(-1);
	}

	send_cmd_to_player(player, CMD_VOLUME, &player->status.volume);//首先设置初始音量
	
	return pid;
}




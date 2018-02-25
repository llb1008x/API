#ifndef _TYPES_H_
#define _TYPES_H_

#include<gtk/gtk.h>
#include<glade/glade.h>
#include<pthread.h>
#include<semaphore.h>
#include"./sungtk_interface/sungtk_interface.h"
#include"./LRC/lyrics_process.h"

#define A8_SCREEN_WIDTH 800
#define A8_SCREEN_HEIGHT 480


#define PATH_SIZE 128						/*路径大小*/
#define SONG_SIZE 128						/*歌名大小*/


#define SONG_PATH "../song/"
#define LRC_PATH "../lyrics/"

typedef struct
{
	GladeXML *gxml;
	GtkWidget * win_back;			/*窗口名,主窗口*/
	GtkWidget * win_back_fixed;		/*窗口固定布局*/
	GtkWidget * win_back_tabel;		/*窗口表格布局*/
	GtkWidget * win_list_eventbox;	/*歌曲列表存放盒子*/
	GtkWidget * win_list_fixed;		/*歌曲列表存放*/

	SunGtkCList * win_list;			/*歌曲列表*/

	GtkWidget * volume_event_box;
	GtkWidget * volume_fixed;
	GtkWidget * mute;	/*静音按钮*/
	GtkWidget * volume_rate;
	GtkWidget * volume_rate_fixed;

	GtkWidget * lrc_light;	/*歌词显示*/


	GtkWidget * rate;	/*进度条*/
	GtkWidget * pause;	/*暂停按钮*/	
	GtkWidget * next;	//下一曲按钮
	GtkWidget * back;	//上一曲按钮
	
	GtkWidget * song;	/*显示当前播放歌曲*/
	GtkWidget * singer;	//歌唱者
	GtkWidget * time;	/*显示当前播放时间*/
	GtkWidget * length;	/*歌曲总长度*/
	GtkWidget * image_artist;	/*艺术家*/
}PLAYER_WIN;

/******歌曲状态信息*****/
typedef struct
{
	char song_cur[SONG_SIZE];		//当前播放的歌曲
	int ispause;               	 	//mplayer暂停标志位；0：未暂停，1：已暂停
	int ismute;						//静音标志位
	int play_change_flag;			//切换歌曲标志位
	int volume;                 	//mplayer音量范围（0-100）
	int temp_volume;
	int play_model;					//播放模式
	int song_cur_num;				//当前播放歌曲编号
	int percent;					//播放进度百分比
	int time_position;				//播放进度时间
	int song_length;				//播放时间总长度

	pthread_rwlock_t status_rwlock; //状态读写锁
}SONG_STATUS;

/******歌曲列表*******/
typedef struct 
{
	char **song_list;				//歌曲列表指针
	int song_num;               	//歌曲总数

	LRC_PTR lrc;

	pthread_rwlock_t list_rwlock;	//列表读写锁
}SONG_LIST;

/*********路径相关*********/
typedef struct
{
	char song_path[PATH_SIZE];				//歌曲路径
	char fifo_path[PATH_SIZE];				//存放命名管道路径名
	char lrc_path[PATH_SIZE];				//存放歌词路径名
	char pic_path[PATH_SIZE];				//存放图片路径名

	pthread_rwlock_t path_rwlock;			//路径读写锁
}PATH;

/*******mplayer进程启动*******/
typedef struct
{
	pid_t pid;						//mplayer启动进程号
	int play_flag;					//mplayer启动标志位;0:未启动,1:已启动

	pthread_rwlock_t process_rwlock;
}PLAYER_PROCESS;


/*******打开的命名管道文件描述符********/
typedef struct
{
	int cmd_fifo_fd;				//向mplayer发送命令的文件描述符
	int msg_fifo_fd;				//接收mplayer信息的文件描述符
	pthread_mutex_t write_cmd_mutex;//写命令的命名管道互斥锁
	
	sem_t sem_wr;					//发送命令的信号量
	sem_t sem_rd;					//接收Mplayer信息的信号量

	pthread_mutex_t rate_cmd_mutex;
}FIFO_FD;


/*******整个mplayer启动结构体*********/
typedef struct 
{
	PLAYER_WIN win;					//Mplayer窗口
	SONG_STATUS status;				//歌曲状态
	SONG_LIST list;					//歌曲列表
	PATH path;						//各种路径
	PLAYER_PROCESS process;
	FIFO_FD fifo;					//命名管道
}PLAYER;



#endif

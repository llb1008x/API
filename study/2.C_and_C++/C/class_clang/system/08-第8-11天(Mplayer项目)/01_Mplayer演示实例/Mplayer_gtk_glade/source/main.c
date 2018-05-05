#include<stdio.h>
#include<string.h>
#include<gtk/gtk.h>
#include<pthread.h>
#include"types.h"
#include"player_display.h"
#include"player_process.h"
#include"player_control.h"

//Mplayer初始化
void player_init(PLAYER *player)
{
	path_init(player);//路径初始化
	lock_init(player);//互斥锁
	fifo_fd_init(player);//命名管道创建、打开
	
	player_window_init(player);//窗口界面初始化
	player_song_list_display(player);//歌曲列表显示
	song_status_init(player);//歌曲状态值初始化

	player_song_cur_display(player);//高亮显示当前播放歌曲
}


/***************************************
函数功能:创建线程
参数类型:PLayer结构体
返回描述:
****************************************/
static void create_pthread(PLAYER *player)
{
	pthread_t pthread_send_cmd, pthread_rcv_msg;
	pthread_t pthread_show_lrc;
	
	pthread_create(&pthread_send_cmd,NULL,pthread_send_player_cmd,(void *)player);//发送命令
	pthread_create(&pthread_rcv_msg,NULL,pthread_rcv_player_msg,(void *)player);//接收信息
	pthread_create(&pthread_show_lrc,NULL,pthread_show_lrc_msg,(void *)player);//歌词显示

	pthread_detach(pthread_send_cmd);
	pthread_detach(pthread_rcv_msg);
	pthread_detach(pthread_show_lrc);
}


int main(int argc, char *argv[])
{
	static PLAYER player;/*定义整个Mplayer结构体*/
	memset(&player, 0, sizeof(player));

	thread_gtk_init();//gtk线程初始化
	gtk_init (&argc, &argv);
	player_init(&player);/*Mplayer初始化*/

	player_start(&player);/*启动Mplayer*/

	create_pthread(&player);/*线程创建初始化*/

	gtk_widget_show_all(player.win.win_back);
	gtk_widget_hide(player.win.volume_rate);//隐藏音量进度条
	gtk_main();
	return 0;
}





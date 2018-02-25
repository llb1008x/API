#include<gtk/gtk.h>
#include<glade/glade.h>
#include<stdio.h>
#include<stdlib.h>
#include"types.h"
#include"player_display.h"
#include"player_control.h"
#include"./sungtk_interface/sungtk_interface.h"
#include"./deal_song_list/deal_song_list.h"
#include"./encoding/gb2312_ucs2.h"

static gboolean callback_list_release(GtkWidget *widget, GdkEventButton *event, gpointer data);


static PLAYER *_player = NULL;

static int _list_pos_y = LIST_START_POS_Y;
static int _list_pos_x = LIST_START_POS_X;
static double _press_x = 0;
static double _press_y = 0;


enum list_press_state {NOTHING, AROUND, FLUCTUATE, SWITCH};
static enum list_press_state _list_flag = NOTHING;



void player_song_cur_show_from_text(PLAYER *player, const char *text)
{
	gtk_label_set_text(GTK_LABEL(player->win.song), text);
}

void player_song_cur_display(PLAYER *player)
{
	const char *text = sungtk_clist_get_row_data(player->win.win_list, player->status.song_cur_num);
	player_song_cur_show_from_text(player, text);
	sungtk_clist_select_row(player->win.win_list, player->status.song_cur_num, LIST_FONT_BRIGHT_COLOR);
}

void player_current_time_display(PLAYER *player, const int time)
{
	gchar show_time[32] = "";
	snprintf(show_time, sizeof(show_time), "%02d:%02d", 
		time/60, time%60);

	gdk_threads_enter();
	gtk_label_set_text(GTK_LABEL(player->win.time), show_time);
	gdk_threads_leave();
}

void player_percent_display(PLAYER *player, const int percent)
{
	double per = (double)percent/100;
	gdk_threads_enter();
	gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(player->win.rate), per);
	gdk_threads_leave();
}

void player_song_length_display(PLAYER *player, const int length)
{
	gchar show_time[32] = "";
	snprintf(show_time, sizeof(show_time), "%02d:%02d", 
		length/60, length%60);

	gdk_threads_enter();
	gtk_label_set_text(GTK_LABEL(player->win.length), show_time);
	gdk_threads_leave();
}

void player_artist_display(PLAYER *player, const char *artist)
{
	char show_buf[128] = "";
	gb2312_to_utf8((unsigned char *)artist, (unsigned char *)show_buf);	

	gdk_threads_enter();
	gtk_label_set_text(GTK_LABEL(player->win.singer), show_buf);
	gdk_threads_leave();
}



static void player_pause_state_display(PLAYER *player)
{
	char *pic[2] = {IMAGE_BUTTON_PLAY_PATH, IMAGE_BUTTON_PAUSE_PATH};
	sungtk_button_set_image(player->win.pause, pic[player->status.ispause], 
		BUTTON_PAUSE_WIDTH, BUTTON_PAUSE_HEIGTH);
}


void player_auto_play(PLAYER *player)
{
	send_cmd_to_player(player, CMD_FRONT, NULL);
	gdk_threads_enter();
	player_song_cur_display(player);
	player_pause_state_display(player);
	gdk_threads_leave();
}



void player_song_list_display(PLAYER *player)
{
	if(player->list.song_list != NULL)
		player->list.song_list = release_song_list(player->list.song_list, player->list.song_num);

	player->list.song_list = get_song_list(player->path.song_path, &player->list.song_num);

	int i;
	for(i=0;i<player->list.song_num;++i)
	{
		//printf("%s\n", player->list.song_list[i]);
		sungtk_clist_append(player->win.win_list, player->list.song_list[i]);
	}
}


static void callback_pause(GtkWidget *widget, gpointer data)
{
	PLAYER *player = (PLAYER *)data;
	send_cmd_to_player(player, CMD_PAUSE, NULL);
	player_pause_state_display(player);
}

static void callback_next(GtkWidget *widget, gpointer data)
{
	PLAYER *player = (PLAYER *)data;
	send_cmd_to_player(player, CMD_FRONT, NULL);
	player_song_cur_display(player);
	player_pause_state_display(player);
}

static void callback_back(GtkWidget *widget, gpointer data)
{
	PLAYER *player = (PLAYER *)data;
	send_cmd_to_player(player, CMD_BACK, NULL);
	player_song_cur_display(player);
	player_pause_state_display(player);
}

static gboolean callback_press_rate(GtkWidget *widget, GdkEventButton *event, gpointer data)
{
	PLAYER *player = (PLAYER *)data;
	double right_press_x = event->x;
	int seek_num = player->status.song_length*right_press_x/player->win.rate->allocation.width - player->status.time_position;
	//printf("seek_num====%d\n", seek_num);
	send_cmd_to_player(player, CMD_FORWORD, &seek_num);
	return TRUE;
}


static void list_comeback(PLAYER *player)
{
	int list_min_y = A8_SCREEN_HEIGHT - player->win.win_list->vbox->allocation.height;
	if(_list_pos_y > LIST_START_POS_Y || list_min_y >= 0){
		_list_pos_y = LIST_START_POS_Y;
		gtk_fixed_move(GTK_FIXED(player->win.win_list_fixed), player->win.win_list->fixed, LIST_START_POS_X, _list_pos_y);
	}else if(_list_pos_y < (0-list_min_y)){
		_list_pos_y = list_min_y;
		gtk_fixed_move(GTK_FIXED(player->win.win_list_fixed), player->win.win_list->fixed, LIST_START_POS_X, _list_pos_y);
	}
}


static gboolean callback_list_release(GtkWidget *widget, GdkEventButton *event, gpointer data)
{
	list_comeback(_player);
	//printf("list press x==%lf, y===%lf\n", event->x, event->y);
	if(_list_flag == NOTHING){
		int row = (int)data;
		//printf("row===============%d\n", row);
		//printf("presss x==%lf, y===%lf, row====%d\n", event->x, event->y, row);
		send_cmd_to_player(_player, CMD_CHANGESONG, &row);
		player_song_cur_display(_player);
		player_pause_state_display(_player);
	}
	
	_list_flag = NOTHING;
	return TRUE;
}


static gboolean callback_list_motion_release(GtkWidget *widget, GdkEventButton *event, gpointer data)
{
	PLAYER *player = (PLAYER *)data;

	list_comeback(player);

	printf("motion release x==%lf, y===%lf\n", event->x, event->y);
	_list_flag = NOTHING;
	return TRUE;
}

static gboolean callback_list_motion_press(GtkWidget *widget, GdkEventButton *event, gpointer data)
{
	_press_x = event->x;
	_press_y = event->y;
	_list_flag = NOTHING;
	printf("motion press x==%lf, y===%lf\n", event->x, event->y);
	return TRUE;
}


static gboolean callback_list_motion(GtkWidget *widget, GdkEventMotion *event, gpointer data)
{
	PLAYER *player = (PLAYER *)data;
	double new_y = event->y;
	double new_x = event->x;

	//printf("motion x==%lf, y===%lf\n", event->x, event->y);

	if((_list_flag == NOTHING)){
		
		if(abs(new_y - _press_y) >= player->win.win_list->row_height){
			_list_flag = FLUCTUATE;
		}else if(abs(new_x - _press_x) > 10){
			_list_flag = FLUCTUATE;
		}
	}
	if((_list_flag == FLUCTUATE) && (abs(new_y - _press_y) > 6)){
		_list_pos_y += (new_y > _press_y) ? 17 : -17;
		//printf("pos y===%d\n", _list_pos_y);
		_press_y = new_y;
		gtk_fixed_move(GTK_FIXED(player->win.win_list_fixed), player->win.win_list->fixed, _list_pos_x, _list_pos_y);
	}

	return TRUE;
}

//音量进度条按下，显示进度条
static gboolean callback_volume_press(GtkWidget *widget, GdkEventButton *event, gpointer data)
{
	PLAYER *player = (PLAYER *)data;
	printf("y====%lf\n", event->y);
	if(event->y > VOL_MUTE_POS_Y)
		gtk_widget_show_all(player->win.volume_rate_fixed);
	return TRUE;
}

//音量进度条抬起，隐藏进度条
static gboolean callback_volume_release(GtkWidget *widget, GdkEventButton *event, gpointer data)
{
	PLAYER *player = (PLAYER *)data;
	gtk_widget_hide_all(player->win.volume_rate_fixed);
	return TRUE;
}

//音量调节滑动回调函数
static gboolean callback_volume_motion(GtkWidget *widget, GdkEventMotion *event, gpointer data)
{
	PLAYER *player = (PLAYER *)data;
	if(VOL_MUTE_POS_Y-event->y > 0){
		double percent = (double)(VOL_MUTE_POS_Y-event->y)/VOL_RATE_HEIGHT;
		int volume = percent*100;
		gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(player->win.volume_rate), percent);
		send_cmd_to_player(player, CMD_VOLUME, &volume);
	}
	return TRUE;
}

//歌曲列表创建初始化
static void player_song_list_init(PLAYER *player)
{
	_player = player;
	player->win.win_list_eventbox = glade_xml_get_widget(player->win.gxml, EVENTBOX_LIST);
	player->win.win_list_fixed = glade_xml_get_widget(player->win.gxml, FIXED_LIST);

	player->win.win_list = sungtk_clist_new();
	sungtk_clist_set_row_height(player->win.win_list, LIST_ROW_HEIGTH);
	sungtk_clist_set_col_width(player->win.win_list, LIST_COL_WIDTH);
	sungtk_clist_set_text_size(player->win.win_list, LIST_FONT_SIZE);
	sungtk_clist_set_select_row_signal(player->win.win_list, "button-release-event", callback_list_release);
	gtk_fixed_put(GTK_FIXED(player->win.win_list_fixed), player->win.win_list->fixed, LIST_START_POS_X, LIST_START_POS_Y);

	g_signal_connect(player->win.win_list_eventbox, "motion-notify-event", G_CALLBACK(callback_list_motion), player);
	g_signal_connect(player->win.win_list_eventbox, "button-release-event", G_CALLBACK(callback_list_motion_release), player);
	g_signal_connect(player->win.win_list_eventbox, "button-press-event", G_CALLBACK(callback_list_motion_press), player);
}

//歌曲按钮控制初始化
static void player_button_init(PLAYER *player)
{
	player->win.back = glade_xml_get_widget(player->win.gxml, BUTTON_BACK);//上一首
	player->win.pause = glade_xml_get_widget(player->win.gxml, BUTTON_PAUSE);//暂停
	player->win.next = glade_xml_get_widget(player->win.gxml, BUTTON_NEXT);//下一首

	sungtk_button_inset_image(player->win.back, IMAGE_BUTTON_BACK_PATH, BUTTON_NEXT_WIDTH, BUTTON_NEXT_HEIGHT);
	sungtk_button_inset_image(player->win.pause, IMAGE_BUTTON_PLAY_PATH, BUTTON_PAUSE_WIDTH, BUTTON_PAUSE_HEIGTH);
	sungtk_button_inset_image(player->win.next, IMAGE_BUTTON_FRONT_PATH, BUTTON_NEXT_WIDTH, BUTTON_NEXT_HEIGHT);

	g_signal_connect(player->win.back, "pressed", G_CALLBACK(callback_back), player);
	g_signal_connect(player->win.pause, "pressed", G_CALLBACK(callback_pause), player);
	g_signal_connect(player->win.next, "pressed", G_CALLBACK(callback_next), player);
}


//歌曲信息显示label初始化
static void player_label_init(PLAYER *player)
{
	player->win.song = glade_xml_get_widget(player->win.gxml, LABEL_SONG);//歌名
	player->win.singer = glade_xml_get_widget(player->win.gxml, LABEL_SINGER);//歌唱家
	player->win.lrc_light = glade_xml_get_widget(player->win.gxml, LABEL_LRC);//歌词
	player->win.time = glade_xml_get_widget(player->win.gxml, LABEL_CUR_TIME);//当前播放时间
	player->win.length = glade_xml_get_widget(player->win.gxml, LABEL_LEN_TIME);//歌曲总长时间

	sungtk_widget_set_font_size(player->win.song, SONG_NAME_FONT_SIZE, FALSE);
	sungtk_widget_set_font_size(player->win.singer, SINGER_FONT_SIZE, FALSE);
	sungtk_widget_set_font_size(player->win.lrc_light, LRC_FONT_SIZE, FALSE);
	sungtk_widget_set_font_size(player->win.time, SONG_TIME_FONT_SIZE, FALSE);
	sungtk_widget_set_font_size(player->win.length, SONG_LENGTH_FONT_SIZE, FALSE);

	sungtk_widget_set_font_color(player->win.song, SONG_NAME_FONT_COLOR, FALSE);
	sungtk_widget_set_font_color(player->win.singer, SINGER_FONT_COLOR, FALSE);
	sungtk_widget_set_font_color(player->win.lrc_light, LRC_FONT_COLOR, FALSE);
	sungtk_widget_set_font_color(player->win.time, SONG_TIME_FONT_COLOR, FALSE);
	sungtk_widget_set_font_color(player->win.length, SONG_LENGTH_FONT_COLOR, FALSE);
}

//歌曲播放进度控件初始化
static void player_progressbar_init(PLAYER *player)
{
	GtkWidget *event_rate = glade_xml_get_widget(player->win.gxml, PROGRESS_EVENTBOX);
	player->win.rate = glade_xml_get_widget(player->win.gxml, PROGRESS_RATE);
	g_signal_connect(event_rate, "button-press-event", G_CALLBACK(callback_press_rate), player);
}

//音量调节控件初始化
static void player_volume_init(PLAYER *player)
{
	player->win.volume_event_box = glade_xml_get_widget(player->win.gxml, VOLUME_EVENTBOX);
	player->win.volume_fixed = glade_xml_get_widget(player->win.gxml, VOLUME_FIXED);
	player->win.mute = glade_xml_get_widget(player->win.gxml, VOLUME_IMAGE);
	sungtk_image_load_picture(player->win.mute, IMAGE_BUTTON_MUTE_PATH, VOL_MUTE_WIDTH, VOL_MUTE_HEIGHT);
	
	player->win.volume_rate_fixed = gtk_fixed_new();
	gtk_fixed_put(GTK_FIXED(player->win.volume_fixed), player->win.volume_rate_fixed, 34, 0);

	player->win.volume_rate = gtk_progress_bar_new();
	gtk_progress_bar_set_orientation(GTK_PROGRESS_BAR(player->win.volume_rate), GTK_PROGRESS_BOTTOM_TO_TOP);
	gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(player->win.volume_rate), 0.3);
	gtk_widget_set_size_request(player->win.volume_rate, VOL_RATE_WIDTH, VOL_RATE_HEIGHT);
	gtk_fixed_put(GTK_FIXED(player->win.volume_rate_fixed), player->win.volume_rate, 0, 0);	

	
	g_signal_connect(player->win.volume_event_box, "button-press-event", G_CALLBACK(callback_volume_press), player);
	g_signal_connect(player->win.volume_event_box, "button-release-event", G_CALLBACK(callback_volume_release), player);
	g_signal_connect(player->win.volume_event_box, "motion-notify-event", G_CALLBACK(callback_volume_motion), player);
}


//所有窗口、控件创建初始化
void player_window_init(PLAYER *player)
{
	 //打开glade
	player->win.gxml=glade_xml_new("../glade/mplayer.glade", NULL, NULL);
	player->win.win_back = glade_xml_get_widget(player->win.gxml, WIN_BACK);//获取glade定义的控件
	player->win.image_artist = glade_xml_get_widget(player->win.gxml, IMAGE_ARTIST);
	sungtk_background_set_picture(player->win.win_back, IMAGE_BACKGROUND_PATH, A8_SCREEN_WIDTH, A8_SCREEN_HEIGHT);
	sungtk_image_load_picture(player->win.image_artist, IMAGE_MUSIC_COMMON_PATH, 150, 150);

	player_button_init(player);//按钮初始化

	player_label_init(player);//label
	
	player_song_list_init(player);//歌曲列表

	player_progressbar_init(player);//歌曲进度

	player_volume_init(player);
}


/********gtk线程初始化*************/
void thread_gtk_init(void)
{
  	if( FALSE == g_thread_supported() ){  // 如果线程没有初始化
		g_thread_init(NULL); // 初始化线程，g_thread_init()必须放在gdk_threads_init()前面
	} 
	gdk_threads_init();
}


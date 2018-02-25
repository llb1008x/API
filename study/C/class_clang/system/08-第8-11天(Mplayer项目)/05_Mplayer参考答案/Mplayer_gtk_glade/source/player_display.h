#ifndef _PLAYER_DISPLAY_H_
#define _PLAYER_DISPLAY_H_

/****************************************/
#define GLADE_FILE "../glade/mplayer.glade"
#define WIN_BACK "window_back"
#define FIXED_BACK "fixed_back"
#define EVENTBOX_LIST "eventbox_list"
#define FIXED_LIST "fixed_list"
#define IMAGE_ARTIST "image_artist"

#define BUTTON_BACK "button_back"
#define BUTTON_PAUSE "button_pause"
#define BUTTON_NEXT "button_next"

#define LABEL_SONG "label_song"
#define LABEL_SINGER "label_singer"
#define LABEL_LRC "label_lrc"
#define LABEL_CUR_TIME "label_cur_time"
#define LABEL_LEN_TIME "label_len_time"

#define PROGRESS_EVENTBOX "eventbox_progress"
#define PROGRESS_RATE "progressbar_rate"

#define VOLUME_EVENTBOX "eventbox_volume"
#define VOLUME_FIXED "fixed_volume"
#define VOLUME_IMAGE "image_volume"


/*************************************/

//================
#define IMAGE_BACKGROUND_PATH "../picture/background/11.jpg"
#define IMAGE_MUSIC_COMMON_PATH "../picture/music.png"


#define IMAGE_BUTTON_PAUSE_PATH "../picture/button_style/play.png"
#define IMAGE_BUTTON_PLAY_PATH "../picture/button_style/pause.png"
#define IMAGE_BUTTON_BACK_PATH "../picture/button_style/back.png"
#define IMAGE_BUTTON_FRONT_PATH "../picture/button_style/front.png"

#define IMAGE_BUTTON_MUTE_PATH "../picture/mute.jpg"
#define VOL_MUTE_WIDTH 40
#define VOL_MUTE_HEIGHT 40
#define VOL_RATE_WIDTH 7
#define VOL_RATE_HEIGHT 85
#define VOL_MUTE_POS_Y 85



#define BUTTON_PAUSE_WIDTH 70
#define BUTTON_PAUSE_HEIGTH 70
#define BUTTON_NEXT_WIDTH 70
#define BUTTON_NEXT_HEIGHT 70


#define LIST_ROW_HEIGTH 40
#define LIST_COL_WIDTH 210
#define LIST_FONT_SIZE 13
#define LIST_START_POS_X 20
#define LIST_START_POS_Y 5


#define LRC_FONT_SIZE 15
#define LRC_FONT_COLOR "white"

#define SONG_NAME_FONT_SIZE 16
#define SONG_NAME_FONT_COLOR "white"

#define SINGER_FONT_SIZE 16
#define SINGER_FONT_COLOR "white"

#define SONG_TIME_FONT_SIZE 12
#define SONG_TIME_FONT_COLOR "black"

#define SONG_LENGTH_FONT_SIZE 12
#define SONG_LENGTH_FONT_COLOR "black"

#define LIST_FONT_BRIGHT_COLOR "white"


extern void player_song_cur_display(PLAYER *player);

extern void player_current_time_display(PLAYER *player, const int time);

extern void player_percent_display(PLAYER *player, const int percent);

extern void player_song_length_display(PLAYER *player, const int length);

extern void player_artist_display(PLAYER *player, const char *artist);

extern void player_auto_play(PLAYER *player);


extern void player_song_list_display(PLAYER *player);


extern void player_window_init(PLAYER *player);

extern void thread_gtk_init(void);

#endif


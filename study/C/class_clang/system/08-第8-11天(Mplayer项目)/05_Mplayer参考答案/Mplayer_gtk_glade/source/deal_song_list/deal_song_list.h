#ifndef _DEAL_SONG_LIST_H_
#define _DEAL_SONG_LIST_H_

extern char **get_song_list(const char *song_path, int *song_num);
extern char **release_song_list(char **song_list, int song_num);


#endif

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
//启动mplayer播放器 
//参数song_path 为歌曲的路径
void mplayer_play(char * song_path)
{
	pid_t pid;
	pid=fork();
	if(pid<0)
	{
		perror("fork");
	}
	else if(pid==0)
	{
		close(1);
		close(2);
		execlp("mplayer","mplayer","-slave","-quiet",song_path,NULL);
		exit(0);
	}
	else
		;
}
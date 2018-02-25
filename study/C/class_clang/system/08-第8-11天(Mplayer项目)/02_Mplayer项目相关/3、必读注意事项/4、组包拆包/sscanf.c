
#include <stdio.h>

typedef struct _mp
{
	char artisit[100];
	int song_length;
}SONG;

int main(int argc,char *argv[])
{
	SONG song_detail;
	
	char buf1[]="ANS_META_ARTIST='JAY'";
	char buf2[]="ANS_LENGTH=256";
	
	if(strstr(buf1,"ANS_META_ARTIST"))
	{
		sscanf(buf1,"ANS_META_ARTIST='%[^']",song_detail.artisit);
	}
	
	if(strstr(buf2,"ANS_LENGTH"))
	{
		sscanf(buf2,"ANS_LENGTH=%d",&song_detail.song_length);
	}
	printf("artisit is %s\n",song_detail.artisit);	
	printf("song_length is %d\n",song_detail.song_length);
}

#include <stdio.h>
#include <string.h>
//将一个整数数据int value=12345 变成字符串
//c+'0'
/*
	put_picture(char *path,int x,int y)
	player_music(char *path);
	
void btow(){
	char path[20],i;
	player_music("song.wav");//开线程播放
	for(i=0;i<10;i++){
		sprintf(path,"btow-%d",i);
		put_picture(path,100,100);
		delay();
	}
	put_picture("btow-1.jpg",100,100);
	delay();
	put_picture("btow-2.jpg",100,100);
	delay();
	put_picture("btow-3.jpg",100,100);
	delay();
	put_picture("btow-4.jpg",100,100);
	delay();
	put_picture("btow-5.jpg",100,100);
	......
	put_picture("btow-10.jpg",100,100);
}
*/
int main()
{
	int value  = 12345,i;
	char str[10];
#if 0	
	str[0] = value/10000%10+'0';
	str[1] = value/1000%10+'0';
	str[2] = value/100%10+'0';
	str[3] = value/10%10+'0';
	str[4] = value/1%10+'0';
	str[5] = '\0';
#else	
	sprintf(str,"%d",value);
#endif	
	printf("%s\n",str);
	
	return 0;
}

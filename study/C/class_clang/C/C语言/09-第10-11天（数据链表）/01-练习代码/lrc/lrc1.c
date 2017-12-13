#include <stdio.h>
#include <string.h>
int main()
{
	char *str="[03:39.68][02:39.34][01:10.71]爱能不能够永远单纯没有悲哀";
	int time_buf[5];
	int i=0,j;
	int min,sec;
	
	i=0;
	while(*str=='[')
	{
		
		
		str=str+10;
	}
	for(j=0;j<i;j++)
	{
		//打印所有的时间
	}
	printf("str=%s\n",str);//打印歌词
	return 0;
}
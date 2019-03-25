/* ************************************************************************
 *       Filename:  time_delay.c
 *    Description:  
 *        Version:  1.0
 *        Created:  2014年02月11日 09时30分23秒
 *       Revision:  none
 *       Compiler:  gcc
 *         Author:  YOUR NAME (), 
 *        Company:  
 * ************************************************************************/

#include <stdio.h>
#include <time.h>
void time_delay(int sec)
{
	int s_time,e_time;
	s_time=time(NULL);
	while(1)
	{
		e_time=time(NULL);
		if(e_time==s_time+sec)
			break;
	}
}
int main(int argc, char *argv[])
{
	while(1)
	{
		printf("hello world\n");
		time_delay(3);
	}
	return 0;
}





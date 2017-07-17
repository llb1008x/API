#include <stdio.h>

struct time
{
	char mic;
	char sec;
};
int main(void)
{
	struct time timer = {0,0};
	
	while(1){
		usleep(1000*1000);
		if(++timer.sec == 60){
			timer.sec = 0;
			if(++timer.mic == 60)
				timer.mic = 0;
		}
		printf("%02d:%02d\r",timer.mic,timer.sec);
		fflush(NULL);
	}
	return 0;
}














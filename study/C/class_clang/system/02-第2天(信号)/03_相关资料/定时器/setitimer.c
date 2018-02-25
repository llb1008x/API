#include<stdio.h>
#include<unistd.h>
#include<signal.h>
#include<sys/time.h>

void print_msg(int signum)
{
	if(signum==SIGALRM)
		printf("alarm\n");
}

int main(int argc, char *argv[])
{
	signal(SIGALRM, print_msg);	//自定义信号处理函数
	
	struct itimerval tick;
	
	tick.it_value.tv_sec = 3;//本次定时设置值
	tick.it_value.tv_usec = 0;
	tick.it_interval.tv_sec = 1;//下次定时取值
	tick.it_interval.tv_usec = 0;
	
	int ret = setitimer(ITIMER_REAL, &tick, NULL);//ITIMER_REAL表示定时到了，产生SIGALRM信号
	if(ret != 0)
	{
		perror("set timer error");
		return -1;
	}

	while(1);
	return 0;
}
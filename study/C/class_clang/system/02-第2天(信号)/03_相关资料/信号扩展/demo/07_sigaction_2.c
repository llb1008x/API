#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>

void signal_handler(int signum)
{
	if(signum == SIGINT)
	{
		int ret;
		printf("I got a signal :SIGINT \n");
		ret = sleep(8);
		/*若此时接收到信号则返回剩余秒数,否则返回0*/
		printf("ret = %d\n", ret);
	}
	else if(signum == SIGRTMIN)
	{
		printf("I got a signal :SIGRTMIN\n");
	}
}

int main(int argc, char *argv[])
{
	struct sigaction act;
	pid_t pid;

	pid = getpid();
	printf("pid = %d\n", pid);
	act.sa_handler = signal_handler;
	sigemptyset(&act.sa_mask);
	act.sa_flags = SA_SIGINFO;
	// sigaddset(&act.sa_mask, SIGINT);
	// sigaddset(&act.sa_mask, SIGRTMIN);
	sigaction(SIGINT, &act, NULL);
	sigaction(SIGRTMIN, &act, NULL);
	while(1);
	return 0;
}


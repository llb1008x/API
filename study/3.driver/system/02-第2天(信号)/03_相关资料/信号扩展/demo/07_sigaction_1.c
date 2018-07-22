#include <stdio.h>
#include <signal.h>

void signal_handler(int signum)
{
	if(signum == SIGINT)
	{
		printf("I got a signal :SIGINT\n");
	}
	else if(signum == SIGQUIT)
	{
		printf("I got a signal :SIGQUIT\n");
	}
}

int main(int argc, char *argv[])
{
	struct sigaction act;
	
	act.sa_handler = signal_handler;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	sigaction(SIGINT, &act, NULL);
	sigaction(SIGQUIT, &act, NULL);
	while(1);
	return 0;
}


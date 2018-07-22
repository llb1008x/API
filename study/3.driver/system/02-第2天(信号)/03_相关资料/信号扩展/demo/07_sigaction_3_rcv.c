#include <signal.h>
#include <stdio.h>

void signal_handler(int signum, siginfo_t *info, void *ptr)
{
	printf("signum = %d\n", signum);
	printf("info->si_pid = %d\n", info->si_pid);
	printf("info->si_sigval = %d\n", info->si_value.sival_int);
}

int main(int argc, char *argv[])
{
	struct sigaction act, oact;
	
	act.sa_sigaction = signal_handler;
	sigemptyset(&act.sa_mask);
	act.sa_flags = SA_SIGINFO;
	sigaction(SIGINT, &act, &oact);
	while(1)
	{
		printf("pid is %d\n", getpid());
		pause();
	}
	return 0;
}


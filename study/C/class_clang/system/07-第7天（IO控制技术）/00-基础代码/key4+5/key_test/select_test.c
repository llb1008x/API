#include <sys/select.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

void select_demo(int sec, int usec)
{
	fd_set rfds;
	struct timeval tv;
	int ret;
	int key_fd;

	key_fd = open("/dev/key4+5", O_RDWR);
	if(key_fd <0)
	{
		perror("open failed\n");
		return;
	}
	while(1)
	{
		FD_ZERO(&rfds);
		FD_SET(key_fd, &rfds);
		
		/* Wait up to five seconds. */
		tv.tv_sec = sec;
		tv.tv_usec = usec;
		
		printf("Wait %d sec %d usec\n", sec, usec);
		ret = select(FD_SETSIZE, &rfds, NULL, NULL, &tv);
		/* Don't rely on the value of tv now! It may be changed by select! */

		if(ret == -1)
			perror("select()");
		else if(ret)
		{
			char key = 0;
			
			if(FD_ISSET(key_fd, &rfds))
			{
				read(key_fd, &key, sizeof(key));
				if(key != 0)
					printf("read from key4+5 key = %#x\n", key);
			}
			/* Do something */
			/* ... */
		}
		else/* Overtime return 0 */
		{
			printf("Overtime !!!\n");
		}
	}
}

int main(int argc, char **argv)
{
	select_demo(2, 0);
	return 0;
}

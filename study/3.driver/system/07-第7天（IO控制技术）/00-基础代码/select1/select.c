#include <sys/select.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

void select_demo(int nseconds)
{
	fd_set rfds;
	struct timeval tv;
	int ret;
	int i;
	int key_fd;

	key_fd = open("/dev/key4+5", O_RDWR);
	if(key_fd <0)
	{
		perror("open failed\n");
		return;
	}

	for(i=nseconds; i>0; i--)
	{
		FD_ZERO(&rfds);
		FD_SET(0, &rfds);
		FD_SET(key_fd, &rfds);

		/* Wait up to five seconds. */
		tv.tv_sec = 1;
		tv.tv_usec = 0;

		ret = select(FD_SETSIZE, &rfds, NULL, NULL, &tv);
		/* Don't rely on the value of tv now! It may be changed by select! */

		if(ret == -1)
			perror("select()");
		else if(ret)
		{
			char key = 0;
			
			if(FD_ISSET(0, &rfds))
			{
				read(0, &key, sizeof(key));
				printf("read from keybord key = %#x\n", key);
			}
			else if(FD_ISSET(key_fd, &rfds))
			{
				read(key_fd, &key, sizeof(key));
				if(key != 0)
					printf("read from key4+5 key = %#x\n", key);
			}
			/* Do something */
			/* ... */
			return;
		}
		else/* Overtime return 0 */
		{
			printf("%d seconds remined\n", i-1);
		}
	}
}

int main(int argc, char **argv)
{
	select_demo(8);
	return 0;
}

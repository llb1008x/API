#include "stdio.h"
#define XMK_STR(x)	#x
#define MK_STR(x)	XMK_STR(x)

unsigned char dt[] = {
	"bootargs="	"sjkfds/skdjfklsd/skdfk"	"\0"
	"bootcmd="	"234/343/3434"		"\0"
	"bootdelay="	MK_STR(3)	"\0"
	"\0"
};

unsigned char env_get_char(int index)
{
	return dt[index];
}

int printenv(char *name, int state)
{
	int i, j;
	char c, buf[17];

	i = 0;
	buf[16] = '\0';
	while (state && env_get_char(i) != '\0') {
		j = 0;
		do {
			buf[j++] = c = env_get_char(i++);
			if (j == sizeof(buf) - 1) {
				if (state <= 1)
					printf("%s",buf);
				j = 0;
			}
		} while (c != '\0');

		if (state <= 1) {
			if (j)
			printf("%s",buf);
			printf("\n");
		}
	}
	return i;
}

int main(void)
{
	printenv(NULL,1);
	
	return 0;
}

#include <stdio.h>
int main(void)
{
	FILE *fp;
	char ch;
	fp = fopen("DUMMY.FIL", "w");

	ch=getc(fp);
	if (ferror(fp))
	{
		printf("Error reading from DUMMY.FIL\n");
		clearerr(fp);
	}
	fclose(fp);
	return 0;
}

#include <stdio.h>
int main(void)
{
	FILE *fp;
	char ch;
	fp=fopen("test.txt","r+");
	if(fp==NULL)
	{
		printf("Cannot open the file\n");
		return 0;
	}
	while(  (ch = fgetc(fp))!=EOF  )
	{
		printf("%c",ch);
	}
	fclose(fp);
	return 0;
}

/*
	while(1)
	{
		ch = fgetc(fp)
		if(feof(fp)!=0)
			printf("%c",ch);
	}
*/
#include <stdio.h>
int main(void)
{
	FILE *fp_read,*fp_write;
	char string1[100];
	if((fp_read=fopen("src.txt","r+"))==NULL)
	{
		printf("Cannot open the file\n");
		return 0;
	}
	if((fp_write=fopen("dest.txt","w+"))==NULL)
	{
		printf("Cannot open the file\n");
		return 0;
	}
	fgets(string1, 10, fp_read);
	printf("%s\n",string1);
	fputs(string1,fp_write);
	fclose(fp_read);
	fclose(fp_write);
	return 0;
}
/*
fgets(string,100,stdin);
fputs(string,stdout);
*/



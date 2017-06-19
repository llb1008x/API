#include <stdio.h>
#include <string.h>

int main(int argc,char *argv[])
{
	FILE *fp,*fp_w;
	unsigned char str[200],ch;
	int len;

	if(argc != 3)
		return 0;
	fp = fopen(argv[1],"a+");
	if(fp == NULL){
		perror("有误");
		return 0;
	}
	fp_w = fopen(argv[2],"w+");
	if(fp_w == NULL){
		perror("有误");
		return 0;
	}
	while(feof(fp) == 0){
		len = fread(str,8,25,fp);
		printf("read:len = %d\n",len);
		len = fwrite(str,8,len,fp_w);
		printf("write:len = %d\n",len);	
	}
	fclose(fp);
	fclose(fp_w);
	return 0;
}





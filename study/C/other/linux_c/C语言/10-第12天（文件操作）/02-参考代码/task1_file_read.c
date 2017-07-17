#include <stdio.h>
#include <stdlib.h>
int main()
{
	unsigned long int file_size=0;
	char *src_file_buf=NULL;

	char src_file_name[]="简单爱.lrc";
	FILE *fp;

	fp=fopen(src_file_name,"rb");
	if(fp==NULL)
	{
		printf("open %s faild\n",src_file_name);
		return -1;
	}

	fseek(fp,0,SEEK_END);
	file_size=ftell(fp);
	rewind(fp);
	src_file_buf=(char *)malloc(file_size+1);
	fread(src_file_buf,file_size,1,fp);

	//*(src_file_buf+file_size)='\0';
	
	src_file_buf[file_size] = '\0';

	printf("%s\n",src_file_buf);
	
	fclose(fp);// 关闭文件
	free(src_file_buf);
	return 0 ;
}
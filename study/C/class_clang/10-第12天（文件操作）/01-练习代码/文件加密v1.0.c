#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define FILE_NAME_LEN	30

void get_file_name(char *dest_file_name,char *src_file_name)
{
	printf("请输入源文件名称[30字符以内]:\n");
	gets(src_file_name);
	printf("请输入目标文件名称[30字符以内]:\n");
	gets(dest_file_name);
}
char *read_src_file(unsigned long int *file_len,char *src_file_name)
{
	char *src;
	FILE *fp;
	
	fp = fopen(src_file_name,"rb");
	if(fp == NULL){
		perror("fopen");
		return NULL;
	}
	fseek(fp,0,SEEK_END);
	*file_len = ftell(fp);
	src = malloc(*file_len);
	rewind(fp);
	if(fread(src,*file_len,1,fp) == 0){
		fclose(fp);
		return NULL;
	}
	fclose(fp);
	return src;
}
char *file_text_encrypt(char *src_file_text,unsigned long int lengh,unsigned char password)
{
	char *ps = src_file_text;
	char *dst = src_file_text;
	unsigned long int i;
	
	for(i=0;i<lengh;i++){
		*src_file_text++ = (*dst++)+password;
	}
	return ps;
}
char *file_text_decrypt(char *src_file_text,unsigned long int lengh,unsigned char password)
{
	char *ps = src_file_text;
	char *dst = src_file_text;
	unsigned long int i;
	
	for(i=0;i<lengh;i++){
		*src_file_text++ = (*dst++)-password;
	}
	return ps;
}
void save_file(char *text,unsigned long int len,char *name)
{
	FILE *fp;
	
	fp = fopen(name,"wb");
	if(fp == NULL)
		perror("fopen");
	if(fwrite(text,len,1,fp) == 0){
		fclose(fp);
		return;
	}
	fclose(fp);
}
int main()
{
	char src_file_name[FILE_NAME_LEN];
	char dest_file_name[FILE_NAME_LEN];
	unsigned long int file_len,i;
	char *src,select;
	unsigned char password;
	
	get_file_name(dest_file_name,src_file_name);
	src = read_src_file(&file_len,src_file_name);
	// for(i=0;i<file_len;i++)
		// printf("%c",*src++);
	printf("请输入密码:\n");
	scanf("%d",&password);
	scanf("%d",&select);
	if(select == 1)
		src = file_text_encrypt(src,file_len,password);
	else if(select == 2)
		src = file_text_decrypt(src,file_len,password);
	save_file(src,file_len,dest_file_name);
	return 0;
}
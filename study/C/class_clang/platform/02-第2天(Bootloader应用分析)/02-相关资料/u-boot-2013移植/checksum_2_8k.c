#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define EXTERN_LEN  8192
int main(int argc,char **argv)
{
     unsigned int file_len = 0;
	 int ch,i=0;
	 unsigned int checksum = 0;
     FILE *infp = NULL;
     FILE *outfp = NULL;
     if(argc <= 0){
          printf("please input the file you want to ext\n");
          return -1;
     }
     infp = fopen(argv[1],"rb");
     if(NULL == infp){
          printf("open file %s error \n",argv[1]);
     }
     outfp = fopen(argv[2],"wb");
	 
	while((ch=fgetc(infp))!=EOF)
	{
		fputc(ch,outfp);
		i++;
		if(i >= 0x10)
			checksum += ch;		
	} 
	while(i != EXTERN_LEN)
	{
		fputc(0,outfp);
		i++;
	}
	 
	fseek(outfp,8,SEEK_SET);
	fwrite(&checksum,1,4,outfp);
	
    fclose(infp);
    fclose(outfp);
	 
    return 0;
}

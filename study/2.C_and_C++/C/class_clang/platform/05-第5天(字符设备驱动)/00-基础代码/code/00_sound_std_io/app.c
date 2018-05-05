#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/soundcard.h>

static unsigned char buffer[4096];

int main(int argc, char **argv)
{	
	int audio_fd;
	FILE *file_fd = NULL;
	int bits = 0;			/*音频量化位数*/
	int channels = 0;		/*单声道1,双声道2*/
	long sample = 0;		/*播放时使用的采样频率*/
	
	if(argc != 2)
		exit(-1); 	
	audio_fd = open("/dev/dsp", O_WRONLY);
	if(audio_fd<0){  		
		perror("");
		exit(-1); 	
	}	
	file_fd = fopen(argv[1], "rb");
	if(file_fd == NULL){
		perror("\n");
		exit(-1);
	}

	fseek(file_fd,22,SEEK_SET);
	fread( &channels, 1,2,file_fd);
	fread( &sample,1, 4,file_fd);
	fseek(file_fd,34,SEEK_SET);
	fread( &bits, 1,2,file_fd);
	printf("bcs:%d,%d,%d\n",bits,channels,sample);

	ioctl(audio_fd, SNDCTL_DSP_SETFMT, &bits);
	ioctl(audio_fd, SNDCTL_DSP_CHANNELS, &channels);
	ioctl(audio_fd, SNDCTL_DSP_SPEED, &sample);

	fseek(file_fd,44,SEEK_SET);
	while(fread( buffer, 1,4096,file_fd)){
		write(audio_fd,buffer, 4096);
	}
	fclose(file_fd);
	close(audio_fd);
	return 0;
}


#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/soundcard.h>

static unsigned char buffer[4096];

int main(int argc, char **argv)
{	
	int audio_fd;
	int file_fd;
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
	file_fd = open(argv[1], O_RDONLY);
	if(file_fd < 0){
		perror("\n");
		exit(-1);
	}

	lseek(file_fd,22,SEEK_SET);
	read( file_fd,&channels, 2);
	read( file_fd,&sample, 4);
	lseek(file_fd,34,SEEK_SET);
	read( file_fd,&bits, 2);
	printf("bcs:%d,%d,%d\n",bits,channels,sample);

	ioctl(audio_fd, SNDCTL_DSP_SETFMT, &bits);
	ioctl(audio_fd, SNDCTL_DSP_CHANNELS, &channels);
	ioctl(audio_fd, SNDCTL_DSP_SPEED, &sample);

	lseek(file_fd,44,SEEK_SET);
	while(read( file_fd,buffer, 4096)){
		write(audio_fd,buffer, 4096);
	}
	close(file_fd);
	close(audio_fd);
	return 0;
}


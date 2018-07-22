
/***************************************************************
  *Project Name:			openCV_test.c  
  *File Name:				lcd_user.c  
  *Description:				
  *Date and Edition:		2012-11-15  v1.1
  *Author:					sunplusedu
  **************************************************************/
  
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <string.h>
#include "lcd_user.h"

#include "highgui.h"
#include "cv.h"  
#include "highgui.h"  
  
#define FB_NAME		"/dev/fb0"

#define DEBUG_MSG

#ifdef 	DEBUG_MSG
#define Dprintf(fmt,arg...)			printf(fmt, ##arg)
#else
#define Dprintf(fmt,arg...)			NULL
#endif

int show_image(pixel_data_t *aryaddr,IplImage *pSrcImage,screen_handler_t *window)
{
	int x,y,i,offset,width;
	unsigned char *img_data = (unsigned char *)pSrcImage->imageData;
	
	Dprintf("depth:%d\n",pSrcImage->depth);
	Dprintf("width:%d\n",pSrcImage->width);
	Dprintf("height:%d\n",pSrcImage->height);
	Dprintf("origin:%d\n\n",pSrcImage->origin);
	
	if((pSrcImage->width > 800) || (pSrcImage->height > 480))
	{
		Dprintf("%s\n","too big");
		return;
	}	
	offset= pSrcImage->widthStep%3;//�����������ص�������
	width = pSrcImage->widthStep/3;
	//pSrcImage->width:����һ�е����ص����
	//pSrcImage->widthStep:����һ�е��ֽ��������ܲ���һ�����ص�������
	for(y=0; y<pSrcImage->height; y++)
	{
		for(x=0; x<width; x++)
		{
			i = y*window->width+x;
			aryaddr[i].blue = img_data[0];
			aryaddr[i].green = img_data[1];
			aryaddr[i].red = img_data[2];
			//aryaddr[i].alpha = 255;
			img_data +=3;
		}
		img_data += offset;//�����������ص�������
	}
}

IplImage * zoom_show(IplImage *pSrcImage)
{
	IplImage *pDstImage = NULL;
	double fScale = 0.96;      //���ű���  
    CvSize czSize;              //Ŀ��ͼ��ߴ�  
  
  //����Ŀ��ͼ���С  
    czSize.width = pSrcImage->width * fScale;  
    czSize.height = pSrcImage->height * fScale;  

    //����ͼ������  
    pDstImage = cvCreateImage(czSize, pSrcImage->depth, pSrcImage->nChannels);  
    cvResize(pSrcImage, pDstImage, CV_INTER_AREA);  
	
	return pDstImage;
}

void save_image(IplImage *pSrcImage)
{
	const char *pstrSaveImageName = "result.jpg";
	IplImage *pDstImage = NULL;
	double fScale = 0.5;      //���ű���  
    CvSize czSize;              //Ŀ��ͼ��ߴ�  
  
	//����Ŀ��ͼ���С  
    czSize.width = pSrcImage->width * fScale;  
    czSize.height = pSrcImage->height * fScale;  
	Dprintf("zoom width:%d\n",czSize.width);
	Dprintf("zoom height:%d\n\n",czSize.height);
 
	//����ͼ������  
    pDstImage = cvCreateImage(czSize, pSrcImage->depth, pSrcImage->nChannels);  
    cvResize(pSrcImage, pDstImage, CV_INTER_AREA);  
	
	//����ͼƬ  
    cvSaveImage(pstrSaveImageName, pDstImage,0);      
    cvReleaseImage(&pDstImage); 
}

int main(int argc, char **argv)
{
	int i;
	int ret;
	int fb_fd;
	int cont = 20;
	screen_handler_t window;
	struct fb_var_screeninfo vinfo;				//�洢��Ļ�ɱ����
	struct fb_fix_screeninfo finfo;				//�洢��Ļ�̶�����
	pixel_data_t *aryaddr;
	char *pstrImageName = "src.jpg";
	
	if(argc > 1)
		pstrImageName = argv[1];
		
    //���ļ��ж�ȡͼ��    
    IplImage *pSrcImage = cvLoadImage(pstrImageName, CV_LOAD_IMAGE_UNCHANGED);  
		
	//��framebuffer�豸
	fb_fd = open(FB_NAME, O_RDWR);
	if(fb_fd < 0){
		perror("fail to open\n");
		return -1;
	}
	
	/*��ȡlcd�豸��ز���*/
	ret = ioctl(fb_fd, FBIOGET_FSCREENINFO, &finfo);
	if (ret != 0){
		perror("fail to get screen info");
		return -1; 
	}
	ret = ioctl(fb_fd, FBIOGET_VSCREENINFO, &vinfo);
	if (ret != 0){
		perror("fail to get screen info");
		return -1; 
	}
	
	window.width = vinfo.xres;
	window.height = vinfo.yres;
	window.bits = vinfo.bits_per_pixel;
	
	window.pixels = window.width*window.height;
	window.size = window.pixels*(window.bits>>3);
	window.line_bytes = window.width*(window.bits>>3);
	
	//debug message
	Dprintf("****************\n\
			screen width: %d\n\
			screen height: %d\n\
			bits_per_pixel: %d\n\
			show size: %d\n\
			****************\n",window.width,window.height,window.bits,window.size);
	
	//ӳ���ڴ棬�û������ֱ�ӷ����豸�ڴ�
	window.addr = (unsigned char *)mmap(0, window.size, PROT_READ|PROT_WRITE, MAP_SHARED, fb_fd, 0);
	if(window.addr == NULL){
		perror("mmap");
		return -1;
	}
	aryaddr = (pixel_data_t*)window.addr;
		
	save_image(pSrcImage);//�������ź��ͼƬ
	while(cont--)
	{
		pSrcImage = zoom_show(pSrcImage);
		show_image(aryaddr,pSrcImage,&window);
		sleep(1);
		memset(window.addr, 0, window.size);//��ʾ��ɫ
	}
	
	munmap(window.addr,window.size);
	cvReleaseImage(&pSrcImage);  
	close(fb_fd);
	return 0;
}

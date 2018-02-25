#include <stdio.h> /* FILE */
#include <fcntl.h>/* O_RDWR */
#include <linux/fb.h>
#include <sys/mman.h>/* PROT_READ,PROT_WRITE,MAP_SHARED */
#include <string.h>/* memset() */
#include <asm/ioctl.h>	/* _IO */
#include <linux/input.h>	/* input_event */
#include "driverapp.h"
#include <stdlib.h>	/* malloc */

#define FB_NAME		"/dev/fb0"
#define LCD_COL			800
#define LCD_ROW			480	

static inline void fb_getpixel(int x,int y,int *r, int *g,int *b,unsigned char *fb_addr)
{
	register unsigned int c, *ptr;
	register unsigned int *pixel = (unsigned int *)fb_addr;

	if (x >= LCD_COL || x < 0 || y >= LCD_ROW || y < 0) {
		*r = *g = *b = 0;
		return;
	}
		
	pixel = (pixel +  y*LCD_COL + x);
	c = *pixel;
	*r = (c & (0xFF << 16)) >> 16;
	*g = (c & (0xFF << 8)) >> 8;
	*b = (c & (0xFF << 0));
}

static inline void fb_putpixel(int x, int y, int r, int g, int b, unsigned char *fb_addr,unsigned char alpha)
{

	register unsigned int c = 0;
	register unsigned int *pixel = (unsigned int *)fb_addr;
	
	if (x >= LCD_COL || x < 0 || y >= LCD_ROW || y < 0) 
		return;
		
	pixel = (pixel +  y*LCD_COL + x);
	c = (r << 16 | g << 8 | b << 0)|(alpha<<24);
	*pixel = c;
}

static inline void fb_blend(int alpha, 
							int r1, int g1, int b1,
							int r2, int g2, int b2,
							int *r, int *g, int *b)
{
	int alpha2;

	if (alpha == 127) {
		*r = (r1 + r2) >> 1;
		*g = (g1 + g2) >> 1;
		*b = (b1 + b2) >> 1;
	} else if (alpha == 255) {
		*r = r2;
		*g = g2;
		*b = b2;
	} else if (!alpha) {
		*r = r1;
		*g = g1;
		*b = b1;
	} else {
		alpha2 = 255 - alpha;
		*r = (((r1 * alpha2) + (r2 * alpha)) >> 8);
		*g = (((g1 * alpha2) + (g2 * alpha)) >> 8);
		*b = (((b1 * alpha2) + (b2 * alpha)) >> 8);
	}	      
}
static inline void fb_putpixel_alpha(int alpha, int x, int y,
									 int r1, int g1, int b1,
									 unsigned char *fb_addr)
{
	int r2, g2, b2, r, g, b;

	fb_getpixel(x, y, &r2, &g2, &b2, fb_addr);
	fb_blend(alpha, r1, g1, b1, r2, g2, b2, &r, &g, &b);
	fb_putpixel(x, y, r, g, b, fb_addr,0xff);
}

static void lcd_draw_bmp(unsigned char *fb_addr,unsigned char *p,int w,int h)
{
	int y, x;
	unsigned int b, g, r;
	unsigned int color;
	unsigned char alpha = 100;/* 0是全透 */
	
	if (w > LCD_COL || h > LCD_ROW) 
		return;

	for (y = 0; y < h; y++)
		for (x = 0; x < w; x++)
		{
			g = *p++;
			r = *p++;
			b = *p++;

			//fb_putpixel(x,h-1-y,r,g,b,fb_addr,alpha);
			fb_putpixel_alpha(alpha,x,h-1-y,r,g,b,fb_addr);
		}

}

static unsigned char *open_path_bmp(unsigned char *path,int *bmp_w,int *bmp_h)
{
	unsigned char *buffer = NULL;
	int fd;

	fd = open(path,O_RDWR);
	if(fd < 0){
		perror("open_path_bmp:");
		return NULL;
	}
	lseek(fd,18,SEEK_SET);
	read(fd,bmp_w,4);
	lseek(fd,22,SEEK_SET);
	read(fd,bmp_h,4);
	printf("%s:w=%d,h=%d\n",path,*bmp_w,*bmp_h);

	buffer = malloc((*bmp_w)*(*bmp_h)*3);
	if(!buffer)
		return NULL;
	lseek(fd,52,SEEK_SET);
	read(fd,buffer,(*bmp_w)*(*bmp_h)*3);
	close(fd);
	return buffer;
	
}

int main(unsigned char argc ,unsigned char *argv[])
{
	int fb_fd;
	int ret;
	struct fb_var_screeninfo vinfo;
	unsigned int screen_size;
	unsigned char *fb_addr = NULL;
	unsigned char *bmp_buffer;
	unsigned char *path_name = "test2.bmp";
	int bmp_w,bmp_h;
	
	fb_fd = open(FB_NAME,O_RDWR);
	if(fb_fd<0){
		perror("open fb0:");
		return fb_fd;
	}
	ret = ioctl(fb_fd, FBIOGET_VSCREENINFO, &vinfo);
	if(ret){
		perror("fb_fd ioctl:");
		return ret;
	}
	screen_size = vinfo.xres*vinfo.yres*(vinfo.bits_per_pixel>>3);
	printf("w=%d,h=%d,bits=%d,size=%d\n",vinfo.xres,vinfo.yres,vinfo.bits_per_pixel,screen_size);

	fb_addr = mmap(0,screen_size, PROT_READ|PROT_WRITE, MAP_SHARED, fb_fd, 0);
	if(fb_addr == NULL){
		perror("mmap:");
		return -1;
	}
#if 0	
	memset(fb_addr, 0, screen_size);/* show black 为什么没有显示黑色? */
	sleep(1);
	memset(fb_addr, 255, screen_size);/* show white */
	sleep(1);
#endif
	if(argc == 2)
		path_name = argv[1];
	bmp_buffer = open_path_bmp(path_name,&bmp_w,&bmp_h);
	if(!bmp_buffer)
		return -1;
	lcd_draw_bmp(fb_addr,bmp_buffer,bmp_w,bmp_h);

	free(bmp_buffer);
	munmap(fb_addr,screen_size);
	close(fb_fd);
	return 0;
}


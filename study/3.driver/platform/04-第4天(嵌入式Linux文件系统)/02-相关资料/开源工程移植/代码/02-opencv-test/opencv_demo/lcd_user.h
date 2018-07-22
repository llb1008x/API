
#ifndef		__LCD_USER_H
#define 	__LCD_USER_H

typedef struct pixel_data{
	unsigned char blue;
	unsigned char green;
	unsigned char red;
	unsigned char alpha;
}pixel_data_t;

typedef struct screen_handler{
	unsigned char *addr;		// framebuffer的地址
	unsigned int width;			// 屏幕的宽度
	unsigned int height;		// 屏幕的高度
	unsigned int pixels;		// 屏幕的像素 pixels = width*height
	unsigned int bits;			// 每个像素的位数
	unsigned int line_bytes;		// 每行像素的位数 line_bytes = width*bits/8
	unsigned long size;			// framebuffer的大小 size = pixels*bits/8
}screen_handler_t;

#endif


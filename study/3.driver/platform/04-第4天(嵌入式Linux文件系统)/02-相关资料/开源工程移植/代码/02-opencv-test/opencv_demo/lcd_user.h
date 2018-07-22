
#ifndef		__LCD_USER_H
#define 	__LCD_USER_H

typedef struct pixel_data{
	unsigned char blue;
	unsigned char green;
	unsigned char red;
	unsigned char alpha;
}pixel_data_t;

typedef struct screen_handler{
	unsigned char *addr;		// framebuffer�ĵ�ַ
	unsigned int width;			// ��Ļ�Ŀ��
	unsigned int height;		// ��Ļ�ĸ߶�
	unsigned int pixels;		// ��Ļ������ pixels = width*height
	unsigned int bits;			// ÿ�����ص�λ��
	unsigned int line_bytes;		// ÿ�����ص�λ�� line_bytes = width*bits/8
	unsigned long size;			// framebuffer�Ĵ�С size = pixels*bits/8
}screen_handler_t;

#endif


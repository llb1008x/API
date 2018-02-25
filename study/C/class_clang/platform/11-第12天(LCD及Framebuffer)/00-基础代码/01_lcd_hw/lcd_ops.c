#include "types.h"

#define CFG_LCD_FBUFFER				(0x48000000)
#define BMP_BUFFER		(0x45000000)

#define LCD_COL			800
#define LCD_ROW			480	

void delay(void)
{
	volatile int i=10000;
	volatile int j=10000;

	for(;i>0;i--)
		for(;j>0;j--);
}

/*在指定点显示出color*/
void lcd_draw_pixel(int row, int col, u32 color)
{
	u32 * pixel = (u32*)CFG_LCD_FBUFFER;

	*(pixel + row * LCD_COL + col) = color;	

	return;
} 

void lcd_clear_screen(u32 color)
{
	int i, j;
		
	for (i = 0; i < LCD_ROW; i++)
		for (j = 0; j < LCD_COL; j++)
		{
			lcd_draw_pixel(i, j, color);
		}

	return;
}

/*画水平线*/
void lcd_draw_hline(int row, int col1, int col2, u32 color)
{
	int j;
	
	for (j = col1; j <= col2; j++)
	{
		lcd_draw_pixel(row, j, color);
	}

	return;
}

/*画垂直线*/
void lcd_draw_vline(int col, int row1, int row2, u32 color)
{
	int i;
	
	for (i = row1; i <= row2; i++)
	{
		lcd_draw_pixel(i, col, color);
	}

	return;
}

/*画垂直交叉线*/
void lcd_draw_cross(int row, int col, int halflen, u32 color)
{
	lcd_draw_hline(row, col-halflen, col+halflen, color);
	
	lcd_draw_vline(col, row-halflen, row+halflen, color);
	
	return;
}

void lcd_draw_bmp(unsigned int bmp_file_addr)
{
	int i, j;
	unsigned char * p = (unsigned char *)BMP_BUFFER;
	u32 blue, green, red;
	u32 color;

	if(bmp_file_addr != 0)
		p = bmp_file_addr;
	
	// read bmp file
	// bmp file header is 54 bytes
	p += 54;
	
	for (i = 0; i < LCD_ROW; i++)
		for (j = 0; j < LCD_COL; j++)
		{
			blue = *p++;
			green = *p++;
			red = *p++;
		
			color = red << 16 | green << 8 | blue << 0;
			
			lcd_draw_pixel(LCD_ROW-1-i, j, color);
		}

	return;
}

void lcd_test(void)
{
	lcd_draw_bmp(0);

	lcd_draw_hline(10,10,200,(58<<16)|(58<<8)|(58));
	lcd_draw_vline(10,10,200,(58<<16)|(58<<8)|(58));
	lcd_draw_cross(100,100,100,(255<<16)|(128<<8)|(64));
	
	return;
}



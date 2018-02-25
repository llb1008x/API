#include "uart.h"

/*
* debug:
* 			tftp 0x45000000 172.20.223.22:test.bmp
*/
void LCD_Initialize(void);
void lcd_test(void);


int
main(void)
{
	printf("\ns5pv%d: test the lcd-demo......\n", 210);
	
	LCD_Initialize();
	lcd_test();

#if 1
	while (1) {		/*ctrl+CÍË³ö */
		if (_getc_int() == 0x03)
			break;
	}
#endif
	return 0;
}

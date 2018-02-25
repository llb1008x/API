#include "uart.h"

int main(void)
{
	printf("\ns5pv210: test the 01-no-subdir......\n");
	printf("\nPress 'Ctrl+C' to end the test......\n");
	
	while(1)
	{
		//ctrl+CÍË³ö
		if(_getc_int() == 0x03)
			break;
	}
	return 0;
}




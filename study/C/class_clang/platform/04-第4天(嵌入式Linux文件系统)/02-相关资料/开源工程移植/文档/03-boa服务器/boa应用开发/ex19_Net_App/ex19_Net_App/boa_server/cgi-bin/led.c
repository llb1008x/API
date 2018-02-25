#include"led.h"

void led_fun(int *led_config)
{
	unsigned int LED = 0;
	int gpg_led = open("/dev/gpg/0", O_RDWR);	// open GPIOG
	ioctl(gpg_led, GPIO_SET_MULTI_PIN_OUT, 0xFF);	// set GPIOG4~7 output
	LED = 0
	    | led_config[7] << 7
			| led_config[6] << 6
			| led_config[5] << 5
			| led_config[4] << 4
			| led_config[3] << 3
			| led_config[2] << 2
			| led_config[1] << 1
			| led_config[0]
			;
//		printf("LED=%d\n",LED);
		write(gpg_led, &LED, sizeof(LED));	// set LED status
	close(gpg_led);
}

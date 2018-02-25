#ifndef _LED_INCLUDE_H
#define _LED_INCLUDE_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <asm/hardware.h>
#include <asm-arm/arch-s3c2410/gpio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>

void led_fun(int *led_config);

#endif //_LED_INCLUDE_H

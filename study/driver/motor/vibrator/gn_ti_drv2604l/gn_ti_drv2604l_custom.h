#ifndef GN_TI_DRV2604L_CUSTOM_H
#define GN_TI_DRV2604L_CUSTOM_H

#include "gn_ti_drv2604l.h"


#define DRV2604L_I2C_BUS_ID 4
#define DRV2604L_I2C_ADDR			0x5A
//#define DRV2604L_GPIO_ENABLE_PIN GPIO45|0x80000000

#if 0
static struct DRV2604L_platform_data  drv2604l_plat_data = {
//	.GpioEnable = DRV2604L_GPIO_ENABLE_PIN,		//enable the chip
	.GpioTrigger = 0,						//external trigger pin, (0: internal trigger)
	.loop = CLOSE_LOOP,
	.RTPFormat = Signed,
	.BIDIRInput = BiDirectional,
	.actuator = {
		.device_type = LRA,
		.rated_vol = 0x2e,
		.over_drive_vol = 0x53,
		.LRAFreq = 240,
	},
};
#endif

#endif


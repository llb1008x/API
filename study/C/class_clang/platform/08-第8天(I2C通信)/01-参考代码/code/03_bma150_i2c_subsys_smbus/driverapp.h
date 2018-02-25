#ifndef __DRIVERAPP_H__
#define __DRIVERAPP_H__

#define GET_POSITION	0x11
#define CTRL_SET		0x12

#define GET_X	0x02
#define GET_Y	0x04
#define GET_Z	0x06

/* Data register addresses */
#define BMA150_DATA_0_REG	0x00
#define BMA150_DATA_1_REG	0x01
#define BMA150_DATA_2_REG	0x02

/* Control register addresses */
#define BMA150_CTRL_0_REG	0x0A
#define BMA150_CTRL_1_REG	0x0B
#define BMA150_CTRL_2_REG	0x14
#define BMA150_CTRL_3_REG	0x15

/* Configuration/Setting register addresses */
#define BMA150_CFG_0_REG	0x0C
#define BMA150_CFG_1_REG	0x0D
#define BMA150_CFG_2_REG	0x0E
#define BMA150_CFG_3_REG	0x0F
#define BMA150_CFG_4_REG	0x10
#define BMA150_CFG_5_REG	0x11


#define BMA150_ACC_X_LSB_REG BMA150_DATA_2_REG

struct position {
	short x;
	short y;
	short z;
};

struct bma150_device {
	unsigned int i2cdata;
	unsigned int i2cclk;
	int usec;
	unsigned char addr;			//device address
};


#endif



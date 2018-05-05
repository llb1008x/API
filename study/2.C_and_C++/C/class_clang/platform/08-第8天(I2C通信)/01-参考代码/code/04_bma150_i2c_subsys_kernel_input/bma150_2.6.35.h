/* ************************************************************************
 *       Filename:  bma150.h
 *    Description:  
 *        Version:  1.0
 *        Created:  2012年10月18日 15时00分27秒
 *       Revision:  none
 *       Compiler:  gcc
 *         Author:  YOUR NAME (), 
 *        Company:  
 * ************************************************************************/

#define BMAIO					0x50
#define BMA_IOCTL_ENABLE		_IO(BMAIO, 0x31)
#define BMA_IOCTL_DISABLE		_IO(BMAIO, 0x32)
#define BMA_IOCTL_IS_ENABLE		_IOR(BMAIO, 0x33, int)
#define BMA_IOCTL_DELAY_GET		_IOR(BMAIO, 0x34, int)
#define BMA_IOCTL_DELAY_SET		_IOW(BMAIO, 0x35, int)
#define BMA_IOCTL_POSITION_GET		_IOR(BMAIO, 0x36, int)
#define BMA_IOCTL_POSITION_SET		_IOW(BMAIO, 0x37, int)
#define BMA_IOCTL_THRESHOLD_GET		_IOR(BMAIO, 0x38, int)
#define BMA_IOCTL_THRESHOLD_SET		_IOW(BMAIO, 0x39, int)
#define BMA_IOCTL_DATA			_IOR(BMAIO, 0x3a, int[3])



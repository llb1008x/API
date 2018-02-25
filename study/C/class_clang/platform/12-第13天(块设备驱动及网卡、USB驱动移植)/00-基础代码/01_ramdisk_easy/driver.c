/*
* Copyright (c) 2014 sunplusedu
* This is a demo, about hwo to create a module
*
* Date and Dition:	2014-03-12	v1.0
* Author:				<xiuhai.deng@sunplusapp.com>
* 
*/

/*
* debug:
* 		cat xxx > /dev/demo_dev
* 		cat /dev/demo_dev
*/

#include <linux/module.h>	/* module_init */
#include <linux/fs.h>	/* file_operations */
#include <linux/device.h>	/* class device */
#include <linux/sched.h>		/* current */
#include <linux/mount.h>		/* struct vfsmount */
#include <asm/io.h>	/* writel() */
#include <plat/gpio-cfg.h>	/* s3c_gpio_cfgpin */
#include <mach/gpio.h>	/* gpio_set_value */
#include <linux/uaccess.h> /* copy_to_user() */
#include <linux/slab.h> /* kzalloc() */
#include <linux/err.h>
#include <asm/delay.h>     /* udelay */
#include <linux/miscdevice.h> /* struct miscdevice */
#include <linux/vmalloc.h>
#include <linux/blkdev.h>
#include <linux/hdreg.h>	/* geo */

#define 	VDISK_SIZE				10*1024*1024		//size is 10M
#define 	KERNEL_SECTOR_SIZE		512

static int ramdisk_major = 0;
static struct gendisk *gdisk;
spinlock_t ramdisk_splock;
static unsigned char *vmem;


static int vdisk_open(struct block_device *bdev, fmode_t mod)
{
	printk(KERN_WARNING "%s\n",__FUNCTION__);
	return 0;
}

static struct block_device_operations vdisk_fops = {
	.owner = THIS_MODULE,
	.open = vdisk_open,
};
static void vdisk_request(struct request_queue *q)
{
	struct request *req;
	unsigned int off;
	unsigned int size;
	printk(KERN_WARNING "%s\n",__FUNCTION__);

	req = blk_fetch_request(q);
	while(req){
		off = req->__sector*KERNEL_SECTOR_SIZE;
		size = blk_rq_cur_bytes(req);
		
		if(rq_data_dir(req) == READ)
			memcpy(req->buffer, vmem+off, size);
		else if(rq_data_dir(req) == WRITE)
			memcpy(vmem+off, req->buffer, size);
		
		if(!__blk_end_request_cur(req,0))
			req = blk_fetch_request(q);
	}
}

static int __init ramdisk_init(void)
{
	signed int ret = 0; 
	printk(KERN_WARNING "%s\n",__FUNCTION__);

	ramdisk_major = register_blkdev(ramdisk_major,"ramdisk");
	if(ramdisk_major < 0){
		ret = ramdisk_major;
		goto register_blkdev_fail;
	}
	vmem = vmalloc(VDISK_SIZE);
	if(vmem <= 0){
		ret = PTR_ERR(vmem);
		goto vmalloc_fail;
	}
	spin_lock_init(&ramdisk_splock);
	gdisk = alloc_disk(3);
	if(gdisk <= 0){
		ret = PTR_ERR(gdisk);
		goto alloc_disk_fail; 
	}
	gdisk->major = ramdisk_major;
	gdisk->first_minor = 0;
	sprintf(gdisk->disk_name,"ramdiska");
	gdisk->queue = blk_init_queue(vdisk_request,&ramdisk_splock);
	set_capacity(gdisk,VDISK_SIZE/KERNEL_SECTOR_SIZE);
	gdisk->fops = &vdisk_fops;
	add_disk(gdisk);
	return 0;
	
alloc_disk_fail:
	vfree(vmem);
vmalloc_fail:
	unregister_blkdev(ramdisk_major, "ramdisk");
register_blkdev_fail:
	return ret;
}
static void __exit ramdisk_exit(void)
{
	printk(KERN_WARNING "%s\n",__FUNCTION__);
	del_gendisk(gdisk);
	put_disk(gdisk);
	vfree(vmem);
	unregister_blkdev(ramdisk_major, "ramdisk");
}
module_init(ramdisk_init);
module_exit(ramdisk_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("xiuhai.deng@sunplusedu.com");

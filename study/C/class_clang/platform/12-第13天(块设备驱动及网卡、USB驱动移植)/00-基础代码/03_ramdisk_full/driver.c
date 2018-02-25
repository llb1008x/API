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
#define 	VRAMDISK_COUNT			2
#define 	VRAMDISK_PARTITION		3

static int ramdisk_major = 0;
struct ramdisk_dev{
	struct gendisk *gdisk;
	spinlock_t ramdisk_splock;
	unsigned char *vmem;
};
static struct ramdisk_dev rdisk[VRAMDISK_COUNT];

static int vdisk_open(struct block_device *bdev, fmode_t mod)
{
	printk(KERN_WARNING "%s\n",__FUNCTION__);
	return 0;
}
static int vdisk_release (struct gendisk *gdisk, fmode_t mod)
{
	printk(KERN_WARNING "%s\n",__FUNCTION__);
	return 0;
}
static int vdisk_ioctl(struct block_device *bdev, fmode_t mod, unsigned int cmd,unsigned long arg)
{
	printk(KERN_WARNING "%s\n",__FUNCTION__);
	return 0;
}
/* 检测到设备变化 */
static int vdisk_media_changed(struct gendisk *gdisk)
{
	printk(KERN_WARNING "%s\n",__FUNCTION__);
	return 0;
}
/* 检测到设备变化后的回调处理函数 */
static int vdisk_revalidate_disk(struct gendisk *gdisk)
{
	printk(KERN_WARNING "%s\n",__FUNCTION__);
	return 0;
}

static int vdisk_getgeo(struct block_device* bdev, struct hd_geometry* hd_geo)
{
	printk(KERN_WARNING "%s\n",__FUNCTION__);
	//	struct ramdisk_dev *prdisk;
	//	prdisk = bdev->bd_disk->private_data;
	
	/* size = cylinders*sectors*heads*SECTOR_SIZE */
	hd_geo->cylinders = VDISK_SIZE/KERNEL_SECTOR_SIZE/4/16;
	hd_geo->heads = 4;
	hd_geo->sectors = 16;
	hd_geo->start = 0;
	return 0;
}

static struct block_device_operations vdisk_fops = {
	.owner = THIS_MODULE,
	.open = vdisk_open,
	.release = vdisk_release,
	.ioctl = vdisk_ioctl,	
	.media_changed = vdisk_media_changed,
	.revalidate_disk = vdisk_revalidate_disk,
	.getgeo = vdisk_getgeo,
};

static void vdisk_request(struct request_queue *q)
{
	struct request *req;
	unsigned int off;
	unsigned int size;
	struct ramdisk_dev *prdisk = NULL;
	printk(KERN_WARNING "%s\n",__FUNCTION__);

	req = blk_fetch_request(q);
	while(req){
		prdisk = req->rq_disk->private_data;
		off = req->__sector*KERNEL_SECTOR_SIZE;
		size = blk_rq_cur_bytes(req);
		
		if(rq_data_dir(req) == READ)
			memcpy(req->buffer, prdisk->vmem+off, size);
		else if(rq_data_dir(req) == WRITE)
			memcpy(prdisk->vmem+off, req->buffer, size);
		
		if(!__blk_end_request_cur(req,0))
			req = blk_fetch_request(q);
	}
}

static int __init ramdisk_init(void)
{
	signed int ret = 0; 
	int lp = 0;
	printk(KERN_WARNING "%s\n",__FUNCTION__);

	ramdisk_major = register_blkdev(ramdisk_major,"ramdisk");
	if(ramdisk_major < 0){
		ret = ramdisk_major;
		goto register_blkdev_fail;
	}
	memset(rdisk,0,sizeof(struct ramdisk_dev)*VRAMDISK_COUNT);
	for(lp=0;lp < VRAMDISK_COUNT;lp++){
		rdisk[lp].vmem = vmalloc(VDISK_SIZE);
		if(rdisk[lp].vmem <= 0){
			ret = PTR_ERR(rdisk[lp].vmem);
			goto vmalloc_fail;
		}
		spin_lock_init(&rdisk[lp].ramdisk_splock);
		rdisk[lp].gdisk = alloc_disk(VRAMDISK_PARTITION);
		if(rdisk[lp].gdisk <= 0){
			ret = PTR_ERR(rdisk[lp].gdisk);
			goto alloc_disk_fail; 
		}
		rdisk[lp].gdisk->major = ramdisk_major;
		rdisk[lp].gdisk->first_minor = lp*VRAMDISK_PARTITION;
		sprintf(rdisk[lp].gdisk->disk_name,"ramdisk%c",'a'+lp);
		rdisk[lp].gdisk->queue = blk_init_queue(vdisk_request,&rdisk[lp].ramdisk_splock);
		set_capacity(rdisk[lp].gdisk,VDISK_SIZE/KERNEL_SECTOR_SIZE);
		rdisk[lp].gdisk->fops = &vdisk_fops;
		rdisk[lp].gdisk->private_data  = rdisk+lp; 
		add_disk(rdisk[lp].gdisk);
	}
	return 0;
	
alloc_disk_fail:
	
	for(lp=0;lp < VRAMDISK_COUNT;lp++){
		if(rdisk[lp].vmem)
			vfree(rdisk[lp].vmem);
	}
vmalloc_fail:
	unregister_blkdev(ramdisk_major, "ramdisk");
register_blkdev_fail:
	return ret;
}
static void __exit ramdisk_exit(void)
{
	int lp = 0;
	printk(KERN_WARNING "%s\n",__FUNCTION__);
	
	for(lp=0;lp < VRAMDISK_COUNT;lp++){
		del_gendisk(rdisk[lp].gdisk);
		put_disk(rdisk[lp].gdisk);
		vfree(rdisk[lp].vmem);
	}
	unregister_blkdev(ramdisk_major, "ramdisk");
}
module_init(ramdisk_init);
module_exit(ramdisk_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("xiuhai.deng@sunplusedu.com");

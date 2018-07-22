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


#define DEVICE_NAME "demo_char"

static int major = 0;
static struct class *demo_class = NULL;
static struct device *demo_device = NULL;
static void __iomem *gph_base = NULL;

static int demo_open(struct inode *pinode, struct file *pfile)
{
	printk(KERN_WARNING "%s\n",__FUNCTION__);
	printk(KERN_WARNING "%s\n",THIS_MODULE->name);
	printk(KERN_WARNING "%d\n",current->pid);
	printk(KERN_WARNING "%s\n",pfile->f_path.mnt->mnt_devname);
	
	return 0;
}
static int demo_release(struct inode *pinode, struct file *pfile)
{
	printk(KERN_WARNING "%s\n",__FUNCTION__);
	return 0;
}
static ssize_t demo_read(struct file *pfile, char __user *puser, size_t count, loff_t *poffset)
{
	static unsigned char data = 0;
	
	printk(KERN_WARNING "%s,%d,%d\n",__FUNCTION__,imajor(pfile->f_dentry->d_inode),iminor(pfile->f_dentry->d_inode));
	printk(KERN_WARNING "count=%d,offset=%d\n",count,*poffset);
	*poffset += count;
	
	data = ~data;
	writel(data,(gph_base+4));
	return count;
}
static ssize_t demo_write(struct file *pfile, const char __user *puser, size_t count, loff_t *poffset)
{
	printk(KERN_WARNING "%s,%d,%d\n",__FUNCTION__,imajor(pfile->f_dentry->d_inode),iminor(pfile->f_dentry->d_inode));
	printk(KERN_WARNING "count=%d,offset=%d\n",count,*poffset);
	*poffset += count;
	return count;
}

struct file_operations demo_fops = {
.owner = THIS_MODULE,
.open = demo_open,
.read = demo_read,
.write = demo_write,
.release = demo_release,
};
static int demo_gph_init(void)
{
	struct resource *gph_res = NULL;
	
	gph_res = request_mem_region(0xe0200c00,0x80,"demo_gph");
	if(IS_ERR(gph_res)){
	return PTR_ERR(gph_res);
	}
	gph_base = ioremap(0xe0200c00,0x80);
	if(IS_ERR(gph_base)){
	release_mem_region(0xe0200c00,0x80);
	return PTR_ERR(gph_base);
	}
	writel(0x1111,gph_base);
	writel(0xf,(gph_base+4));
	return 0;
}
static int __init demo_module_init(void)
{
	int ret = 0;
	
	printk(KERN_WARNING "%s\n",__FUNCTION__);
	major = register_chrdev(major,DEVICE_NAME,&demo_fops);
	if(major < 0)
	{
		ret = major;
		goto register_chrdev_fail;
	}
	demo_class = class_create(THIS_MODULE,DEVICE_NAME);
	if(IS_ERR(demo_class)){
		ret = PTR_ERR(demo_class);
		goto class_create_fail;
	}
	demo_device = device_create(demo_class,NULL,MKDEV(major,0),NULL,"demo_dev");
	if(IS_ERR(demo_device)){
		ret = PTR_ERR(demo_device);
		goto device_create_fail;
	}
	ret = demo_gph_init();
	if(ret != 0)
		goto gph_init_fail;
	
return 0;
gph_init_fail:
	iounmap(gph_base);
device_create_fail:
	class_destroy(demo_class);
class_create_fail:
	unregister_chrdev(major,DEVICE_NAME);
register_chrdev_fail:
	return ret;
}
static void __exit demo_module_exit(void)
{
	printk(KERN_WARNING "%s\n",__FUNCTION__);	

	iounmap(gph_base);
	release_mem_region(0xe0200c00,0x80);
	
	unregister_chrdev(major,DEVICE_NAME);
	device_destroy(demo_class,MKDEV(major,0));
	class_destroy(demo_class);
}

module_init(demo_module_init);
module_exit(demo_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("xiuhai.deng@sunplusapp.com");


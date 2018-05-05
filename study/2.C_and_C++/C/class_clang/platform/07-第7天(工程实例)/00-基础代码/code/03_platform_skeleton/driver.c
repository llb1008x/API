/*
* Copyright (c) 2014 sunplusedu
* This is a demo, about hwo to create a module
*
* Date and Dition:	2014-04-12	v1.0
* Author:				<xiuhai.deng@sunplusapp.com>
* 
*/

/*
* debug:
*		./t
* 		cat /dev/demo_dev
*		echo "xxx" > /dev/demo_dev
* 		cat xxx.y > /dev/demo_dev
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
#include <linux/interrupt.h> /* request_irq() */
#include <linux/irq.h> /* IRQ_TYPE_EDGE_FALLING() */
#include <linux/delay.h> /* mdelay() */
#include <linux/kfifo.h> /* kfifo */
#include <linux/poll.h> /* poll */
#include <linux/kthread.h> /* kthread */
#include <linux/miscdevice.h> /* struct miscdevice */
#include <linux/cdev.h> /* cdev_init */
#include <linux/proc_fs.h>	/* proc_dir_entry */
#include <linux/platform_device.h> /* platform_driver_register() */
#include <linux/ioport.h>	/* resource */

#define GPH2_7  _IO('g',7)
#define GPH2_6	_IO('g',6)
#define GPH2_5	_IO('g',5)
#define GPH2_4	_IO('g',4)

#define DEVICE_NAME "demo_char"

static struct class *demo_class = NULL;
static struct device *demo_device = NULL;
static struct workqueue_struct *key_workqueue = NULL;
static struct work_struct key_work;
static struct kfifo key_fifo;
static atomic_t open_flag = ATOMIC_INIT(0);
static spinlock_t key_spinlock;
static struct hrtimer  hr_timer;
static ktime_t ktime;
/* wait_queue_head_t key_queue; */
DECLARE_WAIT_QUEUE_HEAD(key_queue);
DECLARE_WAIT_QUEUE_HEAD(select_queue);
static struct task_struct *kthread_ops;
static struct completion key_complet;
static struct cdev key_cdev;
static dev_t devid;
static struct proc_dir_entry *proc_root;
static struct proc_dir_entry *proc_file;

struct key_data{
char status;/* up=0,down=1 */
char type;/* short=0 long=1 */
char num;
};
static struct key_data key_val = {
.status = 0,
.type = 0,
.num = 0,
};

static int demo_open(struct inode *pinode, struct file *pfile)
{
	printk(KERN_WARNING "%s\n%s\n%d\n",__FILE__,__FUNCTION__,__LINE__);
	printk(KERN_WARNING "%s\n",THIS_MODULE->name);
	printk(KERN_WARNING "%d\n",current->pid);
	printk(KERN_WARNING "%s\n",pfile->f_path.mnt->mnt_devname);

	if(atomic_read(&open_flag) == 0)
		enable_irq(IRQ_EINT(23));
	atomic_inc(&open_flag);
	return 0;
}
static int demo_release(struct inode *pinode, struct file *pfile)
{
	printk(KERN_WARNING "%s\n",__FUNCTION__);
	hrtimer_cancel(&hr_timer);
	atomic_dec(&open_flag);
	if(atomic_read(&open_flag) == 0)
		disable_irq(IRQ_EINT(23));
	return 0;
}
static ssize_t demo_read(struct file *pfile, char __user *puser, size_t count, loff_t *poffset)
{
	struct key_data key_read;
	signed ret = 0;
	unsigned long irqflag;
	unsigned int len = min(count,sizeof(key_read));

if((pfile->f_flags & O_NONBLOCK) == 0)
	wait_event_interruptible(key_queue,!kfifo_is_empty(&key_fifo));

//	if(kfifo_is_empty(&key_fifo))
//		return -1;
	spin_lock_irqsave(&key_spinlock,irqflag);
	ret = kfifo_out(&key_fifo,&key_read,sizeof(key_read));
	spin_unlock_irqrestore(&key_spinlock,irqflag);
	if(ret != sizeof(key_read))
		return -EFAULT;	

	if(copy_to_user(puser,(void *)&key_read,sizeof(key_read)) != 0)
		return -EFAULT;	
	return len;
}
static ssize_t demo_write(struct file *pfile, const char __user *puser, size_t count, loff_t *poffset)
{
	unsigned char data[30] = {0};
	unsigned int len = min(count,sizeof(data));

	printk(KERN_WARNING "%s,%d,%d\n",__FUNCTION__,imajor(pfile->f_dentry->d_inode),iminor(pfile->f_dentry->d_inode));
	printk(KERN_WARNING "count=%d,offset=%d\n",count,(unsigned int)*poffset);

	if(copy_from_user((void *)&data,puser,len) != 0)
		return -EFAULT;	
	printk(KERN_WARNING "write:%s\n",data);
	
	*poffset += len;
	return len;
}
static long demo_unlocked_ioctl(struct file *pfile, unsigned int cmd, unsigned long arg)
{
	printk(KERN_WARNING "%c,%d\n",_IOC_TYPE(cmd),_IOC_NR(cmd));

	switch(cmd){
	case GPH2_7:
		s3c_gpio_cfgpin(S5PV210_GPH2(7), arg);
		s3c_gpio_setpull(S5PV210_GPH2(7),S3C_GPIO_PULL_UP);
		break;
	case GPH2_6:
		s3c_gpio_cfgpin(S5PV210_GPH2(6), arg);
		s3c_gpio_setpull(S5PV210_GPH2(6),S3C_GPIO_PULL_UP);
		break;
	case GPH2_5:
		s3c_gpio_cfgpin(S5PV210_GPH2(5), arg);
		s3c_gpio_setpull(S5PV210_GPH2(5),S3C_GPIO_PULL_UP);
		break;
	case GPH2_4:
		s3c_gpio_cfgpin(S5PV210_GPH2(4), arg);
		s3c_gpio_setpull(S5PV210_GPH2(4),S3C_GPIO_PULL_UP);
		break;
	default:
		break;
	}
	
	return 0;
}

static unsigned int demo_poll(struct file *pfile, struct poll_table_struct *ptable)
{
	poll_wait(pfile,&select_queue,ptable);
	return kfifo_is_empty(&key_fifo)?0:POLLIN;
}

struct file_operations demo_fops = {
.owner = THIS_MODULE,
.open = demo_open,
.read = demo_read,
.write = demo_write,
.unlocked_ioctl = demo_unlocked_ioctl,
.poll = demo_poll,
.release = demo_release,
};
enum hrtimer_restart key_timerhandler(struct hrtimer *timer)
{

	int ret = 0;
	unsigned long irqflag;
	
//	printk(KERN_WARNING "in:    %s\n",__FUNCTION__);
	spin_lock_irqsave(&key_spinlock,irqflag);
	key_val.type = 1;
	ret = kfifo_in(&key_fifo,&key_val,sizeof(key_val));
	spin_unlock_irqrestore(&key_spinlock,irqflag);
	if(ret != sizeof(key_val))
		printk(KERN_WARNING "kfifo_in fail!\n");
	wake_up_interruptible(&key_queue);
	wake_up_interruptible(&select_queue);
	ktime = ns_to_ktime(1000*1000*100);/* 100ms */
	hrtimer_add_expires(&hr_timer,ktime);/* update timer */
//	printk(KERN_WARNING "exit:  %s\n",__FUNCTION__);

	return HRTIMER_RESTART;
}

static void key_workhandler(struct work_struct *work)
{
	int ret = 0;
	static unsigned char num = 200;
	unsigned long irqflag;
//	printk(KERN_WARNING "%s\n",__FUNCTION__);

	if(key_val.status == 0){/* down */
		s3c_gpio_cfgpin(S5PV210_GPH2(7), S3C_GPIO_INPUT);
		mdelay(30);
		ret = gpio_get_value(S5PV210_GPH2(7));
		s3c_gpio_cfgpin(S5PV210_GPH2(7), S3C_GPIO_SFN(0xf));
		if( ret != 0)
			return;
		num--;
//		if(kfifo_is_full(&key_fifo))
//			return;
		spin_lock_irqsave(&key_spinlock,irqflag);
		key_val.status = 1;
		key_val.num = num;
		key_val.type = 0;
		kfifo_in(&key_fifo,&key_val,sizeof(key_val));	
		spin_unlock_irqrestore(&key_spinlock,irqflag);
		wake_up_interruptible(&key_queue);
		wake_up_interruptible(&select_queue);
		ktime = ns_to_ktime(1000*1000*1000);
		hrtimer_start(&hr_timer,ktime,HRTIMER_MODE_REL);		
	}
	else{/* 如果是 up 则不用去抖动 */
		hrtimer_cancel(&hr_timer);
//		if(kfifo_is_full(&key_fifo))
//			return;
		spin_lock_irqsave(&key_spinlock,irqflag);
		key_val.status = 0;
		key_val.num = num;
		kfifo_in(&key_fifo,&key_val,sizeof(key_val));	
		spin_unlock_irqrestore(&key_spinlock,irqflag);
		wake_up_interruptible(&key_queue);
		wake_up_interruptible(&select_queue);
	}	
//	printk(KERN_WARNING "exit:  %s\n",__FUNCTION__);
}
static int kthread_ops_key(void *data)
{
	s3c_gpio_cfgpin(S5PV210_GPH0(3), S3C_GPIO_OUTPUT);
	s3c_gpio_setpull(S5PV210_GPH0(3),S3C_GPIO_PULL_UP);

	//	printk(KERN_WARNING "in:	%s\n",__FUNCTION__);
	for(;;){
		if(kthread_should_stop())/* 是否终止 */
			break;

		/* ops */
		gpio_set_value(S5PV210_GPH0(3),key_val.num&1);	
		schedule_timeout(HZ);
	}
	//	printk(KERN_WARNING "exit:	%s\n",__FUNCTION__);
	
	complete_and_exit(&key_complet,0);
	return 0;
}

irqreturn_t key1_irqhandler(int irq,void *data)
{
//	printk(KERN_WARNING "in:    %s\n",__FUNCTION__);
	queue_work(key_workqueue,&key_work);
//	printk(KERN_WARNING "exit:  %s\n",__FUNCTION__);
	return IRQ_HANDLED;
}
irqreturn_t key2_irqhandler(int irq,void *data)
{
	printk(KERN_WARNING "%s\n",__FUNCTION__);
	return IRQ_HANDLED;
}

static int demo_gph_init(void)
{
	signed int ret = 0;
	
	s3c_gpio_cfgpin(S5PV210_GPH3(0), S3C_GPIO_OUTPUT);
	gpio_set_value(S5PV210_GPH3(0),0);	

	s3c_gpio_setpull(S5PV210_GPH2(7),S3C_GPIO_PULL_UP);
	s3c_gpio_setpull(S5PV210_GPH2(6),S3C_GPIO_PULL_UP);
	
	//ret = request_irq(IRQ_EINT(23),key_irqhandler,IRQ_TYPE_EDGE_BOTH,"demo_key",NULL);
	ret = request_irq(IRQ_EINT(23),key1_irqhandler,IRQ_TYPE_EDGE_BOTH,"demo_key1",NULL);
	if(ret != 0)
		return -1;
	disable_irq(IRQ_EINT(23));
	ret = request_irq(IRQ_EINT(22),key2_irqhandler,IRQ_TYPE_EDGE_FALLING,"demo_key2",NULL);
	if(ret != 0)
		return -1;
	
	return 0;
}
static int demo_read_proc(char *page, char **start, off_t off,int count, int *eof, void *data)
{
	int ret = 0;

	ret = sprintf(page,"key:status=%d,type=%d,num=%d\n",key_val.status,key_val.type,key_val.num);
	return ret;
}
static int demo_write_proc(struct file *file, const char __user *buffer,unsigned long count, void *data)
{
	unsigned char proc_buff[30] = "";
	unsigned long len = min(count,sizeof(proc_buff));

	if(copy_from_user(proc_buff,buffer,len))
		return -ENOMEM;
	printk(KERN_WARNING "proc write: %s\n",proc_buff);
	if(!strcmp(proc_buff,"clear"))
		kfifo_reset(&key_fifo);
	return len;
}

static int demo_key_probe(struct platform_device *pdev)
{
	signed int ret = 0;
	
	printk(KERN_WARNING "%s\n",__FUNCTION__);
//	devid = MKDEV(250,0);/* 指定主设备号 */
//	ret = register_chrdev_region(devid,1,DEVICE_NAME);
	ret = alloc_chrdev_region(&devid,0,1,DEVICE_NAME);
	if(ret)
		goto register_chrdev_fail;
	cdev_init(&key_cdev,&demo_fops);
	ret = cdev_add(&key_cdev,devid,1);
	if(ret)
		goto register_chrdev_fail;

	demo_class = class_create(THIS_MODULE,DEVICE_NAME);
	if(IS_ERR(demo_class)){
		ret = PTR_ERR(demo_class);
		goto class_create_fail;
	}
	demo_device = device_create(demo_class,NULL,devid,NULL,"demo_dev");
	if(IS_ERR(demo_device)){
		ret = PTR_ERR(demo_device);
		goto device_create_fail;
	}
	key_workqueue = create_workqueue("key_workqueue");
	INIT_WORK(&key_work,key_workhandler);
	ret = kfifo_alloc(&key_fifo,128,GFP_KERNEL);
	if(ret != 0)
		goto device_create_fail;
	spin_lock_init(&key_spinlock);	
	hrtimer_init(&hr_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
	hr_timer.function = key_timerhandler;
	
	ret = demo_gph_init();
	
	init_completion(&key_complet);
	kthread_ops = kthread_create(kthread_ops_key,NULL,"key_ops");
	if(IS_ERR(kthread_ops))
	{
		complete(&key_complet);
		ret = PTR_ERR(kthread_ops);
		goto device_create_fail;
	}
	wake_up_process(kthread_ops);
	
	proc_root = proc_mkdir("demo_key",NULL);
	if(IS_ERR(proc_root))
	{
		ret = PTR_ERR(proc_root);
		goto device_create_fail;
	}
	proc_file = create_proc_entry("key",0666,proc_root);
	if(IS_ERR(proc_file))
	{
		ret = PTR_ERR(proc_file);
		goto create_proc_entry_fail;
	}
	proc_file->read_proc = &demo_read_proc;
	proc_file->write_proc = &demo_write_proc;
	
	
return 0;
create_proc_entry_fail:
	remove_proc_entry("demo_key",NULL);
device_create_fail:
	class_destroy(demo_class);
	kfifo_free(&key_fifo);
class_create_fail:
	cdev_del(&key_cdev);
	unregister_chrdev_region(devid,1);
register_chrdev_fail:
	return ret;
}
static int demo_key_remove(struct platform_device *pdev)
{
	printk(KERN_WARNING "%s\n",__FUNCTION__);
	kthread_stop(kthread_ops);
	wait_for_completion(&key_complet);	
	kfifo_free(&key_fifo);
	destroy_workqueue(key_workqueue);
	free_irq(IRQ_EINT(23),NULL);
	free_irq(IRQ_EINT(22),NULL);
	cdev_del(&key_cdev);
	unregister_chrdev_region(devid,1);
	device_destroy(demo_class,devid);
	class_destroy(demo_class);
	if(!IS_ERR(proc_file))
		remove_proc_entry("key",proc_root);
	if(!IS_ERR(proc_root))
		remove_proc_entry("demo_key",NULL);	
	return 0;
}
static struct platform_driver driver_demo_key = {
.probe = demo_key_probe,
.remove = demo_key_remove,
.driver = {
	.owner = THIS_MODULE,
	.name = "s5pv210-key",
}
};

static int __init demo_module_init(void)
{
	return platform_driver_register(&driver_demo_key);
}
static void __exit demo_module_exit(void)
{
	printk(KERN_WARNING "%s\n",__FUNCTION__);	
	platform_driver_unregister(&driver_demo_key);
	return;
}

module_init(demo_module_init);
module_exit(demo_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("xiuhai.deng@sunplusapp.com");


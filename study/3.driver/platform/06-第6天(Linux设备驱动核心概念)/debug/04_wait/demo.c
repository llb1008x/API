#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <asm/uaccess.h>
#include <plat/gpio-cfg.h>
#include <mach/gpio.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/delay.h>
#include <linux/workqueue.h>
#include <linux/kfifo.h>
#include <linux/timer.h>
#include <linux/wait.h>

static unsigned int major = 0;
static struct class *my_class = NULL;
static struct workqueue_struct *my_workqueue = NULL;
static struct work_struct my_work;
static struct kfifo my_kfifo;
static struct timer_list my_timer;
wait_queue_head_t my_wait;

static int demo_open(struct inode *i_node, struct file *f_ile)
{
	printk(KERN_CRIT "zai %s\n",__FUNCTION__);
//	enable_irq(gpio_to_irq(S5PV210_GPH2(3)));
	return 0;
}

static ssize_t demo_read(struct file *f_ile, char __user *buff, size_t len, loff_t *off_t)
{
	char buf[20];
	int lenth = 0;
//	printk(KERN_CRIT "zai %s\n",__FUNCTION__);
	//���gph2_3���ŵ�ƽ
//	if(gpio_get_value(S5PV210_GPH2(3)))
//		printk("key was down!\n");
	//����
	wait_event_interruptible(my_wait,!kfifo_is_empty(&my_kfifo));
	if(!kfifo_is_empty(&my_kfifo))
		lenth = kfifo_out(&my_kfifo,buf,strlen("key 2"));

	return (lenth-copy_to_user(buff,buf,lenth));
//	return 0;
}

static int demo_release(struct inode *i_node, struct file *f_ile)
{
	printk(KERN_CRIT "zai %s\n",__FUNCTION__);

	return 0;
}

static int demo_ioctl(struct inode *i_node, struct file *f_ile, unsigned int cmd, unsigned long data)
{
/*
	printk(KERN_CRIT "you say cmd %d\n",cmd);
	switch cmd{
			case 1://���
			case 3://�ص�
		}
*/
	return 0;
}

static struct file_operations f_ops = {
	.owner = THIS_MODULE,
	.open = demo_open,
	.read = demo_read,
	.release = demo_release,
	.ioctl = demo_ioctl,
};

static irqreturn_t my_handle(int irq,void *dev_id)
{
//	udelay(1000*1000);
//	mdelay(1000);
	printk(KERN_CRIT "zai %s data = %s\n",__FUNCTION__,(char *)dev_id);
	if(!kfifo_is_full(&my_kfifo))
	{
		kfifo_in(&my_kfifo,dev_id,strlen(dev_id));
		//����
		wake_up_interruptible(&my_wait);
	}
	if(!work_pending(&my_work))
		queue_work(my_workqueue, &my_work);
	
	return IRQ_HANDLED;
}

static void my_workhandle(struct work_struct *work)
{
	printk(KERN_CRIT "zai %s 1\n",__FUNCTION__);
	if(!timer_pending(&my_timer))
	{
		my_timer.expires = jiffies+200;
	//	add_timer(&my_timer);
	}
	printk(KERN_CRIT "*************timer_function data = 97\n");
//	ssleep(1);
//	printk(KERN_CRIT "zai %s 2\n",__FUNCTION__);
}

static void timer_function(unsigned long data)
{
	printk(KERN_CRIT "*************%s data = %d\n",__FUNCTION__,(int)data);
	mod_timer(&my_timer,jiffies+50);
}

static int __init demo_init(void)
{
	struct device *my_device = NULL;
	int ret = 0;
	
	printk(KERN_CRIT "zai %s\n",__FUNCTION__);
	major = register_chrdev(0,"my_driver",&f_ops);
	my_class = class_create(THIS_MODULE, "my_class");
	my_device = device_create(my_class,NULL,MKDEV(major,3),NULL,"my_zxc0");
	//1,GPH3(0)����ߵ�ƽ
	s3c_gpio_cfgpin(S5PV210_GPH3(0),S3C_GPIO_OUTPUT);
	gpio_set_value(S5PV210_GPH3(0),1);
	//2,GPH2(3)����״̬�����ң�����
	//s3c_gpio_cfgpin(S5PV210_GPH2(3),S3C_GPIO_INPUT);
	s3c_gpio_setpull(S5PV210_GPH2(3),S3C_GPIO_PULL_DOWN);
	//3,�����ж�
	ret = request_irq(gpio_to_irq(S5PV210_GPH2(3)),my_handle,IRQF_SHARED|IRQ_TYPE_EDGE_BOTH,"my_irq",(void *)"key 2");
//	disable_irq(gpio_to_irq(S5PV210_GPH2(3)));
	my_workqueue = create_workqueue("my_queue");
	INIT_WORK(&my_work,my_workhandle);
	ret = kfifo_alloc(&my_kfifo,100,GFP_KERNEL);
	init_timer(&my_timer);
	my_timer.function = timer_function;
	my_timer.data = 97;
	init_waitqueue_head(&my_wait);
	
	return 0;
}

static void __exit demo_exit(void)
{
	printk(KERN_CRIT "zai %s\n",__FUNCTION__);
	unregister_chrdev(major,"my_driver");
	device_destroy(my_class, MKDEV(major,3));
	class_destroy(my_class);
	//4,�ͷ��ж�
	free_irq(gpio_to_irq(S5PV210_GPH2(3)),(void *)"key 2");
	destroy_workqueue(my_workqueue);
	kfifo_free(&my_kfifo);
	if(timer_pending(&my_timer))
		del_timer(&my_timer);
}

module_init(demo_init);
module_exit(demo_exit);

MODULE_AUTHOR("sunplusedu");
MODULE_LICENSE("GPL");
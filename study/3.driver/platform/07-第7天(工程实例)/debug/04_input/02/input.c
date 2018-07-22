#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <plat/gpio-cfg.h>
#include <mach/gpio.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/input.h>
//exec 0</dev/tty1
static struct input_dev *my_input = NULL;

static int input_open(struct input_dev *dev)
{
	return 0;
}
static void input_close(struct input_dev *dev)
{

}

static irqreturn_t my_handle(int irq,void *dev_id)
{
	input_report_key(my_input,KEY_L,1);
	input_sync(my_input);
	input_report_key(my_input,KEY_L,0);
	input_sync(my_input);
	input_report_key(my_input,KEY_S,1);
	input_sync(my_input);
	input_report_key(my_input,KEY_S,0);
	input_sync(my_input);
	input_report_key(my_input,KEY_ENTER,1);
	input_sync(my_input);
	input_report_key(my_input,KEY_ENTER,0);
	input_sync(my_input);

	return IRQ_HANDLED;
}

static int __init my_input_init(void)
{
	int ret = 0;
	
	my_input = input_allocate_device();
	my_input->open = input_open;
	my_input->close = input_close;
	set_bit(EV_KEY,my_input->evbit);
	set_bit(EV_SYN,my_input->evbit);
	set_bit(KEY_T,my_input->keybit);
	set_bit(KEY_L,my_input->keybit);
	set_bit(KEY_S,my_input->keybit);
	set_bit(KEY_ENTER,my_input->keybit);

	ret = input_register_device(my_input);
	
	s3c_gpio_cfgpin(S5PV210_GPH3(0),S3C_GPIO_OUTPUT);
	gpio_set_value(S5PV210_GPH3(0),1);
	s3c_gpio_setpull(S5PV210_GPH2(3),S3C_GPIO_PULL_DOWN);
	
	ret = request_irq(gpio_to_irq(S5PV210_GPH2(3)),my_handle,IRQF_SHARED|IRQ_TYPE_EDGE_BOTH,"my_irq",(void *)"key 2");
	
	return 0;
}

static void __exit my_input_exit(void)
{
	input_unregister_device(my_input);
	free_irq(gpio_to_irq(S5PV210_GPH2(3)),(void *)"key 2");
}

module_init(my_input_init);
module_exit(my_input_exit);

MODULE_AUTHOR("SUNPLUS");
MODULE_LICENSE("GPL");
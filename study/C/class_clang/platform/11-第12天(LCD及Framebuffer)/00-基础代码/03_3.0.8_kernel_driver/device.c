
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/platform_device.h>
#include <linux/fb.h>
#include <linux/gfp.h>

#include <mach/irqs.h>
#include <mach/map.h>

#include <plat/fb.h>
#include <plat/devs.h>
#include <plat/cpu.h>
#include <mach/regs-fb.h>
#include <plat/gpio-cfg.h>
#include <linux/gpio.h>
#include <mach/regs-clock.h>


static struct resource s3c_fb_resource[] = {
	[0] = {
		.start = S3C_PA_FB,
		.end   = S3C_PA_FB + SZ_16K - 1,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start = IRQ_LCD_VSYNC,
		.end   = IRQ_LCD_VSYNC,
		.flags = IORESOURCE_IRQ,
	},
	[2] = {
		.start = IRQ_LCD_FIFO,
		.end   = IRQ_LCD_FIFO,
		.flags = IORESOURCE_IRQ,
	},
	[3] = {
		.start = IRQ_LCD_SYSTEM,
		.end   = IRQ_LCD_SYSTEM,
		.flags = IORESOURCE_IRQ,
	},
};

struct platform_device my_s3c_device_fb = {
	.name		  = "s3c-fb",
	.id		  = -1,
	.num_resources	  = ARRAY_SIZE(s3c_fb_resource),
	.resource	  = s3c_fb_resource,
	.dev.dma_mask	  = &my_s3c_device_fb.dev.coherent_dma_mask,
	.dev.coherent_dma_mask = 0xffffffffUL,
};


static struct s3c_fb_pd_win smdkv210_fb_win0 = {
	.win_mode = {
		.left_margin	= 13,
		.right_margin	= 120,
		.upper_margin	= 7,
		.lower_margin	= 5,
		.hsync_len	= 3,
		.vsync_len	= 1,
		.xres		= 800,
		.yres		= 480,
	},
	.max_bpp	= 32,
	.default_bpp	= 32,
};

static void s5pv210_fb_cfg_gpios(unsigned int base, unsigned int nr)
{
	s3c_gpio_cfgrange_nopull(base, nr, S3C_GPIO_SFN(2));

	for (; nr > 0; nr--, base++)
		s5p_gpio_set_drvstr(base, S5P_GPIO_DRVSTR_LV4);
}

void s5pv210_fb_gpio_setup_24bpp(void)
{
	s5pv210_fb_cfg_gpios(S5PV210_GPF0(0), 8);
	s5pv210_fb_cfg_gpios(S5PV210_GPF1(0), 8);
	s5pv210_fb_cfg_gpios(S5PV210_GPF2(0), 8);
	s5pv210_fb_cfg_gpios(S5PV210_GPF3(0), 4);

	/* Set DISPLAY_CONTROL register for Display path selection.
	 *
	 * ouput   |   RGB   |   I80   |   ITU
	 * -----------------------------------
	 *  00     |   MIE   |  FIMD   |  FIMD
	 *  01     | MDNIE   | MDNIE   |  FIMD
	 *  10     |  FIMD   |  FIMD   |  FIMD
	 *  11     |  FIMD   |  FIMD   |  FIMD
	 */
	writel(0x2, S5P_MDNIE_SEL);
}

static struct s3c_fb_platdata smdkv210_lcd0_pdata __initdata = {
	.win[0]		= &smdkv210_fb_win0,
	.default_win = 0,
	.vidcon0	= VIDCON0_VIDOUT_RGB | VIDCON0_PNRMODE_RGB,
	.vidcon1	= VIDCON1_INV_HSYNC | VIDCON1_INV_VSYNC,
	.setup_gpio	= s5pv210_fb_gpio_setup_24bpp,
};


void __init s3c_fb_set_platdata(struct s3c_fb_platdata *pd)
{
	struct s3c_fb_platdata *npd;

	if (!pd) {
		printk(KERN_ERR "%s: no platform data\n", __func__);
		return;
	}

	npd = kmemdup(pd, sizeof(struct s3c_fb_platdata), GFP_KERNEL);
	if (!npd)
		printk(KERN_ERR "%s: no memory for platform data\n", __func__);

	my_s3c_device_fb.dev.platform_data = npd;
}

static int  __init platform_dev_init(void)
{
	int ret = 0;
	ret = platform_device_register(&my_s3c_device_fb);
	if (ret)
		printk("platform_device_register failed\n");
	s3c_fb_set_platdata(&smdkv210_lcd0_pdata);
	return ret;

}

static void  __exit platform_dev_exit(void)
{
	platform_device_unregister(&my_s3c_device_fb);
	printk("platform device unregister\n");
	 return ;
}

module_init(platform_dev_init);
module_exit(platform_dev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("sunplusedu");



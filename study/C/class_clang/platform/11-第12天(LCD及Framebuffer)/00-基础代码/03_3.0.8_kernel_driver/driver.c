/* linux/drivers/video/s3c-fb.c
 *
 * Copyright 2008 Openmoko Inc.
 * Copyright 2008-2010 Simtec Electronics
 *      Ben Dooks <ben@simtec.co.uk>
 *      http://armlinux.simtec.co.uk/
 *
 * Samsung SoC Framebuffer driver
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software FoundatIon.
*/

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/dma-mapping.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/clk.h>
#include <linux/fb.h>
#include <linux/io.h>
#include <linux/uaccess.h>
#include <linux/interrupt.h>
#include <linux/pm_runtime.h>

#include <mach/map.h>
#include <plat/regs-fb-v4.h>
#include <plat/fb.h>

#define cpu_to_le_long _cpu_to_le_long(BITS_PER_LONG)
#define _cpu_to_le_long(x) __cpu_to_le_long(x)
#define __cpu_to_le_long(x) cpu_to_le##x

#define le_long_to_cpu _le_long_to_cpu(BITS_PER_LONG)
#define _le_long_to_cpu(x) __le_long_to_cpu(x)
#define __le_long_to_cpu(x) le##x##_to_cpu


#if BITS_PER_LONG == 64
#define REV_PIXELS_MASK1 0x5555555555555555ul
#define REV_PIXELS_MASK2 0x3333333333333333ul
#define REV_PIXELS_MASK4 0x0f0f0f0f0f0f0f0ful
#else
#define REV_PIXELS_MASK1 0x55555555ul
#define REV_PIXELS_MASK2 0x33333333ul
#define REV_PIXELS_MASK4 0x0f0f0f0ful
#endif


static const u32 cfb_tab8_be[] = {
    0x00000000,0x000000ff,0x0000ff00,0x0000ffff,
    0x00ff0000,0x00ff00ff,0x00ffff00,0x00ffffff,
    0xff000000,0xff0000ff,0xff00ff00,0xff00ffff,
    0xffff0000,0xffff00ff,0xffffff00,0xffffffff
};

static const u32 cfb_tab8_le[] = {
    0x00000000,0xff000000,0x00ff0000,0xffff0000,
    0x0000ff00,0xff00ff00,0x00ffff00,0xffffff00,
    0x000000ff,0xff0000ff,0x00ff00ff,0xffff00ff,
    0x0000ffff,0xff00ffff,0x00ffffff,0xffffffff
};

static const u32 cfb_tab16_be[] = {
    0x00000000, 0x0000ffff, 0xffff0000, 0xffffffff
};

static const u32 cfb_tab16_le[] = {
    0x00000000, 0xffff0000, 0x0000ffff, 0xffffffff
};

static const u32 cfb_tab32[] = {
	0x00000000, 0xffffffff
};

#define FB_WRITEL fb_writel
#define FB_READL  fb_readl

static inline unsigned long rolx(unsigned long word, unsigned int shift, unsigned int x)
{
	return (word << shift) | (word >> (x - shift));
}

static inline unsigned long
comp(unsigned long a, unsigned long b, unsigned long mask)
{
    return ((a ^ b) & mask) ^ b;
}
static inline unsigned long fb_rev_pixels_in_long(unsigned long val,
						  u32 bswapmask)
{
	if (bswapmask & 1)
		val = comp(val >> 1, val << 1, REV_PIXELS_MASK1);
	if (bswapmask & 2)
		val = comp(val >> 2, val << 2, REV_PIXELS_MASK2);
	if (bswapmask & 3)
		val = comp(val >> 4, val << 4, REV_PIXELS_MASK4);
	return val;
}

static inline u32 fb_shifted_pixels_mask_u32(struct fb_info *p, u32 index,
					     u32 bswapmask)
{
	u32 mask;

	if (!bswapmask) {
		mask = FB_SHIFT_HIGH(p, ~(u32)0, index);
	} else {
		mask = 0xff << FB_LEFT_POS(p, 8);
		mask = FB_SHIFT_LOW(p, mask, index & (bswapmask)) & mask;
		mask = FB_SHIFT_HIGH(p, mask, index & ~(bswapmask));
#if defined(__i386__) || defined(__x86_64__)
		/* Shift argument is limited to 0 - 31 on x86 based CPU's */
		if(index + bswapmask < 32)
#endif
			mask |= FB_SHIFT_HIGH(p, ~(u32)0,
					(index + bswapmask) & ~(bswapmask));
	}
	return mask;
}


/* This driver will export a number of framebuffer interfaces depending
 * on the configuration passed in via the platform data. Each fb instance
 * maps to a hardware window. Currently there is no support for runtime
 * setting of the alpha-blending functions that each window has, so only
 * window 0 is actually useful.
 *
 * Window 0 is treated specially, it is used for the basis of the LCD
 * output timings and as the control for the output power-down state.
*/

/* note, the previous use of <mach/regs-fb.h> to get platform specific data
 * has been replaced by using the platform device name to pick the correct
 * configuration data for the system.
*/

#ifdef CONFIG_FB_S3C_DEBUG_REGWRITE
#undef writel
#define writel(v, r) do { \
	printk(KERN_DEBUG "%s: %08x => %p\n", __func__, (unsigned int)v, r); \
	__raw_writel(v, r); } while (0)
#endif /* FB_S3C_DEBUG_REGWRITE */

/* irq_flags bits */
#define S3C_FB_VSYNC_IRQ_EN	0

#define VSYNC_TIMEOUT_MSEC 50

struct s3c_fb;

#define VALID_BPP(x) (1 << ((x) - 1))

#define OSD_BASE(win, variant) ((variant).osd + ((win) * (variant).osd_stride))
#define VIDOSD_A(win, variant) (OSD_BASE(win, variant) + 0x00)
#define VIDOSD_B(win, variant) (OSD_BASE(win, variant) + 0x04)
#define VIDOSD_C(win, variant) (OSD_BASE(win, variant) + 0x08)
#define VIDOSD_D(win, variant) (OSD_BASE(win, variant) + 0x0C)

/**
 * struct s3c_fb_variant - fb variant information
 * @is_2443: Set if S3C2443/S3C2416 style hardware.
 * @nr_windows: The number of windows.
 * @vidtcon: The base for the VIDTCONx registers
 * @wincon: The base for the WINxCON registers.
 * @winmap: The base for the WINxMAP registers.
 * @keycon: The abse for the WxKEYCON registers.
 * @buf_start: Offset of buffer start registers.
 * @buf_size: Offset of buffer size registers.
 * @buf_end: Offset of buffer end registers.
 * @osd: The base for the OSD registers.
 * @palette: Address of palette memory, or 0 if none.
 * @has_prtcon: Set if has PRTCON register.
 * @has_shadowcon: Set if has SHADOWCON register.
 */
struct s3c_fb_variant {
	unsigned int	is_2443:1;
	unsigned short	nr_windows;
	unsigned short	vidtcon;
	unsigned short	wincon;
	unsigned short	winmap;
	unsigned short	keycon;
	unsigned short	buf_start;
	unsigned short	buf_end;
	unsigned short	buf_size;
	unsigned short	osd;
	unsigned short	osd_stride;
	unsigned short	palette[S3C_FB_MAX_WIN];

	unsigned int	has_prtcon:1;
	unsigned int	has_shadowcon:1;
};

/**
 * struct s3c_fb_win_variant
 * @has_osd_c: Set if has OSD C register.
 * @has_osd_d: Set if has OSD D register.
 * @has_osd_alpha: Set if can change alpha transparency for a window.
 * @palette_sz: Size of palette in entries.
 * @palette_16bpp: Set if palette is 16bits wide.
 * @osd_size_off: If != 0, supports setting up OSD for a window; the appropriate
 *                register is located at the given offset from OSD_BASE.
 * @valid_bpp: 1 bit per BPP setting to show valid bits-per-pixel.
 *
 * valid_bpp bit x is set if (x+1)BPP is supported.
 */
struct s3c_fb_win_variant {
	unsigned int	has_osd_c:1;
	unsigned int	has_osd_d:1;
	unsigned int	has_osd_alpha:1;
	unsigned int	palette_16bpp:1;
	unsigned short	osd_size_off;
	unsigned short	palette_sz;
	u32		valid_bpp;
};

/**
 * struct s3c_fb_driverdata - per-device type driver data for init time.
 * @variant: The variant information for this driver.
 * @win: The window information for each window.
 */
struct s3c_fb_driverdata {
	struct s3c_fb_variant	variant;
	struct s3c_fb_win_variant *win[S3C_FB_MAX_WIN];
};

/**
 * struct s3c_fb_palette - palette information
 * @r: Red bitfield.
 * @g: Green bitfield.
 * @b: Blue bitfield.
 * @a: Alpha bitfield.
 */
struct s3c_fb_palette {
	struct fb_bitfield	r;
	struct fb_bitfield	g;
	struct fb_bitfield	b;
	struct fb_bitfield	a;
};

/**
 * struct s3c_fb_win - per window private data for each framebuffer.
 * @windata: The platform data supplied for the window configuration.
 * @parent: The hardware that this window is part of.
 * @fbinfo: Pointer pack to the framebuffer info for this window.
 * @varint: The variant information for this window.
 * @palette_buffer: Buffer/cache to hold palette entries.
 * @pseudo_palette: For use in TRUECOLOUR modes for entries 0..15/
 * @index: The window number of this window.
 * @palette: The bitfields for changing r/g/b into a hardware palette entry.
 */
struct s3c_fb_win {
	struct s3c_fb_pd_win	*windata;
	struct s3c_fb		*parent;
	struct fb_info		*fbinfo;
	struct s3c_fb_palette	 palette;
	struct s3c_fb_win_variant variant;

	u32			*palette_buffer;
	u32			 pseudo_palette[16];
	unsigned int		 index;
};

/**
 * struct s3c_fb_vsync - vsync information
 * @wait:	a queue for processes waiting for vsync
 * @count:	vsync interrupt count
 */
struct s3c_fb_vsync {
	wait_queue_head_t	wait;
	unsigned int		count;
};

/**
 * struct s3c_fb - overall hardware state of the hardware
 * @slock: The spinlock protection for this data sturcture.
 * @dev: The device that we bound to, for printing, etc.
 * @regs_res: The resource we claimed for the IO registers.
 * @bus_clk: The clk (hclk) feeding our interface and possibly pixclk.
 * @regs: The mapped hardware registers.
 * @variant: Variant information for this hardware.
 * @enabled: A bitmask of enabled hardware windows.
 * @pdata: The platform configuration data passed with the device.
 * @windows: The hardware windows that have been claimed.
 * @irq_no: IRQ line number
 * @irq_flags: irq flags
 * @vsync_info: VSYNC-related information (count, queues...)
 */
struct s3c_fb {
	spinlock_t		slock;
	struct device		*dev;
	struct resource		*regs_res;
	struct clk		*bus_clk;
	void __iomem		*regs;
	struct s3c_fb_variant	 variant;

	unsigned char		 enabled;

	struct s3c_fb_platdata	*pdata;
	struct s3c_fb_win	*windows[S3C_FB_MAX_WIN];

	int			 irq_no;
	unsigned long		 irq_flags;
	struct s3c_fb_vsync	 vsync_info;
};

/**
 * s3c_fb_validate_win_bpp - validate the bits-per-pixel for this mode.
 * @win: The device window.
 * @bpp: The bit depth.
 */
static bool s3c_fb_validate_win_bpp(struct s3c_fb_win *win, unsigned int bpp)
{
	return win->variant.valid_bpp & VALID_BPP(bpp);
}

/**
 * s3c_fb_check_var() - framebuffer layer request to verify a given mode.
 * @var: The screen information to verify.
 * @info: The framebuffer device.
 *
 * Framebuffer layer call to verify the given information and allow us to
 * update various information depending on the hardware capabilities.
 */
static int s3c_fb_check_var(struct fb_var_screeninfo *var,
			    struct fb_info *info)
{
	struct s3c_fb_win *win = info->par;
	struct s3c_fb *sfb = win->parent;

	dev_dbg(sfb->dev, "checking parameters\n");

	var->xres_virtual = max(var->xres_virtual, var->xres);
	var->yres_virtual = max(var->yres_virtual, var->yres);

	if (!s3c_fb_validate_win_bpp(win, var->bits_per_pixel)) {
		dev_dbg(sfb->dev, "win %d: unsupported bpp %d\n",
			win->index, var->bits_per_pixel);
		return -EINVAL;
	}

	/* always ensure these are zero, for drop through cases below */
	var->transp.offset = 0;
	var->transp.length = 0;

	switch (var->bits_per_pixel) {
	case 1:
	case 2:
	case 4:
	case 8:
		if (sfb->variant.palette[win->index] != 0) {
			/* non palletised, A:1,R:2,G:3,B:2 mode */
			var->red.offset		= 4;
			var->green.offset	= 2;
			var->blue.offset	= 0;
			var->red.length		= 5;
			var->green.length	= 3;
			var->blue.length	= 2;
			var->transp.offset	= 7;
			var->transp.length	= 1;
		} else {
			var->red.offset	= 0;
			var->red.length	= var->bits_per_pixel;
			var->green	= var->red;
			var->blue	= var->red;
		}
		break;

	case 19:
		/* 666 with one bit alpha/transparency */
		var->transp.offset	= 18;
		var->transp.length	= 1;
	case 18:
		var->bits_per_pixel	= 32;

		/* 666 format */
		var->red.offset		= 12;
		var->green.offset	= 6;
		var->blue.offset	= 0;
		var->red.length		= 6;
		var->green.length	= 6;
		var->blue.length	= 6;
		break;

	case 16:
		/* 16 bpp, 565 format */
		var->red.offset		= 11;
		var->green.offset	= 5;
		var->blue.offset	= 0;
		var->red.length		= 5;
		var->green.length	= 6;
		var->blue.length	= 5;
		break;

	case 32:
	case 28:
	case 25:
		var->transp.length	= var->bits_per_pixel - 24;
		var->transp.offset	= 24;
		/* drop through */
	case 24:
		/* our 24bpp is unpacked, so 32bpp */
		var->bits_per_pixel	= 32;
		var->red.offset		= 16;
		var->red.length		= 8;
		var->green.offset	= 8;
		var->green.length	= 8;
		var->blue.offset	= 0;
		var->blue.length	= 8;
		break;

	default:
		dev_err(sfb->dev, "invalid bpp\n");
	}

	dev_dbg(sfb->dev, "%s: verified parameters\n", __func__);
	return 0;
}

/**
 * s3c_fb_calc_pixclk() - calculate the divider to create the pixel clock.
 * @sfb: The hardware state.
 * @pixclock: The pixel clock wanted, in picoseconds.
 *
 * Given the specified pixel clock, work out the necessary divider to get
 * close to the output frequency.
 */
static int s3c_fb_calc_pixclk(struct s3c_fb *sfb, unsigned int pixclk)
{
	unsigned long clk = clk_get_rate(sfb->bus_clk);
	unsigned long long tmp;
	unsigned int result;

	tmp = (unsigned long long)clk;
	tmp *= pixclk;

	do_div(tmp, 1000000000UL);
	result = (unsigned int)tmp / 1000;

	dev_dbg(sfb->dev, "pixclk=%u, clk=%lu, div=%d (%lu)\n",
		pixclk, clk, result, clk / result);

	return result;
}

/**
 * s3c_fb_align_word() - align pixel count to word boundary
 * @bpp: The number of bits per pixel
 * @pix: The value to be aligned.
 *
 * Align the given pixel count so that it will start on an 32bit word
 * boundary.
 */
static int s3c_fb_align_word(unsigned int bpp, unsigned int pix)
{
	int pix_per_word;

	if (bpp > 16)
		return pix;

	pix_per_word = (8 * 32) / bpp;
	return ALIGN(pix, pix_per_word);
}

/**
 * vidosd_set_size() - set OSD size for a window
 *
 * @win: the window to set OSD size for
 * @size: OSD size register value
 */
static void vidosd_set_size(struct s3c_fb_win *win, u32 size)
{
	struct s3c_fb *sfb = win->parent;

	/* OSD can be set up if osd_size_off != 0 for this window */
	if (win->variant.osd_size_off)
		writel(size, sfb->regs + OSD_BASE(win->index, sfb->variant)
				+ win->variant.osd_size_off);
}

/**
 * vidosd_set_alpha() - set alpha transparency for a window
 *
 * @win: the window to set OSD size for
 * @alpha: alpha register value
 */
static void vidosd_set_alpha(struct s3c_fb_win *win, u32 alpha)
{
	struct s3c_fb *sfb = win->parent;

	if (win->variant.has_osd_alpha)
		writel(alpha, sfb->regs + VIDOSD_C(win->index, sfb->variant));
}

/**
 * shadow_protect_win() - disable updating values from shadow registers at vsync
 *
 * @win: window to protect registers for
 * @protect: 1 to protect (disable updates)
 */
static void shadow_protect_win(struct s3c_fb_win *win, bool protect)
{
	struct s3c_fb *sfb = win->parent;
	u32 reg;

	if (protect) {
		if (sfb->variant.has_prtcon) {
			writel(PRTCON_PROTECT, sfb->regs + PRTCON);
		} else if (sfb->variant.has_shadowcon) {
			reg = readl(sfb->regs + SHADOWCON);
			writel(reg | SHADOWCON_WINx_PROTECT(win->index),
				sfb->regs + SHADOWCON);
		}
	} else {
		if (sfb->variant.has_prtcon) {
			writel(0, sfb->regs + PRTCON);
		} else if (sfb->variant.has_shadowcon) {
			reg = readl(sfb->regs + SHADOWCON);
			writel(reg & ~SHADOWCON_WINx_PROTECT(win->index),
				sfb->regs + SHADOWCON);
		}
	}
}

/**
 * s3c_fb_set_par() - framebuffer request to set new framebuffer state.
 * @info: The framebuffer to change.
 *
 * Framebuffer layer request to set a new mode for the specified framebuffer
 */
static int s3c_fb_set_par(struct fb_info *info)
{
	struct fb_var_screeninfo *var = &info->var;
	struct s3c_fb_win *win = info->par;
	struct s3c_fb *sfb = win->parent;
	void __iomem *regs = sfb->regs;
	void __iomem *buf = regs;
	int win_no = win->index;
	u32 alpha = 0;
	u32 data;
	u32 pagewidth;
	int clkdiv;

	dev_dbg(sfb->dev, "setting framebuffer parameters\n");

	shadow_protect_win(win, 1);

	switch (var->bits_per_pixel) {
	case 32:
	case 24:
	case 16:
	case 12:
		info->fix.visual = FB_VISUAL_TRUECOLOR;
		break;
	case 8:
		if (win->variant.palette_sz >= 256)
			info->fix.visual = FB_VISUAL_PSEUDOCOLOR;
		else
			info->fix.visual = FB_VISUAL_TRUECOLOR;
		break;
	case 1:
		info->fix.visual = FB_VISUAL_MONO01;
		break;
	default:
		info->fix.visual = FB_VISUAL_PSEUDOCOLOR;
		break;
	}

	info->fix.line_length = (var->xres_virtual * var->bits_per_pixel) / 8;

	info->fix.xpanstep = info->var.xres_virtual > info->var.xres ? 1 : 0;
	info->fix.ypanstep = info->var.yres_virtual > info->var.yres ? 1 : 0;

	/* disable the window whilst we update it */
	writel(0, regs + WINCON(win_no));

	/* use platform specified window as the basis for the lcd timings */

	if (win_no == sfb->pdata->default_win) {
		clkdiv = s3c_fb_calc_pixclk(sfb, var->pixclock);

		data = sfb->pdata->vidcon0;
		data &= ~(VIDCON0_CLKVAL_F_MASK | VIDCON0_CLKDIR);

		if (clkdiv > 1)
			data |= VIDCON0_CLKVAL_F(clkdiv-1) | VIDCON0_CLKDIR;
		else
			data &= ~VIDCON0_CLKDIR;	/* 1:1 clock */

		/* write the timing data to the panel */

		if (sfb->variant.is_2443)
			data |= (1 << 5);

		data |= VIDCON0_ENVID | VIDCON0_ENVID_F;
		writel(data, regs + VIDCON0);

		data = VIDTCON0_VBPD(var->upper_margin - 1) |
		       VIDTCON0_VFPD(var->lower_margin - 1) |
		       VIDTCON0_VSPW(var->vsync_len - 1);

		writel(data, regs + sfb->variant.vidtcon);

		data = VIDTCON1_HBPD(var->left_margin - 1) |
		       VIDTCON1_HFPD(var->right_margin - 1) |
		       VIDTCON1_HSPW(var->hsync_len - 1);

		/* VIDTCON1 */
		writel(data, regs + sfb->variant.vidtcon + 4);

		data = VIDTCON2_LINEVAL(var->yres - 1) |
		       VIDTCON2_HOZVAL(var->xres - 1);
		writel(data, regs + sfb->variant.vidtcon + 8);
	}

	/* write the buffer address */

	/* start and end registers stride is 8 */
	buf = regs + win_no * 8;

	writel(info->fix.smem_start, buf + sfb->variant.buf_start);

	data = info->fix.smem_start + info->fix.line_length * var->yres;
	writel(data, buf + sfb->variant.buf_end);

	pagewidth = (var->xres * var->bits_per_pixel) >> 3;
	data = VIDW_BUF_SIZE_OFFSET(info->fix.line_length - pagewidth) |
	       VIDW_BUF_SIZE_PAGEWIDTH(pagewidth);
	writel(data, regs + sfb->variant.buf_size + (win_no * 4));

	/* write 'OSD' registers to control position of framebuffer */

	data = VIDOSDxA_TOPLEFT_X(0) | VIDOSDxA_TOPLEFT_Y(0);
	writel(data, regs + VIDOSD_A(win_no, sfb->variant));

	data = VIDOSDxB_BOTRIGHT_X(s3c_fb_align_word(var->bits_per_pixel,
						     var->xres - 1)) |
	       VIDOSDxB_BOTRIGHT_Y(var->yres - 1);

	writel(data, regs + VIDOSD_B(win_no, sfb->variant));

	data = var->xres * var->yres;

	alpha = VIDISD14C_ALPHA1_R(0xf) |
		VIDISD14C_ALPHA1_G(0xf) |
		VIDISD14C_ALPHA1_B(0xf);

	vidosd_set_alpha(win, alpha);
	vidosd_set_size(win, data);

	/* Enable DMA channel for this window */
	if (sfb->variant.has_shadowcon) {
		data = readl(sfb->regs + SHADOWCON);
		data |= SHADOWCON_CHx_ENABLE(win_no);
		writel(data, sfb->regs + SHADOWCON);
	}

	data = WINCONx_ENWIN;

	/* note, since we have to round up the bits-per-pixel, we end up
	 * relying on the bitfield information for r/g/b/a to work out
	 * exactly which mode of operation is intended. */

	switch (var->bits_per_pixel) {
	case 1:
		data |= WINCON0_BPPMODE_1BPP;
		data |= WINCONx_BITSWP;
		data |= WINCONx_BURSTLEN_4WORD;
		break;
	case 2:
		data |= WINCON0_BPPMODE_2BPP;
		data |= WINCONx_BITSWP;
		data |= WINCONx_BURSTLEN_8WORD;
		break;
	case 4:
		data |= WINCON0_BPPMODE_4BPP;
		data |= WINCONx_BITSWP;
		data |= WINCONx_BURSTLEN_8WORD;
		break;
	case 8:
		if (var->transp.length != 0)
			data |= WINCON1_BPPMODE_8BPP_1232;
		else
			data |= WINCON0_BPPMODE_8BPP_PALETTE;
		data |= WINCONx_BURSTLEN_8WORD;
		data |= WINCONx_BYTSWP;
		break;
	case 16:
		if (var->transp.length != 0)
			data |= WINCON1_BPPMODE_16BPP_A1555;
		else
			data |= WINCON0_BPPMODE_16BPP_565;
		data |= WINCONx_HAWSWP;
		data |= WINCONx_BURSTLEN_16WORD;
		break;
	case 24:
	case 32:
		if (var->red.length == 6) {
			if (var->transp.length != 0)
				data |= WINCON1_BPPMODE_19BPP_A1666;
			else
				data |= WINCON1_BPPMODE_18BPP_666;
		} else if (var->transp.length == 1)
			data |= WINCON1_BPPMODE_25BPP_A1888
				| WINCON1_BLD_PIX;
		else if (var->transp.length == 4)
			data |= WINCON1_BPPMODE_28BPP_A4888
				| WINCON1_BLD_PIX | WINCON1_ALPHA_SEL;
		else
			data |= WINCON0_BPPMODE_24BPP_888;

		data |= WINCONx_WSWP;
		data |= WINCONx_BURSTLEN_16WORD;
		break;
	}

	/* Enable the colour keying for the window below this one */
	if (win_no > 0) {
		u32 keycon0_data = 0, keycon1_data = 0;
		void __iomem *keycon = regs + sfb->variant.keycon;

		keycon0_data = ~(WxKEYCON0_KEYBL_EN |
				WxKEYCON0_KEYEN_F |
				WxKEYCON0_DIRCON) | WxKEYCON0_COMPKEY(0);

		keycon1_data = WxKEYCON1_COLVAL(0xffffff);

		keycon += (win_no - 1) * 8;

		writel(keycon0_data, keycon + WKEYCON0);
		writel(keycon1_data, keycon + WKEYCON1);
	}

	writel(data, regs + sfb->variant.wincon + (win_no * 4));
	writel(0x0, regs + sfb->variant.winmap + (win_no * 4));

	shadow_protect_win(win, 0);

	return 0;
}

/**
 * s3c_fb_update_palette() - set or schedule a palette update.
 * @sfb: The hardware information.
 * @win: The window being updated.
 * @reg: The palette index being changed.
 * @value: The computed palette value.
 *
 * Change the value of a palette register, either by directly writing to
 * the palette (this requires the palette RAM to be disconnected from the
 * hardware whilst this is in progress) or schedule the update for later.
 *
 * At the moment, since we have no VSYNC interrupt support, we simply set
 * the palette entry directly.
 */
static void s3c_fb_update_palette(struct s3c_fb *sfb,
				  struct s3c_fb_win *win,
				  unsigned int reg,
				  u32 value)
{
	void __iomem *palreg;
	u32 palcon;

	palreg = sfb->regs + sfb->variant.palette[win->index];

	dev_dbg(sfb->dev, "%s: win %d, reg %d (%p): %08x\n",
		__func__, win->index, reg, palreg, value);

	win->palette_buffer[reg] = value;

	palcon = readl(sfb->regs + WPALCON);
	writel(palcon | WPALCON_PAL_UPDATE, sfb->regs + WPALCON);

	if (win->variant.palette_16bpp)
		writew(value, palreg + (reg * 2));
	else
		writel(value, palreg + (reg * 4));

	writel(palcon, sfb->regs + WPALCON);
}

static inline unsigned int chan_to_field(unsigned int chan,
					 struct fb_bitfield *bf)
{
	chan &= 0xffff;
	chan >>= 16 - bf->length;
	return chan << bf->offset;
}

/**
 * s3c_fb_setcolreg() - framebuffer layer request to change palette.
 * @regno: The palette index to change.
 * @red: The red field for the palette data.
 * @green: The green field for the palette data.
 * @blue: The blue field for the palette data.
 * @trans: The transparency (alpha) field for the palette data.
 * @info: The framebuffer being changed.
 */
static int s3c_fb_setcolreg(unsigned regno,
			    unsigned red, unsigned green, unsigned blue,
			    unsigned transp, struct fb_info *info)
{
	struct s3c_fb_win *win = info->par;
	struct s3c_fb *sfb = win->parent;
	unsigned int val;

	dev_dbg(sfb->dev, "%s: win %d: %d => rgb=%d/%d/%d\n",
		__func__, win->index, regno, red, green, blue);

	switch (info->fix.visual) {
	case FB_VISUAL_TRUECOLOR:
		/* true-colour, use pseudo-palette */

		if (regno < 16) {
			u32 *pal = info->pseudo_palette;

			val  = chan_to_field(red,   &info->var.red);
			val |= chan_to_field(green, &info->var.green);
			val |= chan_to_field(blue,  &info->var.blue);

			pal[regno] = val;
		}
		break;

	case FB_VISUAL_PSEUDOCOLOR:
		if (regno < win->variant.palette_sz) {
			val  = chan_to_field(red, &win->palette.r);
			val |= chan_to_field(green, &win->palette.g);
			val |= chan_to_field(blue, &win->palette.b);

			s3c_fb_update_palette(sfb, win, regno, val);
		}

		break;

	default:
		return 1;	/* unknown type */
	}

	return 0;
}

/**
 * s3c_fb_enable() - Set the state of the main LCD output
 * @sfb: The main framebuffer state.
 * @enable: The state to set.
 */
static void s3c_fb_enable(struct s3c_fb *sfb, int enable)
{
	u32 vidcon0 = readl(sfb->regs + VIDCON0);

	if (enable)
		vidcon0 |= VIDCON0_ENVID | VIDCON0_ENVID_F;
	else {
		/* see the note in the framebuffer datasheet about
		 * why you cannot take both of these bits down at the
		 * same time. */

		if (!(vidcon0 & VIDCON0_ENVID))
			return;

		vidcon0 |= VIDCON0_ENVID;
		vidcon0 &= ~VIDCON0_ENVID_F;
	}

	writel(vidcon0, sfb->regs + VIDCON0);
}

/**
 * s3c_fb_blank() - blank or unblank the given window
 * @blank_mode: The blank state from FB_BLANK_*
 * @info: The framebuffer to blank.
 *
 * Framebuffer layer request to change the power state.
 */
static int s3c_fb_blank(int blank_mode, struct fb_info *info)
{
	struct s3c_fb_win *win = info->par;
	struct s3c_fb *sfb = win->parent;
	unsigned int index = win->index;
	u32 wincon;

	dev_dbg(sfb->dev, "blank mode %d\n", blank_mode);

	wincon = readl(sfb->regs + sfb->variant.wincon + (index * 4));

	switch (blank_mode) {
	case FB_BLANK_POWERDOWN:
		wincon &= ~WINCONx_ENWIN;
		sfb->enabled &= ~(1 << index);
		/* fall through to FB_BLANK_NORMAL */

	case FB_BLANK_NORMAL:
		/* disable the DMA and display 0x0 (black) */
		writel(WINxMAP_MAP | WINxMAP_MAP_COLOUR(0x0),
		       sfb->regs + sfb->variant.winmap + (index * 4));
		break;

	case FB_BLANK_UNBLANK:
		writel(0x0, sfb->regs + sfb->variant.winmap + (index * 4));
		wincon |= WINCONx_ENWIN;
		sfb->enabled |= (1 << index);
		break;

	case FB_BLANK_VSYNC_SUSPEND:
	case FB_BLANK_HSYNC_SUSPEND:
	default:
		return 1;
	}

	writel(wincon, sfb->regs + sfb->variant.wincon + (index * 4));

	/* Check the enabled state to see if we need to be running the
	 * main LCD interface, as if there are no active windows then
	 * it is highly likely that we also do not need to output
	 * anything.
	 */

	/* We could do something like the following code, but the current
	 * system of using framebuffer events means that we cannot make
	 * the distinction between just window 0 being inactive and all
	 * the windows being down.
	 *
	 * s3c_fb_enable(sfb, sfb->enabled ? 1 : 0);
	*/

	/* we're stuck with this until we can do something about overriding
	 * the power control using the blanking event for a single fb.
	 */
	if (index == sfb->pdata->default_win)
		s3c_fb_enable(sfb, blank_mode != FB_BLANK_POWERDOWN ? 1 : 0);

	return 0;
}

/**
 * s3c_fb_pan_display() - Pan the display.
 *
 * Note that the offsets can be written to the device at any time, as their
 * values are latched at each vsync automatically. This also means that only
 * the last call to this function will have any effect on next vsync, but
 * there is no need to sleep waiting for it to prevent tearing.
 *
 * @var: The screen information to verify.
 * @info: The framebuffer device.
 */
static int s3c_fb_pan_display(struct fb_var_screeninfo *var,
			      struct fb_info *info)
{
	struct s3c_fb_win *win	= info->par;
	struct s3c_fb *sfb	= win->parent;
	void __iomem *buf	= sfb->regs + win->index * 8;
	unsigned int start_boff, end_boff;

	/* Offset in bytes to the start of the displayed area */
	start_boff = var->yoffset * info->fix.line_length;
	/* X offset depends on the current bpp */
	if (info->var.bits_per_pixel >= 8) {
		start_boff += var->xoffset * (info->var.bits_per_pixel >> 3);
	} else {
		switch (info->var.bits_per_pixel) {
		case 4:
			start_boff += var->xoffset >> 1;
			break;
		case 2:
			start_boff += var->xoffset >> 2;
			break;
		case 1:
			start_boff += var->xoffset >> 3;
			break;
		default:
			dev_err(sfb->dev, "invalid bpp\n");
			return -EINVAL;
		}
	}
	/* Offset in bytes to the end of the displayed area */
	end_boff = start_boff + var->yres * info->fix.line_length;

	/* Temporarily turn off per-vsync update from shadow registers until
	 * both start and end addresses are updated to prevent corruption */
	shadow_protect_win(win, 1);

	writel(info->fix.smem_start + start_boff, buf + sfb->variant.buf_start);
	writel(info->fix.smem_start + end_boff, buf + sfb->variant.buf_end);

	shadow_protect_win(win, 0);

	return 0;
}

/**
 * s3c_fb_enable_irq() - enable framebuffer interrupts
 * @sfb: main hardware state
 */
static void s3c_fb_enable_irq(struct s3c_fb *sfb)
{
	void __iomem *regs = sfb->regs;
	u32 irq_ctrl_reg;

	if (!test_and_set_bit(S3C_FB_VSYNC_IRQ_EN, &sfb->irq_flags)) {
		/* IRQ disabled, enable it */
		irq_ctrl_reg = readl(regs + VIDINTCON0);

		irq_ctrl_reg |= VIDINTCON0_INT_ENABLE;
		irq_ctrl_reg |= VIDINTCON0_INT_FRAME;

		irq_ctrl_reg &= ~VIDINTCON0_FRAMESEL0_MASK;
		irq_ctrl_reg |= VIDINTCON0_FRAMESEL0_VSYNC;
		irq_ctrl_reg &= ~VIDINTCON0_FRAMESEL1_MASK;
		irq_ctrl_reg |= VIDINTCON0_FRAMESEL1_NONE;

		writel(irq_ctrl_reg, regs + VIDINTCON0);
	}
}

/**
 * s3c_fb_disable_irq() - disable framebuffer interrupts
 * @sfb: main hardware state
 */
static void s3c_fb_disable_irq(struct s3c_fb *sfb)
{
	void __iomem *regs = sfb->regs;
	u32 irq_ctrl_reg;

	if (test_and_clear_bit(S3C_FB_VSYNC_IRQ_EN, &sfb->irq_flags)) {
		/* IRQ enabled, disable it */
		irq_ctrl_reg = readl(regs + VIDINTCON0);

		irq_ctrl_reg &= ~VIDINTCON0_INT_FRAME;
		irq_ctrl_reg &= ~VIDINTCON0_INT_ENABLE;

		writel(irq_ctrl_reg, regs + VIDINTCON0);
	}
}

static irqreturn_t s3c_fb_irq(int irq, void *dev_id)
{
	struct s3c_fb *sfb = dev_id;
	void __iomem  *regs = sfb->regs;
	u32 irq_sts_reg;

	spin_lock(&sfb->slock);

	irq_sts_reg = readl(regs + VIDINTCON1);

	if (irq_sts_reg & VIDINTCON1_INT_FRAME) {

		/* VSYNC interrupt, accept it */
		writel(VIDINTCON1_INT_FRAME, regs + VIDINTCON1);

		sfb->vsync_info.count++;
		wake_up_interruptible(&sfb->vsync_info.wait);
	}

	/* We only support waiting for VSYNC for now, so it's safe
	 * to always disable irqs here.
	 */
	s3c_fb_disable_irq(sfb);

	spin_unlock(&sfb->slock);
	return IRQ_HANDLED;
}

/**
 * s3c_fb_wait_for_vsync() - sleep until next VSYNC interrupt or timeout
 * @sfb: main hardware state
 * @crtc: head index.
 */
static int s3c_fb_wait_for_vsync(struct s3c_fb *sfb, u32 crtc)
{
	unsigned long count;
	int ret;

	if (crtc != 0)
		return -ENODEV;

	count = sfb->vsync_info.count;
	s3c_fb_enable_irq(sfb);
	ret = wait_event_interruptible_timeout(sfb->vsync_info.wait,
				       count != sfb->vsync_info.count,
				       msecs_to_jiffies(VSYNC_TIMEOUT_MSEC));
	if (ret == 0)
		return -ETIMEDOUT;

	return 0;
}

static int s3c_fb_ioctl(struct fb_info *info, unsigned int cmd,
			unsigned long arg)
{
	struct s3c_fb_win *win = info->par;
	struct s3c_fb *sfb = win->parent;
	int ret;
	u32 crtc;

	switch (cmd) {
	case FBIO_WAITFORVSYNC:
		if (get_user(crtc, (u32 __user *)arg)) {
			ret = -EFAULT;
			break;
		}

		ret = s3c_fb_wait_for_vsync(sfb, crtc);
		break;
	default:
		ret = -ENOTTY;
	}

	return ret;
}

static int s3c_fb_open(struct fb_info *info, int user)
{
	struct s3c_fb_win *win = info->par;
	struct s3c_fb *sfb = win->parent;

	pm_runtime_get_sync(sfb->dev);

	return 0;
}

static int s3c_fb_release(struct fb_info *info, int user)
{
	struct s3c_fb_win *win = info->par;
	struct s3c_fb *sfb = win->parent;

	pm_runtime_put_sync(sfb->dev);

	return 0;
}
static inline u32 fb_compute_bswapmask(struct fb_info *info)
{
	u32 bswapmask = 0;
	unsigned bpp = info->var.bits_per_pixel;

	if ((bpp < 8) && (info->var.nonstd & FB_NONSTD_REV_PIX_IN_B)) {
		/*
		 * Reversed order of pixel layout in bytes
		 * works only for 1, 2 and 4 bpp
		 */
		bswapmask = 7 - bpp + 1;
	}
	return bswapmask;
}
#define fb_shifted_pixels_mask_long(p, i, b) FB_SHIFT_HIGH((p), ~0UL, (i))

static void
bitcpy_rev(struct fb_info *p, unsigned long __iomem *dst, int dst_idx,
		const unsigned long __iomem *src, int src_idx, int bits,
		unsigned n, u32 bswapmask)
{
	unsigned long first, last;
	int shift;

	dst += (n-1)/bits;
	src += (n-1)/bits;
	if ((n-1) % bits) {
		dst_idx += (n-1) % bits;
		dst += dst_idx >> (ffs(bits) - 1);
		dst_idx &= bits - 1;
		src_idx += (n-1) % bits;
		src += src_idx >> (ffs(bits) - 1);
		src_idx &= bits - 1;
	}

	shift = dst_idx-src_idx;

	first = fb_shifted_pixels_mask_long(p, bits - 1 - dst_idx, bswapmask);
	last = ~fb_shifted_pixels_mask_long(p, bits - 1 - ((dst_idx-n) % bits),
					    bswapmask);

	if (!shift) {
		// Same alignment for source and dest

		if ((unsigned long)dst_idx+1 >= n) {
			// Single word
			if (last)
				first &= last;
			FB_WRITEL( comp( FB_READL(src), FB_READL(dst), first), dst);
		} else {
			// Multiple destination words

			// Leading bits
			if (first != ~0UL) {
				FB_WRITEL( comp( FB_READL(src), FB_READL(dst), first), dst);
				dst--;
				src--;
				n -= dst_idx+1;
			}

			// Main chunk
			n /= bits;
			while (n >= 8) {
				FB_WRITEL(FB_READL(src--), dst--);
				FB_WRITEL(FB_READL(src--), dst--);
				FB_WRITEL(FB_READL(src--), dst--);
				FB_WRITEL(FB_READL(src--), dst--);
				FB_WRITEL(FB_READL(src--), dst--);
				FB_WRITEL(FB_READL(src--), dst--);
				FB_WRITEL(FB_READL(src--), dst--);
				FB_WRITEL(FB_READL(src--), dst--);
				n -= 8;
			}
			while (n--)
				FB_WRITEL(FB_READL(src--), dst--);

			// Trailing bits
			if (last)
				FB_WRITEL( comp( FB_READL(src), FB_READL(dst), last), dst);
		}
	} else {
		// Different alignment for source and dest
		unsigned long d0, d1;
		int m;

		int const left = -shift & (bits-1);
		int const right = shift & (bits-1);
		bswapmask &= shift;

		if ((unsigned long)dst_idx+1 >= n) {
			// Single destination word
			if (last)
				first &= last;
			d0 = FB_READL(src);
			if (shift < 0) {
				// Single source word
				d0 <<= left;
			} else if (1+(unsigned long)src_idx >= n) {
				// Single source word
				d0 >>= right;
			} else {
				// 2 source words
				d1 = FB_READL(src - 1);
				d1 = fb_rev_pixels_in_long(d1, bswapmask);
				d0 = d0>>right | d1<<left;
			}
			d0 = fb_rev_pixels_in_long(d0, bswapmask);
			FB_WRITEL(comp(d0, FB_READL(dst), first), dst);
		} else {
			// Multiple destination words
			/** We must always remember the last value read, because in case
			SRC and DST overlap bitwise (e.g. when moving just one pixel in
			1bpp), we always collect one full long for DST and that might
			overlap with the current long from SRC. We store this value in
			'd0'. */

			d0 = FB_READL(src--);
			d0 = fb_rev_pixels_in_long(d0, bswapmask);
			// Leading bits
			if (shift < 0) {
				// Single source word
				d1 = d0;
				d0 <<= left;
			} else {
				// 2 source words
				d1 = FB_READL(src--);
				d1 = fb_rev_pixels_in_long(d1, bswapmask);
				d0 = d0>>right | d1<<left;
			}
			d0 = fb_rev_pixels_in_long(d0, bswapmask);
			FB_WRITEL(comp(d0, FB_READL(dst), first), dst);
			d0 = d1;
			dst--;
			n -= dst_idx+1;

			// Main chunk
			m = n % bits;
			n /= bits;
			while ((n >= 4) && !bswapmask) {
				d1 = FB_READL(src--);
				FB_WRITEL(d0 >> right | d1 << left, dst--);
				d0 = d1;
				d1 = FB_READL(src--);
				FB_WRITEL(d0 >> right | d1 << left, dst--);
				d0 = d1;
				d1 = FB_READL(src--);
				FB_WRITEL(d0 >> right | d1 << left, dst--);
				d0 = d1;
				d1 = FB_READL(src--);
				FB_WRITEL(d0 >> right | d1 << left, dst--);
				d0 = d1;
				n -= 4;
			}
			while (n--) {
				d1 = FB_READL(src--);
				d1 = fb_rev_pixels_in_long(d1, bswapmask);
				d0 = d0 >> right | d1 << left;
				d0 = fb_rev_pixels_in_long(d0, bswapmask);
				FB_WRITEL(d0, dst--);
				d0 = d1;
			}

			// Trailing bits
			if (last) {
				if (m <= left) {
					// Single source word
					d0 >>= right;
				} else {
					// 2 source words
					d1 = FB_READL(src);
					d1 = fb_rev_pixels_in_long(d1,
								bswapmask);
					d0 = d0>>right | d1<<left;
				}
				d0 = fb_rev_pixels_in_long(d0, bswapmask);
				FB_WRITEL(comp(d0, FB_READL(dst), last), dst);
			}
		}
	}
}

static void
bitcpy(struct fb_info *p, unsigned long __iomem *dst, int dst_idx,
		const unsigned long __iomem *src, int src_idx, int bits,
		unsigned n, u32 bswapmask)
{
	unsigned long first, last;
	int const shift = dst_idx-src_idx;
	int left, right;

	first = fb_shifted_pixels_mask_long(p, dst_idx, bswapmask);
	last = ~fb_shifted_pixels_mask_long(p, (dst_idx+n) % bits, bswapmask);

	if (!shift) {
		// Same alignment for source and dest

		if (dst_idx+n <= bits) {
			// Single word
			if (last)
				first &= last;
			FB_WRITEL( comp( FB_READL(src), FB_READL(dst), first), dst);
		} else {
			// Multiple destination words

			// Leading bits
			if (first != ~0UL) {
				FB_WRITEL( comp( FB_READL(src), FB_READL(dst), first), dst);
				dst++;
				src++;
				n -= bits - dst_idx;
			}

			// Main chunk
			n /= bits;
			while (n >= 8) {
				FB_WRITEL(FB_READL(src++), dst++);
				FB_WRITEL(FB_READL(src++), dst++);
				FB_WRITEL(FB_READL(src++), dst++);
				FB_WRITEL(FB_READL(src++), dst++);
				FB_WRITEL(FB_READL(src++), dst++);
				FB_WRITEL(FB_READL(src++), dst++);
				FB_WRITEL(FB_READL(src++), dst++);
				FB_WRITEL(FB_READL(src++), dst++);
				n -= 8;
			}
			while (n--)
				FB_WRITEL(FB_READL(src++), dst++);

			// Trailing bits
			if (last)
				FB_WRITEL( comp( FB_READL(src), FB_READL(dst), last), dst);
		}
	} else {
		/* Different alignment for source and dest */
		unsigned long d0, d1;
		int m;

		right = shift & (bits - 1);
		left = -shift & (bits - 1);
		bswapmask &= shift;

		if (dst_idx+n <= bits) {
			// Single destination word
			if (last)
				first &= last;
			d0 = FB_READL(src);
			d0 = fb_rev_pixels_in_long(d0, bswapmask);
			if (shift > 0) {
				// Single source word
				d0 >>= right;
			} else if (src_idx+n <= bits) {
				// Single source word
				d0 <<= left;
			} else {
				// 2 source words
				d1 = FB_READL(src + 1);
				d1 = fb_rev_pixels_in_long(d1, bswapmask);
				d0 = d0<<left | d1>>right;
			}
			d0 = fb_rev_pixels_in_long(d0, bswapmask);
			FB_WRITEL(comp(d0, FB_READL(dst), first), dst);
		} else {
			// Multiple destination words
			/** We must always remember the last value read, because in case
			SRC and DST overlap bitwise (e.g. when moving just one pixel in
			1bpp), we always collect one full long for DST and that might
			overlap with the current long from SRC. We store this value in
			'd0'. */
			d0 = FB_READL(src++);
			d0 = fb_rev_pixels_in_long(d0, bswapmask);
			// Leading bits
			if (shift > 0) {
				// Single source word
				d1 = d0;
				d0 >>= right;
				dst++;
				n -= bits - dst_idx;
			} else {
				// 2 source words
				d1 = FB_READL(src++);
				d1 = fb_rev_pixels_in_long(d1, bswapmask);

				d0 = d0<<left | d1>>right;
				dst++;
				n -= bits - dst_idx;
			}
			d0 = fb_rev_pixels_in_long(d0, bswapmask);
			FB_WRITEL(comp(d0, FB_READL(dst), first), dst);
			d0 = d1;

			// Main chunk
			m = n % bits;
			n /= bits;
			while ((n >= 4) && !bswapmask) {
				d1 = FB_READL(src++);
				FB_WRITEL(d0 << left | d1 >> right, dst++);
				d0 = d1;
				d1 = FB_READL(src++);
				FB_WRITEL(d0 << left | d1 >> right, dst++);
				d0 = d1;
				d1 = FB_READL(src++);
				FB_WRITEL(d0 << left | d1 >> right, dst++);
				d0 = d1;
				d1 = FB_READL(src++);
				FB_WRITEL(d0 << left | d1 >> right, dst++);
				d0 = d1;
				n -= 4;
			}
			while (n--) {
				d1 = FB_READL(src++);
				d1 = fb_rev_pixels_in_long(d1, bswapmask);
				d0 = d0 << left | d1 >> right;
				d0 = fb_rev_pixels_in_long(d0, bswapmask);
				FB_WRITEL(d0, dst++);
				d0 = d1;
			}

			// Trailing bits
			if (last) {
				if (m <= right) {
					// Single source word
					d0 <<= left;
				} else {
					// 2 source words
					d1 = FB_READL(src);
					d1 = fb_rev_pixels_in_long(d1,
								bswapmask);
					d0 = d0<<left | d1>>right;
				}
				d0 = fb_rev_pixels_in_long(d0, bswapmask);
				FB_WRITEL(comp(d0, FB_READL(dst), last), dst);
			}
		}
	}
}


static void my_cfb_copyarea(struct fb_info *p, const struct fb_copyarea *area)
{
	u32 dx = area->dx, dy = area->dy, sx = area->sx, sy = area->sy;
	u32 height = area->height, width = area->width;
	unsigned long const bits_per_line = p->fix.line_length*8u;
	unsigned long __iomem *dst = NULL, *src = NULL;
	int bits = BITS_PER_LONG, bytes = bits >> 3;
	int dst_idx = 0, src_idx = 0, rev_copy = 0;
	u32 bswapmask = fb_compute_bswapmask(p);

	if (p->state != FBINFO_STATE_RUNNING)
		return;

	/* if the beginning of the target area might overlap with the end of
	the source area, be have to copy the area reverse. */
	if ((dy == sy && dx > sx) || (dy > sy)) {
		dy += height;
		sy += height;
		rev_copy = 1;
	}

	// split the base of the framebuffer into a long-aligned address and the
	// index of the first bit
	dst = src = (unsigned long __iomem *)((unsigned long)p->screen_base & ~(bytes-1));
	dst_idx = src_idx = 8*((unsigned long)p->screen_base & (bytes-1));
	// add offset of source and target area
	dst_idx += dy*bits_per_line + dx*p->var.bits_per_pixel;
	src_idx += sy*bits_per_line + sx*p->var.bits_per_pixel;

	if (p->fbops->fb_sync)
		p->fbops->fb_sync(p);

	if (rev_copy) {
		while (height--) {
			dst_idx -= bits_per_line;
			src_idx -= bits_per_line;
			dst += dst_idx >> (ffs(bits) - 1);
			dst_idx &= (bytes - 1);
			src += src_idx >> (ffs(bits) - 1);
			src_idx &= (bytes - 1);
			bitcpy_rev(p, dst, dst_idx, src, src_idx, bits,
				width*p->var.bits_per_pixel, bswapmask);
		}
	} else {
		while (height--) {
			dst += dst_idx >> (ffs(bits) - 1);
			dst_idx &= (bytes - 1);
			src += src_idx >> (ffs(bits) - 1);
			src_idx &= (bytes - 1);
			bitcpy(p, dst, dst_idx, src, src_idx, bits,
				width*p->var.bits_per_pixel, bswapmask);
			dst_idx += bits_per_line;
			src_idx += bits_per_line;
		}
	}
}


static inline void color_imageblit(const struct fb_image *image, 
				   struct fb_info *p, u8 __iomem *dst1, 
				   u32 start_index,
				   u32 pitch_index)
{
	/* Draw the penguin */
	u32 __iomem *dst, *dst2;
	u32 color = 0, val, shift;
	int i, n, bpp = p->var.bits_per_pixel;
	u32 null_bits = 32 - bpp;
	u32 *palette = (u32 *) p->pseudo_palette;
	const u8 *src = image->data;
	u32 bswapmask = fb_compute_bswapmask(p);

	dst2 = (u32 __iomem *) dst1;
	for (i = image->height; i--; ) {
		n = image->width;
		dst = (u32 __iomem *) dst1;
		shift = 0;
		val = 0;
		
		if (start_index) {
			u32 start_mask = ~fb_shifted_pixels_mask_u32(p,
						start_index, bswapmask);
			val = FB_READL(dst) & start_mask;
			shift = start_index;
		}
		while (n--) {
			if (p->fix.visual == FB_VISUAL_TRUECOLOR ||
			    p->fix.visual == FB_VISUAL_DIRECTCOLOR )
				color = palette[*src];
			else
				color = *src;
			color <<= FB_LEFT_POS(p, bpp);
			val |= FB_SHIFT_HIGH(p, color, shift ^ bswapmask);
			if (shift >= null_bits) {
				FB_WRITEL(val, dst++);
	
				val = (shift == null_bits) ? 0 : 
					FB_SHIFT_LOW(p, color, 32 - shift);
			}
			shift += bpp;
			shift &= (32 - 1);
			src++;
		}
		if (shift) {
			u32 end_mask = fb_shifted_pixels_mask_u32(p, shift,
						bswapmask);

			FB_WRITEL((FB_READL(dst) & end_mask) | val, dst);
		}
		dst1 += p->fix.line_length;
		if (pitch_index) {
			dst2 += p->fix.line_length;
			dst1 = (u8 __iomem *)((long __force)dst2 & ~(sizeof(u32) - 1));

			start_index += pitch_index;
			start_index &= 32 - 1;
		}
	}
}

static inline void slow_imageblit(const struct fb_image *image, struct fb_info *p, 
				  u8 __iomem *dst1, u32 fgcolor,
				  u32 bgcolor, 
				  u32 start_index,
				  u32 pitch_index)
{
	u32 shift, color = 0, bpp = p->var.bits_per_pixel;
	u32 __iomem *dst, *dst2;
	u32 val, pitch = p->fix.line_length;
	u32 null_bits = 32 - bpp;
	u32 spitch = (image->width+7)/8;
	const u8 *src = image->data, *s;
	u32 i, j, l;
	u32 bswapmask = fb_compute_bswapmask(p);

	dst2 = (u32 __iomem *) dst1;
	fgcolor <<= FB_LEFT_POS(p, bpp);
	bgcolor <<= FB_LEFT_POS(p, bpp);

	for (i = image->height; i--; ) {
		shift = val = 0;
		l = 8;
		j = image->width;
		dst = (u32 __iomem *) dst1;
		s = src;

		/* write leading bits */
		if (start_index) {
			u32 start_mask = ~fb_shifted_pixels_mask_u32(p,
						start_index, bswapmask);
			val = FB_READL(dst) & start_mask;
			shift = start_index;
		}

		while (j--) {
			l--;
			color = (*s & (1 << l)) ? fgcolor : bgcolor;
			val |= FB_SHIFT_HIGH(p, color, shift ^ bswapmask);
			
			/* Did the bitshift spill bits to the next long? */
			if (shift >= null_bits) {
				FB_WRITEL(val, dst++);
				val = (shift == null_bits) ? 0 :
					FB_SHIFT_LOW(p, color, 32 - shift);
			}
			shift += bpp;
			shift &= (32 - 1);
			if (!l) { l = 8; s++; };
		}

		/* write trailing bits */
 		if (shift) {
			u32 end_mask = fb_shifted_pixels_mask_u32(p, shift,
						bswapmask);

			FB_WRITEL((FB_READL(dst) & end_mask) | val, dst);
		}
		
		dst1 += pitch;
		src += spitch;	
		if (pitch_index) {
			dst2 += pitch;
			dst1 = (u8 __iomem *)((long __force)dst2 & ~(sizeof(u32) - 1));
			start_index += pitch_index;
			start_index &= 32 - 1;
		}
		
	}
}

/*
 * fast_imageblit - optimized monochrome color expansion
 *
 * Only if:  bits_per_pixel == 8, 16, or 32
 *           image->width is divisible by pixel/dword (ppw);
 *           fix->line_legth is divisible by 4;
 *           beginning and end of a scanline is dword aligned
 */
static inline void fast_imageblit(const struct fb_image *image, struct fb_info *p, 
				  u8 __iomem *dst1, u32 fgcolor, 
				  u32 bgcolor) 
{
	u32 fgx = fgcolor, bgx = bgcolor, bpp = p->var.bits_per_pixel;
	u32 ppw = 32/bpp, spitch = (image->width + 7)/8;
	u32 bit_mask, end_mask, eorx, shift;
	const char *s = image->data, *src;
	u32 __iomem *dst;
	const u32 *tab = NULL;
	int i, j, k;

	switch (bpp) {
	case 8:
		tab = fb_be_math(p) ? cfb_tab8_be : cfb_tab8_le;
		break;
	case 16:
		tab = fb_be_math(p) ? cfb_tab16_be : cfb_tab16_le;
		break;
	case 32:
	default:
		tab = cfb_tab32;
		break;
	}

	for (i = ppw-1; i--; ) {
		fgx <<= bpp;
		bgx <<= bpp;
		fgx |= fgcolor;
		bgx |= bgcolor;
	}
	
	bit_mask = (1 << ppw) - 1;
	eorx = fgx ^ bgx;
	k = image->width/ppw;

	for (i = image->height; i--; ) {
		dst = (u32 __iomem *) dst1, shift = 8; src = s;
		
		for (j = k; j--; ) {
			shift -= ppw;
			end_mask = tab[(*src >> shift) & bit_mask];
			FB_WRITEL((end_mask & eorx)^bgx, dst++);
			if (!shift) { shift = 8; src++; }		
		}
		dst1 += p->fix.line_length;
		s += spitch;
	}
}	
	

static void my_cfb_imageblit(struct fb_info *p, const struct fb_image *image)
{
	u32 fgcolor, bgcolor, start_index, bitstart, pitch_index = 0;
	u32 bpl = sizeof(u32), bpp = p->var.bits_per_pixel;
	u32 width = image->width;
	u32 dx = image->dx, dy = image->dy;
	u8 __iomem *dst1;

	if (p->state != FBINFO_STATE_RUNNING)
		return;

	bitstart = (dy * p->fix.line_length * 8) + (dx * bpp);
	start_index = bitstart & (32 - 1);
	pitch_index = (p->fix.line_length & (bpl - 1)) * 8;

	bitstart /= 8;
	bitstart &= ~(bpl - 1);
	dst1 = p->screen_base + bitstart;

	if (p->fbops->fb_sync)
		p->fbops->fb_sync(p);

	if (image->depth == 1) {
		if (p->fix.visual == FB_VISUAL_TRUECOLOR ||
		    p->fix.visual == FB_VISUAL_DIRECTCOLOR) {
			fgcolor = ((u32*)(p->pseudo_palette))[image->fg_color];
			bgcolor = ((u32*)(p->pseudo_palette))[image->bg_color];
		} else {
			fgcolor = image->fg_color;
			bgcolor = image->bg_color;
		}	
		
		if (32 % bpp == 0 && !start_index && !pitch_index && 
		    ((width & (32/bpp-1)) == 0) &&
		    bpp >= 8 && bpp <= 32) 			
			fast_imageblit(image, p, dst1, fgcolor, bgcolor);
		else 
			slow_imageblit(image, p, dst1, fgcolor, bgcolor,
					start_index, pitch_index);
	} else
		color_imageblit(image, p, dst1, start_index, pitch_index);
}


#if BITS_PER_LONG == 64
static inline unsigned long
pixel_to_pat( u32 bpp, u32 pixel)
{
	switch (bpp) {
	case 1:
		return 0xfffffffffffffffful*pixel;
	case 2:
		return 0x5555555555555555ul*pixel;
	case 4:
		return 0x1111111111111111ul*pixel;
	case 8:
		return 0x0101010101010101ul*pixel;
	case 12:
		return 0x1001001001001001ul*pixel;
	case 16:
		return 0x0001000100010001ul*pixel;
	case 24:
		return 0x0001000001000001ul*pixel;
	case 32:
		return 0x0000000100000001ul*pixel;
	default:
		panic("pixel_to_pat(): unsupported pixelformat\n");
    }
}
#else
static inline unsigned long
pixel_to_pat( u32 bpp, u32 pixel)
{
	switch (bpp) {
	case 1:
		return 0xfffffffful*pixel;
	case 2:
		return 0x55555555ul*pixel;
	case 4:
		return 0x11111111ul*pixel;
	case 8:
		return 0x01010101ul*pixel;
	case 12:
		return 0x01001001ul*pixel;
	case 16:
		return 0x00010001ul*pixel;
	case 24:
		return 0x01000001ul*pixel;
	case 32:
		return 0x00000001ul*pixel;
	default:
		panic("pixel_to_pat(): unsupported pixelformat\n");
    }
}
#endif

static void
bitfill_aligned_rev(struct fb_info *p, unsigned long __iomem *dst,
		    int dst_idx, unsigned long pat, unsigned n, int bits,
		    u32 bswapmask)
{
	unsigned long val = pat, dat;
	unsigned long first, last;

	if (!n)
		return;

	first = fb_shifted_pixels_mask_long(p, dst_idx, bswapmask);
	last = ~fb_shifted_pixels_mask_long(p, (dst_idx+n) % bits, bswapmask);

	if (dst_idx+n <= bits) {
		// Single word
		if (last)
			first &= last;
		dat = FB_READL(dst);
		FB_WRITEL(comp(dat ^ val, dat, first), dst);
	} else {
		// Multiple destination words
		// Leading bits
		if (first!=0UL) {
			dat = FB_READL(dst);
			FB_WRITEL(comp(dat ^ val, dat, first), dst);
			dst++;
			n -= bits - dst_idx;
		}

		// Main chunk
		n /= bits;
		while (n >= 8) {
			FB_WRITEL(FB_READL(dst) ^ val, dst);
			dst++;
			FB_WRITEL(FB_READL(dst) ^ val, dst);
			dst++;
			FB_WRITEL(FB_READL(dst) ^ val, dst);
			dst++;
			FB_WRITEL(FB_READL(dst) ^ val, dst);
			dst++;
			FB_WRITEL(FB_READL(dst) ^ val, dst);
			dst++;
			FB_WRITEL(FB_READL(dst) ^ val, dst);
			dst++;
			FB_WRITEL(FB_READL(dst) ^ val, dst);
			dst++;
			FB_WRITEL(FB_READL(dst) ^ val, dst);
			dst++;
			n -= 8;
		}
		while (n--) {
			FB_WRITEL(FB_READL(dst) ^ val, dst);
			dst++;
		}
		// Trailing bits
		if (last) {
			dat = FB_READL(dst);
			FB_WRITEL(comp(dat ^ val, dat, last), dst);
		}
	}
}

static void
bitfill_aligned(struct fb_info *p, unsigned long __iomem *dst, int dst_idx,
		unsigned long pat, unsigned n, int bits, u32 bswapmask)
{
	unsigned long first, last;

	if (!n)
		return;

	first = fb_shifted_pixels_mask_long(p, dst_idx, bswapmask);
	last = ~fb_shifted_pixels_mask_long(p, (dst_idx+n) % bits, bswapmask);

	if (dst_idx+n <= bits) {
		// Single word
		if (last)
			first &= last;
		FB_WRITEL(comp(pat, FB_READL(dst), first), dst);
	} else {
		// Multiple destination words

		// Leading bits
		if (first!= ~0UL) {
			FB_WRITEL(comp(pat, FB_READL(dst), first), dst);
			dst++;
			n -= bits - dst_idx;
		}

		// Main chunk
		n /= bits;
		while (n >= 8) {
			FB_WRITEL(pat, dst++);
			FB_WRITEL(pat, dst++);
			FB_WRITEL(pat, dst++);
			FB_WRITEL(pat, dst++);
			FB_WRITEL(pat, dst++);
			FB_WRITEL(pat, dst++);
			FB_WRITEL(pat, dst++);
			FB_WRITEL(pat, dst++);
			n -= 8;
		}
		while (n--)
			FB_WRITEL(pat, dst++);

		// Trailing bits
		if (last)
			FB_WRITEL(comp(pat, FB_READL(dst), last), dst);
	}
}

    /*
     *  Unaligned generic pattern invert using 32/64-bit memory accesses
     *  The pattern must have been expanded to a full 32/64-bit value
     *  Left/right are the appropriate shifts to convert to the pattern to be
     *  used for the next 32/64-bit word
     */
		
static void
bitfill_unaligned(struct fb_info *p, unsigned long __iomem *dst, int dst_idx,
				  unsigned long pat, int left, int right, unsigned n, int bits)
		{
			unsigned long first, last;
		
			if (!n)
				return;
		
			first = FB_SHIFT_HIGH(p, ~0UL, dst_idx);
			last = ~(FB_SHIFT_HIGH(p, ~0UL, (dst_idx+n) % bits));
		
			if (dst_idx+n <= bits) {
				// Single word
				if (last)
					first &= last;
				FB_WRITEL(comp(pat, FB_READL(dst), first), dst);
			} else {
				// Multiple destination words
				// Leading bits
				if (first) {
					FB_WRITEL(comp(pat, FB_READL(dst), first), dst);
					dst++;
					pat = pat << left | pat >> right;
					n -= bits - dst_idx;
				}
		
				// Main chunk
				n /= bits;
				while (n >= 4) {
					FB_WRITEL(pat, dst++);
					pat = pat << left | pat >> right;
					FB_WRITEL(pat, dst++);
					pat = pat << left | pat >> right;
					FB_WRITEL(pat, dst++);
					pat = pat << left | pat >> right;
					FB_WRITEL(pat, dst++);
					pat = pat << left | pat >> right;
					n -= 4;
				}
				while (n--) {
					FB_WRITEL(pat, dst++);
					pat = pat << left | pat >> right;
				}
		
				// Trailing bits
				if (last)
					FB_WRITEL(comp(pat, FB_READL(dst), last), dst);
			}
		}

static void
bitfill_unaligned_rev(struct fb_info *p, unsigned long __iomem *dst,
		      int dst_idx, unsigned long pat, int left, int right,
		      unsigned n, int bits)
{
	unsigned long first, last, dat;

	if (!n)
		return;

	first = FB_SHIFT_HIGH(p, ~0UL, dst_idx);
	last = ~(FB_SHIFT_HIGH(p, ~0UL, (dst_idx+n) % bits));

	if (dst_idx+n <= bits) {
		// Single word
		if (last)
			first &= last;
		dat = FB_READL(dst);
		FB_WRITEL(comp(dat ^ pat, dat, first), dst);
	} else {
		// Multiple destination words

		// Leading bits
		if (first != 0UL) {
			dat = FB_READL(dst);
			FB_WRITEL(comp(dat ^ pat, dat, first), dst);
			dst++;
			pat = pat << left | pat >> right;
			n -= bits - dst_idx;
		}

		// Main chunk
		n /= bits;
		while (n >= 4) {
			FB_WRITEL(FB_READL(dst) ^ pat, dst);
			dst++;
			pat = pat << left | pat >> right;
			FB_WRITEL(FB_READL(dst) ^ pat, dst);
			dst++;
			pat = pat << left | pat >> right;
			FB_WRITEL(FB_READL(dst) ^ pat, dst);
			dst++;
			pat = pat << left | pat >> right;
			FB_WRITEL(FB_READL(dst) ^ pat, dst);
			dst++;
			pat = pat << left | pat >> right;
			n -= 4;
		}
		while (n--) {
			FB_WRITEL(FB_READL(dst) ^ pat, dst);
			dst++;
			pat = pat << left | pat >> right;
		}

		// Trailing bits
		if (last) {
			dat = FB_READL(dst);
			FB_WRITEL(comp(dat ^ pat, dat, last), dst);
		}
	}
}

static void my_cfb_fillrect(struct fb_info *p, const struct fb_fillrect *rect)
{
	unsigned long pat, pat2, fg;
	unsigned long width = rect->width, height = rect->height;
	int bits = BITS_PER_LONG, bytes = bits >> 3;
	u32 bpp = p->var.bits_per_pixel;
	unsigned long __iomem *dst;
	int dst_idx, left;

	if (p->state != FBINFO_STATE_RUNNING)
		return;

	if (p->fix.visual == FB_VISUAL_TRUECOLOR ||
	    p->fix.visual == FB_VISUAL_DIRECTCOLOR )
		fg = ((u32 *) (p->pseudo_palette))[rect->color];
	else
		fg = rect->color;

	pat = pixel_to_pat(bpp, fg);

	dst = (unsigned long __iomem *)((unsigned long)p->screen_base & ~(bytes-1));
	dst_idx = ((unsigned long)p->screen_base & (bytes - 1))*8;
	dst_idx += rect->dy*p->fix.line_length*8+rect->dx*bpp;
	/* FIXME For now we support 1-32 bpp only */
	left = bits % bpp;
	if (p->fbops->fb_sync)
		p->fbops->fb_sync(p);
	if (!left) {
		u32 bswapmask = fb_compute_bswapmask(p);
		void (*fill_op32)(struct fb_info *p,
				  unsigned long __iomem *dst, int dst_idx,
		                  unsigned long pat, unsigned n, int bits,
				  u32 bswapmask) = NULL;

		switch (rect->rop) {
		case ROP_XOR:
			fill_op32 = bitfill_aligned_rev;
			break;
		case ROP_COPY:
			fill_op32 = bitfill_aligned;
			break;
		default:
			printk( KERN_ERR "cfb_fillrect(): unknown rop, defaulting to ROP_COPY\n");
			fill_op32 = bitfill_aligned;
			break;
		}
		while (height--) {
			dst += dst_idx >> (ffs(bits) - 1);
			dst_idx &= (bits - 1);
			fill_op32(p, dst, dst_idx, pat, width*bpp, bits,
				  bswapmask);
			dst_idx += p->fix.line_length*8;
		}
	} else {
		int right, r;
		void (*fill_op)(struct fb_info *p, unsigned long __iomem *dst,
				int dst_idx, unsigned long pat, int left,
				int right, unsigned n, int bits) = NULL;
#ifdef __LITTLE_ENDIAN
		right = left;
		left = bpp - right;
#else
		right = bpp - left;
#endif
		switch (rect->rop) {
		case ROP_XOR:
			fill_op = bitfill_unaligned_rev;
			break;
		case ROP_COPY:
			fill_op = bitfill_unaligned;
			break;
		default:
			printk(KERN_ERR "cfb_fillrect(): unknown rop, defaulting to ROP_COPY\n");
			fill_op = bitfill_unaligned;
			break;
		}
		while (height--) {
			dst += dst_idx / bits;
			dst_idx &= (bits - 1);
			r = dst_idx % bpp;
			/* rotate pattern to the correct start position */
			pat2 = le_long_to_cpu(rolx(cpu_to_le_long(pat), r, bpp));
			fill_op(p, dst, dst_idx, pat2, left, right,
				width*bpp, bits);
			dst_idx += p->fix.line_length*8;
		}
	}
}


static struct fb_ops s3c_fb_ops = {
	.owner		= THIS_MODULE,
	.fb_open	= s3c_fb_open,
	.fb_release	= s3c_fb_release,
	.fb_check_var	= s3c_fb_check_var,
	.fb_set_par	= s3c_fb_set_par,
	.fb_blank	= s3c_fb_blank,
	.fb_setcolreg	= s3c_fb_setcolreg,
	.fb_fillrect	= my_cfb_fillrect,
	.fb_copyarea	= my_cfb_copyarea,
	.fb_imageblit	= my_cfb_imageblit,
	.fb_pan_display	= s3c_fb_pan_display,
	.fb_ioctl	= s3c_fb_ioctl,
};

/**
 * s3c_fb_missing_pixclock() - calculates pixel clock
 * @mode: The video mode to change.
 *
 * Calculate the pixel clock when none has been given through platform data.
 */
static void __devinit s3c_fb_missing_pixclock(struct fb_videomode *mode)
{
	u64 pixclk = 1000000000000ULL;
	u32 div;

	div  = mode->left_margin + mode->hsync_len + mode->right_margin +
	       mode->xres;
	div *= mode->upper_margin + mode->vsync_len + mode->lower_margin +
	       mode->yres;
	div *= mode->refresh ? : 60;

	do_div(pixclk, div);

	mode->pixclock = pixclk;
}

/**
 * s3c_fb_alloc_memory() - allocate display memory for framebuffer window
 * @sfb: The base resources for the hardware.
 * @win: The window to initialise memory for.
 *
 * Allocate memory for the given framebuffer.
 */
static int __devinit s3c_fb_alloc_memory(struct s3c_fb *sfb,
					 struct s3c_fb_win *win)
{
	struct s3c_fb_pd_win *windata = win->windata;
	unsigned int real_size, virt_size, size;
	struct fb_info *fbi = win->fbinfo;
	dma_addr_t map_dma;

	dev_dbg(sfb->dev, "allocating memory for display\n");

	real_size = windata->win_mode.xres * windata->win_mode.yres;
	virt_size = windata->virtual_x * windata->virtual_y;

	dev_dbg(sfb->dev, "real_size=%u (%u.%u), virt_size=%u (%u.%u)\n",
		real_size, windata->win_mode.xres, windata->win_mode.yres,
		virt_size, windata->virtual_x, windata->virtual_y);

	size = (real_size > virt_size) ? real_size : virt_size;
	size *= (windata->max_bpp > 16) ? 32 : windata->max_bpp;
	size /= 8;

	fbi->fix.smem_len = size;
	size = PAGE_ALIGN(size);

	dev_dbg(sfb->dev, "want %u bytes for window\n", size);

	fbi->screen_base = dma_alloc_writecombine(sfb->dev, size,
						  &map_dma, GFP_KERNEL);
	if (!fbi->screen_base)
		return -ENOMEM;

	dev_dbg(sfb->dev, "mapped %x to %p\n",
		(unsigned int)map_dma, fbi->screen_base);

	memset(fbi->screen_base, 0x0, size);
	fbi->fix.smem_start = map_dma;

	return 0;
}

/**
 * s3c_fb_free_memory() - free the display memory for the given window
 * @sfb: The base resources for the hardware.
 * @win: The window to free the display memory for.
 *
 * Free the display memory allocated by s3c_fb_alloc_memory().
 */
static void s3c_fb_free_memory(struct s3c_fb *sfb, struct s3c_fb_win *win)
{
	struct fb_info *fbi = win->fbinfo;

	if (fbi->screen_base)
		dma_free_writecombine(sfb->dev, PAGE_ALIGN(fbi->fix.smem_len),
			      fbi->screen_base, fbi->fix.smem_start);
}

/**
 * s3c_fb_release_win() - release resources for a framebuffer window.
 * @win: The window to cleanup the resources for.
 *
 * Release the resources that where claimed for the hardware window,
 * such as the framebuffer instance and any memory claimed for it.
 */
static void s3c_fb_release_win(struct s3c_fb *sfb, struct s3c_fb_win *win)
{
	u32 data;

	if (win->fbinfo) {
		if (sfb->variant.has_shadowcon) {
			data = readl(sfb->regs + SHADOWCON);
			data &= ~SHADOWCON_CHx_ENABLE(win->index);
			data &= ~SHADOWCON_CHx_LOCAL_ENABLE(win->index);
			writel(data, sfb->regs + SHADOWCON);
		}
		unregister_framebuffer(win->fbinfo);
		if (win->fbinfo->cmap.len)
			fb_dealloc_cmap(&win->fbinfo->cmap);
		s3c_fb_free_memory(sfb, win);
		framebuffer_release(win->fbinfo);
	}
}

/**
 * s3c_fb_probe_win() - register an hardware window
 * @sfb: The base resources for the hardware
 * @variant: The variant information for this window.
 * @res: Pointer to where to place the resultant window.
 *
 * Allocate and do the basic initialisation for one of the hardware's graphics
 * windows.
 */
static int __devinit s3c_fb_probe_win(struct s3c_fb *sfb, unsigned int win_no,
				      struct s3c_fb_win_variant *variant,
				      struct s3c_fb_win **res)
{
	struct fb_var_screeninfo *var;
	struct fb_videomode *initmode;
	struct s3c_fb_pd_win *windata;
	struct s3c_fb_win *win;
	struct fb_info *fbinfo;
	int palette_size;
	int ret;

	dev_dbg(sfb->dev, "probing window %d, variant %p\n", win_no, variant);

	init_waitqueue_head(&sfb->vsync_info.wait);

	palette_size = variant->palette_sz * 4;

	fbinfo = framebuffer_alloc(sizeof(struct s3c_fb_win) +
				   palette_size * sizeof(u32), sfb->dev);
	if (!fbinfo) {
		dev_err(sfb->dev, "failed to allocate framebuffer\n");
		return -ENOENT;
	}

	windata = sfb->pdata->win[win_no];
	initmode = &windata->win_mode;

	WARN_ON(windata->max_bpp == 0);
	WARN_ON(windata->win_mode.xres == 0);
	WARN_ON(windata->win_mode.yres == 0);

	win = fbinfo->par;
	*res = win;
	var = &fbinfo->var;
	win->variant = *variant;
	win->fbinfo = fbinfo;
	win->parent = sfb;
	win->windata = windata;
	win->index = win_no;
	win->palette_buffer = (u32 *)(win + 1);

	ret = s3c_fb_alloc_memory(sfb, win);
	if (ret) {
		dev_err(sfb->dev, "failed to allocate display memory\n");
		return ret;
	}

	/* setup the r/b/g positions for the window's palette */
	if (win->variant.palette_16bpp) {
		/* Set RGB 5:6:5 as default */
		win->palette.r.offset = 11;
		win->palette.r.length = 5;
		win->palette.g.offset = 5;
		win->palette.g.length = 6;
		win->palette.b.offset = 0;
		win->palette.b.length = 5;

	} else {
		/* Set 8bpp or 8bpp and 1bit alpha */
		win->palette.r.offset = 16;
		win->palette.r.length = 8;
		win->palette.g.offset = 8;
		win->palette.g.length = 8;
		win->palette.b.offset = 0;
		win->palette.b.length = 8;
	}

	/* setup the initial video mode from the window */
	fb_videomode_to_var(&fbinfo->var, initmode);

	fbinfo->fix.type	= FB_TYPE_PACKED_PIXELS;
	fbinfo->fix.accel	= FB_ACCEL_NONE;
	fbinfo->var.activate	= FB_ACTIVATE_NOW;
	fbinfo->var.vmode	= FB_VMODE_NONINTERLACED;
	fbinfo->var.bits_per_pixel = windata->default_bpp;
	fbinfo->fbops		= &s3c_fb_ops;
	fbinfo->flags		= FBINFO_FLAG_DEFAULT;
	fbinfo->pseudo_palette  = &win->pseudo_palette;

	/* prepare to actually start the framebuffer */

	ret = s3c_fb_check_var(&fbinfo->var, fbinfo);
	if (ret < 0) {
		dev_err(sfb->dev, "check_var failed on initial video params\n");
		return ret;
	}

	/* create initial colour map */

	ret = fb_alloc_cmap(&fbinfo->cmap, win->variant.palette_sz, 1);
	if (ret == 0)
		fb_set_cmap(&fbinfo->cmap, fbinfo);
	else
		dev_err(sfb->dev, "failed to allocate fb cmap\n");

	s3c_fb_set_par(fbinfo);

	dev_dbg(sfb->dev, "about to register framebuffer\n");

	/* run the check_var and set_par on our configuration. */

	ret = register_framebuffer(fbinfo);
	if (ret < 0) {
		dev_err(sfb->dev, "failed to register framebuffer\n");
		return ret;
	}

	dev_info(sfb->dev, "window %d: fb %s\n", win_no, fbinfo->fix.id);

	return 0;
}

/**
 * s3c_fb_clear_win() - clear hardware window registers.
 * @sfb: The base resources for the hardware.
 * @win: The window to process.
 *
 * Reset the specific window registers to a known state.
 */
static void s3c_fb_clear_win(struct s3c_fb *sfb, int win)
{
	void __iomem *regs = sfb->regs;
	u32 reg;

	writel(0, regs + sfb->variant.wincon + (win * 4));
	writel(0, regs + VIDOSD_A(win, sfb->variant));
	writel(0, regs + VIDOSD_B(win, sfb->variant));
	writel(0, regs + VIDOSD_C(win, sfb->variant));
	reg = readl(regs + SHADOWCON);
	writel(reg & ~SHADOWCON_WINx_PROTECT(win), regs + SHADOWCON);
}

static int __devinit s3c_fb_probe(struct platform_device *pdev)
{
	const struct platform_device_id *platid;
	struct s3c_fb_driverdata *fbdrv;
	struct device *dev = &pdev->dev;
	struct s3c_fb_platdata *pd;
	struct s3c_fb *sfb;
	struct resource *res;
	int win;
	int ret = 0;

	platid = platform_get_device_id(pdev);
	fbdrv = (struct s3c_fb_driverdata *)platid->driver_data;

	if (fbdrv->variant.nr_windows > S3C_FB_MAX_WIN) {
		dev_err(dev, "too many windows, cannot attach\n");
		return -EINVAL;
	}

	pd = pdev->dev.platform_data;
	if (!pd) {
		dev_err(dev, "no platform data specified\n");
		return -EINVAL;
	}

	sfb = kzalloc(sizeof(struct s3c_fb), GFP_KERNEL);
	if (!sfb) {
		dev_err(dev, "no memory for framebuffers\n");
		return -ENOMEM;
	}

	dev_dbg(dev, "allocate new framebuffer %p\n", sfb);

	sfb->dev = dev;
	sfb->pdata = pd;
	sfb->variant = fbdrv->variant;

	spin_lock_init(&sfb->slock);

	sfb->bus_clk = clk_get(dev, "lcd");
	if (IS_ERR(sfb->bus_clk)) {
		dev_err(dev, "failed to get bus clock\n");
		ret = PTR_ERR(sfb->bus_clk);
		goto err_sfb;
	}

	clk_enable(sfb->bus_clk);

	pm_runtime_enable(sfb->dev);

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res) {
		dev_err(dev, "failed to find registers\n");
		ret = -ENOENT;
		goto err_clk;
	}

	sfb->regs_res = request_mem_region(res->start, resource_size(res),
					   dev_name(dev));
	if (!sfb->regs_res) {
		dev_err(dev, "failed to claim register region\n");
		ret = -ENOENT;
		goto err_clk;
	}

	sfb->regs = ioremap(res->start, resource_size(res));
	if (!sfb->regs) {
		dev_err(dev, "failed to map registers\n");
		ret = -ENXIO;
		goto err_req_region;
	}

	res = platform_get_resource(pdev, IORESOURCE_IRQ, 0);
	if (!res) {
		dev_err(dev, "failed to acquire irq resource\n");
		ret = -ENOENT;
		goto err_ioremap;
	}
	sfb->irq_no = res->start;
	ret = request_irq(sfb->irq_no, s3c_fb_irq,
			  0, "s3c_fb", sfb);
	if (ret) {
		dev_err(dev, "irq request failed\n");
		goto err_ioremap;
	}

	dev_dbg(dev, "got resources (regs %p), probing windows\n", sfb->regs);

	platform_set_drvdata(pdev, sfb);
	pm_runtime_get_sync(sfb->dev);

	/* setup gpio and output polarity controls */

	pd->setup_gpio();

	writel(pd->vidcon1, sfb->regs + VIDCON1);

	/* zero all windows before we do anything */

	for (win = 0; win < fbdrv->variant.nr_windows; win++)
		s3c_fb_clear_win(sfb, win);

	/* initialise colour key controls */
	for (win = 0; win < (fbdrv->variant.nr_windows - 1); win++) {
		void __iomem *regs = sfb->regs + sfb->variant.keycon;

		regs += (win * 8);
		writel(0xffffff, regs + WKEYCON0);
		writel(0xffffff, regs + WKEYCON1);
	}

	/* we have the register setup, start allocating framebuffers */

	for (win = 0; win < fbdrv->variant.nr_windows; win++) {
		if (!pd->win[win])
			continue;

		if (!pd->win[win]->win_mode.pixclock)
			s3c_fb_missing_pixclock(&pd->win[win]->win_mode);

		ret = s3c_fb_probe_win(sfb, win, fbdrv->win[win],
				       &sfb->windows[win]);
		if (ret < 0) {
			dev_err(dev, "failed to create window %d\n", win);
			for (; win >= 0; win--)
				s3c_fb_release_win(sfb, sfb->windows[win]);
			goto err_irq;
		}
	}

	platform_set_drvdata(pdev, sfb);
	pm_runtime_put_sync(sfb->dev);

printk("If you do not select console support, then add the following code for support logo\n");	
//#if defined(CONFIG_LOGO)
#if 0
	if (fb_prepare_logo( sfb->windows[pd->default_win]->fbinfo, FB_ROTATE_UR)) {
	printk("[valor lion]: Start display and show logo\n");
	/* Start display and show logo on boot */
	fb_set_cmap(&sfb->windows[pd->default_win]->fbinfo->cmap, sfb->windows[pd->default_win]->fbinfo);
	fb_show_logo(sfb->windows[pd->default_win]->fbinfo, FB_ROTATE_UR);
	}	
#endif

	return 0;

err_irq:
	free_irq(sfb->irq_no, sfb);

err_ioremap:
	iounmap(sfb->regs);

err_req_region:
	release_mem_region(sfb->regs_res->start, resource_size(sfb->regs_res));

err_clk:
	clk_disable(sfb->bus_clk);
	clk_put(sfb->bus_clk);

err_sfb:
	kfree(sfb);
	return ret;
}

/**
 * s3c_fb_remove() - Cleanup on module finalisation
 * @pdev: The platform device we are bound to.
 *
 * Shutdown and then release all the resources that the driver allocated
 * on initialisation.
 */
static int __devexit s3c_fb_remove(struct platform_device *pdev)
{
	struct s3c_fb *sfb = platform_get_drvdata(pdev);
	int win;

	pm_runtime_get_sync(sfb->dev);

	for (win = 0; win < S3C_FB_MAX_WIN; win++)
		if (sfb->windows[win])
			s3c_fb_release_win(sfb, sfb->windows[win]);

	free_irq(sfb->irq_no, sfb);

	iounmap(sfb->regs);

	clk_disable(sfb->bus_clk);
	clk_put(sfb->bus_clk);

	release_mem_region(sfb->regs_res->start, resource_size(sfb->regs_res));

	pm_runtime_put_sync(sfb->dev);
	pm_runtime_disable(sfb->dev);

	kfree(sfb);
	return 0;
}

#ifdef CONFIG_PM
static int s3c_fb_suspend(struct device *dev)
{
	struct platform_device *pdev = to_platform_device(dev);
	struct s3c_fb *sfb = platform_get_drvdata(pdev);
	struct s3c_fb_win *win;
	int win_no;

	for (win_no = S3C_FB_MAX_WIN - 1; win_no >= 0; win_no--) {
		win = sfb->windows[win_no];
		if (!win)
			continue;

		/* use the blank function to push into power-down */
		s3c_fb_blank(FB_BLANK_POWERDOWN, win->fbinfo);
	}

	clk_disable(sfb->bus_clk);
	return 0;
}

static int s3c_fb_resume(struct device *dev)
{
	struct platform_device *pdev = to_platform_device(dev);
	struct s3c_fb *sfb = platform_get_drvdata(pdev);
	struct s3c_fb_platdata *pd = sfb->pdata;
	struct s3c_fb_win *win;
	int win_no;

	clk_enable(sfb->bus_clk);

	/* setup gpio and output polarity controls */
	pd->setup_gpio();
	writel(pd->vidcon1, sfb->regs + VIDCON1);

	/* zero all windows before we do anything */
	for (win_no = 0; win_no < sfb->variant.nr_windows; win_no++)
		s3c_fb_clear_win(sfb, win_no);

	for (win_no = 0; win_no < sfb->variant.nr_windows - 1; win_no++) {
		void __iomem *regs = sfb->regs + sfb->variant.keycon;

		regs += (win_no * 8);
		writel(0xffffff, regs + WKEYCON0);
		writel(0xffffff, regs + WKEYCON1);
	}

	/* restore framebuffers */
	for (win_no = 0; win_no < S3C_FB_MAX_WIN; win_no++) {
		win = sfb->windows[win_no];
		if (!win)
			continue;

		dev_dbg(&pdev->dev, "resuming window %d\n", win_no);
		s3c_fb_set_par(win->fbinfo);
	}

	return 0;
}

static int s3c_fb_runtime_suspend(struct device *dev)
{
	struct platform_device *pdev = to_platform_device(dev);
	struct s3c_fb *sfb = platform_get_drvdata(pdev);
	struct s3c_fb_win *win;
	int win_no;

	for (win_no = S3C_FB_MAX_WIN - 1; win_no >= 0; win_no--) {
		win = sfb->windows[win_no];
		if (!win)
			continue;

		/* use the blank function to push into power-down */
		s3c_fb_blank(FB_BLANK_POWERDOWN, win->fbinfo);
	}

	clk_disable(sfb->bus_clk);
	return 0;
}

static int s3c_fb_runtime_resume(struct device *dev)
{
	struct platform_device *pdev = to_platform_device(dev);
	struct s3c_fb *sfb = platform_get_drvdata(pdev);
	struct s3c_fb_platdata *pd = sfb->pdata;
	struct s3c_fb_win *win;
	int win_no;

	clk_enable(sfb->bus_clk);

	/* setup gpio and output polarity controls */
	pd->setup_gpio();
	writel(pd->vidcon1, sfb->regs + VIDCON1);

	/* zero all windows before we do anything */
	for (win_no = 0; win_no < sfb->variant.nr_windows; win_no++)
		s3c_fb_clear_win(sfb, win_no);

	for (win_no = 0; win_no < sfb->variant.nr_windows - 1; win_no++) {
		void __iomem *regs = sfb->regs + sfb->variant.keycon;

		regs += (win_no * 8);
		writel(0xffffff, regs + WKEYCON0);
		writel(0xffffff, regs + WKEYCON1);
	}

	/* restore framebuffers */
	for (win_no = 0; win_no < S3C_FB_MAX_WIN; win_no++) {
		win = sfb->windows[win_no];
		if (!win)
			continue;

		dev_dbg(&pdev->dev, "resuming window %d\n", win_no);
		s3c_fb_set_par(win->fbinfo);
	}

	return 0;
}

#else
#define s3c_fb_suspend NULL
#define s3c_fb_resume  NULL
#define s3c_fb_runtime_suspend NULL
#define s3c_fb_runtime_resume NULL
#endif


#define VALID_BPP124 (VALID_BPP(1) | VALID_BPP(2) | VALID_BPP(4))
#define VALID_BPP1248 (VALID_BPP124 | VALID_BPP(8))

static struct s3c_fb_win_variant s3c_fb_data_64xx_wins[] = {
	[0] = {
		.has_osd_c	= 1,
		.osd_size_off	= 0x8,
		.palette_sz	= 256,
		.valid_bpp	= (VALID_BPP1248 | VALID_BPP(16) |
				   VALID_BPP(18) | VALID_BPP(24)),
	},
	[1] = {
		.has_osd_c	= 1,
		.has_osd_d	= 1,
		.osd_size_off	= 0xc,
		.has_osd_alpha	= 1,
		.palette_sz	= 256,
		.valid_bpp	= (VALID_BPP1248 | VALID_BPP(16) |
				   VALID_BPP(18) | VALID_BPP(19) |
				   VALID_BPP(24) | VALID_BPP(25) |
				   VALID_BPP(28)),
	},
	[2] = {
		.has_osd_c	= 1,
		.has_osd_d	= 1,
		.osd_size_off	= 0xc,
		.has_osd_alpha	= 1,
		.palette_sz	= 16,
		.palette_16bpp	= 1,
		.valid_bpp	= (VALID_BPP1248 | VALID_BPP(16) |
				   VALID_BPP(18) | VALID_BPP(19) |
				   VALID_BPP(24) | VALID_BPP(25) |
				   VALID_BPP(28)),
	},
	[3] = {
		.has_osd_c	= 1,
		.has_osd_alpha	= 1,
		.palette_sz	= 16,
		.palette_16bpp	= 1,
		.valid_bpp	= (VALID_BPP124  | VALID_BPP(16) |
				   VALID_BPP(18) | VALID_BPP(19) |
				   VALID_BPP(24) | VALID_BPP(25) |
				   VALID_BPP(28)),
	},
	[4] = {
		.has_osd_c	= 1,
		.has_osd_alpha	= 1,
		.palette_sz	= 4,
		.palette_16bpp	= 1,
		.valid_bpp	= (VALID_BPP(1) | VALID_BPP(2) |
				   VALID_BPP(16) | VALID_BPP(18) |
				   VALID_BPP(19) | VALID_BPP(24) |
				   VALID_BPP(25) | VALID_BPP(28)),
	},
};

static struct s3c_fb_win_variant s3c_fb_data_s5p_wins[] = {
	[0] = {
		.has_osd_c	= 1,
		.osd_size_off	= 0x8,
		.palette_sz	= 256,
		.valid_bpp	= (VALID_BPP1248 | VALID_BPP(13) |
				   VALID_BPP(15) | VALID_BPP(16) |
				   VALID_BPP(18) | VALID_BPP(19) |
				   VALID_BPP(24) | VALID_BPP(25) |
				   VALID_BPP(32)),
	},
	[1] = {
		.has_osd_c	= 1,
		.has_osd_d	= 1,
		.osd_size_off	= 0xc,
		.has_osd_alpha	= 1,
		.palette_sz	= 256,
		.valid_bpp	= (VALID_BPP1248 | VALID_BPP(13) |
				   VALID_BPP(15) | VALID_BPP(16) |
				   VALID_BPP(18) | VALID_BPP(19) |
				   VALID_BPP(24) | VALID_BPP(25) |
				   VALID_BPP(32)),
	},
	[2] = {
		.has_osd_c	= 1,
		.has_osd_d	= 1,
		.osd_size_off	= 0xc,
		.has_osd_alpha	= 1,
		.palette_sz	= 256,
		.valid_bpp	= (VALID_BPP1248 | VALID_BPP(13) |
				   VALID_BPP(15) | VALID_BPP(16) |
				   VALID_BPP(18) | VALID_BPP(19) |
				   VALID_BPP(24) | VALID_BPP(25) |
				   VALID_BPP(32)),
	},
	[3] = {
		.has_osd_c	= 1,
		.has_osd_alpha	= 1,
		.palette_sz	= 256,
		.valid_bpp	= (VALID_BPP1248 | VALID_BPP(13) |
				   VALID_BPP(15) | VALID_BPP(16) |
				   VALID_BPP(18) | VALID_BPP(19) |
				   VALID_BPP(24) | VALID_BPP(25) |
				   VALID_BPP(32)),
	},
	[4] = {
		.has_osd_c	= 1,
		.has_osd_alpha	= 1,
		.palette_sz	= 256,
		.valid_bpp	= (VALID_BPP1248 | VALID_BPP(13) |
				   VALID_BPP(15) | VALID_BPP(16) |
				   VALID_BPP(18) | VALID_BPP(19) |
				   VALID_BPP(24) | VALID_BPP(25) |
				   VALID_BPP(32)),
	},
};

static struct s3c_fb_driverdata s3c_fb_data_64xx = {
	.variant = {
		.nr_windows	= 5,
		.vidtcon	= VIDTCON0,
		.wincon		= WINCON(0),
		.winmap		= WINxMAP(0),
		.keycon		= WKEYCON,
		.osd		= VIDOSD_BASE,
		.osd_stride	= 16,
		.buf_start	= VIDW_BUF_START(0),
		.buf_size	= VIDW_BUF_SIZE(0),
		.buf_end	= VIDW_BUF_END(0),

		.palette = {
			[0] = 0x400,
			[1] = 0x800,
			[2] = 0x300,
			[3] = 0x320,
			[4] = 0x340,
		},

		.has_prtcon	= 1,
	},
	.win[0]	= &s3c_fb_data_64xx_wins[0],
	.win[1]	= &s3c_fb_data_64xx_wins[1],
	.win[2]	= &s3c_fb_data_64xx_wins[2],
	.win[3]	= &s3c_fb_data_64xx_wins[3],
	.win[4]	= &s3c_fb_data_64xx_wins[4],
};

static struct s3c_fb_driverdata s3c_fb_data_s5pc100 = {
	.variant = {
		.nr_windows	= 5,
		.vidtcon	= VIDTCON0,
		.wincon		= WINCON(0),
		.winmap		= WINxMAP(0),
		.keycon		= WKEYCON,
		.osd		= VIDOSD_BASE,
		.osd_stride	= 16,
		.buf_start	= VIDW_BUF_START(0),
		.buf_size	= VIDW_BUF_SIZE(0),
		.buf_end	= VIDW_BUF_END(0),

		.palette = {
			[0] = 0x2400,
			[1] = 0x2800,
			[2] = 0x2c00,
			[3] = 0x3000,
			[4] = 0x3400,
		},

		.has_prtcon	= 1,
	},
	.win[0]	= &s3c_fb_data_s5p_wins[0],
	.win[1]	= &s3c_fb_data_s5p_wins[1],
	.win[2]	= &s3c_fb_data_s5p_wins[2],
	.win[3]	= &s3c_fb_data_s5p_wins[3],
	.win[4]	= &s3c_fb_data_s5p_wins[4],
};

static struct s3c_fb_driverdata s3c_fb_data_s5pv210 = {
	.variant = {
		.nr_windows	= 5,
		.vidtcon	= VIDTCON0,
		.wincon		= WINCON(0),
		.winmap		= WINxMAP(0),
		.keycon		= WKEYCON,
		.osd		= VIDOSD_BASE,
		.osd_stride	= 16,
		.buf_start	= VIDW_BUF_START(0),
		.buf_size	= VIDW_BUF_SIZE(0),
		.buf_end	= VIDW_BUF_END(0),

		.palette = {
			[0] = 0x2400,
			[1] = 0x2800,
			[2] = 0x2c00,
			[3] = 0x3000,
			[4] = 0x3400,
		},

		.has_shadowcon	= 1,
	},
	.win[0]	= &s3c_fb_data_s5p_wins[0],
	.win[1]	= &s3c_fb_data_s5p_wins[1],
	.win[2]	= &s3c_fb_data_s5p_wins[2],
	.win[3]	= &s3c_fb_data_s5p_wins[3],
	.win[4]	= &s3c_fb_data_s5p_wins[4],
};

/* S3C2443/S3C2416 style hardware */
static struct s3c_fb_driverdata s3c_fb_data_s3c2443 = {
	.variant = {
		.nr_windows	= 2,
		.is_2443	= 1,

		.vidtcon	= 0x08,
		.wincon		= 0x14,
		.winmap		= 0xd0,
		.keycon		= 0xb0,
		.osd		= 0x28,
		.osd_stride	= 12,
		.buf_start	= 0x64,
		.buf_size	= 0x94,
		.buf_end	= 0x7c,

		.palette = {
			[0] = 0x400,
			[1] = 0x800,
		},
	},
	.win[0] = &(struct s3c_fb_win_variant) {
		.palette_sz	= 256,
		.valid_bpp	= VALID_BPP1248 | VALID_BPP(16) | VALID_BPP(24),
	},
	.win[1] = &(struct s3c_fb_win_variant) {
		.has_osd_c	= 1,
		.has_osd_alpha	= 1,
		.palette_sz	= 256,
		.valid_bpp	= (VALID_BPP1248 | VALID_BPP(16) |
				   VALID_BPP(18) | VALID_BPP(19) |
				   VALID_BPP(24) | VALID_BPP(25) |
				   VALID_BPP(28)),
	},
};

static struct platform_device_id s3c_fb_driver_ids[] = {
	{
		.name		= "s3c-fb",
		.driver_data	= (unsigned long)&s3c_fb_data_64xx,
	}, {
		.name		= "s5pc100-fb",
		.driver_data	= (unsigned long)&s3c_fb_data_s5pc100,
	}, {
		.name		= "s5pv210-fb",
		.driver_data	= (unsigned long)&s3c_fb_data_s5pv210,
	}, {
		.name		= "s3c2443-fb",
		.driver_data	= (unsigned long)&s3c_fb_data_s3c2443,
	},
	{},
};
MODULE_DEVICE_TABLE(platform, s3c_fb_driver_ids);

static const struct dev_pm_ops s3cfb_pm_ops = {
	.suspend	= s3c_fb_suspend,
	.resume		= s3c_fb_resume,
	.runtime_suspend	= s3c_fb_runtime_suspend,
	.runtime_resume		= s3c_fb_runtime_resume,
};

static struct platform_driver s3c_fb_driver = {
	.probe		= s3c_fb_probe,
	.remove		= __devexit_p(s3c_fb_remove),
	.id_table	= s3c_fb_driver_ids,
	.driver		= {
		.name	= "s3c-fb",
		.owner	= THIS_MODULE,
		.pm	= &s3cfb_pm_ops,
	},
};

static int __init s3c_fb_init(void)
{
	return platform_driver_register(&s3c_fb_driver);
}

static void __exit s3c_fb_cleanup(void)
{
	platform_driver_unregister(&s3c_fb_driver);
}

module_init(s3c_fb_init);
module_exit(s3c_fb_cleanup);

MODULE_LICENSE("GPL");


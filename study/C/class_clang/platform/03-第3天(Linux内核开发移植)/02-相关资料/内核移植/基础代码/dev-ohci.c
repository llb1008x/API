/*
 * Copyright (C) 2013 sunplusedu
 * Author: valor-lion <zhangda198808@163.com>
 *
 *  This program is free software; you can redistribute  it and/or modify it
 *  under  the terms of  the GNU General  Public License as published by the
 *  Free Software Foundation;  either version 2 of the  License, or (at your
 *  option) any later version.
 *
 */

#include <linux/platform_device.h>
#include <mach/irqs.h>
#include <mach/map.h>
#include <plat/devs.h>
#include <plat/ohci.h>
#include <plat/usb-phy.h>

static struct resource s5p_ohci_resource[] = {
	[0] = {
		.start = S5P_PA_OHCI,
		.end   = S5P_PA_OHCI  + SZ_256 - 1,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start = IRQ_UHOST,
		.end   = IRQ_UHOST,
		.flags = IORESOURCE_IRQ,
	}
};

static u64 samsung_device_dma_mask = 0xffffffffUL;

struct platform_device s5p_device_ohci = {
	.name		= "s5p-ohci",
	.id		= -1,
	.num_resources	= ARRAY_SIZE(s5p_ohci_resource),
	.resource	= s5p_ohci_resource,
	.dev		= {
		.dma_mask		= &samsung_device_dma_mask,
		.coherent_dma_mask	= 0xffffffffUL,
	}
};

void __init s5p_ohci_set_platdata(struct s5p_ohci_platdata *pd)
{
	struct s5p_ohci_platdata *npd;

	npd = s3c_set_platdata(pd, sizeof(struct s5p_ohci_platdata),
			&s5p_device_ohci);
}
/* linux/arch/arm/mach-s3c2440/mach-smdk2440.c
 *
 * Copyright (c) 2004,2005 Simtec Electronics
 *	Ben Dooks <ben@simtec.co.uk>
 *
 * http://www.fluff.org/ben/smdk2440/
 *
 * Thanks to Dimity Andric and TomTom for the loan of an SMDK2440.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
*/

#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/interrupt.h>
#include <linux/list.h>
#include <linux/timer.h>
#include <linux/init.h>
#include <linux/serial_core.h>
#include <linux/platform_device.h>
#include <linux/io.h>

#include <asm/mach/arch.h>
#include <asm/mach/map.h>
#include <asm/mach/irq.h>

#include <mach/hardware.h>
#include <asm/irq.h>
#include <asm/mach-types.h>

#include <plat/regs-serial.h>
#include <mach/regs-gpio.h>
#include <mach/regs-lcd.h>

#include <mach/idle.h>
#include <mach/fb.h>
#include <plat/iic.h>

#include <plat/s3c2410.h>
#include <plat/s3c2440.h>
#include <plat/clock.h>
#include <plat/devs.h>
#include <plat/cpu.h>

#include <plat/common-smdk.h>

#include <linux/mtd/partitions.h>
#include <plat/nand.h>

#include <linux/ioport.h>
#include <linux/dm9000.h>

static struct mtd_partition micro2440_nand_partitions[] = {
	[0] = {
		.name = "bootloader",
		.size = 0x200000,
		.offset = 0,
	},
	[1] = {
		.name = "kernel",
		.size = 0x600000,
		.offset = 0x200000,
	},
	[2] = {
		.name = "rootfs",
		.size = 0x4000000,
		.offset = 0x800000,
	},
	[3] = {
		.name = "userfs",
		.size = 0xb800000,
		.offset = 0x4800000,
	},
};
/**/
static struct s3c2410_nand_set micro2440_nand_sets[] = {
	[0] = {
		.name = "NAND_Flash",
		.nr_chips = 1,
		.nr_partitions = ARRAY_SIZE(micro2440_nand_partitions),
		.partitions = micro2440_nand_partitions,
	},
};

static struct s3c2410_platform_nand micro2440_nand_platform = {
	.tacls = 20,
	.twrph0 = 30,
	.twrph1 = 20,
	.nr_sets = ARRAY_SIZE(micro2440_nand_sets),
	.sets = micro2440_nand_sets,
	.ignore_unset_ecc = 1,
};


#define MACH_MICRO2440_DM9K_BASE (S3C2410_CS4 + 0x300)

static struct resource micro2440_dm9k_resources[] = {
	[0] = {
		.start = MACH_MICRO2440_DM9K_BASE + 0x00,
		.end   = MACH_MICRO2440_DM9K_BASE + 0x03,
		.flags  = IORESOURCE_MEM,
	},	
	[1] = {
		.start = MACH_MICRO2440_DM9K_BASE + 0x04,
		.end   = MACH_MICRO2440_DM9K_BASE + 0x07,
		.flags  = IORESOURCE_MEM,
	},
	[2] = {
		.start = IRQ_EINT7,
		.end   = IRQ_EINT7,
		.flags = IORESOURCE_IRQ | IORESOURCE_IRQ_HIGHEDGE,
	},
};

static struct dm9000_plat_data micro2400_dm9k_pdata = {
	.flags = DM9000_PLATF_16BITONLY |
			 DM9000_PLATF_NO_EEPROM,
	.dev_addr = {0x83, 0x89, 0x76, 0x98, 0x89, 0xaa},
};

static struct platform_device s3c_device_dm9k = {
	.name = "dm9000",
	.id = -1,
	.num_resources = ARRAY_SIZE(micro2440_dm9k_resources),
	.resource = micro2440_dm9k_resources,
	.dev = {
		.platform_data = &micro2400_dm9k_pdata,
	},
};





static struct map_desc smdk2440_iodesc[] __initdata = {
	/* ISA IO Space map (memory space selected by A24) */

	{
		.virtual	= (u32)S3C24XX_VA_ISA_WORD,
		.pfn		= __phys_to_pfn(S3C2410_CS2),
		.length		= 0x10000,
		.type		= MT_DEVICE,
	}, {
		.virtual	= (u32)S3C24XX_VA_ISA_WORD + 0x10000,
		.pfn		= __phys_to_pfn(S3C2410_CS2 + (1<<24)),
		.length		= SZ_4M,
		.type		= MT_DEVICE,
	}, {
		.virtual	= (u32)S3C24XX_VA_ISA_BYTE,
		.pfn		= __phys_to_pfn(S3C2410_CS2),
		.length		= 0x10000,
		.type		= MT_DEVICE,
	}, {
		.virtual	= (u32)S3C24XX_VA_ISA_BYTE + 0x10000,
		.pfn		= __phys_to_pfn(S3C2410_CS2 + (1<<24)),
		.length		= SZ_4M,
		.type		= MT_DEVICE,
	}
};

#define UCON S3C2410_UCON_DEFAULT | S3C2410_UCON_UCLK
#define ULCON S3C2410_LCON_CS8 | S3C2410_LCON_PNONE | S3C2410_LCON_STOPB
#define UFCON S3C2410_UFCON_RXTRIG8 | S3C2410_UFCON_FIFOMODE

static struct s3c2410_uartcfg smdk2440_uartcfgs[] __initdata = {
	[0] = {
		.hwport	     = 0,
		.flags	     = 0,
		.ucon	     = 0x3c5,
		.ulcon	     = 0x03,
		.ufcon	     = 0x51,
	},
	[1] = {
		.hwport	     = 1,
		.flags	     = 0,
		.ucon	     = 0x3c5,
		.ulcon	     = 0x03,
		.ufcon	     = 0x51,
	},
	/* IR port */
	[2] = {
		.hwport	     = 2,
		.flags	     = 0,
		.ucon	     = 0x3c5,
		.ulcon	     = 0x43,
		.ufcon	     = 0x51,
	}
};

/* LCD driver info */

static struct s3c2410fb_display mini2440_lcd_cfg __initdata = {

	.lcdcon5	= S3C2410_LCDCON5_FRM565 |
			  S3C2410_LCDCON5_INVVFRAME |
			  S3C2410_LCDCON5_INVVLINE |
			  S3C2410_LCDCON5_PWREN |
			  S3C2410_LCDCON5_HWSWP,

	.type		= S3C2410_LCDCON1_TFT,

	.width		= 320,
	.height		= 240,

	.pixclock	= 170000, /* HCLK 60 MHz, divisor 10 */
	.xres		= 320,
	.yres		= 240,
	.bpp		= 16,
	.left_margin	= 0x04,
	.right_margin	= 0x04,
	.hsync_len	= 0x01,
	.upper_margin	= 0x09,
	.lower_margin	= 0x02,
	.vsync_len	= 0x01,
};



static struct s3c2410fb_mach_info mini2440_fb_info __initdata = {
	.displays	= &mini2440_lcd_cfg,
	.num_displays	= 1,
	.default_display = 0,

	.gpccon		= 0xaa955699,
	.gpccon_mask	= 0xffc003cc,
	.gpcup		= 0x0000ffff,
	.gpcup_mask	= 0xffffffff,
	.gpdcon		= 0xaa95aaa1,
	.gpdcon_mask	= 0xffc0fff0,
	.gpdup		= 0x0000faff,
	.gpdup_mask	= 0xffffffff,

	.lpcsel		= 0xf82,
};

static struct platform_device *smdk2440_devices[] __initdata = {
	&s3c_device_usb,
	&s3c_device_lcd,
	&s3c_device_wdt,
	&s3c_device_i2c0,
	&s3c_device_iis,
 	&s3c_device_nand,
	&s3c_device_dm9k,
};

static void __init smdk2440_map_io(void)
{
	s3c24xx_init_io(smdk2440_iodesc, ARRAY_SIZE(smdk2440_iodesc));
	s3c24xx_init_clocks(12000000);
	s3c24xx_init_uarts(smdk2440_uartcfgs, ARRAY_SIZE(smdk2440_uartcfgs));
}

static void __init smdk2440_machine_init(void)
{
	s3c24xx_fb_set_platdata(&mini2440_fb_info);
	s3c_i2c0_set_platdata(NULL);

	s3c_device_nand.dev.platform_data = &micro2440_nand_platform;

	platform_add_devices(smdk2440_devices, ARRAY_SIZE(smdk2440_devices));
}

MACHINE_START(MINI2440, "Micro2440 development board")
	/* Maintainer: Ben Dooks <ben@fluff.org> */
	.phys_io	= S3C2410_PA_UART,
	.io_pg_offst	= (((u32)S3C24XX_VA_UART) >> 18) & 0xfffc,
	.boot_params	= S3C2410_SDRAM_PA + 0x100,

	.init_irq	= s3c24xx_init_irq,
	.map_io		= smdk2440_map_io,
	.init_machine	= smdk2440_machine_init,
	.timer		= &s3c24xx_timer,
MACHINE_END

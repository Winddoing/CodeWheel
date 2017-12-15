#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <mach/irqs.h>
#include <plat/irqs.h>

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

#define GPJ2CON  0xe0200280
#define GPJ2DAT  0xe0200284
#define GPJ2PUD  0xe0200288
#define GPH2DAT  0xe0200c44
#define GPH3DAT  0xe0200c64

struct gec_key {
	int key;
	int irq;
	char *name;
};


static struct resource gec_key_resource[] = {
	[0] = {
		.start = GPJ2CON,
		.end   = GPJ2CON + 3,
		.name = "GPJ2CON",
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start = GPJ2DAT,
		.end   = GPJ2DAT + 3,
		.name = "GPJ2DAT",
		.flags = IORESOURCE_MEM,
	},
	[2] = {
		.start = GPJ2PUD,
		.end   = GPJ2PUD + 3,
		.name = "GPJ2PUD",
		.flags = IORESOURCE_MEM,
	},
	[3] = {
		.start = GPH2DAT,
		.end   = GPH2DAT + 3,
		.name = "GPH2DAT",
		.flags = IORESOURCE_MEM,
	},
	[4] = {
		.start = GPH3DAT,
		.end   = GPH3DAT + 3,
		.name = "GPH3DAT",
		.flags = IORESOURCE_MEM,
	},
	[5] = {
		.start = IRQ_EINT(16),
		.end   = IRQ_EINT(16),
		.name = "GEC_INT16",
		.flags = IORESOURCE_IRQ,
	}
};

struct gec_key button[] ={
	
	{0,IRQ_EINT(16),"key1"},
	{1,IRQ_EINT(17),"key2"},
	{2,IRQ_EINT(18),"key3"},
	{3,IRQ_EINT(19),"key4"},
	{4,IRQ_EINT(24),"key5"},
	{5,IRQ_EINT(25),"key6"},
	{6,IRQ_EINT(26),"key7"},
	{7,IRQ_EINT(27),"key8"},
};

void gec_key_release(struct device *dev)
{
	printk(KERN_INFO "gec_key_release\n");
}


static struct platform_device geckey_key_device ={
	.name = "gec_key",
	.id = -1,
	.dev={
		.platform_data = button,
		.release = gec_key_release,
	},
	.num_resources	 = ARRAY_SIZE(gec_key_resource),
	.resource = gec_key_resource,
};

static int __init key_dev_init(void)
{
	printk(KERN_INFO "char_init\n");
	return platform_device_register(&geckey_key_device);

}

static void __exit key_dev_exit(void)
{
	platform_device_unregister(&geckey_key_device);
}
module_init(key_dev_init);
module_exit(key_dev_exit);

MODULE_AUTHOR("lvling <lv__ling@126.com>");
MODULE_DESCRIPTION("char device module.");
MODULE_LICENSE("GPL");


#include <linux/kernel.h>
#include <linux/module.h>

#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <mach/regs-gpio.h>
#include <mach/gpio-fns.h>
#include <asm/uaccess.h>

#include <linux/micro2440_l298.h>

#define MICRO2440_CAR_NAME "mycar"
#define MICR02440_PIN_STAT S3C2410_GPIO_OUTPUT

static int micro2440_l298_ioctl(struct inode *, struct file *, unsigned int, unsigned long);
static int micro2440_l298_release(struct inode *, struct file *);
static int micro2440_l298_open(struct inode *, struct file *);

static unsigned long l298_table[] = {
	S3C2410_GPF(0),	//9
	S3C2410_GPF(1),	//10
	S3C2410_GPF(2),	//11
	S3C2410_GPF(3),	//12
//	S3C2410_GPF(6),	//15
//	S3C2410_GPF(8),	//16
};

static void  micro2440_l298_sets(unsigned long stat)
{
	int i;
	for(i = 0; i <ARRAY_SIZE(l298_table); i++ )
	{
		s3c2410_gpio_setpin(l298_table[i], (stat >> i) & 0x01);
	}
}

static void  micro2440_l298_cfg(void)
{
	int i;
	for(i = 0; i <ARRAY_SIZE(l298_table); i++ )
	{
		s3c2410_gpio_cfgpin(l298_table[i], MICR02440_PIN_STAT);
	}
}
static const struct file_operations micro2440_l298_fops = {
	.owner = THIS_MODULE,
	.ioctl = micro2440_l298_ioctl,
	.release = micro2440_l298_release,
	.open = micro2440_l298_open,
};

static struct miscdevice micro2440_l298_miscdevice = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = MICRO2440_CAR_NAME,

	.fops = &micro2440_l298_fops,	
};

static int micro2440_l298_ioctl(struct inode * inode, struct file * file, unsigned int cmd, unsigned long arg)
{

	switch(cmd)
	{
		case CAR_GO_UP:
			printk("Micro2440 \"l298\" driver loaded fail---------0101------UP\n");
		  	micro2440_l298_sets(0x5);
			break;
		case CAR_GO_DOWN:
			printk("Micro2440 \"l298\" driver loaded fail---------1010-----down\n");
			micro2440_l298_sets(0xA);
			break;
		case CAR_GO_LEFT:
			printk("Micro2440 \"l298\" driver loaded fail---------1001------left\n");
			micro2440_l298_sets(0x9);
			break;
		case CAR_GO_RIGHT:
			printk("Micro2440 \"l298\" driver loaded fail---------0110-------right\n");
			micro2440_l298_sets(0x6);
			break;
		case CAR_GO_STOP:
			printk("Micro2440 \"l298\" driver loaded fail---------0000------stop\n");
			micro2440_l298_sets(0x0);
			break;
	}
	return 0;
}

static int micro2440_l298_release(struct inode * inode, struct file * file)
{
	micro2440_l298_sets(0x0);
	printk("\n\n\nMicro2440 \"l298\" driver loaded fail-----------close\n\n");
	return 0;
}
static int micro2440_l298_open(struct inode * inode, struct file * file)
{
	printk("\n\n\nMicro2440 \"l298\" driver loaded fail-----------open\n\n");
	micro2440_l298_cfg();
	micro2440_l298_sets(0x0);
	return 0;
}

static int __init micro2440_l298_init(void)
{
	int ret;
	ret = misc_register(&micro2440_l298_miscdevice);
	printk("\n\n\nMicro2440 \"l298\" driver loaded success\n\n");
	if(ret < 0)
	{
		printk("\n\n\nMicro2440 \"l298\" driver loaded fail\n\n");
	}

	return 0;
}
static void __exit micro2440_l298_exit(void)
{
	int ret;
	ret =  misc_deregister(&micro2440_l298_miscdevice);
	if(ret < 0)
	{
		printk("\n\n\nMicro2440 \"l298\" driver deregister fail ");
	}
}

module_init(micro2440_l298_init);
module_exit(micro2440_l298_exit);

MODULE_DESCRIPTION("Micro2440 l298 driver");
MODULE_AUTHOR("shaowangquan@gmail.com");
MODULE_LICENSE("GPL");


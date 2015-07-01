#include <linux/kernel.h>
#include <linux/module.h>

#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <mach/regs-gpio.h>
#include <mach/gpio-fns.h>
#include <linux/uaccess.h>

#include <linux/micro2440_leds.h>

#define MICRO2440_DEV "leds"

static int micro2440_leds_ioctl(struct inode * id, struct file * fs, 
								unsigned int i, unsigned long l);
static int micro2440_leds_close(struct inode * id, struct file * fs);
static int micro2440_leds_open (struct inode * id, struct file * fs);
static ssize_t micro2440_leds_write(struct file * fs, const char __user * user_arm, 
									size_t size, loff_t * offset);
static ssize_t micro2440_leds_read(struct file * fs, char __user * user_arm, 
									size_t size, loff_t * offset);

static const struct file_operations micro2440_leds_fops = {
	.owner = THIS_MODULE,
	.read = micro2440_leds_read,
	.write = micro2440_leds_write,
	.ioctl = micro2440_leds_ioctl,
	.open = micro2440_leds_open,
	.release = micro2440_leds_close,
};
static struct miscdevice micro2440_leds_miscdev = {
	.minor = MISC_DYNAMIC_MINOR, 
    .name = MICRO2440_DEV,
    .fops = &micro2440_leds_fops,
};

static unsigned long leds_table[] = {
	S3C2410_GPB(5),
	S3C2410_GPB(6),
	S3C2410_GPB(7),
	S3C2410_GPB(8),
};

static void micro2440_sets(unsigned long stat)
{
	int i;

	for(i = 0; i < ARRAY_SIZE(leds_table); i++)
	{
		s3c2410_gpio_setpin(leds_table[i], ((~stat) >> i) & 0x01);
	}
}

static unsigned long micro2440_gets(void)
{
	int i;
	unsigned long stat = 0x0;

	for(i = 0; i < ARRAY_SIZE(leds_table); i++)
	{
		stat |= ((!s3c2410_gpio_getpin(leds_table[i])) << i);
	}

	return stat;
}
static int micro2440_leds_ioctl(struct inode * inode, struct file * file, 
								unsigned int cmd, unsigned long arg)
{
	struct leds_stat one_stat;

	switch(cmd)
	{
		case LEDS_ALL_ON:
			micro2440_sets(0xFFFFFFFF);
			break;
		case LEDS_ALL_OFF:
			micro2440_sets(0x00000000);
			break;
		case LEDS_ALL_CHA:
			micro2440_sets(~micro2440_gets());
			break;
		case LEDS_SET_ONE:
			copy_from_user(&one_stat, (void __user *)arg, sizeof(one_stat));
			if(one_stat.led_nr >= ARRAY_SIZE(leds_table))
				return -1;
			s3c2410_gpio_setpin(leds_table[one_stat.led_nr], one_stat.led_st);
			break;
		case LEDS_GET_ONE:
			copy_from_user(&one_stat, (void __user *)arg, sizeof(one_stat));
			if(one_stat.led_nr >= ARRAY_SIZE(leds_table))
				return -1;
			one_stat.led_st = (s3c2410_gpio_getpin(leds_table[one_stat.led_nr])) 
						? LED_OFF : LED_ON;
			copy_to_user((void __user *)arg, &one_stat, sizeof(one_stat));
			break;
	}
	return 0;
}

static int micro2440_leds_close(struct inode * id, struct file * fs)
{
 	micro2440_sets(0x00000000);
	return 0;
}

static int micro2440_leds_open (struct inode * id, struct file * file)
{ 
	micro2440_sets(0xFFFFFFFF);
	return 0;
}

static ssize_t micro2440_leds_write(struct file * file, const char __user * buf, 
									size_t len, loff_t * off)
{
	unsigned long  stat;

	copy_from_user(&stat, buf, len);
	if(len < sizeof(stat)) return -1;
	micro2440_sets(stat);
	return sizeof(stat);
}

static ssize_t micro2440_leds_read(struct file * file, char __user * buf, 
									size_t len, loff_t * off) 
{
	unsigned long stat = 0UL;

	if(len < sizeof(stat)) return -1;

	stat = micro2440_gets();
	copy_to_user(buf, &stat, sizeof(stat));

	return sizeof(stat);
}


static int __init micro2440_leds_init(void)
{
	int i,  ret;
	for(i = 0; i < ARRAY_SIZE(leds_table); i++)
	{
		s3c2410_gpio_cfgpin(leds_table[i], S3C2410_GPIO_OUTPUT);		
	}
	ret = misc_register(&micro2440_leds_miscdev);	
	if(ret < 0)
	{
		printk("\n\n\n Micro2440 leds loaded fail...........\n");
	}
	printk("\n\n\n Micro2440 leds loaded success..........\n");
	return 0;
}

static void __exit micro2440_leds_exit(void)
{
	if(misc_register(&micro2440_leds_miscdev) > 0)
		printk("***************Colse micro2440 leds driver\n");
}

module_init(micro2440_leds_init);
module_exit(micro2440_leds_exit);

MODULE_DESCRIPTION("Micro2440 leds dricer");
MODULE_AUTHOR("shaowangquan@gmail.com");
MODULE_LICENSE("GPL");


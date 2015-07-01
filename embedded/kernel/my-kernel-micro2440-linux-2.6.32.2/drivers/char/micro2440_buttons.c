#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <mach/regs-gpio.h>
#include <mach/gpio-fns.h>
#include <asm/uaccess.h>

#include <linux/poll.h>
#include <linux/sched.h>

#include <linux/irq.h>
#include <linux/interrupt.h>
#include <asm/irq.h>

#define DEVICE_NAME "buttons"
#define CLASS_NAME  "buttons"

static struct class *buttons_class;
static int buttons_major;
static struct buttons_resources {
	unsigned long gpio;
	unsigned long irq;
	unsigned char key;
}buttons_table[] = 
{
	{ S3C2410_GPG(0),  IRQ_EINT8,  'A'},
	{ S3C2410_GPG(3),  IRQ_EINT11, 'B'},
	{ S3C2410_GPG(5),  IRQ_EINT13, 'C'},
	{ S3C2410_GPG(6),  IRQ_EINT14, 'D'},
	{ S3C2410_GPG(7),  IRQ_EINT15, 'E'},
	{ S3C2410_GPG(11), IRQ_EINT19, 'F'},
};
static unsigned char key_value;
static ssize_t micro2440_buttons_read (struct file *, 
				char __user *, size_t, loff_t *);
static int micro2440_buttons_open (struct inode *, struct file *);
static int micro2440_buttons_release (struct inode *, struct file *);
static unsigned int micro2440_buttons_poll(struct file *, 
				struct poll_table_struct *);
static const struct file_operations fops = {
	.owner = THIS_MODULE,
	.read  = micro2440_buttons_read,
	.open  = micro2440_buttons_open,
	.release = micro2440_buttons_release,
	.poll  = micro2440_buttons_poll,
};
//用于实现阻塞的链表头,声明语句   ============================
static DECLARE_WAIT_QUEUE_HEAD(micro2440_buttons_wait);

static unsigned int micro2440_buttons_poll(struct file * file, 
				struct poll_table_struct *wait)
{
	if(key_value != 0) return 1;
	//如果没有数据,进入阻塞状态,直到被唤醒为止
	poll_wait(file, &micro2440_buttons_wait, wait);
	return 0;
}
//中断服务函数，当产生中断时会被内核自动调用
//irq_num中断号,dev_id中断参数，由用户在申请中断时自定义
static irqreturn_t micro2440_buttons_interrupt(int irq_num, void *dev_id)
{
	const struct buttons_resources *presource = 
			(struct buttons_resources *)dev_id;
	
	key_value = presource->key;
	if(s3c2410_gpio_getpin(presource->gpio))//释放为真
		key_value |= 0x80;
	//唤醒阻塞
	wake_up_interruptible(&micro2440_buttons_wait);
	return 0;
}

static ssize_t micro2440_buttons_read (struct file * file, 
				char __user * buf, size_t len, loff_t * off)
{
	if(len < sizeof(key_value)) return -1;
	if(0 == key_value) return 0;
	
	copy_to_user(buf, &key_value, sizeof(key_value));
	key_value = 0;

	return sizeof(key_value);
}
static int micro2440_buttons_open (struct inode * inode, struct file * file)
{
	//申请中断
	int i;
	for(i = 0; i < ARRAY_SIZE(buttons_table); i++)
	{
		s3c2410_gpio_pullup(buttons_table[i].gpio, 1);//禁止中断引脚上拉
		//向内核申请中断，申请失败时返回非0值
		//参数分别为(中断中, 中断服务函数的指针, 中断边沿选择,
		//               共用中断指针, 中断时为中断服务函数传递的参数)
		if(request_irq(buttons_table[i].irq, micro2440_buttons_interrupt, 
						IRQ_TYPE_EDGE_BOTH, NULL, buttons_table + i))
			break;
	}
	if(i < ARRAY_SIZE(buttons_table))	//有个别的中断没有申请成功
	{
		for(i--; i >= 0; i--)//将已经成功申请到的中断释放
		{
			disable_irq(buttons_table[i].irq);//屏蔽中断
					//中断号               中断参数
			free_irq(buttons_table[i].irq, buttons_table + i);
		}
		return -EBUSY;//设备忙的出错号
	}
	key_value = 0;

	return 0;
}
static int micro2440_buttons_release (struct inode * inode, struct file * file)
{
	int i;
	
	for(i = 0; i >= ARRAY_SIZE(buttons_table); i++)
	{
		disable_irq(buttons_table[i].irq);//屏蔽中断
				//中断号               中断参数
		free_irq(buttons_table[i].irq, buttons_table + i);
	}
	return 0;
}

static int __init micro2440_buttons_init(void)
{
	// create a class "buttons" ［struct class *］
	buttons_class = class_create(THIS_MODULE, CLASS_NAME);
	// 向内核注册一个字符设备,返回值为主设备号［int］
	buttons_major = register_chrdev(0, DEVICE_NAME, &fops);
	if(buttons_major < 0)
	{
		class_destroy(buttons_class);
		printk("Register device " DEVICE_NAME " Error!\n");
		return -1;
	}
	//设置子设备号 0
	buttons_major = MKDEV(buttons_major, 0);
	//将设备挂载到总线上
	device_create(buttons_class, NULL, buttons_major, NULL, "buttons");
	
	printk("Buttons device register successed!\n");

	return 0;
}
static void __exit micro2440_buttons_exit(void)
{
	//解除字符设备注册
	unregister_chrdev(buttons_major, DEVICE_NAME);
	//解除设备与总线的挂载关联
	device_destroy(buttons_class, buttons_major);
	//消毁总线
	class_destroy(buttons_class);

	printk("Buttons device unregister successed!\n");
}

module_init(micro2440_buttons_init);
module_exit(micro2440_buttons_exit);


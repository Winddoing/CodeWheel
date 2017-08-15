/*#############################################################
 *     File Name	: block_rw.c
 *     Author		: wqshao
 *     Created Time	: 2017年03月25日 星期六 14时47分10秒
 *     Description	:
 *############################################################*/

#include <linux/miscdevice.h>   
#include <linux/delay.h>   
#include <asm/irq.h>   
#include <mach/hardware.h>   
#include <linux/kernel.h>   
#include <linux/module.h>   
#include <linux/init.h>   
#include <linux/mm.h>   
#include <linux/fs.h>   
#include <linux/types.h>   
#include <linux/delay.h>   
#include <linux/moduleparam.h>   
#include <linux/slab.h>   
#include <linux/errno.h>   
#include <linux/ioctl.h>   
#include <linux/cdev.h>   
#include <linux/string.h>   
#include <linux/list.h>   
#include <linux/pci.h>   
#include <asm/uaccess.h>   
#include <asm/atomic.h>   
#include <asm/unistd.h>   
#include <linux/major.h>   

#include <mach/map.h>   
#include <mach/regs-clock.h>   
#include <mach/regs-gpio.h>   

#include <plat/gpio-cfg.h>   
#include <mach/gpio-bank-e.h>   
#include <mach/gpio-bank-k.h>   
#include <mach/gpio-bank-h.h>   
#include <mach/gpio-bank-n.h>   
#include <mach/gpio-bank-l.h>   
#include <mach/gpio-bank-p.h>   

#include <linux/device.h>   

#include <linux/jiffies.h>   
#include <linux/string.h>   

#include <linux/semaphore.h>   
#include <linux/sched.h>    
#include <linux/wait.h>   

#define DEVICE_NAME "test_driver"   
#define T_MAJORS    800   

//设备结构   
static struct _Test_Driver_Device  
{  
	struct cdev fun_cdev;  
	//定义缓冲   
	unsigned char *buffer,*end;  
	//读写指针   
	unsigned char *rp,*wp;  
	//读信号量   
	struct semaphore sem_r;  
	//写信号量   
	struct semaphore sem_w;  
	//等待队列头   
	struct wait_queue_head_t *wq;  
};  
struct _Test_Driver_Device *Test_Driver_Device;  

static dev_t dev;  
static struct class    *test_class;  

//开辟缓存，用来读写   
#define LEN_BUF 256   
static unsigned char Buffer[LEN_BUF];  

//功能：初始化缓存   
static void init_buf(void)  
{  
	memset(Buffer,0,LEN_BUF);  
}  

//功能：读取缓存   
//返回：读取的字节数   
ssize_t test_driver_read(struct file *filp,char __user *buf,size_t count,loff_t *f_pos)  
{  
	int temp1 = 0,temp2 = 0,temp3 = 0;  

	//获取信号量   
	if (down_interruptible(&Test_Driver_Device->sem_r))  
	{  
		return -ERESTARTSYS;  
	}  

	//循环防止解除阻塞时的竞争   
	while (Test_Driver_Device->wp == Test_Driver_Device->rp)  
	{  
		//读不到数据   
		//释放信号量   
		up(&Test_Driver_Device->sem_r);  

		//判断是否是非阻塞读   
		if (filp->f_flags & O_NONBLOCK)  
		{  
			return -EAGAIN;  
		}  

		//如果是阻塞式读，则阻塞   
		if (wait_event_interruptible(Test_Driver_Device->wq,Test_Driver_Device->wp != Test_Driver_Device->rp))  
		{  
			return -ERESTARTSYS;  
		}  

		//获取信号量   
		if (down_interruptible(&Test_Driver_Device->sem_r))  
		{  
			return -ERESTARTSYS;  
		}  
	}  

	if (Test_Driver_Device->wp > Test_Driver_Device->rp)  
	{  
		count = min(count,(size_t)(Test_Driver_Device->wp - Test_Driver_Device->rp));  
		//拷贝数据到用户空间   
		if (copy_to_user(buf,Test_Driver_Device->rp,count))  
		{  
			return -EFAULT;  
		}  
		Test_Driver_Device->rp += count;  
	}  
	else  
	{  
		temp1 = Test_Driver_Device->end - Test_Driver_Device->rp + 1;  
		temp2 = Test_Driver_Device->wp - Test_Driver_Device->buffer;  
		if (count <= temp1)  
		{  
			//拷贝数据到用户空间   
			if (copy_to_user(buf,Test_Driver_Device->rp,count))  
			{  
				return -EFAULT;  
			}  
			Test_Driver_Device->rp += count;  
		}  
		else  
		{  
			//拷贝数据到用户空间   
			if (copy_to_user(buf,Test_Driver_Device->rp,temp1))  
			{  
				return -EFAULT;  
			}  
			Test_Driver_Device->rp = Test_Driver_Device->buffer;  
			temp3 = min(count - temp1,temp2);  
			count = temp1 + temp3;  
			if (copy_to_user(buf + temp1,Test_Driver_Device->rp,temp3))  
			{  
				return -EFAULT;  
			}  
			Test_Driver_Device->rp += temp3;  
		}  
	}  

	if (Test_Driver_Device->rp == Test_Driver_Device->end + 1)  
	{  
		Test_Driver_Device->rp = Test_Driver_Device->buffer;  
	}  
	//释放信号量   
	up(&Test_Driver_Device->sem_r);  
	printk (DEVICE_NAME"\tjdh:rp zhi zhen = %d\n",Test_Driver_Device->rp - Test_Driver_Device->buffer);  

	return count;  
}  

//功能：写入缓存   
//返回：写入的字节数   
ssize_t test_driver_write(struct file *filp,const char __user *buf,size_t count,loff_t *f_pos)  
{  
	int temp1 = 0,temp2 = 0;;  

	//判断需要写入的字节数是否大于缓存   
	if (count > LEN_BUF) {  
		return -ENOMEM;  
	}  

	//获取信号量   
	if (down_interruptible(&Test_Driver_Device->sem_w)) {  
		return -ERESTARTSYS;  
	}  

	//写入缓存   
	if (count <= (Test_Driver_Device->end - Test_Driver_Device->wp + 1)) {  
		//从用户空间拷贝数据   
		if (copy_from_user(Test_Driver_Device->wp,buf,count)) {  
			return -EFAULT;  
		}  
		Test_Driver_Device->wp += count;  
	} else {  
		temp1 = Test_Driver_Device->end - Test_Driver_Device->wp + 1;  
		temp2 = count - temp1;  
		//从用户空间拷贝数据   
		if (copy_from_user(Test_Driver_Device->wp,buf,temp1)) {  
			return -EFAULT;  
		}  
		Test_Driver_Device->wp = Test_Driver_Device->buffer;  
		//从用户空间拷贝数据   
		if (copy_from_user(Test_Driver_Device->wp,buf + temp1,temp2)) {  
			return -EFAULT;  
		}  
		Test_Driver_Device->wp += temp2;  
	}  

	if (Test_Driver_Device->wp == Test_Driver_Device->end + 1) {  
		Test_Driver_Device->wp = Test_Driver_Device->buffer;  
	}  
	//唤醒阻塞进程   
	wake_up_interruptible(&Test_Driver_Device->wq);  
	//释放信号量   
	up(&Test_Driver_Device->sem_w);  
	printk (DEVICE_NAME"\tjdh:wp zhi zhen = %d\n",Test_Driver_Device->wp - Test_Driver_Device->buffer);  

	return count;  
}  

static struct file_operations io_dev_fops = {  
	.owner = THIS_MODULE,  
	.write = test_driver_write,  
	.read = test_driver_read,  
};  

static int __init dev_init(void)  
{  
	int ret;  
	unsigned temp;  

	init_buf();  

	//分配结构体   
	Test_Driver_Device = kmalloc(sizeof(struct _Test_Driver_Device),GFP_KERNEL);  
	if (!Test_Driver_Device)  
	{  
		unregister_chrdev_region(dev,1);  
		device_destroy(test_class, dev);   
		class_destroy(test_class);  

		return -ENOMEM;  
	}  

	//定义缓冲的开始和结束的指针   
	Test_Driver_Device->buffer = Buffer;  
	Test_Driver_Device->end = Buffer + LEN_BUF - 1;  
	Test_Driver_Device->rp = Test_Driver_Device->buffer;  
	Test_Driver_Device->wp = Test_Driver_Device->buffer;  
	//初始化读信号量   
	sema_init(&Test_Driver_Device->sem_r,1);  
	//初始化写信号量   
	sema_init(&Test_Driver_Device->sem_w,1);  
	//初始化等待队列头   
	init_waitqueue_head(&Test_Driver_Device->wq);  

	dev = MKDEV(T_MAJORS,0);  
	cdev_init(&Test_Driver_Device->fun_cdev,&io_dev_fops);  
	ret = register_chrdev_region(dev,1,DEVICE_NAME);  
	if (ret < 0) return 0;  
	ret = cdev_add(&Test_Driver_Device->fun_cdev,dev,1);  
	if (ret < 0) return 0;  

	printk (DEVICE_NAME"\tjdh:test_driver initialized!!\n");  

	test_class = class_create(THIS_MODULE, "test_class1");   
	if (IS_ERR(test_class))   
	{   
		printk(KERN_INFO "create class error\n");   
		return -1;   
	}   
	device_create(test_class, NULL, dev, NULL, "test_driver");   

	return ret;  
}  

static void __exit dev_exit(void)  
{  
	unregister_chrdev_region(dev,1);  

	device_destroy(test_class, dev);   
	class_destroy(test_class);  
}  

module_init(dev_init);  
module_exit(dev_exit);  
MODULE_LICENSE("GPL");  
MODULE_AUTHOR("JDH");  

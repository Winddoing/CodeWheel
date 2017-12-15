#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
//#include <linux/fs.h> //warning
#include <linux/fcntl.h>
#include <asm/uaccess.h>
#include <asm/ioctl.h>
#include <asm/io.h>
#include <linux/ioport.h>
#include <linux/slab.h>
#include <linux/interrupt.h>
#include <plat/irqs.h>
#include <linux/workqueue.h>
#include <linux/timer.h>
#include <linux/wait.h>
#include <asm/atomic.h>
#include <linux/delay.h>
#include <linux/poll.h>
#include <linux/platform_device.h>

#define DEV_NUMBER  1
#define BUF_SIZE    64
#define POWER_ON    1
#define POWER_DOWN  0



#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

struct resource *res1 ,*res2,*res3,*res4,*res5;
struct resource *ress1 ,*ress2,*ress3,*ress4,*ress5;
unsigned long *gpj2c;
unsigned long *gpj2d;
unsigned long *gpj2p;
unsigned long *gph2d;
unsigned long *gph3d;

int major = 0;
int minor = 0;
int pin = 1;
int updown = 3;

struct fasync_struct *gec_fasync;

module_param(major,int,0);
MODULE_PARM_DESC(major, "char device major number");
module_param(minor,int,0);
MODULE_PARM_DESC(minor, "char device minor number");

struct char_dev{
	char *buffer;
	int cnt;
	int power_status;
	struct cdev cdev;
}*chardev;

#define CHAR_IOC_MAGIC 'k'
#define CHAR_READ_POWSTA _IOR(CHAR_IOC_MAGIC, 1, char *)
#define CHAR_SET_POWSTA _IOW(CHAR_IOC_MAGIC, 2, char *)

#define CHAR_IOC_MAXNR 2


struct timer_list gec_timer;
spinlock_t gec_lock = SPIN_LOCK_UNLOCKED;

int key_press = 0;
DECLARE_WAIT_QUEUE_HEAD(gec_wq);

DECLARE_MUTEX(gec_sem);
	
struct gec_key {
	int key;
	int irq;
	char *name;
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


int flag = 0;


atomic_t gec_ato = ATOMIC_INIT(1);

	
void gectasklet_func(unsigned long data)
{
	printk("%s function is called.\n",__FUNCTION__);
	
}

DECLARE_TASKLET(gec_tasklet, gectasklet_func, 0);


void gecwork_func(struct work_struct *work)
{
	printk("%s function is called.\n",__FUNCTION__);
}

DECLARE_WORK(gec_work, gecwork_func);

void gectimer_func(unsigned long data)
{
	int new_updown;
	printk("%s function is called.\n",__FUNCTION__);

	if(pin < 4)
	new_updown  = (*gph2d  >> pin) & 0x1 ;

	if(pin > 3)
	new_updown  = (*gph3d  >> pin) & 0x1 ;
		
	if(updown == new_updown)
		{
			printk("key down.\n");
			chardev->buffer[0] = pin; 
			key_press = 1;
			wake_up_interruptible(&gec_wq);
			kill_fasync(&gec_fasync, SIGIO, POLL_IN);
	    }
	
}



int char_open(struct inode *inode, struct file *filp)
{
	struct char_dev *dev;
	
#if 0
	if(atomic_dec_and_test(&gec_ato))
		{
		
			dev = container_of(inode->i_cdev, struct char_dev, cdev);
			filp->private_data = dev;

			dev->cnt = 0;

			dev->power_status = POWER_ON;

			

			return 0;
		}
	else
		{
			atomic_inc(&gec_ato);
			return -EBUSY;
		}

#endif

			//down_interruptible(&gec_sem);
			spin_lock_irq(&gec_lock);
			printk("%s function is called.\n",__FUNCTION__);

			dev = container_of(inode->i_cdev, struct char_dev, cdev);
			filp->private_data = dev;
			dev->cnt = 0;
			dev->power_status = POWER_ON;

			spin_unlock_irq(&gec_lock);
			return 0;


}

int char_release(struct inode *inode, struct file *filp)
{
	struct char_dev *dev;
	 
	printk("%s function is called.\n", __FUNCTION__);
	
	dev = container_of(inode->i_cdev, struct char_dev, cdev);
	dev->power_status = POWER_DOWN;	

	//atomic_inc(&gec_ato);
	//up(&gec_sem);
	//spin_unlock(&gec_lock);
	
	return 0;
}

ssize_t char_read(struct file *filp, char __user *buf, size_t count,loff_t *f_pos)
{
	struct char_dev *dev = filp->private_data;
	int cnt;

	wait_event_interruptible(gec_wq, key_press);

	printk("%s function is called.\n",__FUNCTION__);

#if 0
	if(!dev->cnt){
		printk("no data to read.\n");
		return -EFAULT;
	}


	if(dev->power_status == POWER_ON){
#endif
		
	//	cnt = min(dev->cnt, min((int)count, BUF_SIZE));
		if(copy_to_user(buf, chardev->buffer, count)){
			printk("copy to user space failed.\n");
			return -EFAULT;
		}
#if 0
		
	}else{
		printk("char device is power down,\n");
		cnt = -1;
	}
#endif
	key_press = 0;
	return cnt;
}

ssize_t char_write(struct file *filp, const char __user *buf, size_t count,loff_t *f_pos)
{
	struct char_dev *dev = filp->private_data;
	int cnt;	

	printk("%s function is called.\n",__FUNCTION__);

	if(dev->power_status == POWER_ON){
		memset(dev->buffer, '\0', BUF_SIZE);

		cnt = min((int)count, BUF_SIZE);
		if(copy_from_user(dev->buffer, buf, cnt)){
			printk("copy from user space failed.\n");
			return -EFAULT;
		}
		
		dev->cnt = cnt;
	}else{
		printk("char device is power down.\n");
		cnt = -1;
	}



	return cnt;
}

int char_ioctl(struct inode *inode, struct file *filp,unsigned int cmd, unsigned long arg)
{
	struct char_dev *dev = filp->private_data;
	int retval = 0;

	printk("%s function is called.\n",__FUNCTION__);

	switch(cmd)
	{
		case 1:			
		     if(arg)
			 	*gpj2d = *gpj2d  &  ~(0x1 << 0);
			 	else
				*gpj2d = *gpj2d | (0x1 << 0);
			break;

		case 3:
		     if(arg)
			 	*gpj2d = *gpj2d  &  ~(0x1 << 1);
			 	else
				*gpj2d = *gpj2d | (0x1 << 1);	
			break;

		default:
			printk("no such command.\n");
			return -EFAULT;
	}		
	return retval;
}


 unsigned int char_poll(struct file *filp, struct poll_table_struct *poll_table)
{
	unsigned long mask = 0;
	poll_wait(filp, &gec_wq, poll_table);

	if(key_press)
		{
			mask = POLLIN | POLLRDNORM;
		}
		
	return mask;
}

int char_fasync(int fd, struct file *filp, int on)
{
	int ret;
	ret = fasync_helper(fd, filp, on, &gec_fasync);
	return ret;
}


static struct file_operations char_fops = {
	.owner = THIS_MODULE,
	.read = char_read,
	.write = char_write,
	.ioctl = char_ioctl,
	.open = char_open,
	.release = char_release,
	.poll = char_poll,
	.fasync = char_fasync,
};

irqreturn_t  gec_handler(int irq, void *data)
{

	printk(KERN_INFO "gec_handler\n");
	pin =  *(int *)data;
	printk(KERN_INFO "gec_handler  pin = %d\n",pin);
	//tasklet_schedule(&gec_tasklet);
	//schedule_work(&gec_work);
	if(pin < 4)
	updown  = (*gph2d  >> pin) & 0x1 ;

	if(pin > 3)
	updown  = (*gph3d  >> pin) & 0x1 ;
	
	mod_timer(&gec_timer, jiffies + 10);
	return IRQ_HANDLED;
}



static int __devinit gec_key_probe(struct platform_device *pdev)
{
	int result,i;
	dev_t devno;
	int err,ret;

	printk("%s is called.\n",__FUNCTION__);

	if(major){
		devno = MKDEV(major,minor);
		result = register_chrdev_region(devno, DEV_NUMBER, "char_dev");			
	}else{
		result = alloc_chrdev_region(&devno, minor, DEV_NUMBER, "char_dev");
		major = MAJOR(devno);
	}
	if(result < 0){
		printk(KERN_WARNING "char device can't get major.\n");
		return result;
	}	

	chardev = (struct char_dev *)kzalloc(sizeof(struct char_dev), GFP_KERNEL);
	chardev->buffer = (char *)kzalloc(BUF_SIZE, GFP_KERNEL);
	chardev->buffer[0] = 1; 
	if(chardev == NULL || chardev->buffer == NULL){
		unregister_chrdev_region(devno, DEV_NUMBER);
		return -ENOMEM;
	}

	cdev_init(&chardev->cdev, &char_fops);
	chardev->cdev.owner = THIS_MODULE;
	err = cdev_add(&chardev->cdev, devno, 1);
	if(err)
		printk(KERN_NOTICE "Error adding cdev.\n");

	ress1 = platform_get_resource(pdev,IORESOURCE_MEM,0);
	ress2 = platform_get_resource(pdev,IORESOURCE_MEM,1);
	ress3 = platform_get_resource(pdev,IORESOURCE_MEM,2);
	ress4 = platform_get_resource(pdev,IORESOURCE_MEM,3);
	ress5 = platform_get_resource(pdev,IORESOURCE_MEM,4);


	res1 =  request_mem_region(ress1->start, 4,ress1->name );
	res2 =  request_mem_region(ress2->start, 4,ress2->name );
	res3 =  request_mem_region(ress3->start, 4,ress3->name );
	res4 =  request_mem_region(ress4->start, 4,ress4->name );
	res5 =  request_mem_region(ress5->start, 4,ress5->name );

	if( !res1  || !res2  || !res3 || !res4 || !res5)
		goto err;
	
	gpj2c = ioremap(ress1->start, 4);
	gpj2d = ioremap(ress2->start, 4);
	gpj2p = ioremap(ress3->start, 4);
	gph2d = ioremap(ress4->start, 4);
	gph3d = ioremap(ress5->start, 4);
	
	if( !gpj2c  || !gpj2d  || !gpj2p || !gph2d  || !gph3d )
		goto err2;

	*gpj2c = (*gpj2c  & ~(0xFFFF << 0x0)) | (0x1111 << 0x0);
	*gpj2p = *gpj2p  &  ~(0xFF << 0);
	*gpj2d = *gpj2d  &  ~(0xFF << 0);


	for(i=0;i<ARRAY_SIZE(button);i++)
	ret = request_irq(button[i].irq, gec_handler, IRQF_DISABLED | IRQF_TRIGGER_FALLING, button[i].name, &button[i].key);

	if(ret)
		goto err3;

	init_timer(&gec_timer);
	gec_timer.data = 0;
	gec_timer.expires = jiffies + 5;
	gec_timer.function = gectimer_func,
	add_timer(&gec_timer);

	return 0;

err:
	printk(KERN_ERR "Error request_mem_region.\n");
	return -EFAULT;
err2:
	printk(KERN_ERR "Error ioremap.\n");
	return -EFAULT;	
err3:
	printk(KERN_ERR "Error request_irq.\n");
	return -EFAULT;	
}

static int __devexit gec_key_remove(struct platform_device *pdev)
{
	int i;
	dev_t devno = MKDEV(major,minor);

	printk("%s is called.\n",__FUNCTION__);
	
	if(chardev){
		kfree(chardev->buffer);
		cdev_del(&chardev->cdev);
		kfree(chardev);
	}
	unregister_chrdev_region(devno, DEV_NUMBER);
	*gpj2d = *gpj2d | (0xFF << 0);
	release_mem_region(ress1->start, 4);
	release_mem_region(ress2->start, 4);
	release_mem_region(ress3->start, 4);
	release_mem_region(ress4->start, 4);
	release_mem_region(ress5->start, 4);
	iounmap(gpj2c);
	iounmap(gpj2d);
	iounmap(gpj2p);
	iounmap(gph2d);
	iounmap(gph3d);

	for(i=0;i<ARRAY_SIZE(button);i++)
	free_irq(button[i].irq, &button[i].key);
	tasklet_kill(&gec_tasklet);
	del_timer(&gec_timer);

	return 0;
}

static struct platform_driver geckey_key_driver = {
	.driver	= {
		.name    = "gec_key",
		.owner	 = THIS_MODULE,
	},
	.probe   = gec_key_probe,
	.remove  = __devexit_p(gec_key_remove),
};





static int __init char_init(void)
{
	printk(KERN_INFO "char_init\n");
	return platform_driver_register(&geckey_key_driver);
}

static void __exit char_exit(void)
{
	platform_driver_unregister(&geckey_key_driver);

}
module_init(char_init);
module_exit(char_exit);

MODULE_AUTHOR("lvling <lv__ling@126.com>");
MODULE_DESCRIPTION("char device module.");
MODULE_LICENSE("GPL");

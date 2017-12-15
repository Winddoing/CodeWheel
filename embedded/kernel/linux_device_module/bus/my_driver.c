#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/device.h>
#include "my_device.h"



static unsigned int drv_value = 100;


static ssize_t drv_value_show(struct device_driver *driver, char *buf)
{
	return snprintf(buf, PAGE_SIZE, "%d\n", drv_value);
}

static ssize_t drv_value_store(struct device_driver *driver, const char *buf,size_t count)
{
	int value;

	if (sscanf(buf, "%d\n", &value) != 1 || value < 0 || value> 1000 )
		return -EINVAL;
	drv_value= value;
	return count;
}

static DRIVER_ATTR(drv_value, 0644, drv_value_show, drv_value_store);



static struct driver_attribute *const my_drv_attrs[] = {
	&driver_attr_drv_value,
	NULL,
};

int my_driver_realprobe(struct my_device *dev)
{
	printk(KERN_INFO "my_driver_realprobe !\n");

	return 0;
}


int my_driver_realremove(struct my_device *dev)
{
	printk(KERN_INFO "my_driver_realremove !\n");

	return 0;
}

int my_driver_probe(struct device *dev)
{
	struct my_device *_dev  = to_my_dev(dev);
	printk(KERN_INFO "my_driver_probe !\n");

	my_driver_realprobe(_dev);
	return 0;
}
EXPORT_SYMBOL_GPL(my_driver_probe);


int my_driver_remove(struct device *dev)
{
	struct my_device *_dev  = to_my_dev(dev);
	printk(KERN_INFO "my_driver_remove !\n");

	my_driver_realremove(_dev);
	return 0;
}
EXPORT_SYMBOL_GPL(my_driver_remove);

struct my_driver my_drv ={

	.name = "my_dev",
	.devid = 0,
	.drv ={
 		.name = "my_dev",
		.bus = &my_bus,
		.probe = my_driver_probe,
		.remove = my_driver_remove,
	},
	.probe = my_driver_realprobe,
	.remove = my_driver_realremove,

};




static int __init drv_init(void)
{
	int ret,i;
	
 	  printk(KERN_INFO "goto	my_driver_register !\n");
	  ret = my_driver_register(&my_drv);
	  if(ret)
	  	goto err2;

	printk(KERN_INFO "goto	driver_create_file !\n");
	for (i = 0; my_drv_attrs[i]; i++) {
		ret= driver_create_file(&my_drv.drv, my_drv_attrs[i]);
		if (ret)
			goto err5;
		}



	return 0;
	

err2:
	printk(KERN_ERR "my_driver_register failed!\n");
	return -EFAULT;

err5:
	printk(KERN_ERR "drv_create_file failed!\n");
	return -EFAULT;

}


static void __exit drv_exit(void)
{
	int i;

	printk(KERN_INFO "goto	driver_remove_file !\n");

	for (i = 0; my_drv_attrs[i]; i++)
		driver_remove_file(&my_drv.drv, my_drv_attrs[i]);
	
	printk(KERN_INFO "goto	my_driver_unregister !\n");
	my_driver_unregister(&my_drv);

}


module_init(drv_init);
module_exit(drv_exit);


MODULE_LICENSE("GPL");

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/device.h>
#include "my_device.h"



static unsigned int dev_value = 100;



static ssize_t dev_value_show(struct device *dev, struct device_attribute *attr,char *buf)
{
	return snprintf(buf, PAGE_SIZE, "%d\n", dev_value);
}

static ssize_t dev_value_store(struct device *dev, struct device_attribute *attr,const char *buf, size_t count)
{
	int value;

	if (sscanf(buf, "%d\n", &value) != 1 || value < 0 || value> 1000 )
		return -EINVAL;
	dev_value= value;
	return count;
}

static DEVICE_ATTR(dev_value, 0644, dev_value_show, dev_value_store);



static struct device_attribute *const my_dev_attrs[] = {
	&dev_attr_dev_value,
	NULL,
};



void my_dev_release(struct device *dev)
{
	printk(KERN_ALERT "my_dev_release !\n");
}


struct my_device my_dev = {

	.name = "my_dev",
	.devid = 0,
	.dev ={
		.init_name = "my_dev",
		.release = my_dev_release,
		.bus =  &my_bus,
	},

};




static int __init dev_init(void)
{
	int ret,i;

  	  printk(KERN_INFO "goto	my_device_register !\n");
	  ret = my_device_register(&my_dev);
	  if(ret)
	  	goto err3;

	printk(KERN_INFO "goto	device_create_file !\n");

	for (i = 0; my_dev_attrs[i]; i++) {
		ret= device_create_file(&my_dev.dev, my_dev_attrs[i]);
		if (ret)
			goto err6;
		}


	return 0;
	

err3:
	printk(KERN_ERR "my_device_register failed!\n");
	return -EFAULT;

err6:
	printk(KERN_ERR "dev_create_file failed!\n");
	return -EFAULT;

}


static void __exit dev_exit(void)
{
	int i;
	
	printk(KERN_INFO "goto	device_remove_file !\n");
	for (i = 0; my_dev_attrs[i]; i++)
		device_remove_file(&my_dev.dev, my_dev_attrs[i]);

	printk(KERN_INFO "goto	my_device_unregister !\n");
	my_device_unregister(&my_dev);

}


module_init(dev_init);
module_exit(dev_exit);


MODULE_LICENSE("GPL");


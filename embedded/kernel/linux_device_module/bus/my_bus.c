#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/device.h>
#include "my_device.h"

static unsigned int bus_value = 100;


static int my_bus_match(struct device *dev, struct device_driver *drv)
{
	struct my_device *_dev = to_my_dev(dev);
	struct my_driver *_drv = to_my_drv(drv);

	//return dev->devid == drv->devid;
	return (strcmp(_dev->name,_drv->name)?0:1);
}

static int my_bus_uevent(struct device *dev, struct kobj_uevent_env *env)
{
	struct my_device *_dev = to_my_dev(dev);
	int retval = 0;
	printk(KERN_INFO "my_bus_uevent !\n");

	if (!_dev)
		return -ENODEV;
	
	printk(KERN_INFO "goto	add_uevent_var !\n");
	/* Set up DEV_TYPE environment variable. */
	retval = add_uevent_var(env, "DEV_ID=%04X", _dev->devid);
	if (retval)
		return retval;

	/* Add MODALIAS= */
	retval = add_uevent_var(env, "MODALIAS=my:%02X", _dev->devid);

	return retval;


	return 0;
}

static int my_bus_probe(struct device *dev)
{
	printk(KERN_INFO "my_bus_probe !\n");
	return 0;
}

static int my_bus_remove(struct device *dev)
{
	printk(KERN_INFO "my_bus_remove !\n");
	return 0;
}

struct bus_type my_bus ={
	   .name = "my_bus",
       .match = my_bus_match,
       .uevent =  my_bus_uevent,
       .probe  =  my_bus_probe,
       .remove = my_bus_remove,
};
EXPORT_SYMBOL_GPL(my_bus);


static ssize_t bus_value_show(struct bus_type *bus, char *buf)
{
	return snprintf(buf, PAGE_SIZE, "%d\n", bus_value);
}

static ssize_t bus_value_store(struct bus_type *bus, const char *buf, size_t count)
{
	int value;

	if (sscanf(buf, "%d\n", &value) != 1 || value < 0 || value> 1000 )
		return -EINVAL;
	bus_value= value;
	return count;
}

static BUS_ATTR(bus_value, 0644, bus_value_show, bus_value_store);



static struct bus_attribute *const my_bus_attrs[] = {
	&bus_attr_bus_value,
	NULL,
};


int my_driver_register(struct my_driver *drv)
{
	drv->drv.bus = &my_bus;
	
	if (!drv->probe)
		drv->drv.probe = my_bus_probe;
	if (!drv->remove)
		drv->drv.remove = my_bus_remove;

	return driver_register(&drv->drv);
}
EXPORT_SYMBOL_GPL(my_driver_register);


void my_driver_unregister(struct my_driver *drv)
{
	driver_unregister(&drv->drv);
}
EXPORT_SYMBOL_GPL(my_driver_unregister);



int my_device_register(struct my_device *dev)
{
	return device_register(&dev->dev);
}
EXPORT_SYMBOL_GPL(my_device_register);


void my_device_unregister(struct my_device *dev)
{
	device_unregister(&dev->dev);
}
EXPORT_SYMBOL_GPL(my_device_unregister);




static int __init bus_init(void)
{
	int ret,i;

	  printk(KERN_INFO "goto  bus_register !\n");
      ret = bus_register(&my_bus);
	  if(ret)
	  	goto err1;
	 
	printk(KERN_INFO "goto  bus_create_file !\n");
	for (i = 0; my_bus_attrs[i]; i++) {
		ret= bus_create_file(&my_bus, my_bus_attrs[i]);
		if (ret)
			goto err4;
		}
	return 0;
	
err1:
	printk(KERN_ERR "bus_register failed!\n");
	return -EFAULT;
err4:
	printk(KERN_ERR "bus_create_file failed!\n");
	return -EFAULT;

}


static void __exit bus_exit(void)
{
	int i;
	
	printk(KERN_INFO "goto  bus_remove_file !\n");	
	for (i = 0; my_bus_attrs[i]; i++)
		bus_remove_file(&my_bus, my_bus_attrs[i]);

	printk(KERN_INFO "goto	bus_unregister !\n");
	bus_unregister(&my_bus);
	
}


module_init(bus_init);
module_exit(bus_exit);


MODULE_LICENSE("GPL");




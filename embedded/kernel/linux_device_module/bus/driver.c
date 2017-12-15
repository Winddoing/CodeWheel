#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/device.h>


struct my_device {
	struct device	dev;
	unsigned int	devid;
	char *name;
};
//EXPORT_SYMBOL_GPL(my_device);

struct my_driver {
	struct device_driver	drv;
	char *name;
	unsigned int		devid;
	int (*probe)(struct my_device *);
	int (*remove)(struct my_device *);
};
//EXPORT_SYMBOL_GPL(my_driver);



static unsigned int bus_value = 100;
static unsigned int drv_value = 100;
static unsigned int dev_value = 100;



#define to_my_dev(_d)	container_of((_d), struct my_device, dev)
#define to_my_drv(_d)	container_of((_d), struct my_driver, drv)


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

	printk(KERN_INFO "goto  add_uevent_var !\n");
	/* Set up DEV_TYPE environment variable. */
	retval = add_uevent_var(env, "DEV_ID=%04X", _dev->devid);
	if (retval)
		return retval;

	/* Add MODALIAS= */
	retval = add_uevent_var(env, "MODALIAS=my:%02X", _dev->devid);

	return retval;
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




static int my_driver_register(struct my_driver *drv)
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




static int my_driver_probe(struct my_device *dev)
{
	printk(KERN_INFO "my_driver_probe !\n");
	return 0;
}

static int my_driver_remove(struct my_device *dev)
{
	printk(KERN_INFO "my_driver_remove !\n");
	return 0;
}


struct my_driver my_drv ={

	.name = "my_dev",
	.devid = 0,
	.drv ={
 		.name = "my_dev",
		.bus = &my_bus,
	},
	.probe = my_driver_probe,
	.remove = my_driver_remove,

};




static int __init bus_init(void)
{
	int ret,i;
	
      ret = bus_register(&my_bus);
	  if(ret)
	  	goto err1;
	  
	  ret = my_driver_register(&my_drv);
	  if(ret)
	  	goto err2;

	  
	  ret = my_device_register(&my_dev);
	  if(ret)
	  	goto err3;


	  
	for (i = 0; my_bus_attrs[i]; i++) {
		ret= bus_create_file(&my_bus, my_bus_attrs[i]);
		if (ret)
			goto err4;
		}
	for (i = 0; my_drv_attrs[i]; i++) {
		ret= driver_create_file(&my_drv.drv, my_drv_attrs[i]);
		if (ret)
			goto err5;
		}
	for (i = 0; my_dev_attrs[i]; i++) {
		ret= device_create_file(&my_dev.dev, my_dev_attrs[i]);
		if (ret)
			goto err6;
		}


	return 0;
	
err1:
	printk(KERN_ERR "bus_register failed!\n");
	return -EFAULT;
err2:
	printk(KERN_ERR "my_driver_register failed!\n");
	return -EFAULT;
err3:
	printk(KERN_ERR "my_device_register failed!\n");
	return -EFAULT;
err4:
	printk(KERN_ERR "bus_create_file failed!\n");
	return -EFAULT;
err5:
	printk(KERN_ERR "drv_create_file failed!\n");
	return -EFAULT;
err6:
	printk(KERN_ERR "dev_create_file failed!\n");
	return -EFAULT;

}


static void __exit bus_exit(void)
{
	int i;
	for (i = 0; my_bus_attrs[i]; i++)
		bus_remove_file(&my_bus, my_bus_attrs[i]);

	for (i = 0; my_drv_attrs[i]; i++)
		driver_remove_file(&my_drv.drv, my_drv_attrs[i]);
	
	for (i = 0; my_dev_attrs[i]; i++)
		device_remove_file(&my_dev.dev, my_dev_attrs[i]);


	my_driver_unregister(&my_drv);
	my_device_unregister(&my_dev);
	bus_unregister(&my_bus);
	

}


module_init(bus_init);
module_exit(bus_exit);


MODULE_LICENSE("GPL");

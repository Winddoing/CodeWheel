#ifndef _MY_DEVICE_
#define _MY_DEVICE_

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

#define to_my_dev(_d)	container_of((_d), struct my_device, dev)
#define to_my_drv(_d)	container_of((_d), struct my_driver, drv)


extern  int my_driver_register(struct my_driver *drv);
extern void my_driver_unregister(struct my_driver *drv);
extern int my_device_register(struct my_device *dev);
extern void my_device_unregister(struct my_device *dev);
extern struct bus_type my_bus;
extern int my_driver_remove(struct device *dev);
extern int my_driver_probe(struct device *dev);



#endif

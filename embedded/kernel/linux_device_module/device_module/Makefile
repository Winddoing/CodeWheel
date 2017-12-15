CONFIG_CHARDEV ?= m

ifneq ($(KERNELRELEASE),)
#char_simple-objs := char.o 
obj-$(CONFIG_CHARDEV) += key_drv.o key_dev.o
else
#KERNELDIR = /lib/modules/$(shell uname -r)/build/
KERNELDIR = /home/gec/android-kernel-20130623
PWD = $(shell pwd)
modules:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
	arm-none-linux-gnueabi-gcc -o io_app io_app.c
endif


install:
	cp *.ko /home/gec/mini_rootfs

clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean
	rm -rf char_app *.symvers

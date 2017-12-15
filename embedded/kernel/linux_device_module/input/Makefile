CONFIG_GEC_INPUT ?= m

ifneq ($(KERNELRELEASE),)
#char_simple-objs := char.o 
obj-$(CONFIG_GEC_INPUT) += input_key.o
else
#KERNELDIR = /lib/modules/$(shell uname -r)/build/
KERNELDIR = 
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

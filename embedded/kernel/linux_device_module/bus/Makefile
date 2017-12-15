CONFIG_MY_BUS = m


#obj-$(CONFIG_MY_BUS) := driver.o
obj-$(CONFIG_MY_BUS) := my_bus.o my_device.o my_driver.o
#obj-$(CONFIG_MY_BUS) := mdriver.o
#obj-$(CONFIG_MY_BUS) := mdriver.o

KDIR=/home/gec/build_kernel/android-kernel-20130621
ROOTFS=/home/gec/mini_rootfs

all:
	make -C $(KDIR) M=`pwd`
test:
	arm-linux-gcc test.c -o test
	cp test /home/gec/mini_rootfs
modules:	
	make -C $(KDIR) M=`pwd` modules
install:
	make -C $(KDIR) M=`pwd` INSTALL_MOD_PATH=$(ROOTFS) modules_install
clean:
	make -C $(KDIR) M=`pwd` clean

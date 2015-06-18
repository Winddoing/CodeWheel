ALL : ledstest buttonstest

KERNEL_DIR = /work/embedded/kernel/linux-2.6.32.2
CC = arm-linux-gcc
CFLAGS = -I$(KERNEL_DIR)/include -Wall -O3

ledstest : ledstest.o
	$(CC) -O3 -o $@ $^ -s
ledstest.o : ledstest.c $(KERNEL_DIR)/include/linux/micro2440_leds.h

buttonstest : buttonstest.o
	$(CC) -O3 -o $@ $^ -s
buttonstest.o : buttonstest.c

.PHONY : clean
clean :
	rm -rf ledstest.o buttonstest.o ledstest buttonstest


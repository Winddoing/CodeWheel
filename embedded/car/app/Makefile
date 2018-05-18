ALL : car.cgi car_control serial.cgi serial_control

CC = arm-linux-gcc
KERNEL_DIR = /home/linux-2.6.32.2
CFLAGS = -I$(KERNEL_DIR)/include -Wall -O3

control : car_control.o
	$(CC) -o $@ $^ -s
car.cgi : car_connect.o
	$(CC) -o $@ $^ -s
serial.cgi : serial_connect.o
	$(CC) -o $@ $^ -s
serial_control : serial_control.o
	$(CC) -o $@ $^ -s

car_control.o : car_control.c $(KERNEL_DIR)/include/linux/micro2440_l298.h
car_connect.o : car_connect.c
serial_connect.o : serial_connect.c
serial_control.o : serial_control.c

clean:
	rm -f car.cgi car_control *.o serial.cgi

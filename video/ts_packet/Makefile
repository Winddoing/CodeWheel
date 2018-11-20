CC=gcc
CFLAGS= -Wall  -g

TARGET=ts_packet
OBJS=ts_packet.o

test:ts_test_file ts_test_output

ts_test_file:ts_test_file.o ts_packet.o
	$(CC) $(CFLAGS) -o $@ $^
	
ts_test_output:ts_test_output.o ts_packet.o
	$(CC) $(CFLAGS) -o $@ $^

$(TARGET):$(OBJS)
	$(CC) $(CFLAGS) -o $@ $^
%.o:%.c
	$(CC) $(CFLAGS) -o $@ -c $^
	
clean:
	rm ts_packet ts_test_file ts_test_output
	rm *.o *.es

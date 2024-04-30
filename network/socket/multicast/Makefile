
CC = gcc
CXX = g++
CFLAGS = -Wall -D_DEBUG -DDEBUG -g -O0
LDFLAGS = 

MODULE_INC = -I/usr/include
MODULE_LIB = -L/usr/lib

CFLAGS  += $(MODULE_INC)
LDFLAGS += $(MODULE_LIB)


SENDOBJS = mcastserver.o
RECVOBJS = mcastclient.o


TARGET = mcastserver mcastclient

all: $(TARGET)

Sender: $(SENDOBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)

Recver: $(RECVOBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)

clean: 
	rm -f *.o
	rm -f $(TARGET)
	rm -f http/*.o
# make rule
%.o : %.c
	$(CC) $(CFLAGS) -c $^ -o $@	

%.o : %.cpp
	$(CC) $(CFLAGS) -c $^ -o $@	

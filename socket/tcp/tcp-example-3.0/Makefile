all : tcpclient tcpserver

CC = gcc
CFLAGS = -Wall -I. -O3

tcpclient : client.o tcp.o shell.o
	$(CC) -o $@ $^
tcpserver : server.o tcp.o shell.o readconf.o
	$(CC) -o $@ $^

client.o : client/client.c tcp.h
	$(CC) -c $< $(CFLAGS)
server.o : server/server.c tcp.h
	$(CC) -c $< $(CFLAGS)
tcp.o : tcp.c tcp.h
shell.o : shell.c shell.h
readconf.o : readconf.c readconf.h 
dest:
	tar cvfz ../tcp.tar.gz ../tcp
	mv ../tcp.tar.gz .

.PHONY : clean
clean:
	rm -f client.o tcp.o server.o shell.o readconf.o tcp.tar.gz 


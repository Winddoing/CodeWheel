CC = gcc

all: aes_c md5_c rsa_c

aes_c:aes.o aes_c.o
	$(CC) -o aes_c aes.o aes_c.o

md5_c:md5.o md5_c.o
	$(CC) -o md5_c md5.o md5_c.o

rsa_c:rsa.o rsa_c.o
	$(CC) -o rsa_c rsa.o rsa_c.o


.PHONY:clean
clean:
	@rm spl -rf
	@rm aes_c -rf
	@rm md5_c -rf
	@rm rsa_c -rf
	@rm *.o -rf

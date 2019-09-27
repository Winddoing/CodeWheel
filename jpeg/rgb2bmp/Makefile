PROGNAME=rgb2bmp

CC=gcc
CFLAGS=-g -Wall 

OBJS = main.o debug.o 

.PHONY: all clean dep superclean

all: $(PROGNAME) main.o debug.o

$(PROGNAME): $(OBJS)
	$(CC) -o $@ $(OBJS)

$(OBJS): %.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@-rm $(PROGNAME) $(OBJS)  

superclean: clean
	@-rm *~

dep:
	@makedepend -Y *.c

# DO NOT DELETE

debug.o: debug.h
main.o: debug.h

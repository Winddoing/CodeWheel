CROSS_COMPILE	?= aarch64-none-linux-gnu-
CC    = ${CROSS_COMPILE}gcc
AS    = ${CROSS_COMPILE}as
LINK  = ${CROSS_COMPILE}ld
OBJDUMP = ${CROSS_COMPILE}objdump
OBJCOPY = ${CROSS_COMPILE}objcopy

CFLAGS += -Wno-builtin-declaration-mismatch

OBJS := start.o putchar.o printf.o memset.o ctype.o cache.o

OBJDUMPFLAGS = -xdS

.PHONY : all
all : baremetal.bin

start.o: start.S
	$(CC) -c -g -o $@ $<

memset.o: memset.S
	$(CC) -c -g -o $@ $<

putchar.o: putchar.c
	$(CC) -c -g $(CFLAGS) -O2 -g -o $@ $<

printf.o: printf.c
	$(CC) -c -g $(CFLAGS) -O2 -g -o $@ $<

ctype.o: ctype.c
	$(CC) -c -g $(CFLAGS) -O2 -g -o $@ $<

cache.o: cache.c
	$(CC) -c -g $(CFLAGS) -O2 -g -o $@ $<

baremetal.elf: $(OBJS) link.lds
	$(LINK) -T link.lds $(OBJS) -o $@
	$(OBJDUMP) ${OBJDUMPFLAGS} $@ > baremetal.asm

baremetal.bin: baremetal.elf
	$(OBJCOPY) -O binary -j .text -j .rodata -j .data $< $@

.PHONY : clean
clean:
	-rm -f baremetal.bin $(OBJS) *.elf *.asm


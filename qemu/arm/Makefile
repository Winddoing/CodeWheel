CROSS_COMPILE ?= aarch64-none-linux-gnu-

test.elf: test.o
	$(CROSS_COMPILE)ld -Ttext 0x0  -o test.elf   $^

%.o : %.S
	$(CROSS_COMPILE)gcc  -o $@ $<  -g  -c

clean:
	rm *.o *.elf    -f

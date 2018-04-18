obj = led_rgb
cc = mips-linux-gnu-
LDFLGAG = --static
ALL:
	$(cc)gcc protocol.c udp.c led.c main.c -O3 -o $(obj) $(LDFLGAG)
	$(cc)strip $(obj)
clean:
	rm -f $(obj)

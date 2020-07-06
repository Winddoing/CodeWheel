
CFLAGS = -Wall

PROGRAMS = server client
DOMAIN_SOCK = unix_socket

all: $(PROGRAMS)

%: %.c
	$(CC) $(CFLAGS) $< -o $@.out

.PHONY: clean
clean:
	rm -f $(PROGRAMS) *.out
	rm -f $(DOMAIN_SOCK)

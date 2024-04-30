SHELL=/bin/sh
RM=rm -f

CFLAGS?=-Wall

BINARY=igmpqd.out
SRCS=igmpqd.c daemon.c logging.c
HDRS=daemon.h logging.h

all: $(BINARY)

$(BINARY): $(SRCS) $(HDRS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRCS) -o $(BINARY)

install: $(BINARY)
	install -d $(PREFIX)/sbin
	install -m 755 -t $(PREFIX)/sbin $(BINARY)

clean:
	$(RM) $(BINARY)

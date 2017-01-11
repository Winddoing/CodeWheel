
PREFIX := mips-linux-gnu-
CC := $(PREFIX)gcc
OBJ :=$(PREFIX)objdump

all: inline_test.s inline_test+O2.s inline_test_dump.s inline_test_dump+O2.s


inline_test.i : inline_test.c
	$(CC) -E $^ -o $@

inline_test.s : inline_test.i
	$(CC) -S $^ -o $@

inline_test+O2.s : inline_test.i
	$(CC) -S $^ -o $@ -O2

inline_test+O2 : inline_test.c
	$(CC)  $^ -o $@ -O1

inline_test : inline_test.c
	$(CC)  $^ -o $@

inline_test_dump+O2.s : inline_test+O2
	$(OBJ) -d $^ > $@

inline_test_dump.s : inline_test
	$(OBJ) -d $^ > $@


clean:
	rm inline_test.i inline_test.s inline_test inline_test_dump.s inline_test+O2.s inline_test+O2 inline_test_dump+O2.s

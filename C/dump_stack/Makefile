
all:
	gcc test.c -o test.out -g -rdynamic -O0
	objdump -d test.out > test.s

clean:
	rm test.out test.s

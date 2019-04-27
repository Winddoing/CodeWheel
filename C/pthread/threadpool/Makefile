
all:
	gcc test.c  threadpool.c -o test.out -lpthread
	#astyle --style=linux --indent=force-tab=4 --convert-tabs --pad-oper --unpad-paren --break-blocks=all -n *.c *.h
	astyle --style=linux --indent=force-tab=4 --convert-tabs --pad-oper -n *.c *.h


clean:
	rm -rf test.out

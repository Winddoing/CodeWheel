


all:
	gcc -fPIC -c count.c
	gcc -shared count.o -o libcount.so
	./libpwd.sh
	gcc -o main main.c -ldl -lpthread


clean:
	rm -rf count.o libcount.so main libcount.h

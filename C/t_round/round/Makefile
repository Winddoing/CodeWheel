round : circle.o area.o volume.o main.o
	gcc -o round circle.o area.o volume.o main.o -lm

circle.o : circle.c
	gcc -c circle.c -Wall -O3
area.o : area.c
	gcc -c area.c -Wall -O3
volume.o : volume.c
	gcc -c volume.c -Wall -O3
main.o : main.c round.h
	gcc -c main.c -Wall -O3 -I.

.PHONY : clean
clean :
	rm -f circle.o area.o volume.o main.o




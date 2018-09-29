
all:
	gcc pthread_create.c -o pthread_create -lpthread
	gcc create.c -o create -lpthread
	gcc atom_tst.c -o atom_tst -lpthread
	gcc test.c -o test -lpthread

clean:
	rm pthread_create create atom_tst test

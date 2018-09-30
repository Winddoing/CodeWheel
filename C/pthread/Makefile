
all:
	gcc pthread_create.c -o pthread_create -lpthread
	gcc pthread.c -o pthread -lpthread
	gcc atom_tst.c -o atom_tst -lpthread
	gcc test.c -o test -lpthread

clean:
	rm pthread_create pthread atom_tst test
	rm *.log

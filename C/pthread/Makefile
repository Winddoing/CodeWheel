
all:
	gcc pthread_create.c -o pthread_create -lpthread
	gcc pthread.c -o pthread -lpthread
	gcc atom_tst.c -o atom_tst -lpthread
	gcc test.c -o test -lpthread
	gcc pthread_key.c -o pthread_key -lpthread

clean:
	rm pthread_create pthread atom_tst test pthread_key
	rm *.log

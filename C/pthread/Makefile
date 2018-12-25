
all:
	gcc pthread_create.c -o pthread_create.out -lpthread
	gcc pthread.c -o pthread.out -lpthread
	gcc atom_tst.c -o atom_tst.out -lpthread
	gcc test.c -o test.out -lpthread
	gcc pthread_key.c -o pthread_key.out -lpthread

clean:
	rm *.out
	rm *.log

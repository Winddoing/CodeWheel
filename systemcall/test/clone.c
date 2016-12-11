
#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <malloc.h>

#include <signal.h>

#include <sys/types.h>
#include <unistd.h>


/*#define FIBER_STACK 8192*/
#define FIBER_STACK  4096 

int a;
void * stack;

int do_something()
{
	int b;

	b += 4;

	printf("b=%d\n", b);
	printf("This is son, the pid is:%d, the a is: %d(%p)\n", getpid(), ++a, &a);
	free(stack);
	_exit(0);
}

int main()
{
	void * stack;
	a = 1;
	stack = malloc(FIBER_STACK);

	if(!stack) {
		printf("The stack failed\n");
		_exit(0);
	}
	printf("creating son thread!!!\n");

/*	clone(&do_something, (char *)stack + FIBER_STACK, CLONE_VM|CLONE_VFORK, 0);*/
	clone(&do_something, (char *)stack + FIBER_STACK, CLONE_VFORK, 0);

	printf("This is father, my pid is: %d, the a is: %d(%p)\n", getpid(), a, &a);

	return 0;
}

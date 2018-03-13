/*
 * Copyright 2013 Myriad Group AG. All Rights Reserved.
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>

#if defined(__mips__)
#include <asm/cachectl.h>
#include <sys/cachectl.h>
#endif

#define BUFFER_SIZE 256
#define LOOPS 10L

static void (* volatile compiled)(void) = NULL;
static void (* volatile executed)(void) = NULL;

static volatile int done = 0;
static int singleThreaded = 0;
static int noflush = 0;
static int log = 0;
static int loops = LOOPS;
static int flush_type = 0;
#if defined (__i386__)
#define FUNCTION_SIZE 1
#elif defined (__mips__)
#define FUNCTION_SIZE 2
#endif

/**
 * Check of a new fragment is available. If so, execute it.
 */
void exec_one(void) {
	if (compiled != executed) {
		(*compiled)();
		if (log) {
			fputc('.', stdout);
			fflush(stdout);
		}
		executed = compiled;
	}
}

/**
 * Loop until someone sets 'done' to true. Poll for new fragments
 * and execute them in a busy loop.
 */
void *exec(void *ignored) {
	while(!done) {
		exec_one();
	}
	pthread_exit(NULL);
#ifdef ANDROID
        return 0;
#endif
}

/**
 * Flush the cache if needed on this architecture and not disabled.
 */
void flush(void *start, long size) {
	if (!noflush) {
		if(flush_type == 0){
			cacheflush(start, (int)size, BCACHE);
		}else {
			__builtin___clear_cache((char *)start, (char *)(start+size));
		}
	}
}

/**
 * Create a buffer to hold fragments. Then execute a number of loops,
 * each of which:
 * - fills the entire buffer with trap instructions
 * - flushes the cache if needed and not disabled
 * - inner loop through the buffer:
 *   - wait for the previous fragment to get executed (if applicable)
 *     this is done in a busy loop to keep one core occupied
 *   - 'compile' a tiny fragment, consisting of only a return, into
 *     the buffer.
 *   - flush the cache if needed and not disabled
 *   - 'publish' the fragment by advancing the 'compiled' function pointer
 */
void *comp(void *ignored) {
	int i;

	/* allocate an executable buffer holding BUFFER_SIZE mini-fragments */
	int size = BUFFER_SIZE * FUNCTION_SIZE * 4;
	int *start = mmap(NULL, size, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (log) printf("start is %p\n", start);
	if (start == MAP_FAILED) {
		perror("mmap failed");
		pthread_exit(NULL);
	}
	int *end = (int *)((int)start + size);
	/* fill the entire fragment with trap instructions */
	for (i = 0; i < loops; i++) {
		int *current;
		while (executed != compiled) {}
		for (current = start; current < end; current++) {
#if defined (__i386__)
			*current = 0xcccccccc; /* 4 * INT 3 */
#elif defined (__mips__)
			*current = 0x0000FFF4; /* TEQ zero, zero, #3FF */
#endif
		}
		flush(start, size);

		/* now compile fragments and wait for them to be executed */
		for (current = start; current < end; current += FUNCTION_SIZE) {
			while (executed != compiled) {}
#if defined(__i386__)
			*current = 0x000000c3; /* RET */
#elif defined(__mips__)
			*current =       0x03e00008; /* JR RA */
			*(current + 1) = 0x00000000; /* NOP in delay slot */
#endif
			flush(current, 4 * FUNCTION_SIZE);
		    /* publish the method pointer */
			compiled = (void (*)())current;
			/* if in single threaded mode, just execute the fragment here,
			   otherwise the executor thread will do this */
			if (singleThreaded) {
				exec_one();
			}
		}
	}
	done = 1;
	pthread_exit(NULL);
	return 0;
}

int main(int argc, char const *argv[])
{
	int i;
	pthread_t compiler, executor;

	for (i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-single") == 0) {
			singleThreaded = 1;
		} else if (strncmp(argv[i], "-loops=", strlen("-loops=")) == 0) {
			loops = atoi(argv[i] + strlen("-loops="));
		} else if (strcmp(argv[i], "-log") == 0) {
			log = 1;
		} else if (strcmp(argv[i], "-noflush") == 0) {
			noflush = 1;
		} else if (strcmp(argv[i], "-h") == 0) {
		 	printf("Usage: %s [-single] [-loops=N] [-log] [-noflush]\n", argv[0]);
		 	return 0;
		} else if (strcmp(argv[i], "-synci") == 0){
			flush_type = 1;
		}else {
			fprintf(stderr, "Unknown argument: %s\n", argv[i]);
			/* elegance and ugliness rolled into one */
			argv[i--] = "-h";
		}
	}

	if (!singleThreaded) {
		pthread_create(&executor, NULL, exec, NULL);
	}
	pthread_create(&compiler, NULL, comp, NULL);
	pthread_join(compiler, NULL);

	if (log) {
		printf("\n");
	}
	printf("done\n");
	pthread_exit(NULL);
    return 0;
}

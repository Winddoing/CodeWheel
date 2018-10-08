#ifndef _COUNT_H
#define _COUNT_H

#include <pthread.h>

int count;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int get();
void inc();

#endif

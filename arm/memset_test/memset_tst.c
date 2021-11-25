/*#############################################################
 *     File Name	: memset_tst.c
 *     Author		: winddoing
 *     Created Time	: 2021年10月18日 星期一 21时57分30秒
 *     Description	:
 *############################################################*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <sys/mman.h>

#define ROUND 5000

const size_t chunk_size = 1<<30;

struct timespec timer_start(){
	struct timespec start_time;
	clock_gettime(CLOCK_MONOTONIC, &start_time);

	return start_time;
}

// call this function to end a timer, returning nanoseconds elapsed as a long
long timer_end(struct timespec start_time){
	struct timespec end_time;
	clock_gettime(CLOCK_MONOTONIC, &end_time);
	long diffInNanos = (end_time.tv_sec - start_time.tv_sec) * (long)1e9 + (end_time.tv_nsec - start_time.tv_nsec);

	return diffInNanos;
}

void print_affinity() {
	cpu_set_t mask;
	long nproc, i;
	if (sched_getaffinity(0, sizeof(cpu_set_t), &mask) == -1) {
		perror("sched_getaffinity");
		assert(false);
	}
	nproc = sysconf(_SC_NPROCESSORS_ONLN);
	printf("sched_getaffinity = ");
	for (i = 0; i < nproc; i++)
		printf("%d ", CPU_ISSET(i, &mask));
}

void bind_to_cpu (){
	cpu_set_t mask;
	print_affinity();
	printf("\n");
	printf("sched_getcpu = %d\n", sched_getcpu());
	CPU_ZERO(&mask);
	CPU_SET(0, &mask);
	if (sched_setaffinity(0, sizeof(cpu_set_t), &mask) == -1) {
		perror("sched_setaffinity");
		assert(false);
	}
	print_affinity();
	printf("\nsched_getcpu = %d\n", sched_getcpu());


}



int main(){

	bind_to_cpu();
	void* memory_pool = mmap(NULL, chunk_size, PROT_READ | PROT_WRITE, MAP_POPULATE | MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
	if (memory_pool == MAP_FAILED){
		fprintf(stderr, "Error: Could not map memory.\n");
		return -1;
	}

	//initialize chunk
	memset(memory_pool, 0xff, chunk_size);
	size_t offset1 = (rand() << 12) % chunk_size;
	void* addr1 = (void*) (memory_pool+offset1);
	size_t offset2 = (rand() << 12) % chunk_size;
	void *addr2 = (void*) (memory_pool+offset2);
	//------------ CACHE ACCESS TIME USING LDR INSTRUCTION ---------------
	asm volatile("DSB 0XF");
	asm volatile("ISB");
	uint32_t value=0;
	struct timespec start = timer_start();
	for (int j = 0; j < ROUND; ++j) {
		asm volatile ("ldr %0, [%1]\n\t" : "=r" (value) : "r" (addr1) );
		asm volatile ("ldr %0, [%1]\n\t" : "=r" (value) : "r" (addr2) );
	}
	long time_taken = timer_end(start);
	printf("cache access time: %ld ns\n", time_taken/ROUND);

	//------------ MEMORY ACCESS TIME USING LDR + DC CIVAC INSTRUCTION --------------
	asm volatile("DSB 0XF");
	asm volatile("ISB");
	uint32_t temp=0;
	start = timer_start();
	for (int j = 0; j < ROUND; ++j) {
		asm volatile(
				"ldr %2, [%0]\n\t"
				"ldr %2, [%1]\n\t"
				"dc civac, %0\n\t"
				"dc civac, %1\n\t"
				::"r" (addr1), "r" (addr2), "r" (temp)
				);
	}
	time_taken = timer_end(start);
	printf("memory access time: %ld ns\n", time_taken/ROUND);

	// ------------ ZVA INSTRUCTION --------------------------------
	asm volatile("DSB 0XF");
	asm volatile("ISB");
	asm volatile(
			"dc civac, %0\n\t"
			"dc civac, %1\n\t"
			::"r" (addr1), "r" (addr2)
			);
	start = timer_start();
	for (int j = 0; j < ROUND; ++j) {
		// use dc zva to access target rows
		asm volatile(
				"dc zva, %0\n\t"
				"dc zva, %1\n\t"
				::"r" (addr1), "r" (addr2)
				);
	}
	time_taken = timer_end(start);
	printf("DC ZVA: %ld ns\n", time_taken/ROUND);

	return 0;
}

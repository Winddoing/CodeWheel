/* Copyright (C) 2024 wqshao All rights reserved.
 *
 *  File Name    : nvme_rw_test.c
 *  Author       : wqshao
 *  Created Time : 2024-05-15 11:03:08
 *  Description  :
 */

#define _GNU_SOURCE //O_DIRECT
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>

#define MMAP_FILE_SIZE (32 * 1024 * 1024)
#define TEST_WRITE_DATA_OFFSET (1 * 1024 * 1024)
#define TEST_READ_DATA_OFFSET (10 * 1024 * 1024)

#define TEST_MEM_SIZE (8 * 1024 * 1024)

#define LOOP_DELAY_MS 8

#define MMAP_TEST

#define TEST_R_THREAD
#define TEST_W_THREAD

static int debug = 1;
#define MLOG(fmt, ...) \
	do { \
	if (debug) \
		printf(fmt, ##__VA_ARGS__);\
	} while(0);

struct test_w_data {
	//uint32_t magic;
	uint8_t data[TEST_MEM_SIZE];
	uint32_t cnt;
	uint32_t status;
};

struct test_r_data {
	//uint32_t magic;
	uint8_t data[TEST_MEM_SIZE];
	uint32_t status;
};

static uint64_t get_time()
{
	struct timespec ts;

	if(clock_gettime(CLOCK_MONOTONIC,&ts) == 0){
		//return ts.tv_sec * 1000LL + (ts.tv_nsec/1000000LL);
		return ts.tv_sec * 1000000LL + (ts.tv_nsec/1000LL);
	}

	return 0;
}

static int nvme_fd = 0;

static void* test_nvme_write(void* data)
{
	struct test_w_data* wdata = (struct test_w_data*)(data + TEST_WRITE_DATA_OFFSET);
	printf("%s, data=%p, wdata=%p\n", __func__, data, wdata);
	printf("data size=%ld\n", sizeof(wdata->data));

	uint64_t stime, etime;
	uint64_t diff_time = 0;
	uint64_t total_time = 0;
	uint32_t ret = 0;
	void* rp = NULL;

	int fd = open("/dev/urandom", O_RDONLY);
	if (fd == -1) {
		printf("%s: open urandom file failed. errno=%d(%s)\n", __func__, errno, strerror(errno));
		return NULL;
	}

	void* src_data = NULL;;
	posix_memalign(&src_data, sysconf(_SC_PAGESIZE), TEST_MEM_SIZE);

	printf("%s: page size=%ld\n", __func__, sysconf(_SC_PAGESIZE));
	if(src_data == NULL || ((uintptr_t)src_data) % sysconf(_SC_PAGESIZE)) {
		printf("%s: malloc buffer failed. src_data=%p\n", __func__, src_data);
		close(fd);
		return NULL;
	}

	uint32_t loop_cnt = 100;
	if (!debug) loop_cnt = 100000;

	for (int i = 0; i < loop_cnt; i++) {
#if 0
		ret = read(fd, src_data, TEST_MEM_SIZE);
		//printf("---src_data[0]=%d, ret=%d, TEST_MEM_SIZE=%d\n", *((uint32_t*)src_data), ret, TEST_MEM_SIZE);
		if (ret != TEST_MEM_SIZE) {
			printf("%s: read urandom buffer error. ret=%d\n", __func__, ret);
			goto err;
		}
#endif

		if (sizeof(wdata->data) != TEST_MEM_SIZE) {
			printf("%s: buf size error. sz=%ld, TEST_MEM_SIZE=%d\n", __func__, sizeof(wdata->data), TEST_MEM_SIZE);
			goto err;
		}

		if (sizeof(wdata->data) % 512) {
			printf("%s: data size must be an integer multiple of 512. sz=%ld\n", __func__, sizeof(wdata->data));
		}

		if (((uintptr_t)wdata->data) % sysconf(_SC_PAGESIZE) || sizeof(wdata->data) % sysconf(_SC_PAGESIZE)) {
			printf("%s: data addr and len error.\n", __func__);
		}

		//ret = madvise(0, 0, MADV_DONTNEED);
		//ret = madvise(wdata->data, 0, MADV_DONTNEED);
		//printf("-----------------madvise ret=%d\n", ret);

		stime = get_time();
#ifdef MMAP_TEST

		//ret = madvise(wdata->data, sizeof(wdata->data), MADV_DONTNEED);
		ret = madvise(wdata->data, 0, MADV_DONTNEED);
		if (ret == -1) {
			printf("%s: data=%p, size=%ld\n", __func__, wdata->data, sizeof(wdata->data));
			perror("madvise");
			goto err;
		}
		rp = memcpy(wdata->data, src_data, sizeof(wdata->data));
		if (rp != wdata->data) {
			printf("%s: memcpy failed. rp=%p, dp=%p\n", __func__, rp, wdata->data);
			goto err;
		}
		msync(wdata->data, sizeof(wdata->data), MS_SYNC);
#else
		ret = pwrite(nvme_fd, src_data, sizeof(wdata->data), TEST_WRITE_DATA_OFFSET);
		if (ret < 0) {
		 	printf("%s: write failed. errno=%d-%s\n", __func__, errno, strerror(errno));
			printf("%s: nvme_fd=%d, src_data=%p, size=%ld. offset=%d\n", __func__, nvme_fd, src_data, sizeof(wdata->data), TEST_WRITE_DATA_OFFSET);
			goto err;
		}
		//fsync(nvme_fd);
		//printf("--------------w size=%ld, %d, ret=%d\n", sizeof(wdata->data), TEST_MEM_SIZE, ret);
#endif
		etime = get_time();

		diff_time = etime - stime;
		total_time += diff_time;
		MLOG("=%s: <%d> write diff_time=%ld\n", __func__, i, diff_time);

		usleep(LOOP_DELAY_MS * 1000);
	}

	total_time /= 1000; //ms
	printf("%s: write speed: %.2fGBps, avg delay=%.2fms\n", __func__, (float)((1.0 * TEST_MEM_SIZE * loop_cnt / 1024 / 1024 / 1024) / (1.0 * total_time / 1000)), (1.0 * total_time) / loop_cnt);

err:
	close(fd);
	free(src_data);

	printf("%s: over. loop_cnt=%d\n", __func__, loop_cnt);
	return NULL;
}

static void* test_nvme_read(void* data)
{
	struct test_r_data* rdata = (struct test_r_data*)(data + TEST_READ_DATA_OFFSET);
	printf("%s, data=%p, rdata=%p\n", __func__, data, rdata);
	uint64_t stime, etime;
	uint64_t diff_time = 0;
	uint64_t total_time = 0;
	uint32_t ret = 0;
	void* rp = NULL;

	void* dst_data = NULL;
	posix_memalign(&dst_data, sysconf(_SC_PAGESIZE), TEST_MEM_SIZE);

	printf("%s: page size=%ld\n", __func__, sysconf(_SC_PAGESIZE));
	if(dst_data == NULL || ((uintptr_t)dst_data) % sysconf(_SC_PAGESIZE)) {
		printf("%s: malloc buffer failed. dst_data=%p\n", __func__, dst_data);
		return NULL;
	}

	uint32_t loop_cnt = 100;
	if (!debug) loop_cnt = 100000;

	for (int i = 0; i < loop_cnt; i++) {
		memset(dst_data, 0, TEST_MEM_SIZE);

		if (sizeof(rdata->data) != TEST_MEM_SIZE) {
			printf("%s: buf size error. sz=%ld, TEST_MEM_SIZE=%d\n", __func__, sizeof(rdata->data), TEST_MEM_SIZE);
			goto err;
		}

		if (((uintptr_t)rdata->data) % sysconf(_SC_PAGESIZE) || sizeof(rdata->data) % sysconf(_SC_PAGESIZE)) {
			printf("%s: data addr and len error.\n", __func__);
		}

		stime = get_time();
#ifdef MMAP_TEST
		//msync(rdata->data, sizeof(rdata->data), MS_SYNC);
#if 1
		ret = madvise(rdata->data, 0, MADV_DONTNEED);
		//ret = madvise(rdata->data, 0, MADV_REMOVE);
		if (ret == -1) {
			printf("%s: data=%p, size=%ld\n", __func__, rdata->data, sizeof(rdata->data));
			perror("madvise");
			goto err;
		}
#endif
		msync(rdata->data, sizeof(rdata->data), MS_INVALIDATE);
		rp = memcpy(dst_data, rdata->data, sizeof(rdata->data));
		if (rp != dst_data) {
			printf("%s: memcpy failed. rp=%p, dp=%p\n", __func__, rp, dst_data);
			goto err;
		}
#else
		//fsync(nvme_fd);
		ret = pread(nvme_fd, dst_data, sizeof(rdata->data), TEST_READ_DATA_OFFSET);
		if (ret < 0) {
		 	printf("%s: read failed. errno=%d-%s\n", __func__, errno, strerror(errno));
			printf("%s: nvme_fd=%d, dst_data=%p, size=%ld, offset=%d\n", __func__, nvme_fd, dst_data, sizeof(rdata->data), TEST_READ_DATA_OFFSET);
			goto err;
		}
		//printf("--------------r size=%ld, %d, ret=%d\n", sizeof(rdata->data), TEST_MEM_SIZE, ret);
#endif
		etime = get_time();

		diff_time = etime - stime;
		total_time += diff_time;
		MLOG("-----------------------------------------------------%s: <%d> read diff_time=%ld\n", __func__, i, diff_time);

		usleep(LOOP_DELAY_MS * 1000);
	}

	total_time /= 1000; //ms
	printf("%s: read speed: %.2fGBps, avg delay=%.2fms\n", __func__, (float)((1.0 * TEST_MEM_SIZE * loop_cnt / 1024 / 1024 / 1024) / (1.0 * total_time / 1000)), (1.0 * total_time) / loop_cnt);

err:
	free(dst_data);

	printf("%s: over. loop_cnt=%d\n", __func__, loop_cnt);
	return NULL;
}

int main(int argc, char *argv[])
{
	//int nvme_fd = 0;
	uint8_t* ptr = NULL;
	char* file_name = NULL;
	uint32_t mmap_len = MMAP_FILE_SIZE;
	int ret = 0;
	pthread_t td1;
	pthread_t td2;

	if (argc != 2) {
		printf("please enter a file!\n");
		printf("%s file_name\n", argv[0]);
		return -1;
	}
	file_name = argv[1];

	//nvme_fd = open(file_name, O_RDWR);
	nvme_fd = open(file_name, O_RDWR | O_SYNC | O_DIRECT);
	//nvme_fd = open(file_name, O_RDWR | O_DIRECT);
	if (nvme_fd < 0) {
		printf("open file error, %d-%s\n", errno, strerror(errno));
		return -1;
	}
	printf("nvme fd=%d\n", nvme_fd);

#ifdef MMAP_TEST
	ptr = (uint8_t*)mmap(NULL, mmap_len, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_SYNC | MAP_LOCKED, nvme_fd, 0);  //nvme0n1
	//ptr = (uint8_t*)mmap(NULL, mmap_len, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_LOCKED, nvme_fd, 0);
	//ptr = (uint8_t*)mmap(NULL, mmap_len, PROT_READ | PROT_WRITE, MAP_SHARED, nvme_fd, 0);
	if (ptr == MAP_FAILED) {
		printf("mmap error: %d-%s, ptr=%p.\n", errno, strerror(errno), ptr);
		close(nvme_fd);
		return -1;
	}
	printf("mmap file vm addr=%p\n", ptr);
#endif

	printf("---------------- start -------------\n");
#ifdef TEST_W_THREAD
	ret = pthread_create(&td1, NULL, test_nvme_write, ptr);
	if (ret != 0) {
		printf("create w thread failed. ret=%d\n", ret);
		goto err;
	}
#endif

#ifdef TEST_R_THREAD
	ret = pthread_create(&td2, NULL, test_nvme_read, ptr);
	if (ret != 0) {
		printf("create r thread failed. ret=%d\n", ret);
		goto err;
	}
#endif

#ifdef TEST_W_THREAD
	pthread_join(td1, NULL);
#endif
#ifdef TEST_R_THREAD
	pthread_join(td2, NULL);
#endif

	printf("---------------- over -------------\n");

err:
#ifdef MMAP_TEST
	munmap(ptr, mmap_len);
#endif
	close(nvme_fd);

	return 0;
}

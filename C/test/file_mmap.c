/* Copyright (C) 2024 wqshao All rights reserved.
 *
 *  File Name    : file_mmap.c
 *  Author       : wqshao
 *  Created Time : 2024-05-08 17:08:03
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
#include <pwd.h>
#include <grp.h>


struct test_data {
	uint32_t val;
	uint64_t time;

	pthread_mutexattr_t attr;
	pthread_mutex_t mutex;
};

int main(int argc, char *argv[])
{
	int fd = 0;
	uint8_t* ptr = NULL;
	struct stat nvme_stat = {0};
	char* file_name = NULL;
	int rw_mode = 0;

	if (argc != 3) {
		printf("please enter a file and mode!\n");
		printf("%s file_name 0/1\n", argv[0]);
		return -1;
	}

	file_name = argv[1];
	rw_mode = atoi(argv[2]);
	printf("args: file name=%s, rw_mode=%d\n", file_name, rw_mode);

	uid_t uid;
	gid_t gid;
	struct passwd *pw;
	struct group *gr;

	uid = getuid();
	gid = getgid();
	pw = getpwuid(uid);
	gr = getgrgid(gid);
	printf("uid=%d(%s), gid=%d(%s)\n", uid, pw->pw_name, gid, gr->gr_name);

	gr = getgrnam("disk");
	initgroups(pw->pw_name, gid);


	//fd = open(file_name, O_RDWR);
	//fd = open(file_name, O_RDWR | O_SYNC | O_DIRECT);  // O_SYNC 严重影响读写性能
	fd = open(file_name, O_RDWR | O_DIRECT);
	if (fd < 0) {
		printf("open file error, %d-%s\n", errno, strerror(errno));
		return -1;
	}

	if (fstat(fd, &nvme_stat) < 0) {
		printf("get file state error:%d-%s\n", errno, strerror(errno));
		close(fd);
		return -1;
	}
	printf("file name=%s, fd=%d, size=%ld, st_mode=%x\n", file_name, fd, nvme_stat.st_size, nvme_stat.st_mode);

	printf("S_ISCHR=%d,S_ISBLK=%d\n", S_ISCHR(nvme_stat.st_mode), S_ISBLK(nvme_stat.st_mode));

	uint32_t mmap_len = 8192;
	int test_vm_olbaet = 1024;

	//ptr = (uint8_t*)mmap(NULL, mmap_len, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_SYNC | MAP_LOCKED, fd, 0);  //nvme0n1
	ptr = (uint8_t*)mmap(NULL, mmap_len, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_LOCKED, fd, 0);
	//ptr = (uint8_t*)mmap(NULL, mmap_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (ptr == MAP_FAILED) {
		printf("mmap error: %d-%s, ptr=%p.\n", errno, strerror(errno), ptr);
		close(fd);
		return -1;
	}
	printf("mmap file vm addr=%p\n", ptr);
	// 卡侧主进程启动后，遍历需要映射的文件后一一完成mmap映射，在映射完成后将其内存空间全部清0

	struct test_data *data = (struct test_data*)(ptr + test_vm_olbaet);
	printf("test data ptr=%p, val ptr=%p, mutex_ptr=%p\n", data, &data->val, &data->mutex);

	struct timeval tv;
	uint64_t time_ms = 0;
	uint8_t lba[512] = {0};

	printf("---lba size=%ld\n", sizeof(lba));

	if (rw_mode == 1) {
		memset(data, 0, sizeof(*data));
		//pthread_mutexattr_init(&data->attr);
		//pthread_mutexattr_setpshared(&data->attr, PTHREAD_PROCESS_SHARED);
		//pthread_mutex_init(&data->mutex, &data->attr);

		printf("--------------Write test -----------------------\n");
		for (int i = 0; i < 100; i++) {
			//pthread_mutex_lock(&data->mutex);
			gettimeofday(&tv, NULL);
			time_ms = tv.tv_sec * 1000 + (1.0 * tv.tv_usec)/1000;
			printf("mmap write test val=%d, time=%ld\n", i, time_ms);

			uint32_t* a = (uint32_t*)lba;
			*a = i;
			pwrite(fd, lba, sizeof(lba), 4096);
			fsync(fd);

			data->val = i;
			data->time = time_ms;
			msync(data, sizeof(*data), MS_SYNC);
			sleep(1);
			//pthread_mutex_unlock(&data->mutex);
		}
	} else {
		printf("**************Read test**************************\n");
		for (int i = 0; i < 100; i++) {
			//pthread_mutex_lock(&data->mutex);
			gettimeofday(&tv, NULL);
			time_ms = tv.tv_sec * 1000 + (1.0 * tv.tv_usec)/1000;
			msync(data, sizeof(*data), MS_SYNC);

			memset(lba, 0, sizeof(lba));
			pread(fd, &lba, sizeof(lba), 4096);
			uint32_t val = *((uint32_t*)lba);

			printf("mmap read test val=%d, get time=%ld, cur time=%ld, read val=%d\n", data->val, data->time, time_ms, val);
			sleep(1);
			//pthread_mutex_unlock(&data->mutex);
		}
	}

	munmap(ptr, mmap_len);
	close(fd);

	return 0;
}

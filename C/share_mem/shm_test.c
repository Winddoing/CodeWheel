/* Copyright (C) 2023 wqshao All rights reserved.
 *
 *  File Name    : shm_test.c
 *  Author       : wqshao
 *  Created Time : 2023-02-11 10:30:47
 *  Description  :
 *		gcc shm_test.c -lpthread
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <pthread.h>

#define MLOG(fmt, ...) printf("<%d> %s:" fmt , getpid(), __func__, ##__VA_ARGS__)

typedef void* shm_handle;

struct shm_obj {
	int shmid;

	void *mem_addr;
	size_t mem_size;

	int32_t *nattach_ptr;
	pthread_mutex_t *nattach_mutex_ptr;	//同步nattach_ptr操作
	pthread_mutex_t *mem_mutex_ptr;		//共享内存资源同步,导出外部使用
};

//初始化共享内存句柄
shm_handle init_shm(key_t key, size_t size)
{
	struct shm_obj *shm = NULL;

	shm = (struct shm_obj*)malloc(sizeof(struct shm_obj));
	if (shm == NULL) {
		MLOG("malloc shm_obj faild. [%d:%s]\n", errno, strerror(errno));
		return NULL;
	}

	shm->mem_size = size + sizeof(int32_t) + sizeof(pthread_mutex_t*) + sizeof(pthread_mutex_t*);

	/* key = ftok(".",1);     //获取键值 */
	shm->shmid = shmget(key, shm->mem_size, 0666 | IPC_CREAT); //打开或者创建共享内存
	if (-1 == shm->shmid) {
		MLOG("shmget failed. [%d:%s]\n", errno, strerror(errno));
		return NULL;
	}

	shm->mem_addr = shmat(shm->shmid, NULL, 0); //共享内存连接到当前进程的地址空间
	if (!shm->mem_addr) {
		MLOG("shmat failed. [%d:%s]\n", errno, strerror(errno));
		return NULL;
	}

	MLOG("key: 0x%08x, shmid: %d, shm size=%ld\n", key, shm->shmid, size);

	/* nattach 用于判断当前共享内存的使用对象 */
	uint8_t *data_ptr = (uint8_t*)shm->mem_addr;

	uint32_t nattach_ptr_pos = shm->mem_size;
	uint32_t nattach_mutex_ptr_pos = shm->mem_size + sizeof(int32_t);
	uint32_t mem_mutex_pos = shm->mem_size + sizeof(int32_t) + sizeof(pthread_mutex_t*);

	shm->nattach_ptr = (int32_t*)(data_ptr + nattach_ptr_pos);
	shm->nattach_mutex_ptr = (pthread_mutex_t*)(data_ptr + nattach_mutex_ptr_pos);
	shm->mem_mutex_ptr = (pthread_mutex_t*)(data_ptr + mem_mutex_pos);

	MLOG("--0--shm nattach: %d, mem_addr=%p, data_ptr=%p, nattach_mutex_ptr=%p\n", *shm->nattach_ptr, shm->mem_addr, data_ptr, shm->nattach_mutex_ptr);

	if (*shm->nattach_ptr < 0)
		*shm->nattach_ptr = 0;

	if (*shm->nattach_ptr == 0) {
		pthread_mutexattr_t attr;
		pthread_mutexattr_init(&attr);
		pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
		pthread_mutex_init(shm->nattach_mutex_ptr, &attr);
		pthread_mutex_init(shm->mem_mutex_ptr, &attr);

		memset(shm->mem_addr, 0, shm->mem_size);
		MLOG("\t*** shm mem area cleared to zero. addr=%p, size=%ld ***\n", shm->mem_addr, shm->mem_size);
	}

	pthread_mutex_lock(shm->nattach_mutex_ptr);
	(*shm->nattach_ptr)++;
	pthread_mutex_unlock(shm->nattach_mutex_ptr);
	MLOG("--1--shm nattach: %d\n", *shm->nattach_ptr);

	return (shm_handle)shm;
}

//获取共享内存指针
void *get_shm_addr(shm_handle handle)
{
	struct shm_obj *shm = (struct shm_obj*)handle;

	MLOG("get shm mem addr: %p\n", shm->mem_addr);

	assert(shm->mem_addr != NULL);

	return shm->mem_addr;
}

//释放共享内存句柄
int free_shm(shm_handle handle)
{
	struct shm_obj *shm = (struct shm_obj*)handle;
	uint64_t nattach = 0;
	int ret = 0;

	MLOG("--0--shm nattach: %d, mem_addr=%p\n", *shm->nattach_ptr, shm->mem_addr);
	if (shm->mem_addr) {
		pthread_mutex_lock(shm->nattach_mutex_ptr);
		if (shm->nattach_ptr) {
			if ((*shm->nattach_ptr) > 0)
				(*shm->nattach_ptr) --;
			nattach = (*shm->nattach_ptr);
		}
		pthread_mutex_unlock(shm->nattach_mutex_ptr);
		MLOG("shm nattach: %ld, shmid=%d\n", nattach, shm->shmid);

		shmdt(shm->mem_addr); //断开进程和内存的连接

		if (shm->shmid >= 0) {
			struct shmid_ds ds;
			if (shmctl(shm->shmid, IPC_STAT, &ds) == 0)
				nattach = ds.shm_nattch;
			MLOG("shm stat nattach = %ld\n", nattach);
			if (ds.shm_nattch == 0) {
				ret = shmctl(shm->shmid, IPC_RMID, &ds); //删除共享内存段
				if (ret == -1) {
					MLOG("shmctl IPC_RMID error, errno=%d(%s)\n", errno, strerror(errno));
				}
			}

			shm->shmid = -1;
		}
		shm->mem_addr = NULL;
		shm->mem_mutex_ptr = NULL;
		shm->nattach_mutex_ptr = NULL;

		MLOG("---exit  nattach = %ld, ret=%d\n", nattach, ret);
		free(shm);
		shm = NULL;
	}

	return ret;
}

struct test {
    int tst1;
    int tst2;
};

int main(int argc, const char *argv[])
{
	pid_t pid, child_pid;
	int i = 0;

	for (i = 0; i < 100; i++) {
		pid = fork();
		if (pid == 0 || pid == -1) {
			break;
		}
	}

	if (pid == -1) {
		MLOG("fork faild.\n");
	} else if (pid == 0) {
		MLOG("This is child process, id=%d, i=%d\n", getpid(), i);

		shm_handle shm = init_shm(1343, sizeof(struct test));

		void* shm_mem_addr = get_shm_addr(shm);

		srand((unsigned)getpid());
		uint32_t t = rand() % 10 + 1;
		printf("pid=%d, i=%d, sleep %ds\n", getpid(), i, t);

		sleep(rand() % 10 + 1);

		free_shm(shm);

		exit(0);
	} else {
		MLOG("This is main process, id=%d\n", getpid());

		do {
			// WNOHANG 非阻塞 轮询 等待带子进程结束
			child_pid = waitpid(pid, NULL, WNOHANG);

			MLOG("The pid (%d) progress has not exited!\n", child_pid);
			sleep(2);

		}while(child_pid == 0);

		exit(0);
	}

	return 0;
}

#include "threadpool.h"

static void *threadpool_func_loop(void *arg)
{
	struct threadpool *pool = (struct threadpool *)arg;
	struct job *pjob = NULL;
	while (1) {
		pthread_mutex_lock(&(pool->mutex));
		//队列为空时，就等待队列非空
		while ((pool->queue_cur_num == 0) && !pool->pool_close) {
			pthread_cond_wait(&(pool->queue_not_empty),
							  &(pool->mutex));
		}
		//线程池关闭，线程就退出
		if (pool->pool_close) {
			pthread_mutex_unlock(&(pool->mutex));
			pthread_exit(NULL);
		}

		pool->queue_cur_num--;
		pjob = pool->head;
		if (pool->queue_cur_num == 0) {
			pool->head = pool->tail = NULL;
		} else {
			pool->head = pjob->next;
		}
		if (pool->queue_cur_num == 0) {
			pthread_cond_signal(&(pool->queue_empty));  //队列为空，就可以通知threadpool_destroy函数，销毁线程函数
		}
		if (pool->queue_cur_num == pool->queue_max_num - 1) {
			pthread_cond_broadcast(&(pool->queue_not_full));    //队列非满，就可以通知threadpool_add_job函数，添加新任务
		}

		pthread_mutex_unlock(&(pool->mutex));

		(*(pjob->callback_function)) (pjob->arg);   //线程真正要做的工作，回调函数的调用

		free(pjob);

		pjob = NULL;
	}
}

struct threadpool *threadpool_init(int thread_num, int queue_max_num)
{
	struct threadpool *pool = NULL;
	int i, ret;

	pool = malloc(sizeof(struct threadpool));
	if (NULL == pool) {
		printf("failed to malloc threadpool!\n");
		return NULL;
	}

	pool->thread_num = thread_num;
	pool->queue_max_num = queue_max_num;
	pool->queue_cur_num = 0;
	pool->head = NULL;
	pool->tail = NULL;

	if (pthread_mutex_init(&(pool->mutex), NULL) != 0) {
		printf("failed to init mutex!\n");
		goto fail_poll;
	}
	if (pthread_cond_init(&(pool->queue_empty), NULL) != 0) {
		printf("failed to init queue_empty!\n");
		goto fail_poll;
	}
	if (pthread_cond_init(&(pool->queue_not_empty), NULL) != 0) {
		printf("failed to init queue_not_empty!\n");
		goto fail_poll;
	}
	if (pthread_cond_init(&(pool->queue_not_full), NULL) != 0) {
		printf("failed to init queue_not_full!\n");
		goto fail_poll;
	}

	pool->pthreads = malloc(sizeof(pthread_t) * thread_num);
	if (NULL == pool->pthreads) {
		printf("failed to malloc pthreads!\n");
		goto fail_thread;
	}
	memset(pool->pthreads, 0, sizeof(pthread_t) * thread_num);

	pool->queue_close = 0;
	pool->pool_close = 0;

	for (i = 0; i < pool->thread_num; ++i) {
		ret =
			pthread_create(&(pool->pthreads[i]), NULL,
						   threadpool_func_loop, (void *)pool);
		if (ret) {
			printf("%s, errno=%d, threadpool_func_loop<%d>\n",
				   __func__, errno, i);
			pool->thread_num = i;
			break;
		}
	}

	return pool;

fail_thread:
	free(pool->pthreads);
fail_poll:
	free(pool);
	return NULL;

}

int threadpool_add_job(struct threadpool *pool,
					   void *(*callback_function) (void *arg), void *arg)
{
	struct job *pjob = NULL;

	assert(pool != NULL);
	assert(callback_function != NULL);
	assert(arg != NULL);

	pthread_mutex_lock(&(pool->mutex));
	while ((pool->queue_cur_num == pool->queue_max_num)
		   && !(pool->queue_close || pool->pool_close)) {
		pthread_cond_wait(&(pool->queue_not_full), &(pool->mutex)); //队列满的时候就等待
	}
	//队列关闭或者线程池关闭就退出
	if (pool->queue_close || pool->pool_close) {
		pthread_mutex_unlock(&(pool->mutex));
		printf("%s, %s close\n", pool->queue_close ? "queue" : "pool");
		return -1;
	}

	pjob = (struct job *)malloc(sizeof(struct job));
	if (NULL == pjob) {
		pthread_mutex_unlock(&(pool->mutex));
		printf("%s malloc\n", __func__);
		return -1;
	}

	memset(pjob, 0, sizeof(struct job));

	pjob->callback_function = callback_function;
	pjob->arg = arg;
	pjob->next = NULL;

	if (pool->head == NULL) {
		pool->head = pool->tail = pjob;
		pthread_cond_broadcast(&(pool->queue_not_empty));   //队列空的时候，有任务来时就通知线程池中的线程：队列非空
	} else {
		pool->tail->next = pjob;
		pool->tail = pjob;
	}
	pool->queue_cur_num++;

	pthread_mutex_unlock(&(pool->mutex));

	return 0;
}

int threadpool_destroy(struct threadpool *pool)
{
	int i;
	struct job *p;

	assert(pool != NULL);
	//线程池已经退出了，就直接返回
	pthread_mutex_lock(&(pool->mutex));
	if (pool->queue_close || pool->pool_close) {
		pthread_mutex_unlock(&(pool->mutex));
		return -1;
	}

	pool->queue_close = 1;  //置队列关闭标志
	while (pool->queue_cur_num != 0) {
		pthread_cond_wait(&(pool->queue_empty), &(pool->mutex));    //等待队列为空
	}

	pool->pool_close = 1;   //置线程池关闭标志
	pthread_mutex_unlock(&(pool->mutex));
	pthread_cond_broadcast(&(pool->queue_not_empty));   //唤醒线程池中正在阻塞的线程
	pthread_cond_broadcast(&(pool->queue_not_full));    //唤醒添加任务的threadpool_add_job函数
	for (i = 0; i < pool->thread_num; ++i) {
		pthread_join(pool->pthreads[i], NULL);  //等待线程池的所有线程执行完毕
	}

	pthread_mutex_destroy(&(pool->mutex));  //清理资源
	pthread_cond_destroy(&(pool->queue_empty));
	pthread_cond_destroy(&(pool->queue_not_empty));
	pthread_cond_destroy(&(pool->queue_not_full));
	free(pool->pthreads);

	while (pool->head != NULL) {
		p = pool->head;
		pool->head = p->next;
		free(p);
	}

	free(pool);

	return 0;
}

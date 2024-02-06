/* Copyright (C) 2024 wqshao All rights reserved.
 *
 *  File Name    : queue.c
 *  Author       : wqshao
 *  Created Time : 2024-01-05 11:09:28
 *  Description  :
 *      https://blog.csdn.net/weixin_50941083/article/details/121823076
 *      https://blog.csdn.net/qq_52208569/article/details/113848831
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

typedef struct node {
	void *data;
	struct node *next;
} node;

typedef struct queue {
	node *head;
	node *tail;
} queue;

void queue_create(queue *pq);
void queue_destory(queue *pq);

int queue_push(queue *pq, void *data);
void* queue_pop(queue *pq);

void* queue_front(queue *pq);
void* queue_back(queue *pq);

int queue_size(queue *pq);
int queue_empty(queue *pq);

void queue_create(queue *pq)
{
	assert(pq);
	pq->head = pq->tail = NULL;
}

void queue_destory(queue *pq)
{
	assert(pq);
	node *cur = pq->head;

	while (cur) {
		node *next = cur->next;
		free(cur);
		cur = next;
	}
	pq->head = pq->tail = NULL;
}

// 入队
int queue_push(queue *pq, void *data)
{
	assert(pq);
	node *n = malloc(sizeof(node));
	if (!n)
		return -1;

	printf("push data=%p\n", data);
	n->data = data;
	n->next = NULL;

	// 添加队列尾部
	if (pq->head == NULL) {
		pq->head = pq->tail = n;
	} else {
		pq->tail->next = n;
		pq->tail = n;
	}

	return 0;
}

// 出队
void* queue_pop(queue *pq)
{
	assert(pq);
	assert(pq->head);
	node *cur = pq->head;
	if (!cur) {
		printf("node is null.\n");
		return NULL;
	}
	node *next = cur->next;

	void *data = cur->data;
	free(cur);

	pq->head = next;
	if (pq->head == NULL) {
		pq->tail = pq->head;
	}
	printf("pop data=%p\n", data);

	return data;
}

// 访问队首
void* queue_front(queue *pq)
{
	assert(pq);
	assert(pq->head);

	return pq->head->data;
}

// 访问队尾
void* queue_back(queue *pq)
{
	assert(pq);
	assert(pq->tail);

	return pq->tail->data;
}

int queue_size(queue *pq)
{
	assert(pq);
	int size = 0;
	node *cur = pq->head;

	while (cur) {
		size++;
		cur = cur->next;
	}

	return size;
}

int queue_empty(queue *pq)
{
	assert(pq);

	return pq->head == NULL ? 1 : 0;
}

int main(int argc, const char *argv[])
{
	queue q;

	queue_create(&q);
#if 1
	int tst[10];
	for (int i = 0; i < 10; i++) {
		tst[i] = i;
		printf("tst data[%d] = %d\n", i, tst[i]);
		queue_push(&q, &tst[i]);
	}
	while (!queue_empty(&q)) {
		printf("- %d\n", *(int*)queue_pop(&q));
	}
#else
	int a = 1;
	queue_push(&q, &a);
	printf("a=%d, pop a=%d\n", a, *(int*)queue_pop(&q));
	a = 3;
	queue_push(&q, &a);
	printf("a=%d, pop a=%d\n", a, *(int*)queue_pop(&q));
	a = 9;
	queue_push(&q, &a);
	printf("a=%d, pop a=%d\n", a, *(int*)queue_pop(&q));
#endif
	queue_destory(&q);

	return 0;
}


/* Copyright (C) 2026 wqshao All rights reserved.
 *
 *  File Name    : hash_table_test.c
 *  Author       : wqshao
 *  Created Time : 2026-03-10 19:58:49
 *  Description  :
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <string.h>

// 哈希表节点
typedef struct Entry {
	int key;
	int value;
	struct Entry *next;
} Entry;

// 哈希表结构
typedef struct HashTable {
	Entry **buckets;    // 桶数组（每个元素指向链表头）
	int size;           // 桶的数量
	int count;          // 当前存储的键值对数量
	float load_factor;  // 扩容阈值
} HashTable;

// 哈希函数：处理负数，取绝对值再取模（注意INT_MIN的特殊情况）
static int hash(int key, int size)
{
	// 使用 (key % size + size) % size 来处理负数
	// 该方法对INT_MIN也安全（因为先取模再调整）
	int h = key % size;
	if (h < 0) h += size;
	return h;
}

// 创建哈希表（size：初始桶数量，load_factor：扩容阈值，通常0.75）
HashTable *createHashTable(int size, float load_factor)
{
	if (size <= 0 || load_factor <= 0.0f || load_factor >= 1.0f) {
		fprintf(stderr, "Invalid parameters for createHashTable\n");
		return NULL;
	}
	HashTable *ht = (HashTable *)malloc(sizeof(HashTable));
	if (!ht) return NULL;
	ht->size = size;
	ht->count = 0;
	ht->load_factor = load_factor;
	ht->buckets = (Entry **)calloc(size, sizeof(Entry *));
	if (!ht->buckets) {
		free(ht);
		return NULL;
	}
	return ht;
}

// 查找键对应的节点（返回节点指针，未找到返回NULL）
static Entry *findEntry(HashTable *ht, int key)
{
	if (!ht) return NULL;
	int idx = hash(key, ht->size);
	Entry *cur = ht->buckets[idx];
	while (cur) {
		if (cur->key == key)
			return cur;
		cur = cur->next;
	}
	return NULL;
}

// 查找键的值（通过value指针返回，成功返回1，失败返回0）
int search(HashTable *ht, int key, int *value)
{
	if (!ht || !value) return 0;
	Entry *entry = findEntry(ht, key);
	if (entry) {
		*value = entry->value;
		return 1;
	}
	return 0;
}

// 重新哈希（扩容或缩容时调用）
static int rehash(HashTable *ht, int new_size)
{
	if (!ht || new_size <= 0) return 0;
	// 分配新桶数组
	Entry **new_buckets = (Entry **)calloc(new_size, sizeof(Entry *));
	if (!new_buckets) return 0;

	// 遍历旧桶，将所有节点重新插入新桶
	for (int i = 0; i < ht->size; i++) {
		Entry *cur = ht->buckets[i];
		while (cur) {
			Entry *next = cur->next;   // 先保存下一个节点
			int new_idx = hash(cur->key, new_size);
			// 头插法插入新桶
			cur->next = new_buckets[new_idx];
			new_buckets[new_idx] = cur;
			cur = next;
		}
	}

	// 释放旧桶数组（节点已被迁移，不能释放节点）
	free(ht->buckets);
	ht->buckets = new_buckets;
	ht->size = new_size;
	return 1;
}

// 插入/更新键值对（成功返回1，失败返回0）
int insert(HashTable *ht, int key, int value)
{
	if (!ht) return 0;

	// 先查找是否已存在
	Entry *existing = findEntry(ht, key);
	if (existing) {
		// 更新值
		existing->value = value;
		return 1;
	}

	// 检查是否需要扩容（装载因子 = count / size）
	if ((float)ht->count / ht->size >= ht->load_factor) {
		int new_size = ht->size * 2;   // 扩大为2倍
		if (!rehash(ht, new_size)) {
			// 扩容失败，仍然尝试插入原表（但可能性能下降）
			fprintf(stderr, "Warning: rehash failed, insert into old table\n");
		}
	}

	int idx = hash(key, ht->size);
	Entry *newEntry = (Entry *)malloc(sizeof(Entry));
	if (!newEntry) return 0;
	newEntry->key = key;
	newEntry->value = value;
	newEntry->next = ht->buckets[idx];
	ht->buckets[idx] = newEntry;
	ht->count++;
	return 1;
}

// 删除键（成功返回1，失败返回0）
int delete (HashTable *ht, int key)
{
	if (!ht) return 0;
	int idx = hash(key, ht->size);
	Entry *cur = ht->buckets[idx];
	Entry *prev = NULL;

	while (cur) {
		if (cur->key == key) {
			// 从链表中移除
			if (prev)
				prev->next = cur->next;
			else
				ht->buckets[idx] = cur->next;
			free(cur);
			ht->count--;
			return 1;
		}
		prev = cur;
		cur = cur->next;
	}
	return 0;   // 未找到
}

// 打印哈希表（用于调试）
void printHashTable(HashTable *ht)
{
	if (!ht) {
		printf("HashTable is NULL\n");
		return;
	}
	printf("HashTable size=%d, count=%d, load=%.2f\n", ht->size, ht->count, (float)ht->count / ht->size);
	for (int i = 0; i < ht->size; i++) {
		printf("bucket[%d]: ", i);
		Entry *cur = ht->buckets[i];
		while (cur) {
			printf("(%d:%d) ", cur->key, cur->value);
			cur = cur->next;
		}
		printf("\n");
	}
}

// 销毁哈希表，释放所有内存
void destroyHashTable(HashTable *ht)
{
	if (!ht) return;
	for (int i = 0; i < ht->size; i++) {
		Entry *cur = ht->buckets[i];
		while (cur) {
			Entry *tmp = cur;
			cur = cur->next;
			free(tmp);
		}
	}
	free(ht->buckets);
	free(ht);
}

// 测试示例
int main()
{
	// 创建哈希表（初始大小5，负载因子0.75）
	HashTable *ht = createHashTable(5, 0.75f);
	if (!ht) {
		fprintf(stderr, "Failed to create hash table\n");
		return 1;
	}

	// 插入一些数据
	insert(ht, 10, 100);
	insert(ht, 22, 200);
	insert(ht, 31, 300);
	insert(ht, 4, 400);
	insert(ht, 15, 500);   // 15%5=0，与10冲突，会链入同一个桶
	insert(ht, 28, 600);
	insert(ht, 17, 700);   // 此时可能触发扩容

	printf("After inserts:\n");
	printHashTable(ht);

	// 查找测试
	int val;
	if (search(ht, 22, &val))
		printf("Found key 22 -> %d\n", val);
	else
		printf("Key 22 not found\n");

	if (search(ht, 99, &val))
		printf("Found key 99 -> %d\n", val);
	else
		printf("Key 99 not found\n");

	// 更新测试
	insert(ht, 10, 1000);
	printf("After update key 10 to 1000:\n");
	printHashTable(ht);

	// 删除测试
	delete (ht, 15);
	delete (ht, 999);  // 不存在的键
	printf("After deleting key 15:\n");
	printHashTable(ht);

	// 释放内存
	destroyHashTable(ht);
	return 0;
}

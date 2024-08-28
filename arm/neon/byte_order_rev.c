/* Copyright (C) 2024 wqshao All rights reserved.
 *
 *  File Name    : byte_order_rev.c
 *  Author       : wqshao
 *  Created Time : 2024-08-27 09:20:11
 *  Description  :
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>
#include <arpa/inet.h>  //htonl

#include <arm_neon.h>

static void reverse_8byte_elements_8bit_neon(uint8x16_t *src, uint8x16_t *dst)
{
	// 假设 src 指向包含16个字节的源数据
	// 我们想要反转这16个字节中的8个16位元素的字节序

	// 加载源数据到NEON寄存器
	uint8x16_t data = vld1q_u8((void *)src);

	// 使用 REV64 指令反转64位双字中的元素字节序
	// 由于我们的元素是8位的，REV64 将反转每两个8位元素（即16位）
	uint8x16_t reversed_data = vrev64q_u8(data);

	// 存储结果到目标地址
	vst1q_u8((void *)dst, reversed_data);
}

static int reverse_8byte_unit_8bit_test()
{
	uint8_t source[16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0xd, 0x0e, 0x0f};
	uint8_t destination[16];

	printf("===> func: %s, line: %d\n", __func__, __LINE__);
	for (int i = 0; i < 16; ++i) {
		printf("0x%02X ", source[i]);
	}
	printf("\n");

	// 调用函数，反转元素的字节序
	reverse_8byte_elements_8bit_neon((uint8x16_t *)source, (uint8x16_t *)destination);

	// 输出结果
	for (int i = 0; i < 16; ++i) {
		printf("0x%02X ", destination[i]);
	}
	printf("\n");
	printf("===> func: %s, line: %d\n", __func__, __LINE__);

	return 0;
}

static void reverse_8byte_elements_16bit_neon(uint16_t *src, uint16_t *dst, size_t count)
{
	// 确保 count 是8的倍数，因为每个REV64操作影响64位（8个16位）
	for (size_t i = 0; i < count; i += 8) {
		// 加载64个字节（32个16位元素）到NEON寄存器
		uint16x8_t data = vld1q_u16((const uint16_t *)src + i);

		// 使用 REV64 指令反转64位双字中的元素字节序
		// 这将反转8个16位元素的字节序
		uint16x8_t reversed_data = vrev64q_u16(data);

		// 存储结果回内存
		vst1q_u16((uint16_t *)dst + i, reversed_data);
	}
}

// 8byte数据以16bit为单位进行反转
static int reverse_8byte_unit_16bit_test()
{
	uint16_t source[16] = {0x0123, 0x4567, 0x89AB, 0xCDEF, 0x1234, 0x5678, 0x9ABC, 0xDEF0,
		0x0123, 0x4567, 0x89AB, 0xCDEF, 0x1234, 0x5678, 0x9ABC, 0xDEF0};
	uint16_t destination[16];

	printf("===> func: %s, line: %d\n", __func__, __LINE__);
	for (int i = 0; i < 16; ++i) {
		printf("0x%04X ", source[i]);
	}
	printf("\n");

	// 调用函数，反转2字节元素的字节序
	reverse_8byte_elements_16bit_neon(source, destination, 16);

	// 输出结果
	for (int i = 0; i < 16; ++i) {
		printf("0x%04X ", destination[i]);
	}
	printf("\n");
	printf("===> func: %s, line: %d\n", __func__, __LINE__);

	return 0;
}

static void reverse_4byte_elements_8bit_neon(uint8_t *src, uint8_t *dst, size_t count)
{
	size_t i = 0;
	asm("nop");
	asm("nop");
	for (i = 0; i < count; i += 16) {
		// 从源数组加载128位数据到NEON寄存器
		uint8x16_t data = vld1q_u8(src + i);

		// 使用 vrev32q_u8 指令反转32位数据的每个字节
		uint8x16_t reversed_data = vrev32q_u8(data);

		// 将结果存储回目标数组
		vst1q_u8(dst + i, reversed_data);
	}
	asm("nop");
	asm("nop");
}

static void reverse_4byte_elements_8bit_neon_optimized(uint8_t *src, uint8_t *dst, size_t count)
{
	size_t i = 0;
#if 0
	if (count % 32 != 0 ) {
		reverse_4byte_elements_8bit_neon(src, dst, count);
		count -= count % 32;
	}

	for (i = 0; i < count; i += 32) {
		// 从源数组加载128位数据到NEON寄存器
		uint8x16_t data0 = vld1q_u8(src + i + 0);
		uint8x16_t data1 = vld1q_u8(src + i + 16);

		// 使用 vrev32q_u8 指令反转32位数据的每个字节
		uint8x16_t reversed_data0 = vrev32q_u8(data0);
		uint8x16_t reversed_data1 = vrev32q_u8(data1);

		// 将结果存储回目标数组
		vst1q_u8(dst + i + 0,  reversed_data0);
		vst1q_u8(dst + i + 16, reversed_data1);
	}
#else
	if (count % 64 != 0) {
		reverse_4byte_elements_8bit_neon(src, dst, count);
		count -= count % 64;
	}

	for (i = 0; i < count; i += 64) {
		// 从源数组加载128位数据到NEON寄存器
#if 1
		uint8x16_t data0 = vld1q_u8(src + i + 0);
		uint8x16_t data1 = vld1q_u8(src + i + 16);
		uint8x16_t data2 = vld1q_u8(src + i + 32);
		uint8x16_t data3 = vld1q_u8(src + i + 48);

		// 使用 vrev32q_u8 指令反转32位数据的每个字节
		uint8x16_t reversed_data0 = vrev32q_u8(data0);
		uint8x16_t reversed_data1 = vrev32q_u8(data1);
		uint8x16_t reversed_data2 = vrev32q_u8(data2);
		uint8x16_t reversed_data3 = vrev32q_u8(data3);

		// 将结果存储回目标数组
		vst1q_u8(dst + i + 0,  reversed_data0);
		vst1q_u8(dst + i + 16, reversed_data1);
		vst1q_u8(dst + i + 32, reversed_data2);
		vst1q_u8(dst + i + 48, reversed_data3);
#else
		// 转换错误
		uint8x16x4_t data = vld4q_u8(src + i);

		uint8x16_t reversed_data0 = vrev16q_u8(data.val[0]);
		uint8x16_t reversed_data1 = vrev16q_u8(data.val[1]);
		uint8x16_t reversed_data2 = vrev16q_u8(data.val[2]);
		uint8x16_t reversed_data3 = vrev16q_u8(data.val[3]);

		//vst4q_u8(dst + i, (uint8x16x4_t){reversed_data0, reversed_data1, reversed_data2, reversed_data3});
		vst4q_u8(dst + i, (uint8x16x4_t){reversed_data3, reversed_data2, reversed_data1, reversed_data0});
		//vst4q_u8(dst + i, (uint8x16x4_t){reversed_data1, reversed_data0, reversed_data3, reversed_data2});
		//vst4q_u8(dst + i, (uint8x16x4_t){data.val[0], data.val[1], data.val[2], data.val[3]});
#endif
	}
#endif
}

static void reverse_4byte_elements_8bit_c(uint8_t *src, uint8_t *dst, size_t count)
{
	uint32_t *tmp_src = (uint32_t*)src;
	uint32_t *tmp_dst = (uint32_t*)dst;

	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	for (size_t i = 0; i < count / 4; i++) {
		//tmp_dst[i] = htonl(tmp_src[i]);
		tmp_dst[i] = ntohl(tmp_src[i]);
	}
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
}

static int reverse_4byte_unit_8bit_test()
{
	uint8_t source[16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0xd, 0x0e, 0x0f};
	uint8_t destination[16];

	printf("===> func: %s, line: %d\n", __func__, __LINE__);
	for (int i = 0; i < 16; ++i) {
		printf("0x%02X ", source[i]);
	}
	printf("\n");

	// 调用函数，反转每4个字节的顺序
	reverse_4byte_elements_8bit_neon_optimized(source, destination, 16);

	// 输出结果
	for (int i = 0; i < 16; ++i) {
		printf("0x%02X ", destination[i]);
	}
	printf("\n");
	printf("===> func: %s, line: %d\n", __func__, __LINE__);

	return 0;
}

static void reverse_2byte_elements_8bit_neon(uint8_t *src, uint8_t *dst, size_t count)
{
	for (size_t i = 0; i < count; i += 16) {
		// 从源数组加载128位数据到NEON寄存器
		uint8x16_t data = vld1q_u8(src + i);

		// 使用 vrev16q_u8 指令反转16位数据的每个字节
		uint8x16_t reversed_data = vrev16q_u8(data);

		// 将结果存储回目标数组
		vst1q_u8(dst + i, reversed_data);
	}
}

static void reverse_2byte_elements_8bit_c(uint8_t *src, uint8_t *dst, size_t count)
{
	uint16_t *tmp_src = (uint16_t*)src;
	uint16_t *tmp_dst = (uint16_t*)dst;

	for (size_t i = 0; i < count / 2; i++) {
		tmp_dst[i] = htons(tmp_src[i]);
	}
}

static int reverse_2byte_unit_8bit_test()
{
	uint8_t source[16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0xd, 0x0e, 0x0f};
	uint8_t destination[16];

	printf("===> func: %s, line: %d\n", __func__, __LINE__);
	for (int i = 0; i < 16; ++i) {
		printf("0x%02X ", source[i]);
	}
	printf("\n");
	// 调用函数，反转每两个字节的顺序
#if 1
	reverse_2byte_elements_8bit_neon(source, destination, 16);
#else
	reverse_2byte_elements_8bit_c(source, destination, 16);
#endif

	// 输出结果
	for (int i = 0; i < 16; ++i) {
		printf("0x%02X ", destination[i]);
	}
	printf("\n");
	printf("===> func: %s, line: %d\n", __func__, __LINE__);

	return 0;
}

static uint64_t get_time()
{
#if 0
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec * 1000000 + tv.tv_usec;
#else
	struct timespec timestamp;
	clock_gettime(CLOCK_REALTIME, &timestamp);
	return timestamp.tv_sec * 1000000000 + timestamp.tv_nsec;
#endif
}

static void save_file(uint8_t *data, size_t size, const char *file_name)
{
	printf("%s: file: %s\n", __func__, file_name);

	FILE *fp = fopen(file_name, "w+");

	fwrite(data, sizeof(uint8_t), size, fp);

	fclose(fp);
}

static int reverse_4byte_4k_data_test()
{
	const size_t data_size = 4096; // 4KB 数据
#if 0
	uint8_t *source = malloc(data_size * sizeof(uint8_t));
	uint8_t *destination = malloc(data_size * sizeof(uint8_t));
#else
	int ret = 0;
	uint8_t *source = NULL;
	uint8_t *destination = NULL;

	ret = posix_memalign((void **)&source, 64, data_size * sizeof(uint8_t));
	ret = posix_memalign((void **)&destination, 64, data_size * sizeof(uint8_t));

#endif
	uint64_t s = 0, e = 0;

	// 初始化源数据，这里只是示例，填充特定模式的数据
	for (size_t i = 0; i < data_size; ++i) {
		source[i] = i % 256; // 填充0到255的值
	}

	for (size_t i = 0; i < 16; ++i) {
		printf("0x%08X ", *((uint32_t*)source + i));
	}
	printf("\n");
	save_file(source, data_size, "/tmp/4aa.bin");

	memset(destination, 0, data_size);
	// 调用函数，反转每两个字节的顺序
	s = get_time();
	reverse_4byte_elements_8bit_neon_optimized(source, destination, data_size);
	//reverse_4byte_elements_8bit_neon(source, destination, data_size);
	e = get_time();
	printf("%s, neon diff time=%ld ns\n", __func__, e - s);
	// 验证结果（这里只是打印前几个结果）
	for (size_t i = 0; i < 16; ++i) {
		printf("0x%08X ", *((uint32_t*)destination + i));
	}
	printf("\n");
	save_file(destination, data_size, "/tmp/4bb.bin");

	memset(destination, 0, data_size);
	s = get_time();
	reverse_4byte_elements_8bit_c(source, destination, data_size);
	e = get_time();
	printf("%s, C diff time=%ld ns\n", __func__, e - s);
	// 验证结果（这里只是打印前几个结果）
	for (size_t i = 0; i < 16; ++i) {
		printf("0x%08X ", *((uint32_t*)destination + i));
	}
	printf("\n");
	save_file(destination, data_size, "/tmp/4cc.bin");

	// 释放分配的内存
	free(source);
	free(destination);
}

static int reverse_2byte_4k_data_test()
{
	const size_t data_size = 4096; // 4KB 数据
#if 0
	uint8_t *source = malloc(data_size * sizeof(uint8_t));
	uint8_t *destination = malloc(data_size * sizeof(uint8_t));
#else
	int ret = 0;
	uint8_t *source = NULL;
	uint8_t *destination = NULL;

	ret = posix_memalign((void **)&source, 64, data_size * sizeof(uint8_t));
	ret = posix_memalign((void **)&destination, 64, data_size * sizeof(uint8_t));

#endif
	uint64_t s = 0, e = 0;

	// 初始化源数据，这里只是示例，填充特定模式的数据
	for (size_t i = 0; i < data_size; ++i) {
		source[i] = i % 256; // 填充0到255的值
	}

	for (size_t i = 0; i < 16; ++i) {
		printf("0x%08X ", *((uint32_t*)source + i));
	}
	printf("\n");
	save_file(source, data_size, "/tmp/2aa.bin");

	memset(destination, 0, data_size);

	// 调用函数，反转每两个字节的顺序
	s = get_time();
	reverse_2byte_elements_8bit_neon(source, destination, data_size);
	e = get_time();
	printf("%s, neon diff time=%ld ns\n", __func__, e - s);
	// 验证结果（这里只是打印前几个结果）
	for (size_t i = 0; i < 16; ++i) {
		printf("0x%08X ", *((uint32_t*)destination + i));
	}
	printf("\n");
	save_file(destination, data_size, "/tmp/2bb.bin");

	memset(destination, 0, data_size);
	s = get_time();
	reverse_2byte_elements_8bit_c(source, destination, data_size);
	e = get_time();
	printf("%s, C diff time=%ld ns\n", __func__, e - s);
	// 验证结果（这里只是打印前几个结果）
	for (size_t i = 0; i < 16; ++i) {
		printf("0x%08X ", *((uint32_t*)destination + i));
	}
	printf("\n");
	save_file(destination, data_size, "/tmp/2cc.bin");

	// 释放分配的内存
	free(source);
	free(destination);
}

static int net_byte_order_rev_test()
{
	uint32_t hl = 0x12345678;

	printf("%s: htonl(0x%08x) = 0x%08x\n", __func__, hl, htonl(hl));
	printf("%s: ntohl(0x%08x) = 0x%08x\n", __func__, hl, ntohl(hl));
}

int main(int argc, const char *argv[])
{
	int ret = 0;

	ret = net_byte_order_rev_test();
	printf("\n");

	ret = reverse_8byte_unit_8bit_test();
	printf("\n");

	ret = reverse_8byte_unit_16bit_test();
	printf("\n");

	ret = reverse_4byte_unit_8bit_test();
	printf("\n");

	ret = reverse_2byte_unit_8bit_test();
	printf("\n");

	ret = reverse_4byte_4k_data_test();
	printf("\n");

	ret = reverse_2byte_4k_data_test();
	printf("\n");

	printf("===> func: %s, line: %d, ret=%d\n", __func__, __LINE__, ret);
	return ret;
}

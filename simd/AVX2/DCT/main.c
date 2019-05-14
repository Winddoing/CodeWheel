/*#############################################################
 *     File Name    : main.c
 *     Author       : winddoing
 *     Created Time : 2019年04月30日 星期二 14时32分54秒
 *     Description  :
 *############################################################*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/time.h>

#include "jpeg_DCT.h"

extern void _jsimd_fdct_islow_avx2(short *data);
extern void _jsimd_fdct_ifast_sse2(short *data);
extern void jpeg_fdct_islow(short *data);
extern void jpeg_fdct_ifast(short *data);


static void dump_uint8(uint8_t *data)
{
	int i = 0;

	for(i = 0; i < 64; i++) {
		printf("%5d ", data[i]);

		if(((i + 1) % 8) == 0)
			printf("\n");
	}

	printf("\n");
}

static void dump_int16(int16_t *data)
{
	int i = 0;

	for(i = 0; i < 64; i++) {
		printf("%5d ", data[i]);

		if(((i + 1) % 8) == 0)
			printf("\n");
	}

	printf("\n");
}

static uint64_t get_time(void)
{
#if 1
	uint64_t var;
	uint32_t hi, lo;

	__asm volatile
	("rdtsc" : "=a"(lo), "=d"(hi));

	var = ((uint64_t)hi << 32) | lo;
	return (var);
#else
	struct timeval tv;
	gettimeofday(&tv, NULL);

	//return tv.tv_sec * 1000 + tv.tv_usec / 1000; //ms
	return tv.tv_sec * 1000000 + tv.tv_usec; //us
#endif
}


static void test0()
{
	int i = 0;
	uint8_t data[64] = {0};
	int16_t out[64] = {0};
	int16_t dct_dst[64] = {0};
	uint64_t s = 0, e = 0;

	for(i = 0; i < 64; i++) {
		data[i] = i;
	}

	printf("===> func: %s, line: %d, start\n", __func__, __LINE__);
	dump_uint8(data);
	s = get_time();
	//fastDCT_x86(data, out);
	fastDCT(data, out, 1);
	e = get_time();
	dump_int16(out);
	printf("===> func: %s, line: %d, dct run time=%ldus\n", __func__, __LINE__, e - s);

	fastIDCTInit(out, dct_dst);
	fastIDCT(dct_dst);
	dump_int16(dct_dst);
	printf("===> func: %s, line: %d, end\n", __func__, __LINE__);
}

static void test0_simd()
{
	int i = 0;
	uint8_t data[64] = {0};
	int16_t out[64] = {0};
	int16_t dct_dst[64] = {0};
	uint64_t s = 0, e = 0;

	for(i = 0; i < 64; i++) {
		data[i] = i;
	}

	printf("===> func: %s, line: %d, start\n", __func__, __LINE__);
	dump_uint8(data);
	s = get_time();
	fastDCT_x86(data, out);
	e = get_time();
	dump_int16(out);
	printf("===> func: %s, line: %d, dct run time=%ldus\n", __func__, __LINE__, e - s);

	fastIDCTInit(out, dct_dst);
	fastIDCT(dct_dst);
	dump_int16(dct_dst);
	printf("===> func: %s, line: %d, end\n", __func__, __LINE__);
}

static void test1()
{
	int i = 0;
	int16_t data[64] = {0};
	uint64_t s = 0, e = 0;

	for(i = 0; i < 64; i++) {
		data[i] = i;
	}

	printf("===> func: %s, line: %d, start\n", __func__, __LINE__);
	dump_int16(data);
	s = get_time();
	jpeg_fdct_islow(data);
	e = get_time();
	dump_int16(data);
	printf("===> func: %s, line: %d, end, run time=%ldus\n", __func__, __LINE__, e - s);
}

static void test1_simd()
{
	int i = 0;
	int16_t data[64] = {0};
	uint64_t s = 0, e = 0;

	for(i = 0; i < 64; i++) {
		data[i] = i;
	}

	printf("===> func: %s, line: %d, start\n", __func__, __LINE__);
	dump_int16(data);
	//jsimd_fdct_islow_avx2(data);
	s = get_time();
	_jsimd_fdct_islow_avx2(data);
	e = get_time();
	dump_int16(data);
	printf("===> func: %s, line: %d, end, run time=%ldus\n", __func__, __LINE__, e - s);
}

static void test2()
{
	int i = 0;
	int16_t data[64] = {0};
	uint64_t s = 0, e = 0;

	for(i = 0; i < 64; i++) {
		data[i] = i;
	}

	printf("===> func: %s, line: %d, start\n", __func__, __LINE__);
	dump_int16(data);
	s = get_time();
	jpeg_fdct_ifast(data);
	e = get_time();
	dump_int16(data);
	printf("===> func: %s, line: %d, end, run time=%ldus\n", __func__, __LINE__, e - s);
}

static void test2_simd()
{
	int i = 0;
	int16_t data[64] = {0};
	uint64_t s = 0, e = 0;

	for(i = 0; i < 64; i++) {
		data[i] = i;
	}

	printf("===> func: %s, line: %d, start\n", __func__, __LINE__);
	dump_int16(data);
	s = get_time();
	//jpeg_fdct_ifast(data);
	_jsimd_fdct_ifast_sse2(data);
	e = get_time();
	dump_int16(data);
	printf("===> func: %s, line: %d, end, run time=%ldus\n", __func__, __LINE__, e - s);
}

int main(int argc, const char *argv[])
{
	/* 测试整体编译通过为正常 */

	test0();
	test0_simd();
	test1();
	test1_simd();
	test2();
	test2_simd();

	return 0;
}

/* Copyright (C) 2026 wqshao All rights reserved.
 *
 *  File Name    : my_offsetof.c
 *  Author       : wqshao
 *  Created Time : 2026-03-05 13:55:09
 *  Description  :
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define  GET_MEMBER_SIZE(type, member)   sizeof(((type*)0)->member)
#define  GET_MEMBER_OFFSET(type, member)  ((size_t)(&(((type*)0)->member)))

typedef struct _test_struct0 {
	char x;
	char y;
	char z;
} test_struct0;

typedef struct _test_struct1 {
	char a;
	char c;
	short b;
	int d;
	test_struct0 e;
} test_struct1;

int main(int arc, char *argv[])
{
	printf("GET_MEMBER_SIZE(test_struct1, a) = %zu\n", GET_MEMBER_SIZE(test_struct1, a));
	printf("GET_MEMBER_SIZE(test_struct1, c) = %zu\n", GET_MEMBER_SIZE(test_struct1, c));
	printf("GET_MEMBER_SIZE(test_struct1, b) = %zu\n", GET_MEMBER_SIZE(test_struct1, b));
	printf("GET_MEMBER_SIZE(test_struct1, d) = %zu\n", GET_MEMBER_SIZE(test_struct1, d));
	printf("GET_MEMBER_SIZE(test_struct1, e) = %zu\n", GET_MEMBER_SIZE(test_struct1, e));
	printf("test_struct1 size = %zu\n", sizeof(test_struct1));

	printf("GET_MEMBER_OFFSET(a): %zu\n", GET_MEMBER_OFFSET(test_struct1, a));
	printf("GET_MEMBER_OFFSET(c): %zu\n", GET_MEMBER_OFFSET(test_struct1, c));
	printf("GET_MEMBER_OFFSET(b): %zu\n", GET_MEMBER_OFFSET(test_struct1, b));
	printf("GET_MEMBER_OFFSET(d): %zu\n", GET_MEMBER_OFFSET(test_struct1, d));
	printf("GET_MEMBER_OFFSET(e): %zu\n", GET_MEMBER_OFFSET(test_struct1, e));

	return 0;
}

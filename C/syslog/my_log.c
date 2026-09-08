/* Copyright (C) 2026 wqshao All rights reserved.
 *
 *  File Name    : my_log.c
 *  Author       : wqshao
 *  Created Time : 2026-03-17 10:56:02
 *  Description  :
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <stdarg.h>

// 使用 __attribute__ 告诉编译器：按照 printf 的规则检查，
// 格式字符串是第1个参数，可变参数从第2个参数开始。
void my_log(const char *format, ...) __attribute__((format(printf, 1, 2)));

void my_log(const char *format, ...)
{
	va_list args;
	va_start(args, format);
	// 将格式化后的内容输出到标准输出
	vprintf(format, args);
	va_end(args);
}

int main()
{
	int number = 42;
	char *text = "answer";

	// 正确的调用，编译器不会报错
	my_log("The %s is %d\n", text, number);

	// 错误的调用：%s 期望一个字符串，但我们传了一个整数
	// 如果编译时加上 -Wall 选项，GCC 会发出警告
	my_log("The %s is %d\n", number, text);

	return 0;
}

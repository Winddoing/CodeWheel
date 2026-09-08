/* Copyright (C) 2026 wqshao All rights reserved.
 *
 *  File Name    : tft_printf.c
 *  Author       : wqshao
 *  Created Time : 2026-03-30 13:51:49
 *  Description  :
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>

struct printf_info {
	char *bf;       //Digit buffer
	char uc;        //Output case character
	char zs;        //non-zero if a digit has been written
	char *outstr;   //Next output position for sprintf()

	/* Output a character */
	void (*putc)(struct printf_info *info, char ch);
};

static void out(struct printf_info *info, char c)
{
	*info->bf++ = c;
}

static void out_dgt(struct printf_info *info, char dgt)
{
	out(info, dgt + (dgt < 10 ? '0' : (info->uc ? 'A' : 'a') - 10));
	info->zs = 1;
}

static void div_out(struct printf_info *info, uint64_t *num, uint64_t div)
{
	unsigned char dgt = 0;

	while (*num >= div) {
		*num -= div;
		dgt++;
	}
	if (info->zs || dgt > 0)
		out_dgt(info, dgt);
}

static void output_num(struct printf_info *info, uint64_t *num, uint64_t div, int step)
{
	if (num) {
		for (; div; div /= step)
			div_out(info, num, div);
	} else {
		out_dgt(info, 0);
	}
}

static size_t strlen(const char * s)
{
        const char *sc;

        for (sc = s; *sc != '\0'; ++sc)
                /* nothing */;
        return sc - s;
}

int __tfp_printf(struct printf_info *info, const char *fmt, va_list va)
{
	char ch;
	char *p;
	uint64_t num, div;
	char buf[21] = {0};

	while ((ch = *(fmt++))) {
		if (ch != '%') {
			info->putc(info, ch);
		} else {
			//格式化输出
			char lz = 0;  //左补0
			char w = 0;   //输出宽度
			int left = 0; //左对齐

			ch = *(fmt++);

			// 适配%#lx输出
			if (ch == '#')
				ch = *(fmt++);

			// 解析左对齐标志 '-'
			if (ch == '-') {
				ch = *(fmt++);
				left = 1;
			}

			// 左补0
			if (ch == '0') {
				ch = *(fmt++);
				lz = 1;
				if (left)
					lz = 0;   // 左对齐时零填充无效
			}

			//处理宽度参数：数字或 *
			if (ch == '*') {
				w = va_arg(va, int);
				if (w < 0) {
					w = -w;
					left = 1;        // 负宽度表示左对齐
				}
				ch = *(fmt++);
			} else if (ch >= '0' && ch <= '9') {
				w = 0;
				while (ch >= '0' && ch <= '9') {
					w = (((w << 2) + w) << 1) + ch - '0';
					ch = *fmt++;
				}
			}


			// 忽略精度,%*s、%-*s、%.*s 和 %-*.*s 等格式
			if (ch == '.') {
				ch = *fmt++;                     // 跳过 '.'
				if (ch == '*') {
					(void)va_arg(va, int);       // 忽略精度值
					ch = *fmt++;                 // 跳过 '*'
				} else {
					while (ch >= '0' && ch <= '9') {
						ch = *fmt++;             // 跳过数字
					}
				}
			}

			int is_long = 0;
			int is_longlong = 0;

			if (ch == 'l') {
				ch = *fmt++;
				if (ch == 'l') {
					is_longlong = 1;
					ch = *fmt++;
				} else {
					is_long = 1;
				}
			}

			info->bf = buf;
			p = info->bf;
			info->zs = 0;

			switch (ch) {
			case '\0' :
				goto abort;
			case 'u' :
			case 'd' :
				if (is_longlong) {
					num = va_arg(va, unsigned long long);
					div = 1000000000000000000;
				} if (is_long) {
					num = va_arg(va, long);
					div = 1000000000000000000;
				} else {
					num = va_arg(va, unsigned int);
					num = (uint32_t)num;
					div = 100000000;
				}
#ifdef __x86_64
				//printf("dec: is_long=%d, num=%ld | ", is_long, num);
#endif
				if (is_longlong || is_long) {
					if ((int64_t)num < 0) {
						num = -(int64_t)num;
						out(info, '-');
					}
				} else {
					if ((int32_t)num < 0) {
						num = -(int32_t)num;
						out(info, '-');
					}
				}

				output_num(info, &num, div, 10); //十进制
				break;
			case 'x' :
			case 'X' :
				info->uc = (ch == 'X');
				if (is_longlong) {
					num = va_arg(va, unsigned long long);
					div = 0x1000000000000000;
				} if (is_long) {
					num = va_arg(va, unsigned long);
					div = 0x1000000000000000;
				} else {
					num = va_arg(va, unsigned int);
					div = 0x10000000;
				}
#ifdef __x86_64
				//printf("hex: is_long=%d, num=%lx | ", is_long, num);
#endif
				output_num(info, &num, div, 0x10);
				break;
			case 'c' :
				out(info, (char)(va_arg(va, int)));
				break;
			case 's' :
				p = va_arg(va, char *);
				break;
			case 'p' :
				num = (unsigned long)va_arg(va, void *);
				div = 0x1000000000000000;
				info->uc = 0;
				out(info, '0');
				out(info, 'x');
				output_num(info, &num, div, 0x10); //十六进制
				break;
			case '%' :
				out(info, '%');
			default:
				break;
			}

			*info->bf = 0;
			info->bf = p;
			int len = strlen(p);    // 计算实际字符串长度
			int pad = w - len;      // 需要填充的字符数
			if (pad < 0) pad = 0;

			if (left) {
				// 左对齐：先输出字符串，后填充空格
				while ((ch = *p++)) info->putc(info, ch);
				while (pad-- > 0) info->putc(info, ' ');
			} else {
				// 右对齐：先填充，后输出字符串
				while (pad-- > 0) info->putc(info, lz ? '0' : ' ');
				while ((ch = *p++)) info->putc(info, ch);
			}

#if 0
			while (*info->bf++ && w > 0)
				w--;
			while (w-- > 0)
				info->putc(info, lz ? '0' : ' ');
			while ((ch = *p++))
				info->putc(info, ch);
#endif
		}
	}
abort:
	return 0;
}

static void my_putc(struct printf_info *info, char c)
{
	putchar(c);
}

static int my_printf(struct printf_info *info, const char *fmt, ...)
{
	int ret = 0;
	va_list va;
	va_start(va, fmt);
	ret = __tfp_printf(info, fmt, va);
	va_end(va);

	return ret;
}

#define _(str) (str)

int main(int argc, const char *argv[])
{
	struct printf_info info;
	info.putc = my_putc;

	my_printf(&info, "abcdef\n");

	my_printf(&info, "%s\n", "123");
	my_printf(&info, "%*s\n", 12, "swq");

	{
		static size_t width[6];

		width[0] = 15;
		width[1] = 15;
		width[2] = 15;
		width[3] = 15;
		width[4] = 15;
		width[5] = 15;

		my_printf(&info, "%*s  %-*s%*s%*s%*s%*s\n",
				width[0], _("#"),
				width[1], _("Suite Name"),
				width[2], _("Init?"),
				width[3], _("Cleanup?"),
				width[4], _("#Tests"),
				width[5], _("Active?"));

		my_printf(&info, "\n%*d. %-*.*s%*s%*s%*u%*s\n",
				width[0], 1,
				width[1], width[1] - 1, "test123",
				width[2]-1, (NULL != NULL) ? _("Yes") : _("No"),
				width[3],   (NULL != NULL) ? _("Yes") : _("No"),
				width[4],   123,
				width[5],   (0 != 1) ? _("Yes") : _("No"));

	}

	return 0;
}

/*#############################################################
 *     File Name	: aa.c
 *     Author		: winddoing
 *     Created Time	: 2019年05月07日 星期二 15时34分49秒
 *     Description	:
 *############################################################*/
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int round_to_nearest = 0;

/* reference unmultiply implementetion (two types of rounding) */
int unmultiply_ref(int x, int a)
{
	if (round_to_nearest)
		return a == 0 ? x : (x * 255 + (a / 2)) / a;
	else
		return a == 0 ? x : (x * 255) / a;
}

/* division free unmultiply, better suitable for ARM NEON optimizations */
int unmultiply(int x, int a, int add8, int mul8, int mul16)
{
	if (round_to_nearest)
		return (x * mul8 * mul16 + (1 << 15)) >> 16;
	else
		return (x * mul8 * mul16 + add8) >> 16;
}

int try_consts(int a, int add8, int mul8, int mul16)
{
	int x;
	for (x = 0; x <= 255; x++)
		if (unmultiply_ref(x, a) != unmultiply(x, a, add8, mul8, mul16))
			break;
	return x;
}

int find_best_consts(int a, int *padd8, int *pmul8, int *pmul16)
{
	int add8, mul8, mul16;
	int best_add8, best_mul8, best_mul16;
	int max_fail_x = -1;

	/*
	 *      * try to find the constants, which preferably provide bit exact
	 *           * results for any 'x' up to 255 (just because we can)
	 *                */
	for (add8 = 0; add8 <= 255; add8++)
	{
		for (mul8 = 0; mul8 <= 255; mul8++)
		{
			for (mul16 = 0; mul16 <= 65535; mul16++)
			{
				int fail_x = try_consts(a, add8, mul8, mul16);
				if (fail_x > max_fail_x)
				{
					best_add8 = add8;
					best_mul8 = mul8;
					best_mul16 = mul16;
					max_fail_x = fail_x;
				}
				if (max_fail_x == 256)
					break;
			}
			if (max_fail_x == 256)
				break;
		}
		if (max_fail_x == 256)
			break;
	}

	/* but only supporting the case when '0 <= x <= a' is strictly required */
	if (max_fail_x <= a)
	{
		printf("!!! fail for a=%d\n", a);
		abort();
	}

	*padd8 = best_add8;
	*pmul8 = best_mul8;
	*pmul16 = best_mul16;
	return 1;
}

void print_table()
{
	int a, x;
	int add8, mul8, mul16;
	for (a = 0; a <= 255; a++)
	{
		find_best_consts(a, &add8, &mul8, &mul16);
		if (a % 4 == 0)
			printf("    .word ");
		printf("0x%08X", (add8 << 24) | (mul8 << 16) | mul16);
		if (a % 4 == 3)
			printf("\n");
		else
			printf(", ");
	}
	printf("\n");
}

int main(void)
{
	round_to_nearest = 0;
	printf("unmultiply_consts_table_round_down:\n");
	print_table();

	round_to_nearest = 1;
	printf("unmultiply_consts_table_round_to_nearest:\n");
	print_table();

	return 0;
}


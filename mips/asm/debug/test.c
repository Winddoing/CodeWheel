/*#############################################################
 *     File Name	: test.c
 *     Author		: wqshao
 *     Created Time	: 2017年10月13日 星期五 16时26分48秒
 *     Description	:
 *############################################################*/

#include<stdio.h>

int main()
{
	__asm__ volatile (
			".set push		    \n\t"  	
			".set noreorder		\n\t"
			".word   0x40028000 \n\t"             //# 8c02a000
			".word   0x3c030000 \n\t"             //# 8c02a004
			".word   0x2463fff8 \n\t"             //# 8c02a008
			".word   0x00431024 \n\t"             //# 8c02a00c
			".word   0x34420003 \n\t"             //# 8c02a010
			".word   0x40828000 \n\t"             //# 8c02a014
			".word   0x3c020800 \n\t"             //# 8c02a018
			".word   0x40826800 \n\t"             //# 8c02a01c
			".word   0x40804800 \n\t"             //# 8c02a020
			".word   0x3c021000 \n\t"             //# 8c02a024
			".word   0x40826000 \n\t"             //# 8c02a028
			".word   0x3c1d8067 \n\t"             //# 8c02a02c
			".word   0x27bd2448 \n\t"             //# 8c02a030
			".word   0x8fbd0000 \n\t"             //# 8c02a034
			".word   0x3c1c8067 \n\t"             //# 8c02a038
			".word   0x279c244c \n\t"             //# 8c02a03c
			".word   0x8f9c0000 \n\t"             //# 8c02a040
			".word   0x3c1f8049 \n\t"             //# 8c02a044
			".word   0x27ffb748 \n\t"             //# 8c02a048
			".word   0x03e00008 \n\t"             //# 8c02a04c
			".word   0x00000000 \n\t"             //# 8c02a050
			".word   0x00000000 \n\t"             //# 8c02a054
			".word   0x00000000 \n\t"             //# 8c02a058
			".word   0x00000000 \n\t"             //# 8c02a05c
			".set    pop		\n\t"
			);

	return 0;
}

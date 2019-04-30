/*#############################################################
 *     File Name	: mmx_ins.c
 *     Author		: winddoing
 *     Created Time	: 2019年04月30日 星期二 14时58分08秒
 *     Description	:
 *############################################################*/

#include <stdio.h>
#include <stdlib.h>

#include <xmmintrin.h>

/*gcc -Wall -mmmx -o ins  mmx_ins.c*/
int main(int argc,char *argv[])
{ 
	/*使用MMX做以下向量的点积*/
	short in1[] = {1, 2, 3, 4};
	short in2[] = {2, 3, 4, 5};
	int out1;
	int out2;

	__m64 m1;    /* MMX支持64位整数的mm寄存器 */
	__m64 m2;    /* MMX操作需要使用mm寄存器 */

	/*每次往mm寄存器装入两个short型的数，注意是两个*/
	m1 = _mm_cvtsi32_si64(((int*)in1)[0]);
	m2 = _mm_cvtsi32_si64(((int*)in2)[0]); 

	/*一条指令进行4个16位整数的乘加*/
	/*生成两个32位整数*/
	m2  = _mm_madd_pi16(m1, m2); 

	/*将低32位整数放入通用寄存器*/
	out1 =  _mm_cvtsi64_si32(m2);

	/*将高32位整数右移后，放入通用寄存器*/
	m2  = _mm_slli_pi32(m2, 32);
	out2 =  _mm_cvtsi64_si32(m2);

	/*清除MMX状态*/
	_mm_empty();
	/*将两个32位数相加，结果为8*/
	out1 += out2;

	printf("a: %d\n", out1);

	return(0);
}

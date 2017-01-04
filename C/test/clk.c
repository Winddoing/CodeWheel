#include <stdio.h>

int main()
{
	unsigned int clk_set, clkrt = 0;

	clk_set = 600000000;

	while (24000000 < clk_set) {                                                                                          
		clkrt++;
		clk_set >>= 1;
	}

	printf("clk_set:%d, clkrt:%d, actual_clk:%d\n",clk_set, clkrt, clk_set/(clkrt+1));



	return 0;
}

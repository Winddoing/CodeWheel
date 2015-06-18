#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef union{
   uint16_t rgb;	
	struct {
		uint16_t b : 5;
		uint16_t g : 6;
		uint16_t r : 5;
	}colors;
}Color;

int main(int argc, char *argv[])
{
	uint8_t r = 0xFF;
	uint8_t g = 0xCC;
	uint8_t b = 0x99;
	Color color;
	//======================
	fprintf(stdout, "sizeof(color)=%d\n", sizeof(color));
	int *pcolor = (int *)&color;
	short *pcolor_s = (short*)&color.colors; 

	*pcolor = 0x1234567;

	fprintf(stdout, "Pcolor  =%8X\n", *pcolor);
	fprintf(stdout, "Pcolor_s=%8X\n", *pcolor_s);

	fprintf(stdout, "rgb_r=%8X\n", color.colors.r);
	fprintf(stdout, "rgb_g=%8X\n", color.colors.g);
	fprintf(stdout, "rgb_b=%8X\n", color.colors.b);

	color.colors.r = 0x12;
	color.colors.g = 0x22;
	color.colors.b = 0x11;

	fprintf(stdout, "rgb=%8X\n", color.colors);
	//======================
	color.colors.r = r >> 3;
	color.colors.g = g >> 2;
	color.colors.b = b >> 3;

	fprintf(stdout, "%04X\n", color.rgb);

	return EXIT_SUCCESS;	
}

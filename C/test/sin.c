#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char *argv[])
{
	fprintf(stdout,  "sin(75)=%.6f\n",
			 sin(75.0 * M_PI / 180.0));

	return EXIT_SUCCESS;
}

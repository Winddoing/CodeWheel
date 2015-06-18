#include <stdio.h>
#include <stdlib.h>
#include <round.h>

#define RADIUS	1.0

int main(int argc, char *argv[])
{
	fprintf(stdout, "r = %.2lf\n"
					"c = %.2lf\n"
					"s = %.2lf\n"
					"v = %.2lf\n",
			RADIUS, circle(RADIUS),
			area(RADIUS),
			volume(RADIUS));

	return EXIT_SUCCESS;
}



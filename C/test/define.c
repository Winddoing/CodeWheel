#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{

	fprintf(stdout,   "%s:%s:%s\n"
				__FILE__,__FUNCTION__,__LINE__);
	return EXIT_SUCCESS;
}

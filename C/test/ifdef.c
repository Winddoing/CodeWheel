#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	#if 1
		fprintf(stdout,   "%s:%s:%s\n"
				__FILE__,__FUNCTION__,__LINE__);
	#else
		fpeintf(stdout, "Hello GCC\n");
	#endif
	return EXIT_SUCCESS;
}

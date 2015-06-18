#include <stdio.h>
#include <stdlib.h>

#include <dirtree.h>

int n = 0;

int main(int argc, char *argv[])
{
	
	if(argc < 2)
	{
		fprintf(stderr, "%sUsage:<dirname>\n", 
					argv[0]);
		return EXIT_FAILURE;
	}
	openDirList(argv[1]);

	return EXIT_SUCCESS;
}


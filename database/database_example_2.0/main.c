#include <stdio.h>
#include <stdlib.h>
#include <mymysql.h>
#include <wave.h>

int main(int argc, char *argv[])
{
	if(0 != connectToMysql())
		return -1;
	
	if(-1 == querySql("SELECT name, bits, rate, channels FROM baseinfo"))
		return -1;

	fprintf(stdout, "Connect to mysql successed!\n");

	return EXIT_SUCCESS;
}



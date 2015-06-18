#include <stdio.h>
#include <stdlib.h>
#include <mymysql.h>
#include <wavedata.h>

int main(int argc, char *argv[])
{
	if(0 != connectToMysql())
		return -1;
	
	if(-1 == InsertWaveFile("xp.wav"))
		return -1;

	fprintf(stdout, "Connect to mysql successed!\n");

	return EXIT_SUCCESS;
}



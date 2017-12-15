#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "key.h"
/*#include "splsrc16k.h"*/
#include "splsrc16k-old.h"
/*#include "x2000-spl.h"*/

int main(int argc, char **argv)
{
	int i, len;
	int splcodemd5[8] = {0};

	if(!strcmp(argv[1], "key")) {
		do_md5(rsanku, sizeof(rsanku), splcodemd5);
		for (i = 0; i < 4; i++)
			printf("key md5 0x%08x\n", splcodemd5[i]);
	}

	len = sizeof(spl);
	printf("spl len : %d\n", len);
	if(!strcmp(argv[1], "spl")) {
		do_md5(spl, len, splcodemd5);
/*		do_md5(spl, atoi(argv[2]), splcodemd5);*/
		for (i = 0; i < 8; i++)
			printf("key md5 0x%08x\n", splcodemd5[i]);
	}

	return 0;
}

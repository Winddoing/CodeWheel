#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "key.h"
#include "splsrc16k.h"
//#include "splckenc16k.h"
//#include "splukenc16k.h"

void data_pri(unsigned int *data, unsigned int len)
{
	unsigned int i;
	for(i = 0; i < len / 4; i += 4)
			printf("0x%08x, 0x%08x, 0x%08x, 0x%08x, \n",
					data[i], data[i + 1], data[i + 2], data[i + 3]);
}

int main(int argc, char **argv)
{
	unsigned int len;

#if 1

	if(!strcmp(argv[1], "ckey") && !strcmp(argv[2], "spl"))
		do_aes(chipkey, spl, atoi(argv[3]), 1);
	else if(!strcmp(argv[1], "ukey") && !strcmp(argv[2], "spl"))
		do_aes(burnukey, spl, atoi(argv[3]), 1);

	if(!strcmp(argv[2], "spl"))
		data_pri(spl, atoi(argv[3]));
#endif

#if 1
	if(!strcmp(argv[1], "ckey") && !strcmp(argv[2], "rsakey")) {

		printf("==============>>> ckey encrypt rsa key .....\n");
		len = 32 * 4;

		printf("====>> rsan .....\n");
		data_pri(rsan, len);
		do_aes(chipkey, rsan, len, 1);
		data_pri(rsan, len);
		do_aes(chipkey, rsan, len, 0);
		data_pri(rsan, len);

		printf("====>> rsaku .....\n");
		data_pri(rsaku, len);
		do_aes(chipkey, rsaku, len, 1);
		data_pri(rsaku, len);
		do_aes(chipkey, rsaku, len, 0);
		data_pri(rsaku, len);

		printf("====>> rsakr .....\n");
		data_pri(rsakr, len);
		do_aes(chipkey, rsakr, len, 1);
		data_pri(rsakr, len);
		do_aes(chipkey, rsakr, len, 0);
		data_pri(rsakr, len);

	} else if(!strcmp(argv[1], "ukey") && !strcmp(argv[2], "rsakey")) {

		printf("==============>>> ukey encrypt rsa key .....\n");
		len = 32 * 4;

		printf("====>> rsan .....\n");
		data_pri(rsan, len);
		do_aes(burnukey, rsan, len, 1);
		data_pri(rsan, len);
		do_aes(burnukey, rsan, len, 0);
		data_pri(rsan, len);

		printf("====>> rsaku .....\n");
		data_pri(rsaku, len);
		do_aes(burnukey, rsaku, len, 1);
		data_pri(rsaku, len);
		do_aes(burnukey, rsaku, len, 0);
		data_pri(rsaku, len);

		printf("====>> rsakr .....\n");
		data_pri(rsakr, len);
		do_aes(burnukey, rsakr, len, 1);
		data_pri(rsakr, len);
		do_aes(burnukey, rsakr, len, 0);
		data_pri(rsakr, len);
	} else if(!strcmp(argv[1], "rkey") && !strcmp(argv[2], "chipkey")) {
		len = 4 * 4;
		data_pri(chipkey, len);
		do_aes(rootkey, chipkey, len, 1);
		data_pri(chipkey, len);
		do_aes(rootkey, chipkey, len, 0);
		data_pri(chipkey, len);
	} else if(!strcmp(argv[1], "rkey") && !strcmp(argv[2], "userkey")) {
		len = 4 * 4;
		data_pri(burnukey, len);
		do_aes(rootkey, burnukey, len, 1);
		data_pri(burnukey, len);
		do_aes(rootkey, burnukey, len, 0);
		data_pri(burnukey, len);
	} else if(!strcmp(argv[1], "rkey") && !strcmp(argv[2], "nkusig")) {
		len = 4 * 4;
		data_pri(rsankusig, len);
		do_aes(rootkey, rsankusig, len, 1);
		data_pri(rsankusig, len);
		do_aes(rootkey, rsankusig, len, 0);
		data_pri(rsankusig, len);
	} else if(!strcmp(argv[1], "ckey") && !strcmp(argv[2], "userkey")) {
		len = 4 * 4;
		data_pri(burnukey, len);
		do_aes(chipkey, burnukey, len, 1);
		data_pri(burnukey, len);
		do_aes(chipkey, burnukey, len, 0);
		data_pri(burnukey, len);
	} else if(!strcmp(argv[1], "aes") && !strcmp(argv[2], "userkey")) {
		{
			unsigned int aes_k[4] = {0x7241ec44,0x41196d8d,0xaf30da74,0xad04f282};  
/*			unsigned int chipkey[4] = {0, 0, 0, 0};  */
			unsigned int chipkey[4] = {0x5eb234ab,0x7ca34546,0x1805c32c,0x0a08f751};  //userkey
/*			unsigned int chipkey[4] = {0x5eb234a8,0x7ca34544,0x1805c32c,0xa08f750};  //userkey*/


			len = 4 * 4;
			data_pri(aes_k, len);
			do_aes(chipkey, aes_k, len, 1);
			data_pri(aes_k, len);
			do_aes(chipkey, aes_k, len, 0);
			data_pri(aes_k, len);
		}
	}

#endif
	return 0;
}

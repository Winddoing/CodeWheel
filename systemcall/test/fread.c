/*#############################################################
 *     File Name	: fread.c
 *     Author		: wqshao
 *     Created Time	: 2017年03月13日 星期一 17时28分25秒
 *     Description	:
 *############################################################*/

#include<stdio.h>
#include <stdlib.h>  

int main(int argc, const char *argv[])
{
	char *buffer;
	int size=16384;
	unsigned int file;
	unsigned int num_read = 0;

	file = fopen("/home_back/wqshao/test/master.tar.gz", "rb");
	buffer = malloc(size); 
				 
	num_read = fread(buffer, 1, size, file);    

	printf("num_read=%d\n", num_read);
	close(file);
	return 0;
}

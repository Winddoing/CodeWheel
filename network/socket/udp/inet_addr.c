/*#############################################################
 *     File Name	: inet_addr.c
 *     Author		: wqshao
 *     Created Time	: 2018年05月10日 星期四 23时28分25秒
 *     Description	:
 *############################################################*/

#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>            
#include <sys/socket.h>           

#include <netinet/in.h>     


int main()
{
	in_addr_t addr; 
/*	addr = inet_addr("127.0.0.1");*/
	addr = inet_addr("0.0.0.0");
	printf("\n%u, INADDR_ANY=%u\n", addr, INADDR_ANY);
	return 0;
}


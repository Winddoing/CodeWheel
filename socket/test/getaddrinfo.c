/*#############################################################
 *     File Name	: getaddrinfo.c
 *     Author		: wqshao
 *     Created Time	: 2018年09月20日 星期四 11时06分12秒
 *     Description	:
 *############################################################*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

int main(int argc, char **argv)
{
	if (argc != 2) {
		printf("Usag...\n");
		printf("  %s 127.0.0.1\n", argv[0]);
		printf("  %s www.baidu.com\n", argv[0]);
		exit(1);
	}

	struct addrinfo hints;
	struct addrinfo *res, *cur;
	int ret;
	struct sockaddr_in *addr;
	char ipbuf[16];

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET; /* Allow IPv4 */
	hints.ai_flags = AI_PASSIVE; /* For wildcard IP address */
	hints.ai_protocol = 0; /* Any protocol */
	hints.ai_socktype = SOCK_STREAM;

	ret = getaddrinfo(argv[1], NULL, &hints, &res);
	if (ret == -1) {
		perror("getaddrinfo");
		exit(1);
	}

	for (cur = res; cur != NULL; cur = cur->ai_next) {
		addr = (struct sockaddr_in *)cur->ai_addr;
		printf("%s\n", inet_ntop(AF_INET, 
					&addr->sin_addr, ipbuf, 16));
	}

	freeaddrinfo(res);

	return 0;
}

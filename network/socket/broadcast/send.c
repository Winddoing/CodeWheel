/*#############################################################
 *     File Name	: send.c
 *     Author		: wqshao
 *     Created Time	: 2018年07月04日 星期三 14时27分09秒
 *     Description	:
 *############################################################*/

#include <stdio.h>
#include <stdlib.h>

#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>


#define LIMITED_BROADCAST

#ifdef LIMITED_BROADCAST
  #define BROADCAST_IP	"255.255.255.255" /* INADDR_BROADCAST */
#else
  #define BROADCAST_IP	"172.16.186.255"
#endif
#define BROADCAST_PORT	12345

int main(int argc, char *argv[])
{
	int fd;
	int broad_flag = 1;
	struct sockaddr_in to;
	char *msg = "hello world.";
	int packet_num = 0;
	int ret = -1;

	fd = socket(AF_INET, SOCK_DGRAM, 0);
	if(-1 == fd) {
		perror("create socket");
		exit(2);
	}

	ret = setsockopt(fd, SOL_SOCKET, SO_BROADCAST, &broad_flag, sizeof(int));
	if (-1 == ret) {
		perror("setsockopt: SO_BROADCAST");
		close(fd);
		return -1;
	}

	to.sin_family = AF_INET;
	to.sin_addr.s_addr = inet_addr(BROADCAST_IP);
	to.sin_port = htons(BROADCAST_PORT);

	while(1) {
		ret = sendto(fd, msg, strlen(msg)+1, 0, (struct sockaddr*)&to,sizeof(struct sockaddr_in));
		if(-1 == ret) {
			perror("sendto");
			exit(2);
		}
		printf("broad %d packets.[%s]\n", ++packet_num, msg);

		sleep(1);
	}

	close(fd);

	return 0;
}

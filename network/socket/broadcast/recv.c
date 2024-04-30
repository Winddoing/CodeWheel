/*#############################################################
 *     File Name	: recv.c
 *     Author		: wqshao
 *     Created Time	: 2018年07月04日 星期三 14时41分49秒
 *     Description	:
 *############################################################*/

#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define LIMITED_BROADCAST

#ifdef LIMITED_BROADCAST
  #define BROADCAST_IP	"255.255.255.255"
#else
  #define BROADCAST_IP	"172.16.186.255"
#endif
#define PORT_NUM 12345

int main(int argc, char *argv[])
{
	char buf[1024];
	int fd;
	struct sockaddr_in sendaddr;
	int recv_len = sizeof(struct sockaddr_in);
	int ret = -1;

	struct sockaddr_in myaddr;

	fd = socket(AF_INET, SOCK_DGRAM, 0);
	if(-1 == fd) {
		perror("create socket");
		return -1;
	}

	myaddr.sin_family = AF_INET;
/*	myaddr.sin_addr.s_addr = htonl(INADDR_ANY);*/
	myaddr.sin_addr.s_addr = inet_addr(BROADCAST_IP);
	myaddr.sin_port = htons(PORT_NUM);

	ret = bind(fd, (struct sockaddr *)&myaddr, sizeof(struct sockaddr_in));
	if(-1 == ret) {
		perror("bind");
		close(fd);
		return -1;
	}

	while(1) {
		ret = recvfrom(fd, buf, sizeof(buf), 0, (struct sockaddr*)&sendaddr, &recv_len);
		if(-1 == ret) {
			perror("recv");
			exit(4);
		}

		printf("recv data from address: %s data: %s\n", inet_ntoa(sendaddr.sin_addr), buf);
	}
	close(fd);
}

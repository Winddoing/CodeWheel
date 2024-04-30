/*#############################################################
 *     File Name	: tcp_udp_tc.c
 *     Author		: wqshao
 *     Created Time	: 2018年09月28日 星期五 18时48分28秒
 *     Description	:
 *############################################################*/

/*
 * TCP
 * client
 * API
 * usage:
 * ./client 192.168.1.78
 *
 * notes:
 * client 端connect()成功之后，此时立刻关闭sockfd断开连接，
 * server 端recv()会立刻返回，其返回值为0；
 * */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define SERVER_PORT 8888
#define MAXLINE 4096

int main(int argc, char** argv)
{
	int    sockfd, n;
	char    recvline[MAXLINE], sendline[MAXLINE];
	struct sockaddr_in    servaddr;
	int recv_len;
	int ret;

	memset(recvline, 0, MAXLINE);
	memset(sendline, 0, MAXLINE);

	if (argc != 2) {
		printf("usage: ./client <ipaddress>\n");
		exit(0);
	}

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("create socket error: %s(errno: %d)\n", strerror(errno),errno);
		exit(0);
	}

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERVER_PORT);

	if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0) {
		printf("inet_pton error for %s/n",argv[1]);
		close(sockfd);
		exit(0);
	}

	if(connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
		printf("connect error: %s(errno: %d)/n",strerror(errno),errno);
		close(sockfd);
		exit(0);
	}


	/*sleep(10);close(sockfd);exit(0);*/
	printf("send msg to server: \n");

	fgets(sendline, MAXLINE - 1, stdin);
	ret = send(sockfd, sendline, strlen(sendline), 0);
	printf("the len of send data = %d.\n", ret);
	if (ret < 0) {
		printf("send msg error: %s(errno: %d)\n", strerror(errno), errno);
		close(sockfd);
		exit(0);
	} else if (ret == 0) {//return 0
		printf("socket disconnnect(or the other side shutdown socket fd, or network problem) and send error: %s(errno: %d)\n",
				strerror(errno), errno);
		close(sockfd);
		exit(0);
	}

	recv_len = recv(sockfd, recvline, MAXLINE - 1, 0);
	if (recv_len < 0) {
		printf("recv socket error: %s(errno: %d)\n",strerror(errno),errno);
		close(sockfd);
		exit(0);
	} else if (recv_len == 0) {//return 0
		printf("socket disconnnect(or the other side shutdown socket fd, or network problem) and recv error: %s(errno: %d)\n",
				strerror(errno), errno);
		close(sockfd);
		exit(0);
	}

	recvline[recv_len] = '\0';
	printf("from server: %s\n", recvline);

	close(sockfd);
	exit(0);
}

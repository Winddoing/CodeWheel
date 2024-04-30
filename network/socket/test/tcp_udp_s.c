/*#############################################################
 *     File Name	: tcp_udp_s.c
 *     Author		: wqshao
 *     Created Time	: 2018年09月28日 星期五 18时42分22秒
 *     Description	:
 *############################################################*/

/*
 * TCP
 * INET
 * use select
 * */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>


#define LENGTH_OF_LISTEN_QUEUE  20
#define SERVER_PORT 8888
#define MAXLINE 4096
#define MAX_FD_NUM  10


static int init_new_client(int client_fd);
static int remove_client(int client_fd);
static int get_max_fd(int fd);


static int client_fdset[MAX_FD_NUM];

int main(int argc, char** argv)
{
	int    tcp_fd, udp_fd, connfd, client_fd;
	struct sockaddr_in  servaddr;
	struct sockaddr_in  client_addr;
	socklen_t client_addr_len = sizeof(client_addr);
	char    recv_buff[4096];
	char    response[] = "recv well done.";
	int     recv_len;
	fd_set readfd;
	int ret;
	int max_fd;
	int i;

	for (i = 0; i < MAX_FD_NUM; i++) {
		client_fdset[i] = -1;
	}

	if ((tcp_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1 ) {
		printf("create socket error: %s(errno: %d)\n",strerror(errno),errno);
		exit(0);
	}


	if ((udp_fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1 ) {
		printf("create socket error: %s(errno: %d)\n",strerror(errno),errno);
		exit(0);
	}

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERVER_PORT);

	if (bind(tcp_fd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1) {
		printf("tcp bind socket error: %s(errno: %d)\n",strerror(errno),errno);
		exit(0);
	}

	if (bind(udp_fd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1) {
		printf("udp bind socket error: %s(errno: %d)\n",strerror(errno),errno);
		exit(0);
	}

	if (listen(tcp_fd, LENGTH_OF_LISTEN_QUEUE) == -1) {
		printf("listen socket error: %s(errno: %d)\n",strerror(errno),errno);
		exit(0);
	}

	printf("----------waiting for client's request---------\n");
	while (1) {

		/*每次调用select前，都要重置监听的描述符*/
		FD_ZERO(&readfd);
		FD_SET(tcp_fd, &readfd);
		FD_SET(udp_fd, &readfd);
		for (i = 0; i < MAX_FD_NUM; i++) {
			if (client_fdset[i] != -1) {
				FD_SET(client_fdset[i], &readfd);
			}
		}

		int fd = tcp_fd;
		if (udp_fd > tcp_fd) {
			fd = udp_fd;
		}

		max_fd = get_max_fd(fd);
		ret = select(max_fd + 1, &readfd, NULL, NULL, NULL);//有数据时才返回
		if (ret == -1) {  //错误情况
			if (errno == EINTR) {//signal interrupt
				continue;
			} else {
				perror("select error");
				exit(0);
			}
		} else if (ret) {    //返回值大于0 有数据到来
			if (FD_ISSET(tcp_fd, &readfd)) {
				if((connfd = accept(tcp_fd, (struct sockaddr *)&client_addr, &client_addr_len)) == -1) {
					printf("accept socket error: %s(errno: %d)\n",strerror(errno),errno);
					continue;
				}
				printf("accept a new  connection from client IP: %s, port: %u\n", inet_ntoa(client_addr.sin_addr),                      ntohs(client_addr.sin_port));
				ret = init_new_client(connfd);
				if (ret != 0) {
					printf("init_new_client() failed ret = %d.\n", ret);
					close(connfd);
				}

			} else if (FD_ISSET(udp_fd, &readfd)) {

				int ret =recvfrom(udp_fd,recv_buff, MAXLINE - 1, 0,(struct sockaddr *)&client_addr, &client_addr_len);
				if(ret<0){
					perror("recvfrom error_1");
					continue;
				}

				recv_buff[ret] = '\0';
				printf("udp recv:");
				printf("recv msg %d byte from client: %s\n", recv_len, recv_buff);

			} else {
				/*one fd  can be read.*/
				for (i = 0; i < MAX_FD_NUM; i++) {
					if (client_fdset[i] != -1 && FD_ISSET(client_fdset[i], &readfd)) {
						client_fd = client_fdset[i];
						break;
					}
				}

				memset(recv_buff, 0, MAXLINE);
				recv_len = recv(client_fd, recv_buff, MAXLINE - 1, 0);
				printf("tcp recv:");
				printf("recv len = %d\n", recv_len);
				if (recv_len <= 0) {
					if (recv_len == 0) {
						printf("socket disconnnect(or the other side shutdown socket fd, or network problem)\n");
						printf("recv socket error: %s(errno: %d)\n",strerror(errno),errno);
					}
					else {
						printf("recv socket error: %s(errno: %d)\n",strerror(errno),errno);
					}
					ret = remove_client(client_fd);
					if (ret != 0) {
						printf("remove_client() failed ret = %d.\n", ret);
					}
					close(client_fd);
					continue;
				}
				recv_buff[recv_len] = '\0';
				printf("recv msg %d byte from client: %s\n", recv_len, recv_buff);

				if (send(client_fd, response, strlen(response), 0) < 0) {
					printf("send socket error: %s(errno: %d)\n", strerror(errno), errno);
					ret = remove_client(client_fd);
					if (ret != 0) {
						printf("remove_client() failed ret = %d.\n", ret);
					}
					close(client_fd);
					continue;
				}
			}
		} else {
			//ret 为0，超时情况
			printf("time out\n");
			//close(keybd_fd);//产生异常，查看结果
		}
	}

	close(tcp_fd);
	close(udp_fd);
	printf("--------server exit------.\n");

	return 0;
}

static int init_new_client(int client_fd)
{
	int i;

	for(i = 0; i < MAX_FD_NUM; i++) {
		if (client_fdset[i] == -1) {
			client_fdset[i] = client_fd;
			break;
		}
	}

	if (i == MAX_FD_NUM) {
		printf("too many client.\n");
		return -1;
	}

	return 0;
}

static int remove_client(int client_fd)
{

	int i;

	for(i = 0; i < MAX_FD_NUM; i++) {
		if (client_fdset[i] == client_fd) {
			client_fdset[i] = -1;
			break;
		}
	}

	if (i == MAX_FD_NUM) {
		printf("client fd is not in list.\n");
		return -1;
	}

	return 0;

}

static int get_max_fd(int fd)
{
	int i;

	for(i = 0; i < MAX_FD_NUM; i++) {
		if (client_fdset[i] > fd) {
			fd = client_fdset[i];
		}
	}

	return fd;
}


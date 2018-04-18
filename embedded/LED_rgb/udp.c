#include<stdio.h>
#include<string.h>
#include<errno.h>
#include<stdlib.h>
#include<unistd.h>

#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>

#include "udp.h"

static int sock_fd;
static struct sockaddr_in addr_serv;
static int dest_len = sizeof(struct sockaddr_in);
static udp_state_t udp_state;

int udp_init(char *ip, int port)
{
	if(udp_state == udp_connect)
		close(sock_fd);
	udp_state = udp_unconnect;
	sock_fd = socket(AF_INET,SOCK_DGRAM,0);
	if(sock_fd)
		udp_state = udp_connect;
	memset(&addr_serv,0,sizeof(addr_serv));
	addr_serv.sin_family = AF_INET;
	addr_serv.sin_addr.s_addr = inet_addr(ip);
	addr_serv.sin_port = htons(port);
	return sock_fd;
}
int udp_send(char *send_buf, int len)
{
	int send_num = -1;
	if(sock_fd)
		send_num = sendto(sock_fd,send_buf,len,0,(struct sockaddr *)&addr_serv,dest_len);
	return send_num;
}
int udp_recv(char *recv_buf, int max_len)
{
	int recv_num = -1;
	if(sock_fd)
		recv_num = recvfrom(sock_fd,recv_buf,max_len,MSG_DONTWAIT,(struct sockaddr *)&addr_serv,&dest_len);
	return recv_num;
}

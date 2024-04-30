#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "udp.h"

static int ipGrade(in_addr_t addr)
{
	if(INADDR_NONE == addr)
		return IP_GRADE_N;

	addr &= 0xFF;
	if(addr <= 127)
		return IP_GRADE_A;
	if(addr <= 191)
		return IP_GRADE_B;
	if(addr <= 223)
		return IP_GRADE_C;
	else
		return IP_GRADE_O;
}

static in_addr_t broadcastAddr(in_addr_t addr)
{
	if(INADDR_ANY == addr)
		return 0x00000000;
	return addr | ipGrade(addr);
}

static int creatUdp(netInfo *info)
{

	return UDPERR_SUCCESS;
}

int creatUdpServer(netInfo *info)
{
	in_addr_t addr;
	struct sockaddr_in srvAddr;

	//step 1.check ip addr
	addr = inet_addr(info->ipAddr);
	if(INADDR_NONE == addr)
		return UDPERR_INVADDR;

	//step 2.creat socket file
	info->sockFd = socket(AF_INET,
			SOCK_DGRAM, 0);
	if(-1 == info->sockFd)
		return UDPERR_CRESOCK;

	//step 3.set srvAddr struct
	bzero(&srvAddr, sizeof(srvAddr));
	srvAddr.sin_family = AF_INET;
	srvAddr.sin_port = htons(info->port);
	srvAddr.sin_addr.s_addr = addr;

	//step 4.bind socket info
	if(-1 == bind(info->sockFd, (struct sockaddr *)&srvAddr, sizeof(srvAddr))) {
		close(info->sockFd);
		info->sockFd = -1;
		return UDPERR_BIND;
	}

	return UDPERR_SUCCESS;
}

int creatUdpClient(netInfo *info)
{
	in_addr_t addr;
	struct sockaddr_in srvAddr;

	//step 1.check ip addr
	addr = inet_addr(info->ipAddr);
	if(INADDR_NONE == addr)
		return UDPERR_INVADDR;

	//step 2.creat socket file
	info->sockFd = socket(AF_INET,
			SOCK_DGRAM, 0);
	if(-1 == info->sockFd)
		return UDPERR_CRESOCK;

	//step 3.set srvAddr struct
	bzero(&srvAddr, sizeof(srvAddr));
	srvAddr.sin_family = AF_INET;
	srvAddr.sin_port = htons(info->port);
	srvAddr.sin_addr.s_addr = addr;

	return UDPERR_SUCCESS;
}

int sendData(const netInfo* info, char* buf, unsigned int len)
{
	int ret;
	struct sockaddr_in addr;

	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(info->port);
	addr.sin_addr.s_addr = broadcastAddr(
				inet_addr(info->ipAddr));
	ret = sendto(info->sockFd, buf, len, 0,
			(struct sockaddr *)&addr, sizeof(addr));
	if(-1 == ret)
		return UDPERR_SEND;

	return UDPERR_SUCCESS;
}

int recvData(netInfo* info, char* buf, unsigned int len)
{
	int ret;
	fd_set rfds;
	struct timeval tv;
	struct sockaddr_in addr;
	socklen_t sockLen;

	tv.tv_sec = 1;
	tv.tv_usec = 0;

	FD_ZERO(&rfds);
	FD_SET(info->sockFd, &rfds);
	ret = select(info->sockFd + 1, &rfds, NULL, NULL, &tv);
	if(-1 == ret)
		return UDPERR_SELECT;
	if(0 == ret)
		return UDPERR_TIMEOUT;

	bzero(&addr, sizeof(addr));
	sockLen = sizeof(addr);
	ret = recvfrom(info->sockFd, buf, len, 0,
					(struct sockaddr *)&addr, &sockLen);
	if(-1 == ret)
		return UDPERR_RECIVE;
	fprintf(stdout, "%s=>%s:%d\n", __func__, inet_ntoa(addr.sin_addr),
			ntohs(addr.sin_port));

	return UDPERR_SUCCESS;
}

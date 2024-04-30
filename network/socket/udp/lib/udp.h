#ifndef __UDP_H__
#define __UDP_H__

#define IP_GRADE_A	0xFFFFFF00
#define IP_GRADE_B	0xFFFF0000
#define IP_GRADE_C	0xFF000000
#define IP_GRADE_O	0XFFFFFFFF
#define IP_GRADE_N	0	//Error IP

#define UDPERR_SUCCESS	0
#define UDPERR_INVADDR	1
#define UDPERR_CRESOCK	2
#define UDPERR_BIND		3
#define UDPERR_RECIVE	4
#define UDPERR_SEND		5
#define UDPERR_TIMEOUT	6
#define UDPERR_SELECT	7
#define SOCKERR_SETOPT	8

typedef struct{
	const char *ipAddr;
	uint16_t port;
	int sockFd;
}netInfo;

int creatUdpBind(netInfo *info);
int sendData(const netInfo* info, char* buf, unsigned int len);
int recvData(netInfo* info, char* buf, unsigned int len);

#endif//__UDP_H__

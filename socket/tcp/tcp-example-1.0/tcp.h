#ifndef TCP_H
#define TCP_H

#include <stdint.h>
#include <sys/socket.h>

#define DEF_BACKLOG	5

typedef struct{
	const char *ipAddr; //ip 点分制
#define LOCAL_SOCKET PF_LOCAL	//用于本地通信
#define REMOT_SOCKET PF_INET	//用于远程通信
	int domain;			//网络通信类型,本机或远程
	uint16_t port;		//port 无符号短整型
	uint16_t backlog;	//等待队列长度
	int sockFd;			//文件描述符,不要修改
}IpInfo;

#define DEF_MAX_LENG	4096
typedef struct{
	uint16_t maxLength;
	uint16_t dataLength;
	uint16_t type;
	char * data;
}Data;

#define ERR_NOMEM		10
#define ERR_SUCCESS		0
int initData(Data *data);
void destroyData(Data *data);


#define TCPERR_SUCCESS	0
#define TCPERR_CRESOCK	1
#define TCPERR_INVADDR	2
#define TCPERR_BIND		3
#define TCPERR_LISTEN	4
#define TCPERR_CONNECT	5
#define TCPERR_ACCEPT	6
#define TCPERR_WRITE	7
#define TCPERR_READ 	8
#define TCPERR_DISCONNECT	9

int writeData(const IpInfo *info, const Data *data);
int readData(const IpInfo *info, Data *data);

int creatTcpListen(IpInfo *info);

int connectToHost(IpInfo *info);

int acceptConnect(const IpInfo *listenInfo, IpInfo *clientInfo);
#endif //TCP


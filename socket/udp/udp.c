#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>

#define DEF_BACKLOG	5
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


typedef struct{
	const char *ipAddr; //ip 点分制
	const char *destIp; //目标ip
	uint16_t port;		//port 无符号短整型
	int sockFd;			//文件描述符,不要修改
}IpInfo;

int creatTcpListen(IpInfo *info)
{
	in_addr_t addr;
	struct sockaddr_in srvAddr;

	addr = inet_addr(info->ipAddr);
	if(INADDR_NONE == addr)
		return TCPERR_INVADDR;
	info->sockFd = socket(PF_INET,
			SOCK_STREAM, 0);
	if(-1 == info->sockFd)
		return TCPERR_CRESOCK;
	bzero(&srvAddr, sizeof(srvAddr));
	srvAddr.sin_addr.s_addr = addr;

	if(-1 == bind(info->sockFd, 
			(struct sockaddr *)&srvAddr,
			sizeof(srvAddr)))
	{
		close(info->sockFd);
		info->sockFd = -1;
		return TCPERR_BIND;
	}
	return TCPERR_SUCCESS;
}
int reciveData(IpInfo *info)
{
	ssize_t recvRet;
	struct sockaddr_in addr;
	char buf[1024];
	in_addr_t addr;
	struct sockaddr_in srvAddr;

	addr = inet_addr(info->deatIp);
	if(INADDR_NONE == addr)
		return TCPERR_INVADDR;
	bzero(&srvAddr, sizeof(srvAddr));
	srvAddr.sin_port = htons(info->port);
	srvAddr.sin_addr.s_addr = addr;


	recvRet = recvfrom(info->sockFd, buf, sizeof(buf), 0.
		      ()	&addr, sizeof(addr));	
	if(-1 == recvRet)
		return -1;
	fprintf(stdout, "[%s] say:%s\n", addr, buf );
	return 0;
}
int sendData(IpInfo *info)
{
	ssize_t sendRet;
	char buf[1024];
	char *pTmp;
	in_addr_t addr;
	struct sockaddr_in srvAddr;

	addr = inet_addr(info->deatIp);
	if(INADDR_NONE == addr)
		return TCPERR_INVADDR;
	bzero(&srvAddr, sizeof(srvAddr));
	srvAddr.sin_port = htons(info->port);
	srvAddr.sin_addr.s_addr = addr;

	bzero(buf, sizeof(buf));
	fgets(buf, sizeof(buf) - 1, stdin);
	pTmp = strrchr(data->data, '\n');
	if(NULL != pTmp)
		*pTmp = '\0';
	sendRet = sendto(info->sockFd, buf, sizeof(buf), 0, 
		  	(struct sockaddr*)info->destIp, 
			sizeof(info->destIp));
	if(-1 == sendRet)
		return -1;
	return sendRet;
}

int main(int argc, char *argv[])
{
	IpInfo info;
	char *pTmp;
	int ret;
	int pid;

	if(argc != 3)
	{
		fprintf(stderr,	"Usage:%s <ip> <port>\n",
				argv[0]);
		return EXIT_FAILURE;
	}
	info.destIp = argv[1];
	info.port = strtol(argv[2], &pTmp, 10);
	if('\0' != *pTmp)
	{
		fprintf(stderr, "Invalid port %s\n", argv[1]);
		return EXIT_FAILURE;
	}
	while(1)
	{
		pid = fork();
		if(-1 == pid) goto Error;	//创建进程失败，退出程序
		if(0 == pid)//子进程中
		{
			fprinf(stderr, "I say:");
			if(0 != reciveData(&info))
				goto Error;
			continue;	//继续接受
		}
		while(1)
		{
			if(0 != sendData(&info))
				goto Error;
		}
	}
	return EXIT_SUCCESS;
Error:
	perror("client");
	destroyData(&data);
	switch(ret)
	{
		case TCPERR_INVADDR:
			//do sth
			break;
	}
	return ret;
}


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

#define IP_GRADE_A	0xFFFFFF00
#define IP_GRADE_B	0xFFFF0000
#define IP_GRADE_C	0xFF000000
#define IP_GRADE_O	0XFFFFFFFF
#define IP_GRADE_N	0	//错误IP

#define UDPERR_SUCCESS	0
#define UDPERR_INVADDR	1
#define UDPERR_CRESOCK	2
#define UDPERR_BIND		3
#define UDPERR_RECIVE	4
#define UDPERR_SEND		5
#define UDPERR_TIMEOUT	6
#define UDPERR_SELECT	7
#define SOCKERR_SETOPT	8

#define SYSERR_FORK		9
#define PROGCMD_QUIT	10

#define QUIT_CMD		"quit"

typedef struct{
	const char *ipAddr; //ip 点分制
	uint16_t port;		//port 无符号短整型
	int sockFd;			//文件描述符,不要修改
}IpInfo;

int ipGrade(in_addr_t addr)
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
in_addr_t broadcastAddr(in_addr_t addr)
{
	if(INADDR_ANY == addr)
		return 0x00000000;
	return addr | ipGrade(addr);
}

int creatUdpBind(IpInfo *info)
{
	in_addr_t addr;
	struct sockaddr_in srvAddr;

	//step 1.check ip addr
	addr = inet_addr(info->ipAddr);
	if(INADDR_NONE == addr)
		return UDPERR_INVADDR;
	//step 2.creat socket file
	info->sockFd = socket(PF_INET,
			SOCK_DGRAM, 0);
	if(-1 == info->sockFd)
		return UDPERR_CRESOCK;
	//step 3.set srvAddr struct
	bzero(&srvAddr, sizeof(srvAddr));
	srvAddr.sin_family = PF_INET;
	srvAddr.sin_port = htons(info->port);
	srvAddr.sin_addr.s_addr = addr;
	//step 4.bind socket infos
	if(-1 == bind(info->sockFd, 
			(struct sockaddr *)&srvAddr,
			sizeof(srvAddr)))
	{
		close(info->sockFd);
		info->sockFd = -1;
		return UDPERR_BIND;
	}
	return UDPERR_SUCCESS;
}
int sendData(const IpInfo *info)
{
	int ret;
	struct sockaddr_in addr;
	char buf[BUFFER_SIZE];

	bzero(buf, sizeof(buf));
	fgets(buf, sizeof(buf) - 1, stdin);
	if(0 == strncasecmp(buf, QUIT_CMD, strlen(QUIT_CMD)))
		return PROGCMD_QUIT;
	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(info->port);
	addr.sin_addr.s_addr = broadcastAddr(
				inet_addr(info->ipAddr));
	ret = sendto(info->sockFd, buf, strlen(buf), 0,
				(struct sockaddr *)&addr, sizeof(addr));
	if(-1 == ret)
		return UDPERR_SEND;
	return UDPERR_SUCCESS;
}
int recvData(IpInfo *info)
{
	int ret;
	fd_set rfds;
	struct timeval tv;
	struct sockaddr_in addr;
	socklen_t sockLen;
	char buf[BUFFER_SIZE];

	tv.tv_sec = 1;
	tv.tv_usec = 0;

	FD_ZERO(&rfds);
	FD_SET(info->sockFd, &rfds);
	ret = select(info->sockFd + 1, &rfds, NULL, NULL, &tv);
	if(-1 == ret) return UDPERR_SELECT;
	if(0 == ret) return UDPERR_TIMEOUT;
	bzero(&addr, sizeof(addr));
	sockLen = sizeof(addr);
	bzero(buf, sizeof(buf));
	ret = recvfrom(info->sockFd, buf, sizeof(buf) - 1, 0,
					(struct sockaddr *)&addr, &sockLen);
	if(-1 == ret) return UDPERR_RECIVE;
	fprintf(stdout, "%s:%d say: %s", inet_ntoa(addr.sin_addr), 
					ntohs(addr.sin_port), buf);
	return UDPERR_SUCCESS;
}
static int isQuited;
void childIntProcess(int signum)
{
	isQuited = PROGCMD_QUIT;
}
int main(int argc, char *argv[])
{
	IpInfo info;
	char *pTmp;
	int ret = 0;
	pid_t pid;
	int tmp = 1;

	if(argc != 3)
	{
		fprintf(stderr,	"Usage:%s <ip> <port>\n",
				argv[0]);
		return EXIT_FAILURE;
	}
	info.ipAddr = argv[1];
	info.port = strtol(argv[2], &pTmp, 10);
	if('\0' != *pTmp)
	{
		fprintf(stderr, "Invalid port %s\n", argv[1]);
		return EXIT_FAILURE;
	}
	ret = creatUdpBind(&info);
	if(UDPERR_SUCCESS != ret)
		goto Error;
	ret = setsockopt(info.sockFd, SOL_SOCKET, SO_BROADCAST, &tmp, sizeof(tmp));
	if(-1 == ret)
	{
		ret = SOCKERR_SETOPT;
		goto Error;
	}
	pid = fork();
	if(-1 == pid)
	{
		ret = SYSERR_FORK;
		goto Error;
	}else if(0 == pid)
	{//in child process:recive data from socket and print
		signal(SIGINT, childIntProcess);
		while(PROGCMD_QUIT != isQuited)
		{
			ret = recvData(&info);
			if(UDPERR_TIMEOUT == ret) continue;
			if(UDPERR_RECIVE == ret)
				perror("Warning : Recive Data Error!\n");
		}
		close(info.sockFd);
		return EXIT_SUCCESS;
	}else
	{//in parent process:get data from stdin and send to socket
		while(1)
		{
			ret = sendData(&info);
			if(ret == PROGCMD_QUIT)
			{
				kill(pid, SIGINT);
				close(info.sockFd);
				return EXIT_SUCCESS;
			}else if(ret != UDPERR_SUCCESS)
			{
				goto Error;
			}
		}
	}
	
	return EXIT_SUCCESS;
Error:
	perror("Error:");
	return EXIT_FAILURE;
}

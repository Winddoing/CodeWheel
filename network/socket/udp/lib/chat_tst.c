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

#define BUFFER_SIZE 1024

#define SYSERR_FORK		9
#define PROGCMD_QUIT	10

#define QUIT_CMD		"quit"

static int isQuited; 
void childIntProcess(int signum)
{
	isQuited = PROGCMD_QUIT;
}
int main(int argc, char *argv[])
{
	netInfo info;
	char *pTmp;
	int ret = 0;
	pid_t pid;
	int tmp = 1;

	if(argc != 3)
	{
		fprintf(stderr,	"Usage:%s <ip> <port>\n",
				argv[0]);
		fprintf(stderr, "local test:./udpchat 127.0.0.1 2222\n");
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
			char rbuf[BUFFER_SIZE];    
			bzero(rbuf, sizeof(rbuf));  
			ret = recvData(&info, rbuf, sizeof(rbuf) - 1);
			if(UDPERR_TIMEOUT == ret) continue;
			if(UDPERR_RECIVE == ret)
				perror("Warning : Recive Data Error!\n");
			fprintf(stdout, "xxxxxx say: %s", rbuf);              

		}
		close(info.sockFd);
		return EXIT_SUCCESS;
	}else
	{//in parent process:get data from stdin and send to socket
		while(1)
		{
			char sbuf[BUFFER_SIZE]; 
			bzero(sbuf, sizeof(sbuf));    
			fgets(sbuf, sizeof(sbuf) - 1, stdin);                       
			if(0 == strncasecmp(sbuf, QUIT_CMD, strlen(QUIT_CMD)))     
				    return PROGCMD_QUIT;                                  

			ret = sendData(&info, sbuf, strlen(sbuf));
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

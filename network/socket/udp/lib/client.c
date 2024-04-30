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

#define SYSERR_FORK		9

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
	if('\0' != *pTmp) {
		fprintf(stderr, "Invalid port %s\n", argv[1]);
		return EXIT_FAILURE;
	}

	ret = creatUdpClient(&info);
	if(UDPERR_SUCCESS != ret)
		goto Error;

	ret = setsockopt(info.sockFd, SOL_SOCKET, SO_BROADCAST, &tmp, sizeof(tmp));
	if(-1 == ret) {
		ret = SOCKERR_SETOPT;
		goto Error;
	}

	pid = fork();
	if (-1 == pid) {
		ret = SYSERR_FORK;
		goto Error;
	} else if (0 == pid) {//in child process:recive data from socket and print
		while(1) {
			char *sbuf = "123456789wqshao";                                
			ret = sendData(&info, sbuf, sizeof(sbuf));                     
			if(ret != UDPERR_SUCCESS) {                                    
				    goto Error;                                                
			}                                                              
			fprintf(stdout, "send: %s, len: %ld\n", sbuf, sizeof(sbuf));   
		}
		close(info.sockFd);
		return EXIT_SUCCESS;
	}

	while(1);
	return EXIT_SUCCESS;
Error:
	perror("Error:");
	return EXIT_FAILURE;
}

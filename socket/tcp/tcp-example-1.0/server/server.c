#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>

#include <tcp.h>
#include <shell.h>

int main(int argc, char *argv[])
{
	IpInfo info, clientInfo;
	char *pTmp;
	int ret;
	pid_t pid;
	Data data;

	data.data = NULL;

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
	if(ERR_SUCCESS != (ret = initData(&data)))
		goto Error;
	info.backlog = DEF_BACKLOG;
	info.domain = REMOT_SOCKET;
	ret = creatTcpListen(&info);
	if(ret != TCPERR_SUCCESS)
		goto Error;
	while(1)
	{
		ret = acceptConnect(&info, &clientInfo);
		if(-1 == ret)
		{
			perror("AcceptConnect");
			continue;
		}
		fprintf(stdout, "Client[%s:%d] is connected!\n",
					clientInfo.ipAddr, clientInfo.port);
		pid = fork();
		if(-1 == pid)
			goto Error;
		if(0 == pid)
		{
			close(clientInfo.sockFd); //关闭客户端连接
			continue;  //继续接受下一个客户端连接
		}
		/* 父进程处理当前客户端通信 */
		close(info.sockFd);
		while(1)
		{
			ret = analyData(&clientInfo, &data);
			if(TCPERR_DISCONNECT == ret)
			{
				fprintf(stdout, "Client[%s:%d] is disconnected!\n",
					clientInfo.ipAddr, clientInfo.port);
				shutdown(clientInfo.sockFd, SHUT_RDWR);
				goto ClientDisconnect;
			}else if(0 != ret)
				goto Error;
		}
	}
ClientDisconnect:
	destroyData(&data);
	return EXIT_SUCCESS;
Error:
	if(data.data != NULL)
		destroyData(&data);
	switch(ret)
	{
		case TCPERR_INVADDR:
			fprintf(stderr, "Invalid ip addr.");
			break;
//		case TCPERR_
	}
	return ret;
}


#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include <tcp.h>
#include <shell.h>

int main(int argc, char *argv[])
{
	IpInfo info, clientInfo;
	char *pTmp;
	int ret;
	Data data;

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
	info.backlog = DEF_BACKLOG;
	info.domain = REMOT_SOCKET;
	ret = creatTcpListen(&info);
	if(ret != TCPERR_SUCCESS)
		goto Error;
	data.maxLength = 1024; //////////
	data.data = (char *)malloc(data.maxLength);
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
		while(1)
		{
			ret = analyData(&clientInfo, &data);
			if(TCPERR_DISCONNECT == ret)
			{
				fprintf(stdout, "Client[%s:%d] is disconnected!\n",
					clientInfo.ipAddr, clientInfo.port);
				break;
			}else if(0 != ret)
				goto Error;
		}
	}
	free(data.data);
	return EXIT_SUCCESS;
Error:
	switch(ret)
	{
		case TCPERR_INVADDR:
			//do sth
			break;
	}
	return ret;
}


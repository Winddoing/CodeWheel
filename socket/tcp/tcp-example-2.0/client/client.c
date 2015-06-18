#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <tcp.h>
#include <shell.h>

int main(int argc, char *argv[])
{
	IpInfo info;
	Data data;
	char *pTmp;
	int ret;

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
	info.domain = REMOT_SOCKET;
	ret = connectToHost(&info);
	if(ret != TCPERR_SUCCESS)
		goto Error;
	data.maxLength = 1024;//////////
	data.data = (char *)malloc(data.maxLength);
	while(1)
	{
		//1.向服务器获取示符
		//打印
		ret = recivePrompt(&info, &data);
		if(0 != ret)
			break;
		fprintf(stderr, "%s", data.data);
		//2.向服务器获取一条命令的运行结果
		//打印
		ret = reciveExecResult(&info, &data);
		if(TYPE_QUITCMD == ret)
		{
			ret = 0;
			break;
		}else if(ret != TCPERR_SUCCESS){
			break;		
		}
		fprintf(stderr, "%s", data.data);
	}
	if(ret != 0) goto Error;
	free(data.data);
	return EXIT_SUCCESS;
Error:
	perror("client");
	switch(ret)
	{
		case TCPERR_INVADDR:
			//do sth
			break;
	}
	return ret;
}


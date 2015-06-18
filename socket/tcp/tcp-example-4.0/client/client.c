#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include <tcp.h>
#include <shell.h>

int main(int argc, char *argv[])
{
	IpInfo info;
	char *pTmp;
	int ret;
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
	info.domain = REMOT_SOCKET;
	ret = connectToHost(&info);
	if(ret != TCPERR_SUCCESS)
		goto Error;
	while(1)
	{
		//1.向服务器获取提示符
		ret = recivePrompt(&info, &data);
		if(ret != TCPERR_SUCCESS)
			break;
		//打印
		fprintf(stderr, data.data);
		//2.向服务器获取一条命令的运行结果
		ret = reciveExecResult(&info, &data);
		if(TYPE_QUITCMD == ret)
		{
			ret = 0;
			break;
		}else if(ret != TCPERR_SUCCESS)
			break;
		//打印
		fprintf(stderr, data.data);
	}
	if(ret != 0) goto Error;
	destroyData(&data);
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


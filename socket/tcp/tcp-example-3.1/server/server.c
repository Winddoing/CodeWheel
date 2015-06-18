#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

#include <tcp.h>
#include <shell.h>
#include <readconf.h>

#define MAX_CLIENT	10

int main(int argc, char *argv[])
{
	IpInfo info, clientInfo;
	IpInfo *infos[MAX_CLIENT];
	char *pTmp;
	int ret = 0;
	Data data;
	char rootPath[128];
	char logName[128];
	int curClientNumber = 1;


	data.data = NULL;
	bzero(infos, sizeof(infos));

	if(argc != 3)
	{
		fprintf(stderr,	"Usage:%s <ip> <port>\n",
				argv[0]);
		return EXIT_FAILURE;
	}
//读取配置文件
	if(0 != getSetupItem(CONF_FILE, ITEM_ROOT, rootPath))
		goto Error;
	if(0 != getSetupItem(CONF_FILE, ITEM_LOGFILE, logName))
		goto Error;

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
//创建守护进程
	if(0 != creatDaemon(rootPath))
		goto Error;
	ret = creatTcpListen(&info);
	if(ret != TCPERR_SUCCESS)
		goto Error;

	while(1)
	{
		//限制最大客户端连接数，如果客户端数量达到最大连接数时，
		//将用于listen的套接定从监听数组中去除。即使有客户端进行
		//连接时，select监听队列也不会进行响应。
		if(curClientNumber < MAX_CLIENT)
		{
			//将这个listen文件描述符也存入这个数组中
			infos[info.sockFd] = &info;
		}else{
			infos[info.sockFd] = NULL;
		}
		//启用select监听，找到响应的文件描述符
		ret = activateSocket(infos, MAX_CLIENT);
		if(-1 == ret) goto Error;
		//测试这个文件描述符是listen用途还是client通信用途
		if(ret == info.sockFd)//用于监听的文件描述符响应的处理
		{
			//接受客户端连接
			ret = acceptConnect(&info, &clientInfo);
			if(-1 == ret)
			{
				perror("AcceptConnect");
				continue;
			}
			//将这个用户IpInfo结构指针添加到数组中
			infos[clientInfo.sockFd] = (IpInfo *)malloc(
							sizeof(IpInfo));
			if(NULL == infos[clientInfo.sockFd])
			{
				perror("no memory!");
				close(clientInfo.sockFd);
				continue;
			}
			curClientNumber++;	//当前客户端数量加一 
			*(infos[clientInfo.sockFd]) = clientInfo;
			//写日志文件
			appendLog(logName, clientInfo.ipAddr, 
					clientInfo.port, STAT_CONNECTED);
		}else{//客户端数接收
			//如果在数据收发时出现客户端断开的情况，一定要将
			clientInfo = *(infos[ret]); //每个客户端的信息
			ret = analyData(&clientInfo, &data);
			if(TCPERR_DISCONNECT == ret)
			{
				appendLog(logName, clientInfo.ipAddr, 
							clientInfo.port, STAT_DISCONNECT);
				free(infos[clientInfo.sockFd]);
				infos[clientInfo.sockFd] = NULL;
				shutdown(clientInfo.sockFd, SHUT_RDWR);
				curClientNumber--;
			}else if(0 != ret)
			{
				goto Error;
			}
		}
	}
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


#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

#include <tcp.h>
#include <shell.h>
#include <readconf.h>

#define MAX_CLIENT 10

int main(int argc, char *argv[])
{
	IpInfo info, clientInfo;
	IpInfo *infos[MAX_CLIENT];
	char *pTmp;
	int ret = 0;
	Data data;
	char rootPath[128];
	char logName[128];

	data.data = NULL;
	bzero(infos, sizeof(infos));///////////////
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
//将linten文件描述符存入数组
	infos[info.sockFd] = &info;

	while(1)
	{
		//启用select监听，找到响应文件描述符
		ret = activateSocket(infos, MAX_CLIENT);
		if(-1 == ret) goto Error;
		//测试这个文件描述符是listen用途还是client通信用途
		if(ret == info.sockFd) //用于监听文件描述符响应的处理
		{
			// 接受客户端连接
			ret = acceptConnect(&info, &clientInfo);
			if(-1 == ret)
			{
				perror("AcceptConnect");
				continue;
			}
			appendLog(logName, clientInfo.ipAddr, 
					clientInfo.port, STAT_CONNECTED);
			//将这个用户的IpInfo结构指针添加到数组中
			infos[info.sockFd] = &clientInfo;
			continue;
			
		}else
		{//客户端数据接受
			ret = analyData(&clientInfo, &data);
			if(TCPERR_DISCONNECT == ret)
			{
				appendLog(logName, clientInfo.ipAddr, 
							clientInfo.port, STAT_DISCONNECT);
				shutdown(clientInfo.sockFd, SHUT_RDWR);
				break;
			}else if(0 != ret)
				goto Error;
		}
			//如果在数据收发是出现客户端断开情况，一定要将
			//这个用户IpInfo结构指针从数组中删除
	}
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


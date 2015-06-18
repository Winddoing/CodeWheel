#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>

#include <tcp.h>

int writeData(const IpInfo *info, const Data *data)
{
	ssize_t ret;
	size_t wrote = 0;
	// 1. send data length
	ret = write(info->sockFd, &(data->dataLength), 
					sizeof(data->dataLength));
	if(ret <= 0) goto ErrorWriteData;
	// 2. send data type
	ret = write(info->sockFd, &(data->type),
					sizeof(data->type));
	if(ret <= 0) goto ErrorWriteData;
	// 3. send data
	while(wrote < data->dataLength){
		ret = write(info->sockFd, data->data + wrote,
						data->dataLength - wrote);
		if(ret <= 0) goto ErrorWriteData;
		wrote += ret;
	}
	return TCPERR_SUCCESS;
ErrorWriteData:
	if(0 == ret) return TCPERR_DISCONNECT;
	return TCPERR_WRITE;
}
int readData(const IpInfo *info, Data *data)
{
	ssize_t ret;
	size_t readed = 0;
	
	// 1. recive data length
	ret = read(info->sockFd, &(data->dataLength), 
					sizeof(data->dataLength));
	if(ret <= 0)
		goto ErrorReadData;
	// 2. recive data type
	ret = read(info->sockFd, &(data->type),
					sizeof(data->type));
	if(ret <= 0) goto ErrorReadData;
	// 3. recive data
	while(readed < data->dataLength)
	{
		ret = read(info->sockFd, data->data + readed,
						data->dataLength - readed);
		if(ret <= 0) goto ErrorReadData;
		readed += ret;
	}
	return TCPERR_SUCCESS;
ErrorReadData:
	if(0 == ret) return TCPERR_DISCONNECT;
	return TCPERR_READ;
}
int acceptConnect(const IpInfo *listenInfo, IpInfo *clientInfo)
{
	struct sockaddr_in clientAddr;
	socklen_t sockLen;
	
	/* 1. init clientAddr and sockLen */
	sockLen = sizeof(clientAddr);
	bzero(&clientAddr, sockLen);

	/* 2. call accept function accept client connect */;
	clientInfo->sockFd = accept(listenInfo->sockFd,
				(struct sockaddr *)&clientAddr, &sockLen);
	if(-1 == clientInfo->sockFd)
		return TCPERR_ACCEPT;
	/* 3. set clientInfo member */
	clientInfo->ipAddr = inet_ntoa(clientAddr.sin_addr);
	clientInfo->port = ntohs(clientAddr.sin_port);

	return TCPERR_SUCCESS;
}
int connectToHost(IpInfo *info)
{	
	in_addr_t addr;
	struct sockaddr_in srvAddr;

	//step 1.check ip addr
	addr = inet_addr(info->ipAddr);
	if(INADDR_NONE == addr)
		return TCPERR_INVADDR;
	//step 2.creat socket file
	info->sockFd = socket(info->domain,
			SOCK_STREAM, 0);
	if(-1 == info->sockFd)
		return TCPERR_CRESOCK;
	//step 3.set srvAddr struct
	bzero(&srvAddr, sizeof(srvAddr));
	srvAddr.sin_family = info->domain;
	srvAddr.sin_port = htons(info->port);
	srvAddr.sin_addr.s_addr = addr;
	//step 4.bind socket infos
	if(-1 == connect(info->sockFd, 
			(struct sockaddr *)&srvAddr,
			sizeof(srvAddr)))
	{
		close(info->sockFd);
		info->sockFd = -1;
		return TCPERR_CONNECT;
	}
	return TCPERR_SUCCESS;
}

int creatTcpListen(IpInfo *info)
{
	in_addr_t addr;
	struct sockaddr_in srvAddr;

	//step 1.check ip addr
	addr = inet_addr(info->ipAddr);
	if(INADDR_NONE == addr)
		return TCPERR_INVADDR;
	//step 2.creat socket file
	info->sockFd = socket(info->domain,
			SOCK_STREAM, 0);
	if(-1 == info->sockFd)
		return TCPERR_CRESOCK;
	//step 3.set srvAddr struct
	bzero(&srvAddr, sizeof(srvAddr));
	srvAddr.sin_family = info->domain;
	srvAddr.sin_port = htons(info->port);
	srvAddr.sin_addr.s_addr = addr;
	//step 4.bind socket infos
	if(-1 == bind(info->sockFd, 
			(struct sockaddr *)&srvAddr,
			sizeof(srvAddr)))
	{
		close(info->sockFd);
		info->sockFd = -1;
		return TCPERR_BIND;
	}
	//step 5.begin listen
	if(-1 == listen(info->sockFd,
			info->backlog))
	{
		close(info->sockFd);
		info->sockFd = -1;
		return TCPERR_LISTEN;
	}
	return TCPERR_SUCCESS;
}


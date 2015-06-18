#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>

#include <tcp.h>

/* ****************************************
 *函数名： activateSocket                 *
 *功  能： 关联多个socket文件描述符，并找 *
 *         到响应的socket文件描述符       *
 *参  数： IP节点结构指针的指针           *
 *         最大的客户端数量               *
 *返回值：失败返回－1                     *
 *		 成功返回下标即文件描述符         *
 **************************************** */
int activateSocket(IpInfo **infos, int maxClient)
{
	int i;
	int ret;
	fd_set rfds;

	FD_ZERO(&rfds);	//清
	for(i = 0; i < maxClient; i++)//关联
	{
		if(infos[i] != NULL)
			FD_SET(i, &rfds);
	}
	ret = select(maxClient, &rfds, NULL, NULL, NULL);
	if(-1 == ret) return -1;
	for(i = 0; i < maxClient; i++)
	{
		if(infos[i] != NULL && FD_ISSET(i, &rfds))
			return i;
	}
	return -1;
}
/* ****************************************
 *函数名：initData                        *
 *功  能：初始化自定义协议数据            *
 *参  数：自定义协议结构体指针            *
 *返回值： 失败  错误号                   *
 *         成功    0                      *
 **************************************** */
int initData(Data *data)
{
	data->maxLength = DEF_MAX_LENG;
	data->data = (char *)malloc(data->maxLength);
	if(NULL == data->data)
		return ERR_NOMEM;
	return ERR_SUCCESS;
}
/* ****************************************
 *函数名： destroyData                    *
 *功  能： 释放自定义协议结构体数         *
 *         据的内存空间                   *
 *参  数： 自定义协议结构体指针           *
 *返回值： 无                             *
 **************************************** */
void destroyData(Data *data)
{
	if(NULL != data->data)
	{
		free(data->data);
		data->data = NULL;
	}
}
/* ****************************************
 *函数名： writeData                      *
 *功  能： 写数据，发送出数据             *
 *参  数： IP节点信息，发送数据           *
 *返回值： 失败  错误号                   *
 *         成功    0                      *
 **************************************** */
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
/* ****************************************
 *函数名： readData                       *
 *功  能： 读数据，接收数据               *
 *参  数： IP节点信息，发送数据      
 *返回值： 失败  错误号                   *
 *         成功    0                      *
 **************************************** */
int readData(const IpInfo *info, Data *data)
{
	ssize_t ret;
	size_t readed = 0;
	
	// 1. recive data length
	ret = read(info->sockFd, &(data->dataLength), 
					sizeof(data->dataLength));
	if(ret <= 0) goto ErrorReadData;
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
/* ****************************************
 *函数名：acceptConnect                   *
 *功  能：请求服务器连接客户端            *
 *参  数：服务器监听到的IP节点信息        *
 *        客户端的IP节点信息              *
 *返回值：失败  错误号                    *
 *        成功    0                       *
 **************************************** */
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
/* ****************************************
 *函数名：  connectToHost                 *
 *功  能：  连接到主机                    *
 *参  数：  IP节点信息                    *
 *返回值： 失败  错误号                   *
 *         成功    0                      *
 **************************************** */
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
/* ****************************************
 *函数名： creatTcpListen                 *
 *功  能： 创建监听                       *
 *参  数： IP节点信息                     *
 *返回值： 失败  错误号                   *
 *         成功    0                      *
 **************************************** */
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


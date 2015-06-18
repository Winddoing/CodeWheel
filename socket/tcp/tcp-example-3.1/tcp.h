#ifndef TCP_H
#define TCP_H

#include <stdint.h>
#include <sys/socket.h>

#define DEF_BACKLOG	5   //等待队列的长度

/* IP节点信息 */
typedef struct{
	const char *ipAddr; //ip 点分制
#define LOCAL_SOCKET PF_LOCAL	//用于本地通信
#define REMOT_SOCKET PF_INET	//用于远程通信
	int domain;			//网络通信类型,本机或远程
	uint16_t port;		//port 无符号短整型
	uint16_t backlog;	//等待队列长度
	int sockFd;			//文件描述符,不要修改
}IpInfo;

/* 自定义的数据传输协议 */
#define DEF_MAX_LENG	4096 //命令行输入字符的最大长度
typedef struct{
	uint16_t maxLength;    //数据的最大长度
	uint16_t dataLength;   //实际传输的数据大小
	uint16_t type;         //传输数据的类型
	char * data;           //传输的数据
}Data;

#define ERR_NOMEM		10
#define ERR_SUCCESS		0
/* ****************************************
 *函数名：initData                        *
 *功  能：初始化自定义协议数据            *
 *参  数：自定义协议结构体指针            *
 *返回值： 失败  错误号                   *
 *         成功    0                      *
 **************************************** */
int initData(Data *data);
/* ****************************************
 *函数名： destroyData                    *
 *功  能： 释放自定义协议结构体数         *
 *         据的内存空间                   *
 *参  数： 自定义协议结构体指针           *
 *返回值： 无                             *
 **************************************** */
void destroyData(Data *data);

#define TCPERR_SUCCESS	0
#define TCPERR_CRESOCK	1
#define TCPERR_INVADDR	2
#define TCPERR_BIND		3
#define TCPERR_LISTEN	4
#define TCPERR_CONNECT	5
#define TCPERR_ACCEPT	6
#define TCPERR_WRITE	7
#define TCPERR_READ 	8
#define TCPERR_DISCONNECT	9

#define STAT_CONNECTED 0
#define STAT_DISCONNECT 1

/* ****************************************
 *函数名： writeData                      *
 *功  能： 写数据，发送出数据             *
 *参  数： IP节点信息，发送数据           *
 *返回值： 失败  错误号                   *
 *         成功    0                      *
 **************************************** */
int writeData(const IpInfo *info, const Data *data);
/* ****************************************
 *函数名： readData                       *
 *功  能： 读数据，接收数据               *
 *参  数： IP节点信息，发送数据      
 *返回值： 失败  错误号                   *
 *         成功    0                      *
 **************************************** */
int readData(const IpInfo *info, Data *data);
/* ****************************************
 *函数名： creatTcpListen                 *
 *功  能： 创建监听                       *
 *参  数： IP节点信息                     *
 *返回值： 失败  错误号                   *
 *         成功    0                      *
 **************************************** */
int creatTcpListen(IpInfo *info);
/* ****************************************
 *函数名：  connectToHost                 *
 *功  能：  连接到主机                    *
 *参  数：  IP节点信息                    *
 *返回值： 失败  错误号                   *
 *         成功    0                      *
 **************************************** */
int connectToHost(IpInfo *info);
/* ****************************************
 *函数名： activateSocket                 *
 *功  能： 关联多个socket文件描述符，并找 *
 *         到响应的socket文件描述符       *
 *参  数： IP节点结构指针的指针           *
 *         最大的客户端数量               *
 *返回值：失败返回－1                     *
 *		 成功返回下标即文件描述符         *
 **************************************** */
int activateSocket(IpInfo **infos, int maxClient);
/* ****************************************
 *函数名：acceptConnect                   *
 *功  能：请求服务器连接客户端            *
 *参  数：服务器监听到的IP节点信息        *
 *        客户端的IP节点信息              *
 *返回值：失败  错误号                    *
 *        成功    0                       *
 **************************************** */
int acceptConnect(const IpInfo *listenInfo, IpInfo *clientInfo);
#endif //TCP


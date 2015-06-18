#ifndef SHELL_H
#define SHELL_H

#include <tcp.h>

#define TYPE_REQPROMPT	'P'
#define TYPE_REQEXERES	'C'
#define TYPE_SNDPROMPT	'p'
#define TYPE_SNDEXERES	'c'
#define TYPE_QUITCMD	'q'

#define TYPE_ERRCMD		('z' + 1)
#define SHELL_ERRRUN	('z' + 2)
#define QUIT_CMD		"QUIT"
/* run in client */
// recive command prompt from server
/* ****************************************
 *函数名： recivePrompt                   *
 *功  能： 得到接收服务器传来的提示符     *
 *参  数： 客户端IP节点                   *
 *         传送数据                       *
 *返回值：失败   错误号                   *
 *		  成功     0                      *
 **************************************** */
int recivePrompt(const IpInfo *info, Data *data);
// recive command exec result from server
/* ****************************************
 *函数名： reciveExecResult               *
 *功  能： 得到接收服务器传来的命令       *
 *         执行结果                       *
 *参  数： 客户端IP节点                   *
 *         传送数据                       *
 *返回值：失败   错误号                   *
 *		  成功     0                      *
 **************************************** */
int reciveExecResult(const IpInfo *info, Data *data);

/* run in server */
/* ****************************************
 *函数名：  analyData                     *
 *功  能：  服务器解析接受到的数据是为获取*
 *          提示符，还是命令              *
 *参  数：  客户端IP节点                  *
 *          传送数据                      *
 *返回值：失败   错误号                   *
 *		  成功     0                      *
 **************************************** */
int analyData(const IpInfo *info, Data *data);
// send command prompt to client
/* ****************************************
 *函数名： sendPrompt                     *
 *功  能： 给客户端发送提示符             *
 *参  数： 客户端IP节点                   *
 *         传送数据                       *
 *返回值：失败   错误号                   *
 *		  成功     0                      *
 **************************************** */
int sendPrompt(const IpInfo *info, Data *data);
// send command exec result to client
/* ****************************************
 *函数名： sendExecResult                 *
 *功  能： 给客户端发送命令执行结果       *
 *参  数： 客户端IP节点                   *
 *         传送数据                       *
 *返回值：失败   错误号                   *
 *		  成功     0                      *
 **************************************** */
int sendExecResult(const IpInfo *info, Data *data);

#endif //SHELL_H


#ifndef READCONF_H
#define READCONF_H

#define CONF_FILE "/usr/etc/rmshell.conf"

#define ITEM_ROOT		"root"     //服务器工作路径变量
#define ITEM_LOGFILE	"logfile"  //日志文件变量

/* ****************************************
 *函数名： creatDaemon                    *
 *功  能： 创建守护进程                   *
 *参  数： 进程工作路径                   *
 *返回值：失败     -1                     *
 *		  成功     0                      *
 **************************************** */
int creatDaemon(const char *workPath);

/* ****************************************
 *函数名： getSetupItem                   *
 *功  能： 从配置文件中读取，服务器工作路 *
 *         径和日志文件存放路径           *
 *参  数： 配置文件路径、想要获取的变量   *
 *         获取路径的存放位置的指针       *
 *返回值：失败     -1                     *
 *		  成功     0                      *
 **************************************** */
int getSetupItem(const char *confName, const char *itemName,
				char *value);
/* ****************************************
 *函数名： appendLog                      *
 *功  能： 将服务器的提示信息，时间、IP   *
 *          端口、连接状态写入日志文件    *
 *参  数： 日志文件路径、客户端IP节点     *
 *          端口，连接状态                *
 *返回值：失败     -1                     *
 *		  成功     0                      *
 **************************************** */
int appendLog(const char *logName, const char *ip, 
				unsigned short port, int stat);
#endif //READCONF

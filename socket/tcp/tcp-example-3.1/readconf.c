#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <tcp.h>
#include <sys/stat.h>
#include <sys/types.h>

/* ****************************************
 *函数名： creatDaemon                    *
 *功  能： 创建守护进程                   *
 *参  数： 进程工作路径                   *
 *返回值：失败     -1                     *
 *		  成功     0                      *
 **************************************** */
int creatDaemon(const char *workPath)
{
	pid_t pid;
	int i;
	//关闭所有可能文件
	for(i = 0; i < 65535; i++)
		close(i);
	setuid(0); //指定启动守护进程的用户
	umask(0);  //
	chdir(workPath);
	pid = fork();
	if(-1 == pid) return -1;
	if(pid != 0) exit(0);
	return 0;
}
/* ****************************************
 *函数名： getSetupItem                   *
 *功  能： 从配置文件中读取，服务器工作路 *
 *         径和日志文件存放路径           *
 *参  数： 配置文件名、想要获取的变量名   *
 *         获取路径的存放位置的指针       *
 *返回值：失败     -1                     *
 *		  成功     0                      *
 **************************************** */
int getSetupItem(const char *confName, const char *itemName,
				char *value)
{
	FILE *fp;
	char *p, *q;
	char buf[1024];

	fp = fopen(confName, "r");
	if(NULL == fp)
		return -1;
/* 在循环中每次从文件中读取一行，然后进行测试 */
	while(1)
	{
		bzero(buf, sizeof(buf));
		if( NULL == fgets(buf, sizeof(buf) - 1, fp))
			break;
		if('#' == buf[0]) continue;

		p = strchr(buf, '=');
		if(NULL == p)
			continue;
		*p = '\0';
		p++;
		q = strrchr(p, '\n');
		if(NULL != q)
			*q = '\0';
		if(0 == strcmp(buf, itemName))
		{
			strcpy(value, p);
			fclose(fp);
			return 0;
		}
	}
	fclose(fp);
	return -1;
}
/* ****************************************
 *函数名： appendLog                      *
 *功  能： 将服务器的提示信息，时间、IP   *
 *          端口、连接状态写入日志文件    *
 *参  数： 日志文件名、客户端IP节点     *
 *          端口，连接状态                *
 *返回值：失败     -1                     *
 *		  成功     0                      *
 **************************************** */
int appendLog(const char *logName, const char *ip, 
				unsigned short port, int stat)
{
	FILE *fp;
	struct tm *tm;
	time_t ctime;

	fp = fopen(logName, "a+");
	if(NULL == fp)
		return -1;

	ctime = time(NULL);	//取得1970年1月1日 00:00:00到当前的秒数
	tm = gmtime(&ctime); //取得系统时间,1900年开始

	fprintf(fp, "%04d/%02d/%02d %02d:%02d:%02d %s:%d %s\n",
					tm->tm_year + 1900, tm->tm_mon + 1, //月份为0~11
					tm->tm_mday, tm->tm_hour + 8,//中国为东8区
					tm->tm_min, tm->tm_sec, ip, port, 
					(stat == STAT_CONNECTED ? "connected" : 
					"disconnected"));
	fclose(fp);

	return 0;
}





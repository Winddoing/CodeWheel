#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <tcp.h>
#include <sys/stat.h>
#include <sys/types.h>

int creatDaemon(const char *workPath)
{
	pid_t pid;
	int i;

	for(i = 0; i < 65535; i++)
		close(i);
	setuid(0);
	umask(0);
	chdir(workPath);
	pid = fork();
	if(-1 == pid) return -1;
	if(pid != 0) exit(0);
	return 0;
}
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





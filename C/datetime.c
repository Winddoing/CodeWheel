#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[])
{
	struct tm *tm;
	time_t ctime;

	ctime = time(NULL);	//取得1970年1月1日 00:00:00到当前的秒数
	tm = gmtime(&ctime); //取得系统时间,1900年开始

	fprintf(stdout, "%04d/%02d/%02d %02d:%02d:%02d\n",
					tm->tm_year + 1900, tm->tm_mon + 1, //月份为0～11
					tm->tm_mday, tm->tm_hour + 8,//中国为东8区
					tm->tm_min, tm->tm_sec);

	return EXIT_SUCCESS;
}



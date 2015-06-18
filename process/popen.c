#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CMD  "date +\"%Y-%m-%d %H:%m:%S\""

int main(int argc, char *argv[])
{
	FILE *fp = NULL;  //变量的初始赋值多为出错处理
	//char *ptr;
	char buf[BUFSIZ + 1];
	int year, month, day, hour, minute, second;

	fp = popen(CMD, "r");
	if(NULL == fp) goto Error;
	bzero(buf, sizeof(buf));
    if(0 == fread(buf, sizeof(char), BUFSIZ, fp))  //不超过缓冲区大小实际就读一次
//	if(0 == fread(ptr, strlen(CMD), 1, fp)) //每次读的大小为所读数据的类型大小
		goto Error;
	pclose(fp);
	fp = NULL;  //指针使用完后赋值给NULL
	sscanf(buf, "%d-%d-%d %d:%d:%d",
				&year, &month, &day,
				&hour, &minute, &second);
	fprintf(stdout, "%04d年%02d月%02d日"
					"%02d时%02d分%02d秒\n",
					year, month, day,
					hour, minute, second);

	return EXIT_SUCCESS;
Error:
	if(NULL != fp)
	{
		pclose(fp);
		fp = NULL;
	}
	return EXIT_FAILURE;
}


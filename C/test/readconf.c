#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CONF_FILE "/usr/etc/rmshell.conf"
static struct {
	const char * itemName;
	const char * descript;
}items[] ={
	{ .itemName = "root",  //与配置文件中的配置项目名称对应
	  .descript = "工作目录", //关于配置项的说明
	},
	{
	  .itemName = "logfile",
	  .descript = "配置文件路径",
	}
};
#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

int main(int argc, char *argv[])
{
	FILE *fp;
	char *p, *q;
	char buf[1024];
	int i;
	
	fp = fopen(CONF_FILE, "r");
	if(NULL == fp)
	{
		perror("Cannot open");
		return EXIT_FAILURE;
	}
	/* 每次从文件中读取一行，然后测试 */
	while(1)
	{
		bzero(buf, sizeof(buf));
		if( NULL == fgets(buf, sizeof(buf) - 1, fp))
			break;
		if('#' == buf[0]) continue;

		p = strstr(buf, "=");
//		p = strchr(buf, '=');
		if(NULL == p)
			continue;
		*p = '\0';
		p++;
		q = strrchr(p, '\n');
		if(NULL != q)
			*q = '\0';
		for(i = 0; i < ARRAY_SIZE(items); i++)
		{
			if(0 == strcmp(buf, items[i].itemName))
				fprintf(stdout, "%s:%s\n", 
						items[i].descript, p);
		}
	}
	fclose(fp);
	return EXIT_SUCCESS;
}



/*#############################################################
 *     File Name	: cpu_hotplug_test.c
 *     Author		: wqshao
 *     Created Time	: 2017年10月18日 星期三 09时56分21秒
 *     Description	:
 *############################################################*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <memory.h>


struct cpu_op {
	int cpu;
	int online;
};

static const struct cpu_op switch_cpu_op[] = {
	{0, 0},
	{1, 0},
	{0, 1},
	{1, 1},
};


int main(int argc, const char *argv[])
{
	int fd;
	char file_name[80];
	char online[4];
	int flags = O_WRONLY | O_CREAT | O_TRUNC | O_CLOEXEC | O_NOFOLLOW;
	int i, ret;

	for(i = 0; i < sizeof(switch_cpu_op)/sizeof(struct cpu_op); i++) {
		int len;

		memset(file_name, 0, sizeof(file_name));
		memset(online, 0, sizeof(online));

		sprintf(file_name, "/sys/devices/system/cpu/cpu%d/online", switch_cpu_op[i].cpu);

		fd = open(file_name, flags, DEFFILEMODE);
		if (fd == -1) {
			printf("Open file fail. (%s)\n", file_name);
			return -1;                                                     
		}
		//itoa(online, switch_cpu_op[i].online, 1);
		sprintf(online, "%d", switch_cpu_op[i].online);
		len = strlen(online); 
		printf("Op CPU%d, online=%s\n", switch_cpu_op[i].cpu, online);

		ret = write(fd, online, len);
		if ( ret == -1) {                                                      
			printf("Write file fail. %d, ()\n", ret);
			continue;
			//return -1;                                                     
		}                                                                   
		printf("=======>>Switch CPU%d %s\n", switch_cpu_op[i].cpu, switch_cpu_op[i].online ? "on" : "off");

		close(fd);
	}


	return 0;
}

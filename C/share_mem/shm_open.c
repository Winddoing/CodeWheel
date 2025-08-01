/* gcc shm_open.c -lrt */
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#define CODERS_SHM_NAME "shm-wq-00"

int main(int argc,char * argv[])
{
	int flag = O_CREAT | O_EXCL | O_RDWR;
	const mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP;

	int shm_fd = shm_open(CODERS_SHM_NAME, flag, mode);
	while (shm_fd == -1) {
		if (errno == 17 /* EEXIST */) {
			printf("%s exists\n", CODERS_SHM_NAME);
			flag = O_RDWR;
			shm_fd = shm_open(CODERS_SHM_NAME, flag, mode);
		} else {
			printf("ERROR: %s(): shm_open failed for %s, errno=%d:%s\n",
					__func__, CODERS_SHM_NAME, errno, strerror(errno));
                    	printf("sudo rm /dev/shm/%s\n", CODERS_SHM_NAME);
			return -1;
		}
	}

	printf("%s: ok. shm_fd=%d, errno=%d\n", __func__, shm_fd, errno);

	close(shm_fd);
	shm_unlink(CODERS_SHM_NAME);

	return 0;
}


/* 麒麟系统权限问题 
 * kylin@kylin-pc:~$ ./a.out
 * shm-wq-00 exists
 * ERROR: main(): shm_open failed for shm-wq-00, errno=13:Permission denied
 *
 * 如果想要多次运行成功，不报权限不足错误，需要将shm文件系统重新remount一下
 *
 * sudo mount -o remount,rw /dev/shm 
 *
 * 根本原因可能与kylin系统开启audit有关
 * 与麒麟系统的kysec安全模块有关，需要将其关闭：setstatus disable
 *
 *
 * ubuntu24.04 当前测试case,无需如何操作即可正常运行
*/

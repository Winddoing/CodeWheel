#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <syslog.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>

#define CVT_TRACE   1
#define CVT_WARNING 2
#define CVT_ERROR   3

static int dbg_level = 3;
/*static int dbg_level = 1;*/
/*#define UIBC_DUMP_DATA*/

#if 0
#define cvt_dbg(level, msg, arg...) do { \
	if ((level) >= dbg_level) { \
            syslog(LOG_NOTICE, "[%s %d] "msg, __FILE__, __LINE__, ##arg);\
	} \
} while (0)
#else
#define cvt_dbg(level, msg, arg...) do { \
	if ((level) >= dbg_level) { \
            printf("[%s %d] "msg, __FILE__, __LINE__, ##arg);\
	} \
} while (0)
#endif

struct uibc_st{
	unsigned int hid_fd;
	unsigned int sock_fd;
	pthread_t thread_pid;
};

#define DWC_OTG_DRIVER_PATH "/lib/modules/dwc_otg.ko"
#define HID_DRIVER_PATH "/lib/modules/g_hid.ko"
#define HID_DEVICE_PATH "/dev/hidg0"

#define HID_DEV_BUF_SZIE  1500
#define SERVER_DEF_PORT 1111 

void load_hidg_driver()
{
	char cmd[256] = {0};
	sprintf(cmd, "insmod %s", DWC_OTG_DRIVER_PATH);
	system(cmd);
	memset(cmd, 0, 256);
	sprintf(cmd, "insmod %s", HID_DRIVER_PATH);
	system(cmd);
}

void remove_hidg_driver()
{
	char cmd[256] = {0};
	sprintf(cmd, "remod %s", HID_DRIVER_PATH);
	system(cmd);
	memset(cmd, 0, 256);
	sprintf(cmd, "remod %s", DWC_OTG_DRIVER_PATH);
	system(cmd);
}

int start_hidg(struct uibc_st *uibc)
{
	load_hidg_driver();

	sleep(1);/*wait driver ready*/

	cvt_dbg(CVT_TRACE, "uibc open device: %s\n", HID_DEVICE_PATH);
	uibc->hid_fd = open(HID_DEVICE_PATH, O_RDWR, 0666);
	if (uibc->hid_fd < 0){
		printf("open device error,\n");
		remove_hidg_driver();
		return -1;
	}

	return 0;
}

static void* uibc_thread(void *threadData)
{
	struct uibc_st *uibc =  (struct uibc_st*)threadData;
	char buf[HID_DEV_BUF_SZIE];
	struct sockaddr_in addr;
	socklen_t socklen = sizeof(addr);
	fd_set rs;
	int ret, len;

	cvt_dbg(CVT_TRACE, "%s enter.\n", __func__);

    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

	do {
		FD_ZERO(&rs);
		FD_SET(uibc->sock_fd, &rs);

		ret = select(uibc->sock_fd + 1, &rs, NULL, NULL, NULL);
		if (ret <= 0) {
			cvt_dbg(CVT_ERROR, "uibc select error! ret=%d\n", ret);
			continue;
		}

		printf("===> func: %s, line: %d\n", __func__, __LINE__);
		if (FD_ISSET(uibc->sock_fd, &rs)) {
			memset(buf, 0, sizeof(buf));
			bzero(&addr, sizeof(addr));
			cvt_dbg(CVT_TRACE,"ubic recvfrom data ...\n");
			len = recvfrom(uibc->sock_fd, buf, sizeof(buf) - 1, 0, (struct sockaddr*)&addr ,&socklen);
			if (len <= 0) {
				cvt_dbg(CVT_ERROR, "uibc socket read error, len=%d\n", len);
				continue;
			}
		}

#ifdef UIBC_DUMP_DATA
		{//debug
			int i = 0;

			printf("uibc read socket data[len: %d]: BIT%d~BIT0\n\t\t",
					len, len - 1);
			for(i = len - 1; i >= 0; i--) {
				printf("%02x ", buf[i]);
				if (!(i + 1) % 16)
					printf("\n");
			}
			printf("\n");
		}
#endif
		ret = write(uibc->hid_fd, buf, len);
		if (ret != len) {
			cvt_dbg(CVT_ERROR, "uibc write data error, ret=%d\n", ret);
		} else 
			cvt_dbg(CVT_TRACE, "uibc write data success len=%d\n", ret);
	} while(1);
}

int main(int argc, char **argv)
{
	struct uibc_st *uibc;
	struct sockaddr_in addr;
	int ret;

	signal(SIGPIPE, SIG_IGN);

	uibc = malloc(sizeof(struct uibc_st));
	if(uibc == NULL)
		return -1;

	start_hidg(uibc);

	uibc->sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (0 >= uibc->sock_fd){
		cvt_dbg(CVT_ERROR, "uibc create_udp_server create socket error\n");
		return -1;
	}

	memset(addr.sin_zero, 0, sizeof(addr.sin_zero));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(SERVER_DEF_PORT);

	ret = bind(uibc->sock_fd, (const struct sockaddr *)&addr, sizeof(addr));
	if (ret < 0) {
		cvt_dbg(CVT_ERROR, "uibc create_udp_server bind error\n");
		return -1;
	}

	ret = pthread_create(&uibc->thread_pid, NULL, uibc_thread, (void*)uibc);
	if (ret) {
		printf("%s: pthread_create fail!\n", __func__);
		return -1;
	}

	pthread_join(uibc->thread_pid, NULL);

	close(uibc->hid_fd);
	close(uibc->sock_fd);
	free(uibc);

	return 0;
}



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <syslog.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>

#include <linux/types.h>
#include <linux/input.h>
#include <linux/hidraw.h>

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
	struct sockaddr* sock_to;
	int sock_tolen;
	pthread_t thread_pid;
};

#define HID_MOUSE_DEVICE_PATH "/dev/hidraw2"
#define SERVER_IP_ADDR  "192.168.49.111"
#define SERVER_DEFAULT_PORT 1111
#define HID_DEV_BUF_SZIE  128 //128Byte
/*#define HID_TARGET_DATA_LEN	64*/

static int open_mouse(struct uibc_st *uibc)
{
	uibc->hid_fd = open(HID_MOUSE_DEVICE_PATH, O_RDWR, 0666);
	if (uibc->hid_fd == -1){
		printf("open device error: %s\n", strerror(errno));
		return -1;
	}

	return 0;
}

static void* uibc_thread(void *threadData)
{
	struct uibc_st *uibc =  (struct uibc_st*)threadData;
	char buf[HID_DEV_BUF_SZIE];
	fd_set rs;
	int ret = 0, len = 0;

	cvt_dbg(CVT_TRACE, "%s enter.\n", __func__);

    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

	do {
		FD_ZERO(&rs);
		FD_SET(uibc->hid_fd, &rs);
		ret = select(uibc->hid_fd + 1, &rs, NULL, NULL, NULL);
		if (ret <= 0) {
			cvt_dbg(CVT_ERROR, "uibc select error! ret=%d\n", ret);
			continue;
		}

		if (FD_ISSET(uibc->hid_fd, &rs)) {
			memset(buf, 0, sizeof(buf));
			len = read(uibc->hid_fd, buf, sizeof(buf));
			if (len <= 0) {
				cvt_dbg(CVT_ERROR, "uibc read error, len=%d\n", len);
				exit(0);
			}
		}
#ifdef UIBC_DUMP_DATA
		{//debug
			int i = 0;

			printf("uibc read device raw data[len: %d]: BIT%d~BIT0\n\t\t",
					len, len - 1);
			for(i = len - 1; i >= 0; i--) {
				printf("%02x ", buf[i]);
				if (!(i+1) % 16)
					printf("\n");
			}
			printf("\n");
		}
#endif

#ifdef HID_TARGET_DATA_LEN
		printf("===> func: %s, line: %d\n", __func__, __LINE__);
		if (HID_TARGET_DATA_LEN != len) {
			cvt_dbg(CVT_ERROR, "uibc read device data error\n");
			continue;
		}
#endif
		ret = sendto(uibc->sock_fd, buf, len, 0, uibc->sock_to, uibc->sock_tolen);
		if (ret < 0)
			cvt_dbg(CVT_ERROR, "uibc write data err:%d \n",ret);
		else
			cvt_dbg(CVT_TRACE, "uibc write success, len=%d!\n", len);
	} while(1);
}

static int uibc_get_device_info(struct uibc_st *uibc)
{
	struct hidraw_devinfo dinfo;
	int ret;

	ret = ioctl(uibc->hid_fd, HIDIOCGRAWINFO, &dinfo);
	if (ret) {
		cvt_dbg(CVT_ERROR, "%s, error ret=%d\n", __func__, ret);
		return ret;
	}

	printf("dinfo: bustype:[0x%08x], vendor:[0x%04x], product:[0x%04x]\n",
			dinfo.bustype, dinfo.vendor, dinfo.product);
	return 0;
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

	ret = open_mouse(uibc);
	if(ret) {
		free(uibc);
		return ret;
	}

	uibc_get_device_info(uibc);

	cvt_dbg(CVT_TRACE, "uibc enter, device: %s\n", HID_MOUSE_DEVICE_PATH);

	uibc->sock_fd = socket(AF_INET,SOCK_DGRAM, 0);
	if (uibc->sock_fd < 0){
	    cvt_dbg(CVT_ERROR, "create play request socket failed\n");
	    return -1;
	}

	memset(addr.sin_zero, 0, sizeof(addr.sin_zero));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(SERVER_IP_ADDR);
	addr.sin_port = htons(SERVER_DEFAULT_PORT);

	uibc->sock_to = (struct sockaddr *)&addr;
	uibc->sock_tolen = sizeof(addr);

	cvt_dbg(CVT_TRACE, "socket fd=%d, ip: %s, port:%d\n",
			        uibc->sock_fd, SERVER_IP_ADDR, SERVER_DEFAULT_PORT);

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



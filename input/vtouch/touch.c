/* Copyright (C) 2023 wqshao All rights reserved.
 *
 *  File Name    : touch.c
 *  Author       : wqshao
 *  Created Time : 2023-12-06 09:45:36
 *  Description  :
 *    linux: gcc touch.c -lpthread
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <linux/uinput.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>  //access
#include <pthread.h>
#include <sys/sysmacros.h> //makedev
#include <dirent.h>
#include <syslog.h>

#ifdef ANDROID
#include <sys/system_properties.h>
#endif

/* uinput.h中定义了如下内容:
#define UINPUT_MAX_NAME_SIZE    80
struct uinput_user_dev {
    char name[UINPUT_MAX_NAME_SIZE];     ------>用户定义生成虚拟设备的名称
    struct input_id id;                  ------>生成描述虚拟设备的一些参数信息(连接方式、厂商ID、产品ID、版本)
    __u32 ff_effects_max;
    __s32 absmax[ABS_CNT];               ------>最大绝对坐标值
    __s32 absmin[ABS_CNT];               ------>最小绝对坐标值
    __s32 absfuzz[ABS_CNT];              ------>坐标的分辨率
    __s32 absflat[ABS_CNT];              ------>坐标的基准值
}; */

#define INPUT_MT_TYPE_B 1

#define UDEV_TOUCH_SCREEN_NAME "VirTouchScreen"
#define UDEV_INPUT_NODE_NAME "/dev/input/event1"

#define UEVENT_MSG_LEN 2048

//#define DEBUG

#if 0
#define LOGE(fmt, ...) printf("<E> [%s:%d] " fmt , __func__, __LINE__, ##__VA_ARGS__)
#define LOGW(fmt, ...) printf("<W> [%s:%d] " fmt , __func__, __LINE__, ##__VA_ARGS__)
#ifdef DEBUG
#define LOGD(fmt, ...) printf("<D> [%s:%d] " fmt , __func__, __LINE__, ##__VA_ARGS__)
#define LOGI(fmt, ...) printf("<I> [%s:%d] " fmt , __func__, __LINE__, ##__VA_ARGS__)
#else
#define LOGD(fmt, ...)
#define LOGI(fmt, ...)
#endif
#else
#define LOGE(fmt, ...) syslog(LOG_ERR, "<E> [%s:%d] " fmt , __func__, __LINE__, ##__VA_ARGS__)
#define LOGW(fmt, ...) syslog(LOG_WARNING, "<W> [%s:%d] " fmt , __func__, __LINE__, ##__VA_ARGS__)
#ifdef DEBUG
#define LOGD(fmt, ...) syslog(LOG_INFO, "<D> [%s:%d] " fmt , __func__, __LINE__, ##__VA_ARGS__)
#define LOGI(fmt, ...) syslog(LOG_INFO, "<I> [%s:%d] " fmt , __func__, __LINE__, ##__VA_ARGS__)
#else
#define LOGD(fmt, ...)
#define LOGI(fmt, ...)
#endif
#endif

static int create_touch_screen(char* dev_name)
{
	struct uinput_user_dev udev;
	struct input_event event;
	int fd;
	int ret = 0;

	fd = open("/dev/uinput", O_WRONLY|O_NONBLOCK);
	if(fd < 0) {
		LOGE("Unable to open /dev/uinput. errno=%d: %s\n", errno, strerror(errno));
		return -1;
	}
	LOGI("open /dev/uinput, fd=%d\n", fd);

	memset(&udev, 0, sizeof(udev));

	//设备的别名
	sprintf(udev.name, "%s", dev_name);
	udev.id.bustype = BUS_USB;
	udev.id.vendor  = 0;
	udev.id.product = 0;
	udev.id.version = 0;

	//udev.absmin[ABS_MT_TOUCH_MAJOR] = 0;
	//udev.absmax[ABS_MT_TOUCH_MAJOR] = 15;
	udev.absmin[ABS_MT_POSITION_X] = 0;      //屏幕最小的X尺寸
	udev.absmax[ABS_MT_POSITION_X] = 2560;   //屏幕最大的X尺寸
	udev.absmin[ABS_MT_POSITION_Y] = 0;      //屏幕最小的Y尺寸
	udev.absmax[ABS_MT_POSITION_Y] = 1440;   //屏幕最大的Y尺寸
#ifdef INPUT_MT_TYPE_B
	udev.absmin[ABS_MT_SLOT] = 0;            //最小的触摸点数
	udev.absmax[ABS_MT_SLOT] = 9;            //MT代表multi touch 多指触摸 最大手指的数量我们设置9
	udev.absmin[ABS_MT_TRACKING_ID] = 0;     //
	udev.absmax[ABS_MT_TRACKING_ID] = 65535; //按键码ID累计叠加最大值
#endif
	//udev.absmin[ABS_MT_PRESSURE] = 0;
	//udev.absmax[ABS_MT_PRESSURE] = 255;      //屏幕按下的压力值

	// Setup the uinput device
	ioctl(fd, UI_SET_EVBIT, EV_KEY);         //该设备支持按键
	ioctl(fd, UI_SET_EVBIT, EV_REL);         //该设备支持释放

	// Touch
	ioctl(fd, UI_SET_EVBIT,  EV_ABS);        //支持触摸
	//ioctl(fd, UI_SET_ABSBIT, ABS_MT_TOUCH_MAJOR);  //报告触摸接触面的横截面积或触摸接触面较长尺寸的长度。
	ioctl(fd, UI_SET_ABSBIT, ABS_MT_POSITION_X);   //报告工具的 X 坐标。
	ioctl(fd, UI_SET_ABSBIT, ABS_MT_POSITION_Y);   //报告工具的 Y 坐标。
	ioctl(fd, UI_SET_ABSBIT, ABS_MT_TRACKING_ID);  //报告工具的跟踪 ID。
	//ioctl(fd, UI_SET_ABSBIT, ABS_MT_PRESSURE);     //报告应用于工具尖端的物理压力或触摸接触面的信号强度。开启在发送event时需要配置
#ifdef INPUT_MT_TYPE_B
	ioctl(fd, UI_SET_ABSBIT, ABS_MT_SLOT);
#endif
	ioctl(fd, UI_SET_PROPBIT, INPUT_PROP_DIRECT);

	ioctl(fd, UI_SET_KEYBIT, BTN_TOUCH);     //表示触摸

	// Key
	ioctl(fd, UI_SET_KEYBIT, KEY_HOMEPAGE);  // Home键
	ioctl(fd, UI_SET_KEYBIT, KEY_BACK);      // 返回键
	ioctl(fd, UI_SET_KEYBIT, KEY_APPSELECT); // Switch APP
	ioctl(fd, UI_SET_KEYBIT, KEY_VOLUMEDOWN);
	ioctl(fd, UI_SET_KEYBIT, KEY_VOLUMEUP);

	/* Create input device into input sub-system */
	write(fd, &udev, sizeof(udev));

	ret = ioctl(fd, UI_DEV_CREATE);
	if (ret < 0) {
		LOGE("ioctl UI_DEV_CREATE failed. errno=%d: %s\n", errno, strerror(errno));
		return -1;
	}

	LOGI("Create input devices success.\n");

	return fd;
}

#ifdef ANDROID
static int make_node(const char* dev_node_name, int major, int minor)
{
	int ret = 0;
	mode_t mode = 0660;
	int type = S_IFCHR;

	if (access(dev_node_name, F_OK) == 0) {
		LOGW("[%s] file exists, remove.\n", dev_node_name);
		unlink(dev_node_name);
	}

	LOGD("Gen node: %s\n", dev_node_name);
	ret = mknod(dev_node_name, mode | type, makedev(major, minor));
	if (ret < 0) {
		LOGE("mknod create node failed. errno=%d: %s\n", errno, strerror(errno));
	}

	chmod(dev_node_name, mode);
	uid_t uid_root = 0;      // id
	gid_t gid_input = 1004;  // id -G input
	chown(dev_node_name, uid_root, gid_input);

	return ret;
}

ssize_t safe_read(int fd, void *buf, size_t count)
{
	ssize_t n;

	for (;;) {
		n = read(fd, buf, count);
		if (n >= 0 || errno != EINTR)
			break;
		errno = 0;
	}

	return n;
}
ssize_t full_read(int fd, void *buf, size_t len)
{
	ssize_t cc;
	ssize_t total;

	total = 0;

	while (len) {
		cc = safe_read(fd, buf, len);

		if (cc < 0) {
			if (total) {
				return total;
			}
			return cc; /* read() returns -1 on failure. */
		}
		if (cc == 0)
			break;
		buf = ((char *)buf) + cc;
		total += cc;
		len -= cc;
	}

	return total;
}

ssize_t read_close(int fd, void *buf, size_t size)
{
	/*int e;*/
	size = full_read(fd, buf, size);
	/*e = errno;*/
	close(fd);
	/*errno = e;*/
	return size;
}

ssize_t open_read_close(const char *filename, void *buf, size_t size)
{
	int fd = open(filename, O_RDONLY);
	if (fd < 0)
		return fd;
	return read_close(fd, buf, size);
}

static int check_input_dev_name(char* dir_name, char* dev_name)
{
	char dev_name_file[512];

	sprintf(dev_name_file, "%s/%s", dir_name, "name");

	LOGI("dev_name_file=%s, dev_name=%s\n", dev_name_file, dev_name);

	if (access(dev_name_file, F_OK)) {
		LOGD("file: %s not exist\n", dev_name_file);
		return -1;
	}

	int len = 0;
	char buf[64];
	memset(buf, 0, sizeof(buf));
	len = open_read_close(dev_name_file, buf, sizeof(buf));
	buf[len - 1] = '\0';
	LOGI("dev name buf=[%s], buf len=%zu, ret len=%d\n", buf, strlen(buf), len);

	if (strcmp(buf, dev_name)) {
		LOGD("udev path %s, name(%s) and dev_name(%s) do not match.\n", dev_name_file, buf, dev_name);
		return -2;
	}

	return 0;
}

static int get_input_dev_info(char* dir_name, int* major, int* minor)
{
	DIR *dirp;
	struct dirent *direntp;
	int event_id = 0;
	int ret = -1;

	LOGI("dir_name: %s\n", dir_name);

	if((dirp = opendir(dir_name)) == NULL) {
		LOGE("Open Dir %s Error: %s\n", dir_name, strerror(errno));
		return -1;
	}

	while((direntp = readdir(dirp)) != NULL) {
		//LOGI("dir or file name: %s\n", direntp->d_name);
		if (strstr(direntp->d_name, "event") == NULL) {
			LOGD("event dir: %s.\n", direntp->d_name);
			continue;
		}

		char event_dev_file[128];
		memset(event_dev_file, 0, sizeof(event_dev_file));
		strcpy(event_dev_file, dir_name);
		strcat(event_dev_file, "/");
		strcat(event_dev_file, direntp->d_name);
		strcat(event_dev_file,"/dev");

		if (access(event_dev_file, F_OK)) {
			LOGD("file: %s not exist\n", event_dev_file);
			continue;
		}

		LOGI("event dev file: %s. d_name=%s\n", event_dev_file, direntp->d_name);
#if 0
		char* id = strtok(direntp->d_name, "event");
		event_id = atoi(id);
		LOGI("--------event id=%d\n", event_id);
#endif
		int len = 0;
		char buf[64];
		memset(buf, 0, sizeof(buf));
		len = open_read_close(event_dev_file, buf, sizeof(buf));
		buf[len - 1] = '\0';
		LOGI("dev name buf=[%s], buf len=%zu, ret len=%d\n", buf, strlen(buf), len);

		sscanf(buf, "%u:%u", major, minor);
		LOGI("======= event major:minor=%d:%d\n", *major, *minor);
		ret = 0;
		break;
	}
	closedir(dirp);

	return ret;
}

static int mk_dev_input_event(char* dev_name)
{
	DIR *dirp;
	struct dirent *direntp;
	char* sys_dev_path="/sys/devices/virtual/input";
	int ret = -1;

	if((dirp = opendir(sys_dev_path)) == NULL) {
		LOGE("Open Dir %s Error: %s\n", sys_dev_path, strerror(errno));
		return -1;
	}

	while((direntp = readdir(dirp)) != NULL) {
		char dirbuf[128];
		memset(dirbuf, 0, sizeof(dirbuf));
		strcpy(dirbuf, sys_dev_path);
		strcat(dirbuf, "/");
		strcat(dirbuf, direntp->d_name);
		LOGI("All path: %s\n", dirbuf);

		if (check_input_dev_name(dirbuf, dev_name) != 0) {
			continue;
		}

		LOGI("Useful path: %s\n", dirbuf);

		int major, minor;
		if (get_input_dev_info(dirbuf, &major, &minor) != 0) {
			LOGE("Get inpt dev(%s) info failed.\n", dirbuf);
			break;
		}
		LOGI("input dev event major:minor=%d:%d.\n", major, minor);

		make_node(UDEV_INPUT_NODE_NAME, major, minor);
		ret = 0;
		break;
	}
	closedir(dirp);

	return ret;
}
#endif

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t udev_cond_run = PTHREAD_COND_INITIALIZER;

static void close_udev_sig(int sig)
{
	LOGD("signal=%d.\n", sig);

	pthread_cond_signal(&udev_cond_run);

	signal(sig, SIG_DFL);
}

int main(int argc, const char *argv[])
{
	int ret = 0;
	int touch_fd = 0;

	signal(SIGINT, close_udev_sig);
	signal(SIGKILL, close_udev_sig);
	signal(SIGTERM, close_udev_sig);

	char dev_name[64];
	char idx[16];

#ifdef ANDROID
	__system_property_get("ro.boot.vtouch_idx", idx);
#else
	sprintf(idx, "%s", "test");
#endif
	LOGI("idx=%s\n", idx);
	if (!strlen(idx)) {
		LOGE("ro.boot.vtouch_idx is not set. Please set a unique ID");
		return -1;
	}
	sprintf(dev_name, "%s-%s", UDEV_TOUCH_SCREEN_NAME, idx);

	LOGW("Create Touch Screen. dev_name=%s, Build Time: %s %s\n", dev_name, __TIME__, __DATE__);
	touch_fd = create_touch_screen(dev_name);
	if (touch_fd < 0) {
		LOGE("create touch screen failed.\n");
		return -1;
	}

#ifdef ANDROID
	if (mk_dev_input_event(dev_name)) {
		LOGE("mk dev input event node failed.\n");
		return -1;
	}
#endif

	pthread_cond_wait(&udev_cond_run, &mutex);
	printf("\n\n\n");

	LOGI("Close Touch Screen.\n");
	close(touch_fd);
	unlink(UDEV_INPUT_NODE_NAME);

	return 0;
}



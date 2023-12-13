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
#include <sys/types.h>
#include <sys/socket.h>
#include <linux/netlink.h> //struct sockaddr_nl
#include <sys/sysmacros.h> //makedev

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

#define UEVENT_MSG_LEN 2048

#define DEBUG

#define LOGE(fmt, ...) printf("<E> [%s:%d] " fmt , __func__, __LINE__, ##__VA_ARGS__)
#define LOGW(fmt, ...) printf("<W> [%s:%d] " fmt , __func__, __LINE__, ##__VA_ARGS__)
#ifdef DEBUG
#define LOGD(fmt, ...) printf("<D> [%s:%d] " fmt , __func__, __LINE__, ##__VA_ARGS__)
#define LOGI(fmt, ...) printf("<I> [%s:%d] " fmt , __func__, __LINE__, ##__VA_ARGS__)
#else
#define LOGD(fmt, ...)
#define LOGI(fmt, ...)
#endif

struct cooling_device {
	const char *name;
    const char *action;
    const char *path;
	const char *subsystem;
	const char *dev_name;
	int major;
	int minor;
};

static int create_touch_screen()
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

	char* idx = getenv("ANDROID_IDX");
	//设备的别名
	sprintf(udev.name, "%s-%s", UDEV_TOUCH_SCREEN_NAME, idx);
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
static int open_uevent_socket(void)
{
    struct sockaddr_nl addr;
    int sz = 64*1024;
    int s = 0;

    memset(&addr, 0, sizeof(addr));
    addr.nl_family = AF_NETLINK;
    addr.nl_pid = getpid();
    addr.nl_groups = 0xffffffff;

    s = socket(PF_NETLINK, SOCK_DGRAM, NETLINK_KOBJECT_UEVENT);
    if (s < 0) {
		LOGE("socket failed. errno=%d: %s\n", errno, strerror(errno));
        return -1;
    }

    setsockopt(s, SOL_SOCKET, SO_RCVBUFFORCE, &sz, sizeof(sz));

    if (bind(s, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
        close(s);
		LOGE("bind failed. errno=%d: %s\n", errno, strerror(errno));
        return -1;
    }

    return s;
}

static int make_node(const char* dev_name, int major, int minor)
{
	int ret = 0;
	char node_name[64];
	mode_t mode = 0660;
	int type = S_IFCHR;

	sprintf(node_name, "/dev/%s", dev_name);

	if (access(node_name, F_OK) == 0) {
		LOGW("[%s] file exists, remove.\n", node_name);
		unlink(node_name);
	}

	LOGD("Gen node: %s\n", node_name);
	ret = mknod(node_name, mode | type, makedev(major, minor));
	if (ret < 0) {
		LOGE("mknod create node failed. errno=%d: %s\n", errno, strerror(errno));
	}

	chmod(node_name, mode);
	uid_t uid_root = 0;  // id
	gid_t gid_input = 1004; // id -G input
	chown(node_name, uid_root, gid_input);

	return ret;
}

static void del_node(const char* dev_name)
{
	int ret = 0;
	char node_name[64];

	sprintf(node_name, "/dev/%s", dev_name);

	LOGD("Del node: %s\n", node_name);
	ret = unlink(node_name);
	if (ret < 0) {
		LOGE("unlink delete node failed. errno=%d: %s\n", errno, strerror(errno));
	}
}

static int parse_uevent(const char* msg, struct cooling_device* cdev)
{
	int ret = 0;
	LOGD("------------------------------------ entry.\n");
    while (*msg) {
        LOGD("%s\n", msg);
        if (!strncmp(msg, "NAME=", 5)) {
            msg += 5;
            cdev->name = msg;
        } else if (!strncmp(msg, "ACTION=", 7)) {
            msg += 7;
            cdev->action = msg;
        } else if (!strncmp(msg, "DEVPATH=", 8)) {
            msg += 8;
            cdev->path = msg;
        } else if (!strncmp(msg, "SUBSYSTEM=", 10)) {
            msg += 10;
            cdev->subsystem = msg;
        } else if (!strncmp(msg, "MAJOR=", 6)) {
            msg += 6;
            cdev->major = atoi(msg);
        } else if (!strncmp(msg, "MINOR=", 6)) {
            msg += 6;
            cdev->minor = atoi(msg);
        } else if (!strncmp(msg, "DEVNAME=", 8)) {
            msg += 8;
            cdev->dev_name = msg;
        }
        while(*msg++);
    }

	if (cdev->subsystem && strncmp(cdev->subsystem, "input", 5)){
		//LOGW("not input subsystem.\n");
		return -1;
	}

	char name[64];
	char* idx = getenv("ANDROID_IDX");
	sprintf(name, "\"%s-%s\"", UDEV_TOUCH_SCREEN_NAME, idx);
	if (cdev->name && strncmp(cdev->name, name, sizeof(name))) {
		LOGW("the current device name(%s) is not %s\n", cdev->name, name);
		return -1;
	}

	LOGD("action=%s, dev_name=%s, path=%s\n", cdev->action, cdev->dev_name, cdev->path);

	char vir_event_dev_path[128];
	sprintf(vir_event_dev_path, "/sys/%s/", cdev->path);

	LOGD("vir_event_dev_path=%s\n", vir_event_dev_path);

	//添加dev设备节点
	if (!strncmp(cdev->action, "add", 3) && cdev->dev_name) {
		LOGD("event: name=%s, action=%s, path=%s, dev_name=%s major:minor=%d:%d\n",
				cdev->name, cdev->action, cdev->path, cdev->dev_name, cdev->major, cdev->minor);

		ret = make_node(cdev->dev_name, cdev->major, cdev->minor);
		return ret;
	}

	//移除dev设备节点
	if (!strncmp(cdev->action, "remove", 6) && cdev->dev_name) {
		LOGD("event: name=%s, action=%s, path=%s, dev_name=%s major:minor=%d:%d\n",
				cdev->name, cdev->action, cdev->path, cdev->dev_name, cdev->major, cdev->minor);
		del_node(cdev->dev_name);
	}

	LOGD("------------------------------------ exit.\n");
	return -2;
}

static void* uevent_handling(void* data)
{
	int socket_fd = -1;
    char msg[UEVENT_MSG_LEN+2];
    int n;

    socket_fd = open_uevent_socket();
    LOGD("entry socket_fd = %d\n", socket_fd);

    do {
        while((n = recv(socket_fd, msg, UEVENT_MSG_LEN, 0)) > 0) {
            struct cooling_device cdev;
            memset(&cdev, 0x0, sizeof(cdev));

            if(n == UEVENT_MSG_LEN)
                continue;

            msg[n] = '\0';
            msg[n+1] = '\0';

            parse_uevent(msg, &cdev);
        }
    } while(1);

	LOGD("exit.\n");
	return NULL;
}

static int my_udev_event()
{
	pthread_t tid;

	if (pthread_create(&tid, NULL, uevent_handling, NULL)) {
		LOGE("child. create thread failed errno=%d: %s\n", errno, strerror(errno));
		return -1;
	}

	return 0;
}
#endif

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t udev_cond_run = PTHREAD_COND_INITIALIZER;

static void close_udev_sig(int sig)
{
	LOGD("signal=%d.\n", sig);

	pthread_cond_signal(&udev_cond_run);

	usleep(100*1000);
	signal(sig, SIG_DFL);
}

int main(int argc, const char *argv[])
{
	int ret = 0;
	int touch_fd = 0;

	signal(SIGINT, close_udev_sig);
	signal(SIGKILL, close_udev_sig);
	signal(SIGTERM, close_udev_sig);

#ifdef ANDROID
	ret = my_udev_event();
	if (ret < 0) {
		LOGE("my_udev_event failed.\n");
		return -1;
	}
#endif

	LOGI("Create Touch Screen.\n");
	touch_fd = create_touch_screen();
	if (touch_fd < 0) {
		LOGE("create_touch_screen failed.\n");
		return -1;
	}

	pthread_cond_wait(&udev_cond_run, &mutex);
	printf("\n\n\n");

	LOGI("Close Touch Screen.\n");
	close(touch_fd);

	return 0;
}



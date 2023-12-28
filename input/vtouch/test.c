/* Copyright (C) 2023 wqshao All rights reserved.
 *
 *  File Name    : test.c
 *  Author       : wqshao
 *  Created Time : 2023-12-11 10:10:27
 *  Description  :
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <linux/input.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define UDEV_EVENT_NAME "/dev/input/event1"

#define INPUT_MT_TYPE_B 1

static int global_tracking_id = 1;

static int device_write_event(int fd, uint16_t type, uint16_t keycode, int32_t value)
{
	struct input_event ev;

	memset(&ev, 0, sizeof(struct input_event));

	ev.type = type;
	ev.code = keycode;
	ev.value = value;

	if (write(fd, &ev, sizeof(struct input_event)) < 0) {
		char * mesg = strerror(errno);
		printf("nibiru uinput errormag info :%s\n", mesg);
		return 0;
	}

	return 1;
}

#ifdef INPUT_MT_TYPE_B
static void touch_down(int fd, int x, int y, int slot, int trackingid)
{
	printf("Touch Screen Type B fd=%d, x,y=(%d, %d), slot=%d, trackingid=%d\n", fd, x, y, slot, trackingid);

	/* minimal event sequence:
	   ABS_MT_SLOT 0
	   ABS_MT_TRACKING_ID 45
	   ABS_MT_POSITION_X x[0]
	   ABS_MT_POSITION_Y y[0]
	   ABS_MT_SLOT 1
	   ABS_MT_TRACKING_ID 46
	   ABS_MT_POSITION_X x[1]
	   ABS_MT_POSITION_Y y[1]
	   SYN_REPORT
	   */

	device_write_event(fd, EV_ABS, ABS_MT_SLOT, slot);
	device_write_event(fd, EV_ABS, ABS_MT_TRACKING_ID, trackingid);
	device_write_event(fd, EV_ABS, ABS_MT_POSITION_X, x);
	device_write_event(fd, EV_ABS, ABS_MT_POSITION_Y, y);
	//device_write_event(fd, EV_SYN, SYN_MT_REPORT, 0);
}
#else
static void touch_down(int fd, int x, int y)
{
	printf("Touch Screen Type A fd=%d, x,y=(%d, %d)\n", fd, x, y);

	/*
	   ABS_MT_POSITION_X x[0]
	   ABS_MT_POSITION_Y y[0]
	   SYN_MT_REPORT
	   ABS_MT_POSITION_X x[1]
	   ABS_MT_POSITION_Y y[1]
	   SYN_MT_REPORT
	   SYN_REPORT
	   */

	device_write_event(fd, EV_ABS, ABS_MT_POSITION_X, x);
	device_write_event(fd, EV_ABS, ABS_MT_POSITION_Y, y);
	device_write_event(fd, EV_SYN, SYN_MT_REPORT, 0);

	//device_write_event(fd, EV_SYN, SYN_REPORT, 0);
}
#endif

static void touch_up(int fd)
{
	printf("touch screen fd = %d\n", fd);
#ifdef INPUT_MT_TYPE_B
	for(int i = 1; i < 6; i++) {
		device_write_event(fd, EV_ABS, ABS_MT_SLOT, i);
		device_write_event(fd, EV_ABS, ABS_MT_TRACKING_ID, -1);
	}
#else
	device_write_event(fd, EV_KEY, BTN_TOUCH, 0);
	device_write_event(fd, EV_ABS, MT_TOOL_FINGER, 0);
#endif
	device_write_event(fd, EV_SYN, SYN_REPORT, 0);
}

static void send_key(int fd, int code)
{
	printf("Send key fd=%d, type=%d, code=%d\n", fd, EV_KEY, code);

	device_write_event(fd, EV_KEY, code, 1);
	device_write_event(fd, EV_SYN, SYN_REPORT, 0);
	device_write_event(fd, EV_KEY, code, 0);
	device_write_event(fd, EV_SYN, SYN_REPORT, 0);

	usleep(500 * 1000);
}

int main(int argc, const char *argv[])
{
	int fd = 0;

	fd = open(UDEV_EVENT_NAME, O_WRONLY|O_NONBLOCK);
	if (fd < 0) {
		printf("Open %s failed. errno=%d: %s\n", UDEV_EVENT_NAME, errno, strerror(errno));
		return -1;
	}
	device_write_event(fd, EV_KEY, BTN_TOUCH, 1);
	device_write_event(fd, EV_ABS, MT_TOOL_FINGER, 1);

#if 1
	for(int i = 0; i < 500; i+=10) {
		printf("----------------i=%d\n", i);
#ifdef INPUT_MT_TYPE_B
		touch_down(fd, 1 * i, i, 1, 1);
		touch_down(fd, 2 * i, i, 2, 2);
		touch_down(fd, 3 * i, i, 3, 3);
		touch_down(fd, 5 * i, i, 4, 4);
		touch_down(fd, 4 * i, i, 5, 5);
		device_write_event(fd, EV_SYN, SYN_REPORT, 0);
#else
		touch_down(fd, 1 * i, i);
		touch_down(fd, 2 * i, i);
		touch_down(fd, 3 * i, i);
		touch_down(fd, 4 * i, i);
		touch_down(fd, 5 * i, i);
		device_write_event(fd, EV_SYN, SYN_REPORT, 0);
#endif
		usleep(50*1000);
	}
	touch_up(fd);
#endif

	send_key(fd, KEY_VOLUMEUP);
	send_key(fd, KEY_VOLUMEUP);
	send_key(fd, KEY_VOLUMEUP);
	send_key(fd, KEY_VOLUMEUP);
	send_key(fd, KEY_VOLUMEDOWN);
	send_key(fd, KEY_VOLUMEDOWN);
	send_key(fd, KEY_VOLUMEDOWN);

#if 1
	send_key(fd, KEY_APPSELECT);
	send_key(fd, KEY_HOMEPAGE);
	usleep(100 * 1000);
	send_key(fd, KEY_APPSELECT);
#endif
	send_key(fd, KEY_BACK);

	usleep(10 * 1000);
	close(fd);

	return 0;
}

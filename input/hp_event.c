/* Copyright (C) 2024 wqshao All rights reserved.
 *
 *  File Name    : hp_event.c
 *  Author       : wqshao
 *  Created Time : 2024-11-19 13:41:22
 *  Description  :
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
//#include <linux/input-event-codes.h>
#include <linux/input.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


void hp_test(int fd)
{
	int ret;
	unsigned long sw_states[SW_MAX/32 + 1];
	int sw = SW_HEADPHONE_INSERT;

	memset(&sw_states[0], 0, sizeof(sw_states));
	ret = ioctl(fd, EVIOCGSW(sizeof(sw_states)), sw_states);
	if (ret < 0) {
		printf("failed to get switch state (%s)\n", strerror(errno));
		return;
	}
	ret = (sw_states[sw/32] >> (sw % 32)) & 1;
	printf("state: %d\n", ret);
	if (ret > 0)
		printf("------>HP connected.\n");
	else
		printf("------>HP disconnected.\n");
}

void usb_hp_test(int fd)
{
	int ret;
	uint32_t key_states[KEY_MAX/32 + 1];
	int eject_key = KEY_EJECTCD;

	memset(&key_states[0], 0, sizeof(key_states));
	ret = ioctl(fd, EVIOCGKEY(sizeof(key_states)), key_states);
	if (ret < 0) {
		printf("failed to get key state (%s)\n", strerror(errno));
		return;
	}

#if 0
	for (int i = 0; i < KEY_MAX; i++) {
		printf("===> func: %s, line: %di, i=%d, val=0x%x\n", __func__, __LINE__, i, key_states[i]);
	}
#endif
	//printf("===> func: %s, line: %d, idx=%d, k=0x%x\n", __func__, __LINE__, eject_key/32, key_states[eject_key/32]);

	ret = (key_states[eject_key/32] >> (eject_key%32)) & 1;
	printf("%s: state: %d\n", __func__, ret);
	if (ret > 0)
		printf("%s:------>HP connected.\n", __func__);
	else
		printf("%s:------>HP disconnected.\n", __func__);

}

int main(int argc, const char *argv[])
{
	char *input_dev="/dev/input/event2";
	int fd = 0;

	printf("%s: argv=%s\n", __func__, argv[1]);

        //fd = open(input_dev, O_RDONLY);
        fd = open(argv[1], O_RDONLY);

	//hp_test(fd);

	usb_hp_test(fd);

	close(fd);

	return 0;
}

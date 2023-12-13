/* Copyright (C) 2023 wqshao All rights reserved.
 *
 *  File Name    : inotify.c
 *  Author       : wqshao
 *  Created Time : 2023-12-12 13:56:55
 *  Description  :
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/inotify.h>

#define EVENT_NUM  12

const char *event_str[EVENT_NUM] = {
	"IN_ACCESS",
	"IN_MODIFY",
	"IN_ATTRIB",
	"IN_CLOSE_WRITE",
	"IN_CLOSE_NOWRITE",
	"IN_OPEN",
	"IN_MOVED_FROM",
	"IN_MOVED_TO",
	"IN_CREATE",
	"IN_DELETE",
	"IN_DELETE_SELF",
	"IN_MOVE_SELF"
};


int inotifyTask(char *argv[])
{
	int errTimes = 0;
	int fd = -1;
INIT_INOTIFY:
	fd = inotify_init();

	if (fd < 0) {
		fprintf(stderr, "inotify_init failed\n");
		printf("Error no.%d: %s\n", errno, strerror(errno));
		goto INOTIFY_FAIL;
	}

	int wd1 = -1;
	int wd2 = -1;
	struct inotify_event *event;
	int length;
	int nread;
	char buf[BUFSIZ];
	int i = 0;
	buf[sizeof(buf) - 1] = 0;
INOTIFY_AGAIN:
	wd1 = inotify_add_watch(fd, argv[1], IN_ALL_EVENTS);

	if (wd1 < 0) {
		fprintf(stderr, "inotify_add_watch %s failed\n", argv[1]);
		printf("Error no.%d: %s\n", errno, strerror(errno));

		if (errTimes < 3) {
			goto INOTIFY_AGAIN;

		} else {
			goto INOTIFY_FAIL;
		}
	}

	wd2 = inotify_add_watch(fd, argv[2], IN_ALL_EVENTS);

	if (wd2 < 0) {
		fprintf(stderr, "inotify_add_watch %s failed\n", argv[2]);
		printf("Error no.%d: %s\n", errno, strerror(errno));

		if (errTimes < 3) {
			goto INOTIFY_AGAIN;

		} else {
			goto INOTIFY_FAIL;
		}
	}

	length = read(fd, buf, sizeof(buf) - 1);
	nread = 0;

	// inotify 事件发生时
	while (length > 0) {
		printf("\n");
		event = (struct inotify_event *)&buf[nread];

		// 遍历所有事件
		for (i = 0; i < EVENT_NUM; i++) {
			// 判断事件是否发生
			if ((event->mask >> i) & 1) {
				// 该监控项为目录或目录下的文件时
				if (event->len > 0) {
					fprintf(stdout, "%s --- %s\n", event->name, event_str[i]);
				}

				// 该监控项为文件时
				else if (event->len == 0) {
					if (event->wd == wd1) {
						fprintf(stdout, "%s --- %s\n", argv[1], event_str[i]);
					}

					if (event->wd == wd2) {
						fprintf(stdout, "%s --- %s\n", argv[2], event_str[i]);
					}
				}
			}
		}

		nread = nread + sizeof(struct inotify_event) + event->len;
		length = length - sizeof(struct inotify_event) - event->len;
	}

	goto INOTIFY_AGAIN;
	close(fd);
	return 0;
INOTIFY_FAIL:
	return -1;
}

int main(int argc, char *argv[])
{
	if (argc < 3) {
		fprintf(stderr, "Usage: %s path path\n", argv[0]);
		return -1;
	}

	if (inotifyTask(argv) == -1) {
		return -1;
	}

	return 0;
}

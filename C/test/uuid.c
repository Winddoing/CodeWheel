/* Copyright (C) 2023 wqshao All rights reserved.
 *
 *  File Name    : uuid.c
 *  Author       : wqshao
 *  Created Time : 2023-12-13 10:05:09
 *  Description  :
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <unistd.h> //read
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>


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

int main(int argc, const char *argv[])
{
	har uuid[64];
	int ret = 0;
	ret = open_read_close("/proc/sys/kernel/random/uuid", uuid, sizeof(uuid));
	uuid[ret - 1] = '\0';

	printf("===> func: %s, line: %d, ret=%d, uuid=[%s]\n", __func__, __LINE__, ret, uuid);
	
	return 0;
}

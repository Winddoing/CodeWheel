/*#############################################################
 *     File Name	: connect.c
 *     Author		: wqshao
 *     Created Time	: 2018年09月27日 星期四 14时09分35秒
 *     Description	:
 *############################################################*/

#include <stdio.h>
#include <stdlib.h>

int net_set_nonblocking(int sock)
{
	int flags, res;

	flags = fcntl(sock, F_GETFL, 0);
	if (flags < 0) {
		flags = 0;
	}

	res = fcntl(sock, F_SETFL, flags | O_NONBLOCK);
	if (res < 0) {
		printf("fcntl return err:%d!\n", res);
		return -1;
	}

	return 0;
}


int connect_timeout(int fd, struct sockaddr_in *addr, unsigned int wait_seconds)
{
	int ret;
	socklen_t addrlen = sizeof(struct sockaddr_in);

	if (wait_seconds > 0)
		net_set_nonblocking(fd);	//设为非阻塞

	ret = connect(fd, (struct sockaddr*)addr, addrlen);
	if (ret < 0 && errno == EINPROGRESS) {
		struct timeval timeout;
		fd_set write_fdset;

		FD_ZERO(&write_fdset);
		FD_SET(fd, &write_fdset);

		timeout.tv_sec = wait_seconds;
		timeout.tv_usec = 0;

		do {
			ret = select(fd + 1, NULL, &write_fdset, NULL, &timeout);
		} while (ret < 0 && errno == EINTR);

		if (ret == 0) {
			ret = -1;
			errno = ETIMEDOUT;
			printf("%s:%d, select error[%d]:%s, ret=%d\n", __func__, __LINE__, errno, strerror(errno), ret);
		} else if (ret < 0) {
			printf("%s:%d, select error[%d]:%s, ret=%d\n", __func__, __LINE__, errno, strerror(errno), ret);
			return -1;
		} else if (ret == 1) {
			int err;
			socklen_t socklen = sizeof(err);
			ret = getsockopt(fd, SOL_SOCKET, SO_ERROR, &err, &socklen);
			if (ret == -1) {
				printf("%s:%d, getsockopt error[%d]:%s, ret=%d\n", __func__, __LINE__, errno, strerror(errno), ret);
				return -1;
			}
			if (err == 0) {
				ret = 0; //success
			} else {
				errno = err;
				ret = -1;
				printf("%s:%d, getsockopt error[%d]:%s, ret=%d\n", __func__, __LINE__, errno, strerror(errno), ret);
			}
		}
	}
	if (wait_seconds > 0) {
		deactivate_nonblock(fd);	//设回阻塞
	}
	return ret;
}



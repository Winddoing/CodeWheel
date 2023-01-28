#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <errno.h>
#include <time.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>

#include "test.h"

s32 count_open_fds(void)
{
	DIR *dir;
	struct dirent *ent;
	s32 count = 0;

	dir = opendir("/proc/self/fd");
	assert(dir && "opening /proc/self/fd failed.");

	errno = 0;
	while ((ent = readdir(dir))) {
		const char *s = ent->d_name;
		if (s[0] == '.' && (s[1] == 0 || (s[1] == '.' && s[2] == 0)))
			continue;
		count++;
	}
	assert(errno == 0 && "reading /proc/self/fd failed.");

	closedir(dir);

	return count;
}

#define USEC_TO_NSEC(n) (1000 * (n))

void test_usleep(useconds_t usec)
{
	struct timespec ts = {
		.tv_sec = 0,
		.tv_nsec = USEC_TO_NSEC(usec)
	};

	assert(nanosleep(&ts, NULL) == 0);
}

void test_sleep(u32 sec)
{
	struct timespec ts = {
		.tv_sec = sec,
		.tv_nsec = 0
	};

	assert(nanosleep(&ts, NULL) == 0);
}

s32 fd_leak_check_enabled = 1;
s32 timeouts_enabled = 1;

void check_fd_leaks(s32 supposed_fds)
{
	s32 num_fds;

	if (fd_leak_check_enabled) {
		num_fds = count_open_fds();
		if (supposed_fds != num_fds) {
			fprintf(stderr, "fd leak detected in test. "
				"Opened %d files, unclosed %d\n", num_fds,
				num_fds - supposed_fds);
			abort();
		}
	} else {
		fprintf(stderr, "FD leak checks disabled\n");
	}
}

void test_set_timeout(u32 to)
{
	s32 re;

	if (!timeouts_enabled) {
		fprintf(stderr, "Timeouts supressed.\n");
		return;
	}

	re = alarm(to);
	fprintf(stderr, "Timeout was %sset", re ? "re-" : "");

	if (to != 0)
		fprintf(stderr, " to %d second%s from now.\n",
			to, to > 1 ? "s" : "");
	else
		fprintf(stderr, " off.\n");
}


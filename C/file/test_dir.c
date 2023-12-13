/* Copyright (C) 2023 wqshao All rights reserved.
 *
 *  File Name    : test_dir.c
 *  Author       : wqshao
 *  Created Time : 2023-12-12 14:01:22
 *  Description  :
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>

/* 文件大小和修改时间 */
static int get_file_size_time(const char *filename)
{
	struct stat statbuf;

	/* 判断未打开文件 */
	if (stat(filename, &statbuf) == -1) {
		printf("Get stat on %s Error: %s\n", filename, strerror(errno));
		return (-1);
	}

	if (S_ISDIR(statbuf.st_mode)) // 目录
		return (1);

	if (S_ISREG(statbuf.st_mode)) // 文件
		printf("%s size: %ld bytes\tmodified at %s", filename, statbuf.st_size, ctime(&statbuf.st_mtime));

	return (0);
}

int main(int argc, char **argv)
{
	DIR *dirp;
	struct dirent *direntp;
	int stats;

	if (argc != 2) {
		printf("Usage: %s filename\n\a", argv[0]);
		exit(1);
	}

	if (((stats = get_file_size_time(argv[1])) == 0) || (stats == -1)) // 文件或出现错误
		exit(1);

	/* 打开目录 */
	if ((dirp = opendir(argv[1])) == NULL) {
		printf("Open Directory %s Error: %s\n", argv[1], strerror(errno));
		exit(1);
	}

	printf("===> func: %s, line: %d, dirp=%p\n", __func__, __LINE__, dirp);

	/* 返回目录中文件大小和修改时间 */
	while ((direntp = readdir(dirp)) != NULL) {
		/* 给文件或目录名添加路径:argv[1]+"/"+direntp->d_name */
		char dirbuf[512];
		memset(dirbuf, 0, sizeof(dirbuf));
		strcpy(dirbuf, argv[1]);
		strcat(dirbuf, "/");
		strcat(dirbuf, direntp->d_name);
		printf("===> func: %s, line: %d, dirbuf=%s\n", __func__, __LINE__, dirbuf);
		usleep(1000 * 1000);

		if (get_file_size_time(dirbuf) == -1)
			break;
	}

	closedir(dirp);
	exit(1);
}


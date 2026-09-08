/* Copyright (C) 2026 wqshao All rights reserved.
 *
 *  File Name    : progress_bar1.c
 *  Author       : wqshao
 *  Created Time : 2026-03-05 13:42:51
 *  Description  : 进度条
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

typedef struct _progress {
	int cur_size;
	int sum_size;
} progress_t;

void progress_bar(progress_t *progress_data)
{
	int percentage = 0;
	int cnt = 0;
	char proc[102]; // 100个字符位 + 最后一个'#' + '\0'

	memset(proc, '\0', sizeof(proc));

	percentage = (int)((long long)progress_data->cur_size * 100 / progress_data->sum_size);
	printf("percentage = %d %%\n", percentage);

	if (percentage <= 100) {
		while (cnt <= percentage) {
			printf("[%-100s] [%d%%]\r", proc, cnt);
			fflush(stdout);
			proc[cnt] = '#';
			usleep(100000);
			cnt++;
		}
	}
	printf("\n");
}

int main(int arc, char *argv[])
{
	progress_t progress_test = {0};

	//progress_test.cur_size = 65;
	progress_test.cur_size = 100;
	progress_test.sum_size = 100;
	progress_bar(&progress_test);

	return 0;
}

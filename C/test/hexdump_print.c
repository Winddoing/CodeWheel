/* Copyright (C) 2024 wqshao All rights reserved.
 *
 *  File Name    : hexdump.c
 *  Author       : wqshao
 *  Created Time : 2024-08-21 14:16:18
 *  Description  :
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>


static void my_print_hex_dump(void *addr, const char *prefix_str, int prefix_type, unsigned int size)
{
	int i, j, k;
	char *buf = (char *)addr;
	ssize_t print_size = 0, ascii_size = 0;
	char print_buf[512] = {0}, ascii_buf[32] = {0};
	unsigned long temp_addr;

	if (prefix_type == 1) {
		//DUMP_PREFIX_ADDRESS=1
		temp_addr = (unsigned long)addr;
	} else {
		//DUMP_PREFIX_OFFSET=2
		temp_addr = 0;
	}

	uint64_t cur_line_sum = 0, last_line_sum = 0;
	int clr = 0;
	for (j = 0; j < size; j += 16) {
		print_size = 0;
		ascii_size = 0;
		print_size += sprintf(print_buf+print_size, "%s%08lx: ", prefix_str, temp_addr + j);

		cur_line_sum = 0;
		for (i = 0; (i < 16 && i < size); i+=4) {
			for (k = 3; k >= 0; k--) {
				print_size += sprintf(print_buf+print_size, "%02x", buf[j + i + k] & 0xff);
				cur_line_sum += buf[j + i + k] & 0xff;
			}
			for (k = 0; k <= 3; k++) {
				if((buf[j + i + k] & 0xff) < ' ' || (buf[j + i + k] & 0xff) > '~') {
					ascii_size += sprintf(ascii_buf+ascii_size, ".");

				} else {
					ascii_size += sprintf(ascii_buf+ascii_size, "%c", buf[j + i + k] & 0xff);
				}
			}
			print_size += sprintf(print_buf+print_size, " ");

		}
		print_size += sprintf(print_buf+print_size, "  %s\n", ascii_buf);

		if (cur_line_sum != last_line_sum) {
			/* 输出与上一行存在差异的行数据 */

			if (clr)
				printf("\n");

			last_line_sum = cur_line_sum;
			printf("%s", print_buf);
			clr = 0;
		} else {
			/* 输出省略信息 */

			if (clr)
				printf("\b*");
			else
				printf("*");

			clr = 1;
		}

		if (j == (size - 16))
			printf("\n%s", print_buf);
	}
	return;
}


int main(int argc, const char *argv[])
{
	int fd = -1;
	struct stat file_stat;
	uint64_t size = 0;
	void *buf = NULL;


	printf("file name: [%s]\n", argv[1]);

	fd = open(argv[1], O_RDONLY);
	if (fd < 0)
		return -1;

	fstat(fd, &file_stat);

	size = file_stat.st_size;
	printf("file size=%ld\n", size);

	buf = malloc(size);
	printf("buffer addr: %p\n", buf);

	read(fd, buf, size);

	getchar();
	printf("===> func: %s, line: %d\n", __func__, __LINE__);
	my_print_hex_dump(buf, "DATA: ", 0, size);
	printf("===> func: %s, line: %d\n", __func__, __LINE__);

	free(buf);
	close(fd);
	
	return 0;
}

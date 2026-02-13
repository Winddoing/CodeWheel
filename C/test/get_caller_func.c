/* Copyright (C) 2025 wqshao All rights reserved.
 *
 *  File Name    : get_caller_func.c
 *  Author       : wqshao
 *  Created Time : 2025-10-09 11:11:44
 *  Description  :
 */

#include <stdio.h>
#include <execinfo.h>
#include <string.h>
#include <stdlib.h>

int get_caller_function_name(char *name, int len)
{
        void *buffer[3] = {NULL};
        char **strings = NULL;
        int size = 0;
        int ret = 0;

        if (name == NULL || len <= 0) {
                return -1;
        }
        name[0] = '\0';

        size = backtrace(buffer, 3);
        if (size < 2)
                return -2;

        strings = backtrace_symbols(buffer, size);
        if (strings == NULL)
                return -3;

        if (strings[2] != NULL) {
                const char *caller_str = strings[2];
                size_t caller_len = strlen(caller_str);

                if (caller_len < (size_t)len) {
                        strcpy(name, caller_str);
                } else {
                        strncpy(name, caller_str, len - 1);
                        name[len - 1] = '\0';
                }
        } else {
                ret = -4;
        }

        if (strings) free(strings);

        return ret;
}

void current_function() {
	char caller[128] = {0};
	get_caller_function_name(caller, sizeof(caller));
	printf("当前函数: %s\n", __func__);
	printf("调用者: %s\n", caller);
}

void some_caller() {
	current_function();
}

int main() {
	some_caller();
	return 0;
}

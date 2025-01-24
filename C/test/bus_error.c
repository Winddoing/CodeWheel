/* Copyright (C) 2025 wqshao All rights reserved.
 *
 *  File Name    : bus_error.c
 *  Author       : wqshao
 *  Created Time : 2025-01-17 09:45:27
 *  Description  :
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

int main(int argc, const char *argv[])
{
    char *ptr = malloc(100);
    free(ptr);

    memset(ptr, 0, 100);  // 访问已释放的内存

    return 0;
}

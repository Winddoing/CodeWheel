/* Copyright (C) 2023 wqshao All rights reserved.
 *
 *  File Name    : test.c
 *  Author       : wqshao
 *  Created Time : 2023-02-02 17:06:00
 *  Description  :
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "kfifo.h"

//定义demo变量
struct demo_type {
    struct kfifo fifo;
};

//定义fifo的元素（使用结构体是为了便于增删内容）
struct demo_element {
    char val;
};

//定义fifo最大保存的元素个数
#define DM_FIFO_ELEMENT_MAX     32

//fifo句柄定义在全局，为了在不同的函数中进行出入队操作
static struct demo_type dm;

void demo(void)
{
    int ret = 0;
    struct demo_element element;

    //申请fifo内存空间，一般在模块初始化时调用
    ret = kfifo_alloc(dm.fifo, sizeof(struct demo_element)*DM_FIFO_ELEMENT_MAX);
    if (ret) {
        printf("kfifo_alloc fail ret=%d\n", ret);
        return;
    }

    //入队
    element.val = 111;
    ret = kfifo_in(dm.fifo, &element, sizeof(struct demo_element));
    if (!ret) {
        printf("kfifo_in fail, fifo is full\n");
    }

    //出队
    element.val = 0; //先清零，便于测试
    ret = kfifo_out(dm.fifo, &element, sizeof(struct demo_element));
    if (ret) {
        printf("kfifo_out element.val=%d\n", element.val);
    } else {
        printf("kfifo_out fail, fifo is empty\n");
    }

    //释放内存空间，一般在模块退出时调用
    kfifo_free(dm.fifo);
}

int main(int argc, const char *argv[])
{

	demo();
	
	return 0;
}

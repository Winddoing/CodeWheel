/* Copyright (C) 2026 wqshao All rights reserved.
 *
 *  File Name    : usb_only_reset.c
 *  Author       : wqshao
 *  Created Time : 2026-03-06 17:14:25
 *  Description  :
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

#if defined(__aarch64__)
	#include <libusb.h>
#else
	#include <libusb-1.0/libusb.h>
#endif

void print_usage(const char *prog)
{
	printf("用法：%s <vid> <pid>\n", prog);
	printf("示例：%s 0x0781 0x5572\n", prog);
	printf("vid/pid 支持 10进制 或 16进制 (0x开头)\n");
}

int main(int argc, char *argv[])
{
	libusb_context *ctx = NULL;
	libusb_device_handle *handle = NULL;
	int ret;
	uint16_t vid, pid;

	// ==========================
	// 1. 解析命令行 VID PID
	// ==========================
	if (argc != 3) {
		print_usage(argv[0]);
		return -1;
	}

	// 支持 0x 开头的16进制 或 普通10进制
	vid = (uint16_t)strtoul(argv[1], NULL, 0);
	pid = (uint16_t)strtoul(argv[2], NULL, 0);

	printf("目标设备：VID=0x%04X, PID=0x%04X\n", vid, pid);

	// ==========================
	// 2. 初始化 libusb
	// ==========================
	ret = libusb_init(&ctx);
	if (ret < 0) {
		fprintf(stderr, "libusb 初始化失败: %s\n", libusb_strerror(ret));
		return -1;
	}

	libusb_set_option(ctx, LIBUSB_OPTION_LOG_LEVEL, LIBUSB_LOG_LEVEL_WARNING);

	// ==========================
	// 3. 打开设备
	// ==========================
	handle = libusb_open_device_with_vid_pid(ctx, vid, pid);
	if (!handle) {
		fprintf(stderr, "打开设备失败！请检查 VID/PID 或权限\n");
		ret = -1;
		goto exit_usb;
	}
	printf("设备已打开\n");

	// ==========================
	// 4. 解绑内核驱动（关键：拦截枚举）
	// ==========================
	ret = libusb_detach_kernel_driver(handle, 0);
	if (ret < 0) {
		if (ret == LIBUSB_ERROR_NOT_FOUND) {
			printf("接口0本来就没有内核驱动，无需解绑\n");
		} else {
			fprintf(stderr, "解绑内核驱动失败: %s\n", libusb_strerror(ret));
			goto close_device;
		}
	} else {
		printf("已成功解绑内核驱动\n");
	}

#if 0
	// ==========================
	// 5. 声明独占接口
	// ==========================
	ret = libusb_claim_interface(handle, 0);
	if (ret < 0) {
		fprintf(stderr, "申请接口失败: %s\n", libusb_strerror(ret));
		goto close_device;
	}
	printf("已独占接口0，内核不会再管理此设备\n");
#endif

	// ==========================
	// 6. 设备复位
	// ==========================
	printf("正在复位设备...\n");
	ret = libusb_reset_device(handle);
	if (ret < 0) {
		fprintf(stderr, "设备复位失败: %s\n", libusb_strerror(ret));
		goto release_interface;
	}
	printf("设备复位完成 ✅\n");

	// ==========================
	// 7. 保持运行（持续拦截枚举）
	// ==========================
	printf("\n==== 设备已被用户态独占，内核不会枚举 ====\n");
	printf("按 Ctrl+C 退出\n");
	while (1) {
		sleep(1);
	}

release_interface:
	libusb_release_interface(handle, 0);
close_device:
	libusb_close(handle);
exit_usb:
	libusb_exit(ctx);
	return ret;
}

/* Copyright (C) 2026 wqshao All rights reserved.
 *
 *  File Name    : usb_exclusive.c
 *  Author       : wqshao
 *  Created Time : 2026-03-07 14:35:17
 *  Description  :
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#if defined(__aarch64__)
	#include <libusb.h>
#else
	#include <libusb-1.0/libusb.h>
#endif

static volatile int g_running = 1;  // 程序运行标志

// 信号处理：Ctrl+C 优雅退出
void sig_handler(int sig)
{
	g_running = 0;
	printf("\n收到退出信号，正在释放设备...\n");
}

// 打印用法
void print_usage(const char *prog)
{
	printf("=====================================================\n");
	printf("USB 设备独占重定向工具（拦截内核枚举）\n");
	printf("用法：sudo %s <vid> <pid>\n", prog);
	printf("示例：sudo %s 0x0781 0x5572\n", prog);
	printf("支持 十进制 / 十六进制 (0x 开头)\n");
	printf("=====================================================\n");
}

// 【核心函数】抢占单个接口：解绑内核驱动 + 声明独占
int claim_single_interface(libusb_device_handle *handle, uint8_t iface_num)
{
	int ret;

	// 1. 解绑内核驱动
	ret = libusb_detach_kernel_driver(handle, iface_num);
	if (ret == 0) {
		printf("→ 接口 %u：已解绑内核驱动\n", iface_num);
	} else if (ret == LIBUSB_ERROR_NOT_FOUND) {
		printf("→ 接口 %u：无内核驱动，无需解绑\n", iface_num);
	} else {
		fprintf(stderr, "→ 接口 %u：解绑内核驱动失败：%s\n", iface_num, libusb_strerror(ret));
		return ret;
	}

	// 2. 声明用户态独占（关键：内核看到此标记就不枚举）
	ret = libusb_claim_interface(handle, iface_num);
	if (ret == 0) {
		printf("→ 接口 %u：已用户态独占 ✅\n", iface_num);
	} else {
		fprintf(stderr, "→ 接口 %u：独占失败：%s\n", iface_num, libusb_strerror(ret));
		return ret;
	}

	return 0;
}

// 【核心函数】抢占设备所有接口（多接口设备必须）
int claim_all_interfaces(libusb_device_handle *handle)
{
	int ret;
	libusb_device *dev = libusb_get_device(handle);
	struct libusb_device_descriptor dev_desc;

	// 获取设备描述符
	ret = libusb_get_device_descriptor(dev, &dev_desc);
	if (ret < 0) {
		fprintf(stderr, "获取设备描述符失败：%s\n", libusb_strerror(ret));
		return ret;
	}

	printf("\n======== 开始抢占所有接口（共 %u 个配置）========\n", dev_desc.bNumConfigurations);

	// 遍历所有配置
	for (uint8_t conf_idx = 0; conf_idx < dev_desc.bNumConfigurations; conf_idx++) {
		struct libusb_config_descriptor *conf = NULL;
		ret = libusb_get_config_descriptor(dev, conf_idx, &conf);
		if (ret < 0) continue;

		// 遍历当前配置下的所有接口
		for (uint8_t if_idx = 0; if_idx < conf->bNumInterfaces; if_idx++) {
			const struct libusb_interface *iface = &conf->interface[if_idx];
			for (int alt_idx = 0; alt_idx < iface->num_altsetting; alt_idx++) {
				const struct libusb_interface_descriptor *alt = &iface->altsetting[alt_idx];
				claim_single_interface(handle, alt->bInterfaceNumber);
			}
		}
		libusb_free_config_descriptor(conf);
	}

	printf("======== 所有接口抢占完成 ========\n\n");
	return 0;
}

// 释放所有接口并重新绑定内核驱动
void release_and_attach_all(libusb_device_handle *handle)
{
    libusb_device *dev = libusb_get_device(handle);
    struct libusb_device_descriptor dev_desc;
    int ret;

    ret = libusb_get_device_descriptor(dev, &dev_desc);
    if (ret < 0) return;

    for (uint8_t conf_idx = 0; conf_idx < dev_desc.bNumConfigurations; conf_idx++) {
        struct libusb_config_descriptor *conf = NULL;
        ret = libusb_get_config_descriptor(dev, conf_idx, &conf);
        if (ret < 0) continue;

        for (uint8_t if_idx = 0; if_idx < conf->bNumInterfaces; if_idx++) {
            const struct libusb_interface *iface = &conf->interface[if_idx];
            for (int alt_idx = 0; alt_idx < iface->num_altsetting; alt_idx++) {
                const struct libusb_interface_descriptor *alt = &iface->altsetting[alt_idx];
                uint8_t iface_num = alt->bInterfaceNumber;

                // 1. 释放接口（如果之前 claim 了）
                ret = libusb_release_interface(handle, iface_num);
                if (ret == 0) {
                    printf("→ 接口 %u：已释放\n", iface_num);
                } else if (ret != LIBUSB_ERROR_NOT_FOUND && ret != LIBUSB_ERROR_NO_DEVICE) {
                    fprintf(stderr, "→ 接口 %u：释放失败：%s\n", iface_num, libusb_strerror(ret));
                }

                // 2. 重新绑定内核驱动（如果之前 detach 了）
                ret = libusb_attach_kernel_driver(handle, iface_num);
                if (ret == 0) {
                    printf("→ 接口 %u：内核驱动已绑定\n", iface_num);
                } else if (ret == LIBUSB_ERROR_NOT_FOUND) {
                    // 无内核驱动，忽略
                } else {
                    fprintf(stderr, "→ 接口 %u：绑定驱动失败：%s\n", iface_num, libusb_strerror(ret));
                }
            }
        }
        libusb_free_config_descriptor(conf);
    }
}

int main(int argc, char *argv[])
{
	libusb_context *ctx = NULL;
	libusb_device_handle *handle = NULL;
	int ret;
	uint16_t vid, pid;

	// 注册信号
	signal(SIGINT, sig_handler);
	signal(SIGTERM, sig_handler);

	// 1. 解析命令行参数
	if (argc != 3) {
		print_usage(argv[0]);
		return -1;
	}
	vid = (uint16_t)strtoul(argv[1], NULL, 0);
	pid = (uint16_t)strtoul(argv[2], NULL, 0);

	printf("目标设备：VID=0x%04X, PID=0x%04X\n", vid, pid);

	// 2. 初始化 libusb
	ret = libusb_init(&ctx);
	if (ret < 0) {
		fprintf(stderr, "libusb 初始化失败：%s\n", libusb_strerror(ret));
		return -1;
	}
	libusb_set_option(ctx, LIBUSB_OPTION_LOG_LEVEL, LIBUSB_LOG_LEVEL_WARNING);

	// 3. 打开设备
	handle = libusb_open_device_with_vid_pid(ctx, vid, pid);
	if (!handle) {
		fprintf(stderr, "错误：未找到设备或权限不足，请加 sudo 运行！\n");
		goto exit_libusb;
	}
	printf("设备已打开\n");

	// 4. 【第一次】抢占所有接口（复位前）
	claim_all_interfaces(handle);

	// 5. 执行设备复位
	printf("正在复位设备...\n");
	ret = libusb_reset_device(handle);
	if (ret < 0) {
		fprintf(stderr, "设备复位失败：%s\n", libusb_strerror(ret));
		goto release_all;
	}
	printf("设备复位完成 ✅\n");

	// ===================== 【核心修复】=====================
	// 复位后内核会清空独占标记 → 必须重新抢占所有接口
	// ======================================================
	printf("\n===== 复位后重新抢占接口（阻止内核枚举）=====\n");
	claim_all_interfaces(handle);

	// 6. 保持独占运行（内核永远不会枚举）
	printf("\n=====================================================\n");
	printf("设备已【完全用户态独占】，内核不会枚举、不会绑定驱动！\n");
	printf("可用于 USB 重定向 / 透传\n");
	printf("按 Ctrl+C 退出并释放设备\n");
	printf("=====================================================\n");

	int elapsed = 0;
	while (g_running && elapsed < 10) {
		sleep(1);
		elapsed++;
	}

	// 7. 优雅退出
release_all:
	release_and_attach_all(handle);
	libusb_close(handle);
exit_libusb:
	libusb_exit(ctx);
	printf("设备已释放，内核可重新枚举\n");
	return ret;
}

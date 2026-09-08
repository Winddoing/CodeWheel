/* Copyright (C) 2026 wqshao All rights reserved.
 *
 *  File Name    : usb_eye.c
 *  Author       : wqshao
 *  Created Time : 2026-03-23 09:26:44
 *  Description  :
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/usbdevice_fs.h>
#include <dirent.h>
#include <errno.h>
#include <stdint.h>
#include <stdarg.h>

#define MAX_HUBS             16
#define MAX_PORTS            16
#define TEST_MODE_COUNT      5
#define PATH_SIZE            512  /* 足够大，彻底安全 */
#define USB_PATH             "/dev/bus/usb"

// USB 标准定义
#define USB_CLASS_HUB		0x09
#define USB_DT_HUB		0x29
#define USB_REQ_GET_DESCRIPTOR	0x06

static const char *test_modes[TEST_MODE_COUNT] = {
	"Test_J", "Test_K", "Test_SE0_NAK", "Test_Packet", "Test_Force_Enable"
};

// 非HUB PHY 寄存器配置
struct usbc_reg {
	int ch;
	uint32_t addr;
};

static const struct usbc_reg phy_ports[] = {
	{1, 0xfe800440},
	{2, 0xfe900440},
	{3, 0xfea00440},
	{4, 0xfeb00440},
};

// HUB 信息结构体
struct hub_info {
	uint8_t bus;
	uint8_t addr;
	uint8_t port_count;
	uint16_t vid;
	uint16_t pid;
	char speed[16];
	char dev_path[PATH_SIZE];
};

/********************************************************************
 * 工具函数：执行系统命令
 ********************************************************************/
static int exec_cmd(const char *fmt, ...)
{
	char cmd[PATH_SIZE];
	va_list ap;
	va_start(ap, fmt);
	vsnprintf(cmd, sizeof(cmd), fmt, ap);
	va_end(ap);
	return system(cmd);
}

/********************************************************************
 * USB 控制请求（纯原生 ioctl）
 ********************************************************************/
static int usb_control(int fd, uint8_t reqtype, uint8_t req,
		uint16_t val, uint16_t idx, void *data, uint16_t len)
{
	struct usbdevfs_ctrltransfer ctrl = {
		.bRequestType = reqtype,
		.bRequest     = req,
		.wValue       = val,
		.wIndex       = idx,
		.wLength      = len,
		.data         = data,
	};
	return ioctl(fd, USBDEVFS_CONTROL, &ctrl);
}

/********************************************************************
 * 原生 USB HUB 扫描（核心：无 libusb / 无 sysfs / 无 debugfs）
 * 遍历 /dev/bus/usb -> 打开设备 -> 获取描述符 -> 识别 HUB
 ********************************************************************/
static int scan_usb_hubs_native(struct hub_info *hub_list)
{
	DIR *bus_dir, *dev_dir;
	struct dirent *bus_ent, *dev_ent;
	int hub_cnt = 0;

	memset(hub_list, 0, sizeof(struct hub_info) * MAX_HUBS);
	bus_dir = opendir(USB_PATH);
	if (!bus_dir) return -1;

	// 遍历总线目录：/dev/bus/usb/001 / 002 ...
	while ((bus_ent = readdir(bus_dir)) != NULL && hub_cnt < MAX_HUBS) {
		if (bus_ent->d_type != DT_DIR || atoi(bus_ent->d_name) == 0)
			continue;

		uint8_t bus_num = atoi(bus_ent->d_name);
		char bus_path[PATH_SIZE] = {0};

		/* 安全拼接路径，消除 GCC 截断警告 */
		strncpy(bus_path, USB_PATH, PATH_SIZE - 1);
		strncat(bus_path, "/", PATH_SIZE - strlen(bus_path) - 1);
		strncat(bus_path, bus_ent->d_name, PATH_SIZE - strlen(bus_path) - 1);

		dev_dir = opendir(bus_path);
		if (!dev_dir) continue;

		// 遍历设备文件：/dev/bus/usb/001/001 ...
		while ((dev_ent = readdir(dev_dir)) != NULL && hub_cnt < MAX_HUBS) {
			uint8_t dev_num = atoi(dev_ent->d_name);
			if (dev_num == 0) continue;

			char dev_path[PATH_SIZE] = {0};
			strncpy(dev_path, bus_path, PATH_SIZE - 1);
			strncat(dev_path, "/", PATH_SIZE - strlen(dev_path) - 1);
			strncat(dev_path, dev_ent->d_name, PATH_SIZE - strlen(dev_path) - 1);

			int fd = open(dev_path, O_RDWR);
			if (fd < 0) continue;

			// 1. 获取 USB 设备描述符
			uint8_t dev_desc[18];
			int ret = usb_control(fd, 0x80, USB_REQ_GET_DESCRIPTOR,
							0x0100, 0, dev_desc, 18);
			if (ret < 0) {
				close(fd);
				continue;
			}

			// 2. 判断是否为 HUB 设备（class = 0x09）
			if (dev_desc[4] != USB_CLASS_HUB) {
				close(fd);
				continue;
			}

			// 3. 填充 HUB 基本信息
			struct hub_info *hub = &hub_list[hub_cnt];
			hub->bus = bus_num;
			hub->addr = dev_num;
			hub->vid = (dev_desc[8]  << 8) | dev_desc[9];
			hub->pid = (dev_desc[10] << 8) | dev_desc[11];
			strncpy(hub->dev_path, dev_path, sizeof(hub->dev_path) - 1);

			// 4. 获取 HUB 描述符（获取端口数量）
			uint8_t hub_desc[9];
			ret = usb_control(fd, 0xA0, USB_REQ_GET_DESCRIPTOR,
							(USB_DT_HUB << 8), 0, hub_desc, 9);
			hub->port_count = (ret >= 8) ? hub_desc[2] : 1;

			// 5. 识别速度（简化版，可扩展）
			strcpy(hub->speed, "High-Speed");

			close(fd);
			hub_cnt++;
		}
		closedir(dev_dir);
	}
	closedir(bus_dir);
	return hub_cnt;
}

/********************************************************************
 * 非 HUB 模式测试
 ********************************************************************/
static void run_non_hub(int ch)
{
	uint32_t reg = 0;
	for (int i = 0; i < 4; i++) {
		if (phy_ports[i].ch == ch) {
			reg = phy_ports[i].addr;
			break;
		}
	}
	if (!reg) {
		printf("invalid channel\n");
		return;
	}

	printf("Start non-hub test channel %d...\n", ch);
	if (ch == 1) exec_cmd("echo on > /sys/usb-vbus/vbus");
	exec_cmd("devmem 0x%08x w 0x8000", reg);
	printf("Press q to stop\n");
	while (getchar() != 'q');
}

/********************************************************************
 * HUB 眼图测试主逻辑
 ********************************************************************/
static void run_hub_test(void)
{
	struct hub_info hubs[MAX_HUBS] = {0};
	int hub_cnt = scan_usb_hubs_native(hubs);

	if (hub_cnt <= 0) {
		printf("No USB HUB found!\n");
		return;
	}

	// 选择 HUB
	printf("\n=== USB HUB List ===\n");
	for (int i = 0; i < hub_cnt; i++) {
		printf("[%d] %s HUB | Bus:%03d Addr:%03d | Ports:%d | VID:%04X PID:%04X\n",
				i, hubs[i].speed, hubs[i].bus, hubs[i].addr,
				hubs[i].port_count, hubs[i].vid, hubs[i].pid);
	}

	int sel_hub;
	printf("Select HUB: ");
	if (scanf("%d", &sel_hub) != 1 || sel_hub < 0 || sel_hub >= hub_cnt) return;
	struct hub_info *h = &hubs[sel_hub];

	// 选择端口
	int sel_port;
	printf("Select port (1-%d): ", h->port_count);
	if (scanf("%d", &sel_port) != 1 || sel_port < 1 || sel_port > h->port_count) return;

	// 选择测试模式
	int test_mode;
	printf("\nTest Mode:\n");
	for (int i = 0; i < 5; i++) printf(" %d: %s\n", i + 1, test_modes[i]);
	printf("Select mode: ");
	if (scanf("%d", &test_mode) != 1 || test_mode < 1 || test_mode > 5) return;

	// 打开 USB 设备
	int fd = open(h->dev_path, O_RDWR);
	if (fd < 0) {
		perror("open");
		return;
	}

	// 挂起其他端口
	uint8_t status[4];
	for (int i = 1; i <= h->port_count; i++) {
		if (i == sel_port) continue;
		memset(status, 0, 4);
		usb_control(fd, 0xA3, 0x00, 0x0000, i, status, 4);
		if (status[0] & 0x02) {
			usb_control(fd, 0x23, 0x03, 0x0002, i, NULL, 0);
		}
	}

	// 启动测试模式
	usb_control(fd, 0x23, 0x03, 0x0015, (test_mode << 8) | sel_port, NULL, 0);
	printf("\nTest started: %s (Port %d)\n", test_modes[test_mode - 1], sel_port);
	printf("Press q to stop...\n");
	while (getchar() != 'q');

	// 复位并退出
	ioctl(fd, USBDEVFS_RESET, NULL);
	close(fd);
	printf("Test stopped\n");
}

/********************************************************************
 * 主函数
 ********************************************************************/
int main(int argc, char **argv)
{
	if (argc != 2) {
		printf("Usage:\n");
		printf("  %s hub      - USB HUB eye test\n", argv[0]);
		printf("  %s 1/2/3/4  - Non-hub PHY test\n", argv[0]);
		return -1;
	}

	if (getuid() != 0) {
		printf("Error: Must run as root!\n");
		return -1;
	}

	if (!strcmp(argv[1], "hub")) {
		run_hub_test();
	} else {
		run_non_hub(atoi(argv[1]));
	}

	return 0;
}

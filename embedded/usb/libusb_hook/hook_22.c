/* Copyright (C) 2026 wqshao All rights reserved.
 *
 *  File Name    : hook.c
 *  Author       : wqshao
 *  Created Time : 2026-03-19 11:40:32
 *  Description  :
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <pthread.h>
#include <dlfcn.h>
#if defined(__aarch64__)
	#include <libusb.h>
#else
	#include <libusb-1.0/libusb.h>
#endif

#define MAX_LOG_SIZE (5 * 1024 * 1024)  // 5MB
#define LOG_FILE0 "/tmp/libusb_hook0.log"
#define LOG_FILE1 "/tmp/libusb_hook1.log"

static pthread_mutex_t log_mutex = PTHREAD_MUTEX_INITIALIZER;
static int current_log_idx = 0;  // 0 for file0, 1 for file1

// 全局真实函数指针（用于避免递归）
static typeof(libusb_init) *real_libusb_init = NULL;
static typeof(libusb_set_option) *real_libusb_set_option = NULL;
static typeof(libusb_open_device_with_vid_pid) *real_libusb_open_device_with_vid_pid = NULL;
static typeof(libusb_get_device) *real_libusb_get_device = NULL;
static typeof(libusb_get_device_descriptor) *real_libusb_get_device_descriptor = NULL;
static typeof(libusb_get_config_descriptor) *real_libusb_get_config_descriptor = NULL;
static typeof(libusb_free_config_descriptor) *real_libusb_free_config_descriptor = NULL;
static typeof(libusb_detach_kernel_driver) *real_libusb_detach_kernel_driver = NULL;
static typeof(libusb_claim_interface) *real_libusb_claim_interface = NULL;
static typeof(libusb_release_interface) *real_libusb_release_interface = NULL;
static typeof(libusb_attach_kernel_driver) *real_libusb_attach_kernel_driver = NULL;
static typeof(libusb_reset_device) *real_libusb_reset_device = NULL;
static typeof(libusb_close) *real_libusb_close = NULL;
static typeof(libusb_exit) *real_libusb_exit = NULL;
static typeof(libusb_strerror) *real_libusb_strerror = NULL;

// 日志写入函数（线程安全，自动轮转）
static void log_write(const char *format, ...)
{
	va_list args;
	char buf[2048];
	int len;

	va_start(args, format);
	len = vsnprintf(buf, sizeof(buf), format, args);
	va_end(args);
	if (len < 0) return;
	if (len >= sizeof(buf)) {
		buf[sizeof(buf) - 1] = '\0';
		len = sizeof(buf) - 1;
	}

	pthread_mutex_lock(&log_mutex);

	const char *path = current_log_idx ? LOG_FILE1 : LOG_FILE0;
	FILE *fp = fopen(path, "a");
	if (fp) {
		fseek(fp, 0, SEEK_END);
		long size = ftell(fp);
		if (size + len > MAX_LOG_SIZE) {
			fclose(fp);
			current_log_idx = 1 - current_log_idx;
			path = current_log_idx ? LOG_FILE1 : LOG_FILE0;
			fp = fopen(path, "w");
			if (fp) {
				fwrite(buf, 1, len, fp);
				fclose(fp);
			} else {
				fprintf(stderr, "Failed to open log file: %s\n", path);
			}
		} else {
			fwrite(buf, 1, len, fp);
			fclose(fp);
		}
	} else {
		fprintf(stderr, "Failed to open log file: %s\n", path);
	}

	pthread_mutex_unlock(&log_mutex);
}

#if 0
// 辅助函数：打印 libusb 错误码名称
static const char *libusb_error_name(int err)
{
	switch (err) {
	case LIBUSB_SUCCESS:
		return "SUCCESS";
	case LIBUSB_ERROR_IO:
		return "IO";
	case LIBUSB_ERROR_INVALID_PARAM:
		return "INVALID_PARAM";
	case LIBUSB_ERROR_ACCESS:
		return "ACCESS";
	case LIBUSB_ERROR_NO_DEVICE:
		return "NO_DEVICE";
	case LIBUSB_ERROR_NOT_FOUND:
		return "NOT_FOUND";
	case LIBUSB_ERROR_BUSY:
		return "BUSY";
	case LIBUSB_ERROR_TIMEOUT:
		return "TIMEOUT";
	case LIBUSB_ERROR_OVERFLOW:
		return "OVERFLOW";
	case LIBUSB_ERROR_PIPE:
		return "PIPE";
	case LIBUSB_ERROR_INTERRUPTED:
		return "INTERRUPTED";
	case LIBUSB_ERROR_NO_MEM:
		return "NO_MEM";
	case LIBUSB_ERROR_NOT_SUPPORTED:
		return "NOT_SUPPORTED";
	case LIBUSB_ERROR_OTHER:
		return "OTHER";
	default:
		return "UNKNOWN";
	}
}
#endif

// 辅助函数：从 libusb_device 获取 VID/PID 前缀字符串
// 返回的字符串为 "[vid=0x%04x pid=0x%04x] " 或空字符串（获取失败时）
static void get_device_prefix(libusb_device *dev, char *prefix, size_t size)
{
	if (!dev) {
		prefix[0] = '\0';
		return;
	}
	struct libusb_device_descriptor desc;
	// 使用真实函数避免递归
	if (real_libusb_get_device_descriptor && real_libusb_get_device_descriptor(dev, &desc) == 0) {
		snprintf(prefix, size, "[vid=0x%04x pid=0x%04x] ", desc.idVendor, desc.idProduct);
	} else {
		prefix[0] = '\0';
	}
}

// 辅助函数：从 libusb_device_handle 获取 VID/PID 前缀
static void get_handle_prefix(libusb_device_handle *handle, char *prefix, size_t size)
{
	if (!handle) {
		prefix[0] = '\0';
		return;
	}
	libusb_device *dev = real_libusb_get_device ? real_libusb_get_device(handle) : NULL;
	get_device_prefix(dev, prefix, size);
}

// 构造函数：初始化真实函数指针并输出加载日志
__attribute__((constructor))
void libusb_hook_init(void)
{
	// 初始化所有真实函数指针
	real_libusb_init = dlsym(RTLD_NEXT, "libusb_init");
	real_libusb_set_option = dlsym(RTLD_NEXT, "libusb_set_option");
	real_libusb_open_device_with_vid_pid = dlsym(RTLD_NEXT, "libusb_open_device_with_vid_pid");
	real_libusb_get_device = dlsym(RTLD_NEXT, "libusb_get_device");
	real_libusb_get_device_descriptor = dlsym(RTLD_NEXT, "libusb_get_device_descriptor");
	real_libusb_get_config_descriptor = dlsym(RTLD_NEXT, "libusb_get_config_descriptor");
	real_libusb_free_config_descriptor = dlsym(RTLD_NEXT, "libusb_free_config_descriptor");
	real_libusb_detach_kernel_driver = dlsym(RTLD_NEXT, "libusb_detach_kernel_driver");
	real_libusb_claim_interface = dlsym(RTLD_NEXT, "libusb_claim_interface");
	real_libusb_release_interface = dlsym(RTLD_NEXT, "libusb_release_interface");
	real_libusb_attach_kernel_driver = dlsym(RTLD_NEXT, "libusb_attach_kernel_driver");
	real_libusb_reset_device = dlsym(RTLD_NEXT, "libusb_reset_device");
	real_libusb_close = dlsym(RTLD_NEXT, "libusb_close");
	real_libusb_exit = dlsym(RTLD_NEXT, "libusb_exit");
	real_libusb_strerror = dlsym(RTLD_NEXT, "libusb_strerror");

	log_write("libusb_hook.so 已加载 ✅\n");
}

// 1. libusb_init
int libusb_init(libusb_context **ctx)
{
	int ret = real_libusb_init(ctx);
	log_write("libusb_init(ctx=%p) -> %d (%s)\n", ctx, ret, libusb_error_name(ret));
	return ret;
}

// 2. libusb_set_option
int libusb_set_option(libusb_context *ctx, enum libusb_option option, ...)
{
	va_list args;
	va_start(args, option);
	int val = va_arg(args, int);
	va_end(args);
	int ret = real_libusb_set_option(ctx, option, val);
	log_write("libusb_set_option(ctx=%p, option=%d, val=%d) -> %d (%s)\n", ctx, option, val, ret, libusb_error_name(ret));
	return ret;
}

// 3. libusb_open_device_with_vid_pid
libusb_device_handle *libusb_open_device_with_vid_pid(libusb_context *ctx, uint16_t vid, uint16_t pid)
{
	libusb_device_handle *handle = real_libusb_open_device_with_vid_pid(ctx, vid, pid);
	// 调用行：可以直接使用传入的 vid/pid 作为前缀
	log_write("[vid=0x%04x pid=0x%04x] libusb_open_device_with_vid_pid(ctx=%p) -> handle=%p\n", vid, pid, ctx, handle);
	return handle;
}

// 4. libusb_get_device
libusb_device *libusb_get_device(libusb_device_handle *handle)
{
	libusb_device *dev = real_libusb_get_device(handle);
	char prefix[64];
	get_handle_prefix(handle, prefix, sizeof(prefix));
	log_write("%slibusb_get_device(handle=%p) -> dev=%p\n", prefix, handle, dev);
	return dev;
}

// 5. libusb_get_device_descriptor
int libusb_get_device_descriptor(libusb_device *dev, struct libusb_device_descriptor *desc)
{
	int ret = real_libusb_get_device_descriptor(dev, desc);
	char prefix[64];
	get_device_prefix(dev, prefix, sizeof(prefix));
	log_write("%slibusb_get_device_descriptor(dev=%p, desc=%p) -> %d (%s)\n", prefix, dev, desc, ret, libusb_error_name(ret));
	if (ret == 0 && desc) {
		log_write("  -> desc: bcdUSB=0x%04x, bDeviceClass=0x%02x, idVendor=0x%04x, idProduct=0x%04x, bcdDevice=0x%04x\n",
				desc->bcdUSB, desc->bDeviceClass, desc->idVendor, desc->idProduct, desc->bcdDevice);
	}
	return ret;
}

// 6. libusb_get_config_descriptor
int libusb_get_config_descriptor(libusb_device *dev, uint8_t config_index, struct libusb_config_descriptor **config)
{
	int ret = real_libusb_get_config_descriptor(dev, config_index, config);
	char prefix[64];
	get_device_prefix(dev, prefix, sizeof(prefix));
	log_write("%slibusb_get_config_descriptor(dev=%p, config_index=%u, config=%p) -> %d (%s)\n", prefix, dev, config_index, config, ret, libusb_error_name(ret));
	if (ret == 0 && config && *config) {
		log_write("  -> config: bNumInterfaces=%u, bConfigurationValue=%u, MaxPower=%u\n",
				(*config)->bNumInterfaces, (*config)->bConfigurationValue, (*config)->MaxPower);
	}
	return ret;
}

// 7. libusb_free_config_descriptor
void libusb_free_config_descriptor(struct libusb_config_descriptor *config)
{
	real_libusb_free_config_descriptor(config);
	log_write("libusb_free_config_descriptor(config=%p)\n", config);
}

// 8. libusb_detach_kernel_driver
int libusb_detach_kernel_driver(libusb_device_handle *handle, int interface_number)
{
	int ret = real_libusb_detach_kernel_driver(handle, interface_number);
	char prefix[64];
	get_handle_prefix(handle, prefix, sizeof(prefix));
	log_write("%slibusb_detach_kernel_driver(handle=%p, interface_number=%d) -> %d (%s)\n", prefix, handle, interface_number, ret, libusb_error_name(ret));
	return ret;
}

// 9. libusb_claim_interface
int libusb_claim_interface(libusb_device_handle *handle, int interface_number)
{
	int ret = real_libusb_claim_interface(handle, interface_number);
	char prefix[64];
	get_handle_prefix(handle, prefix, sizeof(prefix));
	log_write("%slibusb_claim_interface(handle=%p, interface_number=%d) -> %d (%s)\n", prefix, handle, interface_number, ret, libusb_error_name(ret));
	return ret;
}

// 10. libusb_reset_device
int libusb_reset_device(libusb_device_handle *handle)
{
	int ret = real_libusb_reset_device(handle);
	char prefix[64];
	get_handle_prefix(handle, prefix, sizeof(prefix));
	log_write("%slibusb_reset_device(handle=%p) -> %d (%s)\n", prefix, handle, ret, libusb_error_name(ret));
	return ret;
}

// 11. libusb_release_interface
int libusb_release_interface(libusb_device_handle *handle, int interface_number)
{
	int ret = real_libusb_release_interface(handle, interface_number);
	char prefix[64];
	get_handle_prefix(handle, prefix, sizeof(prefix));
	log_write("%slibusb_release_interface(handle=%p, interface_number=%d) -> %d (%s)\n", prefix, handle, interface_number, ret, libusb_error_name(ret));
	return ret;
}

// 12. libusb_attach_kernel_driver
int libusb_attach_kernel_driver(libusb_device_handle *handle, int interface_number)
{
	int ret = real_libusb_attach_kernel_driver(handle, interface_number);
	char prefix[64];
	get_handle_prefix(handle, prefix, sizeof(prefix));
	log_write("%slibusb_attach_kernel_driver(handle=%p, interface_number=%d) -> %d (%s)\n", prefix, handle, interface_number, ret, libusb_error_name(ret));
	return ret;
}

// 13. libusb_close
void libusb_close(libusb_device_handle *handle)
{
	char prefix[64];
	get_handle_prefix(handle, prefix, sizeof(prefix));
	real_libusb_close(handle);
	log_write("%slibusb_close(handle=%p)\n", prefix, handle);
}

// 14. libusb_exit
void libusb_exit(libusb_context *ctx)
{
	real_libusb_exit(ctx);
	log_write("libusb_exit(ctx=%p)\n", ctx);
}

// 15. libusb_strerror
const char *libusb_strerror(int errcode)
{
	const char *s = real_libusb_strerror(errcode);
	log_write("libusb_strerror(errcode=%d) -> \"%s\"\n", errcode, s);
	return s;
}

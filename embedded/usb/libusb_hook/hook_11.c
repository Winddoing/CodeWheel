/* Copyright (C) 2026 wqshao All rights reserved.
 *
 *  File Name    : hook.c
 *  Author       : wqshao
 *  Created Time : 2026-03-19 11:31:21
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

// 日志写入函数（线程安全，自动轮转）
static void log_write(const char *format, ...)
{
	va_list args;
	char buf[2048];  // 足够容纳单条日志
	int len;

	va_start(args, format);
	len = vsnprintf(buf, sizeof(buf), format, args);
	va_end(args);
	if (len < 0) return;
	if (len >= sizeof(buf)) {
		// 截断，但 buf 已包含部分内容
		buf[sizeof(buf) - 1] = '\0';
		len = sizeof(buf) - 1;
	}

	pthread_mutex_lock(&log_mutex);

	const char *path = current_log_idx ? LOG_FILE1 : LOG_FILE0;

	// 以追加模式打开，获取当前大小
	FILE *fp = fopen(path, "a");
	if (fp) {
		fseek(fp, 0, SEEK_END);
		long size = ftell(fp);
		// 如果加上新消息会超过限制，切换文件
		if (size + len > MAX_LOG_SIZE) {
			fclose(fp);
			// 切换到另一个文件
			current_log_idx = 1 - current_log_idx;
			path = current_log_idx ? LOG_FILE1 : LOG_FILE0;
			// 以写模式打开（清空）
			fp = fopen(path, "w");
			if (fp) {
				fwrite(buf, 1, len, fp);
				fclose(fp);
			} else {
				// 打开失败，尝试输出到 stderr 作为后备
				fprintf(stderr, "Failed to open log file: %s\n", path);
			}
		} else {
			// 未超限，直接追加
			fwrite(buf, 1, len, fp);
			fclose(fp);
		}
	} else {
		// 打开失败，尝试 stderr
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

// 辅助宏：获取真实函数指针
#define GET_REAL(name) \
	static typeof(name) *real_##name = NULL; \
	if (!real_##name) { \
		real_##name = (typeof(name)*) dlsym(RTLD_NEXT, #name); \
		if (!real_##name) { \
			fprintf(stderr, "dlsym error: %s\n", dlerror()); \
		} \
	}

// 构造函数：输出一条加载消息到日志
__attribute__((constructor))
void libusb_hook_init(void)
{
	log_write("libusb_hook.so 已加载 ✅\n");
}

// 1. libusb_init
int libusb_init(libusb_context **ctx)
{
	GET_REAL(libusb_init);
	log_write("libusb_init(ctx=%p)\n", ctx);
	int ret = real_libusb_init(ctx);
	log_write("  -> %d (%s)\n", ret, libusb_error_name(ret));
	return ret;
}

// 2. libusb_set_option
int libusb_set_option(libusb_context *ctx, enum libusb_option option, ...)
{
	GET_REAL(libusb_set_option);
	va_list args;
	va_start(args, option);
	int val = va_arg(args, int);
	va_end(args);
	log_write("libusb_set_option(ctx=%p, option=%d, val=%d)\n", ctx, option, val);
	int ret = real_libusb_set_option(ctx, option, val);
	log_write("  -> %d (%s)\n", ret, libusb_error_name(ret));
	return ret;
}

// 3. libusb_open_device_with_vid_pid
libusb_device_handle *libusb_open_device_with_vid_pid(libusb_context *ctx, uint16_t vid, uint16_t pid)
{
	GET_REAL(libusb_open_device_with_vid_pid);
	log_write("libusb_open_device_with_vid_pid(ctx=%p, vid=0x%04x, pid=0x%04x)\n", ctx, vid, pid);
	libusb_device_handle *handle = real_libusb_open_device_with_vid_pid(ctx, vid, pid);
	log_write("  -> handle=%p\n", handle);
	return handle;
}

// 4. libusb_get_device
libusb_device *libusb_get_device(libusb_device_handle *handle)
{
	GET_REAL(libusb_get_device);
	log_write("libusb_get_device(handle=%p)\n", handle);
	libusb_device *dev = real_libusb_get_device(handle);
	log_write("  -> dev=%p\n", dev);
	return dev;
}

// 5. libusb_get_device_descriptor
int libusb_get_device_descriptor(libusb_device *dev, struct libusb_device_descriptor *desc)
{
	GET_REAL(libusb_get_device_descriptor);
	log_write("libusb_get_device_descriptor(dev=%p, desc=%p)\n", dev, desc);
	int ret = real_libusb_get_device_descriptor(dev, desc);
	if (ret == 0 && desc) {
		log_write("  -> desc: bcdUSB=0x%04x, bDeviceClass=0x%02x, idVendor=0x%04x, idProduct=0x%04x, bcdDevice=0x%04x\n",
				desc->bcdUSB, desc->bDeviceClass, desc->idVendor, desc->idProduct, desc->bcdDevice);
	}
	log_write("  -> %d (%s)\n", ret, libusb_error_name(ret));
	return ret;
}

// 6. libusb_get_config_descriptor
int libusb_get_config_descriptor(libusb_device *dev, uint8_t config_index, struct libusb_config_descriptor **config)
{
	GET_REAL(libusb_get_config_descriptor);
	log_write("libusb_get_config_descriptor(dev=%p, config_index=%u, config=%p)\n", dev, config_index, config);
	int ret = real_libusb_get_config_descriptor(dev, config_index, config);
	if (ret == 0 && config && *config) {
		log_write("  -> config: bNumInterfaces=%u, bConfigurationValue=%u, MaxPower=%u\n",
				(*config)->bNumInterfaces, (*config)->bConfigurationValue, (*config)->MaxPower);
	}
	log_write("  -> %d (%s)\n", ret, libusb_error_name(ret));
	return ret;
}

// 7. libusb_free_config_descriptor
void libusb_free_config_descriptor(struct libusb_config_descriptor *config)
{
	GET_REAL(libusb_free_config_descriptor);
	log_write("libusb_free_config_descriptor(config=%p)\n", config);
	real_libusb_free_config_descriptor(config);
}

// 8. libusb_detach_kernel_driver
int libusb_detach_kernel_driver(libusb_device_handle *handle, int interface_number)
{
	GET_REAL(libusb_detach_kernel_driver);
	log_write("libusb_detach_kernel_driver(handle=%p, interface_number=%d)\n", handle, interface_number);
	int ret = real_libusb_detach_kernel_driver(handle, interface_number);
	log_write("  -> %d (%s)\n", ret, libusb_error_name(ret));
	return ret;
}

// 9. libusb_claim_interface
int libusb_claim_interface(libusb_device_handle *handle, int interface_number)
{
	GET_REAL(libusb_claim_interface);
	log_write("libusb_claim_interface(handle=%p, interface_number=%d)\n", handle, interface_number);
	int ret = real_libusb_claim_interface(handle, interface_number);
	log_write("  -> %d (%s)\n", ret, libusb_error_name(ret));
	return ret;
}

// 10. libusb_reset_device
int libusb_reset_device(libusb_device_handle *handle)
{
	GET_REAL(libusb_reset_device);
	log_write("libusb_reset_device(handle=%p)\n", handle);
	int ret = real_libusb_reset_device(handle);
	log_write("  -> %d (%s)\n", ret, libusb_error_name(ret));
	return ret;
}

// 11. libusb_release_interface
int libusb_release_interface(libusb_device_handle *handle, int interface_number)
{
	GET_REAL(libusb_release_interface);
	log_write("libusb_release_interface(handle=%p, interface_number=%d)\n", handle, interface_number);
	int ret = real_libusb_release_interface(handle, interface_number);
	log_write("  -> %d (%s)\n", ret, libusb_error_name(ret));
	return ret;
}

// 12. libusb_attach_kernel_driver
int libusb_attach_kernel_driver(libusb_device_handle *handle, int interface_number)
{
	GET_REAL(libusb_attach_kernel_driver);
	log_write("libusb_attach_kernel_driver(handle=%p, interface_number=%d)\n", handle, interface_number);
	int ret = real_libusb_attach_kernel_driver(handle, interface_number);
	log_write("  -> %d (%s)\n", ret, libusb_error_name(ret));
	return ret;
}

// 13. libusb_close
void libusb_close(libusb_device_handle *handle)
{
	GET_REAL(libusb_close);
	log_write("libusb_close(handle=%p)\n", handle);
	real_libusb_close(handle);
}

// 14. libusb_exit
void libusb_exit(libusb_context *ctx)
{
	GET_REAL(libusb_exit);
	log_write("libusb_exit(ctx=%p)\n", ctx);
	real_libusb_exit(ctx);
}

// 15. libusb_strerror
const char *libusb_strerror(int errcode)
{
	GET_REAL(libusb_strerror);
	log_write("libusb_strerror(errcode=%d)\n", errcode);
	const char *s = real_libusb_strerror(errcode);
	log_write("  -> \"%s\"\n", s);
	return s;
}

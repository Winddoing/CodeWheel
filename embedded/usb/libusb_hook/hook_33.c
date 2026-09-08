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
#include <sys/time.h>   // 新增：gettimeofday
#include <time.h>       // 新增：localtime_r, strftime
#include <unistd.h>          // 新增：getpid
#include <limits.h>          // 新增：PATH_MAX
#include <libgen.h>          // 新增：basename
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

#ifdef __GNUC__
	#define ATTR_PRINTF(fmt, arg) __attribute__((format(printf, fmt, arg)))
#else
	#define ATTR_PRINTF(fmt, arg)
#endif
// 日志写入函数（线程安全，自动轮转，带时间戳）
static void log_write(const char *format, ...) ATTR_PRINTF(1, 2);
static void log_write(const char *format, ...)
{
	va_list args;
	char msg_buf[2048];               // 原始消息缓冲区
	char final_buf[2048 + 64];         // 时间戳 + 原始消息
	int msg_len;

	// 1. 生成原始消息
	va_start(args, format);
	msg_len = vsnprintf(msg_buf, sizeof(msg_buf), format, args);
	va_end(args);
	if (msg_len < 0) return;
	if (msg_len >= sizeof(msg_buf)) {
		msg_buf[sizeof(msg_buf) - 1] = '\0';
		msg_len = sizeof(msg_buf) - 1;
	}

	// 2. 生成时间戳：年月日时分秒.微秒
	struct timeval tv;
	struct tm tm;
	char timestamp[64];
	gettimeofday(&tv, NULL);
	localtime_r(&tv.tv_sec, &tm);      // 线程安全
	strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", &tm);
	// 追加微秒和空格
	snprintf(timestamp + strlen(timestamp), sizeof(timestamp) - strlen(timestamp),
			".%06ld ", tv.tv_usec);

	// 3. 组合最终消息
	int final_len = snprintf(final_buf, sizeof(final_buf), "%s%s", timestamp, msg_buf);
	if (final_len >= sizeof(final_buf)) {
		final_buf[sizeof(final_buf) - 1] = '\0';
		final_len = sizeof(final_buf) - 1;
	}

	// 4. 写入文件（循环写入）
	pthread_mutex_lock(&log_mutex);

	const char *path = current_log_idx ? LOG_FILE1 : LOG_FILE0;
	FILE *fp = fopen(path, "a");
	if (fp) {
		fseek(fp, 0, SEEK_END);
		long size = ftell(fp);
		if (size + final_len > MAX_LOG_SIZE) {
			fclose(fp);
			// 切换到另一个文件并清空
			current_log_idx = 1 - current_log_idx;
			path = current_log_idx ? LOG_FILE1 : LOG_FILE0;
			fp = fopen(path, "w");
			if (fp) {
				fwrite(final_buf, 1, final_len, fp);
				fclose(fp);
			} else {
				fprintf(stderr, "Failed to open log file: %s\n", path);
			}
		} else {
			fwrite(final_buf, 1, final_len, fp);
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

// GCC（及兼容编译器）的扩展属性，用于标记一个函数为共享库的初始化函数。当共享库被加载时，这些构造函数会自动执行（在 main 之前）。
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

	// 获取当前进程信息
	pid_t pid = getpid();
	char exe_path[PATH_MAX];
	char proc_link[64];
	snprintf(proc_link, sizeof(proc_link), "/proc/%d/exe", pid);
	ssize_t len = readlink(proc_link, exe_path, sizeof(exe_path) - 1);
	if (len != -1) {
		exe_path[len] = '\0';
	} else {
		strcpy(exe_path, "unknown");
	}
	char *progname = basename(exe_path);  // 提取进程名（可执行文件名）

	// 获取本共享库的路径
	Dl_info info;
	const char *libpath = "unknown";
	if (dladdr(libusb_hook_init, &info) && info.dli_fname) {
		libpath = info.dli_fname;
	}

	log_write("libusb_hook.so 已加载 ✅ 进程[pid=%d, name=%s] 库路径=%s\n", pid, progname, libpath);
}

// 1. libusb_init
int libusb_init(libusb_context **ctx)
{
	int ret = real_libusb_init(ctx);
	log_write("libusb_init(ctx=%p(%p)) -> %d (%s)\n", ctx, *ctx, ret, libusb_error_name(ret));
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
		log_write("%s  -> desc: bcdUSB=0x%04x, bDeviceClass=0x%02x, idVendor=0x%04x, idProduct=0x%04x, bcdDevice=0x%04x\n", prefix,
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
	log_write("%slibusb_get_config_descriptor(dev=%p, config_index=%u, config=%p(%p)) -> %d (%s)\n", prefix, dev, config_index, config, *config, ret, libusb_error_name(ret));
	if (ret == 0 && config && *config) {
		log_write("%s  -> config: bNumInterfaces=%u, bConfigurationValue=%u, MaxPower=%u\n", prefix,
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

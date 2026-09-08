/* Copyright (C) 2026 wqshao All rights reserved.
 *
 *  File Name    : hook.c
 *  Author       : wqshao
 *  Created Time : 2026-03-21 11:48:06
 *  Description  :
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <pthread.h>
#include <dlfcn.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <limits.h>
#include <libgen.h>
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

// ========== 设备状态管理 ==========
struct device_state {
	char key[256];                // 设备标识（BDF+VID/PID）
	char last_api[64];            // 上次调用的API名称
	int count;                    // 当前连续调用次数
	struct device_state *next;
};

static struct device_state *device_states = NULL;
static pthread_mutex_t state_mutex = PTHREAD_MUTEX_INITIALIZER;

// ========== 全局真实函数指针声明 ==========
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
//static typeof(libusb_strerror) *real_libusb_strerror = NULL;

// 新增函数的真实指针
//static typeof(libusb_alloc_transfer) *real_libusb_alloc_transfer = NULL;
static typeof(libusb_bulk_transfer) *real_libusb_bulk_transfer = NULL;
static typeof(libusb_cancel_transfer) *real_libusb_cancel_transfer = NULL;
static typeof(libusb_clear_halt) *real_libusb_clear_halt = NULL;
static typeof(libusb_control_transfer) *real_libusb_control_transfer = NULL;
static typeof(libusb_get_active_config_descriptor) *real_libusb_get_active_config_descriptor = NULL;
static typeof(libusb_get_bos_descriptor) *real_libusb_get_bos_descriptor = NULL;
static typeof(libusb_get_bus_number) *real_libusb_get_bus_number = NULL;
static typeof(libusb_get_configuration) *real_libusb_get_configuration = NULL;
static typeof(libusb_get_device_address) *real_libusb_get_device_address = NULL;
static typeof(libusb_get_device_list) *real_libusb_get_device_list = NULL;
static typeof(libusb_get_device_speed) *real_libusb_get_device_speed = NULL;
static typeof(libusb_get_parent) *real_libusb_get_parent = NULL;
static typeof(libusb_get_port_number) *real_libusb_get_port_number = NULL;
static typeof(libusb_get_port_numbers) *real_libusb_get_port_numbers = NULL;
static typeof(libusb_get_ss_usb_device_capability_descriptor) *real_libusb_get_ss_usb_device_capability_descriptor = NULL;
static typeof(libusb_get_string_descriptor_ascii) *real_libusb_get_string_descriptor_ascii = NULL;
static typeof(libusb_get_version) *real_libusb_get_version = NULL;
static typeof(libusb_handle_events) *real_libusb_handle_events = NULL;
static typeof(libusb_handle_events_locked) *real_libusb_handle_events_locked = NULL;
//static typeof(libusb_handle_events_timeout_completed) *real_libusb_handle_events_timeout_completed = NULL;
//static typeof(libusb_has_capability) *real_libusb_has_capability = NULL;
static typeof(libusb_hotplug_register_callback) *real_libusb_hotplug_register_callback = NULL;
static typeof(libusb_interrupt_transfer) *real_libusb_interrupt_transfer = NULL;
static typeof(libusb_kernel_driver_active) *real_libusb_kernel_driver_active = NULL;
static typeof(libusb_open) *real_libusb_open = NULL;
static typeof(libusb_set_configuration) *real_libusb_set_configuration = NULL;
static typeof(libusb_set_interface_alt_setting) *real_libusb_set_interface_alt_setting = NULL;
static typeof(libusb_setlocale) *real_libusb_setlocale = NULL;
static typeof(libusb_submit_transfer) *real_libusb_submit_transfer = NULL;
static typeof(libusb_wait_for_event) *real_libusb_wait_for_event = NULL;
static typeof(libusb_wrap_sys_device) *real_libusb_wrap_sys_device = NULL;

// 构造函数：初始化函数指针
__attribute__((constructor))
void libusb_hook_init(void)
{
	// 初始化原有函数指针
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
	//real_libusb_strerror = dlsym(RTLD_NEXT, "libusb_strerror");

	// 初始化新增函数指针
	//real_libusb_alloc_transfer = dlsym(RTLD_NEXT, "libusb_alloc_transfer");
	real_libusb_bulk_transfer = dlsym(RTLD_NEXT, "libusb_bulk_transfer");
	real_libusb_cancel_transfer = dlsym(RTLD_NEXT, "libusb_cancel_transfer");
	real_libusb_clear_halt = dlsym(RTLD_NEXT, "libusb_clear_halt");
	real_libusb_control_transfer = dlsym(RTLD_NEXT, "libusb_control_transfer");
	real_libusb_get_active_config_descriptor = dlsym(RTLD_NEXT, "libusb_get_active_config_descriptor");
	real_libusb_get_bos_descriptor = dlsym(RTLD_NEXT, "libusb_get_bos_descriptor");
	real_libusb_get_bus_number = dlsym(RTLD_NEXT, "libusb_get_bus_number");
	real_libusb_get_configuration = dlsym(RTLD_NEXT, "libusb_get_configuration");
	real_libusb_get_device_address = dlsym(RTLD_NEXT, "libusb_get_device_address");
	real_libusb_get_device_list = dlsym(RTLD_NEXT, "libusb_get_device_list");
	real_libusb_get_device_speed = dlsym(RTLD_NEXT, "libusb_get_device_speed");
	real_libusb_get_parent = dlsym(RTLD_NEXT, "libusb_get_parent");
	real_libusb_get_port_number = dlsym(RTLD_NEXT, "libusb_get_port_number");
	real_libusb_get_port_numbers = dlsym(RTLD_NEXT, "libusb_get_port_numbers");
	real_libusb_get_ss_usb_device_capability_descriptor = dlsym(RTLD_NEXT, "libusb_get_ss_usb_device_capability_descriptor");
	real_libusb_get_string_descriptor_ascii = dlsym(RTLD_NEXT, "libusb_get_string_descriptor_ascii");
	real_libusb_get_version = dlsym(RTLD_NEXT, "libusb_get_version");
	real_libusb_handle_events = dlsym(RTLD_NEXT, "libusb_handle_events");
	real_libusb_handle_events_locked = dlsym(RTLD_NEXT, "libusb_handle_events_locked");
	//real_libusb_handle_events_timeout_completed = dlsym(RTLD_NEXT, "libusb_handle_events_timeout_completed");
	//real_libusb_has_capability = dlsym(RTLD_NEXT, "libusb_has_capability");
	real_libusb_hotplug_register_callback = dlsym(RTLD_NEXT, "libusb_hotplug_register_callback");
	real_libusb_interrupt_transfer = dlsym(RTLD_NEXT, "libusb_interrupt_transfer");
	real_libusb_kernel_driver_active = dlsym(RTLD_NEXT, "libusb_kernel_driver_active");
	real_libusb_open = dlsym(RTLD_NEXT, "libusb_open");
	real_libusb_set_configuration = dlsym(RTLD_NEXT, "libusb_set_configuration");
	real_libusb_set_interface_alt_setting = dlsym(RTLD_NEXT, "libusb_set_interface_alt_setting");
	real_libusb_setlocale = dlsym(RTLD_NEXT, "libusb_setlocale");
	real_libusb_submit_transfer = dlsym(RTLD_NEXT, "libusb_submit_transfer");
	real_libusb_wait_for_event = dlsym(RTLD_NEXT, "libusb_wait_for_event");
	real_libusb_wrap_sys_device = dlsym(RTLD_NEXT, "libusb_wrap_sys_device");
}

// ========== 原有日志函数（不变） ==========
#ifdef __GNUC__
	#define ATTR_PRINTF(fmt, arg) __attribute__((format(printf, fmt, arg)))
#else
	#define ATTR_PRINTF(fmt, arg)
#endif

static void log_write(const char *format, ...) ATTR_PRINTF(1, 2);
static void log_write(const char *format, ...)
{
	va_list args;
	char msg_buf[2048];
	char final_buf[2048 + 64];
	int msg_len;

	va_start(args, format);
	msg_len = vsnprintf(msg_buf, sizeof(msg_buf), format, args);
	va_end(args);
	if (msg_len < 0) return;
	if (msg_len >= sizeof(msg_buf)) {
		msg_buf[sizeof(msg_buf) - 1] = '\0';
		msg_len = sizeof(msg_buf) - 1;
	}

	struct timeval tv;
	struct tm tm;
	char timestamp[64];
	gettimeofday(&tv, NULL);
	localtime_r(&tv.tv_sec, &tm);
	strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", &tm);
	snprintf(timestamp + strlen(timestamp), sizeof(timestamp) - strlen(timestamp),
			".%06ld ", tv.tv_usec);

	int final_len = snprintf(final_buf, sizeof(final_buf), "%s%s", timestamp, msg_buf);
	if (final_len >= sizeof(final_buf)) {
		final_buf[sizeof(final_buf) - 1] = '\0';
		final_len = sizeof(final_buf) - 1;
	}

	pthread_mutex_lock(&log_mutex);

	// 检查当前索引对应的文件是否存在，若不存在则重置索引为0
	const char *path = current_log_idx ? LOG_FILE1 : LOG_FILE0;
	FILE *check = fopen(path, "r");
	if (!check) {
		// 文件不存在，重置索引为0
		current_log_idx = 0;
		path = LOG_FILE0;
	} else {
		fclose(check);
	}

	FILE *fp = fopen(path, "a");
	if (fp) {
		fseek(fp, 0, SEEK_END);
		long size = ftell(fp);
		if (size + final_len > MAX_LOG_SIZE) {
			fclose(fp);
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

// 辅助宏：根据返回值生成错误字符串（仅当 ret<0 时添加错误名称）
#define FORMAT_RET(ret, buf, size) \
	do { \
		if ((ret) < 0) { \
			snprintf((buf), (size), "%d (%s)", (ret), libusb_error_name(ret)); \
		} else { \
			snprintf((buf), (size), "%d", (ret)); \
		} \
	} while(0)

// 查找或创建设备状态
static struct device_state *get_device_state(const char *key)
{
	pthread_mutex_lock(&state_mutex);
	struct device_state *state = device_states;
	while (state) {
		if (strcmp(state->key, key) == 0) {
			pthread_mutex_unlock(&state_mutex);
			return state;
		}
		state = state->next;
	}
	// 创建新状态
	state = calloc(1, sizeof(struct device_state));
	if (state) {
		strncpy(state->key, key, sizeof(state->key) - 1);
		state->last_api[0] = '\0';
		state->count = 0;
		state->next = device_states;
		device_states = state;
	}
	pthread_mutex_unlock(&state_mutex);
	return state;
}

// 更新设备状态（返回是否需要输出本次调用日志）
static int update_device_state(struct device_state *state, const char *api_name)
{
	if (!state) return 1; // 无状态，直接输出

	pthread_mutex_lock(&state_mutex);
	int output = 1;
	if (state->last_api[0] == '\0') {
		// 首次调用该设备
		strncpy(state->last_api, api_name, sizeof(state->last_api) - 1);
		state->count = 1;
		output = 1;
	} else if (strcmp(state->last_api, api_name) == 0) {
		// 相同 API，仅增加计数
		state->count++;
		output = 0; // 不输出本次日志
	} else {
		// 不同 API：先输出上一个 API 的统计，再开始新的
		if (state->count != 1) {
			//如果只调用了一次就不输出这个日志
			char summary[512];
			snprintf(summary, sizeof(summary),
					"%s previous API <%s> was called %d times\n",
					state->key, state->last_api, state->count);
			log_write("%s", summary); // 直接写日志
		}
		// 重置状态
		strncpy(state->last_api, api_name, sizeof(state->last_api) - 1);
		state->count = 1;
		output = 1;
	}
	pthread_mutex_unlock(&state_mutex);
	return output;
}

// 带去重功能的日志输出
static void log_api_call(const char *device_prefix, const char *api_name,
		const char *fmt, ...)
{
	if (!device_prefix || device_prefix[0] == '\0') {
		// 无设备前缀，直接输出（不参与去重）
		va_list args;
		va_start(args, fmt);
		char buf[2048];
		vsnprintf(buf, sizeof(buf), fmt, args);
		va_end(args);
		log_write("%s", buf);
		return;
	}

	// 查找或创建设备状态
	struct device_state *state = get_device_state(device_prefix);
	if (!state) {
		// 状态分配失败，降级为直接输出
		va_list args;
		va_start(args, fmt);
		char buf[2048];
		vsnprintf(buf, sizeof(buf), fmt, args);
		va_end(args);
		log_write("%s", buf);
		return;
	}

	// 更新状态，决定是否输出本次日志
	int need_output = update_device_state(state, api_name);
	if (need_output) {
		va_list args;
		va_start(args, fmt);
		char buf[2048];
		vsnprintf(buf, sizeof(buf), fmt, args);
		va_end(args);
		// 自动添加设备前缀
		char final_buf[2048 + 128];
		snprintf(final_buf, sizeof(final_buf), "%s%s", device_prefix, buf);
		log_write("%s", final_buf);
	}
}

#if 0
static void free_device_states(void)
{
	pthread_mutex_lock(&state_mutex);
	struct device_state *state = device_states;
	while (state) {
		struct device_state *next = state->next;
		free(state);
		state = next;
	}
	device_states = NULL;
	pthread_mutex_unlock(&state_mutex);
}

__attribute__((destructor))
static void libusb_hook_cleanup(void)
{
	free_device_states();
}
#endif

// ========== 辅助函数（获取设备标识） ==========
static void get_device_prefix(libusb_device *dev, char *prefix, size_t size);
static void get_handle_prefix(libusb_device_handle *handle, char *prefix, size_t size);

// 从设备描述符获取 BDF + VID/PID 标识
static void get_device_prefix(libusb_device *dev, char *prefix, size_t size)
{
	if (!dev) {
		prefix[0] = '\0';
		return;
	}

	struct libusb_device_descriptor desc;
	char bdf_str[64] = "";

	// 需要提前初始化这些函数指针（在构造函数中）
	uint8_t bus = real_libusb_get_bus_number ? real_libusb_get_bus_number(dev) : 0;
	uint8_t ports[8];
	int port_count = 0;
	if (real_libusb_get_port_numbers) {
		port_count = real_libusb_get_port_numbers(dev, ports, sizeof(ports));
	}

	if (port_count > 0) {
		char *p = bdf_str;
		p += sprintf(p, "%u", bus);
		for (int i = 0; i < port_count; i++) {
			p += sprintf(p, "-%u", ports[i]);
		}
	} else {
		uint8_t addr = real_libusb_get_device_address ? real_libusb_get_device_address(dev) : 0;
		snprintf(bdf_str, sizeof(bdf_str), "%u-%u", bus, addr);
	}

	if (real_libusb_get_device_descriptor && real_libusb_get_device_descriptor(dev, &desc) == 0) {
		snprintf(prefix, size, "[%s vid=0x%04x pid=0x%04x] ", bdf_str, desc.idVendor, desc.idProduct);
	} else {
		snprintf(prefix, size, "[%s vid=unknown pid=unknown] ", bdf_str);
	}
}

static void get_handle_prefix(libusb_device_handle *handle, char *prefix, size_t size)
{
	if (!handle) {
		prefix[0] = '\0';
		return;
	}
	libusb_device *dev = real_libusb_get_device ? real_libusb_get_device(handle) : NULL;
	get_device_prefix(dev, prefix, size);
}

// ==================== 钩子函数实现 ====================
int libusb_init(libusb_context **ctx)
{
	int ret = real_libusb_init(ctx);
	char ret_str[64];
	FORMAT_RET(ret, ret_str, sizeof(ret_str));
	log_write("libusb_init(ctx=%p(%p)) -> %s\n", ctx, *ctx, ret_str);
	return ret;
}

int libusb_set_option(libusb_context *ctx, enum libusb_option option, ...)
{
	va_list args;
	va_start(args, option);
	int val = va_arg(args, int);
	va_end(args);
	int ret = real_libusb_set_option(ctx, option, val);
	char ret_str[64];
	FORMAT_RET(ret, ret_str, sizeof(ret_str));
	log_write("libusb_set_option(ctx=%p, option=%d, val=%d) -> %s\n", ctx, option, val, ret_str);
	return ret;
}

libusb_device_handle *libusb_open_device_with_vid_pid(libusb_context *ctx, uint16_t vid, uint16_t pid)
{
	libusb_device_handle *handle = real_libusb_open_device_with_vid_pid(ctx, vid, pid);
	char prefix[64];
	snprintf(prefix, sizeof(prefix), "[vid=0x%04x pid=0x%04x] ", vid, pid);
	log_api_call(prefix, "libusb_open_device_with_vid_pid",
			"libusb_open_device_with_vid_pid(ctx=%p) -> handle=%p\n", ctx, handle);
	return handle;
}

libusb_device *libusb_get_device(libusb_device_handle *handle)
{
	libusb_device *dev = real_libusb_get_device(handle);
	char prefix[64];
	get_handle_prefix(handle, prefix, sizeof(prefix));
	log_api_call(prefix, "libusb_get_device",
			"libusb_get_device(handle=%p) -> dev=%p\n", handle, dev);
	return dev;
}

int libusb_get_device_descriptor(libusb_device *dev, struct libusb_device_descriptor *desc)
{
	int ret = real_libusb_get_device_descriptor(dev, desc);
	char prefix[64];
	get_device_prefix(dev, prefix, sizeof(prefix));
	char ret_str[64];
	FORMAT_RET(ret, ret_str, sizeof(ret_str));
	char log_buf[1024];
	if (ret == 0 && desc) {
		snprintf(log_buf, sizeof(log_buf),
				"libusb_get_device_descriptor(dev=%p, desc=%p) -> %s"
				"  -> desc: bcdUSB=0x%04x, bDeviceClass=0x%02x, idVendor=0x%04x, idProduct=0x%04x, bcdDevice=0x%04x\n",
				dev, desc, ret_str,
				desc->bcdUSB, desc->bDeviceClass, desc->idVendor, desc->idProduct, desc->bcdDevice);
	} else {
		snprintf(log_buf, sizeof(log_buf),
				"libusb_get_device_descriptor(dev=%p, desc=%p) -> %s",
				dev, desc, ret_str);
	}
	log_api_call(prefix, "libusb_get_device_descriptor", "%s", log_buf);
	return ret;
}

int libusb_get_config_descriptor(libusb_device *dev, uint8_t config_index, struct libusb_config_descriptor **config)
{
	int ret = real_libusb_get_config_descriptor(dev, config_index, config);
	char prefix[64];
	get_device_prefix(dev, prefix, sizeof(prefix));
	char ret_str[64];
	FORMAT_RET(ret, ret_str, sizeof(ret_str));
	char log_buf[1024];
	if (ret == 0 && config && *config) {
		snprintf(log_buf, sizeof(log_buf),
				"libusb_get_config_descriptor(dev=%p, config_index=%u, config=%p(%p)) -> %s"
				"  -> config: bNumInterfaces=%u, bConfigurationValue=%u, MaxPower=%u\n",
				dev, config_index, config, *config, ret_str,
				(*config)->bNumInterfaces, (*config)->bConfigurationValue, (*config)->MaxPower);
	} else {
		snprintf(log_buf, sizeof(log_buf),
				"libusb_get_config_descriptor(dev=%p, config_index=%u, config=%p(%p)) -> %s\n",
				dev, config_index, config, *config, ret_str);
	}
	log_api_call(prefix, "libusb_get_config_descriptor", "%s", log_buf);
	return ret;
}

void libusb_free_config_descriptor(struct libusb_config_descriptor *config)
{
	real_libusb_free_config_descriptor(config);
	log_write("libusb_free_config_descriptor(config=%p)\n", config);
}

int libusb_detach_kernel_driver(libusb_device_handle *handle, int interface_number)
{
	int ret = real_libusb_detach_kernel_driver(handle, interface_number);
	char prefix[64];
	get_handle_prefix(handle, prefix, sizeof(prefix));
	char ret_str[64];
	FORMAT_RET(ret, ret_str, sizeof(ret_str));
	log_api_call(prefix, "libusb_detach_kernel_driver",
			"libusb_detach_kernel_driver(handle=%p, interface_number=%d) -> %s\n",
			handle, interface_number, ret_str);
	return ret;
}

int libusb_claim_interface(libusb_device_handle *handle, int interface_number)
{
	int ret = real_libusb_claim_interface(handle, interface_number);
	char prefix[64];
	get_handle_prefix(handle, prefix, sizeof(prefix));
	char ret_str[64];
	FORMAT_RET(ret, ret_str, sizeof(ret_str));
	log_api_call(prefix, "libusb_claim_interface",
			"libusb_claim_interface(handle=%p, interface_number=%d) -> %s\n",
			handle, interface_number, ret_str);
	return ret;
}

int libusb_reset_device(libusb_device_handle *handle)
{
	int ret = real_libusb_reset_device(handle);
	char prefix[64];
	get_handle_prefix(handle, prefix, sizeof(prefix));
	char ret_str[64];
	FORMAT_RET(ret, ret_str, sizeof(ret_str));
	log_api_call(prefix, "libusb_reset_device",
			"libusb_reset_device(handle=%p) -> %s\n",
			handle, ret_str);
	return ret;
}

int libusb_release_interface(libusb_device_handle *handle, int interface_number)
{
	int ret = real_libusb_release_interface(handle, interface_number);
	char prefix[64];
	get_handle_prefix(handle, prefix, sizeof(prefix));
	char ret_str[64];
	FORMAT_RET(ret, ret_str, sizeof(ret_str));
	log_api_call(prefix, "libusb_release_interface",
			"libusb_release_interface(handle=%p, interface_number=%d) -> %s\n",
			handle, interface_number, ret_str);
	return ret;
}

int libusb_attach_kernel_driver(libusb_device_handle *handle, int interface_number)
{
	int ret = real_libusb_attach_kernel_driver(handle, interface_number);
	char prefix[64];
	get_handle_prefix(handle, prefix, sizeof(prefix));
	char ret_str[64];
	FORMAT_RET(ret, ret_str, sizeof(ret_str));
	log_api_call(prefix, "libusb_attach_kernel_driver",
			"libusb_attach_kernel_driver(handle=%p, interface_number=%d) -> %s\n",
			handle, interface_number, ret_str);
	return ret;
}

void libusb_close(libusb_device_handle *handle)
{
	char prefix[64];
	get_handle_prefix(handle, prefix, sizeof(prefix));
	real_libusb_close(handle);
	log_api_call(prefix, "libusb_close",
			"libusb_close(handle=%p)\n", handle);
}

void libusb_exit(libusb_context *ctx)
{
	real_libusb_exit(ctx);
	log_write("libusb_exit(ctx=%p)\n", ctx);
}

#if 0
const char *libusb_strerror(int errcode)
{
	const char *s = real_libusb_strerror(errcode);
	log_write("libusb_strerror(errcode=%d) -> \"%s\"\n", errcode, s);
	return s;
}
#endif

// ==================== 新增钩子函数 ====================

// 频繁刷日志
// api计数失效原因是transfer->dev_handle= NULL
#if 0
// libusb_alloc_transfer
struct libusb_transfer *libusb_alloc_transfer(int iso_packets)
{
	struct libusb_transfer *transfer = real_libusb_alloc_transfer(iso_packets);
	char prefix[64] = "";
	if (transfer && transfer->dev_handle) {
		get_handle_prefix(transfer->dev_handle, prefix, sizeof(prefix));
	}
	log_api_call(prefix, "libusb_alloc_transfer",
			"libusb_alloc_transfer(iso_packets=%d) -> transfer=%p dev_handle=%p\n", iso_packets, transfer, transfer->dev_handle);
	return transfer;
}
#endif

// libusb_bulk_transfer
int libusb_bulk_transfer(libusb_device_handle *handle, unsigned char endpoint,
		unsigned char *data, int length, int *actual_length,
		unsigned int timeout)
{
	int ret = real_libusb_bulk_transfer(handle, endpoint, data, length, actual_length, timeout);
	char prefix[64];
	get_handle_prefix(handle, prefix, sizeof(prefix));
	char ret_str[64];
	FORMAT_RET(ret, ret_str, sizeof(ret_str));
	char log_buf[1024];
	if (actual_length) {
		snprintf(log_buf, sizeof(log_buf),
				"libusb_bulk_transfer(handle=%p, endpoint=0x%02x, data=%p, length=%d, actual_length=%p, timeout=%u) -> %s"
				"  -> actual_length=%d\n",
				handle, endpoint, data, length, actual_length, timeout, ret_str,
				*actual_length);
	} else {
		snprintf(log_buf, sizeof(log_buf),
				"libusb_bulk_transfer(handle=%p, endpoint=0x%02x, data=%p, length=%d, actual_length=%p, timeout=%u) -> %s\n",
				handle, endpoint, data, length, actual_length, timeout, ret_str);
	}
	log_api_call(prefix, "libusb_bulk_transfer", "%s", log_buf);
	return ret;
}

// libusb_cancel_transfer - 增加设备前缀
int libusb_cancel_transfer(struct libusb_transfer *transfer)
{
	int ret = real_libusb_cancel_transfer(transfer);
	char prefix[64] = "";
	if (transfer && transfer->dev_handle) {
		get_handle_prefix(transfer->dev_handle, prefix, sizeof(prefix));
	}
	char ret_str[64];
	FORMAT_RET(ret, ret_str, sizeof(ret_str));
	log_api_call(prefix, "libusb_cancel_transfer",
			"libusb_cancel_transfer(transfer=%p) -> %s\n", transfer, ret_str);
	return ret;
}

// libusb_clear_halt
int libusb_clear_halt(libusb_device_handle *handle, unsigned char endpoint)
{
	int ret = real_libusb_clear_halt(handle, endpoint);
	char prefix[64];
	get_handle_prefix(handle, prefix, sizeof(prefix));
	char ret_str[64];
	FORMAT_RET(ret, ret_str, sizeof(ret_str));
	log_api_call(prefix, "libusb_clear_halt",
			"libusb_clear_halt(handle=%p, endpoint=0x%02x) -> %s\n",
			handle, endpoint, ret_str);
	return ret;
}

int libusb_control_transfer(libusb_device_handle *handle, uint8_t bmRequestType,
		uint8_t bRequest, uint16_t wValue, uint16_t wIndex,
		unsigned char *data, uint16_t wLength, unsigned int timeout)
{
	int ret = real_libusb_control_transfer(handle, bmRequestType, bRequest, wValue, wIndex, data, wLength, timeout);
	char prefix[64];
	get_handle_prefix(handle, prefix, sizeof(prefix));
	char ret_str[64];
	FORMAT_RET(ret, ret_str, sizeof(ret_str));
	log_api_call(prefix, "libusb_control_transfer",
			"libusb_control_transfer(handle=%p, bmRequestType=0x%02x, bRequest=0x%02x, wValue=0x%04x, wIndex=0x%04x, data=%p, wLength=%u, timeout=%u) -> %s\n",
			handle, bmRequestType, bRequest, wValue, wIndex, data, wLength, timeout, ret_str);
	return ret;
}

int libusb_get_active_config_descriptor(libusb_device *dev, struct libusb_config_descriptor **config)
{
	int ret = real_libusb_get_active_config_descriptor(dev, config);
	char prefix[64];
	get_device_prefix(dev, prefix, sizeof(prefix));
	char ret_str[64];
	FORMAT_RET(ret, ret_str, sizeof(ret_str));
	log_api_call(prefix, "libusb_get_active_config_descriptor",
			"libusb_get_active_config_descriptor(dev=%p, config=%p(%p)) -> %s\n",
			dev, config, *config, ret_str);
	return ret;
}

int libusb_get_bos_descriptor(libusb_device_handle *handle, struct libusb_bos_descriptor **bos)
{
	int ret = real_libusb_get_bos_descriptor(handle, bos);
	char prefix[64];
	get_handle_prefix(handle, prefix, sizeof(prefix));
	char ret_str[64];
	FORMAT_RET(ret, ret_str, sizeof(ret_str));
	log_api_call(prefix, "libusb_get_bos_descriptor",
			"libusb_get_bos_descriptor(handle=%p, bos=%p(%p)) -> %s\n",
			handle, bos, *bos, ret_str);
	return ret;
}

uint8_t libusb_get_bus_number(libusb_device *dev)
{
	uint8_t ret = real_libusb_get_bus_number(dev);
	char prefix[64];
	get_device_prefix(dev, prefix, sizeof(prefix));
	log_api_call(prefix, "libusb_get_bus_number",
			"libusb_get_bus_number(dev=%p) -> %u\n", dev, ret);
	return ret;
}

int libusb_get_configuration(libusb_device_handle *handle, int *config)
{
	int ret = real_libusb_get_configuration(handle, config);
	char prefix[64];
	get_handle_prefix(handle, prefix, sizeof(prefix));
	char ret_str[64];
	FORMAT_RET(ret, ret_str, sizeof(ret_str));
	char log_buf[1024];
	if (ret == 0 && config) {
		snprintf(log_buf, sizeof(log_buf),
				"libusb_get_configuration(handle=%p, config=%p) -> %s"
				"  -> config=%d\n",
				handle, config, ret_str, *config);
	} else {
		snprintf(log_buf, sizeof(log_buf),
				"libusb_get_configuration(handle=%p, config=%p) -> %s\n",
				handle, config, ret_str);
	}
	log_api_call(prefix, "libusb_get_configuration", "%s", log_buf);
	return ret;
}

uint8_t libusb_get_device_address(libusb_device *dev)
{
	uint8_t ret = real_libusb_get_device_address(dev);
	char prefix[64];
	get_device_prefix(dev, prefix, sizeof(prefix));
	log_api_call(prefix, "libusb_get_device_address",
			"libusb_get_device_address(dev=%p) -> %u\n", dev, ret);
	return ret;
}

int libusb_get_device_speed(libusb_device *dev)
{
	int ret = real_libusb_get_device_speed(dev);
	char prefix[64];
	get_device_prefix(dev, prefix, sizeof(prefix));
	char ret_str[64];
	FORMAT_RET(ret, ret_str, sizeof(ret_str));
	log_api_call(prefix, "libusb_get_device_speed",
			"libusb_get_device_speed(dev=%p) -> %s\n", dev, ret_str);
	return ret;
}

libusb_device *libusb_get_parent(libusb_device *dev)
{
	libusb_device *ret = real_libusb_get_parent(dev);
	char prefix[64];
	get_device_prefix(dev, prefix, sizeof(prefix));
	log_api_call(prefix, "libusb_get_parent",
			"libusb_get_parent(dev=%p) -> dev=%p\n", dev, ret);
	return ret;
}

uint8_t libusb_get_port_number(libusb_device *dev)
{
	uint8_t ret = real_libusb_get_port_number(dev);
	char prefix[64];
	get_device_prefix(dev, prefix, sizeof(prefix));
	log_api_call(prefix, "libusb_get_port_number",
			"libusb_get_port_number(dev=%p) -> %u\n", dev, ret);
	return ret;
}

int libusb_get_port_numbers(libusb_device *dev, uint8_t *port_numbers, int port_numbers_len)
{
	int ret = real_libusb_get_port_numbers(dev, port_numbers, port_numbers_len);
	char prefix[64];
	get_device_prefix(dev, prefix, sizeof(prefix));
	char ret_str[64];
	FORMAT_RET(ret, ret_str, sizeof(ret_str));
	log_api_call(prefix, "libusb_get_port_numbers",
			"libusb_get_port_numbers(dev=%p, port_numbers=%p, port_numbers_len=%d) -> %s\n",
			dev, port_numbers, port_numbers_len, ret_str);
	return ret;
}

int libusb_get_string_descriptor_ascii(libusb_device_handle *handle, uint8_t desc_index,
		unsigned char *data, int length)
{
	int ret = real_libusb_get_string_descriptor_ascii(handle, desc_index, data, length);
	char prefix[64];
	get_handle_prefix(handle, prefix, sizeof(prefix));
	char ret_str[64];
	FORMAT_RET(ret, ret_str, sizeof(ret_str));
	log_api_call(prefix, "libusb_get_string_descriptor_ascii",
			"libusb_get_string_descriptor_ascii(handle=%p, desc_index=%u, data=%p, length=%d) -> %s\n",
			handle, desc_index, data, length, ret_str);
	return ret;
}

int libusb_interrupt_transfer(libusb_device_handle *handle, unsigned char endpoint,
		unsigned char *data, int length, int *actual_length,
		unsigned int timeout)
{
	int ret = real_libusb_interrupt_transfer(handle, endpoint, data, length, actual_length, timeout);
	char prefix[64];
	get_handle_prefix(handle, prefix, sizeof(prefix));
	char ret_str[64];
	FORMAT_RET(ret, ret_str, sizeof(ret_str));
	char log_buf[1024];
	if (actual_length) {
		snprintf(log_buf, sizeof(log_buf),
				"libusb_interrupt_transfer(handle=%p, endpoint=0x%02x, data=%p, length=%d, actual_length=%p, timeout=%u) -> %s"
				"  -> actual_length=%d\n",
				handle, endpoint, data, length, actual_length, timeout, ret_str,
				*actual_length);
	} else {
		snprintf(log_buf, sizeof(log_buf),
				"libusb_interrupt_transfer(handle=%p, endpoint=0x%02x, data=%p, length=%d, actual_length=%p, timeout=%u) -> %s\n",
				handle, endpoint, data, length, actual_length, timeout, ret_str);
	}
	log_api_call(prefix, "libusb_interrupt_transfer", "%s", log_buf);
	return ret;
}

int libusb_kernel_driver_active(libusb_device_handle *handle, int interface_number)
{
	int ret = real_libusb_kernel_driver_active(handle, interface_number);
	char prefix[64];
	get_handle_prefix(handle, prefix, sizeof(prefix));
	char ret_str[64];
	FORMAT_RET(ret, ret_str, sizeof(ret_str));
	log_api_call(prefix, "libusb_kernel_driver_active",
			"libusb_kernel_driver_active(handle=%p, interface_number=%d) -> %s\n",
			handle, interface_number, ret_str);
	return ret;
}

int libusb_open(libusb_device *dev, libusb_device_handle **handle)
{
	int ret = real_libusb_open(dev, handle);
	char prefix[64];
	get_device_prefix(dev, prefix, sizeof(prefix));
	char ret_str[64];
	FORMAT_RET(ret, ret_str, sizeof(ret_str));
	log_api_call(prefix, "libusb_open",
			"libusb_open(dev=%p, handle=%p(%p)) -> %s\n",
			dev, handle, *handle, ret_str);
	return ret;
}

int libusb_set_configuration(libusb_device_handle *handle, int configuration)
{
	int ret = real_libusb_set_configuration(handle, configuration);
	char prefix[64];
	get_handle_prefix(handle, prefix, sizeof(prefix));
	char ret_str[64];
	FORMAT_RET(ret, ret_str, sizeof(ret_str));
	log_api_call(prefix, "libusb_set_configuration",
			"libusb_set_configuration(handle=%p, configuration=%d) -> %s\n",
			handle, configuration, ret_str);
	return ret;
}

int libusb_set_interface_alt_setting(libusb_device_handle *handle, int interface_number, int alternate_setting)
{
	int ret = real_libusb_set_interface_alt_setting(handle, interface_number, alternate_setting);
	char prefix[64];
	get_handle_prefix(handle, prefix, sizeof(prefix));
	char ret_str[64];
	FORMAT_RET(ret, ret_str, sizeof(ret_str));
	log_api_call(prefix, "libusb_set_interface_alt_setting",
			"libusb_set_interface_alt_setting(handle=%p, interface_number=%d, alternate_setting=%d) -> %s\n",
			handle, interface_number, alternate_setting, ret_str);
	return ret;
}

// libusb_submit_transfer - 增加设备前缀
int libusb_submit_transfer(struct libusb_transfer *transfer)
{
	int ret = real_libusb_submit_transfer(transfer);
	char prefix[64] = "";
	if (transfer && transfer->dev_handle) {
		get_handle_prefix(transfer->dev_handle, prefix, sizeof(prefix));
	}
	char ret_str[64];
	FORMAT_RET(ret, ret_str, sizeof(ret_str));
	log_api_call(prefix, "libusb_submit_transfer",
			"libusb_submit_transfer(transfer=%p) -> %s\n", transfer, ret_str);
	return ret;
}

const struct libusb_version *libusb_get_version(void)
{
	const struct libusb_version *ver = real_libusb_get_version();
	log_write("libusb_get_version() -> %d.%d.%d.%d\n",
			ver->major, ver->minor, ver->micro, ver->nano);
	return ver;
}

int libusb_handle_events(libusb_context *ctx)
{
	int ret = real_libusb_handle_events(ctx);
	char ret_str[64];
	FORMAT_RET(ret, ret_str, sizeof(ret_str));
	log_write("libusb_handle_events(ctx=%p) -> %s\n", ctx, ret_str);
	return ret;
}

int libusb_handle_events_locked(libusb_context *ctx, struct timeval *tv)
{
	int ret = real_libusb_handle_events_locked(ctx, tv);
	char ret_str[64];
	FORMAT_RET(ret, ret_str, sizeof(ret_str));
	log_write("libusb_handle_events_locked(ctx=%p, tv=%p) -> %s\n", ctx, tv, ret_str);
	return ret;
}

//频繁刷新日志
#if 0
// libusb_handle_events_timeout_completed
int libusb_handle_events_timeout_completed(libusb_context *ctx, struct timeval *tv, int *completed)
{
	int ret = real_libusb_handle_events_timeout_completed(ctx, tv, completed);
	char ret_str[64];
	FORMAT_RET(ret, ret_str, sizeof(ret_str));
	if (completed) {
		log_write("libusb_handle_events_timeout_completed(ctx=%p, tv=%p, completed=%p) -> %s, *completed=%d\n",
				ctx, tv, completed, ret_str, *completed);
	} else {
		log_write("libusb_handle_events_timeout_completed(ctx=%p, tv=%p, completed=%p) -> %s\n",
				ctx, tv, completed, ret_str);
	}
	return ret;
}
#endif

// libusb_wait_for_event
int libusb_wait_for_event(libusb_context *ctx, struct timeval *tv)
{
	int ret = real_libusb_wait_for_event(ctx, tv);
	char ret_str[64];
	FORMAT_RET(ret, ret_str, sizeof(ret_str));
	log_write("libusb_wait_for_event(ctx=%p, tv=%p) -> %s\n", ctx, tv, ret_str);
	return ret;
}

// libusb_has_capability
#if 0
int libusb_has_capability(uint32_t capability)
{
	int ret = real_libusb_has_capability(capability);
	char ret_str[64];
	FORMAT_RET(ret, ret_str, sizeof(ret_str));
	log_write("libusb_has_capability(capability=%u) -> %s\n", capability, ret_str);
	return ret;
}
#endif

int libusb_get_ss_usb_device_capability_descriptor(
		libusb_context *ctx,
		struct libusb_bos_dev_capability_descriptor *dev_cap,
		struct libusb_ss_usb_device_capability_descriptor **ss_cap)
{
	int ret = real_libusb_get_ss_usb_device_capability_descriptor(ctx, dev_cap, ss_cap);
	char ret_str[64];
	FORMAT_RET(ret, ret_str, sizeof(ret_str));
	log_write("libusb_get_ss_usb_device_capability_descriptor(ctx=%p, dev_cap=%p, ss_cap=%p(%p)) -> %s\n",
			ctx, dev_cap, ss_cap, ss_cap ? *ss_cap : NULL, ret_str);
	return ret;
}


int libusb_hotplug_register_callback(libusb_context *ctx, int events, int flags,
		int vendor_id, int product_id, int dev_class,
		libusb_hotplug_callback_fn cb_fn, void *user_data,
		libusb_hotplug_callback_handle *callback_handle)
{
	int ret = real_libusb_hotplug_register_callback(ctx, events, flags, vendor_id, product_id,
					dev_class, cb_fn, user_data, callback_handle);
	char ret_str[64];
	FORMAT_RET(ret, ret_str, sizeof(ret_str));
	log_write("libusb_hotplug_register_callback(ctx=%p, events=0x%x, flags=%d, vendor_id=0x%04x, product_id=0x%04x, dev_class=%d, cb_fn=%p, user_data=%p, callback_handle=%p, *callback_handle=%d) -> %s\n",
			ctx, events, flags, vendor_id, product_id, dev_class, cb_fn, user_data, callback_handle,
			callback_handle ? *callback_handle : 0, ret_str);
	return ret;
}

int libusb_setlocale(const char *locale)
{
	int ret = real_libusb_setlocale(locale);
	char ret_str[64];
	FORMAT_RET(ret, ret_str, sizeof(ret_str));
	log_write("libusb_setlocale(locale=\"%s\") -> %s\n", locale ? locale : "NULL", ret_str);
	return ret;
}

ssize_t libusb_get_device_list(libusb_context *ctx, libusb_device ***list)
{
	ssize_t ret = real_libusb_get_device_list(ctx, list);
	// 对于返回值为负的情况，同样处理
	if (ret < 0) {
		char ret_str[64];
		FORMAT_RET((int)ret, ret_str, sizeof(ret_str));
		log_write("libusb_get_device_list(ctx=%p, list=%p(%p)) -> %s\n", ctx, list, list ? *list : NULL, ret_str);
	} else {
		log_write("libusb_get_device_list(ctx=%p, list=%p(%p)) -> %zd\n", ctx, list, list ? *list : NULL, ret);
	}
	return ret;
}

int libusb_wrap_sys_device(libusb_context *ctx, intptr_t sys_dev, libusb_device_handle **handle)
{
	int ret = real_libusb_wrap_sys_device(ctx, sys_dev, handle);
	char ret_str[64];
	FORMAT_RET(ret, ret_str, sizeof(ret_str));
	log_write("libusb_wrap_sys_device(ctx=%p, sys_dev=%ld, handle=%p(%p)) -> %s\n",
			ctx, (long)sys_dev, handle, *handle, ret_str);
	return ret;
}

/* Copyright (C) 2026 wqshao All rights reserved.
 *
 *  File Name    : hook.c
 *  Author       : wqshao
 *  Created Time : 2026-03-19 11:12:55
 *  Description  :
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <dlfcn.h>
#if defined(__aarch64__)
	#include <libusb.h>
#else
	#include <libusb-1.0/libusb.h>
#endif

// 辅助宏：获取真实函数指针（线程安全，首次调用时初始化）
#define GET_REAL(name) \
	static typeof(name) *real_##name = NULL; \
	if (!real_##name) { \
		real_##name = (typeof(name)*) dlsym(RTLD_NEXT, #name); \
		if (!real_##name) { \
			fprintf(stderr, "dlsym error: %s\n", dlerror()); \
		} \
	}

#if 0
// 打印 libusb 错误码的简易函数（可选）
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

// 添加一个构造函数，该函数在库加载时自动执行，打印一条确认消息
__attribute__((constructor))
void libusb_hook_init(void)
{
	printf("libusb_hook.so 已加载 ✅\n");
	fflush(stdout);
}

// 1. libusb_init
int libusb_init(libusb_context **ctx)
{
	GET_REAL(libusb_init);
	printf("libusb_init(ctx=%p)\n", ctx);
	int ret = real_libusb_init(ctx);
	printf("  -> %d (%s)\n", ret, libusb_error_name(ret));
	return ret;
}

// 2. libusb_set_option (可变参数，我们假设最常见的情况：option 后跟一个 int)
int libusb_set_option(libusb_context *ctx, enum libusb_option option, ...)
{
	GET_REAL(libusb_set_option);
	va_list args;
	va_start(args, option);
	int val = va_arg(args, int);   // 根据实际使用的 option，我们只取第一个 int 参数
	va_end(args);
	printf("libusb_set_option(ctx=%p, option=%d, val=%d)\n", ctx, option, val);
	int ret = real_libusb_set_option(ctx, option, val);
	printf("  -> %d (%s)\n", ret, libusb_error_name(ret));
	return ret;
}

// 3. libusb_open_device_with_vid_pid
libusb_device_handle *libusb_open_device_with_vid_pid(libusb_context *ctx, uint16_t vid, uint16_t pid)
{
	GET_REAL(libusb_open_device_with_vid_pid);
	printf("libusb_open_device_with_vid_pid(ctx=%p, vid=0x%04x, pid=0x%04x)\n", ctx, vid, pid);
	libusb_device_handle *handle = real_libusb_open_device_with_vid_pid(ctx, vid, pid);
	printf("  -> handle=%p\n", handle);
	return handle;
}

// 4. libusb_get_device
libusb_device *libusb_get_device(libusb_device_handle *handle)
{
	GET_REAL(libusb_get_device);
	printf("libusb_get_device(handle=%p)\n", handle);
	libusb_device *dev = real_libusb_get_device(handle);
	printf("  -> dev=%p\n", dev);
	return dev;
}

// 5. libusb_get_device_descriptor
int libusb_get_device_descriptor(libusb_device *dev, struct libusb_device_descriptor *desc)
{
	GET_REAL(libusb_get_device_descriptor);
	printf("libusb_get_device_descriptor(dev=%p, desc=%p)\n", dev, desc);
	int ret = real_libusb_get_device_descriptor(dev, desc);
	if (ret == 0 && desc) {
		printf("  -> desc: bcdUSB=0x%04x, bDeviceClass=0x%02x, idVendor=0x%04x, idProduct=0x%04x, bcdDevice=0x%04x\n",
				desc->bcdUSB, desc->bDeviceClass, desc->idVendor, desc->idProduct, desc->bcdDevice);
	}
	printf("  -> %d (%s)\n", ret, libusb_error_name(ret));
	return ret;
}

// 6. libusb_get_config_descriptor
int libusb_get_config_descriptor(libusb_device *dev, uint8_t config_index, struct libusb_config_descriptor **config)
{
	GET_REAL(libusb_get_config_descriptor);
	printf("libusb_get_config_descriptor(dev=%p, config_index=%u, config=%p)\n", dev, config_index, config);
	int ret = real_libusb_get_config_descriptor(dev, config_index, config);
	if (ret == 0 && config && *config) {
		printf("  -> config: bNumInterfaces=%u, bConfigurationValue=%u, MaxPower=%u\n",
				(*config)->bNumInterfaces, (*config)->bConfigurationValue, (*config)->MaxPower);
	}
	printf("  -> %d (%s)\n", ret, libusb_error_name(ret));
	return ret;
}

// 7. libusb_free_config_descriptor
void libusb_free_config_descriptor(struct libusb_config_descriptor *config)
{
	GET_REAL(libusb_free_config_descriptor);
	printf("libusb_free_config_descriptor(config=%p)\n", config);
	real_libusb_free_config_descriptor(config);
}

// 8. libusb_detach_kernel_driver
int libusb_detach_kernel_driver(libusb_device_handle *handle, int interface_number)
{
	GET_REAL(libusb_detach_kernel_driver);
	printf("libusb_detach_kernel_driver(handle=%p, interface_number=%d)\n", handle, interface_number);
	int ret = real_libusb_detach_kernel_driver(handle, interface_number);
	printf("  -> %d (%s)\n", ret, libusb_error_name(ret));
	return ret;
}

// 9. libusb_claim_interface
int libusb_claim_interface(libusb_device_handle *handle, int interface_number)
{
	GET_REAL(libusb_claim_interface);
	printf("libusb_claim_interface(handle=%p, interface_number=%d)\n", handle, interface_number);
	int ret = real_libusb_claim_interface(handle, interface_number);
	printf("  -> %d (%s)\n", ret, libusb_error_name(ret));
	return ret;
}

// 10. libusb_reset_device
int libusb_reset_device(libusb_device_handle *handle)
{
	GET_REAL(libusb_reset_device);
	printf("libusb_reset_device(handle=%p)\n", handle);
	int ret = real_libusb_reset_device(handle);
	printf("  -> %d (%s)\n", ret, libusb_error_name(ret));
	return ret;
}

// 11. libusb_release_interface
int libusb_release_interface(libusb_device_handle *handle, int interface_number)
{
	GET_REAL(libusb_release_interface);
	printf("libusb_release_interface(handle=%p, interface_number=%d)\n", handle, interface_number);
	int ret = real_libusb_release_interface(handle, interface_number);
	printf("  -> %d (%s)\n", ret, libusb_error_name(ret));
	return ret;
}

// 12. libusb_attach_kernel_driver
int libusb_attach_kernel_driver(libusb_device_handle *handle, int interface_number)
{
	GET_REAL(libusb_attach_kernel_driver);
	printf("libusb_attach_kernel_driver(handle=%p, interface_number=%d)\n", handle, interface_number);
	int ret = real_libusb_attach_kernel_driver(handle, interface_number);
	printf("  -> %d (%s)\n", ret, libusb_error_name(ret));
	return ret;
}

// 13. libusb_close
void libusb_close(libusb_device_handle *handle)
{
	GET_REAL(libusb_close);
	printf("libusb_close(handle=%p)\n", handle);
	real_libusb_close(handle);
}

// 14. libusb_exit
void libusb_exit(libusb_context *ctx)
{
	GET_REAL(libusb_exit);
	printf("libusb_exit(ctx=%p)\n", ctx);
	real_libusb_exit(ctx);
}

// 15. libusb_strerror（可选）
const char *libusb_strerror(int errcode)
{
	GET_REAL(libusb_strerror);
	printf("libusb_strerror(errcode=%d)\n", errcode);
	const char *s = real_libusb_strerror(errcode);
	printf("  -> \"%s\"\n", s);
	return s;
}

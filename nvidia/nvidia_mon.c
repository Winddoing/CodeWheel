/* Copyright (C) 2026 wqshao All rights reserved.
 *
 *  File Name    : nvidia_mon.c
 *  Author       : wqshao
 *  Created Time : 2026-02-24 13:49:25
 *  Description  :
 */

#include <stdio.h>
#include <stdlib.h>

// 平台相关的头文件和函数
#ifdef __linux__
	#include <dlfcn.h>
	#include <unistd.h>
#elif _WIN32
	#include <windows.h>
#else
	#error "Unsupported platform"
#endif

// ---------- 跨平台类型定义 ----------
// 定义函数指针类型（与 NVML 原始函数签名一致）
typedef void *(*nvmlInit_v2_t)(void);
typedef void *(*nvmlShutdown_t)(void);
typedef void *(*nvmlDeviceGetCount_v2_t)(unsigned int *);
typedef void *(*nvmlDeviceGetHandleByIndex_t)(unsigned int, void **);
typedef void *(*nvmlDeviceGetName_t)(void *, char *, unsigned int);
typedef void *(*nvmlDeviceGetTemperature_t)(void *, int, unsigned int *);
typedef void *(*nvmlDeviceGetFanSpeed_t)(void *, unsigned int *);
typedef void *(*nvmlDeviceGetPowerUsage_t)(void *, unsigned int *);
typedef void *(*nvmlDeviceGetUtilizationRates_t)(void *, void *);
typedef void *(*nvmlDeviceGetMemoryInfo_t)(void *, void *);

// 定义 NVML 中需要的结构体
typedef struct nvmlUtilization_st {
	unsigned int gpu;
	unsigned int memory;
} nvmlUtilization_t;

typedef struct nvmlMemory_st {
	unsigned long long total;
	unsigned long long used;
	unsigned long long free;
} nvmlMemory_t;

#define NVML_SUCCESS 0

// ---------- 平台抽象：库句柄类型 ----------
#ifdef __linux__
	typedef void *library_handle_t;
	#define INVALID_LIBRARY_HANDLE NULL
#elif _WIN32
	typedef HMODULE library_handle_t;
	#define INVALID_LIBRARY_HANDLE NULL
#endif

// ---------- 平台抽象：加载/卸载库 ----------
static library_handle_t load_library()
{
#ifdef __linux__
	void *handle = dlopen("libnvidia-ml.so", RTLD_NOW | RTLD_GLOBAL);
	if (!handle) {
		// 尝试带版本号的后备方案
		handle = dlopen("libnvidia-ml.so.1", RTLD_NOW | RTLD_GLOBAL);
	}
	return handle;
#elif _WIN32
	HMODULE handle = LoadLibraryA("nvml.dll");
	if (!handle) {
		// 尝试从 NVIDIA NVSMI 目录加载
		handle = LoadLibraryA("C:\\Program Files\\NVIDIA Corporation\\NVSMI\\nvml.dll");
	}
	return handle;
#endif
}

static void unload_library(library_handle_t handle)
{
#ifdef __linux__
	dlclose(handle);
#elif _WIN32
	FreeLibrary(handle);
#endif
}

static void *get_symbol(library_handle_t handle, const char *symbol)
{
#ifdef __linux__
	return dlsym(handle, symbol);
#elif _WIN32
	return (void *)GetProcAddress(handle, symbol);
#endif
}

// ---------- 平台抽象：延时函数 ----------
static void sleep_seconds(unsigned int seconds)
{
#ifdef __linux__
	sleep(seconds);
#elif _WIN32
	Sleep(seconds * 1000);
#endif
}

int main(int argc, const char *argv[])
{
	library_handle_t nvml_handle;
	nvmlInit_v2_t nvmlInit_v2;
	nvmlShutdown_t nvmlShutdown;
	nvmlDeviceGetCount_v2_t nvmlDeviceGetCount_v2;
	nvmlDeviceGetHandleByIndex_t nvmlDeviceGetHandleByIndex;
	nvmlDeviceGetName_t nvmlDeviceGetName;
	nvmlDeviceGetTemperature_t nvmlDeviceGetTemperature;
	nvmlDeviceGetFanSpeed_t nvmlDeviceGetFanSpeed;
	nvmlDeviceGetPowerUsage_t nvmlDeviceGetPowerUsage;
	nvmlDeviceGetUtilizationRates_t nvmlDeviceGetUtilizationRates;
	nvmlDeviceGetMemoryInfo_t nvmlDeviceGetMemoryInfo;

	int NVML_TEMPERATURE_GPU = 0;   // 温度类型常量

	// ---------- 第一步：动态加载 NVML 库 ----------
	nvml_handle = load_library();
	if (nvml_handle == INVALID_LIBRARY_HANDLE) {
		printf("未检测到 NVIDIA 显卡或 NVML 库未安装，程序将以无 GPU 模式运行。\n");
		return 0; // 正常退出
	}
	printf("成功加载 NVML 库\n");

	// ---------- 第二步：获取所有需要的函数指针 ----------
	nvmlInit_v2 = (nvmlInit_v2_t)get_symbol(nvml_handle, "nvmlInit_v2");
	nvmlShutdown = (nvmlShutdown_t)get_symbol(nvml_handle, "nvmlShutdown");
	nvmlDeviceGetCount_v2 = (nvmlDeviceGetCount_v2_t)get_symbol(nvml_handle, "nvmlDeviceGetCount_v2");  // 新增
	nvmlDeviceGetHandleByIndex = (nvmlDeviceGetHandleByIndex_t)get_symbol(nvml_handle, "nvmlDeviceGetHandleByIndex_v2");
	nvmlDeviceGetName = (nvmlDeviceGetName_t)get_symbol(nvml_handle, "nvmlDeviceGetName");
	nvmlDeviceGetTemperature = (nvmlDeviceGetTemperature_t)get_symbol(nvml_handle, "nvmlDeviceGetTemperature");
	nvmlDeviceGetFanSpeed = (nvmlDeviceGetFanSpeed_t)get_symbol(nvml_handle, "nvmlDeviceGetFanSpeed");
	nvmlDeviceGetPowerUsage = (nvmlDeviceGetPowerUsage_t)get_symbol(nvml_handle, "nvmlDeviceGetPowerUsage");
	nvmlDeviceGetUtilizationRates = (nvmlDeviceGetUtilizationRates_t)get_symbol(nvml_handle, "nvmlDeviceGetUtilizationRates");
	nvmlDeviceGetMemoryInfo = (nvmlDeviceGetMemoryInfo_t)get_symbol(nvml_handle, "nvmlDeviceGetMemoryInfo");

	// 检查关键函数是否获取成功
	if (!nvmlInit_v2 || !nvmlDeviceGetCount_v2 || !nvmlDeviceGetHandleByIndex) {
		printf("无法获取 NVML 核心函数\n");
		unload_library(nvml_handle);
		return -1;
	}

	// ---------- 第三步：初始化 NVML ----------
	void *result = nvmlInit_v2();
	if (result != (void *)NVML_SUCCESS) {
		printf("NVML 初始化失败\n");
		unload_library(nvml_handle);
		return -1;
	}

	// ---------- 第四步：获取 GPU 设备数量 ----------
	unsigned int device_count = 0;
	result = nvmlDeviceGetCount_v2(&device_count);
	if (result != (void *)NVML_SUCCESS || device_count == 0) {
		printf("系统中未找到 NVIDIA GPU 设备\n");
		nvmlShutdown();
		unload_library(nvml_handle);
		return 0;
	}
	printf("检测到 %u 个 NVIDIA GPU 设备，开始监控...\n\n", device_count);

	// ---------- 第五步：无限循环监控所有 GPU ----------
	while (1) {
		// 遍历每个 GPU 设备
		for (unsigned int i = 0; i < device_count; i++) {
			void *device;
			// 根据索引获取设备句柄
			result = nvmlDeviceGetHandleByIndex(i, &device);
			if (result != (void *)NVML_SUCCESS) {
				printf("无法获取 GPU %u 的句柄，跳过\n", i);
				continue;
			}

			// 输出设备索引
			printf("GPU %u ", i);

			// 获取设备名称
			char name[64];
			if (nvmlDeviceGetName(device, name, sizeof(name)) == (void *)NVML_SUCCESS) {
				printf("(%s)\n", name);
			} else {
				printf("\n");
			}

			// 获取温度
			unsigned int temp;
			if (nvmlDeviceGetTemperature(device, NVML_TEMPERATURE_GPU, &temp) == (void *)NVML_SUCCESS) {
				printf("  温度: %u°C\n", temp);
			}

			// 获取风扇转速
			unsigned int fan_speed;
			if (nvmlDeviceGetFanSpeed(device, &fan_speed) == (void *)NVML_SUCCESS) {
				printf("  风扇转速: %u%%\n", fan_speed);
			}

			// 获取功耗
			unsigned int power;
			if (nvmlDeviceGetPowerUsage(device, &power) == (void *)NVML_SUCCESS) {
				printf("  功耗: %.2f W\n", power / 1000.0);
			}

			// 获取利用率
			nvmlUtilization_t util;
			if (nvmlDeviceGetUtilizationRates(device, &util) == (void *)NVML_SUCCESS) {
				printf("  GPU 利用率: %u%%\n", util.gpu);
				printf("  显存利用率: %u%%\n", util.memory);
			}

			// 获取显存信息
			nvmlMemory_t mem;
			if (nvmlDeviceGetMemoryInfo(device, &mem) == (void *)NVML_SUCCESS) {
				printf("  显存使用: %.2f / %.2f GB\n",
						mem.used / 1024.0 / 1024.0 / 1024.0,
						mem.total / 1024.0 / 1024.0 / 1024.0);
			}

			printf("----------------------------\n");
		}
		// 所有设备输出完毕后增加一个空行，使周期分隔更清晰
		printf("\n");
		sleep_seconds(2); // 每 2 秒刷新一次
	}

	// 清理（实际代码中不会执行到这里）
	nvmlShutdown();
	unload_library(nvml_handle);
	return 0;
}

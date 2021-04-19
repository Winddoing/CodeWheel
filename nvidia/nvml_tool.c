/*#############################################################
 *     File Name	: nvml_tool.c
 *     Author		: winddoing
 *     Created Time	: 2021年04月10日 星期六 23时02分34秒
 *     Description	:
 *############################################################*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "nvml.h"

const char * convertToComputeModeString(nvmlComputeMode_t mode)
{
    switch (mode)
    {
    case NVML_COMPUTEMODE_DEFAULT:
        return "Default";
    case NVML_COMPUTEMODE_EXCLUSIVE_THREAD:
        return "Exclusive_Thread";
    case NVML_COMPUTEMODE_PROHIBITED:
        return "Prohibited";
    case NVML_COMPUTEMODE_EXCLUSIVE_PROCESS:
        return "Exclusive Process";
    default:
        return "Unknown";
    }
}

int main()
{
    nvmlReturn_t result;
    unsigned int device_count, i;

    // First initialize NVML library
    result = nvmlInit();
    if (NVML_SUCCESS != result)
    {
        printf("Failed to initialize NVML: %s\n", nvmlErrorString(result));

        printf("Press ENTER to continue...\n");
        getchar();
        return 1;
    }

    result = nvmlDeviceGetCount(&device_count);
    if (NVML_SUCCESS != result)
    {
        printf("Failed to query device count: %s\n", nvmlErrorString(result));
        goto Error;
    }
    printf("Found %d device%s\n\n", device_count, device_count != 1 ? "s" : "");

    printf("Listing devices:\n");
    for (i = 0; i < device_count; i++)
    {
        nvmlDevice_t device;
        char name[NVML_DEVICE_NAME_BUFFER_SIZE];
        nvmlPciInfo_t pci;
        nvmlComputeMode_t compute_mode;

        // Query for device handle to perform operations on a device
        // You can also query device handle by other features like:
        // nvmlDeviceGetHandleBySerial
        // nvmlDeviceGetHandleByPciBusId
        result = nvmlDeviceGetHandleByIndex(i, &device);
        if (NVML_SUCCESS != result)
        {
            printf("Failed to get handle for device %i: %s\n", i, nvmlErrorString(result));
            goto Error;
        }

        result = nvmlDeviceGetName(device, name, NVML_DEVICE_NAME_BUFFER_SIZE);
        if (NVML_SUCCESS != result)
        {
            printf("Failed to get name of device %i: %s\n", i, nvmlErrorString(result));
            goto Error;
        }

        printf("===> func: %s, line: %d, name: %s\n", __func__, __LINE__, name);
        // pci.busId is very useful to know which device physically you're talking to
        // Using PCI identifier you can also match nvmlDevice handle to CUDA device.
        result = nvmlDeviceGetPciInfo(device, &pci);
        if (NVML_SUCCESS != result)
        {
            printf("Failed to get pci info for device %i: %s\n", i, nvmlErrorString(result));
            goto Error;
        }

        printf("%d. %s [%s]\n", i, name, pci.busId);

#if 0
        // This is a simple example on how you can modify GPU's state
        result = nvmlDeviceGetComputeMode(device, &compute_mode);
        if (NVML_ERROR_NOT_SUPPORTED == result)
            printf("\t This is not CUDA capable device\n");
        else if (NVML_SUCCESS != result)
        {
            printf("Failed to get compute mode for device %i: %s\n", i, nvmlErrorString(result));
            goto Error;
        }
        else
        {
            // try to change compute mode
            printf("\t Changing device's compute mode from '%s' to '%s'\n",
                    convertToComputeModeString(compute_mode),
                    convertToComputeModeString(NVML_COMPUTEMODE_PROHIBITED));

            result = nvmlDeviceSetComputeMode(device, NVML_COMPUTEMODE_PROHIBITED);
            if (NVML_ERROR_NO_PERMISSION == result)
                printf("\t\t Need root privileges to do that: %s\n", nvmlErrorString(result));
            else if (NVML_ERROR_NOT_SUPPORTED == result)
                printf("\t\t Compute mode prohibited not supported. You might be running on\n"
                        "\t\t windows in WDDM driver model or on non-CUDA capable GPU.\n");
            else if (NVML_SUCCESS != result)
            {
                printf("\t\t Failed to set compute mode for device %i: %s\n", i, nvmlErrorString(result));
                goto Error;
            }
            else
            {
                printf("\t Restoring device's compute mode back to '%s'\n",
                        convertToComputeModeString(compute_mode));
                result = nvmlDeviceSetComputeMode(device, compute_mode);
                if (NVML_SUCCESS != result)
                {
                    printf("\t\t Failed to restore compute mode for device %i: %s\n", i, nvmlErrorString(result));
                    goto Error;
                }
            }
        }

        printf("\n");
#endif
        printf("----- 温度 ----- \n");
        unsigned int temperature_threshold = 100;
        result = nvmlDeviceGetTemperatureThreshold(device, NVML_TEMPERATURE_THRESHOLD_SHUTDOWN, &temperature_threshold);
        if (NVML_SUCCESS != result)
        {
            printf("device %i Failed to get NVML_TEMPERATURE_THRESHOLD_SHUTDOWN: %s\n", i, nvmlErrorString(result));
        }
        else
            printf("截止温度： %d 摄氏度  （Temperature at which the GPU will shut down for HW protection）\n", temperature_threshold);

        result = nvmlDeviceGetTemperatureThreshold(device, NVML_TEMPERATURE_THRESHOLD_SLOWDOWN, &temperature_threshold);
        if (NVML_SUCCESS != result)
        {
            printf("device %i Failed NVML_TEMPERATURE_THRESHOLD_SLOWDOWN: %s\n", i, nvmlErrorString(result));
        }
        else
            printf("上限温度： %d 摄氏度  （Temperature at which the GPU will begin slowdown）\n", temperature_threshold);

        unsigned int temperature = 0;
        result = nvmlDeviceGetTemperature(device, NVML_TEMPERATURE_GPU, &temperature);
        if (NVML_SUCCESS != result)
        {
            printf("device %i NVML_TEMPERATURE_GPU Failed: %s\n", i, nvmlErrorString(result));
        }
        else
            printf("当前温度： %d 摄氏度 \n", temperature);

        //使用率
        printf("\n");
        nvmlUtilization_t utilization;
        result = nvmlDeviceGetUtilizationRates(device, &utilization);
        if (NVML_SUCCESS != result)
        {
            printf(" device %i nvmlDeviceGetUtilizationRates Failed : %s\n", i, nvmlErrorString(result));
        }
        else
        {
            printf("----- 使用率 ----- \n");
            printf("GPU 使用率： %d%% \n", utilization.gpu);
            printf("显存使用率： %d%% \n", utilization.memory);
        }


        unsigned int encoder_rate;
        unsigned int encoder_sampling;
        result = nvmlDeviceGetEncoderUtilization(device, &encoder_rate, &encoder_sampling);
        if (NVML_SUCCESS != result)
        {
            printf("device %i nvmlDeviceGetEncoderUtilization Failed : %s\n", i, nvmlErrorString(result));
        } else {
            printf("-------ENC--------\n");
            printf("encoder_rate=%d\n", encoder_rate);
        }

        //FB memory
        printf("\n");
        nvmlMemory_t memory;
        result = nvmlDeviceGetMemoryInfo(device, &memory);
        if (NVML_SUCCESS != result)
        {
            printf("device %i nvmlDeviceGetMemoryInfo Failed : %s\n", i, nvmlErrorString(result));
        }
        else
        {
            printf("------ FB memory ------- \n");
            printf("Total installed FB memory： %lld bytes %ld MB\n", memory.total, memory.total/1024/1024);
            printf("Unallocated FB memory： %lld bytes \n", memory.free);
            printf("Allocated FB memory： %lld bytes \n", memory.used);
        }

        //BAR1 memory
        printf("\n");
        nvmlBAR1Memory_t bar1Memory;
        result = nvmlDeviceGetBAR1MemoryInfo(device, &bar1Memory);
        if (NVML_SUCCESS != result)
        {
            printf("device %i  nvmlDeviceGetBAR1MemoryInfo Failed : %s\n", i, nvmlErrorString(result));
        }
        else
        {
            printf("------ BAR1 memory ------- \n");
            printf("Total BAR1 memory： %lld bytes \n", bar1Memory.bar1Total);
            printf("Unallocated BAR1 memory： %lld bytes \n", bar1Memory.bar1Free);
            printf("Allocated BAR1 memory： %lld bytes \n", bar1Memory.bar1Used);
        }

        //Information about running compute processes on the GPU
        printf("\n");
        unsigned int infoCount;
        nvmlProcessInfo_t infos[999];
        result = nvmlDeviceGetComputeRunningProcesses(device, &infoCount, infos);
        if (NVML_SUCCESS != result)
        {
            printf("Failed to get ComputeRunningProcesses for device %i: %s\n", i, nvmlErrorString(result));
        }
        else
        {
#if 0
            HANDLE handle; //定义CreateToolhelp32Snapshot系统快照句柄
            handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);//获得系统快照句柄
            PROCESSENTRY32 *info; //定义PROCESSENTRY32结构字指
            //PROCESSENTRY32 结构的 dwSize 成员设置成 sizeof(PROCESSENTRY32)
            info = new PROCESSENTRY32;
            info->dwSize = sizeof(PROCESSENTRY32);
            //调用一次     Process32First 函数，从快照中获取进程列表
            Process32First(handle, info);
            //重复调用 Process32Next，直到函数返回 FALSE 为止

            printf("------ Information about running compute processes on the GPU ------- \n");
            for (int i = 0; i < infoCount; i++)
            {
                printf("PID： %d  显存占用：%lld bytes   ", infos[i].pid, infos[i].usedGpuMemory);

                while (Process32Next(handle, info) != FALSE)
                {
                    if (info->th32ProcessID == infos[i].pid)
                    {
                        //printf("  %s\n", info->szExeFile);

                        HANDLE hProcess = NULL;
                        //打开目标进程
                        hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, info->th32ProcessID);
                        if (hProcess == NULL) {
                            printf("\nOpen Process fAiled:%d\n", GetLastError());
                            break;
                        }

                        char strFilePath[MAX_PATH];
                        GetModuleFileNameEx(hProcess, NULL, strFilePath, MAX_PATH);
                        printf(" %s\n", strFilePath);

                        CloseHandle(hProcess);

                        break;
                    }
                }
            }

            delete info;
            CloseHandle(handle);
#endif
        }

        //BAR1 memory
        printf("\n");
        printf("------ Clocks ------- \n");
        unsigned int max_clock;
        result = nvmlDeviceGetMaxClockInfo(device, NVML_CLOCK_GRAPHICS, &max_clock);
        if (NVML_SUCCESS != result)
        {
            printf("device %i   nvmlDeviceGetMaxClockInfo Failed : %s\n", i, nvmlErrorString(result));
        }

        unsigned int clock;
        result = nvmlDeviceGetClockInfo(device, NVML_CLOCK_GRAPHICS, &clock);
        if (NVML_SUCCESS != result)
        {
            printf("Failed to get NVML_CLOCK_GRAPHICS info for device %i: %s\n", i, nvmlErrorString(result));
        }
        else
        {
            printf("GRAPHICS： %6d Mhz   max clock ：%d  \n", clock, max_clock);
        }

        result = nvmlDeviceGetMaxClockInfo(device, NVML_CLOCK_SM, &max_clock);
        if (NVML_SUCCESS != result)
        {
            printf("Failed to get max NVML_CLOCK_SM for device %i: %s\n", i, nvmlErrorString(result));
        }

        result = nvmlDeviceGetClockInfo(device, NVML_CLOCK_SM, &clock);
        if (NVML_SUCCESS != result)
        {
            printf("Failed to get current NVML_CLOCK_SM for device %i: %s\n", i, nvmlErrorString(result));
        }
        else
        {
            printf("      SM： %6d Mhz   max clock ：%d   \n", clock, max_clock);
        }

        result = nvmlDeviceGetMaxClockInfo(device, NVML_CLOCK_MEM, &max_clock);
        if (NVML_SUCCESS != result)
        {
            printf("Failed to get max NVML_CLOCK_MEM for device %i: %s\n", i, nvmlErrorString(result));
        }

        result = nvmlDeviceGetClockInfo(device, NVML_CLOCK_MEM, &clock);
        if (NVML_SUCCESS != result)
        {
            printf("Failed to get current NVML_CLOCK_MEM for device %i: %s\n", i, nvmlErrorString(result));
        }
        else
        {
            printf("     MEM： %6d Mhz   max clock ：%d   \n", clock, max_clock);
        }

        result = nvmlDeviceGetMaxClockInfo(device, NVML_CLOCK_VIDEO, &max_clock);
        if (NVML_SUCCESS != result)
        {
            printf("Failed to get max NVML_CLOCK_VIDEO for device %i: %s\n", i, nvmlErrorString(result));
        }

        result = nvmlDeviceGetClockInfo(device, NVML_CLOCK_VIDEO, &clock);
        if (NVML_SUCCESS != result)
        {
            printf("Failed to get current NVML_CLOCK_VIDEO for device %i: %s\n", i, nvmlErrorString(result));
        }
        else
        {
            printf("   VIDEO： %6d Mhz   max clock ：%d   \n", clock, max_clock);
        }
    }

    printf("-------------------------------------------------------------------- \n");

Error:
    result = nvmlShutdown();
    if (NVML_SUCCESS != result)
        printf("Failed to shutdown NVML: %s\n", nvmlErrorString(result));

    return 0;
}

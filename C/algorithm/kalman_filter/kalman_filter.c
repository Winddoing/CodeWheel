/* Copyright (C) 2025 wqshao All rights reserved.
 *
 *  File Name    : kalman_filter.c
 *  Author       : wqshao
 *  Created Time : 2025-06-06 18:14:08
 *  Description  :
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct {
    float q;    // 过程噪声协方差
    float r;    // 测量噪声协方差
    float x;    // 状态估计值
    float p;    // 估计误差协方差
    float k;    // 卡尔曼增益
} KalmanFilter;

// 初始化卡尔曼滤波器
void KalmanFilter_Init(KalmanFilter *kf, float q, float r, float initial_value, float initial_p) {
    kf->q = q;
    kf->r = r;
    kf->x = initial_value;
    kf->p = initial_p;
    kf->k = 0;
}

// 卡尔曼滤波函数
float KalmanFilter_Update(KalmanFilter *kf, float measurement) {
    /* 预测阶段 */
    // 状态预测：假设系统为恒定模型，因此状态保持不变
    // 误差协方差预测：增加过程噪声
    kf->p = kf->p + kf->q;

    /* 更新阶段 */
    // 计算卡尔曼增益
    kf->k = kf->p / (kf->p + kf->r);
    // 更新状态估计
    kf->x = kf->x + kf->k * (measurement - kf->x);
    // 更新误差协方差
    kf->p = (1 - kf->k) * kf->p;

    return kf->x;
}

// 封装函数：每次传入新测量值，返回滤波后的值
float filterValue(float new_measurement) {
    // 使用静态变量保存滤波器状态（首次调用时初始化）
    static KalmanFilter kf;
    static int is_initialized = 0;

    if (!is_initialized) {
        // 参数初始化（根据实际传感器特性调整）
        float process_noise = 0.01f;   // 过程噪声（模型不确定性）
        float sensor_noise = 0.1f;     // 测量噪声（传感器误差）
        float initial_value = 0;       // 初始估计值
        float initial_error = 1.0f;    // 初始估计误差

        KalmanFilter_Init(&kf, process_noise, sensor_noise, initial_value, initial_error);
        is_initialized = 1;
    }

    // 执行滤波并返回结果
    return KalmanFilter_Update(&kf, new_measurement);
}

/* 示例用法 */
int main() {
    // 模拟传感器数据（示例：实际应用中替换为真实数据）
    //float sensor_data[] = {10.2, 10.5, 10.3, 10.7, 10.6, 10.9, 11.0, 10.8, 11.2, 11.1};
    float sensor_data[] = {55, 45, 60, 59, 30, 40, 55, 58, 50, 60, 21, 44, 54, 58, 55, 56, 67, 54, 45, 44, 60, 61, 60, 51, 63, 52, 54, 55, 44, 54, 58, 59, 56, 55, 33, 43, 41, 55, 51, 53, 58, 56, 57, 55, 57};
    int num_samples = sizeof(sensor_data) / sizeof(sensor_data[0]);

    for (int i = 0; i < num_samples; i++) {
        float raw = sensor_data[i];
        float filtered = filterValue(raw);
        //printf("Raw: %.2f, Filtered: %.2f\n", raw, filtered);
        printf("%d,%.2f,%.2f,\n", i, raw, filtered);
    }

    return 0;
}

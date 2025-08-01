/* Copyright (C) 2025 wqshao All rights reserved.
 *
 *  File Name    : kalman_demo.c
 *  Author       : wqshao
 *  Created Time : 2025-06-09 09:35:58
 *  Description  :
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// 定义卡尔曼滤波器的状态
typedef struct {
    double x_hat;  // 当前帧率估计
    double v_hat;  // 帧率变化率估计
} KalmanState;

// 定义卡尔曼滤波器的参数
typedef struct {
    double A;  // 状态转移矩阵
    double B;  // 控制输入矩阵
    double H;  // 观测矩阵
    double Q;  // 过程噪声协方差
    double R;  // 测量噪声协方差
    double P;  // 估计误差协方差
    double K;  // 卡尔曼增益
} KalmanParams;

// 初始化卡尔曼滤波器
void kalman_init(KalmanState *state, KalmanParams *params) {
    state->x_hat = 0.0;
    state->v_hat = 0.0;
    params->A = 1.0;
    params->B = 0.0;
    params->H = 1.0;
    params->Q = 1e-5;
    params->R = 1e-2;
    params->P = 1.0;
}

// 卡尔曼滤波预测步骤
void kalman_predict(KalmanState *state, KalmanParams *params) {
    state->x_hat = params->A * state->x_hat + params->B * 0.0;
    params->P = params->A * params->P * params->A + params->Q;
}

// 卡尔曼滤波更新步骤
void kalman_update(KalmanState *state, KalmanParams *params, double measurement) {
    params->K = params->P * params->H / (params->H * params->P * params->H + params->R);
    state->x_hat = state->x_hat + params->K * (measurement - params->H * state->x_hat);
    params->P = (1 - params->K * params->H) * params->P;
}

int main() {
    KalmanState state;
    KalmanParams params;
    kalman_init(&state, &params);

    // 模拟测量值
    double measurements[] = {30.0, 32.0, 31.0, 33.0, 32.5, 34.0, 33.5};
    int num_measurements = sizeof(measurements) / sizeof(measurements[0]);

    for (int i = 0; i < num_measurements; i++) {
        kalman_predict(&state, &params);
        kalman_update(&state, &params, measurements[i]);
        printf("Measurement: %.2f, Estimated Frame Rate: %.2f\n", measurements[i], state.x_hat);
    }

    return 0;
}


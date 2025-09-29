/* Copyright (C) 2025 wqshao All rights reserved.
 *
 *  File Name    : crc16.c
 *  Author       : wqshao
 *  Created Time : 2025-08-06 09:31:01
 *  Description  :
 */
#include <stdio.h>
#include <stdint.h>

#define POLY 0x1021  //此值可自定义
#define INIT 0xFFFF  // 初始值
#define XOR_OUT 0x0000 // 结果异或值

// 直接计算法
uint16_t crc16_direct(uint8_t *data, size_t len)
{
	uint16_t crc = INIT;

	for (size_t i = 0; i < len; i++) {
		crc ^= (uint16_t)data[i] << 8;  // 数据移入高位

		for (int j = 0; j < 8; j++) {
			if (crc & 0x8000)
				crc = (crc << 1) ^ POLY;  // 模2除法
			else
				crc <<= 1;
		}
	}
	return crc ^ XOR_OUT;
}

// 查表法
uint16_t crc16_table(uint8_t *data, size_t len)
{
	static uint16_t table[256];
	static int initialized = 0;

	// 初始化CRC表
	if (!initialized) {
		for (uint16_t i = 0; i < 256; i++) {
			uint16_t crc = i << 8;
			for (int j = 0; j < 8; j++) {
				if (crc & 0x8000)
					crc = (crc << 1) ^ POLY;
				else
					crc <<= 1;
			}
			table[i] = crc;
		}
		initialized = 1;
	}

	// 计算CRC
	uint16_t crc = INIT;
	for (size_t i = 0; i < len; i++) {
		crc = (crc << 8) ^ table[(crc >> 8) ^ data[i]];
	}
	return crc ^ XOR_OUT;
}

// 验证：https://www.lammertbies.nl/comm/info/crc-calculation
// 测试数据：123456789
// CRC-CCITT (0xFFFF)	0x29B1
int main()
{
	char test_data[] = "123456789";  // 标准测试数据
	size_t len = sizeof(test_data) - 1;

	// 直接计算
	uint16_t crc_direct = crc16_direct((uint8_t *)test_data, len);
	printf("Direct CRC: 0x%04X\n", crc_direct);

	// 查表计算
	uint16_t crc_table = crc16_table((uint8_t *)test_data, len);
	printf("Table CRC:  0x%04X\n", crc_table);

	return 0;
}

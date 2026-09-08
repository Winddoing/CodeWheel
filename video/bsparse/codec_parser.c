/* Copyright (C) 2026 wqshao All rights reserved.
 *
 *  File Name    : codec_parser.c
 *  Author       : wqshao
 *  Created Time : 2026-02-26 15:46:13
 *  Description  :
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#if defined(__ARM_NEON__) || defined(__ARM_NEON)
#include <arm_neon.h>
#endif

// ======================== NALU类型枚举定义 ========================
/**
 * H.264 (AVC) NALU类型枚举
 * 参考标准：ITU-T H.264 (ISO/IEC 14496-10)
 */
typedef enum {
	AVC_NALU_UNSPECIFIED_0          = 0,    // 未指定
	AVC_NALU_SLICE_NON_IDR          = 1,    // 非IDR图像的编码切片
	AVC_NALU_SLICE_DPA              = 2,    // 数据分区A
	AVC_NALU_SLICE_DPB              = 3,    // 数据分区B
	AVC_NALU_SLICE_DPC              = 4,    // 数据分区C
	AVC_NALU_SLICE_IDR              = 5,    // IDR图像的编码切片
	AVC_NALU_SEI                    = 6,    // 补充增强信息
	AVC_NALU_SPS                    = 7,    // 序列参数集
	AVC_NALU_PPS                    = 8,    // 图像参数集
	AVC_NALU_AUD                    = 9,    // 访问单元分隔符
	AVC_NALU_EOSEQ                  = 10,   // 序列结束
	AVC_NALU_EOSTREAM               = 11,   // 码流结束
	AVC_NALU_FILLER                 = 12,   // 填充数据
	AVC_NALU_SPS_EXT                = 13,   // SPS扩展
	AVC_NALU_PREFIX                 = 14,   // 前缀NAL单元
	AVC_NALU_SUBSET_SPS             = 15,   // 子集SPS
	AVC_NALU_DEPTH_PARAM_SET        = 16,   // 深度参数集
	// 17-18: 保留
	AVC_NALU_SLICE_LAYER_EXT        = 19,   // 切片层扩展
	AVC_NALU_SLICE_EXT              = 20,   // 切片扩展
	// 21-23: 保留
	AVC_NALU_UNSPECIFIED_24_31      = 24    // 24-31: 未指定
} AVC_NALU_Type;

/**
 * H.265 (HEVC) NALU类型枚举
 * 参考标准：ITU-T H.265 (ISO/IEC 23008-2)
 */
typedef enum {
	HEVC_NALU_TRAIL_N               = 0,    // 尾迹N
	HEVC_NALU_TRAIL_R               = 1,    // 尾迹R
	HEVC_NALU_TSA_N                 = 2,    // TSA N
	HEVC_NALU_TSA_R                 = 3,    // TSA R
	HEVC_NALU_STSA_N                = 4,    // STSA N
	HEVC_NALU_STSA_R                = 5,    // STSA R
	HEVC_NALU_RADL_N                = 6,    // RADL N
	HEVC_NALU_RADL_R                = 7,    // RADL R
	HEVC_NALU_RASL_N                = 8,    // RASL N
	HEVC_NALU_RASL_R                = 9,    // RASL R
	// 10-15: 保留
	HEVC_NALU_BLA_W_LP              = 16,   // BLA W LP
	HEVC_NALU_BLA_W_RADL            = 17,   // BLA W RADL
	HEVC_NALU_BLA_N_LP              = 18,   // BLA N LP
	HEVC_NALU_IDR_W_RADL            = 19,   // IDR W RADL
	HEVC_NALU_IDR_N_LP              = 20,   // IDR N LP
	HEVC_NALU_CRA_NUT               = 21,   // CRA NUT
	// 22-23: 保留
	HEVC_NALU_IRAP_VCL22            = 22,   // IRAP VCL 22
	HEVC_NALU_IRAP_VCL23            = 23,   // IRAP VCL 23
	HEVC_NALU_NON_IRAP_VCL24        = 24,   // 非IRAP VCL 24
	HEVC_NALU_NON_IRAP_VCL25        = 25,   // 非IRAP VCL 25
	HEVC_NALU_NON_IRAP_VCL26        = 26,   // 非IRAP VCL 26
	HEVC_NALU_NON_IRAP_VCL27        = 27,   // 非IRAP VCL 27
	HEVC_NALU_NON_IRAP_VCL28        = 28,   // 非IRAP VCL 28
	HEVC_NALU_NON_IRAP_VCL29        = 29,   // 非IRAP VCL 29
	HEVC_NALU_NON_IRAP_VCL30        = 30,   // 非IRAP VCL 30
	HEVC_NALU_NON_IRAP_VCL31        = 31,   // 非IRAP VCL 31
	HEVC_NALU_VPS                   = 32,   // 视频参数集
	HEVC_NALU_SPS                   = 33,   // 序列参数集
	HEVC_NALU_PPS                   = 34,   // 图像参数集
	HEVC_NALU_AUD                   = 35,   // 访问单元分隔符
	HEVC_NALU_EOS                   = 36,   // 序列结束
	HEVC_NALU_EOB                   = 37,   // 比特流结束
	HEVC_NALU_FD                    = 38,   // 填充数据
	HEVC_NALU_SEI_PREFIX            = 39,   // SEI前缀
	HEVC_NALU_SEI_SUFFIX            = 40,   // SEI后缀
	// 41-47: 保留
	// 48-63: 未指定
} HEVC_NALU_Type;

// ======================== 辅助函数 ========================


/**
 * 快速判断缓冲区头部是否为NALU起始码
 * @param data 待检测的缓冲区（非NULL）
 * @param len 缓冲区长度（需至少 >=3，否则直接返回0）
 * @return 0: 不是起始码；3: 匹配3字节起始码(0x00 0x00 0x01)；4: 匹配4字节起始码(0x00 0x00 0x00 0x01)
 */
int is_nalu_start_code_at_head(const uint8_t *data, int len)
{
    // 基础合法性检查：缓冲区为空/长度不足3，直接判定不是起始码
    if (data == NULL || len < 3) {
        return 0;
    }

#if defined(__aarch64__) && (defined(__ARM_NEON__) || defined(__ARM_NEON))
    // -------------------------- ARM64 NEON 快速校验（单次指令） --------------------------
    // 加载前4字节到NEON寄存器（不足4字节也不影响，仅校验需要的位）
    uint8x4_t vec4 = vld1_u8(data);
    // 定义3字节/4字节起始码的参考向量
    const uint8_t start3_ref[4] = {0x00, 0x00, 0x01, 0x00}; // 前3字节为3码起始码
    const uint8_t start4_ref[4] = {0x00, 0x00, 0x00, 0x01}; // 4字节起始码
    uint8x4_t vec3_ref = vld1_u8(start3_ref);
    uint8x4_t vec4_ref = vld1_u8(start4_ref);

    // 批量比较：前4字节是否匹配4码起始码（一次指令完成4字节比较）
    uint8x4_t mask4 = vceq_u8(vec4, vec4_ref);
    if (vget_lane_u8(mask4, 0) && vget_lane_u8(mask4, 1) && vget_lane_u8(mask4, 2) && vget_lane_u8(mask4, 3)) {
        return 4; // 匹配4字节起始码
    }

    // 批量比较：前3字节是否匹配3码起始码（仅校验前3位）
    uint8x4_t mask3 = vceq_u8(vec4, vec3_ref);
    if (vget_lane_u8(mask3, 0) && vget_lane_u8(mask3, 1) && vget_lane_u8(mask3, 2)) {
        return 3; // 匹配3字节起始码
    }

#else
    // -------------------------- 通用平台 快速校验 --------------------------

#if 0
    // 先校验3字节起始码（最常用）
    if (data[0] == 0x00 && data[1] == 0x00 && data[2] == 0x01) {
        return 3;
    }
    // 若长度>=4，再校验4字节起始码
    if (len >= 4 && data[0] == 0x00 && data[1] == 0x00 && data[2] == 0x00 && data[3] == 0x01) {
        return 4;
    }
#else
    uint32_t sc_prefix = *(const uint32_t*)data;
    uint32_t sc_prefix_len = 0;
    uint32_t nal_hdr = 0;

    //printf("===> func: %s, line: %d, prefix = 0x%08x\n", __func__, __LINE__, prefix);

    // 0x00 0x00 0x00 0x01 转为 uint32_t 为 0x01000000
    // 0x00 0x00 0x01 转为 uint32_t 为 0x010000
    if ((sc_prefix & 0x01FFFFFF) == 0x01000000) {		// 匹配4字节特征值
	    sc_prefix_len = 4;
    } else if ((sc_prefix & 0x01FFFF) == 0x010000) {	// 匹配3字节特征值
	    sc_prefix_len = 3;
    }

    nal_hdr = data[sc_prefix_len];

    printf("nal hdr = 0x%02X\n", nal_hdr);

    return sc_prefix_len;
#endif

#endif
    // 不匹配任何起始码
    return 0;
}

/*
 * 查找下一个NALU起始码位置
 * @param data 码流数据
 * @param start_pos 起始查找位置
 * @param len 数据总长度
 * @return 起始码结束位置（即NALU类型字节的位置），未找到返回-1
 */
int find_next_nalu_start_code(const uint8_t *data, int start_pos, int len)
{
    // 入参合法性检查（通用逻辑，所有平台共享）
    if (data == NULL || start_pos < 0 || len <= 0 || start_pos >= len) {
        return -1;
    }

    // GCC ARM64 架构宏：__aarch64__ 是 GCC 识别 ARM64 的内置宏
#if defined(__aarch64__) && (defined(__ARM_NEON__) || defined(__ARM_NEON))
    // -------------------------- ARM64 NEON 加速版（修复类型转换） --------------------------
    const uint8_t *p = data + start_pos;
    const uint8_t *end = data + len;
    uint8x8_t zero8 = vdup_n_u8(0);  // 改为8字节0值向量，适配uint8x8_t类型

    // 批量处理（每次16字节），快速跳过非0x00区域
    while (p + 16 <= end) {
        // 加载16字节到NEON寄存器，拆分为两个8字节向量
        uint8x16_t vec16 = vld1q_u8(p);
        uint8x8_t vec8_low = vget_low_u8(vec16);  // 低8字节: [0-7]
        uint8x8_t vec8_high = vget_high_u8(vec16); // 高8字节: [8-15]

        // 批量比较：检测8字节中是否有0x00字节（返回掩码，有0x00则对应位为1）
        uint8x8_t mask_low = vceq_u8(vec8_low, zero8);
        uint8x8_t mask_high = vceq_u8(vec8_high, zero8);

        // 如果当前16字节中无0x00，直接跳过（vmaxv_u8取掩码最大值，0表示无0x00）
        if (vmaxv_u8(mask_low) == 0 && vmaxv_u8(mask_high) == 0) {
            p += 16;
            continue;
        }

        // 有0x00字节，逐字节检查当前16字节范围的起始码
        for (int i = 0; i < 16 && (p + i) < end - 2; i++) {
            const uint8_t *pos = p + i;
            // 检查3字节起始码 0x00 0x00 0x01
            if (pos[0] == 0x00 && pos[1] == 0x00 && pos[2] == 0x01) {
                return (pos - data) + 3;
            }
            // 检查4字节起始码 0x00 0x00 0x00 0x01（需确保长度足够）
            if ((pos + 3) < end && pos[0] == 0x00 && pos[1] == 0x00 && pos[2] == 0x00 && pos[3] == 0x01) {
                return (pos - data) + 4;
            }
        }

        p += 16;
    }

    // 处理剩余不足16字节的部分
    while (p < end - 2) {
        if (p[0] == 0x00 && p[1] == 0x00 && p[2] == 0x01) {
            return (p - data) + 3;
        }
        if (p + 3 < end && p[0] == 0x00 && p[1] == 0x00 && p[2] == 0x00 && p[3] == 0x01) {
            return (p - data) + 4;
        }
        p++;
    }

#else
    // -------------------------- 通用平台（非ARM64）原版逻辑 --------------------------
    for (int i = start_pos; i < len - 2; i++) {
        // 查找 0x00 0x00 0x01
        if (data[i] == 0x00 && data[i + 1] == 0x00 && data[i + 2] == 0x01) {
            return i + 3;
        }
        // 查找 0x00 0x00 0x00 0x01（需确保长度足够）
        if (i < len - 3 && data[i] == 0x00 && data[i + 1] == 0x00 && data[i + 2] == 0x00 && data[i + 3] == 0x01) {
            return i + 4;
        }
    }
#endif

    // 未找到起始码
    return -1;
}

/**
 * 获取AVC NALU类型名称
 */
const char *get_avc_nalu_type_name(AVC_NALU_Type type)
{
	switch (type) {
	case AVC_NALU_UNSPECIFIED_0:
		return "AVC_NALU_UNSPECIFIED_0";
	case AVC_NALU_SLICE_NON_IDR:
		return "AVC_NALU_SLICE_NON_IDR";
	case AVC_NALU_SLICE_DPA:
		return "AVC_NALU_SLICE_DPA";
	case AVC_NALU_SLICE_DPB:
		return "AVC_NALU_SLICE_DPB";
	case AVC_NALU_SLICE_DPC:
		return "AVC_NALU_SLICE_DPC";
	case AVC_NALU_SLICE_IDR:
		return "AVC_NALU_SLICE_IDR";
	case AVC_NALU_SEI:
		return "AVC_NALU_SEI";
	case AVC_NALU_SPS:
		return "AVC_NALU_SPS";
	case AVC_NALU_PPS:
		return "AVC_NALU_PPS";
	case AVC_NALU_AUD:
		return "AVC_NALU_AUD";
	case AVC_NALU_EOSEQ:
		return "AVC_NALU_EOSEQ";
	case AVC_NALU_EOSTREAM:
		return "AVC_NALU_EOSTREAM";
	case AVC_NALU_FILLER:
		return "AVC_NALU_FILLER";
	case AVC_NALU_SPS_EXT:
		return "AVC_NALU_SPS_EXT";
	case AVC_NALU_PREFIX:
		return "AVC_NALU_PREFIX";
	case AVC_NALU_SUBSET_SPS:
		return "AVC_NALU_SUBSET_SPS";
	case AVC_NALU_DEPTH_PARAM_SET:
		return "AVC_NALU_DEPTH_PARAM_SET";
	case AVC_NALU_SLICE_LAYER_EXT:
		return "AVC_NALU_SLICE_LAYER_EXT";
	case AVC_NALU_SLICE_EXT:
		return "AVC_NALU_SLICE_EXT";
	default:
		if (type >= 24 && type <= 31) return "AVC_NALU_UNSPECIFIED_24_31";
		return "AVC_NALU_UNKNOWN";
	}
}

/**
 * 获取HEVC NALU类型名称
 */
const char *get_hevc_nalu_type_name(HEVC_NALU_Type type)
{
	switch (type) {
	case HEVC_NALU_TRAIL_N:
		return "HEVC_NALU_TRAIL_N";
	case HEVC_NALU_TRAIL_R:
		return "HEVC_NALU_TRAIL_R";
	case HEVC_NALU_TSA_N:
		return "HEVC_NALU_TSA_N";
	case HEVC_NALU_TSA_R:
		return "HEVC_NALU_TSA_R";
	case HEVC_NALU_STSA_N:
		return "HEVC_NALU_STSA_N";
	case HEVC_NALU_STSA_R:
		return "HEVC_NALU_STSA_R";
	case HEVC_NALU_RADL_N:
		return "HEVC_NALU_RADL_N";
	case HEVC_NALU_RADL_R:
		return "HEVC_NALU_RADL_R";
	case HEVC_NALU_RASL_N:
		return "HEVC_NALU_RASL_N";
	case HEVC_NALU_RASL_R:
		return "HEVC_NALU_RASL_R";
	case HEVC_NALU_BLA_W_LP:
		return "HEVC_NALU_BLA_W_LP";
	case HEVC_NALU_BLA_W_RADL:
		return "HEVC_NALU_BLA_W_RADL";
	case HEVC_NALU_BLA_N_LP:
		return "HEVC_NALU_BLA_N_LP";
	case HEVC_NALU_IDR_W_RADL:
		return "HEVC_NALU_IDR_W_RADL";
	case HEVC_NALU_IDR_N_LP:
		return "HEVC_NALU_IDR_N_LP";
	case HEVC_NALU_CRA_NUT:
		return "HEVC_NALU_CRA_NUT";
	case HEVC_NALU_IRAP_VCL22:
		return "HEVC_NALU_IRAP_VCL22";
	case HEVC_NALU_IRAP_VCL23:
		return "HEVC_NALU_IRAP_VCL23";
	case HEVC_NALU_NON_IRAP_VCL24:
		return "HEVC_NALU_NON_IRAP_VCL24";
	case HEVC_NALU_NON_IRAP_VCL25:
		return "HEVC_NALU_NON_IRAP_VCL25";
	case HEVC_NALU_NON_IRAP_VCL26:
		return "HEVC_NALU_NON_IRAP_VCL26";
	case HEVC_NALU_NON_IRAP_VCL27:
		return "HEVC_NALU_NON_IRAP_VCL27";
	case HEVC_NALU_NON_IRAP_VCL28:
		return "HEVC_NALU_NON_IRAP_VCL28";
	case HEVC_NALU_NON_IRAP_VCL29:
		return "HEVC_NALU_NON_IRAP_VCL29";
	case HEVC_NALU_NON_IRAP_VCL30:
		return "HEVC_NALU_NON_IRAP_VCL30";
	case HEVC_NALU_NON_IRAP_VCL31:
		return "HEVC_NALU_NON_IRAP_VCL31";
	case HEVC_NALU_VPS:
		return "HEVC_NALU_VPS";
	case HEVC_NALU_SPS:
		return "HEVC_NALU_SPS";
	case HEVC_NALU_PPS:
		return "HEVC_NALU_PPS";
	case HEVC_NALU_AUD:
		return "HEVC_NALU_AUD";
	case HEVC_NALU_EOS:
		return "HEVC_NALU_EOS";
	case HEVC_NALU_EOB:
		return "HEVC_NALU_EOB";
	case HEVC_NALU_FD:
		return "HEVC_NALU_FD";
	case HEVC_NALU_SEI_PREFIX:
		return "HEVC_NALU_SEI_PREFIX";
	case HEVC_NALU_SEI_SUFFIX:
		return "HEVC_NALU_SEI_SUFFIX";
	default:
		return "HEVC_NALU_UNKNOWN";
	}
}

/**
 * 解析码流类型并输出NALU信息
 * @param data 第一帧码流数据（包含SPS/PPS/I帧）
 * @param len 数据长度
 * @return 0: H.264, 1: H.265, -1: 解析失败
 */
int parse_codec_type(const uint8_t *data, int len)
{
	// 1. 校验输入
	if (data == NULL || len <= 0) {
		printf("错误：输入数据为空或长度无效\n");
		return -1;
	}

	printf("=====================================\n");
	printf("码流解析调试信息：\n");
	printf("  正在遍历第一帧中的NALU单元...\n");

	int current_pos = 0;
	int nalu_count = 0;
	int codec_result = -1; // -1:未知, 0:AVC, 1:HEVC

	// 2. 遍历所有NALU，直到找到可识别的类型
	while (1) {
		// 查找下一个NALU起始码
		int nalu_pos = find_next_nalu_start_code(data, current_pos, len);
		if (nalu_pos == -1 || nalu_pos >= len) {
			break;
		}

		nalu_count++;
		uint8_t nalu_byte1 = data[nalu_pos];

		// H.264: NALU类型是第一个字节的低5位 (bit0-bit4)
		uint8_t avc_nalu_type = nalu_byte1 & 0x1F;

		// H.265: 第一个字节的高6位是NALU类型 (bit1-bit6)
		uint8_t hevc_nalu_type = ((nalu_byte1 >> 1) & 0x3F);

		printf("  NALU #%d: 位置=0x%04X(0x%02X)\n", nalu_count, nalu_pos, nalu_byte1);
		printf("    AVC类型: 0x%02X (%d) -> %s\n", avc_nalu_type, avc_nalu_type,
				get_avc_nalu_type_name((AVC_NALU_Type)avc_nalu_type));
		printf("    HEVC类型: 0x%02X (%d) -> %s\n", hevc_nalu_type, hevc_nalu_type,
				get_hevc_nalu_type_name((HEVC_NALU_Type)hevc_nalu_type));

		// 3. 核心判断逻辑（增强版）
		// 判断为H.264的条件：
		// - 找到SPS/PPS/IDR（强特征）
		// - 找到AVC特有类型（如AUD且HEVC类型不在HEVC有效范围）
		if (avc_nalu_type <= 31 && (avc_nalu_type == AVC_NALU_SPS || avc_nalu_type == AVC_NALU_PPS ||
				avc_nalu_type == AVC_NALU_SLICE_IDR || avc_nalu_type == AVC_NALU_AUD)) {
			codec_result = 0;
			break; // 找到明确特征，退出遍历
		}
		// 判断为H.265的条件
		else if (hevc_nalu_type < 41 && (hevc_nalu_type == HEVC_NALU_SPS || hevc_nalu_type == HEVC_NALU_PPS ||
				hevc_nalu_type == HEVC_NALU_VPS || hevc_nalu_type == HEVC_NALU_AUD ||
				hevc_nalu_type == HEVC_NALU_IDR_N_LP || hevc_nalu_type == HEVC_NALU_IDR_W_RADL )) {
			codec_result = 1;
			break; // 找到明确特征，退出遍历
		}

		// 继续查找下一个NALU
		current_pos = nalu_pos + 1;
		// 防止死循环，最多遍历100个NALU
		if (nalu_count >= 100) {
			printf("  警告：已遍历100个NALU仍未找到明确特征\n");
			break;
		}
	}

	// 4. 输出最终判断结果
	if (codec_result == 0) {
		printf("  码流类型：H.264 (AVC)\n");
		// 输出AVC额外信息
		if (nalu_count > 0) {
			int first_nalu_pos = find_next_nalu_start_code(data, 0, len);
			if (first_nalu_pos != -1 && first_nalu_pos < len) {
				uint8_t nalu_byte1 = data[first_nalu_pos];
				uint8_t avc_ref_idc = (nalu_byte1 >> 5) & 0x03;
				printf("  AVC参考标识(RefIdc)：%d\n", avc_ref_idc);
			}
		}
	} else if (codec_result == 1) {
		printf("  码流类型：H.265 (HEVC)\n");
		// 输出HEVC额外信息
		if (nalu_count > 0) {
			int first_nalu_pos = find_next_nalu_start_code(data, 0, len);
			if (first_nalu_pos != -1 && first_nalu_pos + 1 < len) {
				uint8_t nalu_byte1 = data[first_nalu_pos];
				uint8_t nalu_byte2 = data[first_nalu_pos + 1];
				uint8_t hevc_layer_id = ((nalu_byte1 & 0x01) << 6) | ((nalu_byte2 >> 2) & 0x3F);
				uint8_t hevc_temporal_id = nalu_byte2 & 0x03;
				printf("  HEVC层ID：%d\n", hevc_layer_id);
				printf("  HEVC时间ID：%d\n", hevc_temporal_id);
			}
		}
	} else {
		printf("  码流类型：无法识别\n");
	}
	printf("=====================================\n");

	return codec_result;
}

/**
 * 从帧长度文件读取第一帧的长度
 * @param frame_len_file 帧长度文件路径
 * @return 第一帧长度（字节数），失败返回-1
 * 注：帧长度文件格式要求：每行一个数字，表示对应帧的字节长度
 */
int read_first_frame_length(const char *frame_len_file)
{
	FILE *fp = fopen(frame_len_file, "r");
	if (fp == NULL) {
		printf("错误：无法打开帧长度文件 %s\n", frame_len_file);
		return -1;
	}

	int first_frame_len = -1;
	// 读取第一行的数字（第一帧长度）
	if (fscanf(fp, "%d", &first_frame_len) != 1) {
		printf("错误：读取帧长度文件失败，文件格式可能不正确\n");
		fclose(fp);
		return -1;
	}

	fclose(fp);

	// 校验帧长度合法性
	if (first_frame_len <= 0) {
		printf("错误：读取到的第一帧长度无效 (%d)\n", first_frame_len);
		return -1;
	}

	printf("成功读取第一帧长度：%d 字节\n", first_frame_len);
	return first_frame_len;
}

/**
 * 从码流文件读取第一帧数据
 * @param stream_file 码流文件路径
 * @param frame_len 要读取的第一帧长度
 * @param out_data 输出缓冲区（需要调用者释放内存）
 * @return 实际读取的字节数，失败返回-1
 */
int read_first_frame_data(const char *stream_file, int frame_len, uint8_t **out_data)
{
	if (stream_file == NULL || frame_len <= 0 || out_data == NULL) {
		printf("错误：输入参数无效\n");
		return -1;
	}

	FILE *fp = fopen(stream_file, "rb");
	if (fp == NULL) {
		printf("错误：无法打开码流文件 %s\n", stream_file);
		return -1;
	}

	// 分配内存
	*out_data = (uint8_t *)malloc(frame_len);
	if (*out_data == NULL) {
		printf("错误：内存分配失败\n");
		fclose(fp);
		return -1;
	}

	// 读取第一帧数据
	size_t read_bytes = fread(*out_data, 1, frame_len, fp);
	fclose(fp);

	if (read_bytes != frame_len) {
		printf("错误：读取码流数据失败，期望 %d 字节，实际读取 %zu 字节\n", frame_len, read_bytes);
		free(*out_data);
		*out_data = NULL;
		return -1;
	}

	printf("成功读取第一帧码流数据：%zu 字节\n", read_bytes);
	return (int)read_bytes;
}

// ======================== 主函数 ========================
int main(int argc, char *argv[])
{
	// 检查命令行参数
	if (argc != 2) {
		printf("使用方法：%s <码流文件路径>\n", argv[0]);
		printf("示例：%s video.h264\n", argv[0]);
		return -1;
	}

	const char *stream_file = argv[1];

	printf("开始处理：\n");
	printf("  码流文件：%s\n", stream_file);
	printf("-------------------------------------\n");

	// 1. 第一帧长度
	int first_frame_len = 512;

	// 2. 读取第一帧码流数据
	uint8_t *first_frame_data = NULL;
	int read_len = read_first_frame_data(stream_file, first_frame_len, &first_frame_data);
	if (read_len <= 0 || first_frame_data == NULL) {
		return -1;
	}

	int nal_hdr_size = is_nalu_start_code_at_head(first_frame_data, 4);
	printf("nal_hdr_size = %d\n", nal_hdr_size);

	// 3. 解析码流类型（仅处理第一帧）
	int codec_type = parse_codec_type(first_frame_data, read_len);

	// 输出最终结果
	printf("\n最终结果：");
	switch (codec_type) {
	case 0:
		printf("识别为 H.264 (AVC) 码流\n");
		break;
	case 1:
		printf("识别为 H.265 (HEVC) 码流\n");
		break;
	default:
		printf("无法识别码流类型\n");
		break;
	}

	// 4. 释放内存
	free(first_frame_data);
	first_frame_data = NULL;

	return 0;
}

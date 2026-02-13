/* Copyright (C) 2026 wqshao All rights reserved.
 *
 *  File Name    : tile2linear.c
 *  Author       : wqshao
 *  Created Time : 2026-02-09 15:20:53
 *  Description  :
 *  将YUV数据的存储格式转换
 *   将Tile格式（块存储）转换为线性格式存储。
 *   Tile32x4/Tile64x4,其内部均是4x4的块排列
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>  // getopt/optarg 头文件

// 色彩格式枚举
typedef enum {
	FORMAT_NV12,
	FORMAT_NV16,
	FORMAT_I444
} ImageFormat;

// 位深枚举
typedef enum {
	BIT_DEPTH_8,
	BIT_DEPTH_10
} BitDepth;

// 转换配置结构体
typedef struct {
	int width;          // 图像宽度（像素）
	int height;         // 图像高度（像素）
	int frame_count;        // 总帧数
	ImageFormat format;     // 色彩格式
	BitDepth bit_depth;     // 位深
	const char *input_path;     // 输入文件路径
	const char *output_path;    // 输出文件路径
} ConvertConfig;

/**
 * @brief 计算单个像素的字节数
 * @param bit_depth 位深
 * @return 每个像素占用的字节数
 */
static int get_pixel_bytes(BitDepth bit_depth)
{
	return (bit_depth == BIT_DEPTH_8) ? 1 : 2;
}

/**
 * @brief tile64x4转线性（4x4子块优先存储）
 *  子块在tile内的偏移计算（子块优先）
 * @param tile_data tile格式输入数据
 * @param linear_data 线性格式输出数据
 * @param width 平面宽度
 * @param height 平面高度
 * @param pixel_bytes 每个像素字节数
 *
 * 4x4 block layout:
 * Y:
 *   Yx0, Yx1, Yx2, Yx3
 *   Yx4, Yx5, Yx6, Yx7
 *   Yx8, Yx9, YxA, YxB
 *   YxC, YxD, YxE, YxF
 * UV:
 *   Cbx0, Crx0, Cbx1, Crx1
 *   Cbx2, Crx2, Cbx3, Crx3
 *   Cbx4, Crx4, Cbx5, Crx5
 *   Cbx6, Crx6, Cbx7, Crx7
 */
static void tile64x4_to_linear(const uint8_t *tile_data, uint8_t *linear_data,
		int width, int height, int pixel_bytes)
{
	int tile_w = 64;        // tile宽度
	int tile_h = 4;         // tile高度
	int subblock_w = 4;     // 子块宽度
	int subblock_h = 4;     // 子块高度
	int subblock_size = subblock_w * subblock_h * pixel_bytes; // 单个4x4子块的字节数
	int subblocks_per_tile_row = tile_w / subblock_w; // 每个tile行的子块数（64/4=16）

	// 遍历所有64x4tile（图像级行优先）
	for (int tile_y = 0; tile_y < height; tile_y += tile_h) {
		for (int tile_x = 0; tile_x < width; tile_x += tile_w) {
			// 计算当前tile在输入数据中的起始偏移（所有tile按行优先排列）
			int tile_idx = (tile_y / tile_h) * (width / tile_w) + (tile_x / tile_w);
			const uint8_t *tile_base = tile_data + tile_idx * tile_w * tile_h * pixel_bytes;

			// 遍历当前tile内的所有4x4子块（子块级行优先）
			for (int sb_tile_y = 0; sb_tile_y < tile_h; sb_tile_y += subblock_h) {
				for (int sb_tile_x = 0; sb_tile_x < tile_w; sb_tile_x += subblock_w) {
					// 1. 计算子块在图像中的绝对坐标
					int sb_abs_x = tile_x + sb_tile_x;
					int sb_abs_y = tile_y + sb_tile_y;
					// 子块的有效宽高（处理tile边界的子块）
					int curr_sb_w = (sb_abs_x + subblock_w) > width ? (width - sb_abs_x) : subblock_w;
					int curr_sb_h = (sb_abs_y + subblock_h) > height ? (height - sb_abs_y) : subblock_h;

					// 2. 计算子块在tile内的偏移（子块优先存储）
					// 子块在tile内的行索引（0~0，因为tile_h=4，subblock_h=4）
					int sb_idx_row = sb_tile_y / subblock_h;
					// 子块在tile内的列索引（0~15）
					int sb_idx_col = sb_tile_x / subblock_w;
					// 子块在tile内的编号 = 行索引×每行子块数 + 列索引
					int sb_idx_in_tile = sb_idx_row * subblocks_per_tile_row + sb_idx_col;
					// 子块在tile数据中的起始偏移 = 子块编号 × 子块大小
					const uint8_t *subblock_base = tile_base + sb_idx_in_tile * subblock_size;

#if 0
					// 3. 遍历子块内的每个像素（子块内光栅扫描）
					for (int y = 0; y < curr_sb_h; y++) {
						for (int x = 0; x < curr_sb_w; x++) {
							// 像素在图像中的绝对坐标
							int pixel_abs_x = sb_abs_x + x;
							int pixel_abs_y = sb_abs_y + y;
							// 像素在线性存储中的偏移
							int linear_offset = (pixel_abs_y * width + pixel_abs_x) * pixel_bytes;
							// 像素在子块内的偏移（子块内行优先）
							int subblock_pixel_offset = (y * subblock_w + x) * pixel_bytes;

							// 3.1. 复制像素数据（从子块正确位置→线性正确位置）
							memcpy(linear_data + linear_offset,
									subblock_base + subblock_pixel_offset,
									pixel_bytes);
						}
					}
#else
					// 3. 遍历子块内的每一行（按行批量拷贝）
					int per_copy_pixels = 4;        // 每次批量拷贝的像素数（固定4个）
					int per_copy_bytes = per_copy_pixels * pixel_bytes; // 每次拷贝的字节数
					for (int y = 0; y < curr_sb_h; y++) {
						// 子块内当前行的起始偏移
						const uint8_t *sb_row_base = subblock_base + y * subblock_w * pixel_bytes;
						// 线性存储中当前行的起始偏移
						int linear_row = sb_abs_y + y;
						int linear_col_start = sb_abs_x;
						int linear_row_base = (linear_row * width + linear_col_start) * pixel_bytes;

						// 批量拷贝：按4个像素为单位拷贝（减少memcpy调用）
						int x = 0;
						// 3.1. 先拷贝完整的4像素块
						for (; x + per_copy_pixels <= curr_sb_w; x += per_copy_pixels) {
							memcpy(linear_data + linear_row_base + x * pixel_bytes,
									sb_row_base + x * pixel_bytes,
									per_copy_bytes);
						}
						// 3.2. 处理剩余不足4个的像素（边界情况）
						if (x < curr_sb_w) {
							int remain_pixels = curr_sb_w - x;
							int remain_bytes = remain_pixels * pixel_bytes;
							memcpy(linear_data + linear_row_base + x * pixel_bytes,
									sb_row_base + x * pixel_bytes,
									remain_bytes);
						}
					}
#endif
				}
			}
		}
	}
}

/**
 * @brief NV12格式转换（4:2:0）
 */
static void convert_nv12(const ConvertConfig *config,
		const uint8_t *input_data,
		uint8_t *output_data)
{
	int pixel_bytes = get_pixel_bytes(config->bit_depth);
	int y_plane_size = config->width * config->height * pixel_bytes;

	// 转换Y平面
	tile64x4_to_linear(input_data, output_data,
			config->width, config->height, pixel_bytes);

	// 转换UV平面（UV交织）
	const uint8_t *input_uv = input_data + y_plane_size;
	uint8_t *output_uv = output_data + y_plane_size;
	// 正确参数：
	// - 宽度 = 图像宽度W（UV平面每行有W个样本：W/2个对 × 2样本/对）
	// - 高度 = 图像高度H/2（共H/2行样本）
	// - pixel_bytes = 单样本字节数（与Y平面一致）
	tile64x4_to_linear(input_uv, output_uv,
			config->width, config->height / 2, pixel_bytes);
}

/**
 * @brief NV16格式转换（4:2:2）
 */
static void convert_nv16(const ConvertConfig *config,
		const uint8_t *input_data,
		uint8_t *output_data)
{
	int pixel_bytes = get_pixel_bytes(config->bit_depth);
	int y_plane_size = config->width * config->height * pixel_bytes;

	// 转换Y平面
	tile64x4_to_linear(input_data, output_data,
			config->width, config->height, pixel_bytes);

	// 转换UV平面（4:2:2布局：W×H个样本，每行W/2个对）
	const uint8_t *input_uv = input_data + y_plane_size;
	uint8_t *output_uv = output_data + y_plane_size;
	tile64x4_to_linear(input_uv, output_uv,
			config->width, config->height, pixel_bytes);
}

/**
 * @brief I444格式转换（4:4:4）
 */
static void convert_i444(const ConvertConfig *config,
		const uint8_t *input_data,
		uint8_t *output_data)
{
	int pixel_bytes = get_pixel_bytes(config->bit_depth);
	int plane_size = config->width * config->height * pixel_bytes;

	// 转换Y平面
	tile64x4_to_linear(input_data, output_data,
			config->width, config->height, pixel_bytes);

	// 转换U平面
	const uint8_t *input_u = input_data + plane_size;
	uint8_t *output_u = output_data + plane_size;
	tile64x4_to_linear(input_u, output_u,
			config->width, config->height, pixel_bytes);

	// 转换V平面
	const uint8_t *input_v = input_data + 2 * plane_size;
	uint8_t *output_v = output_data + 2 * plane_size;
	tile64x4_to_linear(input_v, output_v,
			config->width, config->height, pixel_bytes);
}

/**
 * @brief 计算单帧文件大小
 * @param config 转换配置
 * @return 单帧字节数
 */
static size_t calculate_frame_size(const ConvertConfig *config)
{
	int pixel_bytes = get_pixel_bytes(config->bit_depth);
	int y_plane_size = config->width * config->height * pixel_bytes;

	if (config->format == FORMAT_NV12) {
		int uv_plane_size = (config->width / 2) * (config->height / 2) * pixel_bytes * 2;
		return y_plane_size + uv_plane_size;
	} else if (config->format == FORMAT_NV16) {
		int uv_plane_size = config->width * (config->height / 2) * pixel_bytes * 2;
		return y_plane_size + uv_plane_size;
	} else if (config->format == FORMAT_I444) {
		return 3 * y_plane_size;
	}
	return 0;
}

/**
 * @brief 执行多帧图像格式转换
 * @param config 转换配置
 * @return 0成功，非0失败
 */
int convert_tile64x4_to_linear(const ConvertConfig *config)
{
	if (!config || !config->input_path || !config->output_path) {
		fprintf(stderr, "无效的配置参数\n");
		return -1;
	}

	// 1. 计算单帧大小
	size_t frame_size = calculate_frame_size(config);
	if (frame_size == 0) {
		fprintf(stderr, "不支持的图像格式\n");
		return -2;
	}

	// 2. 打开输入/输出文件（流式读写）
	FILE *in_file = fopen(config->input_path, "rb");
	if (!in_file) {
		fprintf(stderr, "无法打开输入文件: %s\n", config->input_path);
		return -4;
	}
	FILE *out_file = fopen(config->output_path, "wb");
	if (!out_file) {
		fprintf(stderr, "无法打开输出文件: %s\n", config->output_path);
		fclose(in_file);
		return -6;
	}

	// 3. 获取输入文件总大小
	fseek(in_file, 0, SEEK_END);
	size_t total_file_size = ftell(in_file);
	fseek(in_file, 0, SEEK_SET);

	// 4. 确定总帧数
	int total_frames = config->frame_count;
	if (total_frames <= 0) {
		if (total_file_size % frame_size != 0) {
			fprintf(stderr, "输入文件大小异常：总大小%zu字节，单帧%zu字节，无法整除\n",
					total_file_size, frame_size);
			fclose(in_file);
			fclose(out_file);
			return -7;
		}
		total_frames = total_file_size / frame_size;
		printf("自动识别帧数：%d帧（总大小%zu ÷ 单帧%zu）\n", total_frames, total_file_size, frame_size);
	} else {
		size_t expected_file_size = (size_t)total_frames * frame_size;
		if (expected_file_size > total_file_size) {
			fprintf(stderr, "指定帧数%d超出文件容量：预期%zu字节，实际%zu字节\n",
					total_frames, expected_file_size, total_file_size);
			fclose(in_file);
			fclose(out_file);
			return -8;
		}
	}

	// 5. 分配单帧缓冲区
	uint8_t *input_frame = (uint8_t *)malloc(frame_size);
	uint8_t *output_frame = (uint8_t *)malloc(frame_size);
	if (!input_frame || !output_frame) {
		fprintf(stderr, "内存分配失败\n");
		free(input_frame);
		free(output_frame);
		fclose(in_file);
		fclose(out_file);
		return -3;
	}

	// 6. 逐帧转换
	for (int frame_idx = 0; frame_idx < total_frames; frame_idx++) {
		size_t read_size = fread(input_frame, 1, frame_size, in_file);
		if (read_size != frame_size) {
			fprintf(stderr, "读取第%d帧失败：预期%zu字节，实际%zu字节\n",
					frame_idx + 1, frame_size, read_size);
			free(input_frame);
			free(output_frame);
			fclose(in_file);
			fclose(out_file);
			return -5;
		}

		memset(output_frame, 0, frame_size);
		if (config->format == FORMAT_NV12) {
			convert_nv12(config, input_frame, output_frame);
		} else if (config->format == FORMAT_NV16) {
			convert_nv16(config, input_frame, output_frame);
		} else if (config->format == FORMAT_I444) {
			convert_i444(config, input_frame, output_frame);
		}

		fwrite(output_frame, 1, frame_size, out_file);

		if ((frame_idx + 1) % 10 == 0 || frame_idx + 1 == total_frames) {
			printf("已处理：%d/%d帧\n", frame_idx + 1, total_frames);
		}
	}

	// 7. 释放资源
	free(input_frame);
	free(output_frame);
	fclose(in_file);
	fclose(out_file);
	printf("多帧转换完成！输出文件: %s（共%d帧）\n", config->output_path, total_frames);
	return 0;
}

/**
 * @brief 打印使用帮助
 */
static void print_usage(const char *prog_name)
{
	printf("用法: %s [参数]\n", prog_name);
	printf("参数说明:\n");
	printf("  -w <宽度>        图像宽度（像素，必须是64的整数倍）\n");
	printf("  -h <高度>        图像高度（像素，必须是4的整数倍）\n");
	printf("  -f <格式>        图像格式（nv12/nv16/i444）\n");
	printf("  -b <位深>        位深（8/10）\n");
	printf("  -n <帧数>        总帧数（可选，不指定则自动计算）\n");
	printf("  -i <输入文件>    tile64x4格式的多帧输入文件路径\n");
	printf("  -o <输出文件>    线性格式的多帧输出文件路径\n");
	printf("示例:\n");
	printf("  # 单帧转换\n");
	printf("  %s -w 1920 -h 1080 -f nv12 -b 8 -n 1 -i input_tile.nv12 -o output_linear.nv12\n", prog_name);
	printf("  # 多帧转换（手动指定帧数）\n");
	printf("  %s -w 1920 -h 1080 -f nv16 -b 10 -n 100 -i input_tile.nv16 -o output_linear.nv16\n", prog_name);
	printf("  # 多帧转换（自动计算帧数）\n");
	printf("  %s -w 1280 -h 720 -f i444 -b 8 -i input_tile_multi.i444 -o output_linear_multi.i444\n", prog_name);
}

int main(int argc, char *argv[])
{
	ConvertConfig config = {0};
	int opt;

	// 解析命令行参数（原有逻辑不变）
	while ((opt = getopt(argc, argv, "w:h:f:b:n:i:o:")) != -1) {
		switch (opt) {
		case 'w':
			config.width = atoi(optarg);
			break;
		case 'h':
			config.height = atoi(optarg);
			break;
		case 'f':
			if (strcmp(optarg, "nv12") == 0) {
				config.format = FORMAT_NV12;
			} else if (strcmp(optarg, "nv16") == 0) {
				config.format = FORMAT_NV16;
			} else if (strcmp(optarg, "i444") == 0) {
				config.format = FORMAT_I444;
			} else {
				fprintf(stderr, "不支持的格式：%s，仅支持nv12/nv16/i444\n", optarg);
				print_usage(argv[0]);
				return -1;
			}
			break;
		case 'b':
			if (atoi(optarg) == 8) {
				config.bit_depth = BIT_DEPTH_8;
			} else if (atoi(optarg) == 10) {
				config.bit_depth = BIT_DEPTH_10;
			} else {
				fprintf(stderr, "不支持的位深：%s，仅支持8/10\n", optarg);
				print_usage(argv[0]);
				return -1;
			}
			break;
		case 'n':
			config.frame_count = atoi(optarg);
			break;
		case 'i':
			config.input_path = optarg;
			break;
		case 'o':
			config.output_path = optarg;
			break;
		default:
			print_usage(argv[0]);
			return -1;
		}
	}

	// 校验基础参数
	if (config.width <= 0 || config.height <= 0 ||
			config.input_path == NULL || config.output_path == NULL) {
		fprintf(stderr, "参数不完整！\n");
		print_usage(argv[0]);
		return -1;
	}

	if (config.width % 64 != 0 || config.height % 4 != 0) {
		fprintf(stderr, "宽度必须是64的整数倍，高度必须是4的整数倍！\n");
		return -1;
	}

	// 执行转换
	return convert_tile64x4_to_linear(&config);
}

/*
 * gen_test_pattern：生成 1080p BGRA 测试图（无外部依赖）。
 *
 * 布局（1920×1080，BGRX 字节序）：
 *   · y∈[0,240)      SMPTE 100% 彩条 8 色条（白黄青绿品红红蓝黑）
 *   · y∈[240,480)    4 块 × 480px：纯色 5 竖条 / 棋盘格 / 细密竖条纹 / 45°斜线
 *   · y∈[480,1080)   左半点阵文字（白底黑字 "1080p" "BGRA TEST"），
 *                    右半水平渐变叠加粗斜线
 *
 * 用途：gpgpu BGRA→NV12 用彩条/纯色做像素级期望值比对、棋盘/条纹验证
 * 色度抽样位置；npu 超分用文字/条纹/斜线/渐变做细节与边缘质量目视。
 *
 * 编译运行：
 *   gcc -O2 -o gen_test_pattern tools/gen_test_pattern.c
 *   ./gen_test_pattern [out_path]      # 默认 resources/raw/pattern_1080p_bgra.raw
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define W  1920
#define H  1080

/* 输出 BGR 序：px[i+0]=B px[i+1]=G px[i+2]=R，A 恒 0xff */
static void fill_rect(uint8_t *bgra, int x0, int y0, int x1, int y1,
                      uint8_t b, uint8_t g, uint8_t r)
{
	for (int y = y0; y < y1; y++)
		for (int x = x0; x < x1; x++) {
			size_t o = ((size_t)y * W + x) * 4;
			bgra[o + 0] = b;
			bgra[o + 1] = g;
			bgra[o + 2] = r;
			bgra[o + 3] = 0xff;
		}
}

/* 按条件置黑/白（条件 true=黑 0，false=白 255），供棋盘/条纹/斜线复用 */
static void fill_cond(uint8_t *bgra, int x0, int y0, int x1, int y1,
                      int (*on)(int, int, void *), void *arg)
{
	for (int y = y0; y < y1; y++)
		for (int x = x0; x < x1; x++) {
			size_t o = ((size_t)y * W + x) * 4;
			uint8_t v = on(x, y, arg) ? 0 : 255;
			bgra[o + 0] = v;
			bgra[o + 1] = v;
			bgra[o + 2] = v;
			bgra[o + 3] = 0xff;
		}
}

struct checker_arg { int x0, y0, cell; };

static int checker_on(int x, int y, void *arg)
{
	struct checker_arg *a = arg;
	return (((x - a->x0) / a->cell + (y - a->y0) / a->cell) & 1) != 0;
}

struct grating_arg { int x0, period; };

static int grating_on(int x, int y, void *arg)
{
	struct grating_arg *a = arg;
	(void)y;
	return (((x - a->x0) / a->period) & 1) != 0;
}

struct diag_arg { int x0, y0, width; };

static int diag_on(int x, int y, void *arg)
{
	struct diag_arg *a = arg;
	return (((x - a->x0) + (y - a->y0)) / a->width & 1) != 0;
}

/* 5×7 点阵字体（每字符 7 行，bit4..0 自左向右为列） */
struct glyph {
	char     c;
	uint8_t  row[7];
};

static const struct glyph font[] = {
	{ '0', { 0x0e, 0x11, 0x13, 0x15, 0x19, 0x11, 0x0e } },
	{ '1', { 0x04, 0x0c, 0x04, 0x04, 0x04, 0x04, 0x0e } },
	{ '8', { 0x0e, 0x11, 0x11, 0x0e, 0x11, 0x11, 0x0e } },
	{ 'p', { 0x1e, 0x11, 0x11, 0x1e, 0x10, 0x10, 0x10 } },
	{ 'A', { 0x0e, 0x11, 0x11, 0x1f, 0x11, 0x11, 0x11 } },
	{ 'B', { 0x1e, 0x11, 0x11, 0x1e, 0x11, 0x11, 0x1e } },
	{ 'E', { 0x1f, 0x10, 0x10, 0x1e, 0x10, 0x10, 0x1f } },
	{ 'G', { 0x0e, 0x11, 0x10, 0x10, 0x13, 0x11, 0x0e } },
	{ 'R', { 0x1e, 0x11, 0x11, 0x1e, 0x14, 0x12, 0x11 } },
	{ 'S', { 0x0e, 0x11, 0x10, 0x0e, 0x01, 0x11, 0x0e } },
	{ 'T', { 0x1f, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04 } },
	{ ' ', { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
};

static void draw_text(uint8_t *bgra, int x0, int y0, int scale,
                      const char *s, uint8_t b, uint8_t g, uint8_t r)
{
	/* 字符间空 1 列 × scale，字形高 7 行 × scale */
	for (const char *p = s; *p; p++) {
		const struct glyph *gl = NULL;

		for (size_t i = 0; i < sizeof(font) / sizeof(font[0]); i++)
			if (font[i].c == *p) {
				gl = &font[i];
				break;
			}
		if (!gl)
			continue;
		for (int gy = 0; gy < 7; gy++)
			for (int gx = 0; gx < 5; gx++) {
				if (!(gl->row[gy] & (1 << (4 - gx))))
					continue;
				fill_rect(bgra, x0 + gx * scale, y0 + gy * scale,
				          x0 + (gx + 1) * scale, y0 + (gy + 1) * scale,
				          b, g, r);
			}
		x0 += 6 * scale;
	}
}

static void usage(const char *prog)
{
	printf("usage: %s [out_path]\n", prog);
	printf("  生成 1920x1080 BGRA 测试图（BGRX 字节序，无文件头），覆盖:\n");
	printf("    SMPTE 100%% 彩条 / 纯色 5 条 / 8px 棋盘格 / 4px 竖条纹 /\n");
	printf("    45 度斜线 / 点阵文字 / 垂直渐变\n");
	printf("  默认 out_path: resources/raw/pattern_1080p_bgra.raw\n");
	printf("  接法: test_gpgpu <out_path> 1920x1080 bgra out.nv12 [frames]\n");
}

int main(int argc, char **argv)
{
	const char *out_path = "resources/raw/pattern_1080p_bgra.raw";

	if (argc > 1) {
		if (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help")) {
			usage(argv[0]);
			return 0;
		}
		out_path = argv[1];
	}
	uint8_t *bgra = malloc((size_t)W * H * 4);

	if (!bgra)
		return 1;
	memset(bgra, 0, (size_t)W * H * 4);

	/* y∈[0,240)：SMPTE 100% 彩条，8 色各 240px */
	static const uint8_t bars[][3] = {
		{ 255, 255, 255 },   /* 白 */ { 0, 255, 255 },   /* 黄 */
		{ 255, 255, 0   },   /* 青 */ { 0, 255, 0   },   /* 绿 */
		{ 255, 0,   255 },   /* 品红 */ { 0, 0, 255 },   /* 红 */
		{ 255, 0,   0   },   /* 蓝 */ { 0, 0, 0   },   /* 黑 */
	};
	for (int i = 0; i < 8; i++)
		fill_rect(bgra, i * 240, 0, (i + 1) * 240, 240,
		          bars[i][0], bars[i][1], bars[i][2]);

	/* y∈[240,480)：四块各 480px */
	{
		/* 纯色 5 竖条：红/绿/蓝/白/黑，各 96px */
		static const uint8_t solid[][3] = {
			{ 0, 0, 255 }, { 0, 255, 0 }, { 255, 0, 0 },
			{ 255, 255, 255 }, { 0, 0, 0 },
		};
		for (int i = 0; i < 5; i++)
			fill_rect(bgra, i * 96, 240, (i + 1) * 96, 480,
			          solid[i][0], solid[i][1], solid[i][2]);
	}
	{
		struct checker_arg a = { 480, 240, 8 };
		fill_cond(bgra, 480, 240, 960, 480, checker_on, &a);
	}
	{
		struct grating_arg a = { 960, 4 };
		fill_cond(bgra, 960, 240, 1440, 480, grating_on, &a);
	}
	{
		struct diag_arg a = { 1440, 240, 4 };
		fill_cond(bgra, 1440, 240, 1920, 480, diag_on, &a);
	}

	/* y∈[480,1080) 左半：白底黑字，验证超分文字边缘 */
	fill_rect(bgra, 0, 480, 960, 1080, 255, 255, 255);
	draw_text(bgra, 40, 520, 16, "1080p", 0, 0, 0);
	draw_text(bgra, 40, 700, 16, "BGRA TEST", 0, 0, 0);

	/* 右半：垂直渐变（黑→白随 y）+ 45° 粗斜线叠加 */
	for (int y = 480; y < H; y++) {
		int v = (y - 480) * 255 / (H - 480);   /* 垂直渐变 */
		for (int x = 960; x < W; x++) {
			size_t o = ((size_t)y * W + x) * 4;
			uint8_t val = (((x - 960) + (y - 480)) / 16 & 1) ? 0 : (uint8_t)v;
			bgra[o + 0] = val;
			bgra[o + 1] = val;
			bgra[o + 2] = val;
			bgra[o + 3] = 0xff;
		}
	}

	FILE *f = fopen(out_path, "wb");
	if (!f) {
		perror("fopen out");
		free(bgra);
		return 1;
	}
	size_t n = fwrite(bgra, 1, (size_t)W * H * 4, f);
	fclose(f);
	free(bgra);
	printf("wrote %s: %zu bytes, %dx%d bgra\n"
	       "  sections: bars/solid/checker/grating/diag/text/gradient\n",
	       out_path, n, W, H);
	return n == (size_t)W * H * 4 ? 0 : 1;
}

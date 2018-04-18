#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <asm/ioctl.h>
#include <linux/fb.h>
#include <malloc.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/mman.h>
#define LOGE(fmt, args...) printf("(%s)(%d):" fmt "\n", __func__, __LINE__, ##args)
#define FB0DEV  "/dev/fb0"

struct jzfb_dev {
	void *data_buf[4];
	void *buf_addr;

	int width;
	int height;

	unsigned int vid_size;
	unsigned int fb_size;
	unsigned int num_buf;
	int bpp;
	int format;

	struct fb_fix_screeninfo fix_info;
	struct fb_var_screeninfo var_info;

	int fd;
};
struct jzfb_dev *jzfb_dev;

#if 0
#define PIN_NUM		0
#define LCD_TEST (0xff << PIN_NUM)
static void jzfb_display_v_color_bar(struct jzfb_dev *jzfb_dev, void *buf)
{
	int i, j;
	int w, h;
	int bpp;
	unsigned short *p16;
	unsigned int *p32;

	p16 = (unsigned short *)buf;
	p32 = (unsigned int *)buf;
	w = jzfb_dev->width;
	h = jzfb_dev->height;
	bpp = jzfb_dev->bpp;


	for (i = 0; i < h; i++) {
		for (j = 0; j < w; j++) {
//			p32[i*w+j] = 0x7f7f7f;
			p32[i*w+j] = LCD_TEST;
		}
	}
}
static void jzfb_display_h_color_bar(struct jzfb_dev *jzfb_dev, void *buf)
{
	int i, j;
	int w, h;
	int bpp;
	unsigned short *p16;
	unsigned int *p32;

	p16 = (unsigned short *)buf;
	p32 = (unsigned int *)buf;
	w = jzfb_dev->width;
	h = jzfb_dev->height;
	bpp = jzfb_dev->bpp;


	for (i = 0; i < h; i++) {
		for (j = 0; j < w; j++) {
//			p32[i*w+j] = 0x7f7f7f;
			p32[i*w+j] = LCD_TEST;
		}
	}
}
#else
static void jzfb_display_v_color_bar(struct jzfb_dev *jzfb_dev, void *buf)
{
	int i, j;
	int w, h;
	int bpp;
	unsigned short *p16;
	unsigned int *p32;

	p16 = (unsigned short *)buf;
	p32 = (unsigned int *)buf;
	w = jzfb_dev->width;
	h = jzfb_dev->height;
	bpp = jzfb_dev->bpp;


	for (i = 0; i < h; i++) {
		for (j = 0; j < w; j++) {
			short c16;
			int c32 = 0;
			switch ((j / 10) % 4) {
			case 0:
				c16 = 0xF800;
				c32 = 0x00FF0000;
				break;
			case 1:
				c16 = 0x07C0;
				c32 = 0x0000FF00;
				break;
			case 2:
				c16 = 0x001F;
				c32 = 0x000000FF;
				break;
			default:
				c16 = 0xFFFF;
				c32 = 0xFFFFFFFF;
				break;
			}
			switch (bpp) {
			case 2:
				*p16++ = c16;
				break;
			default:
				*p32++ = c32;
				break;
			}
		}
	}
}
static void jzfb_display_h_color_bar(struct jzfb_dev *jzfb_dev, void *buf)
{
	int i, j;
	int w, h;
	int bpp;
	unsigned short *p16;
	unsigned int *p32;

	p16 = (unsigned short *)buf;
	p32 = (unsigned int *)buf;
	w = jzfb_dev->width;
	h = jzfb_dev->height;
	bpp = jzfb_dev->bpp;


	for (i = 0; i < h; i++) {
		for (j = 0; j < w; j++) {
			short c16;
			int c32;
			switch ((i / 10) % 4) {
			case 0:
				c16 = 0xF800;
				c32 = 0x00FF0000;
				break;
			case 1:
				c16 = 0x07C0;
				c32 = 0x0000FF00;
				break;
			case 2:
				c16 = 0x001F;
				c32 = 0x000000FF;
				break;
			default:
				c16 = 0xFFFF;
				c32 = 0xFFFFFFFF;
				break;
			}
			switch (bpp) {
			case 2:
				*p16++ = c16;
				break;
			default:
				*p32++ = c32;
				break;
			}
		}
	}
}
#endif

static int jzfb_dev_init() {
	int ret = 0;
	int i, j;

	jzfb_dev = (struct jzfb_dev *)malloc(sizeof(struct jzfb_dev));
	if (jzfb_dev == NULL) {
		LOGE("alloc mem for hwc dev failed!");
		return -ENOMEM;
	}
	memset(jzfb_dev, 0, sizeof(struct jzfb_dev));

	jzfb_dev->fd = open(FB0DEV, O_RDWR);
	if (jzfb_dev->fd < 0) {
		LOGE("%s %d: fb0 open error\n", __FUNCTION__, __LINE__);
		goto err_init;
	}

	/* get framebuffer's var_info */
	if (ioctl(jzfb_dev->fd, FBIOGET_VSCREENINFO, &jzfb_dev->var_info) < 0) {
		LOGE("%s %d: FBIOGET_VSCREENINFO failed",__FUNCTION__, __LINE__);
		goto err_getinfo;
	}

	/* get framebuffer's fix_info */
	if (ioctl(jzfb_dev->fd, FBIOGET_FSCREENINFO, &jzfb_dev->fix_info) < 0) {
		LOGE("%s %d: FBIOGET_FSCREENINFO failed", __FUNCTION__, __LINE__);
		goto err_getinfo;
	}

	jzfb_dev->var_info.width = jzfb_dev->var_info.xres;
	jzfb_dev->var_info.height = jzfb_dev->var_info.yres;
//	jzfb_dev->bpp = jzfb_dev->var_info.bits_per_pixel >> 3;
	jzfb_dev->bpp = 4;

	jzfb_dev->width = jzfb_dev->var_info.xres;
	jzfb_dev->height = jzfb_dev->var_info.yres;
	/*format 888 , use 4 word*/
//	jzfb_dev->fb_size = jzfb_dev->var_info.xres * jzfb_dev->var_info.yres * jzfb_dev->bpp;
	jzfb_dev->fb_size = jzfb_dev->var_info.xres * jzfb_dev->var_info.yres * 4;
	jzfb_dev->num_buf = jzfb_dev->var_info.yres_virtual / jzfb_dev->var_info.yres;
	jzfb_dev->vid_size = jzfb_dev->fb_size * jzfb_dev->num_buf;

	jzfb_dev->buf_addr = mmap(0, jzfb_dev->vid_size, PROT_READ|PROT_WRITE, MAP_SHARED, jzfb_dev->fd, 0);
	if(jzfb_dev->buf_addr == 0) {
		printf("%s %d: Map failed\n", __func__, __LINE__);
		goto err_init;
	}
	for(i = 0; i < jzfb_dev->num_buf; i++) {
		if(jzfb_dev->bpp == 2)
			jzfb_dev->data_buf[i] = (short *)jzfb_dev->buf_addr + i * jzfb_dev->width * jzfb_dev->height;
		else
			jzfb_dev->data_buf[i] = (int *)jzfb_dev->buf_addr + i * jzfb_dev->width * jzfb_dev->height;
	}

	printf("xres = %d, yres = %d line_length = %d fbsize = %d, num_buf = %d, vidSize = %d bpp = %d\n",
			jzfb_dev->var_info.xres, jzfb_dev->var_info.yres,
			jzfb_dev->fix_info.line_length, jzfb_dev->fb_size,
			jzfb_dev->num_buf, jzfb_dev->vid_size, jzfb_dev->bpp);
	return ret;

err_getinfo:
	close(jzfb_dev->fd);
err_init:
	free(jzfb_dev);

	return -EINVAL;
}

void clear(void){
	if(jzfb_dev != NULL)
		free(jzfb_dev);
}

#define JZFB_START_DMA  _IOW('F', 0X131, int)
int jzfb_pan_display(struct jzfb_dev *jzfb_dev, int frm_num)
{
	int ret;
	int startdma = 1;
	switch(frm_num) {
	case 0:
		jzfb_dev->var_info.yoffset = jzfb_dev->height * 0;
		break;
	case 1:
		jzfb_dev->var_info.yoffset = jzfb_dev->height * 1;
		break;
	case 2:
		jzfb_dev->var_info.yoffset = jzfb_dev->height * 2;
		break;
	default:
		LOGE("pan display frm num err!\n");
	}
//	ret = ioctl(jzfb_dev->fd, JZFB_START_DMA, &startdma);
//	if(ret) {
//		printf("lcd start dma failed!\n");
//	}
	ret = ioctl(jzfb_dev->fd, FBIOPAN_DISPLAY, &jzfb_dev->var_info);
	return ret;
}

int test_disp_color_bar(struct jzfb_dev *jzfb_dev)
{
	int ret;
	int i;
	int cnt = 0;

	while(1) {
		for(i = 0; i < jzfb_dev->num_buf; i++) {
			if(cnt % 2 == 0)
				jzfb_display_v_color_bar(jzfb_dev, jzfb_dev->data_buf[i]);
			else
				jzfb_display_h_color_bar(jzfb_dev, jzfb_dev->data_buf[i]);

			ret = jzfb_pan_display(jzfb_dev, i);
			printf("display buff i = %d cnt = %d\n", i, cnt);
			if(ret) {
				LOGE("pan display error!\n");
				return ret;
			}
			sleep(5);
			cnt++;
		}
		if(cnt == 10000000) {
			break;
		}
	}

	return 0;
}

int main()
{
	int ret = -1;

	ret = jzfb_dev_init();
	if(ret) {
		LOGE("Init Error!\n");
		return ret;
	}

	ret = test_disp_color_bar(jzfb_dev);
	if(ret) {
		return ret;
	}

	clear();
	close(jzfb_dev->fd);
	return 0;
}

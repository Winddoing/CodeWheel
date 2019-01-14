#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>
#include <png.h>

const char *png_file = "test.png";

#define V_WIDE 640
#define V_HIGH 480

/*#define BGRA*/

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Texture *texture = NULL;

void rgba_resize(int src_width, int src_height, int dst_width, int dst_height, uint8_t *src_buf,
                 uint8_t *dst_buf)
{
    int hnum = 0, wnum = 0;
    int r_width = src_width * 4;
    int w_width = dst_width * 4;

    for(hnum = 0; hnum < dst_height; hnum++) {
        for(wnum = 0; wnum < dst_width; wnum++) {
            double d_original_img_hnum = hnum * src_height / (double) dst_height;
            double d_original_img_wnum = wnum * src_width / (double) dst_width;
            int i_original_img_hnum = (int) d_original_img_hnum;
            int i_original_img_wnum = (int) d_original_img_wnum;
            double distance_to_a_x = d_original_img_wnum - i_original_img_wnum;//在原图像中与a点的水平距离
            double distance_to_a_y = d_original_img_hnum - i_original_img_hnum;//在原图像中与a点的垂直距离

            int original_point_a =
                i_original_img_hnum * r_width + i_original_img_wnum * 4;//数组位置偏移量，对应于图像的各像素点RGB的起点,相当于点A
            int original_point_b =
                i_original_img_hnum * r_width + (i_original_img_wnum + 1) * 4;//数组位置偏移量，对应于图像的各像素点RGB的起点,相当于点B
            int original_point_c =
                (i_original_img_hnum + 1) * r_width + i_original_img_wnum * 4;//数组位置偏移量，对应于图像的各像素点RGB的起点,相当于点C
            int original_point_d =
                (i_original_img_hnum + 1) * r_width + (i_original_img_wnum + 1) * 4;//数组位置偏移量，对应于图像的各像素点RGB的起点,相当于点D

            if(i_original_img_hnum + 1 == dst_width - 1) {
                original_point_c = original_point_a;
                original_point_d = original_point_b;
            }

            if(i_original_img_wnum + 1 == dst_width - 1) {
                original_point_b = original_point_a;
                original_point_d = original_point_c;
            }

            int pixel_point = hnum * w_width + wnum * 4;//映射尺度变换图像数组位置偏移量
            dst_buf[pixel_point] =
                (uint8_t)(src_buf[original_point_a] * (1 - distance_to_a_x) * (1 - distance_to_a_y) +
                        src_buf[original_point_b] * distance_to_a_x * (1 - distance_to_a_y) +
                        src_buf[original_point_c] * distance_to_a_y * (1 - distance_to_a_x) +
                        src_buf[original_point_d] * distance_to_a_y * distance_to_a_x);
            dst_buf[pixel_point + 1] =
                (uint8_t)(src_buf[original_point_a + 1] * (1 - distance_to_a_x) * (1 - distance_to_a_y) +
                        src_buf[original_point_b + 1] * distance_to_a_x * (1 - distance_to_a_y) +
                        src_buf[original_point_c + 1] * distance_to_a_y * (1 - distance_to_a_x) +
                        src_buf[original_point_d + 1] * distance_to_a_y * distance_to_a_x);
            dst_buf[pixel_point + 2] =
                (uint8_t)(src_buf[original_point_a + 2] * (1 - distance_to_a_x) * (1 - distance_to_a_y) +
                        src_buf[original_point_b + 2] * distance_to_a_x * (1 - distance_to_a_y) +
                        src_buf[original_point_c + 2] * distance_to_a_y * (1 - distance_to_a_x) +
                        src_buf[original_point_d + 2] * distance_to_a_y * distance_to_a_x);

        }
    }
}

int vos_display()
{
    FILE *file = fopen(png_file, "rb");

    if(!file) {
        printf("%s, no test file.\n", __func__);
    }

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);

    if(!png_ptr) {
        printf("%s, png_create_read_struct error! version: %s\n", __func__, PNG_LIBPNG_VER_STRING);
        return -1;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);

    if(!info_ptr) {
        printf("%s, png_create_info_struct error.\n", __func__);
    }

    setjmp(png_jmpbuf(png_ptr));
    png_init_io(png_ptr, file);
    png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_EXPAND, 0);

    int m_width = png_get_image_width(png_ptr, info_ptr);
    int m_height = png_get_image_height(png_ptr, info_ptr);

    int channels   = png_get_channels(png_ptr, info_ptr);
    int bit_depth  = png_get_bit_depth(png_ptr, info_ptr);
    int color_type = png_get_color_type(png_ptr, info_ptr);
    printf("===> func: %s, line: %d, png m_width=%d, m_height=%d, channels=%d, bit_depth=%d, color_type=%d\n",
           __func__, __LINE__, m_width, m_height, channels, bit_depth, color_type);

    int src_size = m_height * m_width * 4;
    unsigned char *rgba = (unsigned char *) calloc(1, src_size);
    int pos = 0;

    // row_pointers里边就是传说中的rgba数据了
    png_bytep *row_pointers = png_get_rows(png_ptr, info_ptr);

    for(int i = 0; i < m_height; i++) {
        for(int j = 0; j < (4 * m_width); j += 4) {
            rgba[pos++] = row_pointers[i][j + 2];   // blue
            rgba[pos++] = row_pointers[i][j + 1];   // green
            rgba[pos++] = row_pointers[i][j];       // red
            rgba[pos++] = row_pointers[i][j + 3];   // alpha

        }
    }

    printf("png 2 rgba (%d*%d) finish. \n", m_width, m_height);
    //rgb resize
    int dst_width = V_WIDE;
    int dst_height = V_HIGH;
    int rgb_resize = src_size;
    unsigned char *rgb_resize_buf = rgba;
    int need_resize = !(dst_height == m_height && dst_width == m_width);

    if(need_resize) {
        rgb_resize = dst_width * dst_height * 4;
        rgb_resize_buf = (unsigned char *) calloc(1, rgb_resize);
        rgba_resize(m_width, m_height, dst_width, dst_height, rgba, rgb_resize_buf);
        printf("rgba resize (%d * %d) finish. \n", dst_width, dst_height);
    }

    //rgb 2 yuv
    int yuv_size = dst_width * dst_height * 3;
    unsigned char *yuv_buf = (unsigned char *) calloc(1, yuv_size);
    unsigned char *y_ptr = yuv_buf, *u_ptr = yuv_buf + dst_height * dst_width, *v_ptr =
                               u_ptr + dst_height * dst_width;
    unsigned char *wy_ptr = y_ptr, *wu_ptr = u_ptr, *wv_ptr = v_ptr;

    for(int i = 0; i < rgb_resize; i += 4) {
        *wy_ptr = (unsigned char)((77 * rgb_resize_buf[i + 2] / 256) + (150 * rgb_resize_buf[i + 1] / 256) +
                                  (29 * rgb_resize_buf[i] / 256));

        *wu_ptr = 128;
        *wv_ptr = 128;
        wy_ptr++;
        wu_ptr++;
        wv_ptr++;
    }

#if 1
    FILE *fw = fopen("png.yuv", "wb");
    fwrite(yuv_buf, 1, yuv_size, fw);
    fclose(fw);
    printf("rgba 2 yuv finish.\n");
#endif

#ifdef BGRA
    SDL_UpdateTexture(texture, NULL, rgb_resize_buf, V_WIDE * 4);
#else
    SDL_UpdateTexture(texture, NULL, yuv_buf, V_WIDE);
#endif
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);

    printf("yuv display finish.\n");
    free(rgba);

    if(need_resize)
        free(rgb_resize_buf);

    free(yuv_buf);
    fclose(file);

    return 0;
}

int main()
{
    int quit = 0;
    SDL_Event event;
    // SDL_Window *window = NULL;
    // SDL_Renderer *renderer = NULL;
    // SDL_Texture *texture = NULL;

    if(SDL_Init(SDL_INIT_EVERYTHING) == -1) {
        printf("===> func: %s, line: %d\n", __func__, __LINE__);
        return -1;
    }

    window = SDL_CreateWindow("SDL2 Test", 100, 100, V_WIDE, V_HIGH, SDL_WINDOW_SHOWN);

    if(window == NULL) {
        printf("===> func: %s, line: %d\n", __func__, __LINE__);
        return -1;
    }

    //renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    renderer = SDL_CreateRenderer(window, -1, 0);

    if(renderer == NULL) {
        printf("===> func: %s, line: %d\n", __func__, __LINE__);
        return -1;
    }

#ifdef BGRA
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_BGRA8888, SDL_TEXTUREACCESS_STREAMING, V_WIDE, V_HIGH);
#else
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_IYUV, SDL_TEXTUREACCESS_STREAMING, V_WIDE, V_HIGH);
#endif

    if(texture == NULL) {
        printf("SDL_CreateTexture faill\n");
        return -1;
    }

    vos_display();

    while(!quit) {
        SDL_WaitEvent(&event);
        switch (event.type) {
        case SDL_QUIT:
            quit = 1;
            break;
        }
    }
    //SDL_Delay(2000);
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}


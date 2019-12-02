// paly BGRA8888 bitmap data

#include <stdio.h>
#include <SDL2/SDL.h>

//Refresh Event
#define REFRESH_EVENT  (SDL_USEREVENT + 1)
//Break
#define BREAK_EVENT  (SDL_USEREVENT + 2)

struct sdl_player {
    uint32_t screen_w;
    uint32_t screen_h;
    SDL_Window *screen;
    SDL_Renderer *sdlRenderer;
    SDL_Texture *sdlTexture;

    uint8_t *buffer;
    uint32_t pixel_w;
    uint32_t pixel_h;
    uint32_t stride;
    uint32_t pixformat;

    SDL_Thread *refresh_thread;
    uint32_t thread_quit;

	FILE *fp;
};

static int refresh_video(void *data)
{
	SDL_Event event;
    struct sdl_player *player = data;

	while(!player->thread_quit) {
		event.type = REFRESH_EVENT;
		SDL_PushEvent(&event);
		SDL_Delay(40);
	}

	//Break
	event.type = BREAK_EVENT;
	SDL_PushEvent(&event);

	return 0;
}

static int create_render_window(struct sdl_player *player)
{
	if(SDL_Init(SDL_INIT_VIDEO)) {
		printf("Could not initialize SDL - %s\n", SDL_GetError());
		return -1;
	}

	player->screen = SDL_CreateWindow("SDL2 Play BGRA8888",
							  SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
							  player->screen_w, player->screen_h,
							  SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	if(!player->screen) {
		printf("SDL: could not create window - exiting:%s\n",
			   SDL_GetError());
		return -1;
	}

	player->sdlRenderer = SDL_CreateRenderer(player->screen, -1, 0);
	player->sdlTexture = SDL_CreateTexture(player->sdlRenderer, player->pixformat,
						  SDL_TEXTUREACCESS_STREAMING, player->pixel_w, player->pixel_h);

    return 0;
}

#define _ALIGH(x, align) (((x) + (align - 1)) & (~(align - 1)))

int main(int argc, char *argv[])
{
    struct sdl_player player;
    uint32_t pixel_data_len = 0;
    uint32_t bpp = 0; //Bit per Pixel
    int ret = 0;

    player.screen_w = 500;
    player.screen_h = 500;

    player.pixel_w = 320;
    player.pixel_h = 180;
	//Note: ARGB8888 in "Little Endian" system stores as B|G|R|A
	player.pixformat = SDL_PIXELFORMAT_ARGB8888;

    if (player.pixformat == SDL_PIXELFORMAT_ARGB8888)
        bpp = 32;

    pixel_data_len = player.pixel_w * player.pixel_h * bpp / 8;

    //内存中像素的宽度，并进行内存对齐处理，一般4字节对齐
    player.stride = _ALIGH(player.pixel_w * bpp / 8, 4);

    ret = create_render_window(&player);
    if (ret) {
        printf("create_render_window fail...\n");
        return -1;
    }

	player.buffer = (uint8_t *)malloc(pixel_data_len * sizeof(uint8_t));
	if(!player.buffer) {
		printf("malloc faild...\n");
		return -1;
	}

	player.fp = fopen("test_bgra_320x180.rgb", "rb+");
	if(player.fp == NULL) {
		printf("cannot open this file\n");
		return -1;
	}

	player.refresh_thread = SDL_CreateThread(refresh_video, NULL, &player);
    player.thread_quit = 0;

	while(1) {
	    SDL_Event event;
		SDL_Rect sdlRect;

		SDL_WaitEvent(&event);

		if(event.type == REFRESH_EVENT) {
			int len = fread(player.buffer, 1, pixel_data_len, player.fp);

			if(len != pixel_data_len) {
				fseek(player.fp, 0, SEEK_SET);
				len = fread(player.buffer, 1, pixel_data_len, player.fp);
			}

			//We don't need to change Endian
			//Because input BGRA pixel data(B|G|R|A)
			//is same as ARGB8888 in Little Endian (B|G|R|A)
			SDL_UpdateTexture(player.sdlTexture, NULL, player.buffer, player.stride);

			//FIX: If window is resize
			sdlRect.x = 0;
			sdlRect.y = 0;
			sdlRect.w = player.screen_w;
			sdlRect.h = player.screen_h;

			SDL_RenderClear(player.sdlRenderer);
			SDL_RenderCopy(player.sdlRenderer, player.sdlTexture, NULL, &sdlRect);
			SDL_RenderPresent(player.sdlRenderer);

		} else if(event.type == SDL_WINDOWEVENT) {
			//If Resize
			SDL_GetWindowSize(player.screen, &player.screen_w, &player.screen_h);

		} else if(event.type == SDL_QUIT) {
			player.thread_quit = 1;

		} else if(event.type == BREAK_EVENT) {
			break;
		}
	}

	SDL_Quit();
	free(player.buffer);

	return 0;
}

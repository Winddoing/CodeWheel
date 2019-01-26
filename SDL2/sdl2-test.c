#include <stdio.h>
#include <SDL2/SDL.h>

static const char *arrow[] = {
    /* width height num_colors chars_per_pixel */
    "    32    32        3            1",
    /* colors */
    "X c #000000",
    ". c #ffffff",
    "  c None",
    /* pixels */
    "X                                ",
    "X.X                              ",
    "X..X                             ",
    "X...X                            ",
    "X....X                           ",
    "X.....X                          ",
    "X......X                         ",
    "X.......X                        ",
    "X........X                       ",
    "X.........X                      ",
    "X......XXXXX                     ",
    "X...X..X                         ",
    "X..X X..X                        ",
    "X.X  X..X                        ",
    "X     X..X                       ",
    "      X..X                       ",
    "       XX                        ",
    "                                 ",
    "                                 ",
    "                                 ",
    "                                 ",
    "                                 ",
    "                                 ",
    "                                 ",
    "                                 ",
    "                                 ",
    "                                 ",
    "                                 ",
    "                                 ",
    "                                 ",
    "                                 ",
    "                                 ",
    "0,0"
};

static SDL_Cursor *init_system_cursor(const char *image[])
{
    int i, row, col;
    Uint8 data[4*32];
    Uint8 mask[4*32];
    int hot_x, hot_y;

    i = -1;
    for (row=0; row<32; ++row) {
        for (col=0; col<32; ++col) {
            if (col % 8) {
                data[i] <<= 1;
                mask[i] <<= 1;
            } else {
                ++i;
                data[i] = mask[i] = 0;
            }
            switch (image[4+row][col]) {
            case 'X':
                data[i] |= 0x01;
                mask[i] |= 0x01;
                break;
            case '.':
                mask[i] |= 0x01;
                break;
            case ' ':
                break;
            }
        }
    }
    sscanf(image[4+row], "%d,%d", &hot_x, &hot_y);
    return SDL_CreateCursor(data, mask, 32, 32, hot_x, hot_y);
}

static int set_cursor(const char *image[], int toggle)
{

    SDL_Cursor* cursor = init_system_cursor(image);
    if (!cursor) {
        SDL_Log("Unable init_system_cursor error: %s\n", SDL_GetError());
        return -1;
    }

    SDL_SetCursor(cursor);

    SDL_ShowCursor(toggle);

    return 0;
}


int main(int argc, char ** argv)
{
    int quit = 0;
    int ret = 0;
    SDL_Event event;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return -1;
    }

#if 1
    SDL_Window * window = SDL_CreateWindow("SDL2 Displaying Image",
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);
#else
    SDL_Window * window = SDL_CreateWindow("SDL2 Displaying Image",
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_FULLSCREEN_DESKTOP);
#endif
    if (!window) {
        SDL_Log("SDL_CreateWindow error: %s", SDL_GetError());
        return -1;
    }
    ret = set_cursor(arrow, SDL_ENABLE);
    if (ret) {
        SDL_Log("set_cursor error...\n");
        return -1;
    }

    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer) {
        SDL_Log("SDL_CreateRenderer error: %s\n", SDL_GetError());
        return -1;
    }

    SDL_Surface * image = SDL_LoadBMP("test.bmp");
    if (!image) {
        SDL_Log("SDL_LoadBMP error: %s\n", SDL_GetError());
        return -1;
    }

    SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, image);
    if (!texture) {
        SDL_Log("SDL_CreateTextureFromSurface error: %s\n", SDL_GetError());
        return -1;
    }

    SDL_Log("SDL show BMP...\n");

    while (!quit) {
        SDL_WaitEvent(&event);

        SDL_Log("event type: %x\n", event.type);
        switch (event.type) {
        case SDL_QUIT:
            SDL_Log("Event: SDL_QUIT");
            quit = 1;
            break;
        case SDL_KEYDOWN:
        case SDL_KEYUP:
            SDL_Log("Event: SDL_KEYDOWN || SDL_KEYUP");
            break;
        case SDL_MOUSEMOTION:       //鼠标移动坐标
            SDL_Log("Event: SDL_MOUSEMOTION motion [x=%d,y=%d]\n", event.motion.x, event.motion.y);
            break;
        case SDL_MOUSEBUTTONDOWN:   //鼠标按键
        case SDL_MOUSEBUTTONUP:
            SDL_Log("Event: SDL_MOUSEBUTTONDOWN || SDL_MOUSEBUTTONUP");
            switch(event.button.button){
            case 1:
                SDL_Log("\tMouse BTN_LEFT\n");
                break;
            case 2:
                SDL_Log("\tMouse BTN_MIDDLE\n");
                break;
            case 3:
                SDL_Log("\tMouse BTN_RIGHT\n");
                break;
            }
            break;
        case SDL_MOUSEWHEEL:        //鼠标滚轮
            SDL_Log("Event: SDL_MOUSEWHEEL wheel [y=%d]\n", event.wheel.y);
            break;
        }

        //如果指定显示位置使用下面注释起来的两句
        //SDL_Rect dstrect = { 5, 5, 320, 240 };
        //SDL_RenderCopy(renderer, texture, NULL, &dstrect);

        //把贴图材质复制到渲染器
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        //显示出来
        SDL_RenderPresent(renderer);
    }
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(image);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

#include <stdio.h>
#include <SDL2/SDL.h>

int main(int argc, char ** argv)
{
    int quit = 0;
    SDL_Event event;

    SDL_Init(SDL_INIT_VIDEO);

#if 1
    SDL_Window * window = SDL_CreateWindow("SDL2 Displaying Image",
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);
#else
    SDL_Window * window = SDL_CreateWindow("SDL2 Displaying Image",
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_FULLSCREEN_DESKTOP);
#endif
    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_Surface * image = SDL_LoadBMP("test.bmp");
    SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, image);

    while (!quit) {
        SDL_WaitEvent(&event);

        switch (event.type) {
        case SDL_QUIT:
            quit = 1;
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

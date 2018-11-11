#ifndef SDL_HELPER_H
#define SDL_HELPER_H

#include <iostream>
#include <SDL2/SDL.h>

class SDL_helper {
public:
    SDL_helper();
    ~SDL_helper();

    void create_window();

    void run_loop();

    void fractalDraw(Uint8 red, Uint8 green, Uint8 blue);

private:
    SDL_Window *window;
    SDL_Renderer *ren;
    SDL_Texture *tex;
    Uint32 *m_buffer1;
    int t_x;
    int t_y;

    void setPixel(int x, int y, Uint8 red, Uint8 green, Uint8 blue);
};

#endif // SDL_HELPER_H

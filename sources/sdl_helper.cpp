#include <functional>
#include "sdl_helper.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

#define FIRST_PIXEL_X SCREEN_WIDTH / 1.5

static void check_SDL_code(bool result, const std::string &message = "", std::function<void(void)> cleanup = []{}) {
    if (result) {
        std::cerr << message << ": " << SDL_GetError() << std::endl;
        cleanup();
        SDL_Quit();
        std::abort();
    }
}

SDL_helper::SDL_helper() :
    window{nullptr},
    ren{nullptr},
    tex{nullptr},
    m_buffer1{nullptr},
    t_x{0},
    t_y{0}
{
    int resultCode = SDL_Init(SDL_INIT_EVERYTHING);
    check_SDL_code(resultCode != 0, "SDL_INIT_EVERYTHING error:");

    m_buffer1 = new Uint32[SCREEN_WIDTH * SCREEN_HEIGHT];
    memset(this->m_buffer1, 0, SCREEN_HEIGHT * SCREEN_WIDTH * sizeof(Uint32));
}

void SDL_helper::setPixel(int x, int y, Uint8 red, Uint8 green, Uint8 blue) {
    if (x < 0 || x > SCREEN_WIDTH || y < 0 || y > SCREEN_HEIGHT) {
        return;
    }

    Uint32 color = 0;
    color += red;
    color <<= 8;
    color += green;
    color <<= 8;
    color += blue;
    color <<= 8;
    color += 0xff;
    m_buffer1[(y * SCREEN_WIDTH) + x] = color;
}

void SDL_helper::fractalDraw(Uint8 red, Uint8 green, Uint8 blue) {
    for (int i = 0; i < 100; i++) {
        int choice = (4 * (double) rand()) / RAND_MAX;

        switch(choice) {
        case 0:
            t_x += ((-t_x + FIRST_PIXEL_X))/2;
            t_y += ((-t_y)/2);
            setPixel(t_x, t_y, red, green, blue);
            break;
        case 1:
            t_x -= (t_x) / 2.0;
            t_y -= (t_y - SCREEN_HEIGHT) / 2.0;
            setPixel(t_x, t_y, red, green, blue);
            break;
        case 2:
            t_x -= (t_x-SCREEN_WIDTH) / 2.0;
            t_y -= (t_y-SCREEN_HEIGHT)/2.0;
            setPixel(t_x, t_y, red, green, blue);
            break;
        case 3:
            t_x -= (t_x - 100) / 2.0;
            t_y -= (t_y - 50)/2.0;
            setPixel(t_x, t_y, red, green, blue);
            break;
        default: break;
        }
    }
}

SDL_helper::~SDL_helper() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(ren);
    SDL_Quit();
}

void SDL_helper::create_window() {
    window = SDL_CreateWindow("Fractal",
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              SCREEN_WIDTH,
                              SCREEN_HEIGHT,
                              SDL_WINDOW_SHOWN);
    check_SDL_code(window == nullptr, "Could not create window: ");

    ren = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    check_SDL_code(ren == nullptr, "SDL_CreateRenderer Error: ", [&]{ SDL_DestroyWindow(window); } );

    tex = SDL_CreateTexture(ren, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, SCREEN_WIDTH, SCREEN_HEIGHT);
    check_SDL_code(tex == nullptr, "SDL_CreateTexture Error: ", [&]{
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(ren);
    } );
}

void SDL_helper::run_loop() {
    bool stop_draw = false;
    int angle = 0;

    const auto updateUI = [&] {
        if (!stop_draw) {
        Uint32 elapsed = SDL_GetTicks();
        unsigned char green = ((1 + sin(elapsed * 0.0001)) * 128);
        unsigned char red = (unsigned char) ((1 + sin(elapsed * 0.0002)) * 128);
        unsigned char blue = (unsigned char) ((1 + sin(elapsed * 0.0003)) * 128);

        fractalDraw(red, green, blue);

        SDL_UpdateTexture(tex, NULL, m_buffer1, SCREEN_WIDTH * sizeof(Uint32));
        }

        SDL_RenderClear(ren);
        SDL_RenderCopyEx(ren, tex, NULL, NULL, angle, NULL, SDL_FLIP_NONE);
        SDL_RenderPresent(ren);
    };

    SDL_Event e;
    bool quit = false;
    while (!quit){

        updateUI();
        angle++;
        if(angle == 200) stop_draw = true;
        if(angle == 360) angle = 0;

        while (SDL_PollEvent(&e)){
            if (e.type == SDL_QUIT){
                quit = true;
            }
            if (e.type == SDL_KEYDOWN){
                quit = true;
            }
        }
    }
}

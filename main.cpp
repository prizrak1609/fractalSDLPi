#include <iostream>
#include "sdl_helper.h"

int main() {

    SDL_helper helper;

    helper.create_window();
    helper.run_loop();

    return 0;
}

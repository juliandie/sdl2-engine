#pragma once
// https://www.youtube.com/watch?v=44tO977slsU&list=PLhfAbcv9cehhkG7ZQK0nfIGJC_C-wSLrx&index=3

// https://stackoverflow.com/questions/53376811/linker-error-sdl-main-unresolved-in-main-getcmdline
#define SDL_MAIN_HANDLED

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"

#include <stdio.h>
#include <string.h>
#include <sstream>
#include <stdint.h>

#include <iostream>

//#include <Windows.h>
//#include <profileapi.h>

#include "Structs.h"

# define LEFT_MOUSE_BUTTON_ID 1
# define MOUSEWHEEL_ID 2
# define RIGHT_MOUSE_BUTTON_ID 3

# define WINDOW_HEIGTH 600
# define WINDOW_WIDTH 600

class Game {

private:
    bool isRunning;

    SDL_Window *window;
    SDL_Renderer *renderer;

    Field field = {};
    SDL_Rect clicked = {};
    SDL_Rect camera = {};


    void renderDebug();
    void renderString(std::string message, int x, int y, int size, TTF_Font *font, SDL_Color color);
    int getDebugY(int size);

public:
    Game();
    ~Game();

    void init(const char *title, int x, int y, int width, int height, int flags);
    void handleEvents();
    void update(int tick);
    void render(int tick);
    void clean();

    bool running();

    int frames = 0;
    int ticks = 0;
    float zoom = 1.0f;

    int scroll = 0;
    int scrollx = 0;
    int scrolly = 0;

    static int64_t GetTicks() {
        return SDL_GetTicks();

        //return SDL_GetPerformanceFrequency() * 100.0f;
    }


};


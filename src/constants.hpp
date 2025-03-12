#pragma once

#include <SDL_pixels.h>

#define DEFAULT_WIDTH 1280
#define DEFAULT_HEIGHT 720
#define DEFAULT_FPS 60
#define WINDOW_NAME "2048!"

SDL_Color WHITE_RGB = {255, 255, 255, 0};
enum class State
{
    INTRO,
    MAIN_MENU,
    GAME,
    GAME_STARTING,
    GAME_OVER,
    PAUSE,
    SETTINGS,
    SHOW_MESSAGEBOX,
    NOTHING,
};

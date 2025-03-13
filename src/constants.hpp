#pragma once

#include <SDL_pixels.h>

#define DEFAULT_WIDTH 1280
#define DEFAULT_HEIGHT 720
#define DEFAULT_FPS 60
#define INTRO_DURATION 1000
#define WINDOW_NAME "2048!"

SDL_Color WHITE_RGB = {255, 255, 255, 0};
SDL_Color TILE_TEXT_DARK_RGB = {117, 100, 82, 0};
SDL_Color TILE_TEXT_LIGHT_RGB = {249, 246, 242, 0};
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

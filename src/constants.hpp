#pragma once

#include <SDL_pixels.h>

#define DEFAULT_WIDTH 1280
#define DEFAULT_HEIGHT 720
#define DEFAULT_FPS 60
#define WINDOW_NAME "2048"

SDL_Color WHITE_RGB = {255, 255, 255, 0};
TTF_Font *DEBUG_FONT = nullptr;
TTF_Font *UI_FONT = nullptr;
enum class State
{
    MAIN_MENU,
    GAME,
    GAME_OVER,
    PAUSE
};

void init_statics()
{
    DEBUG_FONT = TTF_OpenFont("assets/fonts/jetbrains-mono.ttf", 12);
    UI_FONT = TTF_OpenFont("assets/fonts/jetbrains-mono.ttf", 12);
}
#pragma once

#include <SDL_ttf.h>
#include <SDL_image.h>

TTF_Font *DEBUG_FONT, *UI_FONT_32;
// Button
SDL_Texture *BTN_LEFT_TEXTURE, *BTN_RIGHT_TEXTURE, *BTN_MID_TEXTURE, *BTN_LEFT_HOVER_TEXTURE, *BTN_RIGHT_HOVER_TEXTURE, *BTN_MID_HOVER_TEXTURE;
// Game
SDL_Texture *BOARD_TEXTURE;
int load_assets(SDL_Renderer *renderer)
{
    DEBUG_FONT = TTF_OpenFont("assets/fonts/Jetbrains-Mono.ttf", 12);
    UI_FONT_32 = TTF_OpenFont("assets/fonts/Rubik-Regular.ttf", 32);
    BTN_MID_TEXTURE = IMG_LoadTexture(renderer, "assets/img/button/middle.png");
    BTN_LEFT_TEXTURE = IMG_LoadTexture(renderer, "assets/img/button/left.png");
    BTN_RIGHT_TEXTURE = IMG_LoadTexture(renderer, "assets/img/button/right.png");
    BTN_MID_HOVER_TEXTURE = IMG_LoadTexture(renderer, "assets/img/button/middle_hover.png");
    BTN_LEFT_HOVER_TEXTURE = IMG_LoadTexture(renderer, "assets/img/button/left_hover.png");
    BTN_RIGHT_HOVER_TEXTURE = IMG_LoadTexture(renderer, "assets/img/button/right_hover.png");
    BOARD_TEXTURE = IMG_LoadTexture(renderer, "assets/img/board.png");
    // Verify assets
    if (DEBUG_FONT == nullptr || UI_FONT_32 == nullptr || BTN_LEFT_TEXTURE == nullptr || BTN_RIGHT_TEXTURE == nullptr || BTN_MID_TEXTURE == nullptr || BTN_LEFT_HOVER_TEXTURE == nullptr || BTN_RIGHT_HOVER_TEXTURE == nullptr || BTN_MID_HOVER_TEXTURE == nullptr || BOARD_TEXTURE == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't load assets: %s", SDL_GetError());
        return 0;
    }
    return 1;
}
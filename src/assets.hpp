#pragma once

#include <SDL_ttf.h>
#include <SDL_image.h>

TTF_Font *DEBUG_FONT, *UI_FONT_32;
SDL_Texture *BTN_LEFT_TEXTURE, *BTN_RIGHT_TEXTURE, *BTN_MID_TEXTURE, *BTN_LEFT_HOVER_TEXTURE, *BTN_RIGHT_HOVER_TEXTURE, *BTN_MID_HOVER_TEXTURE;

void load_assets(SDL_Renderer *renderer)
{
    DEBUG_FONT = TTF_OpenFont("assets/fonts/Jetbrains-Mono.ttf", 12);
    UI_FONT_32 = TTF_OpenFont("assets/fonts/Rubik-Regular.ttf", 32);
    BTN_MID_TEXTURE = IMG_LoadTexture(renderer, "assets/img/button/middle.png");
    BTN_LEFT_TEXTURE = IMG_LoadTexture(renderer, "assets/img/button/left.png");
    BTN_RIGHT_TEXTURE = IMG_LoadTexture(renderer, "assets/img/button/right.png");
    BTN_MID_HOVER_TEXTURE = IMG_LoadTexture(renderer, "assets/img/button/middle_hover.png");
    BTN_LEFT_HOVER_TEXTURE = IMG_LoadTexture(renderer, "assets/img/button/left_hover.png");
    BTN_RIGHT_HOVER_TEXTURE = IMG_LoadTexture(renderer, "assets/img/button/right_hover.png");
}
#pragma once

#include <unordered_map>
#include <vector>
#include "constants.hpp"
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

// Audio
Mix_Music *BGM;
Mix_Chunk *SWIPE_SFX;
TTF_Font *DEBUG_FONT, *UI_FONT_BOLD_32, *UI_FONT_YURUKA_40, *GAME_FONT_48, *GAME_FONT_40, *UI_FONT_24;
// Button
SDL_Texture *BTN_LEFT_TEXTURE, *BTN_RIGHT_TEXTURE, *BTN_MID_TEXTURE, *BTN_LEFT_HOVER_TEXTURE, *BTN_RIGHT_HOVER_TEXTURE, *BTN_MID_HOVER_TEXTURE;
// Game
std::vector<SDL_Texture *> TILE_TEXTURES;
SDL_Texture *TILE_BIG_TEXTURE;
std::vector<SDL_Texture *> TILE_TEXT_TEXTURES;
std::vector<SDL_Surface *> TILE_TEXT_SURFACES;
std::unordered_map<int, int> TILE_VALUE_INDEX_MAP = {
    {2, 0},
    {4, 1},
    {8, 2},
    {16, 3},
    {32, 4},
    {64, 5},
    {128, 6},
    {256, 7},
    {512, 8},
    {1024, 9},
    {2048, 10},
};
SDL_Texture *BOARD_TEXTURE;
int load_assets(SDL_Renderer *renderer)
{
    BGM = Mix_LoadMUS("assets/audio/bg.ogg");
    SWIPE_SFX = Mix_LoadWAV("assets/audio/swipe.wav");
    DEBUG_FONT = TTF_OpenFont("assets/fonts/jetbrains-mono.ttf", 12);
    UI_FONT_24 = TTF_OpenFont("assets/fonts/Rubik-Light.ttf", 24);
    UI_FONT_BOLD_32 = TTF_OpenFont("assets/fonts/Rubik-Regular.ttf", 32);
    UI_FONT_YURUKA_40 = TTF_OpenFont("assets/fonts/FOT-Yuruka-STD.ttf", 40);
    GAME_FONT_48 = TTF_OpenFont("assets/fonts/Rubik-Medium.ttf", 48);
    GAME_FONT_40 = TTF_OpenFont("assets/fonts/Rubik-Medium.ttf", 40);
    BTN_MID_TEXTURE = IMG_LoadTexture(renderer, "assets/img/button/middle.png");
    BTN_LEFT_TEXTURE = IMG_LoadTexture(renderer, "assets/img/button/left.png");
    BTN_RIGHT_TEXTURE = IMG_LoadTexture(renderer, "assets/img/button/right.png");
    BTN_MID_HOVER_TEXTURE = IMG_LoadTexture(renderer, "assets/img/button/middle_hover.png");
    BTN_LEFT_HOVER_TEXTURE = IMG_LoadTexture(renderer, "assets/img/button/left_hover.png");
    BTN_RIGHT_HOVER_TEXTURE = IMG_LoadTexture(renderer, "assets/img/button/right_hover.png");
    // Game part
    BOARD_TEXTURE = IMG_LoadTexture(renderer, "assets/img/board.png");
    for (int i = 2; i <= 2048; i *= 2)
    {
        std::string path = "assets/img/tile_" + std::to_string(i) + ".png";
        TILE_TEXTURES.push_back(IMG_LoadTexture(renderer, path.c_str()));
        auto target_color = TILE_TEXT_DARK_RGB;
        if (i > 4)
        {
            target_color = TILE_TEXT_LIGHT_RGB;
        }
        auto text_str = std::to_string(i);
        auto text = text_str.c_str();
        SDL_Surface *text_surface;
        if (i < 1024)
        {
            text_surface = TTF_RenderUTF8_Blended(GAME_FONT_48, text, target_color);
            TTF_SizeUTF8(GAME_FONT_48, text, &text_surface->w, &text_surface->h);
        }
        else
        {
            text_surface = TTF_RenderUTF8_Blended(GAME_FONT_40, text, target_color);
            TTF_SizeUTF8(GAME_FONT_40, text, &text_surface->w, &text_surface->h);
        }
        TILE_TEXT_SURFACES.push_back(text_surface);
        TILE_TEXT_TEXTURES.push_back(SDL_CreateTextureFromSurface(renderer, text_surface));
    }
    TILE_BIG_TEXTURE = IMG_LoadTexture(renderer, "assets/img/tile.png");
    // Verify assets
    if (DEBUG_FONT == nullptr || UI_FONT_BOLD_32 == nullptr || GAME_FONT_48 == nullptr || BTN_LEFT_TEXTURE == nullptr || BTN_RIGHT_TEXTURE == nullptr || BTN_MID_TEXTURE == nullptr || BTN_LEFT_HOVER_TEXTURE == nullptr || BTN_RIGHT_HOVER_TEXTURE == nullptr || BTN_MID_HOVER_TEXTURE == nullptr || BOARD_TEXTURE == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't load assets: %s", SDL_GetError());
        return 0;
    }
    return 1;
}
void destroy_assets()
{
    TTF_CloseFont(DEBUG_FONT);
    TTF_CloseFont(UI_FONT_24);
    TTF_CloseFont(UI_FONT_BOLD_32);
    TTF_CloseFont(GAME_FONT_48);
    SDL_DestroyTexture(BTN_LEFT_TEXTURE);
    SDL_DestroyTexture(BTN_RIGHT_TEXTURE);
    SDL_DestroyTexture(BTN_MID_TEXTURE);
    SDL_DestroyTexture(BTN_LEFT_HOVER_TEXTURE);
    SDL_DestroyTexture(BTN_RIGHT_HOVER_TEXTURE);
    SDL_DestroyTexture(BTN_MID_HOVER_TEXTURE);
    SDL_DestroyTexture(BOARD_TEXTURE);
    for (auto texture : TILE_TEXTURES)
    {
        SDL_DestroyTexture(texture);
    }
    for (auto texture : TILE_TEXT_TEXTURES)
    {
        SDL_DestroyTexture(texture);
    }
    for (auto surface : TILE_TEXT_SURFACES)
    {
        SDL_FreeSurface(surface);
    }
    return;
}
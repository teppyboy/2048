#pragma once

#include <functional>
#include <SDL.h>
#include <SDL_image.h>
#include "screen.hpp"
#include <src/element/button.hpp>

class Game : public Screen
{
    SDL_Renderer *renderer;
    SDL_Window *window;
    // Background
    SDL_Texture *bg_texture;
    // Board
    SDL_Rect board_rect;

public:
    Game(SDL_Renderer *renderer, SDL_Window *window)
    {
        this->renderer = renderer;
        this->window = window;
        bg_texture = IMG_LoadTexture(renderer, "assets/img/bg_game.png");
        board_rect.x = 1280 / 2 - 512 / 2;
        board_rect.y = 720 / 2 - 512 / 2;
        board_rect.w = 512;
        board_rect.h = 512;
    }
    int handle_event(SDL_Event event)
    {
        return 0;
    }
    void render()
    {
        SDL_RenderCopy(renderer, bg_texture, NULL, NULL);
        SDL_RenderCopy(renderer, BOARD_TEXTURE, NULL, &board_rect);
    }
    ~Game()
    {
        return;
    }
    void destroy()
    {
        delete this;
    }
};
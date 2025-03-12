#pragma once

#include <functional>
#include <SDL.h>
#include <SDL_image.h>
#include "game.hpp"
#include "screen.hpp"
#include "transition.hpp"
#include <src/element/button.hpp>

class MainMenu : public Screen
{
    SDL_Texture *bg_texture;
    SDL_Rect bg_rect;
    SDL_Renderer *renderer;
    SDL_Window *window;
    SDL_Event event;
    Button *start_button;
    Button *load_button;
    Button *settings_button;
    // Logo part
    SDL_Texture *logo_texture;
    SDL_Rect logo_rect;
    uint64_t last_tick;
    int last_frame;
    Screen *game;
    Screen *transition;
public:
    MainMenu(SDL_Renderer *renderer, SDL_Window *window, Screen *game)
    {
        this->renderer = renderer;
        this->window = window;
        this->game = game;
        bg_texture = IMG_LoadTexture(renderer, "assets/img/bg_menu.png");
        last_tick = SDL_GetTicks64();
        last_frame = 240;
        logo_texture = IMG_LoadTexture(renderer, "assets/img/logo.png");
        logo_rect.x = 1280 / 2 - 512 / 2;
        logo_rect.y = -76;
        logo_rect.w = 512;
        logo_rect.h = 512;
        // Buttons
        start_button = new Button(renderer, window, "Start", 1280 / 2 - 300 / 2, 720 / 2, 300, 60, [&]()
                                  { start_button_callback(); });
        load_button = new Button(renderer, window, "Load", 1280 / 2 - 300 / 2, 720 / 2 + 80, 300, 60, [&]()
                                     { settings_button_callback(); });
        settings_button = new Button(renderer, window, "Settings", 1280 / 2 - 300 / 2, 720 / 2 + 80 + 80, 300, 60, [&]()
                                     { settings_button_callback(); });
    }
    void start_button_callback()
    {
        SDL_LogVerbose(0, "Start button clicked.");
        transition_set(this, game, 1000, State::GAME);
    }
    void load_button_callback()
    {
        SDL_LogVerbose(0, "Load button clicked.");
    }
    void settings_button_callback()
    {
        SDL_LogVerbose(0, "Settings button clicked.");
    }
    int handle_event(SDL_Event event)
    {
        start_button->handle_event(event);
        settings_button->handle_event(event);
        load_button->handle_event(event);
        return 0;
    }
    void render()
    {
        SDL_RenderCopy(renderer, bg_texture, NULL, NULL);
        SDL_RenderCopy(renderer, logo_texture, NULL, &logo_rect);
        (*start_button).render();
        (*load_button).render();
        (*settings_button).render();
    }
    ~MainMenu()
    {
        SDL_DestroyTexture(bg_texture);
        SDL_DestroyTexture(logo_texture);
        delete start_button;
        delete settings_button;
    }
    void destroy()
    {
        delete this;
    }
};
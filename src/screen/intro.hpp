#include <functional>
#include <SDL.h>
#include <SDL_image.h>
#include "screen.hpp"
#include "main_menu.hpp"
#include "../constants.hpp"
#include "../element/button.hpp"
#include "../state.hpp"

#define INTRO_DURATION 1000

class Intro : public Screen
{
    SDL_Renderer *renderer;
    SDL_Window *window;
    SDL_Texture *menu_as_texture;
    Screen *main_menu;
    Uint64 start_time;
    // Board
public:
    Intro(SDL_Renderer *renderer, SDL_Window *window, Screen *main_menu)
    {
        this->renderer = renderer;
        this->window = window;
        this->main_menu = main_menu;
        start_time = SDL_GetTicks64();
        menu_as_texture = SDL_CreateTexture(
            renderer,
            SDL_PIXELFORMAT_RGBA8888,
            SDL_TEXTUREACCESS_TARGET,
            DEFAULT_WIDTH, DEFAULT_HEIGHT
        );
        SDL_SetTextureBlendMode(menu_as_texture, SDL_BLENDMODE_BLEND);
    }
    int handle_event(SDL_Event event)
    {
        return 0;
    }
    void render()
    {
        auto time_elapsed = SDL_GetTicks64() - start_time;
        if (time_elapsed > INTRO_DURATION)
        {
            // Set the texture to full opacity and render copy to prevent black flickering
            SDL_SetTextureAlphaMod(menu_as_texture, 255);
            SDL_RenderCopy(renderer, menu_as_texture, NULL, NULL);
            game_state = State::MAIN_MENU;
            return;
        }
        auto opacity = (int)((double)(SDL_GetTicks64() - start_time) / INTRO_DURATION * 255);
        SDL_SetRenderTarget(renderer, menu_as_texture);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);
        main_menu->render();
        SDL_SetRenderTarget(renderer, NULL);
        SDL_LogVerbose(0, "Opacity: %d", opacity);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_SetTextureAlphaMod(menu_as_texture, opacity);
        SDL_RenderCopy(renderer, menu_as_texture, NULL, NULL);
    }
    ~Intro()
    {
        return;
    }
    void destroy()
    {
        delete this;
    }
};
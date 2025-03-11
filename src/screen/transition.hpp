#pragma once

#include <functional>
#include <SDL.h>
#include <SDL_image.h>
#include "screen.hpp"
#include "main_menu.hpp"
#include "../constants.hpp"
#include "../element/button.hpp"
#include "../state.hpp"

class Transition : public Screen
{
    SDL_Renderer *renderer;
    SDL_Window *window;
    SDL_Texture *src_screen_as_texture;
    SDL_Texture *dst_screen_as_texture;
    SDL_Rect *screen_rect;
    Screen *src_screen;
    Screen *dst_screen;
    Uint64 start_time;
    int duration_ms;
    // Board
public:
    Transition(SDL_Renderer *renderer, SDL_Window *window, Screen *src_screen, Screen *dst_screen, int duration_ms)
    {
        this->renderer = renderer;
        this->window = window;
        this->src_screen = src_screen;
        this->dst_screen = dst_screen;
        this->screen_rect = new SDL_Rect();
        start_time = SDL_GetTicks64();
        src_screen_as_texture = SDL_CreateTexture(
            renderer,
            SDL_PIXELFORMAT_RGBA8888,
            SDL_TEXTUREACCESS_TARGET,
            DEFAULT_WIDTH, DEFAULT_HEIGHT
        );
        dst_screen_as_texture = SDL_CreateTexture(
            renderer,
            SDL_PIXELFORMAT_RGBA8888,
            SDL_TEXTUREACCESS_TARGET,
            DEFAULT_WIDTH, DEFAULT_HEIGHT
        );
        SDL_SetTextureBlendMode(src_screen_as_texture, SDL_BLENDMODE_BLEND);
        SDL_SetTextureBlendMode(dst_screen_as_texture, SDL_BLENDMODE_BLEND);
    }
    int handle_event(SDL_Event event)
    {
        return 0;
    }
    void render()
    {
        auto time_elapsed = SDL_GetTicks64() - start_time;
        SDL_LogVerbose(0, "Time elapsed: %lld", time_elapsed);
        if (time_elapsed > duration_ms)
        {
            // Set the texture to full opacity and render copy to prevent black flickering
            SDL_SetTextureAlphaMod(dst_screen_as_texture, 255);
            SDL_RenderCopy(renderer, dst_screen_as_texture, NULL, NULL);
            request_transition = false;
            game_state = transition_to;
            return;
        }
        auto opacity = (int)((double)(SDL_GetTicks64() - start_time) / duration_ms * 255);
        SDL_LogVerbose(0, "Opacity: %d", opacity);
        // Source screen
        SDL_SetRenderTarget(renderer, src_screen_as_texture);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);
        src_screen->render();
        // Destination screen
        SDL_SetRenderTarget(renderer, dst_screen_as_texture);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);
        dst_screen->render();
        SDL_SetRenderTarget(renderer, NULL);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_SetTextureAlphaMod(src_screen_as_texture, 255 - opacity);
        SDL_RenderCopy(renderer, src_screen_as_texture, NULL, NULL);
        SDL_SetTextureAlphaMod(dst_screen_as_texture, opacity);
        SDL_RenderCopy(renderer, dst_screen_as_texture, NULL, NULL);
    }
    ~Transition()
    {
        return;
    }
    void destroy()
    {
        delete this;
    }
};
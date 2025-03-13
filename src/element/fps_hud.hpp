#pragma once

#include <SDL_ttf.h>
#include "../screen/screen.hpp"

/// @brief HUD for displaying the current FPS.
class FPSHUD : public Screen
{
    SDL_Texture *fps_tex;
    SDL_Rect fps_tex_rect;
    SDL_Surface *sf_text_fps;
    std::string fps_text;
    Uint64 current_tick;
    Uint64 prev_tick;
    Uint64 last_draw_fps;
    bool request_redraw_fps;
    SDL_Renderer *renderer;
    SDL_Window *window;
    double avg_frametime;

public:
    FPSHUD(SDL_Renderer *renderer, SDL_Window *window)
    {
        // Somewhat fake here but okay xd
        avg_frametime = 0;
        this->renderer = renderer;
        this->window = window;
        request_redraw_fps = true;
        fps_tex = nullptr;
        sf_text_fps = nullptr;
        fps_text = std::string("FPS: ??");
        prev_tick = 1;
        last_draw_fps = 1;
        fps_tex_rect.y = 4;
        fps_tex_rect.h = -1;
    }
    int handle_event(SDL_Event _)
    {
        return 0;
    }
    void render()
    {
        current_tick = SDL_GetTicks64();
        // Render current FPS to screen
        auto time_elapsed = current_tick - last_draw_fps;
        auto tick_time = current_tick - prev_tick;
        if (tick_time == 0)
        {
            tick_time = 1;
        }
        if (avg_frametime == 0)
        {
            avg_frametime = tick_time;
        }
        else
        {
            avg_frametime = (avg_frametime + tick_time) / 2;
        }
        // SDL_LogVerbose(0, "FPS time: %lld", time_elapsed);
        // Render every second
        if (time_elapsed > 1000)
        {
            request_redraw_fps = true;
            last_draw_fps = current_tick;
        }
        else if (request_redraw_fps)
        {
            request_redraw_fps = false;
            fps_text = "FPS: " + std::to_string((int)round((double)1000 / avg_frametime));
            sf_text_fps = TTF_RenderUTF8_Blended(DEBUG_FONT, fps_text.c_str(), WHITE_RGB);
            fps_tex = SDL_CreateTextureFromSurface(renderer, sf_text_fps);
            TTF_SizeUTF8(DEBUG_FONT, fps_text.c_str(), &sf_text_fps->w, &sf_text_fps->h);
            SDL_GetWindowSizeInPixels(window, &fps_tex_rect.x, NULL);
            fps_tex_rect.w = sf_text_fps->w; // controls the width of the rect
            fps_tex_rect.x -= fps_tex_rect.w + 4;
            if (fps_tex_rect.h == -1)
            {
                fps_tex_rect.h = sf_text_fps->h;
            }
            avg_frametime = 0;
        }
        prev_tick = current_tick;
        if (fps_tex_rect.w < 0 || fps_tex_rect.h < 0)
        {
            return;
        }
        SDL_RenderCopy(renderer, fps_tex, NULL, &fps_tex_rect);
    }
    ~FPSHUD()
    {
        SDL_DestroyTexture(fps_tex);
        SDL_FreeSurface(sf_text_fps);
    }
    void destroy()
    {
        delete this;
    }
};
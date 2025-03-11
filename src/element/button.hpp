#pragma once

#include <string>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "../assets.hpp"
#include "../screen/screen.hpp"
#include <functional>

class Button : public Screen
{
    std::string text;
    bool is_hovered;
    SDL_Texture *text_texture;
    SDL_Renderer *renderer;
    SDL_Window *window;
    SDL_Rect text_rect;
    SDL_Rect btn_rect;
    SDL_Rect btn_left_rect;
    SDL_Rect btn_right_rect;
    SDL_Rect btn_mid_rect;
    
    // The dark magic thing.
    std::function<void()> callback;

public:
    /// @brief Creates a new button.
    /// You'll have to calculate the width and height of the button yourself.
    /// @param renderer: The SDL renderer
    /// @param window: The SDL window
    /// @param text: The button content
    /// @param x: The x position
    /// @param y: The y position
    /// @param w: The width
    /// @param h: The height
    /// @param callback: The callback function
    Button(SDL_Renderer *renderer, SDL_Window *window, std::string text, int x, int y, int w, int h, std::function<void()> callback)
    {
        this->text = text;
        this->renderer = renderer;
        this->window = window;
        this->callback = callback;
        // The left and right part of the button has 70x256 size, so we can do some math here.
        // Button part
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
        btn_rect = {x, y, w, h};
        auto calc_w = (int)ceil(70 * ((double)h / 256));
        SDL_LogVerbose(0, "[Button] Calculated width: %d", (int)(70 * ((double)h / 256)));
        btn_left_rect = {x, y, calc_w, h};
        btn_right_rect = {x + w - calc_w, y, calc_w, h};
        btn_mid_rect = {x - (calc_w - w / 70), y, w + (calc_w - w / 70) * 2, h};
        // Text part
        SDL_Surface *text_surface = TTF_RenderUTF8_Blended(UI_FONT_32, text.c_str(), WHITE_RGB);
        text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
        TTF_SizeUTF8(UI_FONT_32, text.c_str(), &text_surface->w, &text_surface->h);
        text_rect.x = x + w / 2 - text_surface->w / 2;
        text_rect.y = y + h / 2 - text_surface->h / 2;
        text_rect.w = text_surface->w;
        text_rect.h = text_surface->h;
    }
    void render()
    {
        if (is_hovered)
        {
            SDL_RenderCopy(renderer, BTN_LEFT_HOVER_TEXTURE, NULL, &btn_left_rect);
            SDL_RenderCopy(renderer, BTN_RIGHT_HOVER_TEXTURE, NULL, &btn_right_rect);
            SDL_RenderCopy(renderer, BTN_MID_HOVER_TEXTURE, NULL, &btn_mid_rect);
        }
        else
        {
            SDL_RenderCopy(renderer, BTN_LEFT_TEXTURE, NULL, &btn_left_rect);
            SDL_RenderCopy(renderer, BTN_RIGHT_TEXTURE, NULL, &btn_right_rect);
            SDL_RenderCopy(renderer, BTN_MID_TEXTURE, NULL, &btn_mid_rect);
        }
        SDL_RenderCopy(renderer, text_texture, NULL, &text_rect);
    }
    int handle_event(SDL_Event event)
    {
        int x, y;
        SDL_GetMouseState(&x, &y);
        if (x >= btn_rect.x && x <= btn_rect.x + btn_rect.w && y >= btn_rect.y && y <= btn_rect.y + btn_rect.h)
        {
            if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                callback();
            }
            else
            {
                is_hovered = true;
            }
        } else {
            is_hovered = false;
        }
        return 0;
    }
    void destroy()
    {
        SDL_DestroyTexture(text_texture);
    }
};
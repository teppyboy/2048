#pragma once

#include <string>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "../assets.hpp"
#include "../screen/screen.hpp"
#include <functional>

class RectangleCheckbox : public Screen
{
    std::string text;
    bool is_hovered;
    bool is_checked;
    SDL_Texture *text_texture;
    SDL_Renderer *renderer;
    SDL_Window *window;
    SDL_Rect btn_checked_rect;
    SDL_Rect btn_rect;

    // The dark magic thing.
    std::function<void(bool)> callback;

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
    RectangleCheckbox(SDL_Renderer *renderer, SDL_Window *window, int x, int y, int w, int h, std::function<void(bool)> callback)
    {
        this->text = text;
        this->renderer = renderer;
        this->window = window;
        this->callback = callback;
        is_checked = false;
        btn_rect = {x, y, w, h};
        btn_checked_rect = {
            x + 10,
            y + 10,
            h - 20,
            h - 20,
        };
    }
    void render()
    {
        if (is_hovered)
        {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 40);
        }
        else
        {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 120);
        }
        SDL_RenderFillRect(renderer, &btn_rect);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 80);
        SDL_RenderDrawRect(renderer, &btn_rect);
        if (is_checked)
        {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderFillRect(renderer, &btn_checked_rect);
        }
    }
    void set_checked(bool checked)
    {
        is_checked = checked;
    }
    int handle_event(SDL_Event event)
    {
        int x, y;
        SDL_GetMouseState(&x, &y);
        if (x >= btn_rect.x && x <= btn_rect.x + btn_rect.w && y >= btn_rect.y && y <= btn_rect.y + btn_rect.h)
        {
            if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                is_checked = !is_checked;
                callback(is_checked);
            }
            else
            {
                is_hovered = true;
            }
        }
        else
        {
            is_hovered = false;
        }
        return 0;
    }
    void destroy()
    {
        SDL_DestroyTexture(text_texture);
    }
};
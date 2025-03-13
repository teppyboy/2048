#pragma once

#include <functional>
#include <chrono>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include "../state.hpp"
#include "game.hpp"
#include "../element/button.hpp"
#include "../utils.hpp"

class Saves : public Screen
{
    SDL_Texture *bg_texture;
    SDL_Rect save_bg_rect;
    SDL_Renderer *renderer;
    SDL_Window *window;
    SDL_Event event;
    // Logo part
    SDL_Rect logo_rect;
    uint64_t last_tick;
    Game *game;
    // "Save game" text
    SDL_Texture *save_game_text_texture;
    SDL_Rect save_game_text_rect;
    // Save game info text
    SDL_Texture *save_game_info_text_texture;
    SDL_Rect save_game_info_text_rect;
    // pain pain pain
    RectangleButton *return_btn;
    RectangleButton *add_save_btn;
    // Save list
    int prev_save_len;
    SDL_Texture *save_list_screen_as_texture;
    std::vector<RectangleButton *> save_list_load_buttons;
    std::vector<RectangleButton *> save_list_delete_buttons;

public:
    Saves(SDL_Renderer *renderer, SDL_Window *window, Game *game)
    {
        this->renderer = renderer;
        this->window = window;
        this->game = game;
        prev_save_len = -1;
        bg_texture = IMG_LoadTexture(renderer, "assets/img/bg_save.png");
        last_tick = SDL_GetTicks64();
        save_bg_rect = {
            10,
            70,
            1280 - 20,
            720 - 80 - 50,
        };
        SDL_Surface *save_game_text_surface = TTF_RenderUTF8_Blended(UI_FONT_YURUKA_40, "SAVE GAME", TILE_TEXT_LIGHT_RGB);
        save_game_text_texture = SDL_CreateTextureFromSurface(renderer, save_game_text_surface);
        TTF_SizeUTF8(UI_FONT_YURUKA_40, "SAVE GAME", &save_game_text_surface->w, &save_game_text_surface->h);
        save_game_text_rect = {
            1280 / 2 - save_game_text_surface->w / 2,
            16,
            save_game_text_surface->w,
            save_game_text_surface->h,
        };
        auto save_game_info_c_str = "You can create and delete save files here, they are stored in 'user_data.json' file.";
        SDL_Surface *save_game_info_text_surface = TTF_RenderUTF8_Blended(UI_FONT_24, save_game_info_c_str, TILE_TEXT_LIGHT_RGB);
        save_game_info_text_texture = SDL_CreateTextureFromSurface(renderer, save_game_info_text_surface);
        TTF_SizeUTF8(UI_FONT_24, save_game_info_c_str, &save_game_info_text_surface->w, &save_game_info_text_surface->h);
        save_game_info_text_rect = {
            save_bg_rect.x + 10,
            save_bg_rect.y + 10,
            save_game_info_text_surface->w,
            save_game_info_text_surface->h,
        };
        SDL_FreeSurface(save_game_text_surface);
        SDL_FreeSurface(save_game_info_text_surface);
        // pain pain pain
        add_save_btn = new RectangleButton(renderer, window, "+", 1280 - 20 - 30, 70 + 10, 30, 30, [&]()
                                           { add_save_callback(); });
        return_btn = new RectangleButton(renderer, window, "Return", 10, 720 - 50, 90, 40, [&]()
                                         { return_callback(); });
        // Save list
        save_list_screen_as_texture = SDL_CreateTexture(
            renderer,
            SDL_PIXELFORMAT_RGBA8888,
            SDL_TEXTUREACCESS_TARGET,
            DEFAULT_WIDTH, DEFAULT_HEIGHT);
    }
    void add_save_callback()
    {
        SDL_LogVerbose(0, "Add save button clicked.");
        user_data.saves.push_back(Save{
            .date = time(0),
            .score = game->board.score,
            .grid = game->board.grid,
        });
    }
    void return_callback()
    {
        SDL_LogVerbose(0, "Return button clicked.");
        transition_set(this, prev_screen, 500, prev_state);
    }
    int handle_event(SDL_Event event)
    {
        if (prev_save_len < 7)
        {
            add_save_btn->handle_event(event);
        }
        for (auto btn : save_list_load_buttons)
        {
            btn->handle_event(event);
        }
        for (auto btn : save_list_delete_buttons)
        {
            btn->handle_event(event);
        }
        return_btn->handle_event(event);
        if (event.type == SDL_KEYDOWN)
        {
            switch (event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                return_callback();
                return 0;
            default:
                return 0;
            }
        }
        return 0;
    }
    void render_save_list()
    {
        if (user_data.saves.size() == prev_save_len)
        {
            return;
        }
        // Clean up old buttons
        for (auto btn : save_list_load_buttons)
        {
            delete btn;
        }
        for (auto btn : save_list_delete_buttons)
        {
            delete btn;
        }
        save_list_load_buttons.clear();
        save_list_delete_buttons.clear();
        SDL_SetRenderTarget(renderer, save_list_screen_as_texture);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, bg_texture, NULL, NULL);
        SDL_RenderCopy(renderer, save_game_text_texture, NULL, &save_game_text_rect);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 100);
        SDL_RenderFillRect(renderer, &save_bg_rect);
        SDL_RenderCopy(renderer, save_game_info_text_texture, NULL, &save_game_info_text_rect);
        for (int i = 0; i < user_data.saves.size(); i++)
        {
            auto save = user_data.saves[i];
            SDL_Rect save_rect = {
                save_bg_rect.x + 10,
                save_bg_rect.y + 50 + 70 * i,
                save_bg_rect.w - 20,
                60,
            };
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 100);
            SDL_RenderFillRect(renderer, &save_rect);
            SDL_Color text_color = {255, 255, 255, 255};
            auto pretty_time = unix_time_to_string(save.date, "%Y-%m-%d %H:%M:%S");
            std::string save_info = "Save " + std::to_string(i + 1) + " | " + pretty_time + " | " + std::to_string(save.score) + " points";
            SDL_Surface *save_info_surface = TTF_RenderUTF8_Blended(UI_FONT_24, save_info.c_str(), text_color);
            SDL_Texture *save_info_texture = SDL_CreateTextureFromSurface(renderer, save_info_surface);
            SDL_Rect save_info_rect = {
                save_rect.x + 10,
                save_rect.y + 60 / 2 - save_info_surface->h / 2,
                save_info_surface->w,
                save_info_surface->h,
            };
            save_list_load_buttons.push_back(new RectangleButton(renderer, window, "Load", save_rect.x + save_rect.w - 10 - 70, save_rect.y + 10, 70, 40, [&, i]()
                                                                 {
                                                                    SDL_LogVerbose(0, "Load save button clicked.");
                                                                    game->board.grid = user_data.saves[i].grid;
                                                                    game->board.score = user_data.saves[i].score;
                                                                    game->board.won = false;
                                                                    game->move_result.moved = true;
                                                                    transition_set(this, game, 500, State::GAME); }));
            save_list_delete_buttons.push_back(new RectangleButton(renderer, window, "Delete", save_rect.x + save_rect.w - 10 - 60 - 110, save_rect.y + 10, 90, 40, [&, i]()
                                                                   {
                                                                      SDL_LogVerbose(0, "Delete save button clicked.");
                                                                      user_data.saves.erase(user_data.saves.begin() + i); }));
            SDL_RenderCopy(renderer, save_info_texture, NULL, &save_info_rect);
            SDL_FreeSurface(save_info_surface);
            SDL_DestroyTexture(save_info_texture);
        }
        prev_save_len = user_data.saves.size();
        SDL_SetTextureAlphaMod(save_list_screen_as_texture, 255);
        SDL_SetRenderTarget(renderer, NULL);
    }
    void render()
    {
        render_save_list();
        SDL_RenderCopy(renderer, save_list_screen_as_texture, NULL, NULL);
        if (prev_save_len < 7)
        {
            add_save_btn->render();
        }
        for (auto btn : save_list_load_buttons)
        {
            btn->render();
        }
        for (auto btn : save_list_delete_buttons)
        {
            btn->render();
        }
        return_btn->render();
    }
    ~Saves()
    {
        SDL_DestroyTexture(bg_texture);
    }
    void destroy()
    {
        delete this;
    }
};
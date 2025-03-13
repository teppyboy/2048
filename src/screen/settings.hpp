#pragma once

#include <functional>
#include <chrono>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include "game.hpp"
#include "../audio.hpp"
#include "../state.hpp"
#include "../element/button.hpp"
#include "../element/checkbox.hpp"
#include "../utils.hpp"

class SettingsScreen : public Screen
{
    SDL_Texture *bg_texture;
    SDL_Rect settings_bg_rect;
    SDL_Renderer *renderer;
    SDL_Window *window;
    SDL_Event event;
    // Logo part
    SDL_Rect logo_rect;
    uint64_t last_tick;
    Game *game;
    // "Save game" text
    SDL_Texture *settings_text_texture;
    SDL_Rect settings_text_rect;
    // Save game info text
    SDL_Texture *settings_info_text_texture;
    SDL_Rect settings_info_text_rect;
    // pain pain pain
    RectangleButton *return_btn;
    RectangleCheckbox *fps_toggle;
    SDL_Rect fps_toggle_rect;
    SDL_Texture *fps_toggle_text_texture;
    SDL_Rect fps_toggle_text_rect;
    RectangleCheckbox *bgm_toggle;
    SDL_Rect bgm_toggle_rect;
    SDL_Texture *bgm_toggle_text_texture;
    SDL_Rect bgm_toggle_text_rect;
    RectangleCheckbox *sfx_toggle;
    SDL_Rect sfx_toggle_rect;
    SDL_Texture *sfx_toggle_text_texture;
    SDL_Rect sfx_toggle_text_rect;
    // Settings section
    bool settings_changed;
    SDL_Texture *screen_as_texture;
    void init_setting_sections()
    {
        int i = 0;
        // FPS counter
        fps_toggle_rect = {
            settings_bg_rect.x + 10,
            settings_bg_rect.y + 50 + 70 * i,
            settings_bg_rect.w - 20,
            60,
        };
        auto fps_info_c_str = "FPS counter";
        SDL_Surface *fps_info_surface = TTF_RenderUTF8_Blended(UI_FONT_24, fps_info_c_str, TILE_TEXT_LIGHT_RGB);
        fps_toggle_text_texture = SDL_CreateTextureFromSurface(renderer, fps_info_surface);
        TTF_SizeUTF8(UI_FONT_24, fps_info_c_str, &fps_info_surface->w, &fps_info_surface->h);
        fps_toggle_text_rect = {
            fps_toggle_rect.x + 10,
            fps_toggle_rect.y + 60 / 2 - fps_info_surface->h / 2,
            fps_info_surface->w,
            fps_info_surface->h,
        };
        SDL_FreeSurface(fps_info_surface);
        fps_toggle = new RectangleCheckbox(renderer, window, fps_toggle_rect.x + fps_toggle_rect.w - 10 - 40, fps_toggle_rect.y + 10, 40, 40, [&](bool checked)
        { toggle_fps(checked); });
        fps_toggle->set_checked(settings.fps_hud);
        i++;
        // BGM
        bgm_toggle_rect = {
            settings_bg_rect.x + 10,
            settings_bg_rect.y + 50 + 70 * i,
            settings_bg_rect.w - 20,
            60,
        };
        auto bgm_toggle_c_str = "Background Music";
        SDL_Surface *bgm_toggle_surface = TTF_RenderUTF8_Blended(UI_FONT_24, bgm_toggle_c_str, TILE_TEXT_LIGHT_RGB);
        bgm_toggle_text_texture = SDL_CreateTextureFromSurface(renderer, bgm_toggle_surface);
        TTF_SizeUTF8(UI_FONT_24, bgm_toggle_c_str, &bgm_toggle_surface->w, &bgm_toggle_surface->h);
        bgm_toggle_text_rect = {
            bgm_toggle_rect.x + 10,
            bgm_toggle_rect.y + 60 / 2 - bgm_toggle_surface->h / 2,
            bgm_toggle_surface->w,
            bgm_toggle_surface->h,
        };
        SDL_FreeSurface(bgm_toggle_surface);
        bgm_toggle = new RectangleCheckbox(renderer, window, bgm_toggle_rect.x + bgm_toggle_rect.w - 10 - 40, bgm_toggle_rect.y + 10, 40, 40, [&](bool checked)
        { toggle_bgm(checked); });
        bgm_toggle->set_checked(settings.music_volume > 0);
        i++;
        // SFX
        sfx_toggle_rect = {
            settings_bg_rect.x + 10,
            settings_bg_rect.y + 50 + 70 * i,
            settings_bg_rect.w - 20,
            60,
        };
        auto sfx_toggle_c_str = "Sound Effects";
        SDL_Surface *sfx_toggle_surface = TTF_RenderUTF8_Blended(UI_FONT_24, sfx_toggle_c_str, TILE_TEXT_LIGHT_RGB);
        sfx_toggle_text_texture = SDL_CreateTextureFromSurface(renderer, sfx_toggle_surface);
        TTF_SizeUTF8(UI_FONT_24, sfx_toggle_c_str, &sfx_toggle_surface->w, &sfx_toggle_surface->h);
        sfx_toggle_text_rect = {
            sfx_toggle_rect.x + 10,
            sfx_toggle_rect.y + 60 / 2 - sfx_toggle_surface->h / 2,
            sfx_toggle_surface->w,
            sfx_toggle_surface->h,
        };
        SDL_FreeSurface(sfx_toggle_surface);
        sfx_toggle = new RectangleCheckbox(renderer, window, sfx_toggle_rect.x + sfx_toggle_rect.w - 10 - 40, sfx_toggle_rect.y + 10, 40, 40, [&](bool checked)
        { toggle_sfx(checked); });
        sfx_toggle->set_checked(settings.sfx_volume > 0);
    }

public:
    SettingsScreen(SDL_Renderer *renderer, SDL_Window *window, Game *game)
    {
        this->renderer = renderer;
        this->window = window;
        this->game = game;
        settings_changed = true;
        bg_texture = IMG_LoadTexture(renderer, "assets/img/bg_settings.png");
        last_tick = SDL_GetTicks64();
        settings_bg_rect = {
            10,
            70,
            1280 - 20,
            720 - 80 - 50,
        };
        SDL_Surface *settings_text_surface = TTF_RenderUTF8_Blended(UI_FONT_YURUKA_40, "SETTINGS", TILE_TEXT_LIGHT_RGB);
        settings_text_texture = SDL_CreateTextureFromSurface(renderer, settings_text_surface);
        TTF_SizeUTF8(UI_FONT_YURUKA_40, "SETTINGS", &settings_text_surface->w, &settings_text_surface->h);
        settings_text_rect = {
            1280 / 2 - settings_text_surface->w / 2,
            16,
            settings_text_surface->w,
            settings_text_surface->h,
        };
        auto save_game_info_c_str = "You can change basic settings here, for advanced (and unstable) configuration please check 'settings.json' file.";
        SDL_Surface *save_game_info_text_surface = TTF_RenderUTF8_Blended(UI_FONT_24, save_game_info_c_str, TILE_TEXT_LIGHT_RGB);
        settings_info_text_texture = SDL_CreateTextureFromSurface(renderer, save_game_info_text_surface);
        TTF_SizeUTF8(UI_FONT_24, save_game_info_c_str, &save_game_info_text_surface->w, &save_game_info_text_surface->h);
        settings_info_text_rect = {
            settings_bg_rect.x + 10,
            settings_bg_rect.y + 10,
            save_game_info_text_surface->w,
            save_game_info_text_surface->h,
        };
        SDL_FreeSurface(settings_text_surface);
        SDL_FreeSurface(save_game_info_text_surface);
        // pain pain pain
        return_btn = new RectangleButton(renderer, window, "Return", 10, 720 - 50, 90, 40, [&]()
                                         { return_callback(); });
        init_setting_sections();
        screen_as_texture = SDL_CreateTexture(
            renderer,
            SDL_PIXELFORMAT_RGBA8888,
            SDL_TEXTUREACCESS_TARGET,
            DEFAULT_WIDTH, DEFAULT_HEIGHT);
    }
    void toggle_bgm(bool checked)
    {
        SDL_LogVerbose(0, "BGM toggle clicked.");
        settings.music_volume = checked ? 70 : 0;
        settings.save();
        set_bgm_volume(settings.music_volume);
        settings_changed = true;
    }
    void toggle_sfx(bool checked)
    {
        SDL_LogVerbose(0, "SFX toggle clicked.");
        settings.sfx_volume = checked ? 100 : 0;
        settings.save();
        set_sfx_volume(settings.sfx_volume);
        settings_changed = true;
    }
    void toggle_fps(bool checked)
    {
        SDL_LogVerbose(0, "FPS toggle clicked.");
        settings.fps_hud = checked;
        settings.save();
        settings_changed = true;
    }
    void return_callback()
    {
        SDL_LogVerbose(0, "Return button clicked.");
        transition_set(this, prev_screen, 500, prev_state);
    }
    int handle_event(SDL_Event event)
    {
        return_btn->handle_event(event);
        fps_toggle->handle_event(event);
        bgm_toggle->handle_event(event);
        sfx_toggle->handle_event(event);
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
    void render_settings()
    {
        if (!settings_changed)
        {
            return;
        }
        SDL_SetRenderTarget(renderer, screen_as_texture);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, bg_texture, NULL, NULL);
        SDL_RenderCopy(renderer, settings_text_texture, NULL, &settings_text_rect);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 100);
        SDL_RenderFillRect(renderer, &settings_bg_rect);
        SDL_RenderCopy(renderer, settings_info_text_texture, NULL, &settings_info_text_rect);
        // Render shiet things
        SDL_RenderFillRect(renderer, &fps_toggle_rect);
        SDL_RenderCopy(renderer, fps_toggle_text_texture, NULL, &fps_toggle_text_rect);
        SDL_RenderFillRect(renderer, &bgm_toggle_rect);
        SDL_RenderCopy(renderer, bgm_toggle_text_texture, NULL, &bgm_toggle_text_rect);
        SDL_RenderFillRect(renderer, &sfx_toggle_rect);
        SDL_RenderCopy(renderer, sfx_toggle_text_texture, NULL, &sfx_toggle_text_rect);
        // Cleanup
        SDL_SetRenderTarget(renderer, NULL);
        settings_changed = false;
    }
    void render()
    {
        render_settings();
        SDL_RenderCopy(renderer, screen_as_texture, NULL, NULL);
        return_btn->render();
        fps_toggle->render();
        bgm_toggle->render();
        sfx_toggle->render();
    }
    ~SettingsScreen()
    {
        SDL_DestroyTexture(bg_texture);
    }
    void destroy()
    {
        delete this;
    }
};
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "screen.hpp"

class MainMenu : public Screen
{
    SDL_Texture *bg_texture;
    SDL_Texture *text;
    SDL_Rect text_rect;
    TTF_Font *font_menu;
    SDL_Surface *surface_text;
    SDL_Surface *surface_bg;
    SDL_Rect bg_rect;
    SDL_Renderer *renderer;
    SDL_Window *window;
    SDL_Event event;
    bool request_redraw_fps;
    Uint64 prev_tick;
    std::string fps_text;
    SDL_Surface *sf_text_fps;
    SDL_Texture *fps_tex;
    SDL_Rect fps_tex_rect;
    double default_wait;
    Uint32 max_wait;
    Uint64 frame_start;
    Uint64 frame_end;
    double wait_time;

public:
    MainMenu(SDL_Renderer *renderer, SDL_Window *window)
    {
        this->renderer = renderer;
        this->window = window;
        bg_texture = IMG_LoadTexture(renderer, "assets/images/bg.jpg");
        font_menu = TTF_OpenFont("assets/fonts/Shizuru-Regular.ttf", 64);
        surface_text = TTF_RenderUTF8_Blended(font_menu, "2048 pho riu", WHITE_RGB);
        text = SDL_CreateTextureFromSurface(renderer, surface_text);
        TTF_SizeUTF8(font_menu, "2048 pho riu", &surface_text->w, &surface_text->h);
        text_rect.x = 36;
        text_rect.w = surface_text->w;
        text_rect.h = surface_text->h;
        SDL_LogVerbose(0, "Target FPS: %d", DEFAULT_FPS);
        SDL_LogVerbose(0, "Begin main loop...");
        default_wait = 1000.0f / DEFAULT_FPS;
        max_wait = 1000 / DEFAULT_FPS + 1;
        request_redraw_fps = true;
        prev_tick = 1;
        fps_text = "FPS: 60";
        sf_text_fps = TTF_RenderUTF8_Blended(font_menu, fps_text.c_str(), WHITE_RGB);
        fps_tex = SDL_CreateTextureFromSurface(renderer, sf_text_fps);
        fps_tex_rect.y = 4;
    }
    void init()
    {
        SDL_LogVerbose(0, "MainMenu::init()");
    }
    void update()
    {
        SDL_LogVerbose(0, "MainMenu::update()");
    }
    void render()
    {
        SDL_LogVerbose(0, "MainMenu::render()");
        frame_start = SDL_GetTicks64();
        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT)
        {
            return;
        }
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, bg_texture, NULL, NULL);
        SDL_GetWindowSize(window, NULL, &text_rect.y);
        text_rect.y -= 110;
        SDL_RenderCopy(renderer, text, NULL, &text_rect);
        frame_end = SDL_GetTicks64();
        wait_time = default_wait - (frame_end - frame_start);
        // Render current FPS to screen
        auto time_elapsed = frame_end - prev_tick;
        SDL_LogVerbose(0, "FPS time: %lld", time_elapsed);
        // Render every second
        if (time_elapsed > 1000) {
            request_redraw_fps = true;
            prev_tick = frame_end;
        } else if (request_redraw_fps) {
            request_redraw_fps = false;
            fps_text = std::format("FPS: {}", round((double)1000 / time_elapsed));
            sf_text_fps = TTF_RenderUTF8_Blended(font_menu, fps_text.c_str(), WHITE_RGB);
            fps_tex = SDL_CreateTextureFromSurface(renderer, sf_text_fps);
            TTF_SizeUTF8(font_menu, fps_text.c_str(), &sf_text_fps->w, &sf_text_fps->h);
            SDL_GetWindowSizeInPixels(window, &fps_tex_rect.x, NULL);
            fps_tex_rect.w = sf_text_fps->w; // controls the width of the rect
            fps_tex_rect.h = sf_text_fps->h; // controls the height of the rect
            fps_tex_rect.x -= 300;
        }
        SDL_RenderCopy(renderer, fps_tex, NULL, &fps_tex_rect);
    }
    void destroy()
    {
        SDL_LogVerbose(0, "MainMenu::destroy()");
        SDL_DestroyTexture(bg_texture);
        SDL_DestroyTexture(text);
        SDL_FreeSurface(surface_text);
        TTF_CloseFont(font_menu);
    }
};
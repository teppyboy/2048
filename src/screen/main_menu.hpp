#include <functional>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "../element/fps_hud.hpp"
#include "screen.hpp"
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

public:
    MainMenu(SDL_Renderer *renderer, SDL_Window *window)
    {
        this->renderer = renderer;
        this->window = window;
        bg_texture = IMG_LoadTexture(renderer, "assets/img/bg.jpeg");
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
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
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
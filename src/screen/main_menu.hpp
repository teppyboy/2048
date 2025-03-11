#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "../element/fps_hud.hpp"
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
    FPSHUD *fps_hud;

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
        fps_hud = new FPSHUD(renderer, window);
    }
    int handle_event(SDL_Event event)
    {
        if (event.type == SDL_QUIT)
        {
            return 1;
        }
        return 0;
    }
    void render()
    {
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, bg_texture, NULL, NULL);
        SDL_GetWindowSize(window, NULL, &text_rect.y);
        text_rect.y -= 110;
        SDL_RenderCopy(renderer, text, NULL, &text_rect);
        fps_hud->render();
    }
    ~MainMenu()
    {
        SDL_DestroyTexture(bg_texture);
        SDL_DestroyTexture(text);
        SDL_FreeSurface(surface_text);
        TTF_CloseFont(font_menu);
        delete fps_hud;
    }
    void destroy()
    {
        delete this;
    }
};
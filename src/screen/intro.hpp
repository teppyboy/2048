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
    Screen *main_menu;
    Uint64 start_time;
    // Board
public:
    Intro(SDL_Renderer *renderer, SDL_Window *window, Screen *main_menu)
    {
        this->renderer = renderer;
        this->window = window;
        this->main_menu = main_menu;
        transition_set(this, main_menu, INTRO_DURATION, State::MAIN_MENU);
    }
    int handle_event(SDL_Event event)
    {
        return 0;
    }
    void render()
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
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
#include <functional>
#include <SDL.h>
#include <SDL_image.h>
#include "screen.hpp"
#include "saves.hpp"
#include "main_menu.hpp"
#include "game.hpp"
#include "../constants.hpp"
#include "../element/button.hpp"
#include "../state.hpp"

class Pause : public Screen
{
    SDL_Renderer *renderer;
    SDL_Window *window;
    SDL_Texture *pause_logo;
    SDL_Rect pause_logo_rect;
    // Button
    Button *play_again_button;
    Button *continue_playing_button;
    Button *main_menu_button;
    Button *save_button;
    Button *settings_button;
    // I'm sorry for my laziness
    SDL_Texture *game_screen_as_texture;
    SDL_Texture *our_screen_as_texture;
    // Screens
    Game *game;
    MainMenu *main_menu;
    Uint64 start_time;
    Saves *saves;

private:
    void render_animation_step_1()
    {
        game->render();
    }
    void render_animation_step_2()
    {
        auto opacity = (SDL_GetTicks64() - start_time - 50) * 150 / 150;
        if (opacity > 150)
        {
            opacity = 150;
        }
        SDL_SetRenderTarget(renderer, game_screen_as_texture);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);
        game->render();
        SDL_SetRenderTarget(renderer, NULL);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_SetTextureAlphaMod(game_screen_as_texture, 255 - opacity);
        SDL_RenderCopy(renderer, game_screen_as_texture, NULL, NULL);
    }
    void render_animation_step_3()
    {
        SDL_LogVerbose(0, "Rendering step 3.");
        auto opacity = (SDL_GetTicks64() - start_time - 50 - 150) * 255 / 150;
        if (opacity > 255)
        {
            opacity = 255;
        }
        SDL_SetRenderTarget(renderer, our_screen_as_texture);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);
        // Render everything to the game over screen
        // Set opacity
        SDL_SetTextureAlphaMod(pause_logo, opacity);
        SDL_RenderCopy(renderer, game_screen_as_texture, NULL, NULL);
        SDL_LogVerbose(0, "Rendering our screen with opacity %lld", opacity);
        SDL_RenderCopy(renderer, pause_logo, NULL, &pause_logo_rect);
        play_again_button->render();
        continue_playing_button->render();
        save_button->render();
        settings_button->render();
        main_menu_button->render();
        SDL_SetRenderTarget(renderer, NULL);
        SDL_RenderCopy(renderer, our_screen_as_texture, NULL, NULL);
    }
    void render_final()
    {
        SDL_RenderCopy(renderer, our_screen_as_texture, NULL, NULL);
        play_again_button->render();
        continue_playing_button->render();
        save_button->render();
        settings_button->render();
        main_menu_button->render();
    }
    void play_again_button_callback()
    {
        game->reset();
        transition_set(this, game, 1000, State::PAUSE);
    }
    void continue_playing_button_callback()
    {
        transition_set(this, game, 250, State::GAME);
    }
    void save_button_callback()
    {
        set_prev(this, State::PAUSE);
        transition_set(this, saves, 500, State::SAVES);
    }
    void main_menu_button_callback()
    {
        game->reset();
        transition_set(this, main_menu, 500, State::MAIN_MENU);
    }
    void settings_button_callback()
    {
        transition_set(this, game, 250, State::SETTINGS);
    }

public:
    Pause(SDL_Renderer *renderer, SDL_Window *window, Game *game, MainMenu *main_menu, Saves *saves)
    {
        this->renderer = renderer;
        this->window = window;
        this->game = game;
        this->main_menu = main_menu;
        this->saves = saves;
        start_time = 0;
        pause_logo = IMG_LoadTexture(renderer, "assets/img/paused.png");
        SDL_SetTextureBlendMode(pause_logo, SDL_BLENDMODE_BLEND);
        pause_logo_rect = {
            1280 / 2 - 512 / 2,
            -128,
            512,
            512,
        };
        continue_playing_button = new Button(renderer, window, "CONTINUE", 1280 / 2 - 300 / 2, 720 / 2 - 10 - 60 - 10, 300, 60, [&]()
                                             { continue_playing_button_callback(); });
        play_again_button = new Button(renderer, window, "PLAY AGAIN", 1280 / 2 - 300 / 2, 720 / 2 - 10, 300, 60, [&]()
                                       { play_again_button_callback(); });
        save_button = new Button(renderer, window, "LOAD / SAVE", 1280 / 2 - 300 / 2, 720 / 2 + 60, 300, 60, [&]()
                                 { save_button_callback(); });
        settings_button = new Button(renderer, window, "SETTINGS", 1280 / 2 - 300 / 2, 720 / 2 + 10 + 60 + 60, 300, 60, [&]()
                                     { settings_button_callback(); });
        main_menu_button = new Button(renderer, window, "MAIN MENU", 1280 / 2 - 300 / 2, 720 / 2 + 10 + 60 + 10 + 60 + 60, 300, 60, [&]()
                                      { main_menu_button_callback(); });
        game_screen_as_texture = SDL_CreateTexture(
            renderer,
            SDL_PIXELFORMAT_RGBA8888,
            SDL_TEXTUREACCESS_TARGET,
            DEFAULT_WIDTH, DEFAULT_HEIGHT);
        our_screen_as_texture = SDL_CreateTexture(
            renderer,
            SDL_PIXELFORMAT_RGBA8888,
            SDL_TEXTUREACCESS_TARGET,
            DEFAULT_WIDTH, DEFAULT_HEIGHT);
        SDL_SetTextureBlendMode(game_screen_as_texture, SDL_BLENDMODE_BLEND);
    }
    void start()
    {
        start_time = SDL_GetTicks64();
    }
    int handle_event(SDL_Event event)
    {
        play_again_button->handle_event(event);
        continue_playing_button->handle_event(event);
        save_button->handle_event(event);
        settings_button->handle_event(event);
        main_menu_button->handle_event(event);
        if (event.type == SDL_KEYDOWN)
        {
            // init_game_win = true;
            // game_state = State::WIN;
            switch (event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                continue_playing_button_callback();
                return 0;
            default:
                return 0;
            }
        }
        return 0;
    }
    void render()
    {
        if (init_game_pause)
        {
            start();
            init_game_pause = false;
        }
        auto time_elapsed = SDL_GetTicks64() - start_time;
        if (time_elapsed < 50)
        {
            game->render();
        }
        else if (time_elapsed < 50 + 150 + 50)
        {
            render_animation_step_2();
        }
        else if (time_elapsed < 50 + 150 + 150 + 50)
        {
            render_animation_step_3();
        }
        else
        {
            render_final();
        }
    }
    ~Pause()
    {
        SDL_DestroyTexture(pause_logo);
        SDL_DestroyTexture(game_screen_as_texture);
        SDL_DestroyTexture(our_screen_as_texture);
        delete play_again_button;
        delete continue_playing_button;
        delete save_button;
        SDL_DestroyTexture(pause_logo);
    }
    void destroy()
    {
        delete this;
    }
};
#include <functional>
#include <SDL.h>
#include <SDL_image.h>
#include "screen.hpp"
#include "game.hpp"
#include "saves.hpp"
#include "../constants.hpp"
#include "../element/button.hpp"
#include "../state.hpp"

class Win : public Screen
{
    SDL_Renderer *renderer;
    SDL_Window *window;
    SDL_Texture *game_over_logo;
    SDL_Rect game_over_logo_rect;
    // Button
    Button *play_again_button;
    Button *continue_playing_button;
    Button *save_button;
    // I'm sorry for my laziness
    bool first_final;
    SDL_Texture *game_screen_as_texture;
    SDL_Texture *our_screen_as_texture;
    // Screens
    Game *game;
    GameOver *game_over;
    Uint64 start_time;
    Saves *saves;

private:
    void render_animation_step_1()
    {
        game->render();
    }
    void render_animation_step_2()
    {
        auto opacity = (SDL_GetTicks64() - start_time - 500) * 150 / 750;
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
        auto opacity = (SDL_GetTicks64() - start_time - 500 - 750) * 255 / 350;
        if (opacity > 255)
        {
            opacity = 255;
        }
        SDL_SetRenderTarget(renderer, our_screen_as_texture);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);
        // Render everything to the game over screen
        // Set opacity
        SDL_SetTextureAlphaMod(game_over_logo, opacity);
        SDL_SetTextureAlphaMod(game_over->score_text_texture, 255);
        SDL_SetTextureAlphaMod(game_over->best_score_text_texture, 255);
        SDL_RenderCopy(renderer, game_screen_as_texture, NULL, NULL);
        SDL_LogVerbose(0, "Rendering our screen with opacity %lld", opacity);
        SDL_RenderCopy(renderer, game_over_logo, NULL, &game_over_logo_rect);
        game_over->render_scoreboard();
        play_again_button->render();
        continue_playing_button->render();
        save_button->render();
        SDL_SetRenderTarget(renderer, NULL);
        SDL_RenderCopy(renderer, our_screen_as_texture, NULL, NULL);
    }
    void render_final()
    {
        SDL_RenderCopy(renderer, our_screen_as_texture, NULL, NULL);
        play_again_button->render();
        continue_playing_button->render();
        save_button->render();
    }
    void play_again_button_callback()
    {
        game->reset();
        transition_set(this, game, 1000, State::GAME);
    }
    void continue_playing_button_callback()
    {
        game->continue_playing_after_win = true;
        transition_set(this, game, 500, State::GAME);
    }
    void save_button_callback()
    {
        set_prev(this, State::WIN);
        transition_set(this, saves, 500, State::SAVES);
    }

public:
    Win(SDL_Renderer *renderer, SDL_Window *window, Game *game, GameOver *game_over, Saves *saves)
    {
        this->renderer = renderer;
        this->window = window;
        this->game = game;
        this->game_over = game_over;
        this->saves = saves;
        first_final = true;
        start_time = 0;
        game_over_logo = IMG_LoadTexture(renderer, "assets/img/win.png");
        SDL_SetTextureBlendMode(game_over_logo, SDL_BLENDMODE_BLEND);
        game_over_logo_rect = {
            1280 / 2 - 512 / 2,
            -128,
            512,
            512,
        };
        game_over->init_scoreboard();
        continue_playing_button = new Button(renderer, window, "CONTINUE", 1280 / 2 - 300 / 2, 640, 300, 60, [&]()
                                             { continue_playing_button_callback(); });
        play_again_button = new Button(renderer, window, "PLAY AGAIN", 1280 / 2 - 300 / 2 + 320, 640, 300, 60, [&]()
                                       { play_again_button_callback(); });
        save_button = new Button(renderer, window, "LOAD / SAVE", 1280 / 2 - 300 / 2 - 320, 640, 300, 60, [&]()
                                 { save_button_callback(); });
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
        return 0;
    }
    void render()
    {
        if (init_game_win)
        {
            start();
            init_game_win = false;
        }
        auto time_elapsed = SDL_GetTicks64() - start_time;
        if (time_elapsed < 500)
        {
            game->render();
        }
        else if (time_elapsed < 500 + 750 + 50)
        {
            render_animation_step_2();
        }
        else if (time_elapsed < 500 + 750 + 50 + 350)
        {
            render_animation_step_3();
        }
        else
        {
            Uint8 opacity;
            SDL_GetTextureAlphaMod(game_screen_as_texture, &opacity);
            SDL_LogVerbose(0, "Back screen opacity: %d", opacity);
            render_final();
        }
    }
    ~Win()
    {
        SDL_DestroyTexture(game_over_logo);
        SDL_DestroyTexture(game_screen_as_texture);
        SDL_DestroyTexture(our_screen_as_texture);
        delete play_again_button;
        delete continue_playing_button;
        delete save_button;
        SDL_DestroyTexture(game_over_logo);
    }
    void destroy()
    {
        delete this;
    }
};
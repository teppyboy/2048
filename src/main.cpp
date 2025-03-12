#include <format>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "constants.hpp"
#include "assets.hpp"
#include "state.hpp"
#include "element/fps_hud.hpp"
#include "screen/main_menu.hpp"
#include "screen/game.hpp"
#include "screen/intro.hpp"
#include "screen/transition.hpp"

/**
 * Initializes SDL2 and its subsystems.
 *
 * @return `true` if initialization was successful, `false` otherwise.
 */
bool init_sdl()
{
    SDL_LogVerbose(0, "Initializing SDL application '%s'...", WINDOW_NAME);
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
        return false;
    }
    if (IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) != (IMG_INIT_JPG | IMG_INIT_PNG))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL_image: %s", IMG_GetError());
        return false;
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize Mix_OpenAudio: %s", Mix_GetError());
        SDL_Quit();
        return false;
    }
    if (TTF_Init())
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize TTF: %s", TTF_GetError());
        return false;
    }
    return true;
}

/**
 * Cleanup SDL2 and its subsystems.
 *
 */
void cleanup_sdl()
{
    SDL_LogVerbose(0, "Cleaning up SDL application '%s'...", WINDOW_NAME);
    TTF_Quit();
    Mix_Quit();
    IMG_Quit();
    SDL_Quit();
}

void play_bgm()
{
    Mix_Music *sound = Mix_LoadMUS("assets/audio/bg.mp3");
    if (!sound)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Mix_LoadWAV failed: %s", Mix_GetError());
        Mix_CloseAudio();
    }
    if (Mix_PlayMusic(sound, 0))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Mix_PlayChannel failed: %s", Mix_GetError());
    }
}

int main(int argc, char *argv[])
{
#ifdef DEBUG
    SDL_LogSetAllPriority(SDL_LOG_PRIORITY_VERBOSE);
    SDL_LogWarn(0, "This is a debug build and should not be used in production.");
#endif
    // In Windows we prioritize Direct3D first
    // then for all other platforms we prioritize Vulkan and finally fallback to SDL2's default
    SDL_LogVerbose(0, "Available render drivers:");
    int render_driver_index = -1;
    for (int i = 0; i < SDL_GetNumRenderDrivers(); i++)
    {
        SDL_RendererInfo renderer_info = {};
        SDL_GetRenderDriverInfo(i, &renderer_info);
        SDL_LogVerbose(0, "Renderer %d: %s", i, renderer_info.name);
        auto render_name = std::string(renderer_info.name);
#ifdef _WIN32
        if (render_name.find("direct3d") != std::string::npos)
        {
            render_driver_index = i;
        }
#endif
        if (render_name.find("vulkan") != std::string::npos && render_driver_index == -1)
        {
            render_driver_index = i;
        }
    }
    SDL_Window *window = SDL_CreateWindow(WINDOW_NAME, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, DEFAULT_WIDTH, DEFAULT_HEIGHT, SDL_WINDOW_HIDDEN | SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, render_driver_index, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_Surface *surface;
    SDL_Event event;

    if (!init_sdl())
    {
        return 1;
    }
    load_assets(renderer);
    SDL_SetWindowTitle(window, WINDOW_NAME);
    auto game = new Game(renderer, window);
    auto main_menu = new MainMenu(renderer, window, game);
    auto intro = new Intro(renderer, window, main_menu);
    auto fps_hud = new FPSHUD(renderer, window);
    Transition *transition = nullptr;
    SDL_ShowWindow(window);
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
    while (true)
    {
        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT)
        {
            return 0;
        }
        SDL_RenderClear(renderer);
        if (transition_request)
        {
            SDL_LogVerbose(0, "Transitioning from %d to %d", (int)game_state, (int)transition_to);
            if (transition == nullptr)
            {
                SDL_LogVerbose(0, "Creating new transition with duration %d", transition_duration);
                transition = new Transition(renderer, window, transition_src_screen, transition_dst_screen, transition_duration);
            }
            transition->render();
        }
        else
        {
            if (transition != nullptr)
            {
                SDL_LogVerbose(0, "Destroying transition");
                transition->destroy();
                transition = nullptr;
            }
            switch (game_state)
            {
            case State::INTRO:
                intro->render();
                break;
            case State::MAIN_MENU:
                main_menu->handle_event(event);
                main_menu->render();
                break;
            case State::GAME_STARTING:
                game->handle_event(event);
                game->render();
                break;
            case State::GAME:
                game->handle_event(event);
                game->render();
                break;
            case State::GAME_OVER:
                break;
            case State::PAUSE:
                break;
            case State::SETTINGS:
                break;
            case State::SHOW_MESSAGEBOX:
                break;
            }
        }
        if (fps_hud_enabled)
        {
            fps_hud->render();
        }
        SDL_RenderPresent(renderer);
    }

    // Cleanup
    delete main_menu;
    delete fps_hud;
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    cleanup_sdl();

    return 0;
}
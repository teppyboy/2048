#include <format>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "constants.hpp"
#include "state.hpp"
#include "screen/main_menu.hpp"

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
        return 1;
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
    SDL_Window *window = SDL_CreateWindow(WINDOW_NAME, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, DEFAULT_WIDTH, DEFAULT_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_Surface *surface;
    SDL_Event event;

    if (!init_sdl())
    {
        return 1;
    }

    SDL_SetWindowTitle(window, WINDOW_NAME);

    auto main_menu = MainMenu(renderer, window);
    while (true)
    {
        main_menu.render();
        SDL_RenderPresent(renderer);
    }

    // Cleanup
    main_menu.destroy();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    cleanup_sdl();

    return 0;
}
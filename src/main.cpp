#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <format>

#define DEFAULT_WIDTH 1280
#define DEFAULT_HEIGHT 720
#define DEFAULT_FPS 60
#define WINDOW_NAME "SDL2 Template"

/**
 * Initializes SDL2 and its subsystems.
 *
 * @return `true` if initialization was successful, `false` otherwise.
 */
bool init_sdl()
{
    SDL_LogVerbose(0, "Initializing SDL application '%s'...", WINDOW_NAME);
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
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

    // Background thingy
    SDL_Texture *bg_texture = IMG_LoadTexture(renderer, "assets/img/bg.jpeg");
    // Watermark
    SDL_Color text_color = {255, 255, 255, 0};
    TTF_Font *font_shizuru = TTF_OpenFont("assets/fonts/Shizuru-Regular.ttf", 64);
    SDL_Surface *surface_text = TTF_RenderUTF8_Blended(font_shizuru, "SDL2 Template", text_color);
    SDL_Texture *text = SDL_CreateTextureFromSurface(renderer, surface_text);
    TTF_SizeUTF8(font_shizuru, "SDL2 Template", &surface_text->w, &surface_text->h);
    SDL_Rect text_rect;            // create a rect
    text_rect.x = 36;              // controls the rect's x coordinate
    text_rect.w = surface_text->w; // controls the width of the rect
    text_rect.h = surface_text->h; // controls the height of the rect

    // Play audio
    play_bgm();

    SDL_LogVerbose(0, "Target FPS: %d", DEFAULT_FPS);
    SDL_LogVerbose(0, "Begin main loop...");
    double default_wait = 1000.0f / DEFAULT_FPS;
    Uint32 max_wait = 1000 / DEFAULT_FPS + 1;
    // FPS Text
    bool request_redraw_fps = true;
    Uint64 prev_tick = 1;
    std::string fps_text = "FPS: 60";
    SDL_Surface *sf_text_fps;
    SDL_Texture *fps_tex;
    SDL_Rect fps_tex_rect;            // create a rect
    fps_tex_rect.y = 4;              // controls the rect's x coordinate
    while (true)
    {
        // Frame start for FPS limiter
        Uint64 frame_start = SDL_GetTicks64();
        // Event handling as normal
        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT)
        {
            break;
        }
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
        SDL_RenderClear(renderer);
        // Show background
        SDL_RenderCopy(renderer, bg_texture, NULL, NULL);
        // Show font
        SDL_GetWindowSizeInPixels(window, NULL, &text_rect.y);
        text_rect.y -= 110;
        SDL_RenderCopy(renderer, text, NULL, &text_rect);
        
        // Limit FPS
        Uint64 frame_end = SDL_GetTicks64();
        double wait_time = default_wait - (frame_end - frame_start);
        if (wait_time > max_wait)
        {
            wait_time = max_wait;
        }
        if (wait_time > 0)
        {
            SDL_LogVerbose(0, "Wait time: %f", wait_time);
            // SDL_LogVerbose(0, "Diff: %d", current_tick - last_tick);
            SDL_Delay((Uint32)(wait_time));
        }
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
            sf_text_fps = TTF_RenderUTF8_Blended(font_shizuru, fps_text.c_str(), text_color);
            fps_tex = SDL_CreateTextureFromSurface(renderer, sf_text_fps);
            TTF_SizeUTF8(font_shizuru, fps_text.c_str(), &sf_text_fps->w, &sf_text_fps->h);
            SDL_GetWindowSizeInPixels(window, &fps_tex_rect.x, NULL);
            fps_tex_rect.w = sf_text_fps->w; // controls the width of the rect
            fps_tex_rect.h = sf_text_fps->h; // controls the height of the rect
            fps_tex_rect.x -= 300;
        }
        SDL_RenderCopy(renderer, fps_tex, NULL, &fps_tex_rect);
        // Update screen
        SDL_RenderPresent(renderer);
    }

    // Cleanup
    // Background
    SDL_DestroyTexture(bg_texture);
    // Font
    SDL_FreeSurface(surface_text);
    SDL_DestroyTexture(text);
    TTF_CloseFont(font_shizuru);
    // Everything else
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    cleanup_sdl();

    return 0;
}
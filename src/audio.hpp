#include <SDL_mixer.h>
#include <SDL.h>
#include "assets.hpp"

void play_bgm()
{
    Mix_PlayMusic(BGM, -1);
}

void set_bgm_volume(int volume) {
    auto actual_volume = volume * 128 * 80 / 100 / 100;
    SDL_LogVerbose(0, "Setting BGM volume to %d", actual_volume);
    Mix_VolumeMusic(actual_volume);
}

void set_sfx_volume(int volume) {
    auto actual_volume = volume * 128 / 100;
    Mix_VolumeChunk(SWIPE_SFX, actual_volume);
}
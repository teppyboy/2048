// This file is for shared state across the game, for user settings it's stored in 'settings.hpp'
#pragma once 

#include "constants.hpp"
#include "screen/screen.hpp"
State game_state = State::INTRO;
bool init_game_over = false;
bool init_game_win = false;
bool init_game_pause = false;
int game_best_score = 0;
int game_score = 0;

// Transition mess
bool transition_request;
Screen *transition_src_screen;
Screen *transition_dst_screen;
int transition_duration;
State transition_to;
void transition_reset() {
    transition_request = false;
    transition_src_screen = nullptr;
    transition_dst_screen = nullptr;
    transition_duration = 0;
    transition_to = State::NOTHING;
}
void transition_set(Screen *src, Screen *dst, int duration, State end_state) {
    transition_src_screen = src;
    transition_dst_screen = dst;
    transition_duration = duration;
    transition_to = end_state;
    transition_request = true;
}
#ifdef DEBUG
bool fps_hud_enabled = true;
#else
bool fps_hud_enabled = false;
#endif
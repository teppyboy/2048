// This file is for shared state across the game, for user settings it's stored in 'settings.hpp'
#pragma once 

#include "constants.hpp"
#include "screen/screen.hpp"
#include "struct.hpp"

State game_state = State::INTRO;
Settings settings = Settings::load();
UserData user_data = UserData::load();
bool init_game_over = false;
bool init_game_win = false;
bool init_game_pause = false;

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

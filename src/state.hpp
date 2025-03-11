// This file is for shared state across the game, for user settings it's stored in 'settings.hpp'
#pragma once 

#include "constants.hpp"
#include "screen/screen.hpp"
State game_state = State::INTRO;

// Transition mess
bool request_transition = false;
Screen *src_screen = nullptr;
Screen *dst_screen = nullptr;
int transition_duration = 2000;
State transition_to = State::MAIN_MENU;

#ifdef DEBUG
bool fps_hud_enabled = true;
#else
bool fps_hud_enabled = false;
#endif
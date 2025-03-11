#pragma once
// This should not be called "Screen" but I can't think of a better name.
// This is the base class for all screens and elements in the game.
class Screen
{
public:
    virtual int handle_event(SDL_Event event) = 0;
    virtual void render() = 0;
    virtual void destroy() = 0;
    virtual ~Screen() = default;
};
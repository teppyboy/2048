#pragma once
class Screen
{
public:
    virtual int handle_event(SDL_Event event) = 0;
    virtual void render() = 0;
    virtual void destroy() = 0;
    virtual ~Screen() = default;
};
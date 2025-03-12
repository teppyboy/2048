#pragma once

#include <functional>
#include <SDL.h>
#include <SDL_image.h>
#include "screen.hpp"
#include <src/element/button.hpp>
#include <src/game/board.hpp>

class Game : public Screen
{
    SDL_Renderer *renderer;
    SDL_Window *window;
    SDL_Texture *screen_as_texture;
    // Background
    SDL_Texture *bg_texture;
    // Board
    SDL_Rect board_rect;
    Board board;
    int tile_size;
    bool board_changed;
    Uint64 animation_start_time;
    std::pair<int, int> last_new_tile_pos;
private:
    void _render() {
        SDL_RenderCopy(renderer, bg_texture, NULL, NULL);
        SDL_RenderCopy(renderer, BOARD_TEXTURE, NULL, &board_rect);
        for (int i = 0; i < board.size; i++)
        {
            for (int j = 0; j < board.size; j++)
            {
                if (board.grid[i][j] == 0)
                {
                    continue;
                }
                if (last_new_tile_pos.first == i && last_new_tile_pos.second == j)
                {
                    continue;
                }
                render_tile(i, j, board.grid[i][j]);
            }
        }
    }
public:
    Game(SDL_Renderer *renderer, SDL_Window *window)
    {
        this->renderer = renderer;
        this->window = window;
        // Board has a size of 2048x2048
        // Tile has a size of 437x437
        bg_texture = IMG_LoadTexture(renderer, "assets/img/bg_game.png");
        board_rect.x = 1280 / 2 - 512 / 2;
        board_rect.y = 720 / 2 - 512 / 2;
        board_rect.w = 512;
        board_rect.h = 512;
        tile_size = 437 / 4;
        this->last_new_tile_pos = {-1, -1};
        this->animation_start_time = 0;
        board = Board(4);
        board_changed = true; 
        screen_as_texture = SDL_CreateTexture(
            renderer,
            SDL_PIXELFORMAT_RGBA8888,
            SDL_TEXTUREACCESS_TARGET,
            DEFAULT_WIDTH, DEFAULT_HEIGHT
        );
        SDL_SetTextureBlendMode(screen_as_texture, SDL_BLENDMODE_BLEND);
    }
    int handle_event(SDL_Event event)
    {
        if (event.type == SDL_KEYDOWN) {
            auto old_grid = board.grid;
            switch (event.key.keysym.sym) {
                case SDLK_w:
                    board_changed = board.move(0); // Up
                    break;
                case SDLK_d:
                    board_changed = board.move(1); // Right
                    break;
                case SDLK_s:
                    board_changed = board.move(2); // Down
                    break;
                case SDLK_a:
                    board_changed = board.move(3); // Left
                    break;
                default:
                    return 0;
            }
            if (board_changed) {
                last_new_tile_pos = board.add_tile();
                animation_start_time = 0;
            }
        }
        return 0;
    }
    void render_tile_free(int x, int y, int value, double scale = 1) {
        // Render the tile
        SDL_LogVerbose(0, "Rendering tile at %d, %d with value %d scale %f", x, y, value, scale);
        int index = TILE_VALUE_INDEX_MAP[value];
        // + 60 because of the border
        SDL_Rect tile_rect = {x, y, (int)((double)tile_size * scale), (int)((double)tile_size * scale)};
        SDL_LogVerbose(0, "Tile rect: %d, %d, %d, %d", tile_rect.x, tile_rect.y, tile_rect.w, tile_rect.h);
        SDL_LogVerbose(0, "Tile texture: %d", index);
        auto text_surface = TILE_TEXT_SURFACES[index];
        auto text_texture = TILE_TEXT_TEXTURES[index];
        SDL_RenderCopy(renderer, TILE_TEXTURES[index], NULL, &tile_rect);
        if (scale == 1) {
            SDL_Rect text_rect = {
                (tile_rect.x + tile_rect.w / 2 - text_surface->w / 2),
                (tile_rect.y + tile_rect.h / 2 - text_surface->h / 2),
                (int)((double)(text_surface->w) * scale),
                (int)((double)(text_surface->h) * scale),
            };
            SDL_RenderCopy(renderer, text_texture, NULL, &text_rect);
        }
    }
    void render_tile(int x, int y, int value, double scale = 1)
    {
        render_tile_free(board_rect.x + 60 / 4 + 60 / 4 * x + x * tile_size, board_rect.y + 60 / 4 + 60 / 4 * y + y * tile_size, value, scale);
    }
    void render_new_tile() {
        SDL_SetRenderTarget(renderer, screen_as_texture);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        if (animation_start_time == 0) {
            animation_start_time = SDL_GetTicks64();
        }
        auto i = last_new_tile_pos.first;
        auto j = last_new_tile_pos.second;
        auto time_elapsed = SDL_GetTicks64() - animation_start_time;
        auto scale = (double)time_elapsed / 100;
        SDL_LogVerbose(0, "Time elapsed: %lld", time_elapsed);
        SDL_LogVerbose(0, "Scale: %f", scale);
        if (time_elapsed > 100)
        {
            scale = 1;
            last_new_tile_pos = {-1, -1};
            animation_start_time = 0;
        }
        render_tile(i, j, board.grid[i][j], scale);
        SDL_SetRenderTarget(renderer, NULL);
    }
    void render()
    {
        if (board_changed) {
            SDL_SetRenderTarget(renderer, screen_as_texture);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
            SDL_RenderClear(renderer);
            _render();
            SDL_SetRenderTarget(renderer, NULL);
            board_changed = false;
        }
        if (last_new_tile_pos.first != -1 && last_new_tile_pos.second != -1) {
            render_new_tile();
        }
        SDL_RenderCopy(renderer, screen_as_texture, NULL, NULL);
    }
    ~Game()
    {
        return;
    }
    void destroy()
    {
        delete this;
    }
};
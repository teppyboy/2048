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
    int tile_size;
    Board::MoveResult move_result;
    Uint64 animation_start_time;
    Uint64 current_tick;
    std::pair<int, int> last_new_tile_pos;
    bool is_game_over;
    // Score
    SDL_Rect score_rect;
    SDL_Rect score_text_rect;
    SDL_Texture *score_text_texture;
    SDL_Rect best_score_rect;
    SDL_Rect best_score_text_rect;
    SDL_Texture *best_score_text_texture;

private:
    void _render()
    {
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
                if (move_result.moved)
                {
                    for (const auto &move : move_result.moved_tiles)
                    {
                        if (move.to_row == i && move.to_col == j)
                        {
                            goto end_render;
                        }
                    }
                    for (const auto &move : move_result.merged_tiles)
                    {
                        if (move.to_row == i && move.to_col == j)
                        {
                            goto end_render;
                        }
                    }
                }
                render_tile(i, j, board.grid[i][j]);
            end_render:;
            }
        }
    }

public:
    Board board;
    Game(SDL_Renderer *renderer, SDL_Window *window)
    {
        this->renderer = renderer;
        this->window = window;
        // Board has a size of 2048x2048
        // Tile has a size of 437x437
        bg_texture = IMG_LoadTexture(renderer, "assets/img/bg_game.png");
        board_rect = {
            1280 / 2 - 512 / 2,
            720 / 2 - 512 / 2,
            512,
            512,
        };
        tile_size = 437 / 4;
        this->last_new_tile_pos = {-1, -1};
        this->animation_start_time = 0;
        this->current_tick = SDL_GetTicks64();
        move_result = Board::MoveResult();
        move_result.moved = true;
        board = Board(4);
        is_game_over = false;
        screen_as_texture = SDL_CreateTexture(
            renderer,
            SDL_PIXELFORMAT_RGBA8888,
            SDL_TEXTUREACCESS_TARGET,
            DEFAULT_WIDTH, DEFAULT_HEIGHT);
        SDL_SetTextureBlendMode(screen_as_texture, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        // Score
        score_rect = {
            (1280 / 2 - 512 / 2) / 2 - 220 / 2,
            720 / 2 - 94,
            220,
            80,
        };
        SDL_Surface *text_surface = TTF_RenderUTF8_Blended(UI_FONT_24, "SCORE", TILE_TEXT_DARK_RGB);
        score_text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
        TTF_SizeUTF8(UI_FONT_24, "SCORE", &text_surface->w, &text_surface->h);
        score_text_rect = {
            score_rect.x + score_rect.w / 2 - text_surface->w / 2,
            score_rect.y + 6,
            text_surface->w,
            text_surface->h,
        };
        // Best score
        best_score_rect = {
            (1280 / 2 - 512 / 2) / 2 - 220 / 2,
            720 / 2 + 14,
            220,
            80,
        };
        text_surface = TTF_RenderUTF8_Blended(UI_FONT_24, "BEST", TILE_TEXT_DARK_RGB);
        best_score_text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
        TTF_SizeUTF8(UI_FONT_24, "BEST", &text_surface->w, &text_surface->h);
        best_score_text_rect = {
            best_score_rect.x + best_score_rect.w / 2 - text_surface->w / 2,
            best_score_rect.y + 6,
            text_surface->w,
            text_surface->h,
        };
    }
    void reset()
    {
        board.reset();
        is_game_over = false;
        last_new_tile_pos = {-1, -1};
        animation_start_time = 0;
        current_tick = SDL_GetTicks64();
        move_result = Board::MoveResult();
        move_result.moved = true;
    }
    int handle_event(SDL_Event event)
    {
        if (is_game_over)
        {
            return 0;
        }
        auto time_elapsed = current_tick - animation_start_time;
        if (time_elapsed < 100)
        {
            return 0;
        }
        if (event.type == SDL_KEYDOWN)
        {
            init_game_over = true;
            game_state = State::GAME_OVER;
            auto old_grid = board.grid;
            switch (event.key.keysym.sym)
            {
            case SDLK_w:
                move_result = board.move(0); // Up
                break;
            case SDLK_d:
                move_result = board.move(1); // Right
                break;
            case SDLK_s:
                move_result = board.move(2); // Down
                break;
            case SDLK_a:
                move_result = board.move(3); // Left
                break;
            default:
                return 0;
            }
            if (move_result.moved)
            {
                for (const auto &move : move_result.moved_tiles)
                {
                    SDL_LogVerbose(0, "Tile %d moved from (%d,%d) to (%d,%d)\n",
                                   move.value, move.from_row, move.from_col, move.to_row, move.to_col);
                }
                for (const auto &merge : move_result.merged_tiles)
                {
                    SDL_LogVerbose(0, "Tiles at (%d,%d) and (%d,%d) merged at (%d,%d) to form %d\n",
                                   merge.from_row_1, merge.from_col_1, merge.from_row_2, merge.from_col_2,
                                   merge.to_row, merge.to_col, merge.valueResult);
                }
                last_new_tile_pos = board.add_tile();
                animation_start_time = current_tick;
            }
            else
            {
                is_game_over = board.is_game_over();
                if (is_game_over)
                {
                    SDL_LogVerbose(0, "Game over.");
                    game_score = board.score;
                    init_game_over = true;
                    game_state = State::GAME_OVER;
                }
            }
        }
        return 0;
    }
    /// @brief Renders the tile at the specified position on the screen
    /// @param x
    /// @param y
    /// @param value
    /// @param scale
    void render_tile_free(int x, int y, int value, double scale = 1)
    {
        // Render the tile
        SDL_LogVerbose(0, "Rendering tile at %d, %d with value %d scale %f", x, y, value, scale);
        int index = TILE_VALUE_INDEX_MAP[value];
        auto it = TILE_VALUE_INDEX_MAP.find(value);
        bool free_needed = false;
        SDL_Surface *text_surface;
        SDL_Texture *text_texture;
        SDL_Texture *tile_texture;
        if (it != TILE_VALUE_INDEX_MAP.end())
        {
            int index = it->second;
            text_surface = TILE_TEXT_SURFACES[index];
            text_texture = TILE_TEXT_TEXTURES[index];
            tile_texture = TILE_TEXTURES[index];
        }
        else
        {
            free_needed = true;
            text_surface = TTF_RenderUTF8_Blended(UI_FONT_BOLD_32, std::to_string(value).c_str(), TILE_TEXT_DARK_RGB);
            text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
            tile_texture = TILE_BIG_TEXTURE;
            TILE_TEXT_SURFACES.push_back(text_surface);
            TILE_TEXT_TEXTURES.push_back(text_texture);
            TILE_TEXTURES.push_back(tile_texture);
            TILE_VALUE_INDEX_MAP.emplace(value, TILE_TEXT_TEXTURES.size() - 1);
        }
        auto size = (int)((double)tile_size * scale) + (int)((double)(tile_size / 2) * ((double)1 - scale));
        SDL_Rect tile_rect = {x, y, size, size};
        SDL_LogVerbose(0, "Tile rect: %d, %d, %d, %d", tile_rect.x, tile_rect.y, tile_rect.w, tile_rect.h);
        SDL_LogVerbose(0, "Tile texture: %d", index);
        SDL_RenderCopy(renderer, tile_texture, NULL, &tile_rect);
        if (scale == 1)
        {
            SDL_Rect text_rect = {
                (tile_rect.x + tile_rect.w / 2 - text_surface->w / 2),
                (tile_rect.y + tile_rect.h / 2 - text_surface->h / 2),
                (int)((double)(text_surface->w) * scale),
                (int)((double)(text_surface->h) * scale),
            };
            SDL_RenderCopy(renderer, text_texture, NULL, &text_rect);
        }
    }
    /// @brief Renders the tile at the position of the board
    /// @param x
    /// @param y
    /// @param value
    /// @param scale
    void render_tile(int x, int y, int value, double scale = 1)
    {
        // + 60 because of the border
        render_tile_free(board_rect.x + 60 / 4 + 60 / 4 * x + x * tile_size, board_rect.y + 60 / 4 + 60 / 4 * y + y * tile_size, value, scale);
    }
    void animation_render_new_tile()
    {
        auto i = last_new_tile_pos.first;
        auto j = last_new_tile_pos.second;
        auto time_elapsed = current_tick - animation_start_time;
        auto scale = (double)time_elapsed / 100;
        SDL_LogVerbose(0, "Time elapsed: %lld", time_elapsed);
        SDL_LogVerbose(0, "Scale: %f", scale);
        if (time_elapsed > 100)
        {
            SDL_SetRenderTarget(renderer, screen_as_texture);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
            scale = 1;
            last_new_tile_pos = {-1, -1};
        }
        render_tile(i, j, board.grid[i][j], scale);
        if (time_elapsed > 100)
        {
            SDL_SetRenderTarget(renderer, NULL);
            SDL_RenderCopy(renderer, screen_as_texture, NULL, NULL);
        }
    }
    void animation_render_moved_tiles()
    {
        auto time_elapsed = current_tick - animation_start_time;
        auto position = (double)time_elapsed / 100;
        SDL_LogVerbose(0, "Time elapsed: %lld", time_elapsed);
        SDL_LogVerbose(0, "Position in animation: %f", position);
        if (time_elapsed > 100)
        {
            SDL_SetRenderTarget(renderer, screen_as_texture);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        }
        for (const auto &move : move_result.moved_tiles)
        {
            auto from_x = move.from_row;
            auto from_y = move.from_col;
            auto to_x = move.to_row;
            auto to_y = move.to_col;
            if (time_elapsed > 100)
            {
                render_tile(to_x, to_y, move.value);
            }
            else
            {
                auto diff_x_calculated = (to_x - from_x) * (tile_size + 60 / 4);
                auto diff_y_calculated = (to_y - from_y) * (tile_size + 60 / 4);
                auto calculated_x = board_rect.x + 60 / 4 + 60 / 4 * from_x + from_x * tile_size + (int)((double)diff_x_calculated * position);
                auto calculated_y = board_rect.y + 60 / 4 + 60 / 4 * from_y + from_y * tile_size + (int)((double)diff_y_calculated * position);
                render_tile_free(calculated_x, calculated_y, move.value);
            }
        }
        if (time_elapsed > 100)
        {
            SDL_SetRenderTarget(renderer, NULL);
            SDL_RenderCopy(renderer, screen_as_texture, NULL, NULL);
            move_result.moved_tiles.clear();
        }
    }
    void animation_render_merged_tiles()
    {
        auto time_elapsed = current_tick - animation_start_time;
        auto position = (double)time_elapsed / 60;
        SDL_LogVerbose(0, "Time elapsed: %lld", time_elapsed);
        SDL_LogVerbose(0, "Position in animation: %f", position);
        if (time_elapsed > 100)
        {
            SDL_SetRenderTarget(renderer, screen_as_texture);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        }
        for (const auto &merge : move_result.merged_tiles)
        {
            auto from_x_1 = merge.from_row_1;
            auto from_y_1 = merge.from_col_1;
            auto from_x_2 = merge.from_row_2;
            auto from_y_2 = merge.from_col_2;
            auto to_x = merge.to_row;
            auto to_y = merge.to_col;
            if (time_elapsed > 100)
            {
                render_tile(to_x, to_y, merge.valueResult);
            }
            else if (time_elapsed > 60)
            {
                auto scale = (double)time_elapsed / 100;
                render_tile(to_x, to_y, merge.valueResult, scale);
            }
            else
            {
                auto diff_x_1_calculated = (to_x - from_x_1) * (tile_size + 60 / 4);
                auto diff_y_1_calculated = (to_y - from_y_1) * (tile_size + 60 / 4);
                auto calculated_x_1 = board_rect.x + 60 / 4 + 60 / 4 * from_x_1 + from_x_1 * tile_size + (int)((double)diff_x_1_calculated * position);
                auto calculated_y_1 = board_rect.y + 60 / 4 + 60 / 4 * from_y_1 + from_y_1 * tile_size + (int)((double)diff_y_1_calculated * position);
                auto diff_x_2_calculated = (to_x - from_x_2) * (tile_size + 60 / 4);
                auto diff_y_2_calculated = (to_y - from_y_2) * (tile_size + 60 / 4);
                auto calculated_x_2 = board_rect.x + 60 / 4 + 60 / 4 * from_x_2 + from_x_2 * tile_size + (int)((double)diff_x_2_calculated * position);
                auto calculated_y_2 = board_rect.y + 60 / 4 + 60 / 4 * from_y_2 + from_y_2 * tile_size + (int)((double)diff_y_2_calculated * position);
                auto value = merge.valueResult / 2;
                render_tile_free(calculated_x_1, calculated_y_1, value);
                render_tile_free(calculated_x_2, calculated_y_2, value);
            }
        }
        if (time_elapsed > 100)
        {
            SDL_SetRenderTarget(renderer, NULL);
            SDL_RenderCopy(renderer, screen_as_texture, NULL, NULL);
            move_result.merged_tiles.clear();
        }
    }
    void render_scoreboard()
    {
        // The rectangle
        // Score
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 30);
        SDL_RenderFillRect(renderer, &score_rect);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 100);
        SDL_RenderDrawRect(renderer, &score_rect);
        // Best score
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 30);
        SDL_RenderFillRect(renderer, &best_score_rect);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 100);
        SDL_RenderDrawRect(renderer, &best_score_rect);
        // The "SCORE" text
        SDL_RenderCopy(renderer, score_text_texture, NULL, &score_text_rect);
        // The "BEST" text
        SDL_RenderCopy(renderer, best_score_text_texture, NULL, &best_score_text_rect);
        // The score
        SDL_Surface *score_surface = TTF_RenderUTF8_Blended(UI_FONT_BOLD_32, std::to_string(board.score).c_str(), TILE_TEXT_DARK_RGB);
        SDL_Texture *score_texture = SDL_CreateTextureFromSurface(renderer, score_surface);
        TTF_SizeUTF8(UI_FONT_BOLD_32, std::to_string(board.score).c_str(), &score_surface->w, &score_surface->h);
        SDL_Rect score_text_rect = {
            score_rect.x + score_rect.w / 2 - score_surface->w / 2,
            score_rect.y + 36,
            score_surface->w,
            score_surface->h,
        };
        if (board.score > game_best_score)
        {
            game_best_score = board.score;
        }
        // The best score
        SDL_Surface *best_score_surface = TTF_RenderUTF8_Blended(UI_FONT_BOLD_32, std::to_string(game_best_score).c_str(), TILE_TEXT_DARK_RGB);
        SDL_Texture *best_score_texture = SDL_CreateTextureFromSurface(renderer, best_score_surface);
        TTF_SizeUTF8(UI_FONT_BOLD_32, std::to_string(game_best_score).c_str(), &best_score_surface->w, &best_score_surface->h);
        SDL_Rect best_score_text_rect = {
            best_score_rect.x + best_score_rect.w / 2 - best_score_surface->w / 2,
            best_score_rect.y + 36,
            best_score_surface->w,
            best_score_surface->h,
        };
        SDL_RenderCopy(renderer, score_texture, NULL, &score_text_rect);
        SDL_RenderCopy(renderer, best_score_texture, NULL, &best_score_text_rect);
        SDL_DestroyTexture(score_texture);
        SDL_DestroyTexture(best_score_texture);
        SDL_FreeSurface(score_surface);
        SDL_FreeSurface(best_score_surface);
    }
    void render()
    {
        current_tick = SDL_GetTicks64();
        if (move_result.moved)
        {
            SDL_SetRenderTarget(renderer, screen_as_texture);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
            SDL_RenderClear(renderer);
            _render();
            render_scoreboard();
            SDL_SetRenderTarget(renderer, NULL);
            move_result.moved = false;
        }
        SDL_RenderCopy(renderer, screen_as_texture, NULL, NULL);
        if (last_new_tile_pos.first != -1)
        {
            animation_render_new_tile();
        }
        if (move_result.moved_tiles.size() > 0)
        {
            animation_render_moved_tiles();
        }
        if (move_result.merged_tiles.size() > 0)
        {
            animation_render_merged_tiles();
        }
    }
    ~Game()
    {
        SDL_DestroyTexture(screen_as_texture);
        SDL_DestroyTexture(bg_texture);
        return;
    }
    void destroy()
    {
        delete this;
    }
};
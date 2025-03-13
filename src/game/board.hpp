/// Before my instructor or anyone asking, this piece of shit is
/// 90% AI generated and I'm sorry for that. I just don't want to 
/// learn the 2048 algorithm from scratch because I have to deal with
/// other things like rendering and animation too.

#include <iostream>
#include <vector>
#include <random>

// Define structures to track tile movements and merges
struct TileMove
{
    int from_row, from_col; // Original position
    int to_row, to_col;     // New position
    int value;            // Tile value
};

struct TileMerge
{
    int from_row_1, from_col_1; // First tile original position
    int from_row_2, from_col_2; // Second tile original position
    int to_row, to_col;       // Position where merge happened
    int valueResult;        // Value after merge (2x original)
};

class Board
{
    std::mt19937 rng;

public:
    struct MoveResult
    {
        bool moved;
        std::vector<TileMove> moved_tiles;
        std::vector<TileMerge> merged_tiles;
    };

private:
    void rotate_clockwise()
    {
        std::vector<std::vector<int>> rotated(size, std::vector<int>(size, 0));
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                rotated[j][size - 1 - i] = grid[i][j];
            }
        }
        grid = rotated;
    }
    std::pair<int, int> convert_position(int row, int col, int rotations)
    {
        std::pair<int, int> pos(row, col);
        for (int i = 0; i < rotations; i++)
        {
            pos = {pos.second, size - 1 - pos.first};
        }
        return pos;
    }
    // Implementation for left move that tracks tile movements and merges
    MoveResult move_left_with_tracking()
    {
        MoveResult result;
        result.moved = false;

        // Create a position map to track where each tile came from
        std::vector<std::vector<std::pair<int, int>>> orig_pos(size, std::vector<std::pair<int, int>>(size));

        // Initialize original positions
        for (int row = 0; row < size; row++)
        {
            for (int col = 0; col < size; col++)
            {
                orig_pos[row][col] = {row, col};
            }
        }

        for (int row = 0; row < size; row++)
        {
            std::vector<bool> merged(size, false);

            for (int col = 1; col < size; col++)
            {
                if (grid[row][col] == 0)
                    continue;

                int value = grid[row][col];
                std::pair<int, int> tileorig_pos = orig_pos[row][col];
                int target_col = col;

                // Slide the tile left as far as possible
                while (target_col > 0 && grid[row][target_col - 1] == 0)
                {
                    // Move tile
                    grid[row][target_col - 1] = grid[row][target_col];
                    grid[row][target_col] = 0;

                    // Track original position
                    orig_pos[row][target_col - 1] = tileorig_pos;

                    target_col--;
                    result.moved = true;
                }

                // Check for possible merge
                if (target_col > 0 && grid[row][target_col - 1] == grid[row][target_col] && !merged[target_col - 1])
                {
                    // Get original positions for both tiles involved in merge
                    std::pair<int, int> leftTileorig_pos = orig_pos[row][target_col - 1];

                    // Record merge
                    TileMerge mergeInfo;
                    mergeInfo.from_row_1 = leftTileorig_pos.first;
                    mergeInfo.from_col_1 = leftTileorig_pos.second;
                    mergeInfo.from_row_2 = tileorig_pos.first;
                    mergeInfo.from_col_2 = tileorig_pos.second;
                    mergeInfo.to_row = row;
                    mergeInfo.to_col = target_col - 1;
                    mergeInfo.valueResult = grid[row][target_col - 1] * 2;

                    result.merged_tiles.push_back(mergeInfo);

                    // Perform merge
                    grid[row][target_col - 1] *= 2;
                    grid[row][target_col] = 0;
                    merged[target_col - 1] = true;
                    result.moved = true;
                    // add score
                    score += grid[row][target_col - 1];
                    // Set won flag
                    if (grid[row][target_col - 1] == 2048)
                    {
                        won = true;
                    }
                }
                // If the tile moved but didn't merge, record the move
                else if (target_col != col)
                {
                    TileMove moveInfo;
                    moveInfo.from_row = tileorig_pos.first;
                    moveInfo.from_col = tileorig_pos.second;
                    moveInfo.to_row = row;
                    moveInfo.to_col = target_col;
                    moveInfo.value = value;

                    result.moved_tiles.push_back(moveInfo);
                }
            }
        }

        return result;
    }

public:
    std::vector<std::vector<int>> grid;
    // You should not set this variable lol
    bool won;
    int size;
    int score;
    Board(int size = 4)
    {
        this->size = size;
        this->score = 0;
        this->won = false;
        this->rng = std::mt19937(std::random_device()());
        this->grid = std::vector<std::vector<int>>(size, std::vector<int>(size, 0));
        this->add_tile();
        this->add_tile();
    }
    std::pair<int, int> add_tile()
    {
        std::vector<std::pair<int, int>> empty_tiles;
        for (int i = 0; i < this->size; i++)
        {
            for (int j = 0; j < this->size; j++)
            {
                if (this->grid[i][j] == 0)
                {
                    empty_tiles.push_back({i, j});
                }
            }
        }
        if (empty_tiles.size() == 0)
        {
            return std::pair<int, int>(-1, -1);
        }
        std::uniform_int_distribution<int> dist(0, empty_tiles.size() - 1);
        auto [i, j] = empty_tiles[dist(rng)];
        std::uniform_int_distribution<int> tile_dist(0, 9);
        this->grid[i][j] = (tile_dist(rng) < 9) ? 2 : 4;
        return std::pair<int, int>(i, j);
    }
    MoveResult move(int direction)
    {
        MoveResult result;
        result.moved = false;
        for (int i = 0; i < direction; i++)
        {
            rotate_clockwise();
        }
        auto moveResult = move_left_with_tracking();
        result.moved = moveResult.moved;
        for (auto &move : moveResult.moved_tiles)
        {
            auto fromPos = convert_position(move.from_row, move.from_col, (4 - direction) % 4);
            auto toPos = convert_position(move.to_row, move.to_col, (4 - direction) % 4);
            move.from_row = fromPos.first;
            move.from_col = fromPos.second;
            move.to_row = toPos.first;
            move.to_col = toPos.second;
        }
        for (auto &merge : moveResult.merged_tiles)
        {
            auto fromPos1 = convert_position(merge.from_row_1, merge.from_col_1, (4 - direction) % 4);
            auto fromPos2 = convert_position(merge.from_row_2, merge.from_col_2, (4 - direction) % 4);
            auto toPos = convert_position(merge.to_row, merge.to_col, (4 - direction) % 4);
            merge.from_row_1 = fromPos1.first;
            merge.from_col_1 = fromPos1.second;
            merge.from_row_2 = fromPos2.first;
            merge.from_col_2 = fromPos2.second;
            merge.to_row = toPos.first;
            merge.to_col = toPos.second;
        }
        result.moved_tiles = moveResult.moved_tiles;
        result.merged_tiles = moveResult.merged_tiles;
        for (int i = 0; i < (4 - direction) % 4; i++)
        {
            rotate_clockwise();
        }

        return result;
    }
    bool is_game_over()
    {
        for (int row = 0; row < size; row++)
        {
            for (int col = 0; col < size; col++)
            {
                if (grid[row][col] == 0)
                {
                    return false;
                }
            }
        }
        for (int row = 0; row < size; row++)
        {
            for (int col = 0; col < size - 1; col++)
            {
                if (grid[row][col] == grid[row][col + 1])
                {
                    return false;
                }
            }
        }
        for (int row = 0; row < size - 1; row++)
        {
            for (int col = 0; col < size; col++)
            {
                if (grid[row][col] == grid[row + 1][col])
                {
                    return false;
                }
            }
        }
        return true;
    }
    void reset()
    {
        for (int row = 0; row < size; row++)
        {
            for (int col = 0; col < size; col++)
            {
                grid[row][col] = 0;
            }
        }
        score = 0;
        won = false;
        add_tile();
        add_tile();
    }
};
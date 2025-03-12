#include <iostream>
#include <vector>
#include <random>

/// @brief Before my instructor or anyone asking,
/// this piece of shit is 90% AI generated and I'm sorry for that.
/// I just don't want to learn the 2048 algorithm from scratch.
class Board
{
    std::mt19937 rng;

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
    void add_tile()
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
            return;
        }
        std::uniform_int_distribution<int> dist(0, empty_tiles.size() - 1);
        auto [i, j] = empty_tiles[dist(rng)];
        std::uniform_int_distribution<int> tile_dist(0, 9);
        this->grid[i][j] = (tile_dist(rng) < 9) ? 2 : 4;
    }
    bool move(int direction)
    {
        bool moved = false;
        for (int i = 0; i < direction; i++)
        {
            rotate_clockwise();
        }
        for (int row = 0; row < size; row++)
        {
            for (int col = 1; col < size; col++)
            {
                if (grid[row][col] == 0)
                    continue;

                int curr = col;
                while (curr > 0 && grid[row][curr - 1] == 0)
                {
                    grid[row][curr - 1] = grid[row][curr];
                    grid[row][curr] = 0;
                    curr--;
                    moved = true;
                }
                if (curr > 0 && grid[row][curr - 1] == grid[row][curr])
                {
                    grid[row][curr - 1] *= 2;
                    grid[row][curr] = 0;
                    moved = true;
                }
            }
            for (int col = 1; col < size; col++)
            {
                if (grid[row][col] == 0)
                    continue;

                int curr = col;
                while (curr > 0 && grid[row][curr - 1] == 0)
                {
                    grid[row][curr - 1] = grid[row][curr];
                    grid[row][curr] = 0;
                    curr--;
                    moved = true;
                }
            }
        }
        for (int i = 0; i < (4 - direction) % 4; i++)
        {
            rotate_clockwise();
        }
        if (moved)
        {
            for (int row = 0; row < size; row++)
            {
                for (int col = 0; col < size; col++)
                {
                    if (grid[row][col] == 2048)
                    {
                        won = true;
                    }
                }
            }
        }

        return moved;
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
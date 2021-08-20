#include "sudoku_solver.h"
#include <iostream>
#include <set>
#include <vector>

SudokuSolver::SudokuSolver(grid2d_t &grid)
    : m_grid{ grid }
{
}

void SudokuSolver::printGrid()
{
    for (const std::vector<Tile> &row : m_grid)
    {
        for (const Tile &tile : row)
        {
            std::cout << tile.value << ',';
        }
        std::cout << '\n';
    }
    std::cout << "\n\n";
}

void SudokuSolver::printTile(std::size_t y, std::size_t x)
{
    Tile &tile{ m_grid[y][x] };
    std::cout << '(' << tile.row << ',' << tile.column << "): " << tile.value
              << "\n    ";
    for (int digit : tile.impossible)
    {
        std::cout << digit << ' ';
    }
    std::cout << "\n\n";
}

bool operator<(const Tile &tile1, const Tile &tile2)
{
    return ((tile1.row * 10) + tile1.column < (tile2.row * 10) + tile2.column);
}

/*
Look at a collective set of squares, and determine whether there is only one
possible place for the given digit to go; if so then return it. If not, then
return nullptr.
*/

Tile* SudokuSolver::findUniqueTile(std::vector<Tile*> &tile_set, int digit)
{
    /*
    Iterate through the tiles and count how many cannot hold this digit.
    We will also keep track of the last open tile that we found, so that
    if there is only one we can return it.
    */
    int restricted{ 0 };
    Tile *last_open{};
    for (auto &tile : tile_set)
    {
        /*
        We will assume that the caller knows that the digit
        does need to go somewhere in the tile set.
        */
        

        if (tile->value > 0 or
            tile->impossible.find(digit) != tile->impossible.end())
        {
            ++restricted;
        }
        else
        {
            last_open = tile;
        }
    }

    if (restricted == 8)
    {
        return last_open;
    }

    return nullptr;
}


int SudokuSolver::examineTileSet(std::vector<Tile*> &tile_set)
{
    int digits_found{ 0 };
    for (int digit{1}; digit <= 9; ++digit)
    {
        bool skip_digit{ false };
        for (const auto &tile : tile_set)
        {
            if (tile->value == digit)
            {
                skip_digit = true;
                break;
            }
        }
        if (skip_digit)
        {
            continue;
        }

        Tile *unique_tile{ findUniqueTile(tile_set, digit) };
        if (unique_tile and unique_tile->value == 0)
        {
            unique_tile->value = digit;
            for (int i{ 0 }; i <= 9; ++i)
            {
                if (i != digit)
                {
                    unique_tile->impossible.insert(i);
                }
            }
            ++digits_found;
            std::cout << "Tileset has only one spot for " << digit << ".\n";
            printGrid();
        }
    }

    return digits_found;
}

void SudokuSolver::getBox(
    std::vector<Tile*> &box, std::size_t row_index, std::size_t column_index)
{
    std::size_t box_left_index{ column_index - (column_index % 3) };
    std::size_t box_top_index{ row_index - (row_index % 3) };

    for(std::size_t y{ box_top_index }; y < box_top_index + 3; ++y)
    {
        for(std::size_t x{ box_left_index}; x < box_left_index + 3; ++x)
        {
            box.push_back( &m_grid[y][x] );
        }
    }
}

void SudokuSolver::getColumn(
    std::vector<Tile*> &column, std::size_t column_index)
{
    for (std::size_t y{ 0 }; y < 9; ++y)
    {
        std::vector<Tile> &row = m_grid[y];
        if (!row.empty())
        {
            column.push_back( &row[column_index] );
        }
    }
}

void SudokuSolver::getRow(std::vector<Tile*> &row, std::size_t row_index)
{
    for (std::size_t x{ 0 }; x < 9; ++x)
    {
        row.push_back( &m_grid[row_index][x] );
    }
}

int SudokuSolver::findUniqueDigit(Tile &tile)
{
    if (tile.impossible.size() != 9)
    {
        return 0;
    }

    for (int digit{ 1 }; digit <= 9; ++digit)
    {
        if (tile.impossible.find(digit) == tile.impossible.end())
        {
            tile.value = digit;
            std::cout << "Tile must be a " << digit << ".\n";
            printGrid();
        }
        else
        {
            tile.impossible.insert(digit);
        }
    }

    return 1;
}

int SudokuSolver::findImpossible(Tile &tile, std::vector<Tile*> &tile_set)
{
    int marked_impossible{ 0 };
    for (auto &other_tile : tile_set)
    {
        if (tile.impossible.find(other_tile->value) == tile.impossible.end())
        {
            tile.impossible.insert(other_tile->value);
            ++marked_impossible;
        }
    }

    return marked_impossible;
}

int SudokuSolver::handleTile(Tile &tile)
{
    int found{ 0 };
    int marked_impossible{ 0 };

    // Check the row.
    std::vector<Tile*> row{};
    getRow(row, tile.row);
    marked_impossible += findImpossible(tile, row);
    found += examineTileSet(row);

    // Check the column.
    std::vector<Tile*> column{};
    getColumn(column, tile.column);
    marked_impossible += findImpossible(tile, column);
    found += examineTileSet(column);

    // Check the box.
    std::vector<Tile*> box{};
    getBox(box, tile.row, tile.column);
    marked_impossible += findImpossible(tile, box);
    found += examineTileSet(box);

    found += findUniqueDigit(tile);

    if (marked_impossible)
    {
        //std::cout << "Marked " << marked_impossible << '\n';
    }

    if (found)
    {
        //std::cout << "Found " << found << '\n';
    }

    if (marked_impossible or found)
    {
        return found;
    }
    return -1;
}

bool SudokuSolver::zeroesRemain()
{
    for (const std::vector<Tile> &row : m_grid)
    {
        for (const Tile &tile : row)
        {
            if (tile.value == 0)
            {
                return true;
            }
        }
    }
    return false;
}

int SudokuSolver::doIteration()
{
    int found{ 0 };
    bool should_continue{ false };
    for (std::vector<Tile> &row : m_grid)
    {

        for (Tile &tile : row)
        {
            // If we already know it then don't bother!
            if (tile.value != 0)
            {
                continue;
            }

            int digits_found{ handleTile(tile) };
            if (digits_found != -1)
            {
                found += digits_found;
                should_continue = true;
            }
        }
    }
    if (should_continue)
    {
        return found;
    }

    return -1;
}

void SudokuSolver::solve()
{
    for (int iterations{ 0 }; zeroesRemain(); ++iterations)
    {
        std::cout << "::ITERATION " << iterations << "::\n";
        int found = doIteration();

        if (found == -1)
        {
            std::cout << "Failed after " << iterations << " iterations.\n";
            return;
        }

        std::cout << "Found " << found << " new digits.\n\n";
    }

    std::cout << "SOLVED!\n";
}

#ifndef SUDOKU_SOLVER_H_INCLUDED
#define SUDOKU_SOLVER_H_INCLUDED

#include <array>
#include <set>
#include <vector>

struct Tile
{
    std::size_t row;
    std::size_t column;
    int value;
    std::set<int> impossible;
};

// 2DGrid_t is a 9 by 9 2 dimensional array.
using grid2d_t = std::vector<std::vector<Tile>>;

class SudokuSolver
{
    // Grid will be a multidimensional vector.
    grid2d_t m_grid;

    Tile* findUniqueTile(std::vector<Tile*> &tile_set, int digit);

    int examineTileSet(std::vector<Tile*> &tile_set);

    int findUniqueDigit(Tile &tile);

    int findImpossible(Tile &tile, std::vector<Tile*> &tileSet);

    int handleTile(Tile &tile);

    // Sets need to do comparisons.
    friend bool operator<(const Tile &tile1, const Tile &tile2);

public:
    SudokuSolver(grid2d_t &grid);
    void printGrid();
    void printTile(std::size_t y, std::size_t x);

    void getBox(
        std::vector<Tile*> &box, std::size_t row_index, std::size_t column_index);

    void getColumn(std::vector<Tile*> &column, std::size_t column_index);

    void getRow(std::vector<Tile*> &row, std::size_t row_index);

    bool zeroesRemain();
    int doIteration();
    void solve();
};

#endif // SUDOKU_SOLVER_H_INCLUDED

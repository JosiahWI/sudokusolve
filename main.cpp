#include "sudoku_solver.h"
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <set>
#include <string>

grid2d_t* getInputGrid(grid2d_t *grid)
{
    std::ifstream in_stream{ "grid.txt" };

    if (!in_stream)
    {
        std::cerr << "grid.txt could not be opened for reading.\n";
        exit(1);
    }

    for (std::size_t y{ 0 }; in_stream; ++y)
    {
        std::string strInput;
        std::getline(in_stream, strInput);
        std::vector<Tile> next_row{};
        std::size_t x{ 0 };
        for (const char next_char : strInput)
        {
            if (next_char != ',')
            {
                // I don't know why I have to subtract 48 but I do.
                int tile_value{ static_cast<int>(next_char) - 48 };
                /*
                I won't dynamically allocate the tiles, because then I
                have to call the destructor on each tile before I delete
                the grid. A simple class with a destructor would fix this,
                but that can be another project.
                */
                std::set<int> impossible{};
                if (tile_value != 0)
                {
                    // 0 is also added, because it has a value now.
                    for (int digit{ 0 }; digit <= 9; ++digit)
                    {
                        if (digit != tile_value)
                        {
                            impossible.insert(digit);
                        }
                    }
                }
                next_row.push_back( {y, x, tile_value, impossible} );
                ++x;
            }
        }
        grid->push_back(next_row);
    }
    return grid;
}

int main() {
    grid2d_t *grid{ new(grid2d_t) };
    grid = getInputGrid(grid);

    SudokuSolver solver{ *grid };
    solver.printGrid();
    solver.solve();
    solver.printGrid();

    delete grid;

    return 0;
}

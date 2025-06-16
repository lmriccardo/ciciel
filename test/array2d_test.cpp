#include <iostream>
#include <iomanip>
#include <data_structures/grids/array2d.hpp>

using namespace ccl::ds::grids;

template <size_t R, size_t C>
void printOutGrid(Array2D<int, R, C>& grid)
{
    size_t col = grid.size(1);
    std::cout << std::endl << "Array2D:" << std::endl;
    for(size_t idx = 0; idx < grid.size(); ++idx)
    {
        if(idx % col == 0 && idx > 0)
            std::cout << std::endl;

        std::cout << std::setw(3) << grid[idx] << " ";
    }

    std::cout << std::endl;
}

int main()
{
    Array2D<int, 4, 3> grid;
    std::cout << grid.size() << std::endl;

    try
    {
        grid.set( 1, 12 );
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    for ( size_t i = 0; i < grid.size(); ++i )
        grid.set( static_cast<int>(i + 1), i );

    printOutGrid(grid);
    
    grid.transpose();
    
    printOutGrid(grid);
    
    std::cout << std::endl;
    std::cout << "Nof Columns:\t" << grid.size( 1 ) << std::endl;
    std::cout << "Nof Rows:\t" << grid.size( 0 ) << std::endl;

    try
    {
        grid.at( 3, 1 );
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    std::cout << "Element position (1, 3): " << grid.at(1, 3) << std::endl;

    grid.transpose();
    Array2D<int, 3, 4> grid_t = grid.deepTranspose();

    printOutGrid(grid_t);

    return 0;
}
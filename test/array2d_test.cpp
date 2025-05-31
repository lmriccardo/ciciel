#include <iostream>
#include <data_structures/grids/array2d.hpp>

using namespace ccl::ds::grids;

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

    auto it = grid.begin();

    for ( ; it != grid.end(); ++it )
    {
        std::cout << "Element " << it.pos() << " : " << *it << std::endl;
    }

    grid.transpose();

    std::cout << std::endl;

    it = grid.begin();

    for ( ; it != grid.end(); ++it )
    {
        auto el = *it;
        std::cout << "Element " << it.pos() << " : " << el << std::endl;
    }

    std::cout << "Nof Columns: " << grid.size( 1 ) << std::endl;
    std::cout << "Nof Rows: " << grid.size( 0 ) << std::endl;

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

    auto it2 = grid_t.begin();

    for ( ; it2 != grid_t.end(); ++it2 )
    {
        auto el = *it2;
        std::cout << "Element " << it2.pos() << " : " << el << std::endl;
    }

    return 0;
}
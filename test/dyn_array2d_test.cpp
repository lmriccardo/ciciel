#include <data_structures/grids/dynamic_array2d.hpp>
#include <iostream>

using namespace ccl::ds::grids;

int main()
{
    DynamicArray2D<int> grid( 4, 3 );

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

    grid.extend( 3, 2 );

    for ( size_t r_idx = 0; r_idx < grid.size( (size_t)ccl::ds::Selector2D::ROWS ); ++r_idx )
    {
        for ( size_t c_idx = 0; c_idx < grid.size( (size_t)ccl::ds::Selector2D::COLUMNS ); ++c_idx )
        {
            std::cout << grid.at( r_idx, c_idx ) << " ";
        }

        std::cout << std::endl;
    }

    return 0;
}
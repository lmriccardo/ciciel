#include <data_structures/data_structures.hpp>

using namespace ccl::ds::grids;

int main()
{
    Vec2<int> v { 1, 2 };
    std::cout << v << std::endl;
    
    v.set(3, 1);
    std::cout << v << std::endl;

    return 0;
}
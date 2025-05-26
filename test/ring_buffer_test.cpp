#include <data_structures/data_structures.hpp>
#include <iostream>
#include <iomanip>

namespace ds = ccl::ds::buffers;

int main()
{
    ds::RingBuffer<int> rb(10);

    std::cout << "RingBuffer size: " << rb.size() << std::endl;
    std::cout << "RingBuffer capacity: " << rb.capacity() << std::endl;

    int front, back;
    std::cout << "RingBuffer tryFront result: " << std::boolalpha << rb.tryFront(front) << std::endl;

    try {
        front = rb.front();
    } catch ( std::exception& e ) {
        std::cerr << "RingBuffer front result: " << e.what() << std::endl;
    }

    for ( int idx = 1; idx < 11; ++idx )
    {
        std::cout << "Putting element: " << idx << std::endl;
        rb.put(idx);
    }

    std::cout << "\n";
    for ( int idx = 0; idx < 10; ++idx )
    {
        std::cout << "Reading element at position: " << idx << " -> "
                  << rb[idx]
                  << std::endl;
    }

    std::cout << "\n";
    std::cout << "RingBuffer tryFront result: " << std::boolalpha 
              << rb.tryFront(front) << " -> " 
              << front << std::endl;

    std::cout << "RingBuffer tryBack result: " << std::boolalpha 
              << rb.tryBack(back) << " -> " 
              << back << std::endl;

    std::cout << "RingBuffer size: " << rb.size() << std::endl;
    
    std::cout << "Putting a new element overwriting the front: 11" << std::endl;
    rb.put(11);
    std::cout << "New front: " << rb.front() << std::endl;
    std::cout << "New back: " << rb.back() << std::endl;
    std::cout << "RingBuffer size: " << rb.size() << std::endl;

    int pop_back;
    std::cout << "RingBuffer tryPopBack result: " << std::boolalpha << rb.tryPopBack(pop_back) << std::endl;

    std::cout << "\n";
    for ( size_t idx = 0; idx < rb.size(); ++idx )
    {
        std::cout << "Reading element at position: " << idx << " -> "
                  << rb[idx]
                  << std::endl;
    }

    rb.clear();
    std::cout << "RingBuffer cleared" << std::endl;
    std::cout << "RingBuffer size: " << rb.size() << std::endl;
    std::cout << "RingBuffer capacity: " << rb.capacity() << std::endl;
    std::cout << "RingBuffer tryFront result: " << std::boolalpha << rb.tryFront(front) << std::endl;

    std::cout << "Copying into a new RingBuffer" << std::endl;
    ds::RingBuffer<int> rb2 = rb;
    
    std::cout << "Creating from vector" << std::endl;
    ds::RingBuffer<int> rb3{ std::vector<int>{2, 4, 6, 8, 10} };
    std::cout << "RingBuffer3 size: " << rb3.size() << std::endl;
    std::cout << "RingBuffer3 capacity: " << rb3.capacity() << std::endl;
    
    for ( size_t idx = 0; idx < rb3.size(); ++idx )
    {
        std::cout << "RingBuffer3 Reading element at position: " << idx << " -> "
                  << rb3[idx]
                  << std::endl;
    }

    std::cout << "Trying iterators" << std::endl;
    for ( const auto& elem : rb3 )
    {
        std::cout << "Element: " << elem << std::endl;
    }

    std::cout << "Trying iterators explicit version" << std::endl;
    ds::RingBuffer<int>::iterator it = rb3.begin();
    for ( ; it != rb3.end(); it++ )
    {
        std::cout << "Element: " << *it << std::endl;
    }

    return 0;
}
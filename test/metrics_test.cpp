#include <metrics/metrics_gather.hpp>
#include <thread>
#include <array>
#include <vector>

using namespace ccl::metrics;

namespace prova
{
    void inner_perform_task()
    {
        auto collector = MetricsCollector::create( __FUNCNAME__() );

        std::array<int, 1024 * 512> array;
        for (size_t i = 0; i < array.size(); ++i) {
            array[i] = i;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        collector->collect(); // Collect all the metrics
    }

    void perform_task() {
        
        auto collector = MetricsCollector::create( __FUNCNAME__() );

        // Simulate some work
        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        // Simulate allocating some memory
        std::vector<int> large_vector(1024 * 1024, 0); // ~4MB of RAM
        std::array<int, 1024 * 512> array;
        
        // New code to force physical memory allocation
        for (size_t i = 0; i < large_vector.size(); ++i) {
            large_vector[i] = i;
        }

        for (size_t i = 0; i < array.size(); ++i) {
            array[i] = i;
        }

        inner_perform_task();

        std::this_thread::sleep_for(std::chrono::milliseconds(200));

        collector->collect();
    }
}

int main()
{
    prova::perform_task();

    return 0;
}
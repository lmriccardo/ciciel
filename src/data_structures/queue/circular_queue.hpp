#pragma once

#include <data_structures/buffers/ring_buffer.hpp>
#include "queue_interface.hpp"

namespace ccl::ds
{
    template <typename T>
    class CircularQueue : RandomAccessQueueInterface<T>
    {
    private:
        RingBuffer<T> m_buffer;
    };
};
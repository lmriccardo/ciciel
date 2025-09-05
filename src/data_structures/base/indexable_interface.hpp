#pragma once

#include <cstddef>

namespace ccl::ds::base
{
    /**
     * Just an interface for a generic container. The only usage is to 
     * create base iterator classes for iterable objects.
     */
    template <typename T>
    class IndexableInterface
    {
    public:
        ~IndexableInterface() = default;

        virtual constexpr T&       at(size_t)               = 0;
        virtual constexpr const T& at(size_t)         const = 0;
        virtual constexpr const T& operator[](size_t) const = 0;
        virtual constexpr T&       operator[](size_t)       = 0;
    };

    template <typename T>
    class Indexable2DInterface : public IndexableInterface<T>
    {
    public:
        ~Indexable2DInterface() = default;

        using IndexableInterface<T>::at;

        virtual constexpr T&       at(size_t, size_t)       = 0;
        virtual constexpr const T& at(size_t, size_t) const = 0;
    };
};
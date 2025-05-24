#pragma once

namespace ccl::ds::base
{
    /**
     * Just an interface for a generic container. The only usage is to 
     * create base iterator classes for iterable objects.
     */
    template <typename T>
    class ContainerInterface
    {
    public:
        ~ContainerInterface() = default;
        virtual T& operator[](size_t) = 0;
        virtual const T& operator[](size_t) const = 0;
    };
};
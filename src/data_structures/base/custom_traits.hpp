#pragma once

#include <cstddef>
#include <type_traits>
#include <array>
#include <vector>

namespace ccl::ds::base
{
    template <typename T, typename U>
    struct is_std_array_of : std::false_type {};

    template <typename T, std::size_t N>
    struct is_std_array_of<T, std::array<T, N>> : std::true_type {};

    template <typename T, typename U>
    using is_container = std::enable_if_t<std::is_same_v<std::decay_t<U>, std::vector<T>> 
        || is_std_array_of<T, std::decay_t<U>>::value>;

    // TODO: Custom type traits for checking the template Container parameter must be created.
}
#pragma once

namespace ccl::cli::ui
{
    struct ASCII
    {
        // Double charset for double-size border line
        static const char32_t DOUBLE_VERTICAL   = U'║';
        static const char32_t DOUBLE_HORIZONTAL = U'═';
        static const char32_t DOUBLE_DOWN_RIGHT = U'╔';
        static const char32_t DOUBLE_UP_RIGHT   = U'╚';
        static const char32_t DOUBLE_DOWN_LEFT  = U'╗';
        static const char32_t DOUBLE_UP_LEFT    = U'╝';

        // Single charset for unit-size border line
        static const char32_t LIGHT_VERTICAL   = U'│';
        static const char32_t LIGHT_DOWN_LEFT  = U'┐';
        static const char32_t LIGHT_UP_RIGHT   = U'└';
        static const char32_t LIGHT_HORIZONTAL = U'─';
        static const char32_t LIGHT_UP_LEFT    = U'┘';
        static const char32_t LIGHT_DOWN_RIGHT = U'┌';

        static const char32_t SOLID_BLOCK      = U'█';
        static const char32_t LOWER_HALF_BLOCK = U'▄';
        static const char32_t LEFT_HALF_BLOCK  = U'▌';
        static const char32_t RIGHT_HALF_BLOCK = U'▐';
        static const char32_t UPPER_HALF_BLOCK = U'▀';
    };
}
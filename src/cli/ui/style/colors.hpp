#pragma once

#include <ostream>
#include <data_structures/grids/vec3.hpp>

namespace ccl::cli::ui
{
    struct Color : public ccl::ds::grids::Vec3<u_int16_t>
    {
        int m_xterm_number = 256;

        Color() = default;
        Color( u_int16_t r, u_int16_t g, u_int16_t b );
        Color( u_int16_t r, u_int16_t g, u_int16_t b, int x_num );
        virtual ~Color() = default;

        Color(const Color&) = default;
        Color& operator=(const Color&) = default;

        using ccl::ds::grids::Vec3<u_int16_t>::operator=;

        u_int16_t& r();
        u_int16_t& g();
        u_int16_t& b();
        
        const u_int16_t& r() const;
        const u_int16_t& g() const;
        const u_int16_t& b() const;
        
        unsigned int hex() const;
    };

    std::ostream& operator<<( std::ostream& os, const Color& color );

    struct Colors
    {
        inline const static Color Black   = Color{   0,   0,   0,  0 };
        inline const static Color Maroon  = Color{ 128,   0,   0,  1 };
        inline const static Color Green   = Color{   0, 128,   0,  2 };
        inline const static Color Olive   = Color{ 128, 128,   0,  3 };
        inline const static Color Navy    = Color{   0,   0, 128,  4 };
        inline const static Color Purple  = Color{ 128,   0, 128,  5 };
        inline const static Color Teal    = Color{   0, 128, 128,  6 };
        inline const static Color Silver  = Color{ 192, 192, 192,  7 };
        inline const static Color Grey    = Color{ 128, 128, 128,  8 };
        inline const static Color Red     = Color{ 255,   0,   0,  9 };
        inline const static Color Lime    = Color{   0, 255,   0, 10 };
        inline const static Color Yellow  = Color{ 255, 255,   0, 11 };
        inline const static Color Blue    = Color{   0,   0, 255, 12 };
        inline const static Color Fuchsia = Color{ 255,   0, 255, 13 };
        inline const static Color Aqua    = Color{   0, 255, 255, 14 };
        inline const static Color White   = Color{ 255, 255, 255, 15 };
    };
};
#pragma once

#include <ostream>
#include <data_structures/grids/vec3.hpp>

namespace ccl::cli::ui
{
    struct Color : public ccl::ds::grids::Vec3<u_int16_t>
    {
        int m_xterm_number = 256;

        Color( u_int16_t r, u_int16_t g, u_int16_t b );
        Color( u_int16_t r, u_int16_t g, u_int16_t b, int x_num );
        virtual ~Color() = default;

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
        inline static Color BLACK   = Color{   0,   0,   0,  0 };
        inline static Color MAROON  = Color{ 128,   0,   0,  1 };
        inline static Color GREEN   = Color{   0, 128,   0,  2 };
        inline static Color OLIVE   = Color{ 128, 128,   0,  3 };
        inline static Color NAVY    = Color{   0,   0, 128,  4 };
        inline static Color PURPLE  = Color{ 128,   0, 128,  5 };
        inline static Color TEAL    = Color{   0, 128, 128,  6 };
        inline static Color SILVER  = Color{ 192, 192, 192,  7 };
        inline static Color GREY    = Color{ 128, 128, 128,  8 };
        inline static Color RED     = Color{ 255,   0,   0,  9 };
        inline static Color LIME    = Color{   0, 255,   0, 10 };
        inline static Color YELLOW  = Color{ 255, 255,   0, 11 };
        inline static Color BLUE    = Color{   0,   0, 255, 12 };
        inline static Color FUCHSIA = Color{ 255,   0, 255, 13 };
        inline static Color AQUA    = Color{   0, 255, 255, 14 };
        inline static Color WHITE   = Color{ 255, 255, 255, 15 }; 
    };
};
#include "colors.hpp"

using namespace ccl::cli::ui;
using namespace ccl::ds::grids;

Color::Color(u_int16_t r, u_int16_t g, u_int16_t b)
    : Vec3<u_int16_t>(r, g, b)
{
}

Color::Color(u_int16_t r, u_int16_t g, u_int16_t b, int x_num)
    : Color( r, g, b )
{
    m_xterm_number = x_num;
}

u_int16_t &Color::r() { return m_x; }
u_int16_t &Color::g() { return m_y; }
u_int16_t &Color::b() { return m_z; }

const u_int16_t &Color::r() const { return m_x; }
const u_int16_t &Color::g() const { return m_y; }
const u_int16_t &Color::b() const { return m_z; }

unsigned int Color::hex() const
{
    return (r() << 16) | (g() << 8) | b();
}

std::ostream &ccl::cli::ui::operator<<(std::ostream &os, const Color &color)
{
    const Vec3<u_int16_t>& vec = color;
    os << "Color" << vec << "#0x" << std::hex << color.hex()
       << "#XTERM-IDX{" << color.m_xterm_number << "}";
    
    return os;
}

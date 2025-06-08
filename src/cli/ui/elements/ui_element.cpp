#include "ui_element.hpp"

using namespace ccl::cli::ui;

UIElement::UIElement(size_t w, size_t h, size_t x, size_t y)
    : m_pos_x( x ), m_pos_y( y )
{
    m_winsize.ws_row = h;
    m_winsize.ws_col = w;
}

std::pair<size_t, size_t> UIElement::getWinsize() const
{
    return { m_winsize.ws_row, m_winsize.ws_col };
}

std::pair<size_t, size_t> UIElement::getVertexCoord(Vertex v) const
{
    switch (v)
    {
        case Vertex::TL: return { m_pos_x, m_pos_y };
        case Vertex::TR: return { m_pos_x + m_winsize.ws_col, m_pos_y };
        case Vertex::BL: return { m_pos_x, m_pos_y + m_winsize.ws_row };
        case Vertex::BR: return {
            m_pos_x + m_winsize.ws_col,
            m_pos_y + m_winsize.ws_row
        };
        
        default:
            throw std::invalid_argument("Unknow vertex direction");
            return {};
    }
}

void UIElement::setWinsize(size_t rows, size_t cols)
{
    m_winsize.ws_col = cols;
    m_winsize.ws_row = rows;
}

void UIElement::setStartPosition(size_t s_x, size_t s_y)
{
    m_pos_x = s_x;
    m_pos_y = s_y;
}
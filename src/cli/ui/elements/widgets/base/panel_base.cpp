#include "panel_base.hpp"

using namespace ccl::cli::ui;

void PanelBase::drawTitle(ScreenBuffer &buffer) const
{
    buffer.set( m_title, m_pos_y, m_pos_x + TITLE_OFFSET, m_title_style, false );
}

PanelBase::PanelBase(const std::string &id, size_t w, size_t h,
                     size_t x, size_t y, bool leaf)
    : Widget(id, w, h, x, y, leaf)
{
    m_title_style = DefaultTitleStyle();
}

void PanelBase::setRepacking( bool value )
{
    m_needs_repacking = value;

    if ( m_needs_repacking && m_parent != nullptr )
    {
        // Sets repacking also to children
        auto panel_parent = reinterpret_cast<PanelBase*>(m_parent);
        
        if (!panel_parent->needsRepacking())
        {
            // Set the value to the parent widget (which is also a panel widget)
            panel_parent->setRepacking( value );
        }
    }
}

void PanelBase::setVerticalAlignment(LayoutAlignment value)
{
    m_v_align = value;
}

void PanelBase::setHorizontalAlignment(LayoutAlignment value)
{
    m_h_align = value;
}

void PanelBase::setTitle(const std::u32string &title, const Style &style)
{
    m_title = title;
    m_title_style = style;

    // When setting the title we should also change the minimum size
    // for the panel width, otherwise it might go out of bound.
    size_t titleSize = TITLE_OFFSET + u32swidth( title );
    setMinimumSize( titleSize + TITLE_OFFSET, getMinimumSize().second );
}

void PanelBase::setTitle(const std::string &title, const Style &style)
{
    setTitle( utf8to32(title), style );
}

void PanelBase::setTitle(const std::u32string &title)
{
    setTitle( title, m_title_style );
}

void PanelBase::setTitle(const std::string &title)
{
    setTitle( title, m_title_style );
}

void PanelBase::setParent( Widget& parent )
{
    this->Widget::setParent( parent );
    auto ptr = static_cast<PanelBase*>( &parent );
    m_parent_idx = ptr->getCurrentChildIndex();
}

const std::u32string &PanelBase::getTitle() const
{
    return m_title;
}

Style &PanelBase::getTitleStyle()
{
    return m_title_style;
}

LayoutAlignment PanelBase::getVerticalAlignment() const
{
    return m_v_align;
}

LayoutAlignment PanelBase::getHorizontalAlignment() const
{
    return m_h_align;
}

int PanelBase::getCurrentChildIndex() const
{
    return m_children_idx;
}

bool PanelBase::needsRepacking() const
{
    return m_needs_repacking;
}

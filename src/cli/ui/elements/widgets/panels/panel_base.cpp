#include "panel_base.hpp"

using namespace ccl::cli::ui;

PanelBase::PanelBase( const std::string &id, size_t w, size_t h,
               size_t x, size_t y, bool leaf )
    : Widget( id, w, h, x, y, leaf )
{

}

void PanelBase::setRepacking( bool value )
{
    m_needs_repacking = value;
    if ( m_needs_repacking && m_parent != nullptr )
    {
        // Set the value to the parent widget (which
        // is also a panel widget)
        reinterpret_cast<PanelBase*>(m_parent)->setRepacking( value );
    }
}
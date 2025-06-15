#pragma once

#include <memory>
#include <unordered_map>
#include <sstream>
#include <cli/ui/elements/containers/container.hpp>
#include <cli/ui/elements/containers/hbox_container.hpp>
#include "panel_base.hpp"

namespace ccl::cli::ui
{
    /**
     * A Panel Widget is a Container widget which is used to holds other
     * widgets ( control / container ) and organize them in a given layout.
     * Its layout manager is specified as its template parameter. There
     * are 4 possible layouts: Horizontal, Vertical, Grid and Absolute.
     */
    template <typename ContainerT, typename C = enable_if_container_t<ContainerT>>
    class PanelWidget : public PanelBase
    {
    protected:
        std::unique_ptr<ContainerT> m_container;

    public:
        PanelWidget( const std::string& id, size_t w, size_t h, size_t x, size_t y );
        virtual ~PanelWidget() = default;

        void addChild( Widget& child ) override;
        void removeChild( const std::string& id ) override;

        /**
         * Apply the layout from the container to all children of
         * of the current widget. This method must be called before
         * every draw.
         */
        void pack() override;

        /**
         * Draw the content of the panel and its children into the buffer
         * @param buffer The screen buffer
         */
        void draw( ScreenBuffer& buffer ) const override;
    };

    template <typename ContainerT, typename C>
    inline PanelWidget<ContainerT, C>::PanelWidget(
        const std::string &id, size_t w, size_t h, size_t x, size_t y
    ) : PanelBase(id, w, h, x, y, false)
    {
        m_container = std::make_unique<ContainerT>(this);
    }

    template <typename ContainerT, typename C>
    inline void PanelWidget<ContainerT, C>::addChild( Widget& w )
    {
        // First check that the widget ID is not a duplicate
        const std::string& w_name = w.getId();
        if ( m_childIds.count( w_name ) )
        {
            std::stringstream ss;
            ss << "[PanelWidget::addChild] Another widget with name: "
               << w_name << " already exists in this subtree.!!";

            throw std::runtime_error( ss.str() );
        }

        w.setParent( *this ); // Set the parent for the given widget
        m_childIds.insert( { w_name, &w } ); // Insert into the mapping
        m_container->addChild( &w ); // Add the child to the container
    }

    template <typename ContainerT, typename C>
    inline void PanelWidget<ContainerT, C>::removeChild(const std::string &id)
    {
        if ( m_container->removeChildById( id ) > 0 )
        {
            this->PanelBase::setRepacking( true );
        }
    }

    template <typename ContainerT, typename C>
    inline void PanelWidget<ContainerT, C>::pack()
    {
        if ( !m_needs_repacking ) return;

        // Apply the layout to all its children
        m_container->layout();

        // Then call pack on all sub Panels of the current one
        for ( PanelBase* panel: m_container->template getChildrenByType<PanelBase>() )
        {
            panel->setRepacking( true ); // Force repacking for children
            panel->pack();
        }

        m_needs_repacking = false;
    }

    template <typename ContainerT, typename C>
    inline void PanelWidget<ContainerT, C>::draw(ScreenBuffer &buffer) const
    {
        if (!isVisible()) return;
        this->PanelBase::draw( buffer ); // Draw the border
        drawTitle( buffer ); // Draw the title

        // Draw all the children
        for ( size_t idx = 0; idx < m_container->getNofChildren(); ++idx )
        {
            m_container->getChild(idx)->draw( buffer );
        }
    }

    /**
     * A Panel with HBox layout manager
     */
    using HBoxPanel = PanelWidget<HBoxContainer>;
}
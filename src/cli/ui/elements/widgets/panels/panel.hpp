#pragma once

#include <memory>
#include <unordered_map>
#include <cli/ui/elements/containers/container.hpp>
#include <cli/ui/elements/containers/hbox_container.hpp>
#include <sstream>
#include "../widget.hpp"

namespace ccl::cli::ui
{
    template <typename ContainerT, typename C = enable_if_container_t<ContainerT>>
    class PanelWidget : public Widget
    {
    protected:
        std::unique_ptr<ContainerT> m_container;
        std::unordered_map<std::string, Widget*> m_childIds;

        void drawBorder() const;

    public:
        PanelWidget( const std::string&, size_t, size_t, size_t, size_t );
        virtual ~PanelWidget() = default;

        void addChild( Widget& );
        void pack();
        void draw( const ScreenBuffer& ) const override;
        bool isColliding(size_t, size_t) const override;
    };

    template <typename ContainerT, typename C>
    inline void PanelWidget<ContainerT, C>::drawBorder() const
    {
    }

    template <typename ContainerT, typename C>
    inline PanelWidget<ContainerT, C>::PanelWidget(
        const std::string &id, size_t w, size_t h, size_t x, size_t y) : Widget(id, w, h, x, y, false)
    {
        m_container = std::make_unique<ContainerT>(w, h, x, y);
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
    inline void PanelWidget<ContainerT, C>::pack()
    {
        m_container->layout();
    }

    template <typename ContainerT, typename C>
    inline void PanelWidget<ContainerT, C>::draw(const ScreenBuffer &) const
    {
        drawBorder();
    }

    template <typename ContainerT, typename C>
    inline bool PanelWidget<ContainerT, C>::isColliding(size_t, size_t) const
    {
        return false;
    }

    using HBoxPanel = PanelWidget<HBoxContainer>;
}
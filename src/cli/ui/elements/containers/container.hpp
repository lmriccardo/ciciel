#pragma once

#include <vector>
#include <memory>
#include <type_traits>
#include <cli/ui/elements/widgets/widget.hpp>
#include <cli/ui/elements/widgets/panels/panel_base.hpp>

namespace ccl::cli::ui
{
    /**
     * This class represents a generic container. It is a UI element
     * but it cannot be part of the UI tree and it is only used
     * to organize elements in a panel using a certain order.
     */
    class Container
    {
    protected:
        std::vector<Widget*> m_children;
        PanelBase* m_panel;

    public:
        Container( PanelBase* panelptr );
        virtual ~Container() = default;

        void addChild(Widget*);
        int removeChild( size_t );
        int removeChildById( const std::string& );
        Widget* getChild( size_t );
        size_t getNofChildren() const;

        template <typename WidgetT>
        std::vector<WidgetT*> getChildrenByType() const;

        const std::vector<Widget*>& getAllChildren() const;
        std::vector<Widget*>& getAllChildren();

        /**
         * Apply the given layout to all children. This operation
         * might modify start and stop position, as well as
         * dimension of the children contained.
         */
        virtual void layout() = 0;
    };

    template <typename WidgetT>
    inline std::vector<WidgetT *> Container::getChildrenByType() const
    {
        static_assert( std::is_base_of_v<Widget, WidgetT>, 
            "WidgetT must derive from Widget" );

        std::vector<WidgetT*> result;

        for ( Widget* child: m_children )
        {
            if (auto casted = dynamic_cast<WidgetT*>(child))
            {
                result.push_back( casted );
            }
        }

        return result;
    }

    template <typename T, typename = void>
    struct is_ui_container : std::false_type {};

    template <typename T>
    struct is_ui_container<T, std::enable_if_t<std::is_base_of_v<Container, T>>> 
        : std::true_type {};

    template <typename T>
    inline constexpr bool is_ui_container_v = is_ui_container<T>::value;
    
    template <typename T, bool = is_ui_container_v<T>>
    struct enable_if_container {
        static_assert(sizeof(T) == 0, 
            "Template parameter must be a valid UI container type."
        );
    };

    template <typename T>
    struct enable_if_container<T, true> { using type = void; };

    template <typename T>
    using enable_if_container_t = typename enable_if_container<T>::type;
};
#pragma once

#include <unordered_map>
#include <cli/ui/elements/widgets/widget.hpp>

namespace ccl::cli::ui
{
    /**
     * Base class for all panels. It is used only for pointer
     * casting and downcasting.
     */
    class PanelBase : public Widget
    {
    protected:
        std::unordered_map<std::string, Widget*> m_childIds;
        bool m_needs_repacking = true;

    public:
        PanelBase( const std::string&, size_t, size_t, size_t, size_t, bool );
        virtual ~PanelBase() = default;

        virtual void addChild( Widget& ) = 0;
        virtual void removeChild( const std::string& ) = 0;
        virtual void pack() = 0;

        /**
         * If the current panel needs repacking. In case the given value
         * is positive, then repacking is applied to parent widget.
         * 
         * @param value True if needs repacking, False otherwise
         */
        void setRepacking( bool );
    };
};
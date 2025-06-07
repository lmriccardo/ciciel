#pragma once

#include <unordered_map>
#include <termio.h>
#include <stdexcept>

namespace ccl::cli::ui
{
    enum class Vertex
    {
        TL, // Top-Left coordinate 
        TR, // Top-Right coordinate
        BL, // Bottom-Left coordinate
        BR  // Bottom-Right coordinate
    };

    // struct 

    /**
     * This abstract class represents any possible object in the UI library. 
     * These types of objects are, for example, any type of widgets (Buttons, 
     * Labels, TextBoxes, and so on) and containers (which are used to manage
     * layout of their children) ... Each of them has its own way to compute 
     * when a generic point (x,y) is colliding with them. 
     * 
     * Each element has its own unique identifier and name
     */
    class UIElement
    {
    protected:
        size_t         m_pos_x;   // Top-left X position
        size_t         m_pos_y;   // Top-left Y position
        struct winsize m_winsize; // The size of the object
    
    public:
        UIElement( size_t, size_t, size_t, size_t );
        ~UIElement() = default;

        std::pair<size_t,size_t> getWinsize() const;
        std::pair<size_t, size_t> getVertexCoord( Vertex ) const;

        void setWinsize( size_t, size_t );
        void setStartPosition( size_t, size_t );
    };
}
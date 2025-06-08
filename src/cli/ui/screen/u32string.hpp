#pragma once

#include <string>
#include <utf8proc.h>
#include <algorithm>
#include <stdexcept>

namespace ccl::cli::ui
{
    /**
     * Converts an input UTF8 char into a char32_t type.
     * 
     * @param utf8_c The input UTF8 char
     * @param utf32_dst The char32_t conversion destination
     * 
     * @return The number of bytes consumed
     */
    ssize_t utf8to32c( const char*, char32_t* ) noexcept;

    /**
     * Converts an input UTF8-encoded string into a u32string, whose elements
     * are expressed not as char but as char32_t, useful when writing to console.
     * 
     * @param content The input UTF8 encoded string
     * @param dest The conversion destination u32string
     * 
     * @return The total dimension of whats read (nof bytes consumed for conversion)
     */
    ssize_t utf8to32( const std::string&, std::u32string& ) noexcept;

    /**
     * Converts an input UTF8-encoded string into a u32string. If error throws.
     * @param content The input UTF8 encoded string
     * @return The converted string
     */
    std::u32string utf8to32( const std::string& );

    /**
     * Converts a char32_t into a UTF8 encoded char and returns the number
     * of bytes read during encoding.
     * 
     * @param c32_in The char32_t that needs to be converted
     * @param dest The conversion destination
     * 
     * @return The number of bytes read during conversion (0 for any error or empty)
     */
    ssize_t utf32to8c( const char32_t*, char* ) noexcept;

    int charwidth( const char32_t* ) noexcept;
    
    int u32swidth( const std::u32string& ) noexcept;
}
#pragma once

#include <string>
#include <utf8proc.h>
#include <algorithm>
#include <stdexcept>

namespace ccl::cli::ui
{
    enum class TextAlignment;

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

    /**
     * Computes the actual dimension in columns of the input wide char.
     * @param c32_in The input wide char
     * @return The number of occupied columns in the terminal
     */
    int charwidth( const char32_t* );
    
    /**
     * Computes the actual dimension in columns of the input u32 string.
     * @param content The u32 string
     * @return The number of occupied columns in the terminal by the string
     */
    int u32swidth( const std::u32string& );

    /**
     * Check if the input char32_t character is a valid codepoint.
     * @param c32_in The codepoint to check
     * @return True if valid, False otherwise
     */
    bool is_codepoint_valid( const char32_t* ) noexcept;

    /**
     * Add horizontal (left and rigth) padding to the input content and
     * returns the padded string.
     * 
     * @param content The string to which padding must be added
     * @param rpad The rigth padding size
     * @param lpad The left padding size
     * 
     * @return The padded string
     */
    std::u32string u32pad( const std::u32string&, size_t, size_t ) noexcept;

    /**
     * Create a new string with the input content aligned according to the
     * TextAlignment parameter on total size.
     * 
     * @param content The string to align
     * @param alignment Text alignment type
     * @param total_len Total dimension of the aligned string
     * 
     * @return A u32 string aligned wrt input parameters
     */
    std::u32string u32align( const std::u32string&, TextAlignment, size_t ) noexcept;
}
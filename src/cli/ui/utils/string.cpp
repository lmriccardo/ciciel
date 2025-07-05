#include "string.hpp"
#include <cli/ui/style/style.hpp>

ssize_t ccl::cli::ui::utf8to32c(const char *utf8_c, char32_t *utf32_dest) noexcept
{
    auto ptr = reinterpret_cast<const utf8proc_uint8_t*>(utf8_c);
    utf8proc_int32_t cp;
    ssize_t len = utf8proc_iterate( ptr, -1, &cp );
    if ( len < 0 ) return -1;
    *utf32_dest = static_cast<char32_t>(cp);
    return len;
}

ssize_t ccl::cli::ui::utf8to32( const std::string& content, std::u32string& dest ) noexcept
{
    const char* c_content = content.c_str();
    ssize_t total_len = 0;
    ssize_t len;

    while ( *c_content )
    {
        char32_t c32;

        if ( (len = utf8to32c( c_content, &c32 )) < 0 )
        {
            return -1;
        }

        dest.push_back( c32 );
        c_content += len;
        total_len += len;
    }

    return total_len;
}

std::u32string ccl::cli::ui::utf8to32( const std::string& content )
{
    std::u32string result;

    if ( utf8to32(content, result) < 0 )
    {
        throw std::runtime_error( "Unable to perform conversion" );
    }

    return result;
}

ssize_t ccl::cli::ui::utf32to8c(const char32_t *c32_in, char *dst) noexcept
{
    auto uc = reinterpret_cast<const utf8proc_int32_t*>(c32_in);
    return utf8proc_encode_char(*uc, reinterpret_cast<utf8proc_uint8_t*>(dst));
}

// int ccl::cli::ui::charwidth(const char32_t *c32_in)
// {
//     auto codepoint = reinterpret_cast<const utf8proc_int32_t *>(c32_in);
//     int dimension; if ((dimension = utf8proc_charwidth( *codepoint )) == 0)
//     {
//         throw std::runtime_error( "Width of wide char returned 0!!" );
//     }

//     return dimension;
// }

// int ccl::cli::ui::u32swidth(const std::u32string &content)
// {
//     int total_len = 0;

//     for ( char32_t c : content )
//     {
//         total_len += charwidth( &c );
//     }
    
//     return total_len;
// }

int ccl::cli::ui::charwidth(const char32_t *c32_in)
{
    if (!c32_in) return 0;

    utf8proc_int32_t codepoint = static_cast<utf8proc_int32_t>(*c32_in);

    // Get the width of the codepoint
    int width = utf8proc_charwidth(codepoint);

    // Return 0 for non-printing or variation selectors
    if (width < 0) return 0;

    return width;
}

int ccl::cli::ui::u32swidth(const std::u32string &content)
{
    if (content.empty()) return 0;

    int total_width = 0;
    size_t len = content.size();

    for (size_t i = 0; i < len;) {
        utf8proc_int32_t current = content[i];
        size_t cluster_start = i;
        ++i;

        while (i < len) {
            utf8proc_int32_t next = content[i];
            int is_break = utf8proc_grapheme_break(current, next);
            if (is_break) break;
            current = next;
            ++i;
        }

        // Call using pointer to first codepoint of cluster
        total_width += charwidth(&content[cluster_start]);
    }

    return total_width;
}

bool ccl::cli::ui::is_codepoint_valid(const char32_t *c32_in) noexcept
{
    auto codepoint = reinterpret_cast<const utf8proc_int32_t*>(c32_in);
    return utf8proc_codepoint_valid( *codepoint );
}

std::u32string ccl::cli::ui::u32pad(
    const std::u32string &content, size_t rpad, size_t lpad
) noexcept {
    return std::u32string( lpad, ' ' ) + content + std::u32string( rpad, ' ' );
}

std::u32string ccl::cli::ui::u32align(const std::u32string &content, 
    TextAlignment alignment, size_t total_len) noexcept
{
    // Compute the actual dimension of the string
    int actual_size = u32swidth( content );

    if ( total_len <=1 || total_len < (size_t)actual_size ) return content;

    if ( alignment == TextAlignment::Center )
    {
        // Compute the padding at each side
        int left_padding  = ( total_len - 1 ) / 2 - (actual_size / 2);
        int rigth_padding = total_len - actual_size - left_padding;

        // Build the final string
        return u32pad( content, rigth_padding, left_padding );
    }

    size_t padsize = static_cast<size_t>(total_len - actual_size);
    if ( alignment == TextAlignment::Left ) return u32pad( content, padsize, 0 );
    return u32pad( content, 0, padsize );
}

void ccl::cli::ui::insert_shift(std::u32string &string, size_t pos, char32_t ch) noexcept
{
    if ( pos >= string.size() ) return;
    
    // Shift characters to the rigth, dropping the last character
    for ( size_t i = string.size(); i > pos; --i )
    {
        string[ i ] = string[ i - 1 ];
    }

    string[ pos ] = ch;
}

void ccl::cli::ui::remove_shift(std::u32string &string, size_t pos, size_t curr_size) noexcept
{
    if ( (int)pos < 0 || string[ pos ] == '\0' ) return;

    // Shift all characters one position to the left
    for ( size_t i = pos; i < curr_size - 1; ++i )
    {
        string[ i ] = string[ i + 1 ];
    }

    string[ curr_size - 1 ] = '\0';
}

void ccl::cli::ui::print_bytes(const std::string &sequence)
{
    for ( unsigned char b: sequence )
    {
        std::cout << "\\x"
                  << std::uppercase
                  << std::hex
                  << std::setw(2)
                  << std::setfill('0')
                  << static_cast<int>(b);
    }

    std::cout << std::endl;
}

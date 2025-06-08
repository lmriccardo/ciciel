#include "u32string.hpp"

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

int ccl::cli::ui::charwidth(const char32_t *c32_in) noexcept
{
    auto codepoint = reinterpret_cast<const utf8proc_int32_t *>(c32_in);
    return utf8proc_charwidth( *codepoint );
}

int ccl::cli::ui::u32swidth(const std::u32string &content) noexcept
{
    int total_len = 0;
    std::for_each( content.begin(), content.end(), 
                   [&]( char32_t c ) 
                   { total_len += charwidth( &c ); } );
    
    return total_len;
}

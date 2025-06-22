#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <filesystem>

namespace ccl::sys::io
{
    /**
     * Read a non-binary file determined by the input path and set its content
     * into the destination string variable.
     * 
     * @param p The path of the file
     * @param dst The destination string with the final content
     * @return The total number of bytes read
     */
    size_t readfile( const std::filesystem::path& p, std::string& dst );
}
#include "file.hpp"

size_t ccl::sys::io::readfile(const std::filesystem::path &p, std::string &dst)
{
    // Check that the input path is not a folder 
    if ( std::filesystem::is_directory( p ) )
    {
        std::cerr << "Input path points to a folder" << std::endl;
        return 0;
    }

    std::ifstream inputFile(p);
    if ( !inputFile.is_open() )
    {
        std::cerr << "Error opening the file" << std::endl;
        return 0;
    }

    // Determine the total number of bytes of the file
    inputFile.seekg( 0, std::ios::end );
    std::streampos fileSize = inputFile.tellg();
    inputFile.seekg( 0, std::ios::beg );

    std::string line;
    while ( std::getline( inputFile, line ) )
    {
        dst += line;
    }

    inputFile.close();
    return fileSize;
}
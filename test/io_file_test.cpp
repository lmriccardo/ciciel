#include <sys/io/file/file_io.hpp>

using namespace ccl::sys::io;

int main()
{
    std::string filePath = "/workspaces/ciciel/test/simple_file.txt";

    FileIO fstream( filePath, iom::Read | iom::Write | iom::App );

    // Read every line in the file
    std::string line;
    while ( fstream.readLine( line ) != 0 )
    {
        std::cout << line;
    }

    std::cout << std::endl;

    // Write a new line
    // std::string newline = "This is a new line from code";
    // fstream.write( newline.c_str(), newline.size() );

    fstream.seekg( 0, iop::Beg );
    for ( auto& line: fstream )
    {
        std::cout << line;
    }

    std::cout << std::endl;
    
    return 0;
}
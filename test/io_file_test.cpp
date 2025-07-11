#include <sys/io/file/file_io.hpp>
#include <sys/io/file/buffered_io.hpp>

using namespace ccl::sys::io;

void simple_file_io( const std::string& filePath )
{
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
}

void buffered_file_io( const std::string& filePath )
{
    BufferedFileIO bfstream( filePath, iom::Read | iom::Write | iom::App );
    bfstream.setBufferCapacity( 5 );

    // Write a number of bytes
    bfstream.write( "\nciao", 5 );
    bfstream.flush();

    bfstream.write("\nCiao mi chiamo Riccardo!", 25);
    bfstream.seekg( -1, iop::End );

    std::string line;
    ssize_t n = bfstream.readLine( line );
    std::cout << "Read " << n << " bytes: " << line << std::endl;

    // Write using the stream << operator
    bfstream << std::string("\nCiao!!!!");
    bfstream << "\nCiao2!!!!";

    std::cout << "\n" << bfstream << std::endl;
}

void create_and_fill( const std::string& filePath )
{
    FileIO fstream( filePath, iom::Read | iom::Write | iom::App );

    std::string newFile = "/workspaces/ciciel/test/simple_file1.txt";
    FileIO nfstream( newFile, iom::Read | iom::Write | iom::Create );

    std::string content;
    nfstream << ( fstream >> content );
}

int main()
{
    std::string filePath = "/workspaces/ciciel/test/simple_file.txt";
    simple_file_io( filePath );
    buffered_file_io( filePath );
    create_and_fill( filePath );
    
    return 0;
}
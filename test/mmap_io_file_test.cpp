#include <io/file/mmap_file_io.hpp>

using namespace ccl::sys::io;

int main()
{
    std::string filePath = "/workspaces/ciciel/test/simple_file.txt";
    iom open_mode = iom::Read | iom::Write | iom::App;
    protf pfl = protf::Read | protf::Write;
    mapf  mfl = mapf::Shared;
    MappedFileIO mfstream( filePath, open_mode, pfl, mfl );

    const char* msg = "\nHey Hey Hey!!!!";
    mfstream.write( msg, strlen(msg) );
    
    return 0;
}
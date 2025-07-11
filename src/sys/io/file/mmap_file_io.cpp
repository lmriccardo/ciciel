#include "mmap_file_io.hpp"

using namespace ccl::sys::io;

#ifndef _WIN32

MappedFileIO::MappedFileIO(const std::string &filePath, iom mode)
    : FileIOBase( filePath, mode )
{
    
}

MappedFileIO::~MappedFileIO()
{
}

#else

// Do Windows implementation

#endif
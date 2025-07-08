#pragma once

#include <fstream>
#include <string>
#include <sys/io/base/stream_io.hpp>

namespace ccl::sys::io
{
    class FileIO : public StreamIO
    {
    protected:
        std::string m_filePath; // The path of the current file
        size_t m_currSize;      // The total current size of the file in bytes

    public:
        FileIO( const std::string& path );
        virtual ~FileIO() = default;

        FileIO( const FileIO& other ) = delete;
        FileIO& operator=( const FileIO& other ) = delete;

        
    };
}
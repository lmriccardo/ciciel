#pragma once

#include <data_structures/buffers/byte_buffer.hpp>
#include "file_io.hpp"

#ifndef _WIN32
#include <sys/mman.h>
#else
// Imports for Windows
#endif

namespace ccl::sys::io
{
    /**
     * @brief
     */
    class MappedFileIO : public FileIOBase
    {
    private:
        ds::buffers::ByteBuffer m_mapBuffer; // The buffer with the file content

    public:
        MappedFileIO( const std::string& filePath, iom mode );
        virtual ~MappedFileIO();
    };
}
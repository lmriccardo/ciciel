#pragma once

#include "file_io.hpp"
#include <data_structures/buffers/byte_buffer.hpp>

namespace ccl::sys::io
{
    /**
     * @brief Buffered version of the File IO Stream
     * 
     * It has an internal buffer to fill with content before being flushed
     * out into the actual file stream. The buffer has a user-defined 
     * capacity, which every time it is exceeded it automatically flushes
     * its content into the file.
     */
    class BufferedFileIO : public FileIO
    {
    private:
        ds::buffers::ByteBuffer m_buffer; // The buffer before flushing
    
    public:
        using FileIO::FileIO; // Use the constructor of the super class
        BufferedFileIO( const std::string& path, iom mode, size_t buff_size );
        virtual ~BufferedFileIO() = default;

        BufferedFileIO( BufferedFileIO&& other );
        BufferedFileIO& operator=( BufferedFileIO&& other );
        
        // Delete the copy constructor, no double ownership of the
        // file handler and buffer ( data consistency ).
        BufferedFileIO( const BufferedFileIO& other ) = delete;
        BufferedFileIO& operator=( const BufferedFileIO& other ) = delete;

        /**
         * Set the internal buffer capacity
         * @param capacity The new capacity of the buffer
         */
        void setBufferCapacity( size_t capacity );

        /**
         * Write the content of the input buffer into the internal buffer.
         * If the size of the input buffer exceeds the max capacity of the
         * internale buffer, it would be flushed out and then filled with
         * the remaining data.
         * 
         * @param buff The buffer to be written
         * @param nbyts The size of the input buffer
         * @return The total number of bytes written
         */
        ssize_t write( const char* buff, size_t nbytes ) override;

        /**
         * Flush the content of the buffer into the file.
         */
        ssize_t flush();
    };
};
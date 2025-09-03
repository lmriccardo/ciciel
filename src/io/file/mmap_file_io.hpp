#pragma once

#include <type_traits>
#include <data_structures/buffers/byte_buffer.hpp>

#include "file_io.hpp"

#ifndef _WIN32
#include <sys/mman.h>
#else
// Imports for Windows
#endif

namespace ccl::sys::io
{

#ifndef _WIN32

    /**
     * @brief Protection flags
     * 
     * These specify how the memory can be accessed.
     */
    enum class protf : unsigned
    {
        None  = 1 << 0, // No access allowed
        Read  = 1 << 1, // Pages can be read
        Write = 1 << 2, // Pages can be written
        Exec  = 1 << 3  // Pages can be executed
    };

    /**
     * @brief Mapping Flags
     * 
     * These define the nature of the mapping.
     */
    enum class mapf : unsigned
    {
        Shared    = 1 << 0, // Updates are written to the underlying file
        Private   = 1 << 1, // Changes are not written back to the file (copy-on-write)
        Fixed     = 1 << 3, // Map at the exact address
        Populate  = 1 << 4  // Prefault pages
    };

    template<> struct enable_bitmask_operator<protf> : std::true_type {};

#else

    /**
     * @brief Protection flags
     * 
     * These specify how the memory can be accessed.
     */
    enum class protf : DWORD
    {
        ReadOnly  = 1 << 0, // Read access only
        ReadWrite = 1 << 1, // Read and write access
        WriteCopy = 1 << 2, // Copy-on-write access
        ExecRead  = 1 << 3, // Read and execute access
        ExecRW    = 1 << 4  // Read, write and execute access
    };

    /**
     * @brief Mapping Flags
     * 
     * These define the nature of the mapping.
     */
    enum class mapf : DWORD
    {
        Read  = 1 << 0, // Read access
        Write = 1 << 1, // Write access
        Exec  = 1 << 2, // Execute access
        All   = 1 << 3  // All access 
    };

#endif

    template<> struct enable_bitmask_operator<mapf> : std::true_type {};

    FLAG_T __make_flags( protf flags );
    FLAG_T __make_flags( mapf flags );

    enum class MSyncPolicy
    {
        SYNC_ON_WRITE,
        SYNC_ON_CLOSE
    };

    /**
     * @brief
     */
    class MappedFileIO : public FileIOBase
    {
    private:
        ds::buffers::ByteBuffer m_mapBuffer; // The buffer with the file content
        protf                   m_protFlags; // Input protection flags
        mapf                    m_mapFlags;  // Input map flags

        size_t m_growFactor  = 2; // The capacity grow factor
        MSyncPolicy m_policy = MSyncPolicy::SYNC_ON_WRITE;

#ifdef _WIN32
        // We need one more argument to use
        HANDLE m_hMap; // The mapping handler returned by Windows
#endif

        void * mmap ( OFF_T fsize, FLAG_T pfl, FLAG_T mfl );
        void   remap( OFF_T size, bool keep_size );

        OFF_T compute_position( OFF_T offset, iop position ) const;
        void  set_position    ( OFF_T position );

        void free();

    public:
        MappedFileIO( const std::string& filePath, iom mode, protf pfl, mapf mfl );
        virtual ~MappedFileIO();

        /**
         * Move the internal buffer pointer to the specified position.
         * The final position is computed as the actual position (iop
         * parameter) + offset. 
         * 
         * This function sets the read pointer.
         * 
         * @param offset The offset
         * @param position The starting position
         */
        void seekg( OFF_T offset, iop position );

        /**
         * Move the internal buffer pointer to the specified position.
         * The final position is computed as the actual position (iop
         * parameter) + offset. 
         * 
         * This function sets the write pointer.
         * 
         * @param offset The offset
         * @param position The starting position
         */
        void seekp( OFF_T offset, iop position );

        /**
         * Read rsize bytes from the file and save them into the input
         * destination char buffer. If the handler (fd) is not valid
         * it returns -1, otherwise the number of bytes actually read.
         * 
         * @param dst The destination buffer ( must have the input capacity )
         * @param rsize The number of bytes to read from the file
         * @return The total number of bytes read
         */
        ssize_t read( char* dst, size_t rsize ) override;

        /**
         * Writes nbytes into the file from the source input buffer.
         * It returns -1 on error (e.g., invalid handler), >= 0 otherwise.
         * 
         * @param src The source buffer
         * @param nbytes The total number of bytes to write
         * @return The total number of written bytes
         */
        virtual ssize_t write( const char* src, size_t nbytes ) override;

        /**
         * Sets the capacity grow factor
         * @param factor The input factor
         */
        void setCapacityGrowFactor( size_t factor );

        /**
         * Set the synchronization policy.
         */
        void setSyncPolicy( MSyncPolicy policy );
    };
}
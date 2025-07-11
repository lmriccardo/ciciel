#pragma once

#include <fstream>
#include <string>
#include <cstring>
#include <sys/io/base/stream_io.hpp>
#include <data_structures/base/iterators.hpp>

#undef FLAG_T
#undef OFF_T

#ifndef _WIN32

#include <sys/stat.h>
#define FLAG_T int
#define OFF_T  off_t

#else

#define FLAG_T DWORD
#define OFF_T  int64_t

#endif

namespace ccl::sys::io
{
    enum class iom : unsigned
    {
        None   = 0,
        Read   = 1 << 0, // open the file in read mode
        Write  = 1 << 1, // open the file in write mode
        App    = 1 << 2, // open the file for appending
        Trunc  = 1 << 3, // open the file and truncate it
        Create = 1 << 4, // create if not exists and open
        Bin    = 1 << 5, // open the file for reading bytes
    };

    /**
     * Bitwise OR between two io open mode
     * @param a The first open mode
     * @param b The second open mode
     * @return The Bitwise OR between a and b
     */
    iom operator|(iom a, iom b);

    /**
     * Overload the bitwise AND operator.
     */
    iom operator&(iom a, iom b);

    /**
     * Checks if the given mode is not None
     */
    bool iom2bool( iom a );

    // Forward declaration for iop class
    enum class iop;

    /**
     * @brief Base abstract class for File Streams.
     * 
     * Generic base class with a number of pure virtual methods that is used
     * as super class for both Windows and Linux (or POSIX) File stream
     * handlers. It is able to manage two different pointers for writing and
     * reading from the file that can be set using the seekp and seekg methods
     * respectively.
     * 
     * For perfect abstraction and generalization all methods that behave
     * differently on Windows and POSIX are keep virtual, while all the others
     * have been directly implemented.
     */
    class FileIOBase : public StreamIO
    {
    public:
        using T = typename std::string;

    protected:
        std::string m_filePath; // The path of the current file
        iom         m_openMode; // The open mode of the current file

        OFF_T m_readIdx  = 0; // The position of the read pointer
        OFF_T m_writeIdx = 0; // The position of the write pointer
    
    public:
        FileIOBase( const std::string& path, iom flags )
            : m_filePath( path ), m_openMode( flags )
        {}

        FileIOBase( FileIOBase&& other );
        FileIOBase& operator=( FileIOBase&& other );

        virtual ~FileIOBase() = default;

        /**
         * Check if the current open file has been opened
         * with the input flag.
         * 
         * @param flag Flag to check
         * @return True or False, depending on the flag and the open mode
         */
        bool hasFlag( iom flag ) const;

        /**
         * Checks if the current file is still open. This actually checks
         * if the underlying stream handler (the file decriptor) is valid.
         * @return True or False
         */
        bool isOpen() const;

        virtual void seekg( OFF_T offset, iop position ) = 0;
        virtual void seekp( OFF_T offset, iop position ) = 0;
        virtual OFF_T tellp() const = 0;
        virtual OFF_T tellg() const = 0;
        virtual OFF_T getSize() const = 0;
        virtual ssize_t readLine( std::string& line ) = 0;

        /**
         * Returns the total number of lines
         */
        size_t getNofLines();
    };

    /**
     * @brief Simple iterator class for File streams.
     * This iterator iterates over the rows of the file.
     */
    class file_iterator : 
        public ds::base::abstract_iterator<FileIOBase::T,
                                           FileIOBase::T*,
                                           FileIOBase::T&,
                                           FileIOBase,
                                           file_iterator>
    {
    private:
        using Base = ds::base::abstract_iterator<FileIOBase::T,FileIOBase::T*,
            FileIOBase::T&,FileIOBase,file_iterator>;

        using typename Base::reference;
        using typename Base::pointer;

        mutable FileIOBase::T m_currLine; // The current line read from the file
        size_t                m_fileSize; // The current size of the file

    public:
        file_iterator( FileIOBase* file_io, size_t size );

        reference operator* () const override;
        pointer   operator->() const override;
    };

    /**
     * @brief The Iterable version of the File Stream class.
     * 
     * It uses the `file_iterator` as returns objecte for `begin`
     * and `end` method. As already said, it iterates for each
     * row in the input file.
     * 
     * Note on thread-safety: It is not thread safe. Once the
     * iterator is created the total number of lines set is fixed.
     * Which means that, if another thread removes a line from the
     * file, the object will continue iterates returning empty lines.
     */
    class FileIOIterable : public FileIOBase
    {
    public:
        using iterator = file_iterator;

        using FileIOBase::FileIOBase;
        virtual ~FileIOIterable() = default;

        FileIOIterable(FileIOIterable&& other)
            : FileIOBase(std::move(other)) 
        {}

        FileIOIterable& operator=(FileIOIterable&& other) {
            FileIOBase::operator=(std::move(other));
            return *this;
        }

        iterator begin();
        iterator end();
    };

#ifndef _WIN32

    enum class iop
    {
        Beg = SEEK_SET,
        Cur = SEEK_CUR,
        End = SEEK_END,
    };

#else

#endif

    FLAG_T __make_flags( iom mode );

    class FileIO : public FileIOIterable
    {
    public:
        FileIO( const std::string& path, iom flags );
        virtual ~FileIO() = default;

        FileIO( const FileIO& other ) = delete;
        FileIO& operator=( const FileIO& other ) = delete;

        FileIO( FileIO&& other )
            : FileIOIterable( std::move(other) )
        {}

        FileIO& operator=( FileIO&& other )
        {
            FileIOIterable::operator=( std::move(other) );
            return *this;
        }
        
        /**
         * Moves the read pointer to the given offset starting from
         * the input position (for Linux it calls lseek).
         * 
         * @param offset The relative offset
         * @param position The actual position from which to compute the offset 
         */
        void seekg( OFF_T offset, iop position ) override;

        /**
         * Moves the write pointer to the given offset starting from
         * the input position (for Linux it calls lseek).
         * 
         * @param offset The relative offset
         * @param position The actual position from which to compute the offset 
         */
        void seekp( OFF_T offset, iop position ) override;

        /**
         * Returns the write pointer 
         */
        OFF_T tellp() const override;

        /**
         * Returns the read pointer
         */
        OFF_T tellg() const override;

        /**
         * Returns the size in bytes of the file
         */
        OFF_T getSize() const override;

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
         * Reads a single line from the file and returns the total number
         * of bytes read. Returns -1 if any error, >= 0 otherwise.
         * 
         * @param line The destination where to save the line
         * @return Total number of bytes read
         */
        ssize_t readLine( std::string& line ) override;

        /**
         * Read the entire file content into the input string.
         * @param dest The destination string for file content
         */
        void read( std::string& dest );

        /**
         * Writes nbytes into the file from the source input buffer.
         * It returns -1 on error (e.g., invalid handler), >= 0 otherwise.
         * 
         * @param src The source buffer
         * @param nbytes The total number of bytes to write
         * @return The total number of written bytes
         */
        virtual ssize_t write( const char* src, size_t nbytes ) override;

        using StreamIO::write; // Take the write with string input

        friend std::ostream& operator<<( std::ostream& ostream, FileIO& stream );
    };

    FileIO&       operator<<( FileIO& stream, const std::string& data );
    FileIO&       operator<<( FileIO& stream, const char* data );
    std::ostream& operator<<( std::ostream& ostream, FileIO& stream );
    
    std::string& operator>>( FileIO& stream, std::string& dest );
}
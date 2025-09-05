#pragma once

#include <ctype.h>
#include <cstring>
#include <memory>
#include <endian.h>
#include <cstdlib>
#include <iostream>

#include <data_structures/base/enum.hpp>

namespace ccl::ds::buffers
{
    class ByteBuffer
    {
    public:
        const static size_t BYTE_SIZE = 1;
        const static size_t SHORT_SIZE = 2;
        const static size_t INT_SIZE = 4;
        const static size_t INT_SIZE_64 = 8;

        ByteBuffer() : m_buffer(nullptr), m_capacity(0), m_position(0), 
            m_size(0), m_order(ByteOrder::LittleEndian) {}

        ByteBuffer(const size_t capacity);
        ByteBuffer(const unsigned char *buffer, const size_t size);
        ByteBuffer(const ByteBuffer &other);
        ByteBuffer(ByteBuffer &&other) noexcept;
        virtual ~ByteBuffer();

        ByteBuffer &operator=(const ByteBuffer &other);
        ByteBuffer &operator=(ByteBuffer &&other) noexcept;

        void allocate(const size_t capacity); // Allocate new capacity
        size_t position() const;              // Returns the current position
        void position(const size_t newpos);   // Sets the new position

        void setByteOrder(const ByteOrder &order); // Set the byte order
        void setBuffer( unsigned char* buffer, size_t size, bool kee_size ); // Set the current buffer

        const ByteOrder &getByteOrder() const;     // Returns current byte order
        size_t getBufferCapacity() const;          // Returns the buffer capacity
        size_t getBufferSize() const;              // Returns the current buffer size
        size_t getRemainingCapacity() const;       // Returns the remaining capacity
        size_t getRemainingSize() const;           // Returns the remaining size (from position)

        bool isEmpty() const; // If the buffer is empty or not
        void clear();         // Clear the entire buffer

        void put(const unsigned char data);               // Put a single byte into the buffer
        void putUnsignedShort(const unsigned short data); // Put a single short into the buffer
        void putUnsignedInt(const unsigned int data);     // Put a single integer 32 into the buffer
        void putUnsignedLong(const uint64_t data);        // Put a single integer 64 into the buffer

        // Copy the input buffer into the class buffer if possible
        void putBuffer(const unsigned char *buff, const size_t start, const size_t size);
        void putBuffer(const unsigned char *buff, const size_t size);

        unsigned char get();                                  // Returns a single byte from the buffer
        unsigned short getUnsignedShort();                    // Returns a single short from the buffer
        unsigned int getUnsignedInt();                        // Returns a single int from the buffer
        unsigned long getUnsignedLong();                      // Returns a single long from the buffer
        unsigned char get(const size_t pos_in);               // Returns a single byte from the position
        unsigned short getUnsignedShort(const size_t pos_in); // Returns a single short from position
        unsigned int getUnsignedInt(const size_t pos_in);     // Returns a single int from position
        unsigned long getUnsignedLong(const size_t pos_in);   // Returns a single long from position

        short getShort();                    // Returns a signed short from the buffer
        int getInt();                        // Returns a signed int from the buffer
        long getLong();                      // Returns a signed long from the buffer
        short getShort(const size_t pos_in); // Returns a singed short from position
        int getInt(const size_t pos_in);     // Returns a singed int from position
        long getLong(const size_t pos_in);   // Returns a signed long from position

        // Copy the content of the buffer into the destination
        void getBuffer(unsigned char *dst, const size_t start, const size_t size);
        void getBuffer(unsigned char *dst, const size_t size);

        void getBuffer(unsigned char *dst); // Copy the entire buffer
        unsigned char *getBuffer() const;   // Returns the actual pointer to the buffer

    private:
        void checkForOutOfBound(const size_t curr_pos, bool on_size);
        void errorIfEmpty();
        void errorIfNegative(const size_t value);

        unsigned char *m_buffer;   // The actual byte buffer
        size_t         m_capacity; // The maximum capacity of the buffer
        size_t         m_position; // The current position into the buffer
        size_t         m_size;     // The total number of elements into the buffer
        ByteOrder      m_order;    // The byte order (host or network-order)

        bool m_has_ownership = true; // It has the ownership of the buffer
    };

    typedef std::shared_ptr<ByteBuffer> ByteBuffer_ptr;
    typedef std::unique_ptr<ByteBuffer> ByteBuffer_uptr;
}
#include "byte_buffer.hpp"

using namespace ccl::ds::buffers;

ByteBuffer::ByteBuffer(const size_t capacity)
    : m_position(0), m_size(0), m_order(ByteOrder::LittleEndian)
{
    if (capacity == 0)
    {
        std::cerr << "[ByteBuffer:Error] Cannot create a ByteBuffer with 0 capacity"
                  << std::endl;

        throw std::invalid_argument("Error");
    }

    if (static_cast<int>(capacity) < 0)
    {
        std::cerr << "[ByteBuffer:Error] Cannot create a ByteBuffer with negative capacity"
                  << std::endl;

        throw std::invalid_argument("Error");
    }

    m_capacity = capacity;
    m_buffer = new unsigned char[capacity];
    memset(m_buffer, 0, capacity * sizeof(unsigned char));
}

ByteBuffer::ByteBuffer(const unsigned char *buffer, const size_t size)
    : ByteBuffer(size)
{
    m_size = size;

    // Copy the input buffer into the ByteBuffer buffer
    memcpy(m_buffer, buffer, size * sizeof(unsigned char));
}

ByteBuffer::ByteBuffer(const ByteBuffer &other)
{
    // Copy initial simple values
    m_capacity = other.m_capacity;
    m_position = other.m_position;
    m_order = other.m_order;
    m_size = other.m_size;

    // Copy the buffer
    m_buffer = new unsigned char[m_capacity];
    memcpy(m_buffer, other.m_buffer, m_capacity * sizeof(unsigned char));
}

ByteBuffer::ByteBuffer(ByteBuffer &&other) noexcept
{
    // Copy initial simple values
    m_capacity = other.m_capacity;
    m_position = other.m_position;
    m_order = other.m_order;
    m_size = other.m_size;

    // Copy the buffer
    m_buffer = other.m_buffer;

    // Reset the other buffer
    other.m_buffer = nullptr;
    other.m_capacity = 0;
    other.m_position = 0;
    other.m_size = 0;
}

ByteBuffer::~ByteBuffer()
{
    delete[] m_buffer;
}

ByteBuffer &ByteBuffer::operator=(const ByteBuffer &other)
{
    // We need to delete the previous buffer
    if (m_buffer != nullptr) delete[] m_buffer;

    // Copy initial simple values
    m_capacity = other.m_capacity;
    m_position = other.m_position;
    m_order = other.m_order;
    m_size = other.m_size;

    // Copy the buffer
    m_buffer = new unsigned char[m_capacity];
    memcpy(m_buffer, other.m_buffer, m_capacity * sizeof(unsigned char));

    return *this;
}

ByteBuffer &ByteBuffer::operator=(ByteBuffer &&other) noexcept
{
    // We need to delete the previous buffer
    if (m_buffer != nullptr) delete[] m_buffer;

    // Copy initial simple values
    m_capacity = other.m_capacity;
    m_position = other.m_position;
    m_order = other.m_order;
    m_size = other.m_size;

    // Copy the buffer
    m_buffer = other.m_buffer;

    // Reset the other buffer
    other.m_buffer = nullptr;
    other.m_capacity = 0;
    other.m_position = 0;
    other.m_size = 0;

    return *this;
}

void ByteBuffer::allocate(const size_t capacity)
{
    if (m_capacity < capacity)
    {
        // We need to resize the memory location
        unsigned char* tmpbuffer = new unsigned char[capacity];

        // If the buffer is not empty we need to copy the current
        // content into the new buffer and then free the memory
        if (!(m_buffer == nullptr))
        {
            memcpy(tmpbuffer, m_buffer, m_capacity*sizeof(unsigned char));
            delete[] m_buffer;
        }

        m_buffer = tmpbuffer;
        m_capacity = capacity;
    }
}

size_t ByteBuffer::position() const
{
    return m_position;
}

void ByteBuffer::position(const size_t newpos)
{
    // Check if the new position is inside the range
    m_position = newpos;
}

void ByteBuffer::setByteOrder(const ByteOrder &order)
{
    m_order = order;
}

const ccl::ds::ByteOrder &ByteBuffer::getByteOrder() const
{
    return m_order;
}

size_t ByteBuffer::getBufferCapacity() const
{
    return m_capacity;
}

size_t ByteBuffer::getBufferSize() const
{
    return m_size;
}

size_t ByteBuffer::getRemainingCapacity() const
{
    return m_capacity - m_size;
}

size_t ByteBuffer::getRemainingSize() const
{
    return m_size - m_position;
}

bool ByteBuffer::isEmpty() const
{
    return m_size == 0;
}

void ByteBuffer::clear()
{
    memset(m_buffer, 0, m_capacity*sizeof(unsigned char));
    m_position = 0;
    m_size = 0;
}

void ByteBuffer::put(const unsigned char data)
{
    checkForOutOfBound(position() + BYTE_SIZE, false);
    m_buffer[m_position++] = data;
    m_size++;
}

void ByteBuffer::putUnsignedShort(const unsigned short data)
{
    checkForOutOfBound(position() + SHORT_SIZE, false);

    // Convert into big endian or little endian if necessary
    unsigned short x = data;
    if (m_order == ByteOrder::BigEndian) x = htobe16(data);
    if (m_order == ByteOrder::LittleEndian) x = htole16(data);

    // Put it into the buffer
    memcpy(m_buffer + m_position, &x, sizeof(unsigned short));
    m_position += SHORT_SIZE;
    m_size = m_position;
}

void ByteBuffer::putUnsignedInt(const unsigned int data)
{
    checkForOutOfBound(position() + INT_SIZE, false);

    // Convert into big endian or little endian if necessary
    unsigned int x = data;
    if (m_order == ByteOrder::BigEndian) x = htobe32(data);
    if (m_order == ByteOrder::LittleEndian) x = htole32(data);

    // Put it into the buffer
    memcpy(m_buffer + m_position, &x, sizeof(unsigned int));
    m_position += INT_SIZE;
    m_size = m_position;
}

void ByteBuffer::putUnsignedLong(const uint64_t data)
{
    checkForOutOfBound(position() + INT_SIZE_64, false);

    // Convert into big endian or little endian if necessary
    uint64_t x = data;
    if (m_order == ByteOrder::BigEndian) x = htobe64(data);
    if (m_order == ByteOrder::LittleEndian) x = htole64(data);

    // Put it into the buffer
    memcpy(m_buffer + m_position, &x, sizeof(uint64_t));
    m_position += INT_SIZE_64;
    m_size = m_position;
}

void ByteBuffer::putBuffer(const unsigned char *buff, const size_t start, const size_t size)
{
    position(start); // Change the current position to start
    checkForOutOfBound(position() + size, false);
    memcpy(m_buffer + m_position, buff, size * sizeof(unsigned char));
    position(start + size);
    m_size = (m_position > m_size) ? m_position : m_size;
}

void ByteBuffer::putBuffer(const unsigned char *buff, const size_t size)
{
    putBuffer(buff, position(), size);
}

unsigned char ByteBuffer::get()
{
    unsigned char x = get(position());
    m_position++;
    return x;
}

unsigned short ByteBuffer::getUnsignedShort()
{
    unsigned short x = getUnsignedShort(position());
    m_position += SHORT_SIZE;
    return x;
}

short ByteBuffer::getShort()
{
    return static_cast<short>(getUnsignedShort());
}

unsigned int ByteBuffer::getUnsignedInt()
{
    unsigned int x = getUnsignedInt(position());
    m_position += INT_SIZE;
    return x;
}

int ByteBuffer::getInt()
{
    return static_cast<int>(getUnsignedInt());
}

unsigned long ByteBuffer::getUnsignedLong()
{
    unsigned long x = getUnsignedLong(position());
    m_position += INT_SIZE_64;
    return x;
}

unsigned char ByteBuffer::get(const size_t pos_in)
{
    errorIfEmpty();
    errorIfNegative(pos_in);
    checkForOutOfBound(pos_in + BYTE_SIZE, true);

    unsigned char x = m_buffer[pos_in];

    return x;
}

unsigned short ByteBuffer::getUnsignedShort(const size_t pos_in)
{
    errorIfEmpty();
    errorIfNegative(pos_in);
    checkForOutOfBound(pos_in + SHORT_SIZE, true);

    // Get the element from the buffer
    unsigned short x;
    memcpy(&x, m_buffer + pos_in, sizeof(unsigned short));

    // Converts if necessary
    if (m_order == ByteOrder::BigEndian) x = be16toh(x);
    if (m_order == ByteOrder::LittleEndian) x = le16toh(x);

    return x;
}

unsigned int ByteBuffer::getUnsignedInt(const size_t pos_in)
{
    errorIfEmpty();
    errorIfNegative(pos_in);
    checkForOutOfBound(pos_in + INT_SIZE, true);

    // Get the element from the buffer
    unsigned int x;
    memcpy(&x, m_buffer + pos_in, sizeof(unsigned int));

    // Converts if necessary
    if (m_order == ByteOrder::BigEndian) x = be32toh(x);
    if (m_order == ByteOrder::LittleEndian) x = le32toh(x);

    return x;
}

unsigned long ByteBuffer::getUnsignedLong(const size_t pos_in)
{
    errorIfEmpty();
    errorIfNegative(pos_in);
    checkForOutOfBound(pos_in + INT_SIZE_64, true);

    // Get the element from the buffer
    unsigned long x;
    memcpy(&x, m_buffer + pos_in, sizeof(unsigned long));

    // Converts if necessary
    if (m_order == ByteOrder::BigEndian) x = be64toh(x);
    if (m_order == ByteOrder::LittleEndian) x = le64toh(x);

    return x;
}

long ByteBuffer::getLong()
{
    return static_cast<long>(getUnsignedLong());
}

short ByteBuffer::getShort(const size_t pos_in)
{
    return static_cast<short>(getUnsignedShort(pos_in));
}

int ByteBuffer::getInt(const size_t pos_in)
{
    return static_cast<int>(getUnsignedInt(pos_in));
}

long ByteBuffer::getLong(const size_t pos_in)
{
    return static_cast<long>(getUnsignedLong(pos_in));
}

void ByteBuffer::getBuffer(unsigned char *dst, const size_t start, const size_t size)
{
    errorIfEmpty();
    checkForOutOfBound(start + size, true);
    memcpy(dst, m_buffer + start, size * sizeof(unsigned char));
}

void ByteBuffer::getBuffer(unsigned char *dst, const size_t size)
{
    getBuffer(dst, position(), size);
    m_position += size;
}

void ByteBuffer::getBuffer(unsigned char *dst)
{
    getBuffer(dst, 0, m_capacity);
}

unsigned char *ByteBuffer::getBuffer() const
{
    return m_buffer;
}

void ByteBuffer::checkForOutOfBound(const size_t curr_pos, bool on_size)
{
    // If the current position is beyond the capacity error
    if (curr_pos > m_capacity)
    {
        std::cerr << "[ByteBuffer:Error] Current position " << curr_pos
                  << " is beyond buffer capacity of " << m_capacity
                  << std::endl;

        throw std::overflow_error("Error");
    }

    // Otherwise, if the current position is beyond the actual size
    if (curr_pos > m_size && on_size)
    {
        std::cout << "[ByteBuffer:Warning] Current position " << curr_pos
                  << " is beyond the actual size of " << m_size
                  << ". Undefined behaviour."
                  << std::endl;
    }
}

void ByteBuffer::errorIfEmpty()
{
    if (isEmpty())
    {
        std::cerr << "[ByteBuffer:Error] Trying to access an empty"
                  << " array of bytes. This lead to undefined behavior"
                  << std::endl;

        throw std::runtime_error("Error");
    }
}

void ByteBuffer::errorIfNegative(const size_t value)
{
    if (static_cast<int>(value) < 0)
    {
        std::cerr << "[ByteBuffer:Error] Value " << value
                  << " cannot be negative"
                  << std::endl;

        throw std::invalid_argument("Error");
    }
}
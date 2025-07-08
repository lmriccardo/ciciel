#include "stream_io.hpp"

using namespace ccl::sys::io;

StreamIO::StreamIO(NativeHandleType handle)
    : m_handler(handle)
{
    if (!m_handler.isValid())
    {
        throw std::runtime_error("Failed to create StreamIO with invalid handle.");
    }
}

StreamIO::StreamIO(StreamHandler<NativeHandleType>&& handler)
    : m_handler(std::move(handler))
{}

StreamIO::StreamIO(StreamIO&& other)
    : m_handler(std::move(other.m_handler))
{}

StreamIO& StreamIO::operator=(StreamIO&& other)
{
    if (this != &other)
    {
        m_handler = std::move(other.m_handler);
    }
    return *this;
}

NativeHandleType StreamIO::getNativeHandle() const
{
    return m_handler.get();
}

bool StreamIO::isOpen() const
{
    return m_handler.isValid();
}

void StreamIO::close()
{
    if (m_handler.isValid())
    {
        m_handler.reset();
    }
}
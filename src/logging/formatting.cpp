#include "formatting.hpp"

using namespace ccl::logging;

std::string DefaultStringFormatter::format(LoggingLevel lvl, const std::string &msg)
{
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t( now );
    std::tm tm {};

#ifdef _WIN32
    localtime_s( &tm, &now_c );
#else
    localtime_r( &now_c, &tm );
#endif

    std::string logging_lvl;
    switch (lvl)
    {
        case LoggingLevel::INFO: logging_lvl = "INFO"; break;
        case LoggingLevel::WARNING: logging_lvl = "WARNING"; break;
        case LoggingLevel::ERROR: logging_lvl = "ERROR"; break;
        default: break;
    }

    std::ostringstream oss;
    oss << "[" << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << "] "
        << "[" << logging_lvl << "] " << msg
        << std::endl;

    return oss.str();
}

// ------------------------------------------------------------------------
// SYS LOG FORMAT CLASS
// ------------------------------------------------------------------------

SD_Element::SD_Element(const std::string &sdid)
: m_sdid( sdid )
{}

SD_Element::SD_Element(const SD_Element &other)
: m_sdid( other.m_sdid ), m_sd_params( other.m_sd_params )
{}

SD_Element::SD_Element(SD_Element &&other)
: m_sdid( std::move( other.m_sdid ) ),
  m_sd_params( std::move( other.m_sd_params ) )
{}

SD_Element &SD_Element::operator=(const SD_Element &other)
{
    if ( this != &other )
    {
        m_sdid = other.m_sdid;
        m_sd_params = other.m_sd_params;
    }

    return *this;
}

SD_Element &SD_Element::operator=(SD_Element &&other)
{
    m_sdid = std::move( other.m_sdid );
    m_sd_params = std::move( other.m_sd_params );
    return *this;
}

bool SD_Element::isEmpty() const
{
    return m_sd_params.empty();
}

void SD_Element::addParameter(const std::string &k, const std::string &v)
{
    // Here, we should check for known SD Parameters and SD Elements
    if ( 
           ( m_sdid == "timeQuality" && k != "tzKnown" && k != "isSynced" && k != "syncAccuracy" )
        || ( m_sdid == "origin"      && k != "ip" && k != "enterpriseId" 
                                     && k != "software" && k != "swVersion" )
        || ( m_sdid == "meta"        && k != "sequenceId" && k != "sysUpTime"&& k != "language")
    ) {
        throw std::runtime_error( "[Error] Known SD_ELEMENTS should have known SD_PARAM" );
    }

    m_sd_params.insert_or_assign( k, v );
}

int ccl::logging::SD_Element::getValue(const std::string &k, std::string &out) const
{
    if ( m_sd_params.find( k ) != m_sd_params.end() )
    {
        out = m_sd_params.at( k );
        return 0;
    }

    return -1;
}

std::string SD_Element::format() const
{
    // Returns an empty string if the SD Element has no parameters
    if ( isEmpty() ) return "";

    std::stringstream sd_elements_str;
    sd_elements_str << "[" << m_sdid;

    for ( const auto& [key,value]: m_sd_params )
    {
        sd_elements_str << " " << key << "=\"" << value << "\"";
    }

    sd_elements_str << "]";
    return sd_elements_str.str();
}

SD_Element::cs_ptr SD_Element::toShared() const
{
    return shared_from_this();
}

void SysLogFormatter::addElement( const std::string& sdid )
{
    if ( m_sd_elements.find( sdid ) != m_sd_elements.end() )
    {
        return;
    }

    m_sd_elements.insert( { sdid, std::make_shared<SD_Element>(sdid) } );
}

SD_Element::w_ptr SysLogFormatter::getElement(const std::string &sdid) const
{
    auto it = m_sd_elements.find( sdid );
    if ( it != m_sd_elements.end() )
    {
        return SD_Element::w_ptr( it->second );
    }

    return SD_Element::w_ptr();
}

int SysLogFormatter::addParameter(
    const std::string &sdid, const std::string &k, const std::string &v
) {
    auto w_elem = getElement( sdid );
    if ( auto elem = w_elem.lock() )
    {
        try
        {
            elem->addParameter( k, v );
            return 0;
        }
        catch(const std::runtime_error& e)
        {
            std::cerr << e.what() << '\n';
        }

        return -1;
    } 
    else 
    {
        return -1;
    }
}

std::string SysLogFormatter::format(SysLogFacility f, SysLogSeverity s, const std::string &msg)
{
    return std::string();
}

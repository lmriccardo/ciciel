#pragma once

#include <string>
#include <chrono>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <vector>
#include <unordered_map>
#include <stdexcept>
#include <memory>
#include <iostream>
#include "enum.hpp"

namespace ccl::logging
{
    class StringFormatterInterface
    {
    public:
        virtual std::string format( LoggingLevel lvl, const std::string& msg ) = 0;
    };

    /**
     * @brief The default string formatter
     * 
     * Format the logging string as: [<date-time>][<level>] <message>
     */
    class DefaultStringFormatter : public StringFormatterInterface
    {
    public:
        std::string format( LoggingLevel lvl, const std::string& msg );
    };

    // ------------------------------------------------------------------------
    // SYS LOG FORMAT CLASS
    // ------------------------------------------------------------------------

    /**
     * @brief Structured Data Element
     * 
     * A Structured-Data Element is a collection of metadata called SD_PARAM
     * (a key-value pair string to string), which is used to provide more
     * information to the logged message. In general, STRUCTURED-DATA provides
     * a mechanism to express information in a well-defined, easily parsable
     * and interpretable data format. SD_Element is a single element in the
     * STRUCTURED-DATA field of the SysLog Logging message.
     * 
     * Reference: [The SysLog Protocol](https://datatracker.ietf.org/doc/html/rfc5424)
     */
    class SD_Element : public std::enable_shared_from_this<SD_Element>
    {
    private:
        std::string m_sdid;
        std::unordered_map<std::string, std::string> m_sd_params;

    public:
        using s_ptr  = typename std::shared_ptr<SD_Element>;
        using cs_ptr = typename std::shared_ptr<const SD_Element>;
        using w_ptr  = typename std::weak_ptr<SD_Element>;

        SD_Element( const std::string& sdid );
        SD_Element( const SD_Element& other );
        SD_Element( SD_Element&& other);

        SD_Element& operator=( const SD_Element& other );
        SD_Element& operator=( SD_Element&& other );

        cs_ptr toShared() const;

        bool isEmpty() const;
        void addParameter( const std::string& k, const std::string& v );

        /**
         * Get the corresponding value of the input key if it exists.
         * Returns 0 on success, and -1 on failure.
         */
        int getValue( const std::string& k, std::string& out ) const;
        
        /**
         * Format the SD Element into the suitable format for SysLog.
         * Returns a string.
         */
        std::string format() const;
    };

    class SysLogFormatter
    {
    private:
        const int m_version = 1; // Version of the SYSLOG message format

        std::unordered_map<std::string, SD_Element::s_ptr> m_sd_elements;
        
    public:
        void addElement( const std::string& sdid );
        SD_Element::w_ptr getElement( const std::string& sdid ) const;

        /**
         * Adds the corresponding SD Parameter (k, v) in the given SD elements if
         * it exists in the formatter. On success, 0 is returned, otherwise -1.
         */
        int addParameter( const std::string& sdid, const std::string& k, const std::string& v );

        std::string format( SysLogFacility f, SysLogSeverity s, const std::string& msg );
    };
}
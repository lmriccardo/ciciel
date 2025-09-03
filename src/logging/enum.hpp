#pragma once

namespace ccl::logging
{
    enum class LoggingLevel
    {
        INFO,       // Lowest logging level, usually for debug
        WARNING,    // Something might not be went as expected
        ERROR       // Highest logging level, an error occurred
    };

    enum class SysLogFacility
    {
        KERNEL           = 0,  // kernel messages
        USER_LEVEL       = 1,  // user-level messages
        MAIL_SYSTEM      = 2,  // mail system
        SYS_DAEMONS      = 3,  // system daemons
        SECURITY         = 4,  // security/authorization messages
        SYSLOGD_INTERNAL = 5,  // messages generated internally by syslogd
        LINE_PRINTER     = 6,  // line printer subsystem
        NETWORK_NEWS     = 7,  // network news subsystem
        UUCP_SUBSYSTEM   = 8,  // UUCP subsystem
        CLOCK_DAEMON     = 9,  // clock daemon
        FTP_DAEMON       = 11, // FTP daemon
        NTP_SUBSYSTEM    = 12, // NTP subsystem
        LOG_AUDIT        = 13, // log audit
        LOG_ALERT        = 14, // log alert
        LOCAL_USE_0      = 16, // local use 0  (local0)
        LOCAL_USE_1      = 17, // local use 1  (local1)
        LOCAL_USE_2      = 18, // local use 2  (local2)
        LOCAL_USE_3      = 19, // local use 3  (local3)
        LOCAL_USE_4      = 20, // local use 4  (local4)
        LOCAL_USE_5      = 21, // local use 5  (local5)
        LOCAL_USE_6      = 22, // local use 6  (local6)
        LOCAL_USE_7      = 23, // local use 7  (local7)
    };

    enum class SysLogSeverity
    {
        EMERGENCY,     // System is unusable
        ALERT,         // Action must be taken immediately
        CRITICAL,      // Critical Conditions
        ERROR,         // Error Conditions
        WARNING,       // Warning Conditions
        NOTICE,        // Normal but significant condition
        INFORMATIONAL, // Informational messages
        DEBUG          // Debug-level messages
    };
}
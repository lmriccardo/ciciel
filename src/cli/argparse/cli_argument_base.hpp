#pragma once

#include <string>
#include <stdio.h>
#include <sstream>
#include <algorithm>
#include <memory>

namespace ccl::cli::argparse
{
    /**
     * This is the base class for all Command-line Arguments.
     * It cannot be instantiated since it does not contain the main working
     * logic for actual cli parsable arguments. All derivative classe HAVE TO
     * implement virtual methods in order to work.
     */
    class CliArgumentBase
    {
    protected:
        std::string m_full_name;  // The full name of the argument
        std::string m_short_name; // A short version of the argument name (if any)
        std::string m_help_str;   // A small description of the parameter

        bool m_has_value = false; // If the argument has also the value (not just the default one)
        bool m_required  = false; // If the parameter is required or not

    public:
        CliArgumentBase( const std::string& name );
        CliArgumentBase( const std::string& name, const std::string& short_name );
        virtual ~CliArgumentBase() = default;

        void setRequired( bool value );
        void setDescription( const std::string& description );

        std::string getArgumentName_upper();

        const std::string& getName() const;
        const std::string& getShortName() const;
        const std::string& getDescription() const;
        
        bool hasShortName() const;
        bool hasValue() const;
        bool isRequired() const;

        virtual std::string getPatternMatch() const = 0;
        virtual void setValue( const std::string& value ) = 0;
        virtual void clean() = 0;
    };

    typedef std::unique_ptr<CliArgumentBase> CliArgumentBase_uptr;
}
#pragma once

#include "cli_argument_base.hpp"

namespace ccl::cli::argparse
{
    template <typename T>
    class TemplateCliArgument : public CliArgumentBase
    {
    protected:
        T m_default_value; // The default value of the parameter
        T m_value;         // The final value

    public:
        TemplateCliArgument( const std::string& name, const std::string& short_name );
        TemplateCliArgument( const std::string& name );
        virtual ~TemplateCliArgument() = default;

        void setDefaultValue( const T& default_value );
        T getValue() const;
        void clean() override;

        virtual std::string getPatternMatch() const;
    };

    template <typename T>
    inline TemplateCliArgument<T>::TemplateCliArgument(
        const std::string &name, const std::string &short_name)
        : CliArgumentBase(name, short_name)
    {
    }

    template <typename T>
    inline TemplateCliArgument<T>::TemplateCliArgument(const std::string &name)
        : CliArgumentBase(name)
    {

    }

    template <typename T>
    inline void TemplateCliArgument<T>::setDefaultValue(const T &default_value)
    {
        m_default_value = default_value;
    }

    template <typename T>
    inline T TemplateCliArgument<T>::getValue() const
    {
        if (!hasValue()) return m_default_value;
        return m_value;
    }

    template <typename T>
    inline void TemplateCliArgument<T>::clean()
    {
        m_has_value = false;
    }

    template <typename T>
    inline std::string TemplateCliArgument<T>::getPatternMatch() const
    {
        if (isRequired()) return "";

        std::string pattern;
        pattern = "--" + getName();
        
        // If the argument does not have the short name
        if (!hasShortName()) return "(" + pattern + ")";

        // If the argument has also the short name we need to include it
        return "(-" + getShortName() + "|" + pattern + ")";
    }
}
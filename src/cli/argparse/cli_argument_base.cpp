#include "cli_argument_base.hpp"

using namespace ccl::cli::argparse;

CliArgumentBase::CliArgumentBase(const std::string &name, const std::string &short_name)
    : m_full_name( name ), m_short_name( short_name )
{
}

CliArgumentBase::CliArgumentBase(const std::string &name)
    : CliArgumentBase( name, "" )
{
}

void CliArgumentBase::setRequired(bool value)
{
    m_required = value;
}

void CliArgumentBase::setDescription(const std::string &description)
{
    m_help_str = description;
}

const std::string &CliArgumentBase::getName() const
{
    return m_full_name;
}

const std::string &CliArgumentBase::getShortName() const
{
    return m_short_name;
}

const std::string &CliArgumentBase::getDescription() const
{
    return m_help_str;
}

bool CliArgumentBase::hasShortName() const
{
    return m_short_name.size() > 0;
}

bool CliArgumentBase::hasValue() const
{
    return m_has_value;
}

bool CliArgumentBase::isRequired() const
{
    return m_required;
}

std::string CliArgumentBase::getArgumentName_upper()
{
    std::string name_upper;
    std::transform( m_full_name.begin(), m_full_name.end(), name_upper.begin(),
                    [](unsigned char c) { return std::toupper(c); } );

    return name_upper;
}

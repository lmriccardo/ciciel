#include "cli_arguments.hpp"

using namespace ccl::cli::argparse;

BooleanArgument::BooleanArgument(const std::string &name)
    : TemplateCliArgument<bool>(name)
{
}

BooleanArgument::BooleanArgument(const std::string &name, const std::string &sname)
    : TemplateCliArgument<bool>(name, sname)
{
}

std::string BooleanArgument::getPatternMatch() const
{
    std::string pattern = TemplateCliArgument<bool>::getPatternMatch();
    return pattern + "?";
}

void BooleanArgument::setValue(const std::string &value)
{
    m_value = (value.compare("true") == 0);
    m_has_value = true;
}

void BooleanArgument::setValue()
{
    if (!m_has_value) m_value = !m_default_value;
    m_has_value = true;
}

IntegerArgument::IntegerArgument(const std::string &name)
    : TemplateCliArgument<int>(name)
{
}

IntegerArgument::IntegerArgument(const std::string &name, const std::string &sname)
    : TemplateCliArgument<int>(name, sname)
{
}

std::string IntegerArgument::getPatternMatch() const
{
    std::string pattern = TemplateCliArgument<int>::getPatternMatch();
    pattern = pattern + "(?:=|\\s+)(\\d+)";
    return "(?:" + pattern + ")?";

    // return "(\\d+)";
}

void IntegerArgument::setValue(const std::string& value)
{
    m_value = std::stoi(value);
    m_has_value = true;
}

ccl::cli::argparse::StringArgument::StringArgument(const std::string &name)
    : TemplateCliArgument<std::string>(name)
{
}

ccl::cli::argparse::StringArgument::StringArgument(const std::string &name, const std::string &sname)
    : TemplateCliArgument<std::string>(name, sname)
{
}

std::string StringArgument::getPatternMatch() const
{
    std::string pattern = TemplateCliArgument<std::string>::getPatternMatch();
    pattern = pattern + "(?:=|\\s+)([\\w\\./_\\\\-]+)";
    return "(?:" + pattern + ")?";

    // return "([\\w\\./_\\\\-]+)";
}

void StringArgument::setValue(const std::string& value)
{
    m_value = value;
    m_has_value = true;
}

ccl::cli::argparse::DoubleArgument::DoubleArgument(const std::string &name)
    : TemplateCliArgument<double>(name)
{
}

ccl::cli::argparse::DoubleArgument::DoubleArgument(const std::string &name, const std::string &sname)
    : TemplateCliArgument<double>(name, sname)
{
}

std::string DoubleArgument::getPatternMatch() const
{
    std::string pattern = TemplateCliArgument<double>::getPatternMatch();
    pattern = pattern + "(?:=|\\s+)(\\d+\\.\\d+)";
    return "(?:" + pattern + ")?";

    // return "(\\d+\\.{1}\\d+)";
}

void DoubleArgument::setValue(const std::string& value) 
{
    m_value = std::stod(value);
    m_has_value = true;
}
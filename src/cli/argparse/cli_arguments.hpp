#pragma once

#include "cli_argument_template.hpp"

namespace ccl::cli::argparse
{
    class BooleanArgument : public TemplateCliArgument<bool>
    {
    public:
        BooleanArgument(const std::string &name);
        BooleanArgument(const std::string &name, const std::string &sname);
        virtual ~BooleanArgument() = default;

        std::string getPatternMatch() const override;
        void setValue( const std::string& value ) override;
        void setValue();
    };

    class IntegerArgument : public TemplateCliArgument<int>
    {
    public:
        IntegerArgument(const std::string &name);
        IntegerArgument(const std::string &name, const std::string &sname);
        virtual ~IntegerArgument() = default;

        std::string getPatternMatch() const override;
        void setValue( const std::string& value ) override;
    };

    class StringArgument : public TemplateCliArgument<std::string>
    {
    public:
        StringArgument(const std::string &name);
        StringArgument(const std::string &name, const std::string &sname);
        virtual ~StringArgument() = default;

        std::string getPatternMatch() const override;
        void setValue( const std::string& value ) override;
    };

    class DoubleArgument : public TemplateCliArgument<double>
    {
    public:
        DoubleArgument(const std::string &name);
        DoubleArgument(const std::string &name, const std::string &sname);
        virtual ~DoubleArgument() = default;

        std::string getPatternMatch() const override;
        void setValue(const std::string& value) override;
    };
}
#pragma once

#include <vector>
#include <unordered_map>
#include <regex>
#include <memory>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <type_traits>
#include <stdexcept>
#include "cli_arguments.hpp"

namespace ccl::cli::argparse
{
    struct ParserArgument_t
    {
        std::string name;       // The name of the argument
        std::string short_name; // The short name of the argument
        std::string help_str;   // The help string of the argument
        bool required;          // If the argument is required or not
    };

    class ArgumentParser
    {
    private:
        enum 
        {
            BOOLEAN, // A boolean command line argument
            STRING,  // A string command line argument
            INTEGER, // An integer command line argument
            DOUBLE   // A double command line argument
        };

        std::vector<CliArgumentBase_uptr> m_required_args; // Command line arguments
        std::vector<CliArgumentBase_uptr> m_optional_args; // Optional command line arguments
        std::unordered_map<std::string, std::pair<int, CliArgumentBase*>> m_arg_map; // Maps command line name to their type

        std::string m_name; // The name of the executable

        void prettify() const;
        std::string combinePatterns() const;
        void handleMatchingGroups( const std::smatch& match );
        
        void continueIfExists(const std::string &name) const;
        bool checkArgumentType(const std::string &name, int type) const;
        void continueIfNoErrors(const std::string &name, int type) const;

        template <typename TArgClass, typename TValue, int Type, typename = std::enable_if_t<std::is_base_of_v<TemplateCliArgument<TValue>, TArgClass>>>
        void addTypedArgument(const ParserArgument_t& arg, TValue default_value);

        template <typename TArgClass, typename TValue, int Type, typename = std::enable_if_t<std::is_base_of_v<TemplateCliArgument<TValue>, TArgClass>>>
        TValue getArgumentValue(const std::string& arg_name) const;

    public:
        ArgumentParser(const char *name) : m_name(name) {};
        ArgumentParser(const std::string& name) : m_name(name) {};
        ~ArgumentParser();
        
        ArgumentParser(const ArgumentParser& other) = delete; // Delete the copy constructor
        ArgumentParser& operator=(const ArgumentParser& other) = delete; // Delete also the operator =

        const std::string& getProgramName() const;
        void printUsage() const;
        void parse( int argc, const char** argv );
        void clean();

        template <typename TValue>
        void addArgument( const ParserArgument_t &arg_params, TValue default_value );

        template <typename TValue>
        void addArgument( const ParserArgument_t &arg_params );

        template <typename TValue>
        TValue getValue(const std::string& arg_name) const;
    };

    template <typename TArgClass, typename TValue, int Type, typename>
    inline void ArgumentParser::addTypedArgument(const ParserArgument_t &arg_params, TValue default_value)
    {
        // First we need to check if the input type exists
        if ( Type != BOOLEAN && Type != INTEGER && Type != DOUBLE && Type != STRING )
        {
            throw std::invalid_argument( "Invalid input Type value." );
        }

        auto arg = std::make_unique<TArgClass>(arg_params.name, arg_params.short_name);
        arg->setRequired( arg_params.required );
        arg->setDescription( arg_params.help_str );
        arg->setDefaultValue( default_value );

        CliArgumentBase* ptr = arg.get();

        if ( arg_params.required ) {
            m_required_args.push_back(std::move(arg));
        } else {
            m_optional_args.push_back(std::move(arg));
        }
        
        m_arg_map.insert( { arg_params.name, { Type, ptr } } );
    }

    template <typename TArgClass, typename TValue, int Type, typename>
    inline TValue ArgumentParser::getArgumentValue(const std::string &arg_name) const
    {
        // Check that there are no errors then continue
        continueIfNoErrors(arg_name, Type);

        // Take the corresponding argument
        CliArgumentBase* arg = m_arg_map.at(arg_name).second;
        return static_cast<TArgClass *>(arg)->getValue();
    }

    template <typename TValue>
    inline void ArgumentParser::addArgument( const ParserArgument_t &arg_params, TValue default_value )
    {
        if constexpr ( std::is_same_v<TValue, bool> ) {
            addTypedArgument<BooleanArgument, TValue, BOOLEAN>(arg_params, default_value);
        } else if constexpr ( std::is_same_v<TValue, std::string> ) {
            addTypedArgument<StringArgument, TValue, STRING>(arg_params, default_value);
        } else if constexpr ( std::is_same_v<TValue, int> ) {
            addTypedArgument<IntegerArgument, TValue, INTEGER>(arg_params, default_value);
        } else if constexpr ( std::is_same_v<TValue, double> ) {
            addTypedArgument<DoubleArgument, TValue, DOUBLE>(arg_params, default_value);
        } else {
            throw std::invalid_argument( "Type parameter does not match int, double, string or bool valid types." );
        }
    }

    template <typename TValue>
    inline void ArgumentParser::addArgument(const ParserArgument_t &arg_params)
    {
        addArgument<TValue>(arg_params, TValue());
    }

    template <typename TValue>
    inline TValue ArgumentParser::getValue(const std::string &arg_name) const
    {
        if constexpr ( std::is_same_v<TValue, bool> ) {
            return getArgumentValue<BooleanArgument, TValue, BOOLEAN>(arg_name);
        } else if constexpr ( std::is_same_v<TValue, std::string> ) {
            return getArgumentValue<StringArgument, TValue, STRING>(arg_name);
        } else if constexpr ( std::is_same_v<TValue, int> ) {
            return getArgumentValue<IntegerArgument, TValue, INTEGER>(arg_name);
        } else if constexpr ( std::is_same_v<TValue, double> ) {
            return getArgumentValue<DoubleArgument, TValue, DOUBLE>(arg_name);
        }

        throw std::invalid_argument( "Type parameter does not match int, double, string or bool valid types." );
        return TValue{};
    }
}
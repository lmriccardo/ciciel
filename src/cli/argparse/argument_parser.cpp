#include "argument_parser.hpp"

using namespace ccl::cli::argparse;

void ArgumentParser::prettify() const
{
    std::stringstream required_s, optional_s;
    for (const auto &option : m_required_args)
    {
        required_s << std::left << "    " << std::setw(10);
        required_s << option->getName();
        required_s << option->getDescription();
        required_s << std::endl;
    }

    for (const auto &option : m_optional_args)
    {
        optional_s << std::left << "    " << std::setw(6);
        optional_s << ((option->hasShortName()) ? "-" + option->getShortName() + "," : " ");
        optional_s << std::setw(20) << "--" + option->getName()
                   << option->getDescription() << std::endl;
    }

    if (m_optional_args.size() > 0)
    {
        std::cout << "These are the OPTIONS parameter" << std::endl;
        std::cout << optional_s.str();
    }

    std::cout << std::endl;

    if (m_required_args.size() > 0)
    {
        std::cout << "These are the required arguments" << std::endl;
        std::cout << required_s.str();
    }
}

std::string ArgumentParser::combinePatterns() const
{
    std::string pattern = "^"; // Initialize the output pattern

    // First put all the optional arguments
    for (const auto &option : m_optional_args)
    {
        pattern += "\\s*" + option->getPatternMatch();
    }

    // Then all the required arguments
    for (const auto &option : m_required_args)
    {
        pattern += "\\s+" + option->getPatternMatch();
    }

    pattern += "$";
    
    return pattern;
}

void ArgumentParser::handleMatchingGroups(const std::smatch &match)
{
    // Print captured groups
    CliArgumentBase *option;
    size_t opt_argument_idx = 0, pos_argument_idx = 0, curridx = 1;
    int nofargs, valueidx;

    while ( curridx < match.size() )
    {
        // Take the correct command line optiona or position argument
        if ( opt_argument_idx < m_optional_args.size() ) {
            option = m_optional_args[opt_argument_idx++].get();
        } else {
            option = m_required_args[pos_argument_idx++].get();
        }

        // Take the argument type to understand how many arguments it requires
        auto arg = m_arg_map[option->getName()];
        int type = arg.first;
        nofargs = ( type != static_cast<int>(BOOLEAN) && !option->isRequired() ) ? 2 : 1;

        // If there is no match for the current argument, continue
        if ( !match[curridx].matched )
        {
            curridx += nofargs;
            continue;
        }

        // If the type is boolean than set the correct value
        if ( type == static_cast<int>(BOOLEAN) )
        {
            ((BooleanArgument *)option)->setValue(); // We know that it is boolean
        }
        else
        {
            valueidx = ( nofargs > 1 ) ? curridx + 1 : curridx;
            option->setValue(match[valueidx].str());
        }

        curridx += nofargs;
    }
}

void ArgumentParser::continueIfExists(const std::string &name) const
{
    // Check if the parameter name is in the argument map
    auto pos = m_arg_map.find(name);
    if (pos == m_arg_map.end())
    {
        printf("Argument %s does not exists.\n", name.c_str());
        throw std::runtime_error("Error");
    }
}

bool ArgumentParser::checkArgumentType(const std::string &name, int type) const
{
    return m_arg_map.at(name).first == type;
}

void ArgumentParser::continueIfNoErrors(const std::string &name, int type) const
{
    continueIfExists(name); // Check if the input parameter exists

    // Check that the given argument matches the given type
    if (!checkArgumentType(name, type))
    {
        printf("Input argument %s does not match input type\n", name.c_str());
        throw std::runtime_error("Error");
    }
}

ArgumentParser::~ArgumentParser()
{
}

const std::string &ArgumentParser::getProgramName() const
{
    return m_name;
}

void ArgumentParser::printUsage() const
{
    printf("Usage: %s ", getProgramName().c_str()); // First we need to print the program name

    // Then we need to check if there are parameters
    if (m_optional_args.size() + m_required_args.size() < 1)
    {
        printf("\n");
        return;
    }

    // Check for any required and optional parameters
    if (m_optional_args.size() > 0) printf("[OPTIONS]...");
    if (m_required_args.size() > 0)
    {
        for (const auto &option : m_required_args)
        {
            if (option->isRequired())
            {
                printf(" %s", option->getArgumentName_upper().c_str());
            }
        }
    }

    printf("\n\n");
    prettify();
}

void ArgumentParser::parse(int argc, const char **argv)
{
    if (argc < 2)
    {
        printUsage();
        exit(EXIT_FAILURE);
    }

    // First we need to assemble the entire string using input argv
    std::string input = " ";
    for (int index = 1; index < argc; index++)
    {
        input += std::string(argv[index]);
        if (index < argc - 1)
            input += " ";
    }

    std::string pattern = combinePatterns(); // Take the pattern string
    std::regex cmdline_r(pattern);           // Create the regex
    std::smatch match;                       // Create the match results

    if (std::regex_match(input, match, cmdline_r))
    {
        handleMatchingGroups(match);
    }
    else
    {
        std::cerr << "Input arguments bad formatting" << std::endl;
        printUsage();
        throw std::runtime_error("");
    }
}

void ArgumentParser::clean()
{
    for ( auto& arg_pair : m_arg_map )
    {
        arg_pair.second.second->clean();
    }
}